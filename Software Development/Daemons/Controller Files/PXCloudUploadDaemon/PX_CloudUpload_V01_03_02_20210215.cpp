//sudo g++ -std=c++11 -Wall -o PX_CloudUploadTerminal_V01_03_02_20210622 PX_CloudUploadTerminal_V01_03_02_20210622.cpp PX_GeneralOperations.cpp -ljsoncpp -lmysqlcppconn -lpthread -lcurl -I/opt/lampp/include/ -L/opt/parcx/lib/
//added contract parking and updated access whitelist

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


#define DATABASE "parcx_dashboard"

//#define URL "http://192.168.1.207/PXCloudService/services.php"
string url;
using namespace std;

GeneralOperations general;
int cloud_operator_id;
string carpark_name, facility_name, device_name;

string facility_number;
string webservice_response;
string start_time = "";
string start_date = "";
string end_time = "";
string end_date = "";

struct table_details {
    string table_name;
    int task;
    int rowlimit;
};

void WriteToLog(string function, string msg) {
    general.writeLog("Daemons/PXCloudUpload/ApplicationLog_PX_Cloud_Upload_" + general.currentDateTime("%Y-%m-%d"), function, msg);
}

void WriteException(string function, string msg) {    
    general.writeLog("Daemons/PXCloudUpload/ExceptionLog_PX_Cloud_Upload_" + general.currentDateTime("%Y-%m-%d"), function, msg);
    WriteToLog("Exception: "+function,msg);
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

string intToHex(int val) 
    {
    std::stringstream ss;
    ss << std::hex << val;
    std::string result(ss.str());
    return result;
    }

string generateTicketNumber(string facility, string carpark, string device, string ticketType) 
    {
    try {
        switch (facility.length()) {
            case 1:
                facility = "00000" + facility;
                break;
            case 2:
                facility = "0000" + facility;
                break;
            case 3:
                facility = "000" + facility;
                break;
            case 4:
                facility = "00" + facility;
                break;
            case 5:
                facility = "0" + facility;
                break;
        }

        string hexCarpark = intToHex(stoi(carpark));
        if (hexCarpark.length() == 1)
            hexCarpark = "0" + hexCarpark;
        string hexDevice = intToHex(stoi(device));
        if (hexDevice.length() == 1)
            hexDevice = "0" + hexDevice;
        string hexTicketType = intToHex(stoi(ticketType));
        if (hexTicketType.length() == 1)
            hexTicketType = "0" + hexTicketType;
       
        struct tm tm;
        string startDate = "2018-01-01 00:00:00";       
        strptime(startDate.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
        time_t t = mktime(&tm);
        time_t now = time(NULL);
        long seconds = difftime(now, t);
        seconds = seconds + 1415926535;
        std::stringstream ss;
        string ticketNo;
        ss << std::hex << seconds;
        std::string ticket(ss.str());
        ticketNo = facility + hexCarpark + hexDevice + hexTicketType + ticket;
        return ticketNo;
        } 
    catch (exception &e) 
        {
        WriteException("Generate Ticket number", e.what());
        return "";
        }
    }

void GetDayclosuretime() 
    {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select setting_name,setting_value from system_settings where setting_name in ('dayclosure_start_time','dayclosure_end_time')");
        while (rs->next()) {
            if (rs->getString("setting_name") == "dayclosure_start_time") {
                start_time = rs->getString("setting_value");
                WriteToLog("getDayclosuretime", "dayclosure_start_time :" + start_time);
            } else {
                end_time = rs->getString("setting_value");
                WriteToLog("getDayclosuretime", "dayclosure_end_time :" + end_time);
            }
        }
        delete rs;
        delete stmt;
        delete conn;
    } catch (sql::SQLException &e) {
        WriteException("getDayclosuretime", e.what());
    }
}

string CallWebservice(string jsonstring) {    
    string readBuffer = "";
    CURL *curl;
    CURLcode res;
    try {
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstring.c_str());
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) 
                {              
                //WriteToLog("CallWebservice", "Request:" + jsonstring);
                WriteToLog("CallWebservice", "Curl Failed");
                WriteToLog("CallWebservice", "Return Code:" + to_string(res));
                }
            curl_easy_cleanup(curl);
        }
    } catch (exception &e) {       
        WriteException("CallWebservice", e.what());
    }
    return readBuffer;
}

