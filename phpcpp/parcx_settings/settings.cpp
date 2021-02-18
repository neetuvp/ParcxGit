#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
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
    General.writeLog("WebApplication/ApplicationLogs/PX-Settings-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("WebApplication/ExceptionLogs/PX-Settings-"+General.currentDateTime("%Y-%m-%d"),function,message); 
    Php::out<<message<<endl;
	writeLog(function,"Exception: "+message);   
    }

string ToString(Php::Value param)
	{
	string value=param;
	return value;
	}

Php::Value insertUpdateFacility(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB);
        if(ToString(json["id"])=="")
            prep_stmt = con->prepareStatement("select * from system_facility where facility_number=?");
        else
            {
            prep_stmt = con->prepareStatement("select * from system_facility where facility_number=? and facility_id!=?");
            prep_stmt->setString(2, ToString(json["id"]));   
            }
        
        prep_stmt->setString(1, ToString(json["facility_number"]));
        res=prep_stmt->executeQuery();
        if(res->next())
            {
            msg = "Facility number already exist.Try with another facility number";   
            delete prep_stmt;
            delete res; 
            }
        else
            {
            if(ToString(json["id"])=="")    
                {
                prep_stmt = con->prepareStatement("insert into system_facility(facility_number,facility_name,facility_location,user_id,status) values(?,?,?,?,1)");
                prep_stmt->setString(4, ToString(json["user_id"]));
                }
            else
                {                
                prep_stmt = con->prepareStatement("update system_facility set facility_number=?,facility_name=?,facility_location=? where facility_id=?");
                prep_stmt->setString(4, ToString(json["id"]));
                }
            
            prep_stmt->setString(1, ToString(json["facility_number"]));
            prep_stmt->setString(2, ToString(json["facility_name"]));
            prep_stmt->setString(3, ToString(json["facility_location"]));
            
            if (!prep_stmt->execute())
                msg = "Successfull";
            delete prep_stmt;
            }                			
		delete con;          
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateFacility",e.what());
        }
    return msg;
    }

Php::Value insertUpdateSettings(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB);
        if(ToString(json["id"])=="")
            {
            prep_stmt = con->prepareStatement("select * from system_settings where setting_name=?");                
            prep_stmt->setString(1, ToString(json["setting_name"]));
            res=prep_stmt->executeQuery();
            if(res->next())
                {
                msg = "Settings already exist";   
                delete prep_stmt;
                delete res; 
                delete con;  
                return msg;
                }
            }                
        if(ToString(json["id"])=="")    
            {
            prep_stmt = con->prepareStatement("insert into system_settings(setting_value,setting_name,setting_label,setting_status) values(?,?,?,1)");                
            prep_stmt->setString(2, ToString(json["setting_name"]));           
            prep_stmt->setString(3, ToString(json["setting_description"]));           
            }
        else
            {                
            prep_stmt = con->prepareStatement("update system_settings set setting_value=? where settings_id=?");
            prep_stmt->setString(2, ToString(json["id"]));
            }
        
        prep_stmt->setString(1, ToString(json["setting_value"]));           
        
        if (!prep_stmt->execute())
            {
            msg = "Successfull";
            if(ToString(json["id"])!="" && ToString(json["setting_name"])=="facility_number")  
                {
                facility_number= json["setting_value"];   
                
                prep_stmt = con->prepareStatement("update system_carparks set facility_number=?");
                prep_stmt->setInt(1,facility_number);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update system_devices set facility_number=?");
                prep_stmt->setInt(1,facility_number);
                prep_stmt->execute();
                delete prep_stmt;

                delete con;
                con=General.mysqlConnect(DashboardDB);

                prep_stmt = con->prepareStatement("update counters set facility_number=?");
                prep_stmt->setInt(1,facility_number);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update parking_revenue set facility_number=?");
                prep_stmt->setInt(1,facility_number);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update watchdog_device_status set facility_number=?");
                prep_stmt->setInt(1,facility_number);
                prep_stmt->execute();
                delete prep_stmt;

                delete con;
                }

            if(ToString(json["id"])!="" && ToString(json["setting_name"])=="facility_name")  
                {
                string facility_name= json["setting_value"];   
                
                prep_stmt = con->prepareStatement("update system_carparks set facility_name=?");
                prep_stmt->setString(1,facility_name);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update system_devices set facility_name=?");
                prep_stmt->setString(1,facility_name);
                prep_stmt->execute();
                delete prep_stmt;

                delete con;
                con=General.mysqlConnect(DashboardDB);

                prep_stmt = con->prepareStatement("update counters set carpark_name=? where counter_type=0");
                prep_stmt->setString(1,facility_name);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update parking_revenue set facility_name=?");
                prep_stmt->setString(1,facility_name);
                prep_stmt->execute();
                delete prep_stmt;

                prep_stmt = con->prepareStatement("update watchdog_device_status set facility_name=?");
                prep_stmt->setString(1,facility_name);
                prep_stmt->execute();
                delete prep_stmt;

                delete con;
                }
            }
        else
            {
            delete prep_stmt;    
            delete con;   
            }                                         					        
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateSettings",e.what());
        }
    return msg;
    }
	
Php::Value insertUpdateMaintenanceSettings(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB);                   
        if(ToString(json["id"])=="")    
            {
            prep_stmt = con->prepareStatement("insert into device_maintenance(setting_value,setting_name,enabled) values(?,?,1)");                
            prep_stmt->setString(2, ToString(json["setting_name"]));           
            }
        else
            {                
            prep_stmt = con->prepareStatement("update device_maintenance set setting_value=? where id=?");
            prep_stmt->setString(2, ToString(json["id"]));
            }
        
        prep_stmt->setString(1, ToString(json["setting_value"]));           
        
        if (!prep_stmt->execute())
            {
            msg = "Successfull";
            }
        else
            {
            delete prep_stmt;    
            delete con;   
            }                                         					        
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateMaintenanceSettings",e.what());
        }
    return msg;
    }
	
