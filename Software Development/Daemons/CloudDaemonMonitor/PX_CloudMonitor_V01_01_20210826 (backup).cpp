//sudo g++ -std=c++11 -Wall -o PX_CloudMonitor_V01_01_20210826 PX_CloudMonitor_V01_01_20210826.cpp PX_GeneralOperations.cpp -lmysqlcppconn -I/opt/lampp/include/ -L/opt/parcx/lib/
/*
Notes:
1.Add alarm_notifications table with appropriate alarm code-add 100 and 101 codes for upload and download
2.Add facility_emails table
3.Add monitor_upload,monitor_download,monitor_time_interval,upload_notification_sent,download_notification_sent fields in the system_facility table.
4.Add notification_sent field in counters table
5.Add notification_sent field in watchdog_device_alarms
6.Add notification_sent field in watchdog_device_alarms_iot
7.Added multiple email feature(in the column
*/

#include <curl/curl.h>
#include <syslog.h>
#include<sys/stat.h>
#include<sstream>
#include <thread> 
#include <sys/file.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<jsoncpp/json/json.h>
#include "PX_GeneralOperations.h"


#define DATABASE "pxcloud_server"
//#define email_from_address "parcx@alfalakme.com"
//#define email_from_name "Parcx"
string url;
using namespace std;
GeneralOperations general;
int cloud_operator_id;
string carpark_name, facility_name, device_name,email_list;
char delimiter = ',';
int monitor_daemons,monitor_watchdog_alarms,monitor_watchdog_alarms_iot,monitor_occupancy,monitor_handshake,cloud_monitor_interval;
string email_from_address,email_from_name,cloud_monitor_email_subject;
string facility_number;
string webservice_response;
string start_time = "";
string end_time = "";
string start_date = "";
string end_date = "";

struct table_details {
    string table_name;
    string db_name;
    int task;
    int rowlimit;
};



void WriteToLog(string function, string msg) {
    general.writeLog("Daemons/PXCloudMonitor/ApplicationLog_PX_Cloud_Monitor_" + general.currentDateTime("%Y-%m-%d"), function, msg);
}

void WriteException(string function, string msg) {    
    general.writeLog("Daemons/PXCloudMonitor/ExceptionLog_PX_Cloud_Monitor_" + general.currentDateTime("%Y-%m-%d"), function, msg);
    WriteToLog("Exception: "+function,msg);
}

string generateTransactionId()
{    							    		    
   char fmt[64], buf[64];
   struct timeval  tv;
   struct tm       *tm;

   gettimeofday(&tv, NULL);
   tm = localtime(&tv.tv_sec);
   strftime(fmt, sizeof fmt, "%Y%m%d%H%M%S%%06u", tm);
   snprintf(buf, sizeof buf, fmt, tv.tv_usec);
   return buf;
}