void GetSystemSettings() {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    try {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select  setting_name,setting_value from device_settings where setting_name in('cloud_operator_id','facility_number','facility_name','cloud_upload_webservice')");
        while (res->next()) {
          
            if (res->getString("setting_name") == "facility_number") {
                facility_number = res->getString("setting_value");
            }
            if (res->getString("setting_name") == "facility_name") {
                facility_name = res->getString("setting_value");
            }           
            if (res->getString("setting_name") == "cloud_upload_webservice") {
                url = res->getString("setting_value");
            }
            if (res->getString("setting_name") == "cloud_operator_id") {
                cloud_operator_id = res->getInt("setting_value");
            }
        }

        delete res;
        delete stmt;
        delete conn;
        WriteToLog("facility_number",facility_number);
        WriteToLog("facility_name",facility_name);
        WriteToLog("url",url);
        WriteToLog("cloud_operator_id",to_string(cloud_operator_id));
    } catch (sql::SQLException &e) {
        WriteException("GetSettings", e.what());
    }
}

void UpdatePXCloudUploadTime(string table_name) {
    sql::Connection *conn;
    sql::Statement *stmt;
    try {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        stmt->executeUpdate("Update pxcloud_upload_settings set cloud_upload_date_time=CURRENT_TIMESTAMP() where table_name = '" + table_name + "'");
        delete stmt;
        delete conn;
    } catch (sql::SQLException &e) {
        WriteException("UpdatePXCloudUploadTime", e.what());
    }
}

Json::Value GetTableData(string table_name,int rowlimit, int task) {   

    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    sql::ResultSetMetaData *res_meta;

    Json::Value row, json;
    Json::FastWriter fw;   

    try 
        {
        Json::Value array;
        con = general.mysqlConnect();
        stmt = con->createStatement();
        res = stmt->executeQuery("Select * from "+table_name+" where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            res_meta = res -> getMetaData();
            int columns = res_meta -> getColumnCount();
            int i=1;
            while(res->next())
                {
                for (i = 1; i <= columns; i++)
                    row[res_meta -> getColumnLabel(i)] = res->getString(i).asStdString();                        
                array.append(row);
                }
            
            json["data"] = array;
            json["task"] = task;           
            } 
        delete res;
        delete stmt;
        delete con;
        } 
    catch (const std::exception& e) 
        {       
        WriteException("GetTableData", e.what());        
        }
    return json;

    }


void UpdateUploadStatus(string sql,string table) 
    {
    sql::Connection *conn;
    sql::Statement *stmt;
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        stmt->executeUpdate(sql);
        sql="Update pxcloud_upload_settings set cloud_upload_date_time=CURRENT_TIMESTAMP() where table_name = '" + table + "'";
        stmt->executeUpdate(sql);        
        delete stmt;
        delete conn;
        } 
    catch (sql::SQLException &e) 
        {        
        WriteException("UpdateUploadStatus", e.what());
        }
    }

void ProcessRequest(string jsonstring) 
    {
    bool parse;
    int j = 0, i = 0;
    Json::Reader reader;
    Json::Value array;
    string id_string;
    string sql;
    try 
        {
        string webservice_response = CallWebservice(jsonstring);        
        WriteToLog("ProcessRequest", "Webservice Response:" + webservice_response);

        if (webservice_response > "") 
            {
            parse = reader.parse(webservice_response, array, true);
            if (parse) 
                {
                for (j = 0; j < (signed)array["response"].size(); j++) 
                    { 
                    id_string = "";
                    sql = "";
                    for (i = 0; i < (signed)array["response"][j]["data"].size(); i++) 
                        {
                        id_string = id_string + array["response"][j]["data"][i].asString();
                        if (i < (signed)(array["response"][j]["data"].size() - 1))                             
                            id_string = id_string + ",";                            
                        }

                    if (array["response"][j]["table"] == "parking_movements_access") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_movements_access set cloud_upload_status = 1,upload_date_time=CURRENT_TIMESTAMP() where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","parking_movements_access");                            
                            }
                        } 
                    
                    else if (array["response"][j]["table"] == "parking_movements_manual") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_movements_manual set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","parking_movements_manual");                            
                            }
                        } 
		    else if (array["response"][j]["table"] == "parking_movements") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_movements set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","parking_movements_manual");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "parking_reservation") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_reservation set cloud_upload_status = 1 where reservation_id in (" + id_string + ")";                            
                            UpdateUploadStatus(sql, "parcx_reporting","parking_reservation");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "contract_parking_transactions") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update contract_parking_transactions set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","contract_parking_transactions");                            
                            }
                        }
                    }
                } 
            else 
                {
                WriteToLog("ProcessRequest", "Parse Error");
                }
            }
        } 
    catch (const std::exception& e) 
        {
        WriteException("ProcessRequest", e.what());        
        }
    }

