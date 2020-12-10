#include <phpcpp.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include<sstream>


#include "mysql_connection.h"
#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/prepared_statement.h>


using namespace std;
sql::Connection *server_con,*reporting_con,*con;
sql::Driver *driver;
sql::Statement *stmt;
sql::ResultSet *res;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define ServerDB "PXCloud_Server"
#define ReportingDB "PXCloud_Reporting"


ifstream fp;
char buf[80];
int expiry=20;

string expiryDate()
	{
	time_t now=time(NULL);	
	tm* timeinfo;
	char buffer[80];
	time(&now);
	timeinfo=localtime(&now);
	timeinfo->tm_mday+=expiry;
	strftime(buffer,80,"%Y-%m-%d ",timeinfo);		
	string datetime(buffer);
	return datetime;	
	}
	
string currentDateTime()
	{
	time_t now = time(0);
	struct tm tstruct;
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	return buf;
	}

string currentDate()
	{
	time_t now = time(0);
	struct tm tstruct;                                   
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return buf;
	}

void writeLog(string function,string txt)
	{
	cout<<txt<<endl;
	std::ofstream fp;
	string  name="/var/log/parcx/Services/ApplicationLogs/Parcx-Walet-Phpcpp-"+currentDate()+".log";    
	fp.open(name.c_str(),ios::app);
	fp<<currentDateTime()<<"\t"<<function<<": "<<txt<<endl;
	fp.close();
	}
	
void writeException(string function,string txt)
	{	
	writeLog(function,txt);
	ofstream fp;
	string  name="/var/log/parcx/Services/ExceptionLogs/Parcx-Walet-Phpcpp-"+currentDate()+".log";
	fp.open(name.c_str(),ios::app);                                    
	fp<<currentDateTime()<<"\t"<<function<<endl<<txt<<endl<<endl;
	fp.close();
	}

sql::Connection* connectReportingDB()
	{
    try
		{               
        driver = get_driver_instance();
        con = driver->connect(DBServer, DBUsername, DBPassword);        
        con->setSchema(ReportingDB);
		}
    catch(const std::exception& e)
		{        
        writeException("connectReportingDB",e.what());
		}
    return con;
	}

sql::Connection* connectServerDB()
	{
    try
		{        
        driver = get_driver_instance();
        con = driver->connect(DBServer, DBUsername, DBPassword);        
        con->setSchema(ServerDB);
		}
    catch(const std::exception& e)
		{        
        writeException("connectServerDB",e.what());
		}
    return con;
	}

bool isPresent(string data,string find)
	{	
	bool match=false;
	stringstream ss(data);
	string word;
	while(getline(ss,word,','))
		{
		if(find==word)
			{
			match=true;
			break;
			}
		}
	return match;
	}
	
Php::Value check_whitelist(string plate_number,string parking_zone,string carpark,string facility)
	{	
	Php::Value response;
	response["whitelist_present"]="false";	
	response["access_allowed"]="false";	
	try
		{		
		string sql="select * from access_whitelist where status=1 AND plate_number='"+plate_number+"'";
        stmt = server_con->createStatement();
        res=stmt->executeQuery(sql);
		if(res->next())
			{
			response["whitelist_present"]="true";	
			writeLog("check_whitelist","Present in access whitelist");	
			response["user_id"]=string(res->getString("wallet_user_id"));
			response["wallet_payment"]=string(res->getString("wallet_payment"));
			response["season_card"]=string(res->getString("season_card"));			
			
			string zone=res->getString("access_zones");
			string expiry_date=res->getString("expiry_date");
			string start_date=res->getString("start_date");
			response["expiry_date"]=expiry_date;
			
			if(zone.find(parking_zone)==string::npos)
				response["result"]="Invalid parking zone";
			else if(expiry_date.compare(currentDate())<0)
				response["result"]="Access expired";
			else if(start_date.compare(currentDate())>0)
				response["result"]="Access not started";
			else if(!isPresent(res->getString("carpark_number"),carpark))				
				response["result"]="Invalid carpark";
			else if(!isPresent(res->getString("facility_numbers"),facility))
				response["result"]="Invalid facility";
			else							
				{
				response["access_allowed"]="true";		
				response["result"]= "Allow Access";					
				}
							
			}
		else
			{			
			response["result"]="Not in access whitelist";
			}
		string result=response["result"];	
		writeLog("updateWaletTransaction","whitelist:"+result); 					
		}	
	catch(const std::exception& e)
		{        
        writeException("check_whitelist",e.what());        
		}
	return response;
	} //End of check_whitelist
	