string getCarparkName(int carpark_id)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string carpark_name="";
    try{
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select carpark_name from system_carparks where carpark_id = "+to_string(carpark_id));
        if(res->next())
        {
            carpark_name = res->getString("carpark_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(sql::SQLException &e) {
        WriteException("getCarparkName", e.what());
    }
    
    return carpark_name;
}

string getDeviceName(int carpark_id,int device_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string device_name="";
    try{
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select device_name from system_devices where carpark_id = "+to_string(carpark_id)+" and device_number ="+to_string(device_number));
        if(res->next())
        {
            device_name = res->getString("device_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(sql::SQLException &e) {
        WriteException("getDeviceName", e.what());
    }
    
    return device_name;
}



string getFacilityName(int facility_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string facility_name="";
    try{
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select facility_name from system_facility where facility_number = "+to_string(facility_number));
        if(res->next())
        {
            facility_name = res->getString("facility_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(sql::SQLException &e) {
        WriteException("getFacilityName", e.what());
    }
    
    return facility_name;
}

int PushNotification(string transaction_id,string email_to_address,string email_to_name,string email_html_body)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string sql1="",sql2="";
    int result = 0;
    try {
        
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select transaction_id from push_notifications where application_user_id = 0 and email_to_address = '"+email_to_address+"' order by date_time desc limit 1");
        if(res->next())
        {
            sql1 = "Update push_notifications set email_html_body = replace(email_html_body,'</table>','"+email_html_body+"</table>'),updated_date_time=NOW() where transaction_id = '"+res->getString("transaction_id")+"'";

            sql2 = "Update push_notifications_report set email_html_body = replace(email_html_body,'</table>','"+email_html_body+"</table>'),updated_date_time=NOW() where transaction_id = '"+res->getString("transaction_id")+"'";
            //WriteToLog("PushNotification"," SQL"+sql1);
            result = stmt->executeUpdate(sql1);
            stmt->executeUpdate(sql2);
        }
        else
        {        
            email_html_body = "<table border=1 style=\\'border-collapse:collapse\\'><tr><th>Date</th><th>Facility/Carpark</th><th>Device</th><th>Alarm</th></tr>" + email_html_body;
            sql1 = "Insert into push_notifications(transaction_id,application_user_id,date_time,notification_type,email_to_address,email_from_address,email_from_name,email_to_name,email_html_body,email_subject,email_attachment_name) values ('"+transaction_id+"',0,CURRENT_TIMESTAMP(),2,'"+email_to_address+"','"+email_from_address+"','"+email_from_name+"','"+email_to_name+"','"+email_html_body+"</table>','"+cloud_monitor_email_subject+"','')";

            sql2 = "Insert into push_notifications_report(transaction_id,application_user_id,date_time,notification_type,email_to_address,email_from_address,email_from_name,email_to_name,email_html_body,email_subject,email_attachment_name) values ('"+transaction_id+"',0,CURRENT_TIMESTAMP(),2,'"+email_to_address+"','"+email_from_address+"','"+email_from_name+"','"+email_to_name+"','"+email_html_body+"</table>','"+cloud_monitor_email_subject+"','')";
            // WriteToLog("InsertEmailNotification"," SQL"+sql1);

            result = stmt->executeUpdate(sql1);
            stmt->executeUpdate(sql2);
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("PushNotification", e.what());
    }
    return result;
}

std::vector<std::string> split(const std::string& s, char delimiter)                                                                                                                     
{                                                                                                                                                                                            
   std::vector<std::string> splits;                                                                                                                                                           
   std::string split;                                                                                                                                                                         
   std::istringstream ss(s);                                                                                                                                                                  
   while (std::getline(ss, split, delimiter))                                                                                                                                            
   {                                                                                                                                                                                          
      splits.push_back(split);                                                                                                                                                                
   }                                                                                                                                                                                          
   return splits;                                                                                                                                                                             
}

void MonitorCloudDaemons()
{
    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res,*res1;
    string transaction_id,email_subject,email_text,email_body_html;
    string designation = "";
    
    int notify_operator=0,notify_technical=0,notify_parcx = 0,result = 0;
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select * FROM system_facility where monitor_upload=1 and upload_notification_sent = 0 and last_upload_datetime< DATE_SUB(NOW(), INTERVAL monitor_time_interval HOUR)");
        if(res->rowsCount()>0)
        {
            res1 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='100'");
            if(res1->next())
            {
                notify_operator = res1->getInt("notify_operator");
                notify_technical = res1->getInt("notify_technical");
                notify_parcx = res1->getInt("notify_parcx");
                email_subject = res1->getString("alarm");
                email_text = res1->getString("email_text");
            }
            delete res1;

            while (res->next()) { 
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"' and status = 1");
                if(res1->rowsCount()>0)
                {
                    while(res1->next())
                    {
                        designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                            
                            email_list = res1->getString("email_id");
                            auto list = split(email_list,delimiter);
                            for(auto email:list)
                            {
                                transaction_id = generateTransactionId();
                                email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("facility_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                                result = PushNotification(transaction_id,email,res1->getString("name"),email_body_html);
                                if(result>0)
                                {
                                  WriteToLog("MonitorCloudDaemons","Cloud Upload Daemon Notification added in the Push Notifications table");
                                  stmt->executeUpdate("Update system_facility set upload_notification_sent = 1 where facility_number = '"+res->getString("facility_number")+"'");
                                }  
                            }
                        }

                    }
                }          
                delete res1;
            }
        }
        

        res = stmt->executeQuery("Select * FROM system_facility where monitor_download=1 and download_notification_sent=0 and last_download_datetime< DATE_SUB(NOW(), INTERVAL monitor_time_interval HOUR)");
        if(res->rowsCount()>0)
        {
            res1 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='101'");
            if(res1->next())
            {
                notify_operator = res1->getInt("notify_operator");
                notify_technical = res1->getInt("notify_technical");
                notify_parcx = res1->getInt("notify_parcx");
                email_subject = res1->getString("alarm");
                email_text = res1->getString("email_text");
            }
            delete res1;

            while (res->next()) { 
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"' and status = 1");
                if(res1->rowsCount()>0)
                {
                     while(res1->next())
                     {
                         designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                            
                            email_list = res1->getString("email_id");
                            auto list = split(email_list,delimiter);
                            for(auto email:list)
                            {
                                transaction_id = generateTransactionId();
                                email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("facility_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                                result = PushNotification(transaction_id,email,res1->getString("name"),email_body_html);
                                //WriteToLog("Download","result="+to_string(result));
                                if(result>0)
                                {
                                  WriteToLog("MonitorCloudDaemons","Cloud Download Daemon Notification added in the Push Notifications table");
                                  stmt->executeUpdate("Update system_facility set download_notification_sent = 1 where facility_number = '"+res->getString("facility_number")+"'");
                                }                             
                            }
                            
                        }

                     }
                }          
                delete res1;
            }
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("MonitorCloudDaemons", e.what());
    }
}

void NotifyWatchdogAlarms()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res,*res1;
    int notify_operator=0,notify_technical=0,notify_parcx = 0,result=0;
    string transaction_id,email_subject,email_text,email_body_html;
    string designation = "";
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select * FROM watchdog_device_alarms where dismiss=0 and notification_sent = 0");
        while(res->next())
        {
            notify_operator=0;
            notify_technical=0;
            notify_parcx = 0;
            res1 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='"+res->getString("alarm_code")+"'");
            if(res1->next())
            {
                notify_operator = res1->getInt("notify_operator");
                notify_technical = res1->getInt("notify_technical");
                notify_parcx = res1->getInt("notify_parcx");
                email_subject = res1->getString("alarm");
                email_text = res1->getString("email_text");
            }
            delete res1;
            
            res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"' and status=1");
            if(res1->rowsCount()>0)
            {
                while(res1->next())
                {
                    designation=res1->getString("designation");
                    if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                    {

                        email_list = res1->getString("email_id");
                        auto list = split(email_list,delimiter);
                        for(auto email:list)
                        {
                            transaction_id = generateTransactionId();
                            email_body_html = "<tr><td>"+res->getString("alarm_date_time")+"</td><td>"+res->getString("carpark_name")+"</td><td>"+res->getString("device_name")+"</td><td>"+email_text+"</td></tr>";
                            result =PushNotification(transaction_id,email,res1->getString("name"),email_body_html);
                            if(result>0)
                            {
                              WriteToLog("NotifyWatchdogAlarms","Watchdog device alarms notification added in the Push Notifications table");
                              stmt->executeUpdate("Update watchdog_device_alarms set notification_sent = 1 where id = "+res->getString("id"));
                            }
                        }                   
                    }
                }
            }          
            delete res1;
            
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("NotifyWatchdogAlarms", e.what());
    }

}

void MonitorOccupancy()
{
    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res,*res1;
    string transaction_id,email_subject,email_text,email_body_html;
    string designation = "",counter_type;
    int notify_operator=0,notify_technical=0,notify_parcx = 0,result = 0;
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select * FROM counters where current_level>=occupancy_threshold and counter_type=1 and notification_sent=0");
        if(res->rowsCount()>0)
        {
            res1 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='102'");
            if(res1->next())
            {
                notify_operator = res1->getInt("notify_operator");
                notify_technical = res1->getInt("notify_technical");
                notify_parcx = res1->getInt("notify_parcx");
                email_subject = res1->getString("alarm");
                email_text = res1->getString("email_text");
            }
            delete res1;

            while (res->next()) { 
                
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"' and status=1");
                if(res1->rowsCount()>0)
                {
                     while(res1->next())
                     {
                        designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                            email_list = res1->getString("email_id");
                            auto list = split(email_list,delimiter);
                            for(auto email:list)
                            {
                                transaction_id = generateTransactionId();
                                email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("carpark_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                                result = PushNotification(transaction_id,email,res1->getString("name"),email_body_html);
                                if(result>0)
                                {
                                  WriteToLog("MonitorOccupancy","Counters Notification added in the Push Notifications table");
                                  stmt->executeUpdate("Update counters set notification_sent = 1 where counter_type=1 and facility_number = '"+res->getString("facility_number")+"'");
                                }
                            }
                            
                        }
                     }
                }          
                delete res1;
            }
        }
        
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("MonitorOccupancy", e.what());
    }
}