void CollectData(int interval) 
    {
    sql::ResultSet *res;
    sql::Connection *conn;
    sql::Statement *stmt;
    Json::FastWriter fw;
    Json::Value json, response;     
    json["facility_number"] = facility_number;    
    json["operator_id"] = cloud_operator_id;
    json["transaction_id"] = generateTicketNumber(facility_number, "1", "1", "00");
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select table_name,task,upload_row_limit from pxcloud_upload_settings where day_closure = 0 and enabled=1 and time_interval = " + to_string(interval));
        int n = res->rowsCount();
        struct table_details tb[n];
        int i = 0;
        while (res->next()) 
            {
            tb[i].table_name = res->getString("table_name");
            tb[i].task = res->getInt("task");
            tb[i].rowlimit = res->getInt("upload_row_limit");
            i++;
            }
        delete res;
        delete stmt;
        delete conn;
        while (true) 
            {
            json["table"].clear();
            for (i = 0; i < n; i++) 
                {                
                response = GetTableData(tb[i].table_name, tb[i].rowlimit, tb[i].task);
                if (!response.isNull())
                    json["table"].append(response);
                }


            string jsonstring = fw.write(json);
            if (json["table"].size() > 0) 
                {                
                WriteToLog("CollectData", "Thread delay:"+to_string(interval)+"\tFull request:   " + fw.write(json));
                ProcessRequest(jsonstring);
                } 
            else if (json["table"].size() == 0) 
                {
                WriteToLog("CollectData", "Thread delay: "+to_string(interval)+"\tNo Records Found");
                }
            sleep(interval);
            }
        } 
    catch (const std::exception& e) 
        {
        WriteException("CollectData", e.what());        
        }
    }

int GetInterval() 
    {
    int seconds;
    char out[30], out1[30];
    string stime = general.currentDateTime("%Y-%m-%d") + " " + start_time; //Start Time
    struct tm tm, tm1;
    strptime(stime.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    time_t e = mktime(&tm);
    time_t n = time(NULL);    
    seconds = difftime(e, n);

    start_date = stime;
    if (seconds < 0) 
        {
        tm.tm_mday += 1;
        time_t e = mktime(&tm);
        time_t now = time(NULL);
        seconds = difftime(e, now);
        strftime(out, 30, "%Y-%m-%d %H:%M:%S", &tm);
        string stime(out);
        start_date = stime;
        }

    strptime(start_date.c_str(), "%Y-%m-%d %H:%M:%S", &tm1);
    tm1.tm_mday += 1;
    tm1.tm_sec -= 1;
    mktime(&tm1);
    strftime(out1, 30, "%Y-%m-%d %H:%M:%S", &tm1);
    string etime(out1);
    end_date = etime;

    WriteToLog("Time", "start:" + start_date + " end:" + end_date);    
    return seconds;
    }

void DCDaemon() 
    {
    WriteToLog("DCDaemon", "DCDaemon Starts");

    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::FastWriter fw;
    Json::Value json, response;        
    json["facility_number"] = facility_number;    
    json["operator_id"] = cloud_operator_id;
    json["transaction_id"] = generateTicketNumber(facility_number, "1", "1", "00");

    string table_name;
    int task, rowlimit;   
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select table_name,task,upload_row_limit from pxcloud_upload_settings where enabled=1 and day_closure = 1");
        if (rs->rowsCount() > 0) 
            {
            while (rs->next()) 
                {
                table_name = rs->getString("table_name");
                task = rs->getInt("task");
                rowlimit = rs->getInt("upload_row_limit");
                response =GetTableData(table_name, rowlimit, task);
                if (!response.isNull())
                    json["table"].append(response);

                }
            string jsonstring = fw.write(json);
            if (json["table"].size() > 0) 
                {                
                WriteToLog("DCDaemon", "Full req:   " + fw.write(json));
                ProcessRequest(jsonstring);
                } 
            else if (json["table"].size() == 0) 
                {
                WriteToLog("DCDaemon", "No DC tables Found");
                }
            }

        delete rs;
        delete stmt;
        delete conn;
        sleep(1);
        } 
    catch (exception &e) 
        {        
        WriteException("DCDaemon", e.what());
        }
    }