Php::Value updateCloudUploadSettings(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB);
                       
        prep_stmt = con->prepareStatement("update pxcloud_upload_settings set day_closure=?,time_interval=?,upload_row_limit=? where id=?");        

        prep_stmt->setString(1, ToString(json["dc"]));   
        prep_stmt->setString(2, ToString(json["interval"]));   
        prep_stmt->setString(3, ToString(json["limit"]));   		
        prep_stmt->setString(4, ToString(json["id"]));
        
        if (!prep_stmt->execute())
            {
            msg = "Successfull";
            }
        else
            {
            delete prep_stmt;    
            delete con;   
            }                                         					        
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateMaintenanceSettings",e.what());
        }
    return msg;
    }
	
Php::Value updateCloudDownloadSettings(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        con= General.mysqlConnect(ServerDB);
                       
	prep_stmt = con->prepareStatement("update pxcloud_download_settings set day_closure=?,time_interval=?,download_row_limit=? where id=?");	            
        
        prep_stmt->setString(1, ToString(json["dc"]));   
	prep_stmt->setString(2, ToString(json["interval"]));   
	prep_stmt->setString(3, ToString(json["limit"]));   		      
        prep_stmt->setString(4, ToString(json["id"]));
        
        if (!prep_stmt->execute())
            {
            msg = "Successfull";
            }
        else
            {
            delete prep_stmt;    
            delete con;   
            }                                         					        
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateMaintenanceSettings",e.what());
        }
    return msg;
    }

Php::Value insertUpdateCarpark(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        string id=json["id"];
        int facility_number=json["facility_number"];
        int carpark_number=json["carpark_number"];        
        string user_id=json["user_id"];
        string facility_name=json["facility_name"];
        string carpark_name=json["carpark_name"];
        string total_spaces=json["total_spaces"];
        string occupancy_threshold=json["occupancy_threshold"];
        string reservation_spaces=json["reservation_spaces"];
        string access_spaces=json["access_spaces"];
        string shortterm_spaces=json["shortterm_spaces"];
        string rate_type=json["rate_type"];
        string rate_plan=json["rate_plan"];
        string reservation_rate_plan=json["reservation_rate_plan"];
        
        
        con= General.mysqlConnect(ServerDB);
        if(id=="")
            prep_stmt = con->prepareStatement("select * from system_carparks where facility_number=? and carpark_number=?");
        else
            {
            prep_stmt = con->prepareStatement("select * from system_carparks where facility_number=? and carpark_number=? and carpark_id!=?");
            prep_stmt->setString(3, id); 
            }
        
        prep_stmt->setInt(1, facility_number);
        prep_stmt->setInt(2, carpark_number);
        res=prep_stmt->executeQuery();
        if(res->next())
            {
            msg = "Carpark number already exist in "+facility_name+".Try with another carpark number";   
            delete prep_stmt;
            delete res;             
            }
        else
            {
            if(id=="")    
                {
                prep_stmt = con->prepareStatement("insert into system_carparks(facility_number,facility_name,carpark_number,carpark_name,total_spaces ,occupancy_threshold,reservation_spaces,access_spaces,shortterm_spaces,rate_type,rate_plan,reservation_rate_plan,user_id,status) values(?,?,?,?,?,?,?,?,?,?,?,?,?,1)");
                prep_stmt->setString(13, user_id);       
                }
            else
                {
                prep_stmt = con->prepareStatement("update system_carparks set facility_number=?,facility_name=?,carpark_number=?,carpark_name=?,total_spaces=?,occupancy_threshold=?,reservation_spaces=?,access_spaces=?,shortterm_spaces=?,rate_type=?,rate_plan=?,reservation_rate_plan=? where carpark_id=?");    
                prep_stmt->setString(13, id);       
                }
            
            prep_stmt->setInt(1, facility_number);
            prep_stmt->setString(2, facility_name);               
            prep_stmt->setInt(3, carpark_number);
            prep_stmt->setString(4, carpark_name);
            prep_stmt->setString(5, total_spaces);
            prep_stmt->setString(6, occupancy_threshold);
            prep_stmt->setString(7, reservation_spaces);
            prep_stmt->setString(8, access_spaces);            
            prep_stmt->setString(9, shortterm_spaces);   
            prep_stmt->setString(10, rate_type);   
            prep_stmt->setString(11, rate_plan);   
            prep_stmt->setString(12, reservation_rate_plan);   
                    
            if (!prep_stmt->execute())
                {
                msg = "Successfull";
                
                prep_stmt = con->prepareStatement("update system_devices set rate_type=?,rate_plan=?,reservation_rate_plan=? where carpark_number=? and facility_number=?");
                prep_stmt->setString(1, rate_type);   
                prep_stmt->setString(2, rate_plan);   
                prep_stmt->setString(3, reservation_rate_plan);   
                prep_stmt->setInt(4, carpark_number);  
                prep_stmt->setInt(5, facility_number);  
                prep_stmt->executeUpdate();
                delete prep_stmt;
            
                facilityFlag=false;
                carparkFlag=false;	                               
                
                dcon=General.mysqlConnect(DashboardDB);
                prep_stmt = dcon->prepareStatement("select * from counters where facility_number="+ToString(json["previous_facility"]));
                res=prep_stmt->executeQuery();
                while(res->next())
                    {
                    if(res->getInt("count_category")==0 && res->getInt("counter_type")==0 && res->getInt("facility_number")==facility_number) 
                       facilityFlag=true;    
                    if(res->getInt("count_category")==0 && res->getInt("counter_type")==1 && res->getInt("carpark_number")==carpark_number) 
                        carparkFlag=true;
                    }
                delete res;
                delete prep_stmt;               
                if(carparkFlag==false)
                    {
                    prep_stmt = dcon->prepareStatement("insert into counters(facility_number,carpark_number,carpark_name,total_spaces,occupancy_threshold,total_reservation_spaces,total_access_spaces,total_shortterm_spaces,dashboard_order,counter_type)values(?,?,?,?,?,?,?,?,?,1)");                    
                    prep_stmt->setInt(9, carpark_number);
                    }
                else
                    {
                    prep_stmt = dcon->prepareStatement("update counters set facility_number=?,carpark_number=?,carpark_name=?,total_spaces=?,occupancy_threshold=?,total_reservation_spaces=?,total_access_spaces=?,total_shortterm_spaces=? where carpark_number=? and facility_number=? and counter_type=1");                            
                    prep_stmt->setString(9, ToString(json["previous_carpark"]));
                    prep_stmt->setString(10, ToString(json["previous_facility"]));
                    }
                prep_stmt->setInt(1, facility_number);                             
                prep_stmt->setInt(2, carpark_number);
                prep_stmt->setString(3, carpark_name);
                prep_stmt->setString(4, total_spaces);
                prep_stmt->setString(5, occupancy_threshold);
                prep_stmt->setString(6, reservation_spaces);
                prep_stmt->setString(7, access_spaces);            
                prep_stmt->setString(8, shortterm_spaces);                 
                prep_stmt->executeUpdate();

                delete prep_stmt;
                prep_stmt = con->prepareStatement("SELECT sum(total_spaces) as total_spaces,sum(reservation_spaces) as reservation_spaces,sum(access_spaces) as access_spaces,sum(shortterm_spaces) as shortterm_spaces FROM system_carparks where facility_number=?");
                prep_stmt->setString(1, ToString(json["previous_facility"]));                
                res=prep_stmt->executeQuery();
                if(res->next())
                    {                        
                    delete prep_stmt;              
                    if(facilityFlag==false)
                        {
                        prep_stmt = dcon->prepareStatement("insert into counters(facility_number,carpark_name,total_spaces,total_reservation_spaces,total_access_spaces,total_shortterm_spaces,dashboard_order,counter_type)values(?,?,?,?,?,?,?,0)");                    
                        prep_stmt->setInt(7, facility_number);
                        }
                    else
                        {
                        prep_stmt = dcon->prepareStatement("update counters set facility_number=?,carpark_name=?,total_spaces=?,total_reservation_spaces=?,total_access_spaces=?,total_shortterm_spaces=? where facility_number=? and counter_type=0");                                               
                        prep_stmt->setString(7, ToString(json["previous_facility"]));
                        }
                    prep_stmt->setInt(1, facility_number);
                    prep_stmt->setString(2, facility_name);                               
                    prep_stmt->setString(3, res->getString("total_spaces"));
                    prep_stmt->setString(4, res->getString("reservation_spaces"));
                    prep_stmt->setString(5, res->getString("access_spaces"));
                    prep_stmt->setString(6, res->getString("shortterm_spaces"));
                    prep_stmt->executeUpdate();
                    
                    delete prep_stmt;
                    delete res;                
                    }
                }            
            }
		delete con;       
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateCarpark",e.what());
        }
    return msg;
    }