void NotifyWatchdogAlarmsIOT()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res,*res1;
    int notify_operator=0,notify_technical=0,notify_parcx = 0,result=0;
    string transaction_id,email_subject,email_text,email_body_html;
    string designation = "";
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select * FROM watchdog_device_alarms_iot where dismiss=0 and notification_sent = 0");
        while(res->next())
        {
            notify_operator=0;
            notify_technical=0;
            notify_parcx = 0;
            res1 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='"+res->getString("alarm_code")+"'");
            if(res1->next())
            {
                notify_operator = res1->getInt("notify_operator");
                notify_technical = res1->getInt("notify_technical");
                notify_parcx = res1->getInt("notify_parcx");
                email_subject = res1->getString("alarm");
                email_text = res1->getString("email_text");
            }
            delete res1;
            
            res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"' and status=1");
            if(res1->rowsCount()>0)
            {
                while(res1->next())
                {
                    designation=res1->getString("designation");
                    if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                    {

                        email_list = res1->getString("email_id");
                        auto list = split(email_list,delimiter);
                        for(auto email:list)
                        {
                            transaction_id = generateTransactionId();
                            email_body_html = "<tr><td>"+res->getString("alarm_date_time")+"</td><td>"+res->getString("carpark_name")+"</td><td>"+res->getString("device_name")+"</td><td>"+email_text+"</td></tr>";
                            result =PushNotification(transaction_id,email,res1->getString("name"),email_body_html);
                            if(result>0)
                            {
                              WriteToLog("NotifyWatchdogAlarmsIOT","Watchdog device alarms notification added in the Push Notifications table");
                              stmt->executeUpdate("Update watchdog_device_alarms_iot set notification_sent = 1 where id = "+res->getString("id"));
                            }
                        }
                        
                    }
                }
            }          
            delete res1;
            
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("NotifyWatchdogAlarmsIOT", e.what());
    }

}