void PostDCData() 
    {
    int SLEEP_INTERVAL;
    GetDayclosuretime();
    WriteToLog("Main", "Url:" + url);    
    while (true) 
        {        
        WriteToLog("", "******************************************Log******************************************");
        DCDaemon();
        SLEEP_INTERVAL = GetInterval(); //Add 30 min to dayclosure time;
        WriteToLog("Main", "Daemon Sleeps for " + std::to_string(SLEEP_INTERVAL) + " seconds");
        WriteToLog("Main", "StartDate:" + start_date + " EndDate:" + end_date);
        sleep(SLEEP_INTERVAL);
        WriteToLog("DCDaemon", "Daemon Sleeps for 30 minutes more");
        sleep(30 * 60);       
        }
    }

void PostData() 
    {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int i = 0, n = 0;    
    int interval;
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select distinct(time_interval) from pxcloud_upload_settings where day_closure=0 and enabled=1");
        n = res->rowsCount();
        thread updatethread[n], dcthread;

        while (res->next()) 
            {
            interval = res->getInt("time_interval");
            updatethread[i] = thread(CollectData, interval);                        
            i++;
            }
        delete res;        
        res = stmt->executeQuery("select id from pxcloud_upload_settings where enabled=1 and day_closure = 1");
        if (res->next()) 
            {
            dcthread = thread(PostDCData);
            dcthread.join();            
            }
        delete res;
        delete stmt;
        delete conn;

        for (i = 0; i < n; i++)
            {
            updatethread[i].join();            
            }

        } 
    catch (exception &e) 
        {
        WriteException("PostData", e.what());
        }
    }

void UpdateProcessId(int pid) 
    {
    sql::Connection *conn;
    sql::Statement *stmt;   
    try 
        {
        conn = general.mysqlConnect();
        stmt = conn->createStatement();
        stmt->executeUpdate("Update daemon_status set pid = " + to_string(pid) + ",start_time=CURRENT_TIMESTAMP() where daemon_label = 'Cloud Upload Daemon'");
        } 
    catch (exception &e) 
        {        
        WriteException("UpdateProcessId", e.what());
        }
    delete stmt;
    delete conn;
    }

int main(void) 
    {
    int pid_file = open("px_cloud_upload_daemon.lock", O_CREAT | O_RDWR, 0644);
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
        pid_t pid, sid;
        pid = fork();
        if (pid > 0) 
            {
            WriteToLog("Main: ", "Cloud Upload Daemon Version : PX_CloudUpload_V01_03_02_20210215    Process Id:" + to_string(pid));
            cout << "Cloud Upload Daemon Version : PX_CloudUpload_V01_03_02_20210215" << "Process Id :" << pid << endl;

            UpdateProcessId(pid);
            exit(EXIT_SUCCESS);
            } 
        else if (pid < 0) 
            {
            exit(EXIT_FAILURE);
            }
        umask(0);
        openlog("daemon-named", LOG_NOWAIT | LOG_PID, LOG_USER);
        syslog(LOG_NOTICE, "Successfully started PXCloudUploadDaemon");
        WriteToLog("main", "Successfully started PXCloudUploadDaemon");
        sid = setsid();
        if (sid < 0) 
            {
            exit(EXIT_FAILURE);
            }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        WriteToLog("Main", "******************************************Log******************************************");
        GetSystemSettings();
        PostData();
        }
    return 0;
    }
