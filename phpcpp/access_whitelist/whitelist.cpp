#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset_metadata.h>
#include<phpcpp.h>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateFormat "%Y-%m-%d"

using namespace std;
GeneralOperations General;
sql::Connection *con,*dcon;;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *prep_stmt;
string query;
string previousData;
bool facilityFlag,carparkFlag,deviceFlag;
int carpark_number,facility_number,device_number,device_type;

void writeLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-WhitelistSettings-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("WebApplication/ExceptionLogs/PX-WhitelistSettings-"+General.currentDateTime("%Y-%m-%d"),function,message); 
    Php::out<<message<<endl;
	writeLog(function,"Exception: "+message);   
    }

string ToString(Php::Value param)
	{
	string value=param;
	return value;
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


Php::Value insertUpdateWhitelist(Php::Value json)
    {
    string msg = "Failed";    
    try
        {        
        con= General.mysqlConnect(ServerDB);   
        string id=json["id"];
        string report_id=json["report_id"];
        if(id=="")    
            {            
            prep_stmt = con->prepareStatement("insert into access_whitelist(facility_number,carpark_number,access_zones,ticket_id,plate_number,tag,tag_serial,tag_tid,country,validity_start_date,validity_expiry_date,customer_name,antipassback_enabled,season_card,wallet_payment,corporate_parker,personalized_message_line1,personalized_message_line2,access_id,status) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,'"+to_string(accessId())+"',1)");            
            }
        else
            {
            prep_stmt = con->prepareStatement("update access_whitelist set facility_number=?,carpark_number=?,access_zones=?,ticket_id=?,plate_number=?,tag=?,tag_serial=?,tag_tid=?,country=?,validity_start_date=?,validity_expiry_date=?,customer_name=?,antipassback_enabled=?,season_card=?,wallet_payment=?,corporate_parker=?,personalized_message_line1=?,personalized_message_line2=? where access_whitelist_id=?");    
            prep_stmt->setString(19, id);       
            }
                                    
        prep_stmt->setString(1, ToString(json["facility_number"]));
        prep_stmt->setString(2, ToString(json["carpark_number"]));               
        prep_stmt->setString(3, ToString(json["access_zones"]));
        prep_stmt->setString(4, ToString(json["ticket_id"]));
        prep_stmt->setString(5, ToString(json["plate_number"]));
        prep_stmt->setString(6, ToString(json["tag"]));
        prep_stmt->setString(7, ToString(json["tag_serial"]));
        prep_stmt->setString(8, ToString(json["tag_tid"]));            
        prep_stmt->setString(9, ToString(json["country"]));
        prep_stmt->setString(10, ToString(json["start_date"]));
        prep_stmt->setString(11, ToString(json["expiry_date"]));
        prep_stmt->setString(12, ToString(json["customer_name"]));
        prep_stmt->setString(13, ToString(json["anti_passback"]));
        prep_stmt->setString(14, ToString(json["season_card"]));
        prep_stmt->setString(15, ToString(json["wallet_payment"]));
        prep_stmt->setString(16, ToString(json["corporate_parker"]));
        prep_stmt->setString(17, ToString(json["message1"]));
        prep_stmt->setString(18, ToString(json["message2"]));
                
        if (!prep_stmt->execute())            
            {
            msg = "Successfull"; 
            delete prep_stmt;                                                                      
		    delete con;               	                
            if(report_id!="0")
                {
                con= General.mysqlConnect(ReportingDB);       
                prep_stmt = con->prepareStatement("update parking_movements_access set whitelist_added=1 where tag =?");    
                prep_stmt->setString(1, ToString(json["tag"]));    
                prep_stmt->execute(); 
                delete prep_stmt;                                                                      
		        delete con;
                }
            }
              
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateWhitelist",e.what());
        }
    return msg;
    }

Php::Value enableDisable(string table,string id_field,string status_field,Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB); 
        prep_stmt = con->prepareStatement("update "+table+" set "+status_field+"=? where "+id_field+"=?");
        prep_stmt->setString(1, ToString(json["status"]));
        prep_stmt->setString(2, ToString(json["id"]));       
        if (!prep_stmt->execute())
				msg = "Successfull";	
		delete prep_stmt;
		delete con;       
        }
    catch(const std::exception& e)
        {
        writeException("enableDisable",e.what());
        }
    return msg;
    }