void MonitorHandshake()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res,*res1,*res2;
    string transaction_id,email_subject,email_text,email_body_html;
    string designation = "";
    int notify_operator=0,notify_technical=0,notify_parcx = 0,result = 0,monitor_interval=1;
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select distinct(a.device_number),a.facility_number from handshake_iot as a join system_devices_iot as b on a.device_number = b.device_number and a.facility_number = b.facility_number where b.device_enabled = 1");
        if(res->rowsCount()>0)
        {
            while(res->next())
            {
                res1 = stmt->executeQuery("SELECT * FROM handshake_iot where device_number = "+res->getString("device_number")+" and facility_number="+res->getString("facility_number")+" order by date_time desc limit 1");
                if(res1->next())
                {
                    if(res1->getInt("notification_sent")==0)
                    {
                        int *diff = general.differenceDateTime(general.currentDateTime("%Y-%m-%d %H:%M:%S"),res1->getString("date_time"),"%Y-%m-%d %H:%M:%S");
                        res2 = stmt->executeQuery("Select server_handshake_interval from system_devices_iot where device_number = "+res->getString("device_number")+" and facility_number="+res->getString("facility_number"));
                        if(res2->next())
                        {
                            monitor_interval = res2->getInt("server_handshake_interval");//in minutes
                        }
                        else
                        {
                            monitor_interval = 30;
                        }
                        delete res2;
                        

                        monitor_interval = monitor_interval+2; //Add 2 min delay to send handshake notification
                        if(diff[0]>monitor_interval * 60)
                        {

                            res2 = stmt->executeQuery("Select * from alarm_notifications where alarm_code ='103'");
                            if(res2->next())
                            {
                                notify_operator = res2->getInt("notify_operator");
                                notify_technical = res2->getInt("notify_technical");
                                notify_parcx = res2->getInt("notify_parcx");
                                email_subject = res2->getString("alarm");
                                email_text = res2->getString("email_text");
                            }
                            delete res2;

                            res2 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res1->getString("facility_number")+"' and status=1");
                            if(res2->rowsCount()>0)
                            {
                                while(res2->next())
                                {
                                    designation=res2->getString("designation");
                                    if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                                    {
                                        email_list = res2->getString("email_id");
                                        auto list = split(email_list,delimiter);
                                        for(auto email:list)
                                        {
                                            transaction_id = generateTransactionId();
                                            email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+getFacilityName(res1->getInt("facility_number"))+"</td><td>"+res1->getString("device_name")+"</td><td>"+email_text+"</td></tr>";
                                            result = PushNotification(transaction_id,email,res2->getString("name"),email_body_html);
                                            if(result>0)
                                            {
                                              WriteToLog("MonitorHandshake","Handshake Notification added in the Push Notifications table");
                                              stmt->executeUpdate("Update handshake_iot set notification_sent = 1 where id = "+res1->getString("id"));
                                            }
                                        }
                                    }
                                }
                            }          
                            delete res2;
                        }

                    }
                    delete res1;
                }

            }

        }
        
        delete res;
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("MonitorCloudDaemons", e.what());
    }
}

