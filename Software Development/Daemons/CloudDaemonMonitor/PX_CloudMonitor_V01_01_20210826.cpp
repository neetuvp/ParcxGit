//sudo g++ -std=c++11 -Wall -o PX_CloudMonitor_V01_01_20210826 PX_CloudMonitor_V01_01_20210826.cpp PX_GeneralOperations.cpp -lmysqlcppconn -I/opt/lampp/include/ -L/opt/parcx/lib/
/*
Notes:
1.Add alarm_notifications table with appropriate alarm code-add 100 and 101 codes for upload and download
2.Add facility_emails table
3.Add monitor_upload,monitor_download,monitor_time_interval,upload_notification_sent,download_notification_sent fields in the system_facility table.
4.Add notification_sent field in counters table
5.Add notification_sent field in watchdog_device_alarms
6.Add notification_sent field in watchdog_device_alarms_iot
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
#define email_from_address "parcx@alfalakme.com"
#define email_from_name "Parcx"
string url;
using namespace std;
GeneralOperations general;
int cloud_operator_id;
string carpark_name, facility_name, device_name;

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
            WriteToLog("PushNotification"," SQL"+sql1);
            result = stmt->executeUpdate(sql1);
            stmt->executeUpdate(sql2);
        }
        else
        {        
            email_html_body = "<table border=1><tr><th>Date</th><th>Facility/Carpark</th><th>Device</th><th>Alarm</th></tr>" + email_html_body;
            sql1 = "Insert into push_notifications(transaction_id,application_user_id,date_time,notification_type,email_to_address,email_from_address,email_from_name,email_to_name,email_html_body,email_subject,email_attachment_name) values ('"+transaction_id+"',0,CURRENT_TIMESTAMP(),2,'"+email_to_address+"','"+email_from_address+"','"+email_from_name+"','"+email_to_name+"','"+email_html_body+"</table>','Important Notice','')";

            sql2 = "Insert into push_notifications_report(transaction_id,application_user_id,date_time,notification_type,email_to_address,email_from_address,email_from_name,email_to_name,email_html_body,email_subject,email_attachment_name) values ('"+transaction_id+"',0,CURRENT_TIMESTAMP(),2,'"+email_to_address+"','"+email_from_address+"','"+email_from_name+"','"+email_to_name+"','"+email_html_body+"</table>','Important Notice','')";
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
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"'");
                if(res1->rowsCount()>0)
                {
                     while(res1->next())
                     {
                         designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                          transaction_id = generateTransactionId();
                          email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("facility_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                          result = PushNotification(transaction_id,res1->getString("email_id"),res1->getString("name"),email_body_html);
                          if(result>0)
                          {
                            WriteToLog("MonitorCloudDaemons","Cloud Upload Daemon Notification added in the Push Notifications table");
                            stmt->executeUpdate("Update system_facility set upload_notification_sent = 1 where facility_number = '"+res->getString("facility_number")+"'");
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
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"'");
                if(res1->rowsCount()>0)
                {
                     while(res1->next())
                     {
                         designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                          transaction_id = generateTransactionId();
                          email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("facility_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                          result = PushNotification(transaction_id,res1->getString("email_id"),res1->getString("name"),email_body_html);
                          //WriteToLog("Download","result="+to_string(result));
                          if(result>0)
                          {
                            WriteToLog("MonitorCloudDaemons","Cloud Download Daemon Notification added in the Push Notifications table");
                            stmt->executeUpdate("Update system_facility set download_notification_sent = 1 where facility_number = '"+res->getString("facility_number")+"'");
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
            
            res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"'");
            if(res1->rowsCount()>0)
            {
                while(res1->next())
                {
                    designation=res1->getString("designation");
                    if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                    {
                        transaction_id = generateTransactionId();
                        email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+getCarparkName(res->getInt("carpark_id"))+"</td><td>"+getDeviceName(res->getInt("carpark_id"),res->getInt("device_number"))+"</td><td>"+email_text+"</td></tr>";
                        result =PushNotification(transaction_id,res1->getString("email_id"),res1->getString("name"),email_body_html);
                        if(result>0)
                        {
                          WriteToLog("NotifyWatchdogAlarms","Watchdog device alarms notification added in the Push Notifications table");
                          stmt->executeUpdate("Update watchdog_device_alarms set notification_sent = 1 where id = "+res->getString("id"));
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
                
                res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"'");
                if(res1->rowsCount()>0)
                {
                     while(res1->next())
                     {
                        designation=res1->getString("designation");
                        if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                        {
                            
                            transaction_id = generateTransactionId();
                            email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+res->getString("carpark_name")+"</td><td></td><td>"+email_text+"</td></tr>";
                            result = PushNotification(transaction_id,res1->getString("email_id"),res1->getString("name"),email_body_html);
                            if(result>0)
                            {
                              WriteToLog("MonitorOccupancy","Counters Notification added in the Push Notifications table");
                              stmt->executeUpdate("Update counters set notification_sent = 1 where counter_type=1 and facility_number = '"+res->getString("facility_number")+"'");
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
            
            res1 = stmt->executeQuery("Select * from facility_emails where facility_number = '"+res->getString("facility_number")+"'");
            if(res1->rowsCount()>0)
            {
                while(res1->next())
                {
                    designation=res1->getString("designation");
                    if((designation=="operator" && notify_operator==1)||(designation=="technical" && notify_technical==1)||(designation=="parcx" && notify_parcx==1))
                    {
                        transaction_id = generateTransactionId();
                        email_body_html = "<tr><td>"+general.currentDateTime("%d-%m-%Y %H:%M:%S")+"</td><td>"+getCarparkName(res->getInt("carpark_id"))+"</td><td>"+getDeviceName(res->getInt("carpark_id"),res->getInt("device_number"))+"</td><td>"+email_text+"</td></tr>";
                        result =PushNotification(transaction_id,res1->getString("email_id"),res1->getString("name"),email_body_html);
                        if(result>0)
                        {
                          WriteToLog("NotifyWatchdogAlarmsIOT","Watchdog device alarms notification added in the Push Notifications table");
                          stmt->executeUpdate("Update watchdog_device_alarms_iot set notification_sent = 1 where id = "+res->getString("id"));
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

        const int SLEEP_INTERVAL = 30;
        WriteToLog("","******************************************Log******************************************"); 
        while(1)
        {   
           ResetNotificationFlag();
           MonitorCloudDaemons();
           NotifyWatchdogAlarms();
           NotifyWatchdogAlarmsIOT();
           MonitorOccupancy();
           sleep(SLEEP_INTERVAL);
        }
    }
    return 0;
}