Php::Value insertUpdateDevice(Php::Value json)
    {
    string msg = "Failed";    
    try
        {
        string id=json["id"];
        carpark_number=json["carpark_number"];
        facility_number=json["facility_number"];
        device_number=json["device_number"];
        device_type=json["device_category"];

        con= General.mysqlConnect(ServerDB);
        if(id=="")
            prep_stmt = con->prepareStatement("select * from system_devices where facility_number=? and device_number=?");
        else
            {
            prep_stmt = con->prepareStatement("select * from system_devices where facility_number=? and device_number=? and id!=?");
            prep_stmt->setString(3, id); 
            }
        
        prep_stmt->setInt(1, facility_number);
        prep_stmt->setInt(2, device_number);       
        res=prep_stmt->executeQuery();
        if(res->next())
            {
            msg = "Device number already exist in facility-"+ToString(json["facility_name"])+".Try with another device number";   
            delete prep_stmt;
            delete res;             
            }
        else
            {
            if(id=="")    
                {
                prep_stmt = con->prepareStatement("select * from system_carparks where facility_number=? and carpark_number=?");
                prep_stmt->setInt(1, facility_number);
                prep_stmt->setInt(2, carpark_number);    
                res=prep_stmt->executeQuery();
                res->next();
                
                prep_stmt = con->prepareStatement("insert into system_devices(facility_number,facility_name,carpark_number,carpark_name,device_number,device_name,device_category,device_category_name,device_ip,camera_id,customer_receipt_mandatory,shift_receipt_mandatory,shift_physical_cash_count_required,synch_whitelist_flag,issue_lost,camera_index,anpr_enabled, wiegand_enabled, access_enabled, reservation_enabled, review_mode, device_function, barrier_open_time_limit,duration_hold_barrier_open, display_anpr_image, barrier_open_status_type, bms_status_enabled, barrier_status_monitoring, wiegand2_enabled, server_handshake_interval, plate_capturing_wait_delay, quick_barrier_close, payment_enabled_exit,rate_plan,reservation_rate_plan,rate_type,user_id,device_enabled) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,1)");
                prep_stmt->setString(34, res->getString("rate_plan"));
                prep_stmt->setString(35,res->getString("reservation_rate_plan"));
                prep_stmt->setString(36, res->getString("rate_type"));
                prep_stmt->setString(37, ToString(json["user_id"]));       
                delete res;
                }
            else
                {
                prep_stmt = con->prepareStatement("update system_devices set facility_number=?,facility_name=?,carpark_number=?,carpark_name=?,device_number=?,device_name=?,device_category=?,device_category_name=?,device_ip=?,camera_id=?,customer_receipt_mandatory=?,shift_receipt_mandatory=?,shift_physical_cash_count_required=?,synch_whitelist_flag=?,issue_lost=?,camera_index=?,anpr_enabled=?, wiegand_enabled=?, access_enabled=?, reservation_enabled=?, review_mode=?, device_function=?, barrier_open_time_limit=?,duration_hold_barrier_open=?, display_anpr_image=?, barrier_open_status_type=?, bms_status_enabled=?, barrier_status_monitoring=?, wiegand2_enabled=?, server_handshake_interval=?, plate_capturing_wait_delay=?,quick_barrier_close=?, payment_enabled_exit=? where id=?");    
                prep_stmt->setString(34,id);       
                }
                                        
            prep_stmt->setString(1, ToString(json["facility_number"]));
            prep_stmt->setString(2, ToString(json["facility_name"]));               
            prep_stmt->setString(3, ToString(json["carpark_number"]));
            prep_stmt->setString(4, ToString(json["carpark_name"]));
            prep_stmt->setString(5, ToString(json["device_number"]));
            prep_stmt->setString(6, ToString(json["device_name"]));
            prep_stmt->setString(7, ToString(json["device_category"]));
            prep_stmt->setString(8, ToString(json["device_category_name"]));            
            prep_stmt->setString(9, ToString(json["device_ip"]));
            prep_stmt->setString(10, ToString(json["camera_id"]));
            prep_stmt->setString(11, ToString(json["customer_receipt_mandatory"]));
            prep_stmt->setString(12, ToString(json["shift_receipt_mandatory"]));
            prep_stmt->setString(13, ToString(json["physical_cash_count"]));
            prep_stmt->setString(14, ToString(json["synch_whitelist"]));
            prep_stmt->setString(15, ToString(json["issue_lost"]));
            prep_stmt->setString(16, ToString(json["camera_index"]));
            prep_stmt->setString(17, ToString(json["anpr_enabled"]));
            prep_stmt->setString(18, ToString(json["wiegand_enabled"]));
            prep_stmt->setString(19, ToString(json["access_enabled"]));
            prep_stmt->setString(20, ToString(json["reservation_enabled"]));
            prep_stmt->setString(21, ToString(json["review_mode"]));
            prep_stmt->setString(22, ToString(json["device_function"]));
            prep_stmt->setString(23, ToString(json["barrier_open_time_limit"]));
            prep_stmt->setString(24, ToString(json["duration_hold_barrier_open"]));
            prep_stmt->setString(25, ToString(json["display_anpr_image"]));
            prep_stmt->setString(26, ToString(json["barrier_open_status_type"]));
            prep_stmt->setString(27, ToString(json["bms_status_enabled"]));
            prep_stmt->setString(28, ToString(json["barrier_status_monitoring"]));
            prep_stmt->setString(29, ToString(json["wiegand2_enabled"]));
            prep_stmt->setString(30, ToString(json["server_handshake_interval"]));
            prep_stmt->setString(31, ToString(json["plate_capturing_wait_delay"]));
            prep_stmt->setString(32, ToString(json["quick_barrier_close"]));
            prep_stmt->setString(33, ToString(json["payment_enabled_exit"]));
            
                    
            if (!prep_stmt->execute())
                {
                msg = "Successfull";               	
                deviceFlag=false;                
                dcon=General.mysqlConnect(DashboardDB);
                prep_stmt = dcon->prepareStatement("select * from watchdog_device_status where facility_number="+ToString(json["previous_facility"])+" and carpark_number="+ToString(json["previous_carpark"])+" and device_number="+ToString(json["previous_device"]));
                res=prep_stmt->executeQuery();
                if(res->next())                    
                    deviceFlag=true;
               
                delete res;
                delete prep_stmt;               
                if(deviceFlag==false)
                    {                    
                    prep_stmt = dcon->prepareStatement("insert into watchdog_device_status(facility_number,facility_name,carpark_number,carpark_name,device_number,device_name,device_ip,device_type)values(?,?,?,?,?,?,?,?)");                                        
                    }
                else
                    {
                    prep_stmt = dcon->prepareStatement("update watchdog_device_status set facility_number=?,facility_name=?,carpark_number=?,carpark_name=?,device_number=?,device_name=?,device_ip=?,device_type=? where carpark_number=? and facility_number=? and device_number=?");                            
                    prep_stmt->setString(9, ToString(json["previous_carpark"]));
                    prep_stmt->setString(10, ToString(json["previous_facility"]));
                    prep_stmt->setString(11, ToString(json["previous_device"]));
                    }
                prep_stmt->setString(1, ToString(json["facility_number"]));
                prep_stmt->setString(2, ToString(json["facility_name"]));
                prep_stmt->setString(3, ToString(json["carpark_number"]));
                prep_stmt->setString(4, ToString(json["carpark_name"]));
                prep_stmt->setString(5, ToString(json["device_number"]));
                prep_stmt->setString(6, ToString(json["device_name"]));
                prep_stmt->setString(7, ToString(json["device_ip"]));
                prep_stmt->setString(8, ToString(json["device_category"]));                            
                prep_stmt->executeUpdate();

                delete prep_stmt;
                if(device_type==3||device_type==4||device_type==5)
                    {
                    deviceFlag=false;    
                    prep_stmt = dcon->prepareStatement("select * from parking_revenue where facility_number="+ToString(json["previous_facility"])+" and carpark_number="+ToString(json["previous_carpark"])+" and device_number="+ToString(json["previous_device"]));
                    res=prep_stmt->executeQuery();                    
                    if(res->next())
                        deviceFlag=true;                       
                    delete res;
                    delete prep_stmt;              

                    if(deviceFlag==false)
                        {
                        prep_stmt = dcon->prepareStatement("insert into parking_revenue(facility_number,facility_name,carpark_number,carpark_name,device_number,device_name,device_type)values(?,?,?,?,?,?,?)");                                           
                        }
                     else
                        {
                        prep_stmt = dcon->prepareStatement("update parking_revenue set facility_number=?,facility_name=?,carpark_number=?,carpark_name=?,device_number=?,device_name=?,device_type=? where carpark_number=? and facility_number=? and device_number=?");                            
                        prep_stmt->setString(8, ToString(json["previous_carpark"]));
                        prep_stmt->setString(9, ToString(json["previous_facility"]));
                        prep_stmt->setString(10, ToString(json["previous_device"]));
                        }
                    prep_stmt->setString(1, ToString(json["facility_number"]));
                    prep_stmt->setString(2, ToString(json["facility_name"]));
                    prep_stmt->setString(3, ToString(json["carpark_number"]));
                    prep_stmt->setString(4, ToString(json["carpark_name"]));
                    prep_stmt->setString(5, ToString(json["device_number"]));
                    prep_stmt->setString(6, ToString(json["device_name"]));                    
                    prep_stmt->setString(7, ToString(json["device_category"]));                            
                    prep_stmt->executeUpdate();
                    
                    delete prep_stmt;                                                     
                    }
                }            
            }
		delete con;       
        }
    catch(const std::exception& e)
        {
        writeException("insertUpdateDevice",e.what());
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

void showFacilityList()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_facility");
        if(res->rowsCount()>0)
            {
           // Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Facility Number</th>"<<endl;
            Php::out<<"<th>Facility Name</th>"<<endl;            
            Php::out<<"<th>Facility Location</th>"<<endl;
            Php::out<<"<th></th>"<<endl;
            Php::out<<"<th></th>"<<endl;		           
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("facility_id")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("facility_number")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("facility_name")+"</td>"<<endl;            
            Php::out<<"<td>"+res->getString("facility_location")+"</td>"<<endl;
            Php::out << "<td>"<< std::endl;
            if(res->getInt("status")==1)
                Php::out << "<button type='button' class='col btn btn-danger facility-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success facility-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info facility-edit' ><i class='fas fa-edit'></i>Edit</button>"<< std::endl;           
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showFacilityList",e.what());
        }
    
    }

void showDeviceList()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_devices");
        if(res->rowsCount()>0)
            {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Device Number</th>"<<endl;
            Php::out<<"<th>Device Name</th>"<<endl;            
            Php::out<<"<th>Device IP</th>"<<endl;
            Php::out<<"<th>Device Category</th>"<<endl;
            Php::out<<"<th>Carpark</th>"<<endl;
            Php::out<<"<th>Facility</th>"<<endl;            
            Php::out<<"<th></th>"<<endl;
            Php::out<<"<th></th>"<<endl;		           
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl; 
            Php::out<<"<td>"+res->getString("device_number")+"</td>"<<endl;                      
            Php::out<<"<td>"+res->getString("device_name")+"</td>"<<endl;            
            Php::out<<"<td>"+res->getString("device_ip")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("device_category_name")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("facility_name")+"</td>"<<endl;
            Php::out << "<td>"<< std::endl;
            if(res->getInt("device_enabled")==1)
                Php::out << "<button type='button' class='col btn btn-danger device-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success device-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;
            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info device-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;           
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showDeviceList",e.what());
        }
    
    }
  