void showAccessWhitelistList()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from access_whitelist order by access_whitelist_id desc");
        if(res->rowsCount()>0)
            {            
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Customer Name</th>"<<endl;
            Php::out<<"<th>Plate Number</th>"<<endl;            
            Php::out<<"<th>Ticket Id</th>"<<endl;
            Php::out<<"<th>Tag</th>"<<endl;
            Php::out<<"<th>AntiPassBack</th>"<<endl;
            Php::out<<"<th>Valid From</th>"<<endl;
            Php::out<<"<th>Valid To</th>"<<endl;
            Php::out<<"<th></th>"<<endl;
            Php::out<<"<th></th>"<<endl;		           
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("access_whitelist_id")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("customer_name")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;            
            Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("tag")+"</td>"<<endl;
            if(res->getInt("antipassback_enabled")==1)
                Php::out<<"<td>Enabled</td>"<<endl;
            else            
                Php::out<<"<td>Disabled</td>"<<endl;
                                   
            Php::out<<"<td>"+res->getString("validity_start_date")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("validity_expiry_date")+"</td>"<<endl;
            Php::out << "<td>"<< std::endl;
            if(res->getInt("status")==1)
                Php::out << "<button type='button' class='col btn btn-danger whitelist-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success whitelist-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info whitelist-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;           
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }        
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showAccessWhitelistList",e.what());
        }
    
    }

void showDeviceListForAccessSynch()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_devices");
        if(res->rowsCount()>0)
            {             
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Device Number</th>"<<endl;
            Php::out<<"<th>Device Name</th>"<<endl;            
            Php::out<<"<th>Device IP</th>"<<endl;
            Php::out<<"<th>Synch Whitelist</th>"<<endl;                                  
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl; 
            Php::out<<"<td>"+res->getString("device_number")+"</td>"<<endl;                      
            Php::out<<"<td>"+res->getString("device_name")+"</td>"<<endl;            
            Php::out<<"<td>"+res->getString("device_ip")+"</td>"<<endl;                        
            Php::out << "<td>"<< std::endl;
            if(res->getInt("synch_whitelist_flag")==1)
                Php::out << "<button type='button' class='btn btn-danger synch-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='btn btn-success synch-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;            
            Php::out<<"</tr>"<<endl;	
            }           
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showDeviceListForAccessSynch",e.what());
        }
    
    }
  
Php::Value getDetails(string table,string id,Php::Value json)
    {
    Php::Value response;    
    try
        {
        con= General.mysqlConnect(ServerDB);
        prep_stmt = con->prepareStatement("select * from "+table+" where "+id+"=?");
        prep_stmt->setString(1, ToString(json["id"]));
        res=prep_stmt->executeQuery();
        if(res->next())
            {
            sql::ResultSetMetaData *res_meta = res -> getMetaData();
			int columns = res_meta -> getColumnCount();   
			for (int i = 1; i <= columns; i++) 							
				response[res_meta -> getColumnLabel(i)]=string(res->getString(i));				
            }
        delete res;    
        delete stmt;
		delete con;
        }
    catch(const std::exception& e)
        {
        writeException("getDetails",e.what());
        } 
    return response;       
    }