Php::Value updateWaletTransaction(Php::Parameters &params)
	{
    Php::Value response;    
    try
		{        
        
        string plate_number=params[0];
        string ticket_id=params[1];
		int amount=params[2];
		string device_number=params[3];
		string device_name=params[4];
		string carpark_number=params[5];
		string carpark_name=params[6];
		string facility_number=params[7];
		string facility_name=params[8];
		int balance=0;
		response["wallet_present"]="false";
		response["season_card"]="false";
		response["wallet_payment"]="false";
		
		writeLog("updateWaletTransaction","plate_number: "+plate_number+"\tTicket : "+ticket_id+"'\tamount:"+to_string(amount));	
		writeLog("updateWaletTransaction","device_name: "+device_name+"\tcarpark_name : "+carpark_name+"\tfacility_name : "+facility_name);			
		
		server_con= connectServerDB();
		reporting_con= connectReportingDB();
		Php::Value whitelist=check_whitelist(plate_number,"",carpark_number,facility_number);		
		string status=whitelist["whitelist_present"];
		string result=whitelist["result"];
		response["result"]=result;
		
		writeLog("updateWaletTransaction","Whitelist_present:"+status);
		if(status=="true")
			{
			Php::Value access_allowed=whitelist["access_allowed"];	
			if(access_allowed=="true")
				{
				writeLog("updateWaletTransaction","whitelict check success");	
				string season_card=whitelist["season_card"];	
				string wallet_payment=whitelist["wallet_payment"];
				string user_id=whitelist["user_id"];
				writeLog("updateWaletTransaction","user_id: "+user_id+"\tseason_card:"+season_card+"\twallet_payment:"+wallet_payment);
				if(season_card=="1")
					{
					writeLog("updateWaletTransaction","Season card");	
					response["result"]="Allow Access";
					response["season_card"]="true";
					}
				else if(wallet_payment=="1")
					{
					response["wallet_payment"]="true";						
					string sql="select * from parking_ewallet where user_id='"+user_id+"'";        					
					stmt = server_con->createStatement();
					res=stmt->executeQuery(sql);
					if(res->next())
						{
						writeLog("updateWaletTransaction","Walet present");	
						response["wallet_present"]="true";	
						if(res->getInt("wallet_enabled")==1)
							{
							string 	expiry_date=res->getString("expiry_date");
							if(expiry_date.compare(currentDate())<0)
								response["result"]="Wallet expired";
							else
								{
								balance=res->getInt("wallet_balance");	
								if(balance>0)
									{
									string name=res->getString("user_name");									
									sql="insert into ewallet_transactions(transaction_type,transaction_description,user_id,user_name,plate_number,ticket_id,amount,device_number,device_name,carpark_number,carpark_name,facility_number,facility_name,date_time)values(100,'Parking Fee','"+user_id+"','"+name+"','"+plate_number+"','"+ticket_id+"','"+to_string(amount)+"','"+device_number+"','"+device_name+"','"+carpark_number+"','"+carpark_name+"','"+facility_number+"','"+facility_name+"',CURRENT_TIMESTAMP)";
									stmt = reporting_con->createStatement();
									int result=stmt->executeUpdate(sql);
									if(result==1)
										{
										sql="update parking_ewallet set last_deducted_amount='"+to_string(amount)+"',last_payment_date_time=CURRENT_TIMESTAMP,wallet_balance=wallet_balance-"+to_string(amount)+" where user_id="+user_id;	
										stmt = server_con->createStatement();
										result=stmt->executeUpdate(sql);
										response["result"]="Allow Access";
										balance=balance-amount;
										response["wallet_balance"]=balance;
										}
									}
								else
									{
									response["result"]="No wallet balance";
									response["wallet_balance"]=balance;
									}
								}
							}
						else
							{
							response["result"]="Walet disabled";
							}
						}
					else
						response["wallet_present"]="false";	
					}
				else
					{
					response["result"]="Wallet payment not active";	
					}
				}
			}
			
        
        
		stmt->close();
        server_con->close();
		reporting_con->close();
        delete stmt;
        delete server_con;
		delete reporting_con;
        return response;

		}
    catch(const std::exception& e)
		{       
        writeException("updateWaletTransaction",e.what());
        return response;
		}
	}
		
Php::Value getWalletDetails(Php::Parameters &params)
	{
    Php::Value response;    
    try
		{                
        string user_id=params[0];        	
		writeLog("getWalletDetails","user_id: "+user_id);				
		server_con= connectServerDB();		
        string sql="select * from parking_ewallet where user_id='"+user_id+"'";        
        stmt = server_con->createStatement();
        res=stmt->executeQuery(sql);
		if(res->next())
			{			
			response["wallet_present"]="true";	
			response["balance_amount"]=string(res->getString("wallet_balance"));
			response["expiry_date"]=string(res->getString("expiry_date"));
			response["wallet_enabled"]=string(res->getString("wallet_enabled"))	;		
			}
		else
			response["wallet_present"]="false";
        
		stmt->close();
        server_con->close();		
        delete stmt;
        delete server_con;		
        return response;

		}
    catch(const std::exception& e)
		{       
        writeException("getWalletDetails",e.what());
        return response;
		}
	}
	