void showCarparkList()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_carparks");
        if(res->rowsCount()>0)
            {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>Carpark Number</th>"<<endl;
            Php::out<<"<th>Carpark Name</th>"<<endl;           
            Php::out<<"<th>Facility Name</th>"<<endl;
            Php::out<<"<th>Total Spaces</th>"<<endl;           
            Php::out<<"<th></th>"<<endl;		           
            Php::out<<"<th></th>"<<endl;		           
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("carpark_id")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("carpark_number")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;            
            Php::out<<"<td>"+res->getString("facility_name")+"</td>"<<endl;
            Php::out<<"<td>"+res->getString("total_spaces")+"</td>"<<endl;
            
            Php::out << "<td>"<< std::endl;
            if(res->getInt("status")==1)
                Php::out << "<button type='button' class='col btn btn-danger carpark-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success carpark-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;

            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info carpark-edit' ><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showCarparkList",e.what());
        }
    
    }

void showSettingList()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_settings");
        if(res->rowsCount()>0)
            {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
			Php::out<<"<tr>"<<endl;
            Php::out<<"<th>No</th>"<<endl;
            Php::out<<"<th>Setting Name</th>"<<endl;
            Php::out<<"<th>Setting value</th>"<<endl;                   
            Php::out<<"<th></th>"<<endl;		           
            Php::out<<"<th></th>"<<endl;		           
			Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            }
        while(res->next())
            {
            Php::out<<"<tr data-id='"<<res->getString("settings_id")<<"' data-name='"<<res->getString("setting_name")<<"'>"<<endl;                       
            Php::out<<"<td>"+res->getString("settings_id")+"</td>"<<endl;           
            Php::out<<"<td>"+res->getString("setting_label")+"</td>"<<endl;           
            Php::out<<"<td>"+res->getString("setting_value")+"</td>"<<endl;           
            
            Php::out << "<td>"<< std::endl;
            if(res->getInt("setting_status")==1)
                Php::out << "<button type='button' class='col btn btn-danger setting-enable-disable-btn'>Disable</button>"<< std::endl;
            else
                Php::out << "<button type='button' class='col btn btn-success setting-enable-disable-btn'>Enable</button>"<< std::endl;
            Php::out << "</td>"<< std::endl;

            Php::out << "<td>"<< std::endl;
            Php::out << "<button type='button' class='col btn btn-info setting-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
            Php::out << "</td>"<< std::endl;
            Php::out<<"</tr>"<<endl;	
            }
        //Php::out<<"</table>"<<endl;	
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showSettingList",e.what());
        }    
    }
	
