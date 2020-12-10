//CURL-OPENSSL Required to access https  : sudo apt-get install libcurl4-openssl-dev
//JSON.h install :   sudo apt-get install libjsoncpp-dev
//make -B
//sudo make install
//service apache2 restart

//Version=201911210909
#include <phpcpp.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include <cstring>
#include <curl/curl.h>
#include <fstream>
#include <ctime>
#include <time.h>
#include <syslog.h>
#include<sys/stat.h>
#include<sstream>
#include<string.h>
#include<unistd.h>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

#include<jsoncpp/json/json.h>
#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define DATABASE "parcx_reporting"
#define DATABASE_SERVER "parcx_server"
#define DATABASE_DASHBOARD "parcx_dashboard"
using namespace std;

sql::Connection *conn;
sql::Driver *driver;
std::string query;
bool cloud=false;
bool flagSet=false;
int plate_review_confidence_rate=0;

string currentTime()
    {
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%H:%M:%S",timeinfo);		
    string datetime(buffer);
    return datetime;
    }
string currentDate()
    {
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%Y-%m-%d",timeinfo);		
    string date(buffer);
    return date;
    }
void WriteToLog(string msg)
    {
    cout<<msg<<endl;
    ofstream myfile;
    myfile.open("/opt/parcx/Logs/Services/ExceptionLogs/PX-DeviceUpload-"+currentDate()+".log",fstream::app);
    myfile<<currentTime()+"   "<<msg<<endl;
    myfile.close();
    }

sql::Connection* DBConnection(string db)
    {
    driver = get_driver_instance();
    conn = driver->connect(DBServer,DBUsername,DBPassword);  
    conn->setSchema(db);
    return conn;
    }