Php::Value getWalletTransactions(Php::Parameters &params)
	{
    Php::Value row; 
	Php::Value response;
    try
		{        
        
        string user_id=params[0];
        		
		writeLog("getWalletTransactions","user_id: "+user_id);		
		
		reporting_con= connectReportingDB();
        string sql="select * from ewallet_transactions where user_id='"+user_id+"'";        
        stmt = reporting_con->createStatement();
        res=stmt->executeQuery(sql);
		int i=0;
		string location="";
		while(res->next())
			{					
			//row["transactionType"]=string(res->getString("date_time"))+" "+string(res->getString("transaction_description"));
			//row["transactionLocation"]=string(res->getString("carpark_name"))+" "+string(res->getString("facility_name"));
			//row["price"]=string(res->getString("amount"));
			//row["status"]="Completed";
			if(res->getInt("transaction_type")==105)
				location="Online";
			else
				location=res->getString("carpark_name")+" "+res->getString("facility_name");
			row["uniqueId"]=res->getInt("id");	
			row["data"]=string(res->getString("date_time"))+" - "+string(res->getString("transaction_description"))+"|Location: "+location+"|Price: AED "+string(res->getString("amount"))+"|Status: Completed";
			response[i]=row;
			i++;
			}		
        
		stmt->close();
        reporting_con->close();		
        delete stmt;
        delete reporting_con;		
        return response;

		}
    catch(const std::exception& e)
		{       
        writeException("getWalletTransactions",e.what());
        return response;
		}
	}	

Php::Value updateWalletPayment(Php::Parameters &params)
	{
    Php::Value response; 
	string sql;	
    try
		{        
        
        string user_id=params[0];		
        string amount=params[1];
		int result=0;		
		string name="";
		
		
		writeLog("updateWalletPayment","user_id: "+user_id+"\tamount : "+amount);		
		
		server_con= connectServerDB();
		reporting_con= connectReportingDB();
        sql="select * from parking_ewallet where user_id='"+user_id+"'";        
        stmt = server_con->createStatement();
        res=stmt->executeQuery(sql);
		if(res->next())
			{			
			string balance=res->getString("wallet_balance");
			response["expiry_date"]=string(res->getString("expiry_date"));
			response["wallet_enabled"]=res->getInt("wallet_enabled");			
			name=res->getString("user_name");						
			sql="insert into ewallet_transactions(transaction_type,transaction_description,user_id,user_name,amount,date_time)values('105','Top up','"+user_id+"','"+name+"',"+amount+",CURRENT_TIMESTAMP)";
			stmt = reporting_con->createStatement();
			result=stmt->executeUpdate(sql);
			if(result==1)
				{
				sql="update parking_ewallet set last_top_up_amount='"+amount+"',last_top_up_date_time=CURRENT_TIMESTAMP,wallet_balance=wallet_balance+"+amount+",expiry_date=DATE_ADD(NOW(), INTERVAL "+to_string(expiry)+" DAY) where user_id="+user_id;	
				stmt = server_con->createStatement();
				result=stmt->executeUpdate(sql);				
				response["wallet_balance"]=stoi(balance)+stoi(amount);
				}					
			}		
		else
			{
			sql="select username from system_users where user_id='"+user_id+"'";                
			res=stmt->executeQuery(sql);
			if(res->next())
					name=res->getString("username");
			sql="insert into parking_ewallet(user_id,last_top_up_amount,last_top_up_date_time,wallet_balance,expiry_date,wallet_enabled)values('"+user_id+"',"+amount+",CURRENT_TIMESTAMP,"+amount+",DATE_ADD(NOW(), INTERVAL "+to_string(expiry)+" DAY),1)";
			stmt = server_con->createStatement();
			result=stmt->executeUpdate(sql);
			response["wallet_balance"]=stoi(amount)	;	
			response["expiry_date"]=expiryDate();
			response["wallet_enabled"]=1;
			
			sql="insert into ewallet_transactions(transaction_type,transaction_description,user_id,user_name,amount,date_time)values('105','Top up','"+user_id+"','"+name+"',"+amount+",CURRENT_TIMESTAMP)";
			stmt = reporting_con->createStatement();
			result=stmt->executeUpdate(sql);
			}
        
		stmt->close();
        server_con->close();
		reporting_con->close();
        delete stmt;
        delete server_con;
		delete reporting_con;
        return response;

		}
    catch(const std::exception& e)
		{       
        writeException("updateWaletTransaction",e.what());
		writeLog("sql",sql);
        return response;
		}
	}
		
extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_Walet", "1.0");        
		extension.add<updateWaletTransaction>("updateWaletTransaction");  
		extension.add<getWalletDetails>("getWalletDetails");  		
		extension.add<getWalletTransactions>("getWalletTransactions");  		
		extension.add<updateWalletPayment>("updateWalletPayment");  		
        return extension;
		}
	}