void showMaintenanceSettingsList()
    {
	string id = "0";
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from device_maintenance");
        if(res->rowsCount()>0)
            {
            
            Php::out << "<thead>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>No</th>"<<endl;
            Php::out<<"<th>Setting Name</th>"<<endl;
            Php::out<<"<th>Setting value</th>"<<endl;                   
            Php::out<<"<th></th>"<<endl;		           
            Php::out<<"<th></th>"<<endl;		           
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            
            while(res->next())
                {
                id = res->getString("id");
                Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl;                       
                Php::out<<"<td>"+res->getString("id")+"</td>"<<endl;           
                Php::out<<"<td>"+res->getString("setting_label")+"</td>"<<endl;           
                Php::out<<"<td>"+res->getString("setting_value")+"</td>"<<endl;    				
                Php::out << "<td><button type='button' class='col btn btn-danger maintenance-setting-cancel-btn' id='cancelbtn"+id+"' style='display:none' >Cancel</button></td>"<< std::endl;								
                Php::out << "<td>"<< std::endl;
                Php::out << "<button type='button' class='col btn btn-info maintenance-setting-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
                Php::out << "</td>"<< std::endl;
                Php::out<<"</tr>"<<endl;	
                }
            //Php::out << "</table>" << std::endl;	
            }
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showMaintenanceSettingsList",e.what());
        }    
    }
	
