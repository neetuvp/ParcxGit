//CURL-OPENSSL Required to access https  : sudo apt-get install libcurl4-openssl-dev
//JSON.h install :   sudo apt-get install libjsoncpp-dev
//sudo g++ -Wall -o ParkingRevenues  ParkingRevenues.cpp -L/usr/lib -lcurl -ljsoncpp -lmysqlcppconn
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
#include <mysql/mysql.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

#include<jsoncpp/json/json.h>
#define DBServer "DBServer"
#define DBUsername "parcx"
#define DBPassword "Parcx123!"
#define DATABASE_REPORTING "parcx_reporting"
#define DATABASE_SERVER "parcx_server"
#define EAI_URL "http://192.168.1.202:8080/test.php"
using namespace std;

sql::Connection *conn;
sql::Driver *driver;
string currentDateTime()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);		
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
    std::strftime(buffer,80,"%d-%m-%Y",timeinfo);		
    string date(buffer);
    return date;
}
void WriteToLog(string msg)
{
    ofstream myfile;
myfile.open("Logs/Signage"+currentDate()+".log",fstream::app);

    //myfile.open("/var/log/parcx/Logs/Daemons/ApplicationLogs/ParkAssist"+currentDate()+".log",fstream::app);
    //myfile<<"Function--> "+function<<endl;
    //myfile<<"Message --> "+msg<<endl;
    myfile<<"Date : "<<currentDateTime()+"   "<<msg<<endl;

    //myfile<<"--------------------------------------------------------------------\n";
    myfile.close();
}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
/*static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    return size * nitems;
}*/
static size_t OnReceiveData (void * pData, size_t tSize, size_t tCount, void * pmUser)
{
    size_t length = tSize * tCount, index = 0;
    while (index < length)
    {
        unsigned char *temp = (unsigned char *)pData + index;
        if ((temp[0] == '\r') || (temp[0] == '\n'))
            break;
        index++;
    }

    std::string str((unsigned char*)pData, (unsigned char*)pData + index);
    std::map<std::string, std::string>* pmHeader = (std::map<std::string, std::string>*)pmUser;
    size_t pos = str.find(": ");
    if (pos != std::string::npos)
        pmHeader->insert(std::pair<std::string, std::string> (str.substr(0, pos), str.substr(pos + 2)));

    return (tCount);
}
sql::Connection* DBConnection(string db)
{
  driver = get_driver_instance();
  conn = driver->connect(DBServer,DBUsername,DBPassword);
  conn->setSchema(db);
  return conn;
}

void CallEAIParkingRevenues(string url)
{
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::Value request(Json::arrayValue);
    Json::Value revenue;
    string readBuffer,status="0";
    std::map<std::string,std::string> headerBuffer;
    string device_id,creation_datetime,reception_datetime,descr;
    
    /*conn = DBConnection(DATABASE_REPORTING);
    stmt = conn->createStatement();
    rs=stmt->executeQuery("Select device_number,alarm_description,Date_Format(alarm_date_time,'%Y-%m-%dT%TZ') as alarm_date_time,Date_Format(create_date_time,'%Y-%m-%dT%TZ') as create_date_time from watchdog_device_alarms where EAI_status = 0 order by id");
    while(rs->next())
    {*/
        revenue["amount"] = "20000.00";
        revenue["currency"]="AED";
        revenue["revenueType"]="card",
        revenue["parkingId"]="P1";
        revenue["paymentTerminalId"] = "22";
        revenue["date"] = currentDate();

        request.append(revenue);

        revenue["amount"] = "10000.00";
        revenue["currency"]="AED";
        revenue["revenueType"]="cash",
        revenue["parkingId"]="P1";
        revenue["paymentTerminalId"] = "22";
        revenue["date"] = currentDate();

        request.append(revenue);
        
       // alarm["values"] =valuearr.append(description);
    //}
    cout<<"RequestArray:"<<request<<endl;




   CURL *curl;
    CURLcode res;
    struct curl_slist *headers=NULL;
    //slist1 = curl_slist_append(slist1, "Content-Type: multipart/form-data;");
    headers = curl_slist_append(headers, "method: POST");
    headers = curl_slist_append(headers, "scheme:https");
    curl = curl_easy_init();
    if(curl) {
        WriteToLog("URL:"+url);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,headers);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request);
        //curl_easy_setopt(curl, CURLOPT_USERNAME, "api-afme");
        //curl_easy_setopt(curl, CURLOPT_PASSWORD, "spublye7igIqivAS");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L);            
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        //curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,header_callback);
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,OnReceiveData);
        curl_easy_setopt(curl,CURLOPT_HEADERDATA,&headerBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        //if(headerBuffer > "")
        //{
            
            //cout<<"Header:"<<headerBuffer<<endl;
            
            std::map<std::string,std::string>::const_iterator itt;
            
            for(itt = headerBuffer.begin();itt!=headerBuffer.end();itt++)
            {
                if(itt->first=="status")
                {
                    cout<<itt->first<<":"<<itt->second<<endl;
                    status = itt->second;
                }
            }
            if(status=="201")
            {
                //stmt->executeUpdate("Update watchdog_device_alarms set EAI_status =1 where EAI_status=0");
            }
            //delete rs;
            //delete stmt;
            //delete conn;

            //reader.parse(readBuffer,root,false);
            //int size = root.size();
            //cout<<"Size:"<<root.size()<<endl;
            /*for(int i=0;i<=size;i++)
            {
                freespaces = freespaces + root[i]["counts"]["available"].asInt();
                total = total + root[i]["counts"]["total"].asInt();
                currentlevel = currentlevel + root[i]["counts"]["occupied"].asInt();
                
            }*/
            

            //InserttoDB(phase,freespaces,total,currentlevel);
           
            
       // }
       // else{
        // result = 0;

            //cout<<"Connection Failed"<<endl;
            //fprintf(stderr,"curl_easy_perform return : %s [%d]",curl_easy_strerror(res),res);
            //WriteToLog("Connection Failed");
           // WriteToLog("curl_easy_perform return :"+string(curl_easy_strerror(res)));
           // cout<<endl;
        //}
    }
}

int main(void)
{
    
    /*pid_t pid,sid;

    pid = fork();

    if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    else if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    umask(0);
    openlog("daemon-named",LOG_NOWAIT|LOG_PID,LOG_USER);
    syslog(LOG_NOTICE,"Successfully started ParkAssist Service");
    sid = setsid();
    if(sid<0)
    {
        exit(EXIT_FAILURE);
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    const int SLEEP_INTERVAL = 15;

    while(1)
    {*/

        WriteToLog("******************************************Log******************************************");
        //string url = "https://<EAIHost>:<port>>/api/v1/oms/ibms/data";   
        string url = EAI_URL;  
    
        CallEAIParkingRevenues(url);   
        
        
       // sleep(SLEEP_INTERVAL);
   /* }
    return 0;*/
}