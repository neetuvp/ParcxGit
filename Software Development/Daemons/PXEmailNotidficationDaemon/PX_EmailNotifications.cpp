//  sudo g++ -Wall  -std=c++11 -fpic -o PX_EmailNotifications_V01_01_20210826  PX_EmailNotifications.cpp PX_GeneralOperations.cpp -lcurl -lmysqlcppconn -ljsoncpp -I/opt/lampp/include/ -L/opt/parcx/lib/


//1.Removed header tag- not updated in production
//2.When from_address is empty,take it from the settings table  - not updated in production

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
#include <thread> 
#include <sys/file.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "PX_GeneralOperations.h"

#include<jsoncpp/json/json.h>


#define DATABASE "pxcloud_server"
//#define LOGO_URL "http://65.1.116.38/parcx.png"
string logo_url="http://65.1.116.38/parcx.png";
string email_from_address,email_from_name,aws_path;
int email_notification_interval=60;
using namespace std;
char buf[80];
GeneralOperations general;
sql::Connection *conn;
sql::Driver *driver;
void WriteToLog(string function, string msg) {
    general.writeLog("Daemons/PXEmailNotification/ApplicationLog_" + general.currentDateTime("%Y-%m-%d"), function, msg);
}

void WriteException(string function, string msg) {    
    general.writeLog("Daemons/PXEmailNotification/ExceptionLog_" + general.currentDateTime("%Y-%m-%d"), function, msg);
    WriteToLog("Exception: "+function,msg);
}


/*sql::Connection* DBConnection(string db)
{
    try{
        driver = get_driver_instance();
        conn = driver->connect(DBServer,DBUsername,DBPassword);  
        conn->setSchema(db);
        
    }
    catch(exception &e)
    {
        writeException("DBConnection",e.what());
    }
    return conn;
    
}*/



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}