void showCloudUploadSettings()
{
	try
        {
		string id="0",dc="";
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from pxcloud_upload_settings");
        if(res->rowsCount()>0)
        {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>No</th>"<<endl;
            Php::out<<"<th>Table</th>"<<endl;
            Php::out<<"<th>Dayclosure</th>"<<endl;                   
           // Php::out<<"<th>Task</th>"<<endl;	
            Php::out<<"<th>Upload Limit</th>"<<endl;	
            Php::out<<"<th>Time Interval</th>"<<endl;				
            Php::out<<"<th>Cloud Upload Date</th>"<<endl;
            Php::out<<"<th></th><th></th>"<<endl;	
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            
            while(res->next())
                    {
                    id = res->getString("id");
                    Php::out<<"<tr data-id='"<<id<<"' >"<<endl;  
                    Php::out<<"<td>"+res->getString("id")+"</td>"<<endl;                      	
                    Php::out<<"<td>"+res->getString("label")+"</td>"<<endl;           
                    //Php::out<<"<td>"+res->getString("day_closure")+"</td>"<<endl;  
                    if(res->getString("day_closure")=="1")
                    {
                            dc = "checked";
                    }
                    else{
                            dc = "";
                    }
                    Php::out<<"<td><input type='checkbox' id='dc"+id+"'  "+dc+" onclick='return false;'></td>";		
                    //Php::out<<"<td id='task"+id+"'>"+res->getString("task")+"</td>"<<endl;  
                    Php::out<<"<td id='limit"+id+"'>"+res->getString("upload_row_limit")+"</td>"<<endl;  
                    Php::out<<"<td id='interval"+id+"'>"+res->getString("time_interval")+"</td>"<<endl;  
                    Php::out<<"<td>"+res->getString("cloud_upload_date_time")+"</td>"<<endl; 
                    Php::out << "<td>"<< std::endl;
                    if(res->getInt("enabled")==1)
                            Php::out << "<button type='button' class='col btn btn-danger upload-setting-enable-disable-btn'>Disable</button>"<< std::endl;
                    else
                            Php::out << "<button type='button' class='col btn btn-success upload-setting-enable-disable-btn'>Enable</button>"<< std::endl;
                    Php::out << "</td>"<< std::endl;

                    Php::out << "<td>"<< std::endl;
                    Php::out << "<button type='button' class='col btn btn-info upload-setting-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
                    Php::out << "</td>"<< std::endl;
                    Php::out<<"</tr>"<<endl;	
                    }
            //Php::out << "</table>" << std::endl;	
		}
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showSettingList",e.what());
        }
}

void showCloudDownloadSettings()
{
	try
        {
		string id="0",dc="";
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from pxcloud_download_settings");
        if(res->rowsCount()>0)
        {
            //Php::out<<"<table class='table table-blue'>"<<endl;
            Php::out << "<thead>" << std::endl;
            Php::out<<"<tr>"<<endl;
            Php::out<<"<th>No</th>"<<endl;
            Php::out<<"<th>Table</th>"<<endl;
            Php::out<<"<th>Dayclosure</th>"<<endl;                   
           // Php::out<<"<th>Task</th>"<<endl;	
            Php::out<<"<th>Download Limit</th>"<<endl;	
            Php::out<<"<th>Time Interval</th>"<<endl;				
            Php::out<<"<th>Cloud Download Date</th>"<<endl;
            Php::out<<"<th></th><th></th>"<<endl;	
            Php::out<<"</tr>"<<endl;	
            Php::out << "</thead>" << std::endl;		
            
            while(res->next())
                {
                        id = res->getString("id");
                Php::out<<"<tr data-id='"<<res->getString("id")<<"'>"<<endl;  
                Php::out<<"<td>"+res->getString("id")+"</td>"<<endl;                      	
                Php::out<<"<td>"+res->getString("label")+"</td>"<<endl;           
                //Php::out<<"<td>"+res->getString("day_closure")+"</td>"<<endl;  
                if(res->getString("day_closure")=="1")
                {
                        dc = "checked";
                }
                else{
                        dc = "";
                }
                Php::out<<"<td><input type='checkbox' id='dc"+id+"'  "+dc+" onclick='return false;'></td>";		
               // Php::out<<"<td id='task"+id+"'>"+res->getString("task")+"</td>"<<endl;  
                Php::out<<"<td id='limit"+id+"'>"+res->getString("download_row_limit")+"</td>"<<endl;  
                Php::out<<"<td id='interval"+id+"'>"+res->getString("time_interval")+"</td>"<<endl;  
                Php::out<<"<td>"+res->getString("cloud_download_date_time")+"</td>"<<endl; 
                Php::out << "<td>"<< std::endl;
                if(res->getInt("enabled")==1)
                        Php::out << "<button type='button' class='col btn btn-danger download-setting-enable-disable-btn'>Disable</button>"<< std::endl;
                else
                        Php::out << "<button type='button' class='col btn btn-success download-setting-enable-disable-btn'>Enable</button>"<< std::endl;
                Php::out << "</td>"<< std::endl;

                Php::out << "<td>"<< std::endl;
                Php::out << "<button type='button' class='col btn btn-info download-setting-edit'><i class='fas fa-edit'></i>Edit</button>"<< std::endl;            
                Php::out << "</td>"<< std::endl;
                Php::out<<"</tr>"<<endl;	
                }
			///Php::out << "</table>" << std::endl;	
            }
        delete res;    
        delete stmt;
		delete con;  
        }
    catch(const std::exception& e)
        {
        writeException("showSettingList",e.what());
        }
}