void ResetNotificationFlag()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        //Update Cloud Upload/Download Daemon notification flag
        stmt->executeUpdate("Update system_facility set upload_notification_sent =0 where monitor_upload=1 and last_upload_datetime> DATE_SUB(NOW(), INTERVAL monitor_time_interval HOUR)");
        stmt->executeUpdate("Update system_facility set download_notification_sent =0 where monitor_download=1 and last_download_datetime> DATE_SUB(NOW(), INTERVAL monitor_time_interval HOUR)");
        //Update occupancy threshold notification flag 
        stmt->executeUpdate("Update counters set notification_sent=0 where current_level<occupancy_threshold and counter_type=1");               
        delete stmt;
        delete conn;
    }
    catch (sql::SQLException &e) {
        WriteException("ResetNotificationFlag", e.what());
    }
}

void UpdatePID(string pid) {
    sql::Connection *conn;
    sql::Statement *stmt;
    
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        stmt->executeUpdate("Update daemon_status set pid = " + pid + ",start_time=CURRENT_TIMESTAMP() where daemon_name like 'PX_CloudMonitor%'");                
        delete stmt;
        delete conn;        
    } catch (sql::SQLException &e) {
        WriteException("UpdatePID", e.what());
    }
}

void GetSettings()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    try 
    {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
		//WriteToLog("GetSSHSettings","host:"+ssh_host);
        res  = stmt->executeQuery("Select setting_name,setting_value from cloud_settings where status = 1 and setting_name in ('monitor_daemons','monitor_watchdog_alarms','monitor_watchdog_alarms_iot','monitor_occupancy','monitor_handshake','cloud_monitor_interval','email_from_address','email_from_name','cloud_monitor_email_subject')");
        while(res->next())
        {
            if(res->getString("setting_name")=="monitor_daemons")
                monitor_daemons = res->getInt("setting_value");
            else if(res->getString("setting_name")=="monitor_watchdog_alarms")
                monitor_watchdog_alarms = res->getInt("setting_value");
            else if(res->getString("setting_name")=="monitor_watchdog_alarms_iot")
                monitor_watchdog_alarms_iot = res->getInt("setting_value");
            else if(res->getString("setting_name")=="monitor_occupancy")
                monitor_occupancy = res->getInt("setting_value");
            else if(res->getString("setting_name")=="monitor_handshake")
                monitor_handshake = res->getInt("setting_value");
            else if(res->getString("setting_name")=="cloud_monitor_interval")
                cloud_monitor_interval = res->getInt("setting_value");
            else if(res->getString("setting_name")=="email_from_address")
                email_from_address = res->getString("setting_value");
            else if(res->getString("setting_name")=="email_from_name")
                email_from_name = res->getString("setting_value");
            else if(res->getString("setting_name")=="cloud_monitor_email_subject")
                cloud_monitor_email_subject = res->getString("setting_value");
                
        }

        
        delete res;
        delete stmt;
        delete conn;

        WriteToLog("monitor_daemons",to_string(monitor_daemons));
        WriteToLog("monitor_watchdog_alarms",to_string(monitor_watchdog_alarms));
	WriteToLog("monitor_watchdog_alarms_iot",to_string(monitor_watchdog_alarms_iot));
        WriteToLog("monitor_occupancy",to_string(monitor_occupancy));
	WriteToLog("monitor_handshake",to_string(monitor_handshake));
        WriteToLog("cloud_monitor_interval",to_string(cloud_monitor_interval));
        WriteToLog("email_from_address",email_from_address);
        WriteToLog("email_from_name",email_from_name);
        WriteToLog("cloud_monitor_email_subject",cloud_monitor_email_subject);
        
    }
    catch (sql::SQLException &e) {
        WriteException("GetSettings", e.what());
    }
}