void EmailNotifications()
{
    //string url = "http://localhost//aws_email/php_mail.php";
    string url = aws_path;
    Json::Value json;
    Json::Value request;
    Json::Value jsonid;
    Json::FastWriter fw;
    Json::Reader reader;
    bool parse=false;
    Json::Value data;
    int id= 0;
    string transaction_id="",device_id="";
    string deviceID="";
    int n=0;
    try
    {
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *rs;

        sql::Connection *con_write;
        sql::Statement *stmt_write;
        string email_header="",email_footer="",email_content;
        con = general.mysqlConnect(DATABASE);
        stmt = con->createStatement();
        string sql = "select * from push_notifications where notification_type = 2 and email_to_address>'' limit 15";
        //WriteToLog(sql);
        rs = stmt->executeQuery(sql);
        if(rs->rowsCount()>0)
        {
            //string encoded_image =  base64encode("parcx.png");
            while(rs->next())
            {
                try{
                    email_footer = "";
                    if(rs->getString("email_from_address")!="")
                    {
                        json["email_from_address"]=rs->getString("email_from_address").asStdString();
                        json["email_from_name"]=rs->getString("email_from_name").asStdString();
                    }
                    else
                    {
                        json["email_from_address"]=email_from_address;
                        json["email_from_name"]=email_from_name;
                    }

                    json["email_to_address"]=rs->getString("email_to_address").asStdString();					
                    json["email_to_name"]=rs->getString("email_to_name").asStdString();		
                    json["email_subject"]=rs->getString("email_subject").asStdString();
                    //email_header = "<p>Dear "+rs->getString("email_to_name").asStdString()+",</p>";
                    email_content ="<p>"+rs->getString("email_html_body").asStdString()+"</p>";
                   // email_footer = "<p>"+rs->getString("email_from_name").asStdString()+"</p>";

                    email_footer = email_footer +"<p><img src = '"+logo_url+"' width='80' height='40'></p>";
                    WriteToLog("EmailNotifications",email_header + email_content + email_footer);
                    json["email_body_html"]=email_header + email_content + email_footer;					
                    json["email_body_plain_text"]="";
                    if(rs->getString("email_html_body")!="")
                    {
                        json["email_attachment_included"]="0";					
                    }
                    else
                    {
                        json["email_attachment_included"]="1";	
                        json["email_attachment_path"]="";
                        json["email_attachment_filename"]=rs->getString("email_attachment_name").asStdString();
                    }

                    json["transaction_id"] = rs->getString("transaction_id").asStdString();
                    id = rs->getInt("notification_id");

                    string jsonstring = fw.write(json);
                    WriteToLog("EmailNotifications","Email Notification Request:"+jsonstring);
                    WriteToLog("EmailNotifications","Ids:"+to_string(id));
                    if(!jsonstring.empty())
                    {
                        string readBuffer="";
                        CURL *curl;
                        CURLcode res;
                        struct curl_slist *headers=NULL;
                        headers = curl_slist_append(headers, "Accept-encoding:gzip,deflate");
                        headers = curl_slist_append(headers, "Accept:application/json");
                        headers = curl_slist_append(headers, "Content-Type:application/json");
                   // headers = curl_slist_append(headers, "Username:external");

                        curl = curl_easy_init();
                        if(curl) 
                        {
                            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                            //curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
                            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); 
                            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstring.c_str());
                            curl_easy_setopt(curl, CURLOPT_ENCODING ,"");
                            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);            
                            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);            
                            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
                            res = curl_easy_perform(curl);
                            if(res==CURLE_OK)
                            {
                               WriteToLog("EmailNotifications","Push Notification Curl Successful");
                               WriteToLog("EmailNotifications","Response:"+readBuffer);
                               parse = reader.parse(readBuffer,json,true);
                               if(parse)
                               {
                                    if(json["status_code"]==200)
                                    {
                                        transaction_id = json["transaction_id"].asString();
                                        con_write = general.mysqlConnect(DATABASE);
                                        stmt_write = con_write->createStatement();

                                        n = stmt_write->executeUpdate("Update push_notifications_report set notification_sent_datetime = CURRENT_TIMESTAMP() where transaction_id ='"+transaction_id+"' and notification_type=2");
                                        delete stmt_write;
                                        delete con_write;

                                        if(n>0)
                                        {
                                                con_write = general.mysqlConnect(DATABASE);
                                                stmt_write = con_write->createStatement();
                                                stmt_write->executeUpdate("Delete from push_notifications where notification_id ="+to_string(id));
                                                delete stmt_write;
                                                delete con_write;
                                        }

                                    }


                               }
                               else{
                                    WriteToLog("EmailNotifications","Email Notification Parse Error");
                                    WriteException("EmailNotifications","Email Notification Parse Error");
                                    WriteException("EmailNotifications","Response:"+readBuffer);
                               }
                            }
                            else{
                                cout<<"Curl failed:"<<res<<endl;
                                WriteToLog("EmailNotifications","Email Notification Curl Failed");
                                WriteException("EmailNotifications","Email Notification Curl Failed");
                            }
                            curl_easy_cleanup(curl);
                        }
                    }	
                }
                catch(const std::exception& e)
                {
                        WriteToLog("EmailNotifications",e.what());
                        WriteException("EmailNotifications", e.what());
                }

            }

        }
        else{
                //WriteToLog("EmailNotifications","No Rows Found");
        }
        delete rs;
        delete stmt;
        delete con;
    }
    catch(const std::exception& e)
    {
            WriteToLog("EmailNotifications",e.what());
            WriteException("EmailNotifications", e.what());
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
	//	WriteToLog("Query","Select setting_name,setting_value from cloud_settings where status = 1 and setting_name = 'email_logo_url'");
        res  = stmt->executeQuery("Select setting_name,setting_value from cloud_settings where status = 1 and setting_name in ('email_logo_url','email_from_address','email_from_name','email_notification_interval','aws_path')");
        while(res->next())
        {
            if(res->getString("setting_name")=="email_logo_url"){
                logo_url = res->getString("setting_value");      
                	//WriteToLog("logo",logo_url);
                }
            if(res->getString("setting_name")=="email_from_address"){
                email_from_address = res->getString("setting_value");      
                	//WriteToLog("logo",logo_url);
                }
            if(res->getString("setting_name")=="email_from_name"){
                email_from_name = res->getString("setting_value");      
                	//WriteToLog("logo",logo_url);
                }
            if(res->getString("setting_name")=="email_notification_interval"){
                email_notification_interval = res->getInt("setting_value");      
                	//WriteToLog("logo",logo_url);
                }
            if(res->getString("setting_name")=="aws_path"){
                aws_path = res->getString("setting_value");      
                	//WriteToLog("logo",logo_url);
                }
                
        }

        
        delete res;
        delete stmt;
        delete conn;

        WriteToLog("email_logo_url",logo_url);
        
    }
    catch (sql::SQLException &e) {
        WriteException("GetSettings", e.what());
    }
}

void UpdatePID(string pid) {
    sql::Connection *conn;
    sql::Statement *stmt;
    
    try {
        conn = general.mysqlConnect(DATABASE);
        stmt = conn->createStatement();
        stmt->executeUpdate("Update daemon_status set pid = " + pid + ",start_time=CURRENT_TIMESTAMP() where daemon_name like 'PX_EmailNotifications%'");                
        delete stmt;
        delete conn;        
    } catch (sql::SQLException &e) {
        WriteException("UpdatePID", e.what());
    }
} 

int main(void)
{
    
    string proccess_id;
    int pid_file = open("px_email_daemon.lock", O_CREAT | O_RDWR, 0644);
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
            WriteToLog("Main: ", "Email Notification Daemon Version : PX_EmailNotifications_V01_01_20210826    Process Id:" + proccess_id);
            cout << "Email Notification Daemon Version : PX_EmailNotifications_V01_01_20210826" << "Process Id :" << proccess_id << endl; 
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

        const int SLEEP_INTERVAL = email_notification_interval;
        WriteToLog("","******************************************Log******************************************"); 
        GetSettings();
        while(1)
        {
            //WriteToLog("******************************************Log******************************************");       
            EmailNotifications();
            sleep(SLEEP_INTERVAL);
        }
    }
    return 0;
}