int occuranceOfComma(string str)
	{
	int n=0;
	for (int i=0;i<(signed)str.length();i++)
		{
		if(str[i]==',')
			n++;
		}
	return n;
	}

void showProductList()
	{
	try
		{								
		con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_products order by product_id desc");					
		
		if(res->rowsCount()>0)			
			{	
			//Php::out<<"<table class='table table-blue'>" << std::endl;	
			Php::out << "<thead>" << std::endl;
            Php::out << "<tr>" << std::endl;   
			Php::out << "<th>No</th>" << std::endl;
            Php::out << "<th>Product Name</th>" << std::endl;								
			Php::out << "<th>Product Type</th>" << std::endl;
			Php::out << "<th>Validity Days</th>" << std::endl;                        
            Php::out << "<th>Valid From</th>" << std::endl;
            Php::out << "<th>Valid To</th>" << std::endl;
            Php::out << "<th></th>" << std::endl;
            Php::out << "<th></th>" << std::endl;
            Php::out << "</tr>" << std::endl;
            Php::out << "</thead>" << std::endl;
			
			int no = 1;
			while (res->next())
				{								
				Php::out << "<tr data-id='"<<res->getString("product_id")<<"'>"<< std::endl;
				Php::out << "<td>"<<no<<"</td>"<< std::endl;
                Php::out << "<td>"<<res->getString("product_name")<<"</td>"<< std::endl;				
				Php::out << "<td>"<<res->getString("product_type")<<"</td>"<< std::endl;
				if(res->getString("product_type")=="Validation")
					{
					if(occuranceOfComma(res->getString("validity_days"))==6)
						Php::out << "<td>All Days</td>"<< std::endl;
					else
						Php::out << "<td>"<<res->getString("validity_days")<<"</td>"<< std::endl;								
					}
				else
					Php::out << "<td>"<<res->getString("product_value")<<"</td>"<< std::endl;
				
                Php::out << "<td>"<<res->getString("start_date")<<"</td>"<< std::endl;				
				Php::out << "<td>"<<res->getString("expiry_date")<<"</td>"<< std::endl;															
				if(res->getInt("status")==1)
					Php::out << "<td><button type='button' class='col btn btn-danger product-enable-disable-btn'>Disable</button></td>"<< std::endl;
				else
					Php::out << "<td><button type='button' class='col btn btn-success product-enable-disable-btn'>Enable</button></td>"<< std::endl;

				Php::out << "<td><button type='button' class='col btn btn-info product-edit'><i class='fas fa-edit'></i>Edit</button></td>"<< std::endl;            	                
                Php::out << "</tr>"<< std::endl;	
				no++;
				}
			//Php::out <<"</table>"<<std::endl;
			}
		delete res;	
        delete stmt;	
		delete con;		
		}
	catch (exception &e)
		{
		writeException("showProductList", e.what());			
		}
	}

void facilityDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        // res=stmt->executeQuery("select * from system_facility");
        // while(res->next())
        //     {
        //     Php::out << "<option value='"<<res->getString("facility_number")<<"'>"<<res->getString("facility_name")<<"</option>" << std::endl;   
        //     }
        res=stmt->executeQuery("select setting_value from system_settings where setting_name in('facility_name','facility_number') order by setting_name desc");
        if(res->next())
            Php::out << "<option value='"<<res->getString("setting_value")<<"'>";
        if(res->next())
            Php::out <<res->getString("setting_value")<<"</option>" << std::endl;
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("facilityDropdown",e.what());
        }
    }

void carparkDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_carparks");
        while(res->next())
            {
            Php::out << "<option value='"<<res->getString("carpark_number")<<"'>"<<res->getString("carpark_name")<<"</option>" << std::endl;   
            }
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("carparkDropdown",e.what());
        }
    }

void discountDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from revenue_discounts");
        while(res->next())
            {
            Php::out << "<option value='"<<res->getString("discount_id")<<"'>"<<res->getString("discount_name")<<"</option>" << std::endl;   
            }
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("carparkDropdown",e.what());
        }
    }

void validationProductDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_products where product_type='Validation'");
        while(res->next())
            {
            Php::out << "<option value='"<<res->getString("product_id")<<"'>"<<res->getString("product_name")<<"</option>" << std::endl;   
            }
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("validationProductDropdown",e.what());
        }
    }

void deviceDropdown(Php::Value json)
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_devices where device_category in ("+ToString(json["type"])+")");
        while(res->next())
            {
            Php::out << "<option value='"<<res->getString("device_number")<<"'>"<<res->getString("device_name")<<"</option>" << std::endl;   
            }
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("deviceDropdown",e.what());
        }
    }

void validatorDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from system_validators");
        while(res->next())
            {
            Php::out << "<option value='"<<res->getString("user_id")<<"'>"<<res->getString("display_name")<<"</option>" << std::endl;   
            }
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("validatorDropdown",e.what());
        }
    }