Php::Value syncWhitelist()
	{
	string message="";
	try
	    {	
        sql::ResultSet *device;	
        sql::Driver *driver;
        sql::Connection *con_local=NULL;	
        sql::Statement *stmt_local;	
		con= General.mysqlConnect(ServerDB);
		stmt = con->createStatement();							
		driver = get_driver_instance();
		string ip,devicename,carpark;	

        writeLog("syncWhitelist","Connected to db");
		device = stmt->executeQuery("Select device_ip,device_name from system_devices where synch_whitelist_flag = 1");
		while (device->next())
		    {            
			try
                {	
                ip = device->getString("device_ip");                
                devicename = device->getString("device_name");	
                writeLog("syncWhitelist","IP: "+ip+"\tDevice:"+devicename);				
                con_local = driver->connect(ip, "parcxterminal", "1fromParcx!19514");			
                con_local->setSchema("ParcxTerminal");	
                writeLog("syncWhitelist","Connected to "+ip);		
                stmt_local = con_local->createStatement();				
                stmt_local->executeUpdate("Truncate table access_whitelist");				
                stmt_local->executeUpdate("Set SQL_Mode='ALLOW_INVALID_DATES'");
                res = stmt->executeQuery("Select * from access_whitelist");               
                while (res->next())
                    {
                    prep_stmt = con_local->prepareStatement("Insert into access_whitelist(id,customer_name,ticket_id,plate_number,tag,tag_serial,start_date,expiry_date,access_zones,carpark_number,facility_number,tag_tid,antipassback,status,personalized_message_line1,personalized_message_line2) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
                    prep_stmt->setString(1, res->getString("access_whitelist_id"));
                    prep_stmt->setString(2, res->getString("customer_name"));
                    prep_stmt->setString(3, res->getString("ticket_id"));
                    prep_stmt->setString(4, res->getString("plate_number"));
                    prep_stmt->setString(5, res->getString("tag"));
                    prep_stmt->setString(6, res->getString("tag_serial"));
                    prep_stmt->setString(7, res->getString("validity_start_date"));
                    prep_stmt->setString(8, res->getString("validity_expiry_date"));				
                    prep_stmt->setString(9, res->getString("access_zones"));
                    prep_stmt->setString(10, res->getString("carpark_number"));
                    prep_stmt->setString(11, res->getString("facility_number"));
                    prep_stmt->setString(12, res->getString("tag_tid"));
                    prep_stmt->setString(13, res->getString("antipassback_enabled"));
                    prep_stmt->setString(14, res->getString("status"));
                    prep_stmt->setString(15, res->getString("personalized_message_line1"));
                    prep_stmt->setString(16, res->getString("personalized_message_line2"));
                    prep_stmt->executeUpdate();

				    delete prep_stmt;                
				    }
                delete stmt_local;	
								
                stmt->executeUpdate("Update system_devices set last_updated_whitelist = NOW() where device_ip = '" + ip + "'");
                message+=devicename+"("+ip+"):Success\n";                
                writeLog("syncWhitelist","Success "+ip);	
                delete stmt;			
                delete con_local;			
                delete res;			
			    }   
			catch (exception &e)
			    {
				writeException(ip+":syncWhitelist", e.what());
				message += devicename + "(" + ip +  ") : Failed\n ";
			    }								
		    }
		delete device;				
		delete con;		
		
		return message;
		}
	catch (exception &e)
		{			
        writeException("syncWhitelist", e.what());			
        return message;
		}	
	}

Php::Value parcxWhitelistSettings(Php::Parameters &params)
	{
	Php::Value data=params[0];      
    int task=data["task"];   
    Php::Value response;
    switch (task)
        {        
        case 1:showAccessWhitelistList();
            break;
        case 2:response=enableDisable("access_whitelist","access_whitelist_id","status",data); 
		    break;            
        case 3:response=insertUpdateWhitelist(data); 
		    break;
        case 4:response=getDetails("access_whitelist","access_whitelist_id",data); 
		    break; 
        case 5:response=syncWhitelist();
		    break;  
        case 6:showDeviceListForAccessSynch();
            break;  
        case 7:response=enableDisable("system_devices","id","synch_whitelist_flag",data); 
		    break; 
		}
	return response;
	}
    
extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_WhitelistSettings", "1.0");
        extension.add<parcxWhitelistSettings>("parcxWhitelistSettings");               
        return extension;
		}
	}