void setCloudFlag()
    {
    try
        {
        if(flagSet==false)
            {
            sql::Statement *stmt;
            sql::ResultSet *res;
            conn = DBConnection(DATABASE_SERVER);
            stmt = conn->createStatement(); 
            res=stmt->executeQuery("select setting_name,setting_value from system_settings where setting_name in ('cloud_enabled','plate_review_confidence_rate')");
            if(res->next())
                { 
                if(res->getString("setting_name")=="cloud_enabled")                       
                    {
                    if(res->getInt("setting_value")==1)                    
                        cloud=true;                    
                    }
                else
                    {
                    plate_review_confidence_rate=res->getInt("setting_value");
                    }
                
                }
            // if(cloud)    
            //     WriteToLog("setCloudFlag :true");
            // else
            //     WriteToLog("setCloudFlag :false");
            flagSet=true;
            delete stmt;
            delete conn;
            }    
       
        }
    catch(const std::exception& e)
        {
        WriteToLog("****setCloudFlag*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;         
        }
        
    }

Php::Value UploadParkingMovements(Php::Value request)
{
    setCloudFlag();
    sql::Statement *stmt;
    Php::Value result;   
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();         
        for(int i=0;i<request.size();i++)
        {
			string id = request[i]["id"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string carpark_number = request[i]["carpark_number"];           
            string carpark_name = request[i]["carpark_name"];           
            string facility_number = request[i]["facility_number"];             

            string date_time = request[i]["date_time"];            
            string movement_type = request[i]["movement_type"];
            string ticket_id = request[i]["ticket_id"];              
            string plate_number = request[i]["plate_number"];  
            string chip_utid = request[i]["chip_utid"];  
            string operation_mode = request[i]["operation_mode"]; 
			string entry_grace_period = request[i]["entry_grace_period"];	
            string plate_captured_id = request[i]["plate_captured_id"];	
            if(plate_captured_id=="")
                plate_captured_id="NULL";
            if(movement_type!="1")
                movement_type="2";
			query="INSERT into parking_movements(device_name,device_number,movement_type,ticket_id,carpark_number,facility_number,date_time,plate_number,chip_utid,operation_mode,plate_captured_id,carpark_name) VALUES('" + device_name + "'," +device_number + ",'" + movement_type + "','" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','"+chip_utid+"','"+operation_mode+"',"+plate_captured_id+",'"+carpark_name+"')";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;               
            }
			 if(stoi(movement_type) == 1)                
				{
					query="INSERT into open_transactions(device_name,device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,operation_mode,entry_grace_period,entry_type,plate_captured_id,carpark_name,movement_type) VALUES('" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','"+chip_utid+"','"+operation_mode+"','"+entry_grace_period+"',1,"+plate_captured_id+",'"+carpark_name+"',1)";
					result = stmt->executeUpdate(query);
                    // query="INSERT into track_ticket(entry_device_name,entry_device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,entry_grace_period,entry_type,plate_captured_id,carpark_name) VALUES('" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','"+chip_utid+"','"+entry_grace_period+"',1,"+plate_captured_id+",'"+carpark_name+"')";
					// result = stmt->executeUpdate(query);
                    if(plate_captured_id!="NULL")
                        {
                                             
                        query="select lower(plate_number) as plate_number,confidence_rate from plates_captured where id="+plate_captured_id;    
                        sql::ResultSet *res;
                        res=stmt->executeQuery(query);
                        if(res->next())
                            {
                            plate_number=res->getString("plate_number");                               
                            if((plate_number.find("no plate")!= std::string::npos) || (plate_number.find("noplate")!= std::string::npos))
                                {
                                query="insert into parcx_dashboard.ticket_check(date_time,device_number,device_name,carpark_number,carpark_name,device_type,ticket_id,plate_number,result,facility_number,result_description)values(CURRENT_TIMESTAMP,"+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',1,'"+ticket_id+"','"+plate_number+"','entry_with_no_plate','"+facility_number+"','ANPR entry with no plate')";
                                stmt->executeUpdate(query);
                                }

                            else if(res->getInt("confidence_rate")<=plate_review_confidence_rate)    
                                {                                    
                                query="insert into parcx_dashboard.ticket_check(date_time,device_number,device_name,carpark_number,carpark_name,device_type,ticket_id,plate_number,result,facility_number,result_description)values(CURRENT_TIMESTAMP,"+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',1,'"+ticket_id+"','"+plate_number+"','entry_with_less_confidence_rate','"+facility_number+"','ANPR entry with less confidence rate')";
                                stmt->executeUpdate(query);                                
                                }
                            
                             
                            
                            }
                            
                        }

				}
            else if(stoi(movement_type) == 2)                
				{
                    if(cloud==false)
                        {
                        query="Delete from open_transactions where ticket_id = '"+ticket_id+"' and entry_type=1";
                        }
                    else
                        {
                        query="update open_transactions set movement_type=2,cloud_upload_status=0 where ticket_id = '"+ticket_id+"' and entry_type=1";
                        }
                    
                        result = stmt->executeUpdate(query);

                    // query="update track_ticket set exit_date_time='"+date_time+"',exit_device_number='"+device_number+"',exit_device_name='"+device_name+"' where ticket_id = '"+ticket_id+"' and entry_type=1 ORDER BY id DESC LIMIT 1";
                    // result = stmt->executeUpdate(query);
				}
			
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadParkingMovements*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}

Php::Value UploadParkingAccessMovements(Php::Value request)
{
    setCloudFlag();   
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();   
        
        for(int i=0;i<request.size();i++)
        {
            string id = request[i]["id"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string carpark_number = request[i]["carpark_number"];                       
            string facility_number = request[i]["facility_number"];  
            string facility_name = request[i]["facility_name"];  

            string movement_type = request[i]["movement_type"];
            string date_time = request[i]["date_time"];
            string tag = request[i]["tag"];
            string status_message = request[i]["status_message"];
            string tag_serial = request[i]["tag_serial"];           
            string tid = request[i]["tid"];
            string epc = request[i]["epc"];      
            string epc_list = request[i]["epc_list"];
            string epc_count = request[i]["epc_count"];
            string carpark_name = request[i]["carpark_name"];   
            string operation_mode = request[i]["operation_mode"]; 
            string parking_zone = request[i]["parking_zone"];  
            string plate_captured_id = request[i]["plate_captured_id"];	
            string ticket_id = request[i]["ticket_id"];	
            string plate_number = request[i]["plate_number"];	
            if(plate_captured_id=="")
                plate_captured_id="NULL";
            if(movement_type!="1")
                movement_type="2";
			query="INSERT into parking_movements_access(device_name,device_number,movement_type,carpark_number,facility_number,date_time,tag,status_message,tag_serial,tid,epc,epc_list,epc_count,carpark_name,operation_mode,parking_zone,plate_captured_id,ticket_id,plate_number) VALUES('" + device_name + "'," + device_number + "," + movement_type + "," + carpark_number + "," + facility_number + ",'" + date_time + "','" + tag + "','" + status_message + "','" + tag_serial + "','"+tid+"','"+epc+"','"+epc_list+"',"+epc_count+",'"+carpark_name+"','"+operation_mode+"','"+parking_zone+"',"+plate_captured_id+",'"+ticket_id+"','"+plate_number+"')";
            result = stmt->executeUpdate(query);
			
            if(result==1)
            {
                jsonresponse[i] = id;               
            }
            if(status_message=="Access Allowed")
            {
                if(stoi(movement_type) == 1)                
					{	                        
                    query="INSERT into open_transactions(carpark_name,movement_type,entry_type,device_name,device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,operation_mode,entry_grace_period,plate_captured_id) VALUES('"+carpark_name+"',1,2,'" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','','"+operation_mode+"',CURRENT_TIMESTAMP,"+plate_captured_id+")";                        
					result = stmt->executeUpdate(query);                
					}
            else if(stoi(movement_type) == 2)                
				{
                    if(cloud==false)                                            
                        query="Delete from open_transactions where ticket_id = '"+ticket_id+"' and entry_type=2"; 
                    else
                        query="update open_transactions set movement_type=2,cloud_upload_status=0 where ticket_id = '"+ticket_id+"' and entry_type=2";
                                                               
                    result = stmt->executeUpdate(query);
				}			
            }                            
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);           
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
         WriteToLog("****UploadParkingAccessMovements*****");
         WriteToLog(e.what());
		 WriteToLog(query);
		 delete conn;  
         return e.what();
    }
    
    return result;
       
}

Php::Value UploadWatchdogDeviceAlarms(Php::Value request)
{    
   sql::Statement *stmt;
    Php::Value result;   
    Json::Value jsonresponse;       
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement(); 
        int code;      
        for(int i=0;i<request.size();i++)
        {
            string id = request[i]["id"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string carpark_number = request[i]["carpark_number"];           
            string carpark_name = request[i]["carpark_name"];           
            string facility_number = request[i]["facility_number"];
            string alarm_severity = request[i]["alarm_severity"];
            string alarm_code = request[i]["alarm_code"];      
            string alarm_description = request[i]["alarm_description"];
            string date_time = request[i]["alarm_date_time"];
            query= "INSERT into watchdog_device_alarms(carpark_number,carpark_name,facility_number,device_name,device_number,alarm_severity,alarm_code,alarm_description,alarm_date_time) VALUES("+carpark_number+",'"+carpark_name+"',"+facility_number+",'" + device_name + "'," + device_number + "," + alarm_severity + "," + alarm_code + ",'" + alarm_description + "','" + date_time + "')";     
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;               
            }
        code=request[i]["alarm_code"];
        string device_type=request[i]["device_type"];
        if(code%2==1)//alarm raised
            {
            query="insert into parcx_dashboard.ticket_check(date_time,device_number,device_name,carpark_number,carpark_name,device_type,ticket_id,plate_number,result,facility_number,result_description)values(CURRENT_TIMESTAMP,"+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',"+device_type+",'','','alarm_"+alarm_code+"','"+facility_number+"','"+alarm_description+"')";
            stmt->executeUpdate(query);    
            }
        else
            {       
            code=code-1;     
            query="delete from parcx_dashboard.ticket_check where result='alarm_"+to_string(code)+"' and device_number="+device_number;
            stmt->executeUpdate(query);             
            }        
        
        Json::FastWriter fw;
        string res = fw.write(jsonresponse);
        result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {        
         WriteToLog("****WatchdogDeviceAlarms*****");
         WriteToLog(e.what());
		 WriteToLog(query);
		 delete conn;  
         return e.what();
    }
    
    return result;
       
}

Php::Value UploadParkingBlacklists(Php::Value request)
{
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();       
        for(int i=0;i<request.size();i++)
        {
            string id = request[i]["id"];
            string ticket_id = request[i]["ticket_id"];
            //string ticket_category = request[i]["ticket_category"];
            string plate_number = request[i]["plate_number"];
            string date_time = request[i]["date_time"];      
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string facility_number = request[i]["facility_number"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];            
            string movement_type = request[i]["movement_type"];
            string chip_utid = request[i]["chip_utid"];      
            string operation_mode = request[i]["operation_mode"];
            string blacklisting_reason = request[i]["blacklisting_reason"];
            string blacklisting_description = request[i]["blacklisting_description"];
                     
			query="INSERT into parking_blacklist(ticket_id,plate_number,date_time,device_number,device_name,facility_number,carpark_number,carpark_name,movement_type,chip_utid,operation_mode,blacklisting_reason,blacklisting_description)VALUES('"+ticket_id+"','"+plate_number+"','"+date_time+"',"+device_number+",'"+device_name+"','"+facility_number+"',"+carpark_number+",'"+carpark_name+"',"+movement_type+",'"+chip_utid+"','"+operation_mode+"','"+blacklisting_reason+"','"+blacklisting_description+"')";
            result = stmt->executeUpdate(query);
			
			
            if(result==1)
            {
                jsonresponse[i] = id;                
            }
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        WriteToLog("****UploadParkingBlacklist*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}

Php::Value UploadParkingValidations(Php::Value request)
{    
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {          
            string carpark_number = request[i]["carpark_number"];           
            string carpark_name = request[i]["carpark_name"];           
            string facility_number = request[i]["facility_number"];              

            string id = request[i]["id"];
            string ticket_id = request[i]["ticket_id"];
            string validation_value = request[i]["validation_value"];
            string validation_type = request[i]["validation_type"];
            string validation_provider = request[i]["validation_provider"];      
            string date_time = request[i]["date_time"];
			query="INSERT into parking_validation(carpark_number,carpark_name,facility_number,ticket_id,validation_value,validation_type,validator_id,date_time)VALUES("+carpark_number+",'"+carpark_name+"',"+facility_number+","+ticket_id+","+validation_value+",'"+validation_type+"','"+validation_provider+"','"+date_time+"')";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;               
            }
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadParkingValidation*****");
         WriteToLog(e.what());
		 WriteToLog(query);
		 delete conn;  
         return e.what();
    }
    
    return result;       
}

Php::Value UploadPayments(Php::Value request)
{    
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;   
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];      
            string id = request[i]["id"];
			string operator_id = request[i]["operator_id"];
			string operator_name = request[i]["operator_name"];
			string shift_id = request[i]["shift_id"];
			string parking_rate_label = request[i]["parking_rate_label"];
			string parking_rate_name = request[i]["parking_rate_name"];
			string entry_grace_period = request[i]["entry_grace_period"];
			string exit_grace_period = request[i]["exit_grace_period"];
			string vat_type = request[i]["vat_type"];
			string vat_percentage = request[i]["vat_percentage"];
			string ticket_id = request[i]["ticket_id"];
			
			string entry_date_time = request[i]["entry_date_time"];
			if(entry_date_time=="")
				entry_date_time="NULL";
			else
				entry_date_time="'"+entry_date_time+"'";
			
			string payment_date_time = request[i]["payment_date_time"];
			
			string max_exit_date_time = request[i]["max_exit_date_time"];
			if(max_exit_date_time=="")
				max_exit_date_time="NULL";
			else
				max_exit_date_time="'"+max_exit_date_time+"'";	
			
			string parking_duration = request[i]["parking_duration"];
			string payment_category = request[i]["payment_category"];
			string parking_fee = request[i]["parking_fee"];
			string vat_amount = request[i]["vat_amount"];
			string lost_fee = request[i]["lost_fee"];
			string admin_fixed_charges = request[i]["admin_fixed_charges"];
			string ticket_replacement_fee = request[i]["ticket_replacement_fee"];
			string discount_amount = request[i]["discount_amount"];
			string gross_amount = request[i]["gross_amount"];
			string amount_received = request[i]["amount_received"];
			string discount_id = request[i]["discount_id"];
            if(discount_id=="")
                discount_id="0";
			string discount_category = request[i]["discount_category"];
			string discount_name = request[i]["discount_name"];
			string coupon_id = request[i]["coupon_id"];
			string coupon_category = request[i]["coupon_category"];
			string coupon_source = request[i]["coupon_source"];
			string payment_type = request[i]["payment_type"];
			string validation_value = request[i]["validation_value"];
			string validation_id = request[i]["validation_id"];
			string balance_returned = request[i]["balance_returned"];
			string credit_note = request[i]["credit_note"];
			string bank_notes = request[i]["bank_notes"];
			string receipt_number = request[i]["receipt_number"];
            string authentication_code = request[i]["authentication_code"];
            string entry_plate_number = request[i]["entry_plate_number"];
            string exit_plate_number = request[i]["exit_plate_number"];
            string central_cashier = request[i]["central_cashier"]; 
            string wallet_points = request[i]["wallet_points"]; 
            if(central_cashier!="1")
                central_cashier="0";                          
			
			query="INSERT into revenue_payments(device_number,device_name,carpark_number,carpark_name,facility_number,operator_id,operator_name,shift_id,parking_rate_label,parking_rate_name,entry_grace_period,exit_grace_period,vat_type,vat_percentage,ticket_id,entry_date_time,payment_date_time,max_exit_date_time,parking_duration,payment_category,parking_fee,vat_amount,lost_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,gross_amount,amount_received,discount_id,discount_category,discount_name,coupon_id,coupon_category,coupon_source,payment_type,validation_value,validation_id,receipt_number,bank_notes,balance_returned,credit_note,authentication_code,entry_plate_number,exit_plate_number,wallet_points)VALUES('"+device_number+"','"+device_name+"',"+carpark_number+",'"+carpark_name+"','"+facility_number+"',"+operator_id+",'"+operator_name+"','"+shift_id+"','"+parking_rate_label+"','"+parking_rate_name+"','"+entry_grace_period+"','"+exit_grace_period+"','"+vat_type+"','"+vat_percentage+"','"+ticket_id+"',"+entry_date_time+",'"+payment_date_time+"',"+max_exit_date_time+",'"+parking_duration+"','"+payment_category+"','"+parking_fee+"','"+vat_amount+"','"+lost_fee+"','"+admin_fixed_charges+"','"+ticket_replacement_fee+"','"+discount_amount+"','"+gross_amount+"','"+amount_received+"','"+discount_id+"','"+discount_category+"','"+discount_name+"','"+coupon_id+"','"+coupon_category+"','"+coupon_source+"','"+payment_type+"','"+validation_value+"','"+validation_id+"','"+receipt_number+"','"+bank_notes+"','"+balance_returned+"','"+credit_note+"','"+authentication_code+"','"+entry_plate_number+"','"+exit_plate_number+"',"+wallet_points+")";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;              
            }
            if(stoi(central_cashier)==1)
                {
                if(stof(lost_fee)>0)
                    {
                    query="INSERT into open_transactions(carpark_name,movement_type,device_name,device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,operation_mode,entry_grace_period,entry_type) VALUES('"+carpark_name+"',1,'" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +payment_date_time + "','','','',"+max_exit_date_time+",1)";
                    result = stmt->executeUpdate(query);
                    }
                query="update open_transactions set last_payment_date_time='"+payment_date_time+"',max_exit_date_time="+max_exit_date_time+",total_amount_paid=total_amount_paid+"+gross_amount+",parking_rate='"+parking_rate_name+"' where ticket_id='"+ticket_id+"'";
                result = stmt->executeUpdate(query);                
                }
            // query="update track_ticket set payment_date_time='"+payment_date_time+"',payment_device_number='"+device_number+"',payment_device_name='"+device_name+"',max_exit_date_time="+max_exit_date_time+",total_amount_paid=total_amount_paid+"+gross_amount+" where ticket_id = '"+ticket_id+"' and entry_type=1 ORDER BY id DESC LIMIT 1";
            // result = stmt->executeUpdate(query);
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        
        WriteToLog("****UploadPayments*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}

Php::Value UpdateCashLevels(Php::Value request,int type)
{
    sql::Statement *stmt;
    sql::ResultSet *res;
    Php::Value result;    
	Json::Value jsonresponse; 	
    try
    {
        conn = DBConnection(DATABASE_DASHBOARD);
        stmt = conn->createStatement();         
        for(int i=0;i<request.size();i++)
        {
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];
            string denomination = request[i]["denomination"];            
            string current_level = request[i]["current_level"];
            string float_level = request[i]["float_level"];      
            string max_threshold = request[i]["max_threshold"];            
             
			if(type==1)//apm_cashbox_levels
				{
                query="select * from apm_cashbox_levels where device_number="+device_number+" and carpark_number="+carpark_number+" and facility_number="+facility_number+" and denomination='"+denomination+"'";
                res=stmt->executeQuery(query);
                if(res->next())
                    {
                    delete res;    
                    query="update apm_cashbox_levels set current_level='"+current_level+"',float_level='"+float_level+"',max_threshold='"+max_threshold+"',last_updated_datetime=CURRENT_TIMESTAMP where device_number="+device_number+" and carpark_number="+carpark_number+" and facility_number="+facility_number+" and denomination='"+denomination+"'";
                    }
                else
                    query="insert into apm_cashbox_levels(device_number,device_name,carpark_number,carpark_name,facility_number,denomination,current_level,float_level,max_threshold)values('"+device_number+"','"+device_name+"','"+carpark_number+"','"+carpark_name+"','"+facility_number+"','"+denomination+"','"+current_level+"','"+float_level+"','"+max_threshold+"')";
                
                }
				
			if(type==2)//apm_recycle_levels
				{
                query="select * from apm_recycle_levels where device_number="+device_number+" and carpark_number="+carpark_number+" and facility_number="+facility_number+" and denomination='"+denomination+"'";
                res=stmt->executeQuery(query);
                if(res->next())
                    {
                    delete res;
                    query="update apm_recycle_levels set current_level='"+current_level+"',float_level='"+float_level+"',max_threshold='"+max_threshold+"',last_updated_datetime=CURRENT_TIMESTAMP where device_number="+device_number+" and carpark_number="+carpark_number+" and facility_number="+facility_number+" and denomination='"+denomination+"'";
                    }
                else
                    query="insert into apm_recycle_levels(device_number,device_name,carpark_number,carpark_name,facility_number,denomination,current_level,float_level,max_threshold)values('"+device_number+"','"+device_name+"','"+carpark_number+"','"+carpark_name+"','"+facility_number+"','"+denomination+"','"+current_level+"','"+float_level+"','"+max_threshold+"')";                
                }
				
            result = stmt->executeUpdate(query);	
			if(result==1)
				{
                jsonresponse[i] = denomination;                
				}
            Json::FastWriter fw;
            string respone = fw.write(jsonresponse);
            result = respone;
			
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        WriteToLog("****UpdateCashLevels*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
	return result;
}

Php::Value UploadApmCashPayout(Php::Value request)
{    
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();       
        for(int i=0;i<request.size();i++)
        {
			string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];

            string id = request[i]["id"];
            string user = request[i]["user"];
            string datetime = request[i]["datetime"];
            string operation = request[i]["operation"];
            string cashlevel_before_payout = request[i]["cashlevel_before_payout"];      
            string cashlevel_after_payout = request[i]["cashlevel_after_payout"];
			string notes_payout = request[i]["notes_payout"];
			string total_payout_amount = request[i]["total_payout_amount"];
										
			query="INSERT into revenue_apm_cash_payout(facility_number,device_number,device_name,carpark_number,carpark_name,user,datetime,operation,cashlevel_before_payout,cashlevel_after_payout,notes_payout,total_payout_amount)VALUES("+facility_number+",'"+device_number+"','"+device_name+"',"+carpark_number+",'"+carpark_name+"','"+user+"','"+datetime+"','"+operation+"','"+cashlevel_before_payout+"','"+cashlevel_after_payout+"','"+notes_payout+"','"+total_payout_amount+"')";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {        
        WriteToLog("****UploadApmCashPayout*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;     
}

Php::Value UploadApmCashRefill(Php::Value request)
{    
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();         
        for(int i=0;i<request.size();i++)
        {
			string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];

            string id = request[i]["id"];
            string user = request[i]["user"];
            string datetime = request[i]["datetime"];            
            string cashlevel_before_refill = request[i]["cashlevel_before_refill"];      
            string cashlevel_after_refill = request[i]["cashlevel_after_refill"];
			string notes_refilled = request[i]["notes_refilled"];
			string total_amount_refilled = request[i]["total_amount_refilled"];
										
			query="INSERT into revenue_apm_cash_refill(facility_number,device_number,device_name,carpark_number,carpark_name,user,datetime,cashlevel_before_refill,cashlevel_after_refill,notes_refilled,total_amount_refilled)VALUES("+facility_number+",'"+device_number+"','"+device_name+"',"+carpark_number+",'"+carpark_name+"','"+user+"','"+datetime+"','"+cashlevel_before_refill+"','"+cashlevel_after_refill+"','"+notes_refilled+"','"+total_amount_refilled+"')";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {        
        WriteToLog("****UploadApmCashRefill*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn; 
        return e.what(); 
    }
    
    return result;
       
}

Php::Value UploadParkingReservationMovements(Php::Value request)
{
    sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
			string id = request[i]["id"];
            string carpark_number = request[i]["carpark_number"];           
            string date_time = request[i]["date_time"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string facility_number = request[i]["facility_number"];
            string carpark_name = request[i]["carpark_name"];

            string movement_type = request[i]["movement_type"];
            string ticket_id = request[i]["ticket_id"];              
            string plate_number = request[i]["plate_number"];  
            string plate_captured_id = request[i]["plate_captured_id"];	  
            if(plate_captured_id=="")
                plate_captured_id="NULL";                      
			query="INSERT into parking_movements_reservation(carpark_name,device_name,device_number,movement_type,ticket_id,carpark_number,facility_number,date_time,plate_number,plate_captured_id) VALUES('"+carpark_name+"','" + device_name + "'," +device_number + ",'" + movement_type + "','" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"',"+plate_captured_id+")";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }
			 
			
			query="update parking_reservation set used_flag="+movement_type+" where ticket_id='"+ticket_id+"' and carpark_number="+carpark_number;
			result = stmt->executeUpdate(query);
			
            if(stoi(movement_type)==1)
                query="INSERT into open_transactions(carpark_name,movement_type,device_name,device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,operation_mode,entry_grace_period,entry_type,plate_captured_id) VALUES('"+carpark_name+"',1,'" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','','',CURRENT_TIMESTAMP,3,"+plate_captured_id+")";
            else
                {
                if(cloud==false)                    
                    query="Delete from open_transactions where ticket_id = '"+ticket_id+"' and entry_type=3";
                else
                    query="update open_transactions set movement_type=2,cloud_upload_status=0 where ticket_id = '"+ticket_id+"' and entry_type=3";
                               
                }
            result = stmt->executeUpdate(query);

            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadParkingReservationMovements*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}

Php::Value UploadWaletTransaction(Php::Value request)
	{
	sql::Statement *stmt;
    Php::Value result;    
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
			string id = request[i]["id"];
            string carpark_number = request[i]["carpark_number"];           
            string carpark_name = request[i]["carpark_name"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string facility_number = request[i]["facility_number"];
			string facility_name = request[i]["facility_name"];
            string amount = request[i]["amount"];
            string ticket_id = request[i]["ticket_id"];              
            string plate_number = request[i]["plate_number"];                          
			query="INSERT into walet_transactions(device_name,device_number,ticket_id,carpark_number,carpark_name,facility_number,facility_name,date_time,plate_number,amount) VALUES('" + device_name + "'," +device_number + ",'" + ticket_id+ "'," + carpark_number + ",'" + carpark_name + "'," +facility_number + ",'" + facility_name + "',CURRENT_TIMESTAMP,'"+plate_number+"',"+amount+")";
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }
			 
			
			query="update walet set balance_amount=balance_amount-"+amount+" where plate_number='"+plate_number+"' order by id desc limit 1";
			
			result = stmt->executeUpdate(query);
			
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
		{       
        WriteToLog("****UploadWaletTransaction*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
		}
    
    return result;
	}
	
Php::Value UploadRevenueShifts(Php::Value request)
{    
    sql::Statement *stmt;
	sql::ResultSet *res;
    Php::Value result;   
    Json::Value jsonresponse;   
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];  
			
            string id = request[i]["id"];
			string operator_id = request[i]["operator_id"];
			string operator_name = request[i]["operator_name"];
			string shift_id = request[i]["shift_id"];			
			string login_time = request[i]["login_time"];
			string logout_time = request[i]["logout_time"];				
			string shift_duration = request[i]["shift_duration"];
			string parking_fees = request[i]["parking_fees"];			
			string vat_amount = request[i]["vat_amount"];			
			string lost_fees = request[i]["lost_fees"];
			string admin_fixed_charges = request[i]["admin_fixed_charges"];
			string ticket_replacement_fee = request[i]["ticket_replacement_fee"];
			string discount_amount = request[i]["discount_amount"];							
			string shift_earnings = request[i]["shift_earnings"];										
			string payin_amount = request[i]["payin_amount"];
			string payout_amount = request[i]["payout_amount"];
			string shift_opening_amount = request[i]["shift_opening_amount"];
			string shift_closing_amount = request[i]["shift_closing_amount"];
			string shift_status = request[i]["shift_status"];
			string physical_cash_collected = request[i]["physical_cash_collected"];	
            string wallet_points = request[i]["wallet_points"];	

			query="select * from revenue_shifts where shift_id='"+shift_id+"'";
			res=stmt->executeQuery(query);
			if(!res->next())
				{
				//No shift	,create shift
				query="INSERT INTO revenue_shifts(device_number,device_name,carpark_number,carpark_name,facility_number,shift_id,operator_id,operator_name,login_time,payin_amount,shift_opening_amount,last_updated_date_time)VALUES("+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',"+facility_number+",'"+shift_id+"',"+operator_id+",'"+operator_name+"','"+login_time+"',"+payin_amount+","+shift_opening_amount+",CURRENT_TIMESTAMP)";				
				result = stmt->executeUpdate(query);
				}
				
			if(stoi(parking_fees)>0 || stoi(shift_earnings)>0)
				{
				query="UPDATE revenue_shifts set wallet_points="+wallet_points+",parking_fees="+parking_fees+",vat_amount="+vat_amount+",lost_fee="+lost_fees+",admin_fixed_charges="+admin_fixed_charges+",ticket_replacement_fee="+ticket_replacement_fee+",discount_amount="+discount_amount+",shift_earnings="+shift_earnings+",last_updated_date_time=CURRENT_TIMESTAMP where shift_id='"+shift_id+"'";
				result = stmt->executeUpdate(query);
                
				}
			if(stoi(shift_status)==1)	
				{
				query="Update revenue_shifts set shift_status=1,payout_amount="+payout_amount+",shift_closing_amount="+shift_closing_amount+",logout_time='"+logout_time+"',shift_duration='"+shift_duration+"',physical_cash_collected="+physical_cash_collected+",last_updated_date_time=CURRENT_TIMESTAMP where shift_id='"+shift_id+"'";
                
				result = stmt->executeUpdate(query);
                 
				}
            
            
            if(result==1 || result ==0)            
                jsonresponse[i] = id;              			
            
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        
        WriteToLog("****UploadRevenueShifts*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}	

Php::Value UploadRevenuePhysicalCashCollected(Php::Value request)
{    
    sql::Statement *stmt;	
    Php::Value result;    
    Json::Value jsonresponse;   
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];  
			
            string id = request[i]["id"];
			string user_id = request[i]["user_id"];
			string user_name = request[i]["user_name"];
			string shift_id = request[i]["shift_id"];			
			string denomination1 = request[i]["denomination1"];
			string denomination2 = request[i]["denomination2"];
			string denomination3 = request[i]["denomination3"];
			string denomination4 = request[i]["denomination4"];
			string denomination5 = request[i]["denomination5"];
			string denomination6 = request[i]["denomination6"];
			string denomination7 = request[i]["denomination7"];
			string denomination8 = request[i]["denomination8"];
			string denomination9 = request[i]["denomination9"];
			string denomination10 = request[i]["denomination10"];			
			string total = request[i]["total"];
			string date_time = request[i]["date_time"];

			
			query="INSERT INTO revenue_physical_cash(device_number,device_name,carpark_number,carpark_name,facility_number,shift_id,user_id,user_name,denomination1,denomination2,denomination3,denomination4,denomination5,denomination6,denomination7,denomination8,denomination9,denomination10,total,date_time,create_date_time)VALUES("+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',"+facility_number+",'"+shift_id+"',"+user_id+",'"+user_name+"',"+denomination1+","+denomination2+","+denomination3+","+denomination4+","+denomination5+","+denomination6+","+denomination7+","+denomination8+","+denomination9+","+denomination10+","+total+",'"+date_time+"',CURRENT_TIMESTAMP)";				
			result = stmt->executeUpdate(query);
				
           
            if(result==1)            
                jsonresponse[i] = id;              			
            
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        
        WriteToLog("****UploadRevenuePhysicalCashCollected*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}	

Php::Value UploadRevenueTransactionExceptions(Php::Value request)
{    
    sql::Statement *stmt;	
    Php::Value result;   
    Json::Value jsonresponse;   
    try
    {
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
            string device_number = request[i]["device_number"];
            string device_name = request[i]["device_name"];
            string carpark_number = request[i]["carpark_number"];
            string carpark_name = request[i]["carpark_name"];
            string facility_number = request[i]["facility_number"];  
			
            string id = request[i]["id"];
			string 	ticket_id = request[i]["ticket_id"];
			string chipcoin_datetime = request[i]["chipcoin_datetime"];
			string paid_status = request[i]["paid_status"];			
			string exception = request[i]["exception"];
			string date_time = request[i]["date_time"];
			string amount_received = request[i]["amount_received"];
			string balance_returned = request[i]["balance_returned"];
			string credit_note = request[i]["credit_note"];
			string bank_notes = request[i]["bank_notes"];			
			
			query="INSERT INTO revenue_payment_exceptions(device_number,device_name,carpark_number,carpark_name,facility_number,ticket_id,chipcoin_datetime,paid_status,exception,date_time,amount_received,balance_returned,credit_note,bank_notes)VALUES("+device_number+",'"+device_name+"',"+carpark_number+",'"+carpark_name+"',"+facility_number+",'"+ticket_id+"','"+chipcoin_datetime+"','"+paid_status+"','"+exception+"','"+date_time+"','"+amount_received+"','"+balance_returned+"','"+credit_note+"','"+bank_notes+"')";				
			result = stmt->executeUpdate(query);
				
           
            if(result==1)            
                jsonresponse[i] = id;              			
            
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {
        
        WriteToLog("****UploadRevenueTransactionExceptions*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}	

Php::Value UploadParkingManualMovements(Php::Value request)
{
    sql::Statement *stmt;
    Php::Value result;
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
			string id = request[i]["id"];
            string carpark_name = request[i]["carpark_name"];
            string carpark_number = request[i]["carpark_number"];           
            string date_time = request[i]["date_time"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string facility_number = request[i]["facility_number"];
            string movement_type = request[i]["movement_type"];
            string ticket_id = request[i]["ticket_id"];              
            string operator_id = request[i]["operator_id"]; 
            string operator_name = request[i]["operator_name"]; 
            string action = request[i]["action"]; 
            string description = request[i]["description"]; 
            string reason = request[i]["reason"]; 
			query="INSERT into parking_movements_manual(carpark_name,device_name,device_number,movement_type,carpark_number,facility_number,date_time,operator_id,operator_name,action,description,reason)VALUES('"+carpark_name+"','" + device_name + "'," +device_number + ",'" + movement_type + "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+operator_id+"','"+operator_name+"','"+action+"','"+description+"','"+reason+"')";            
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }			 			
			
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadParkingManualMovements*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn;  
        return e.what();
    }
    
    return result;
       
}

Php::Value UploadParkingMovementExceptions(Php::Value request)
{
    sql::Statement *stmt;
    Php::Value result;
    
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
			string id = request[i]["id"];
            string carpark_number = request[i]["carpark_number"];  
            string carpark_name = request[i]["carpark_name"];         
            string date_time = request[i]["date_time"];
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];
            string facility_number = request[i]["facility_number"];
            string chip_utid = request[i]["chip_utid"];
            string ticket_id = request[i]["ticket_id"];              
            string plate_number = request[i]["plate_number"];
			string exception = request[i]["exception"];
			
			query="INSERT into parking_movement_exceptions(carpark_name,device_name,device_number,chip_utid,ticket_id,carpark_number,facility_number,date_time,plate_number,exception) VALUES('"+carpark_name+"','" + device_name + "'," +device_number + ",'" + chip_utid + "','" + ticket_id+ "'," + carpark_number + "," +facility_number + ",'" +date_time + "','"+plate_number+"','"+exception+"')";            
            result = stmt->executeUpdate(query);
           
            if(result==1)
            {
                jsonresponse[i] = id;                
            }			 		
			
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadParkingMovementExceptions*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn; 
        return e.what(); 
    }
    
    return result;
       
}

long accessId()
	{
	struct tm tm;
	string startDate="2020-01-01 00:00:00";			
			
	strptime(startDate.c_str(),"%Y-%m-%d %H:%M:%S",&tm);		
	time_t t=mktime(&tm);
						
	time_t now=time(NULL);
	long seconds=difftime(now,t);
	return seconds;					
	}

Php::Value UploadContractPrkingsubscriptions(Php::Value request)
{
    sql::Statement *stmt;
    sql::ResultSet *res;
    Php::Value result;
    
    Json::Value jsonresponse;
    try
    {        
        conn = DBConnection(DATABASE);
        stmt = conn->createStatement();        
        for(int i=0;i<request.size();i++)
        {
            string device_name = request[i]["device_name"];
            string device_number = request[i]["device_number"];

            string id = request[i]["id"];
            string product_id = request[i]["product_id"];    
            string product_name = request[i]["product_name"];    
            string product_price = request[i]["product_price"];    
            string ticket_id = request[i]["ticket_id"];              
            string plate_number = request[i]["plate_number"];
            string tag = request[i]["tag"];
            string date_time = request[i]["date_time"];
            string customer_name = request[i]["customer_name"];
            string company_name = request[i]["company_name"];
            string customer_email = request[i]["customer_email"];
            string customer_mobile = request[i]["customer_mobile"];            
            string description = request[i]["description"];
            string validity_days = request[i]["validity_days"];
            string validity_start_date = request[i]["validity_start_date"];
            string validity_end_date = request[i]["validity_end_date"];                        
            string carpark_number = request[i]["carpark_number"];                                            
            string facility_number = request[i]["facility_number"];            
            
			           
			
            query="INSERT into contract_parking_transactions(device_name,device_number,ticket_id,plate_number,tag,date_time,product_id,product_name,product_price,customer_name,company_name,customer_email,customer_mobile,description,validity_days,validity_start_date,validity_end_date,carpark_number,facility_number) VALUES('"+device_name+"','"+device_number+"','"+ticket_id+"','"+plate_number+"','"+tag+"','"+date_time+"','"+product_id+"','"+product_name+"','"+product_price+"','"+customer_name+"','"+company_name+"','"+customer_email+"','"+customer_mobile+"','"+description+"','"+validity_days+"','"+validity_start_date+"','"+validity_end_date+"','"+carpark_number+"','"+facility_number+"')";            
            result = stmt->executeUpdate(query);
           
            if(result==1)
                {
                query="select * from parcx_server.access_whitelist where ticket_id='"+ticket_id+"'";
                res=stmt->executeQuery(query);
                if(res->next())//exit renew subscription
                    {                     
                    string id=res->getString("access_whitelist_id");                      
                    query="update parcx_server.access_whitelist set validity_start_date= '"+validity_start_date+"',validity_expiry_date='"+validity_end_date+"' where access_whitelist_id="+id;                               
                    stmt->executeUpdate(query);                     
                    delete res;
                    }
                else
                    {
                    query="insert into parcx_server.access_whitelist(facility_number,carpark_number,access_zones,ticket_id,access_id,plate_number,tag,validity_start_date,validity_expiry_date,customer_name,status)values""('"+facility_number+"','"+carpark_number+"','','"+ticket_id+"','"+to_string(accessId())+"','"+plate_number+"','"+tag+"','"+validity_start_date+"','"+validity_end_date+"','"+customer_name+"',1)";                    
                    stmt->executeUpdate(query);   
                    }
                jsonresponse[i] = id;                
                }			 		
			
            Json::FastWriter fw;
            string res = fw.write(jsonresponse);
            result = res;
        }
        
        delete stmt;
        delete conn;  
    }
    catch(sql::SQLException &e)
    {       
        WriteToLog("****UploadContractPrkingsubscriptions*****");
        WriteToLog(e.what());
		WriteToLog(query);
		delete conn; 
        return e.what(); 
    }
    
    return result;
       
}

Php::Value uploadToServer(Php::Parameters &params)
    {
    Php::Value data=params[0];      
    int task=data["task"];   
    Php::Value request=data["data"];
    Php::Value response;
    switch (task)
        {
        case 1:                
			response = UploadParkingMovements(request);			
			break;
		case 2:        
			response = UploadParkingAccessMovements(request);			
			break;
		case 3:        
			response = UploadWatchdogDeviceAlarms(request);			
			break;
		case 4:        
			response = UploadParkingBlacklists(request);			
			break;
		case 5:        
			response = UploadParkingValidations(request);			
			break;
		case 6:        
			response = UploadPayments(request);			
			break;
	   case 7:	   
			response=UpdateCashLevels(request,1);//apm_cashbox_levels			
			break;
	   case 8:
			response=UpdateCashLevels(request,2);//apm_recycle_levels			
			break;
	   case 9:        
			response = UploadApmCashPayout(request);			
			break;
	   case 10:        
			response = UploadApmCashRefill(request);			
			break;	
		case 11:        
			response = UploadParkingReservationMovements(request);			
			break;
		case 12:        
			response = UploadWaletTransaction(request);			
			break;
		case 13:        
			response = UploadRevenueShifts(request);			
			break;			
		case 14:        
			response = UploadRevenuePhysicalCashCollected(request);			
			break;
		case 15:        
			response = UploadRevenueTransactionExceptions(request);			
			break;
		case 16:        
			response = UploadParkingManualMovements(request);			
			break;		   
        case 17:    
            response=UploadParkingMovementExceptions(request);    
            break;     
        case 18:
            response=UploadContractPrkingsubscriptions(request);
            break;
    }
    return response;
    }
extern "C" {
    
    PHPCPP_EXPORT void *get_module() 
    {        
        static Php::Extension extension("PX_DeviceUpload", "1.0");
        extension.add<uploadToServer>("uploadToServer");        
        return extension;
    }
}