void parkingRateDropdown()
    {
    try
        {
        con= General.mysqlConnect(ServerDB); 
        stmt=con->createStatement();
        res=stmt->executeQuery("select * from revenue_parking_rate_label");
        if (res->next())
			{
            Php::out << "<option value='parking_rates1'>" << res->getString("parking_rate1") << "</option>" << std::endl;
            Php::out << "<option value='parking_rates2'>" << res->getString("parking_rate2") << "</option>" << std::endl;
            Php::out << "<option value='parking_rates3'>" << res->getString("parking_rate3") << "</option>" << std::endl;
            Php::out << "<option value='parking_rates4'>" << res->getString("parking_rate4") << "</option>" << std::endl;
            Php::out << "<option value='parking_rates5'>" << res->getString("parking_rate5") << "</option>" << std::endl;
			}
       
        delete res;    
        delete stmt;
		delete con; 
        }
    catch(const std::exception& e)
        {
        writeException("parkingRateDropdown",e.what());
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

Php::Value insertUpdateSystemProduct(Php::Value json)
	{
	string msg = "Failed";
	try
		{								
		con= General.mysqlConnect(ServerDB);
		if(ToString(json["id"])=="0")
			prep_stmt = con->prepareStatement("insert into system_products(product_name,start_date,expiry_date,validity_days,validity_time_slots,product_type,product_value,description,carparks,date_validity,validation_type,product_price,facility_number,user_id,status)VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,1)");
		else
			prep_stmt = con->prepareStatement("update system_products set product_name=?,start_date=?,expiry_date=?,validity_days=?,validity_time_slots=?,product_type=?,product_value=?,description=?,carparks=?,date_validity=?,validation_type=?,product_price=?,facility_number=? where product_id=?");
			
		prep_stmt->setString(1, ToString(json["product_name"]));
		if(stoi(json["date_validity"])==1 || ToString(json["product_type"])=="contract_parking")
			{
			prep_stmt->setString(2, ToString(json["start_date"]));
			prep_stmt->setString(3, ToString(json["expiry_date"]));
			}
		else
			{
			prep_stmt->setNull(2,0);
			prep_stmt->setNull(3,0);
			}
		prep_stmt->setString(4, ToString(json["validity_days"]));
		prep_stmt->setString(5, ToString(json["validity_time_slots"]));
		prep_stmt->setString(6, ToString(json["product_type"]));
		prep_stmt->setString(7, ToString(json["product_value"]));
		prep_stmt->setString(8, ToString(json["description"]));
		prep_stmt->setString(9, ToString(json["carparks"]));		
		prep_stmt->setString(10, ToString(json["date_validity"]));		
		prep_stmt->setString(11, ToString(json["validation_type"]));
		prep_stmt->setString(12, ToString(json["product_price"]));					
        prep_stmt->setString(13, ToString(json["facility_number"]));					
		if(ToString(json["id"])=="0")			
			prep_stmt->setString(14, ToString(json["user_id"]));												
		else
			prep_stmt->setString(14, ToString(json["id"]));
		

		if (!prep_stmt->execute())
				msg = "Successfull";	
		delete prep_stmt;
		delete con;
		}
	catch (exception &e)
		{				
		writeException("insertUpdateValidationProduct", e.what());				
		}
	return msg;	
	}

Php::Value parcxSettings(Php::Parameters &params)
	{
	Php::Value data=params[0];      
    int task=data["task"];   
    Php::Value response;
    switch (task)
        {
        case 1:response=insertUpdateFacility(data); 
		    break; 
        case 2:showFacilityList(); 
            break;
        case 3:response=enableDisable("system_facility","facility_id","status",data); 
		    break;  
        case 4:showCarparkList(); 
            break;
        case 5:facilityDropdown(); 
            break; 
        case 6:response=insertUpdateCarpark(data); 
		    break; 
        case 7:response=enableDisable("system_carparks","carpark_id","status",data); 
		    break; 
        case 8:response=getDetails("system_facility","facility_id",data); 
		    break; 
        case 9:response=getDetails("system_carparks","carpark_id",data); 
		    break; 
        case 10:showDeviceList(); 
            break;
        case 11:response=enableDisable("system_devices","id","device_enabled",data); 
		    break;
        case 12:carparkDropdown(); 
            break;
        case 13:parkingRateDropdown(); 
            break;
        case 14:deviceDropdown(data); 
            break;
        case 15:response=insertUpdateDevice(data); 
		    break;
        case 16:response=getDetails("system_devices","id",data); 
		    break; 
        case 17:showSettingList(); 
            break;
        case 18:response=insertUpdateSettings(data); 
		    break;
        case 19:response=enableDisable("system_settings","settings_id","setting_status",data); 
		    break;
        case 20:validatorDropdown();
            break;
        case 21:validationProductDropdown();
            break;
        case 22:showMaintenanceSettingsList();
                break;
        case 23:response=insertUpdateMaintenanceSettings(data); 
                break;
        case 24:response=enableDisable("device_maintenance","id","enabled",data);
                break;
        case 25:showCloudUploadSettings();
                break;
        case 26:response=updateCloudUploadSettings(data); 
                break;
        case 27:response=enableDisable("pxcloud_upload_settings","id","enabled",data);
                break;
        case 28:showCloudDownloadSettings();
                break;
        case 29:response=updateCloudDownloadSettings(data); 
                break;
        case 30:response=enableDisable("pxcloud_download_settings","id","enabled",data);
                break;
        case 31:showProductList();
            break;
        case 32:response=enableDisable("system_products","product_id","status",data);
			break;
        case 33:response=insertUpdateSystemProduct(data);
            break;
        case 34:response=getDetails("system_products","product_id",data); 
		    break;
        case 35:discountDropdown(); 
            break;
		}
	return response;
	}
    
extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_Settings", "1.0");
        extension.add<parcxSettings>("parcxSettings");               
        return extension;
		}
	}