int main(void)
{
    
    string proccess_id;
    int pid_file = open("px_cloud_monitor_daemon.lock", O_CREAT | O_RDWR, 0644);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if (rc) 
        {
        if (EWOULDBLOCK == errno) 
            {
            WriteToLog("Main: ", "Another Instance is running");
            cout << "Another Instance is running" << endl; // another instance is running
            }
        } 
    else 
        {
        pid_t pid,sid;

        pid = fork();
        proccess_id=to_string(pid);
        if(pid > 0)
        {
            WriteToLog("Main: ", "Cloud Monitor Daemon Version : PX_CloudMonitor_V01_01_20210826    Process Id:" + proccess_id);
            cout << "Cloud Monitor Daemon Version : PX_CloudMonitor_V01_01_20210826" << "Process Id :" << proccess_id << endl; 
            UpdatePID(proccess_id);
            exit(EXIT_SUCCESS);
        }
        else if(pid < 0)
        {
            exit(EXIT_FAILURE);
        }
        umask(0);
        openlog("daemon-named",LOG_NOWAIT|LOG_PID,LOG_USER);
        syslog(LOG_NOTICE,"Successfully started Notification Service");
        WriteToLog("","Successfully started Notification Service");
        sid = setsid();
        if(sid<0)
        {
            exit(EXIT_FAILURE);
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        
        WriteToLog("","******************************************Log******************************************"); 
        GetSettings();
        const int SLEEP_INTERVAL = cloud_monitor_interval;
        while(1)
        {   
           ResetNotificationFlag();
           if(monitor_daemons==1)
                MonitorCloudDaemons();
           if(monitor_watchdog_alarms ==1) 
                NotifyWatchdogAlarms();
           if(monitor_watchdog_alarms_iot ==1)
                NotifyWatchdogAlarmsIOT();
           if(monitor_occupancy==1)
                MonitorOccupancy();
           if(monitor_handshake==1)
                MonitorHandshake();
           sleep(SLEEP_INTERVAL);
        }
    }
    return 0;
}
