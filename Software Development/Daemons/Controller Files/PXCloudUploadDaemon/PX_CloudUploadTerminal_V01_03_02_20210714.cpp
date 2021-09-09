//sudo g++ -std=c++11 -Wall -o PX_CloudUploadTerminal_V01_03_02_20210714 PX_CloudUploadTerminal_V01_03_02_20210714.cpp PX_GeneralOperations.cpp -ljsoncpp -lmysqlcppconn -lpthread -lcurl -I/opt/lampp/include/ -L/opt/parcx/lib/
//NEW DAEMON

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

string cloud_upload_webservice_url,client_webservice_url;
using namespace std;
GeneralOperations general;
int cloud_operator_id;
string carpark_name, facility_name, device_name,iot_name,iot_mode;
int carpark_number,device_number,client_type=0;
string facility_number;
string webservice_response;
string start_time = "";
string end_time = "";
string start_date = "";
string end_date = "";

struct table_details {
    string table_name;
    int task;
    int rowlimit;
};

void WriteToLog(string function, string msg) {
    cout<<function<<":"<<msg<<endl;
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


string generateTransactionId()
    {    							    		
      																				    
    struct tm tm;
    string startDate="2020-01-01 00:00:00";    

    strptime(startDate.c_str(),"%Y-%m-%d %H:%M:%S",&tm);		
    time_t t=mktime(&tm);

    time_t now=time(NULL);
    long seconds=difftime(now,t);

    seconds=seconds+1415926535;			
			
    std::stringstream ss;				
    ss << std::hex << seconds;
    std::string ticket(ss.str());									
    
    return facility_number+"011100"+ticket;   
    }

string CallWebservice(string jsonstring,string webservice_url) {    
    string readBuffer = "";
    CURL *curl;
    CURLcode res;
    try {
        curl = curl_easy_init();
	struct curl_slist *headers = NULL;
	    //headers = curl_slist_append(headers, "Accept: application/json");
	    headers = curl_slist_append(headers, "Content-Type: application/json");
	    //headers = curl_slist_append(headers, "charset: utf-8");
        if (curl) {
	    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dark Secret Ninja/1.0");
            curl_easy_setopt(curl, CURLOPT_URL, webservice_url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstring.c_str());
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) 
                {                             
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

void UpdatePID(string pid) {
    sql::Connection *conn;
    sql::Statement *stmt;
    
    try {
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        stmt->executeUpdate("Update daemon_status set pid = " + pid + ",start_time=CURRENT_TIMESTAMP() where daemon_name like 'PX_CloudUpload%'");                
        delete stmt;
        delete conn;        
    } catch (sql::SQLException &e) {
        WriteException("UpdatePID", e.what());
    }
}

void GetSystemSettings() {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    try {
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();        
        
        res = stmt->executeQuery("Select  setting_name,setting_value from device_settings where setting_name in('dayclosure_start_time','dayclosure_end_time','cloud_operator_id','facility_number','facility_name','cloud_upload_webservice','carpark_number','carpark_name','device_number','device_name','client_webservice_url','iot_mode','IOT_mode_name')");
        while (res->next()) {          
            if (res->getString("setting_name") == "facility_number") 
                facility_number = res->getString("setting_value");
            
            if (res->getString("setting_name") == "facility_name") 
                facility_name = res->getString("setting_value");
                       
            if (res->getString("setting_name") == "cloud_upload_webservice") 
                cloud_upload_webservice_url = res->getString("setting_value");
            
            if (res->getString("setting_name") == "cloud_operator_id") 
                cloud_operator_id = res->getInt("setting_value");
            
            if (res->getString("setting_name") == "dayclosure_start_time") 
                start_time = res->getString("setting_value");
               
            if (res->getString("setting_name") == "dayclosure_end_time") 
                end_time = res->getString("setting_value");    

	    if (res->getString("setting_name") == "device_name")
                device_name = res->getString("setting_value");

            if (res->getString("setting_name") == "device_number")
                device_number = res->getInt("setting_value");

	    if (res->getString("setting_name") == "carpark_number")
                carpark_number = res->getInt("setting_value");

	    if (res->getString("setting_name") == "carpark_name")
                carpark_name = res->getString("setting_value");
	   
	    if (res->getString("setting_name") == "client_webservice_url")
                client_webservice_url = res->getString("setting_value");
                       
            if (res->getString("setting_name") == "iot_mode")
                client_type = res->getInt("setting_value");

	    if (res->getString("setting_name") == "IOT_mode_name")
                iot_name = res->getString("setting_value");
                       
        }

        delete res;
        delete stmt;
        delete conn;
        
        WriteToLog("facility_number",facility_number);
        WriteToLog("facility_name",facility_name);
	WriteToLog("carpark_number",to_string(carpark_number));
        WriteToLog("carpark_name",carpark_name);
	WriteToLog("device_number",to_string(device_number));
        WriteToLog("device_name",device_name);
        WriteToLog("cloud_upload_webservice_url",cloud_upload_webservice_url);
        WriteToLog("cloud_operator_id",to_string(cloud_operator_id));
        WriteToLog("getDayclosuretime", "dayclosure_start_time :" + start_time);
        WriteToLog("getDayclosuretime", "dayclosure_end_time :" + end_time);
	WriteToLog("getDayclosuretime", "iot mode :" + to_string(client_type));
	WriteToLog("getDayclosuretime", "iot mode name :" + iot_name);
    } catch (sql::SQLException &e) {
        WriteException("GetSettings", e.what());
    }
}

Json::Value TableList(Json::Value tables,int n) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res; 
    sql::ResultSetMetaData *res_meta;
    Json::Value json,response;
    Json::Value array,row;
    int i=0,j=0;
    int columns;
    string table,db;
    string columname;
    int rowlimit,task;
            
    try 
        {        
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        for(i=0;i<n;i++)
            {
            table=tables[i]["table_name"].asString();
            rowlimit=tables[i]["rowlimit"].asInt();
            task=tables[i]["task"].asInt();
            
            res = stmt->executeQuery("Select * from "+table+" where cloud_upload_status=0 limit " + to_string(rowlimit));
            if (res->rowsCount() > 0) 
                {
                array.clear();
                row.clear();
                res_meta = res -> getMetaData();
                columns = res_meta -> getColumnCount();

                while (res->next()) 
                    {                    
                    for (j = 1; j <= columns; j++)
                        {  
                        columname=res_meta->getColumnTypeName(j);
                        if(columname.find("INT")!=std::string::npos)
                            row[res_meta -> getColumnLabel(j)] = res->getInt(j);
                        else
                            row[res_meta -> getColumnLabel(j)] = res->getString(j).asStdString();
                        }
                    row["id"] = row["id"];
                    row["facility_number"] = facility_number;                     
                    row["facility_name"] = facility_name;
                    row["carpark_number"] = carpark_number;
		    row["carpark_name"] = carpark_name;
		    row["device_number"] = device_number;
		    row["device_name"] = device_name;
		    row["iot_mode"] = client_type;
		    row["iot_mode_name"] = iot_name;
                    array.append(row);
                    }
                json["data"] = array;
                json["task"] = task;  
                response.append(json);
                }
            delete res;
            }
        delete stmt;
        delete conn;

        } 
    catch (const std::exception& e) 
        {        
        WriteException("TableList", e.what());       
        }
    return response;
    }

void UpdateUploadStatus(Json::Value json) 
    {
    sql::Connection *con;
    sql::Statement *stmt;
    string id_string,table,db;
    try 
        {                
        con = general.mysqlConnect("ParcxTerminal");
        stmt = con->createStatement();
        for (int i = 0; i < (signed)json.size(); i++) 
            { 
            table=json[i]["table_name"].asString();
            id_string=json[i]["id_string"].asString();            
            if(id_string!="")
                stmt->executeUpdate("Update "+table+" set cloud_upload_status = 1 where id in (" + id_string + ")");            
            
            
            stmt->executeUpdate("Update pxcloud_upload_settings set cloud_upload_date_time=CURRENT_TIMESTAMP() where table_name = '" + table + "'");
            }
        delete stmt;
        delete con;
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
    try 
        {
        WriteToLog("ProcessRequest", "Cloud Webservice Request: " + jsonstring);
        string webservice_response = CallWebservice(jsonstring,cloud_upload_webservice_url);        
        WriteToLog("ProcessRequest", "Cloud Webservice Response: " + webservice_response);
        if (webservice_response > "") 
            {
            parse = reader.parse(webservice_response, array, true);
            if (parse) 
                {                
                Json::Value json;
                string id_string;    
                string table_name;
                for (j = 0; j < (signed)array["response"].size(); j++) 
                    {                                                            
                    table_name=array["response"][j]["table"].asString();
                    id_string = "";
                    for (i = 0; i < (signed)array["response"][j]["data"].size(); i++) 
                        {
                        id_string = id_string + array["response"][j]["data"][i].asString();
                        if (i < (signed)(array["response"][j]["data"].size() - 1))                             
                            id_string = id_string + ",";                            
                        }
                    
                    json[j]["table_name"]=table_name;
                    json[j]["id_string"]=id_string;  
                    
                    
                    }
                if (!json.isNull())
                    UpdateUploadStatus(json);
                
                json.clear();
                array.clear();
                } 
            else                                
                WriteToLog("ProcessRequest", "Parse Error");                
            }
        }  
    catch (const std::exception& e) 
        {
        WriteException("ProcessRequest", e.what());        
        }
    }
void PostRealTimeData(int interval) 
    { 

    sql::Connection *conn;
    sql::Statement *stmt;    
    sql::ResultSet *res;
    
    Json::Value json,tables; 
    Json::FastWriter fw;   
    int n=0,i=0;
    string jsonstring;
    cout<<"herer12"<<endl; 
    json["facility_number"] = facility_number;    
    json["operator_id"] = cloud_operator_id;    
//cout<<"herer11"<<endl;   
    try 
        {
	
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select table_name,task,upload_row_limit from pxcloud_upload_settings where day_closure = 0 and enabled=1 and time_interval = " + to_string(interval));
        n = res->rowsCount();   
	cout<<"herer12"<<endl;             
        while (res->next()) 
            {
            tables[i]["table_name"] = res->getString("table_name").asStdString();
            tables[i]["task"] = res->getInt("task");
            tables[i]["rowlimit"] = res->getInt("upload_row_limit");
            
            i++;
            }
        
        delete res;
        delete stmt;
        delete conn;
        
        while (true) 
            {
            try
                {
                json["transaction_id"] = generateTransactionId();
                json["table"].clear();
                json["table"]=TableList(tables,n);                                    
                
                if (json["table"].size() > 0) 
                    {                
                    jsonstring = fw.write(json);
                    ProcessRequest(jsonstring);
                    } 
                else if (json["table"].size() == 0)                     
                    WriteToLog("PostRealTimeData", "Thread delay: "+to_string(interval)+"\tNo Records Found");
                    
                sleep(interval);
                }
            catch (const std::exception& e) 
                {
                WriteException("PostRealTimeData", e.what());        
                }
            }
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostRealTimeData", e.what());        
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
void PostDCData() 
    {    
    sql::Connection *conn;
    sql::Statement *stmt;    
    sql::ResultSet *res;
    
    Json::Value json,tables; 
    Json::FastWriter fw;   
    int n=0,i=0;
    string jsonstring;
    int SLEEP_INTERVAL;
    
    json["facility_number"] = facility_number;    
    json["operator_id"] = cloud_operator_id;    
    try 
        {
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select table_name,task,upload_row_limit from pxcloud_upload_settings where day_closure = 1 and enabled=1");
        n = res->rowsCount();                
        while (res->next()) 
            {
            tables[i]["table_name"] = res->getString("table_name").asStdString();
            tables[i]["task"] = res->getInt("task");
            tables[i]["rowlimit"] = res->getInt("upload_row_limit");
            i++;
            }
        
        delete res;
        delete stmt;
        delete conn;
        
        while (true) 
            {
            try 
                {
                json["transaction_id"] = generateTransactionId();
                json["table"].clear();
                json["table"]=TableList(tables,n);                                    

                
                if (json["table"].size() > 0)                                                 
                    {
                    jsonstring = fw.write(json);
                    ProcessRequest(jsonstring);                
                    }
                else if (json["table"].size() == 0)                 
                    WriteToLog("PostDCData", "No DC Records Found");  

                SLEEP_INTERVAL = GetInterval(); 
                WriteToLog("PostDCData", "Daemon Sleeps for " + std::to_string(SLEEP_INTERVAL) + " seconds");
                WriteToLog("PostDCData", "StartDate:" + start_date + " EndDate:" + end_date);
                sleep(SLEEP_INTERVAL);
                WriteToLog("PostDCData", "Daemon Sleeps for 30 minutes more");
                sleep(30 * 60); 
                }
            catch (const std::exception& e) 
                {
                WriteException("PostDCData", e.what());        
                }
            }        
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostDCData", e.what());        
        }
    }

void PostParkingMovements(int upload_limit) { // Post Bulk transactions
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value jsonrow,jsondata,jsonResponse;
    string jsonstring,query;
    Json::FastWriter fw; 
    Json::Reader reader;
    int i;
    string transaction_id,id_string;  
    try {   
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();                                             
        if (conn) {
	    if(client_type==1)
	    {
		    transaction_id = generateTransactionId();
		    query = "select id,ticket_id,DATE_FORMAT(date_time, '%Y-%m-%dT%TZ') as date_time,movement_type from parking_movements where upload_status_server1=0 limit "+to_string(upload_limit);
			
		    res = stmt->executeQuery(query);
		    if (res->rowsCount() > 0) {
			i=1;
		        while (res->next()) {
		            jsonrow["device_number"] = to_string(device_number);
		            jsonrow["carpark_number"] = carpark_number;                  
		            jsonrow["facility_number"] = facility_number;
		            jsonrow["id"] = res->getInt("id");
		            jsonrow["ticket_id"] = res->getString("ticket_id").asStdString();
		            jsonrow["date_time"] = res->getString("date_time").asStdString();
		            jsonrow["movement_type"]=res->getInt("movement_type");
			    jsonrow["transaction_id"]=transaction_id;
			    jsonrow["authentication_key"]="00000";
		            jsondata.append(jsonrow);
			    id_string = id_string + res->getString("id");
			    if (i <= (signed)(res->rowsCount() - 1))                             
			    	id_string = id_string + ","; 
			    i++;
		            jsonrow.clear();
		        }
		        
		        jsonstring = fw.write(jsondata);
			
		        WriteToLog("PostParkingMovements","Client Webservice Request:"+jsonstring+"   Client URL:"+client_webservice_url);
		        string webservice_response = CallWebservice(jsonstring,client_webservice_url);        
			WriteToLog("PostParkingMovements", "Client Webservice Response: " + webservice_response);
			
			if (webservice_response > "") 
			    {
				if(reader.parse(webservice_response,jsonResponse))
				{
					
					WriteToLog("PostParkingMovements", "ids: " + id_string);
					
					if(jsonResponse["data"]==true)
					{
						//WriteToLog("PostParkingMovements", "ids: " + id_string);
						query = "update parking_movements set upload_status_server1=1 where id in (" + id_string + ")";
						cout<<query<<endl;
						stmt->executeUpdate(query);
					}
					
				}
				

				
			}
		    } else
		        WriteToLog("PostParkingMovements","No data to PostMovements");
		    delete res;

	    }
	    
    	
        } // local db connection 
	delete stmt;
	delete conn;
	 				
    } catch (exception &e) {
        WriteToLog("PostParkingMovements",e.what());
    }
} // END Function. Post Parking Movements to Server


/*void PostParkingMovements(int upload_limit) { //Post individual transactions
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value jsonrow,jsondata,jsonResponse;
    string jsonstring,query;
    Json::FastWriter fw; 
    Json::Reader reader;
    int i;
    string transaction_id,id_string;  
    try {   
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();                                             
        if (conn) {
	    if(client_type==1)
	    {
		    transaction_id = generateTransactionId();
		    query = "select id,ticket_id,DATE_FORMAT(date_time, '%Y-%m-%dT%TZ') as date_time,movement_type from parking_movements where upload_status_server1=0 limit "+to_string(upload_limit);
			
		    res = stmt->executeQuery(query);
		    if (res->rowsCount() > 0) {
			i=1;
		        while (res->next()) {
			    id_string="";
		            jsonrow["device_number"] = to_string(device_number);
		            jsonrow["carpark_number"] = carpark_number;                  
		            jsonrow["facility_number"] = facility_number;
		            jsonrow["id"] = res->getInt("id");
		            jsonrow["ticket_id"] = res->getString("ticket_id").asStdString();
		            jsonrow["date_time"] = res->getString("date_time").asStdString();
		            jsonrow["movement_type"]=res->getInt("movement_type");
			    jsonrow["transaction_id"]=transaction_id;
			    jsonrow["authentication_key"]="00000";
			    id_string = res->getString("id");
			    jsonstring = fw.write(jsonrow);
			
			    WriteToLog("PostParkingMovements","Client Webservice Request:"+jsonstring+"   Client URL:"+client_webservice_url);
			    string webservice_response = CallWebservice(jsonstring,client_webservice_url);        
			    WriteToLog("PostParkingMovements", "Client Webservice Response: " + webservice_response);
				
			    if (webservice_response > "") 
			    {
				if(reader.parse(webservice_response,jsonResponse))
				{
					WriteToLog("PostParkingMovements", "id: " + id_string);
					
					if(jsonResponse["data"]==true)
					{
						//WriteToLog("PostParkingMovements", "ids: " + id_string);
						query = "update parking_movements set upload_status_server1=1 where id = " + id_string;
						cout<<query<<endl;
						stmt->executeUpdate(query);
					}
					
				}	
			    }
		           
			    i++;
		            jsonrow.clear();
		        }
		        
		        
		    } else
		        WriteToLog("PostParkingMovements","No data to PostMovements");
		    delete res;

	    }
	    
    	
        } // local db connection 
	delete stmt;
	delete conn;
	 				
    } catch (exception &e) {
        WriteToLog("PostParkingMovements",e.what());
    }
} */// END Function. Post Parking Movements to Server

void PostParkingMovementsAccess(int upload_limit) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value jsonrow,jsondata;
    string jsonstring,query,jsonResponse;
    Json::FastWriter fw; 
    Json::Reader jsonReader;
    string transaction_id;  
    try {   
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();                                             
        if (conn) {
	    if(client_type==1)
	    {
		    transaction_id = generateTransactionId();
		    query = "select id,ticket_id,DATE_FORMAT(date_time, '%Y-%m-%dT%TZ') as date_time,movement_type from parking_movements_access where upload_status_server1=0 limit "+to_string(upload_limit);
			
		    res = stmt->executeQuery(query);
		    if (res->rowsCount() > 0) {
		        while (res->next()) {
		            jsonrow["device_number"] = to_string(device_number);
		            jsonrow["carpark_number"] = carpark_number;                  
		            jsonrow["facility_number"] = facility_number;
		            jsonrow["id"] = res->getInt("id");
		            jsonrow["ticket_id"] = res->getString("ticket_id").asStdString();
		            jsonrow["date_time"] = res->getString("date_time").asStdString();
		            jsonrow["movement_type"]=res->getInt("movement_type");
			    jsonrow["transaction_id"]=transaction_id;
			    jsonrow["authentication_key"]="00000";
		            jsondata.append(jsonrow);
		            jsonrow.clear();
		        }
		        
		        jsonstring = fw.write(jsondata);

		        WriteToLog("PostParkingMovementsAccess","Webservice Request:"+jsonstring+"   Client URL:"+client_webservice_url);
		        string webservice_response = CallWebservice(jsonstring,client_webservice_url);        
			WriteToLog("PostParkingMovementsAccess", "Webservice Response: " + webservice_response);
			if (webservice_response > "") 
			{
				/*if(jsonReader.parse(webservice_response, jsonResponse)){
				    for (int i = 0; i < jsonResponse.size(); i++) {
				       // query = "update parking_movements set upload_status_server1=1 where id='" + jsonResponse[i].asString() + "'";
				       // stmt->executeUpdate(query);
				    }
				    WriteToLog("PostParkingMovements",to_string(jsonResponse.size()) + " Parking Movements uploaded");
				} else {
				    WriteToLog("PostParkingMovements","Parking Movements: Could not parse HTTP data as JSON");
		        	}*/
			}
		    } else
		        WriteToLog("PostParkingMovementsAccess","No data to PostMovementsAccess");
		    delete res;

	    }
	    
    	
        } // local db connection 
	delete stmt;
	delete conn;
	 				
    } catch (exception &e) {
        WriteToLog("PostParkingMovementsAccess",e.what());
    }
} // END Function. Post Parking Movements Access to Server

void PostParkingMovementsManual(int upload_limit) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value jsonrow,jsondata;
    string jsonstring,query,jsonResponse;
    Json::FastWriter fw; 
    Json::Reader jsonReader;
    string transaction_id;  
    try {   
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();                                             
        if (conn) {
	    if(client_type==1)
	    {
		    transaction_id = generateTransactionId();
		    query = "select id,action,DATE_FORMAT(date_time, '%Y-%m-%dT%TZ') as date_time,reason from parking_movements_manual where upload_status_server1=0 limit "+to_string(upload_limit);
			
		    res = stmt->executeQuery(query);
		    if (res->rowsCount() > 0) {
		        while (res->next()) {
		            jsonrow["device_number"] = to_string(device_number);
		            jsonrow["carpark_number"] = carpark_number;                  
		            jsonrow["facility_number"] = facility_number;
		            jsonrow["id"] = res->getInt("id");
		            jsonrow["action"] = res->getString("action").asStdString();
		            jsonrow["date_time"] = res->getString("date_time").asStdString();
		            jsonrow["reason"]=res->getString("reason").asStdString();
			    jsonrow["transaction_id"]=transaction_id;
			    jsonrow["authentication_key"]="00000";
		            jsondata.append(jsonrow);
		            jsonrow.clear();
		        }
		        
		        jsonstring = fw.write(jsondata);

		        WriteToLog("PostParkingMovementsManual","Webservice Request:"+jsonstring+"   Client URL:"+client_webservice_url);
		        string webservice_response = CallWebservice(jsonstring,client_webservice_url);        
			WriteToLog("PostParkingMovementsManual", "Webservice Response: " + webservice_response);
			if (webservice_response > "") 
			{
				/*if(jsonReader.parse(webservice_response, jsonResponse)){
				    for (int i = 0; i < jsonResponse.size(); i++) {
				       // query = "update parking_movements set upload_status_server1=1 where id='" + jsonResponse[i].asString() + "'";
				       // stmt->executeUpdate(query);
				    }
				    WriteToLog("PostParkingMovements",to_string(jsonResponse.size()) + " Parking Movements uploaded");
				} else {
				    WriteToLog("PostParkingMovements","Parking Movements: Could not parse HTTP data as JSON");
		        	}*/
			}
		    } else
		        WriteToLog("PostParkingMovementsManual","No data to PostMovementsManual");
		    delete res;

	    }
	    
    	
        } // local db connection 
	delete stmt;
	delete conn;
	 				
    } catch (exception &e) {
        WriteToLog("PostParkingMovementsManual",e.what());
    }
} // END Function. Post Parking Movements Manual to Server

void PostParkingMovementsReservation(int upload_limit) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value jsonrow,jsondata;
    string jsonstring,query,jsonResponse;
    Json::FastWriter fw; 
    Json::Reader jsonReader;
    string transaction_id;  
    try {   
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();                                             
        if (conn) {
	    if(client_type==1)
	    {
		    transaction_id = generateTransactionId();
		    query = "select id,ticket_id,DATE_FORMAT(date_time, '%Y-%m-%dT%TZ') as date_time,movement_type from parking_movements_reservation where upload_status_server1=0 limit "+to_string(upload_limit);
			
		    res = stmt->executeQuery(query);
		    if (res->rowsCount() > 0) {
		        while (res->next()) {
		            jsonrow["device_number"] = to_string(device_number);
		            jsonrow["carpark_number"] = carpark_number;                  
		            jsonrow["facility_number"] = facility_number;
		            jsonrow["id"] = res->getInt("id");
		            jsonrow["ticket_id"] = res->getString("ticket_id").asStdString();
		            jsonrow["date_time"] = res->getString("date_time").asStdString();
		            jsonrow["movement_type"]=res->getInt("movement_type");
			    jsonrow["transaction_id"]=transaction_id;
			    jsonrow["authentication_key"]="00000";
		            jsondata.append(jsonrow);
		            jsonrow.clear();
		        }
		        
		        jsonstring = fw.write(jsondata);

		        WriteToLog("PostParkingMovementsReservation","Webservice Request:"+jsonstring+"   Client URL:"+client_webservice_url);
		        string webservice_response = CallWebservice(jsonstring,client_webservice_url);        
			WriteToLog("PostParkingMovementsReservation", "Webservice Response: " + webservice_response);
			if (webservice_response > "") 
			{
				/*if(jsonReader.parse(webservice_response, jsonResponse)){
				    for (int i = 0; i < jsonResponse.size(); i++) {
				       // query = "update parking_movements set upload_status_server1=1 where id='" + jsonResponse[i].asString() + "'";
				       // stmt->executeUpdate(query);
				    }
				    WriteToLog("PostParkingMovements",to_string(jsonResponse.size()) + " Parking Movements uploaded");
				} else {
				    WriteToLog("PostParkingMovements","Parking Movements: Could not parse HTTP data as JSON");
		        	}*/
			}
		    } else
		        WriteToLog("PostParkingMovementsReservation","No data to PostMovementsReservation");
		    delete res;

	    }
	    
    	
        } // local db connection 
	delete stmt;
	delete conn;
	 				
    } catch (exception &e) {
        WriteToLog("PostParkingMovementsReservation",e.what());
    }
} // END Function. Post Parking Movements Reservation to Server


void PostToClientServer() 
{        
    sql::Connection *conn;
    sql::Statement *stmt;    
    sql::ResultSet *res;
    
    Json::Value json,tables; 
    Json::FastWriter fw;   
    //int n=0,i=0;
    string jsonstring;
    int parking_movements_enabled,parking_movements_limit,parking_movements_manual_enabled,parking_movements_manual_limit,parking_movements_access_enabled,parking_movements_access_limit,parking_movements_reservation_enabled,parking_movements_reservation_limit;
    
    try 
    {         
	conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select table_name,upload_row_limit from pxcloud_upload_settings where enabled=1");  
	while(res->next())
	{
		if(res->getString("table_name")=="parking_movements")
		{
			parking_movements_enabled = 1;
			parking_movements_limit = res->getInt("upload_row_limit");
		}
		else if(res->getString("table_name")=="parking_movements_access")
		{
			parking_movements_access_enabled = 1;
			parking_movements_access_limit = res->getInt("upload_row_limit");
		}
		else if(res->getString("table_name")=="parking_movements_manual")
		{
			parking_movements_manual_enabled = 1;
			parking_movements_manual_limit = res->getInt("upload_row_limit");
		}
		else if(res->getString("table_name")=="parking_movements_reservation")
		{
			parking_movements_reservation_enabled = 1;
			parking_movements_reservation_limit = res->getInt("upload_row_limit");
		}
	}
	delete res;
	delete stmt;
	delete conn;

 
        while (true) 
    	{
	    if(parking_movements_enabled==1)
	    {
	    	PostParkingMovements(parking_movements_limit);
		sleep(1);
	    }

	    if(parking_movements_access_enabled==1)
	    {
	    	PostParkingMovementsAccess(parking_movements_access_limit);
		sleep(1);
	    }

	    if(parking_movements_manual_enabled==1)
	    {
	    	PostParkingMovementsManual(parking_movements_manual_limit);
		sleep(1);
	    }

	    if(parking_movements_reservation_enabled==1)
	    {
	    	PostParkingMovementsReservation(parking_movements_reservation_limit);
		sleep(1);
	    }
		

		
	    sleep(10);
		
    	}
    } 
    catch (const std::exception& e) 
    {
        WriteException("PostToClientServer", e.what());        
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
        conn = general.mysqlConnect("ParcxTerminal");
        stmt = conn->createStatement();
        

	//Realtime Thread
	res = stmt->executeQuery("Select distinct(time_interval) from pxcloud_upload_settings where day_closure=0 and enabled=1");
        n = res->rowsCount();
	thread updatethread[n], dcthread,posttoserverthread;
        while (res->next()) 
            {
            interval = res->getInt("time_interval");
            updatethread[i] = thread(PostRealTimeData, interval);                        
            i++;
            }           
        delete res; 
        WriteToLog("PostData", "Created Realtime thread");
        

	//DayClosure Thread
        res = stmt->executeQuery("select id from pxcloud_upload_settings where enabled=1 and day_closure = 1");
        if (res->next())             
            {
            dcthread = thread(PostDCData);                                 
            WriteToLog("PostData", "Created DC thread");
            }
        delete res;        
            

	//Post to ClientServer Thread
	if(client_type==1)
	{
		res = stmt->executeQuery("select id from pxcloud_upload_settings where enabled=1");
		if (res->next())             
		    {
		    posttoserverthread = thread(PostToClientServer);                                 
		    WriteToLog("PostData", "Created posttoserver thread");
		    }
		delete res;  
	}

	delete stmt;
        delete conn;   
        
        
        WriteToLog("PostData", "JOINING THREADS");
        if(dcthread.joinable())
            dcthread.join();  

        for (i = 0; i < n; i++)            
            updatethread[i].join();   
	
	if(posttoserverthread.joinable())
            posttoserverthread.join();  

	                  

        } 

	
	
    catch (exception &e) 
        {
        WriteException("PostData", e.what());
        }
    }


int main(void) 
{
    string proccess_id;
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
        proccess_id=to_string(pid);
        if (pid > 0) 
            {
            WriteToLog("Main: ", "Cloud Terminal Upload Daemon Version : PX_CloudUploadTerminal_V01_03_07_20210714    Process Id:" + proccess_id);
            cout << "Cloud Upload Daemon Version : PX_CloudUploadTerminal_V01_03_07_20210714" << "Process Id :" << proccess_id << endl;                       
            UpdatePID(proccess_id);
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
