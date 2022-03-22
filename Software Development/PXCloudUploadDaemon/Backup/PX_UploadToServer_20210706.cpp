//Raspberry
//g++ -std=c++11 -o PX_UploadToServer_20210706_terminal  PX_UploadToServer_20210706.cpp  -I/usr/include/mariadb/ -lcurl -ljsoncpp -lmysqlclient

//centos
//g++ -std=c++11 -o PX_UploadToServer_20210706_cashier   -I/usr/lib -I/opt/lampp/include -L/opt/lampp/lib PX_UploadToServer_20210706.cpp  -lcurl -ljsoncpp -lmysqlclient


//JSON.h install :   sudo apt-get install libjsoncpp-dev
//curl.h :	sudo apt-get install libcurl4-openssl-dev
//mysql.h:	sudo apt-get install libmysqlclient-dev or libmariadb-dev

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <mysql.h>

#define SERVER "127.0.0.1"
#define USER_CENTOS "parcxservice"
#define DATABASE_APM "parcxAPM"
#define USER_TERMINAL "parcxterminal"
#define DATABASE_TERMINAL "ParcxTerminal"
#define DATABASE_CASHIER "ParcxCashier"
#define PASSWORD "1fromParcx!19514"
#define TERMINAL_PASSWORD "StTa36mHGPg2KfWY"
using namespace std;
Json::Value settings;
Jdon::Value table_settings;

class UploadToServer {
    ofstream logfile;
    time_t current_time;
    struct tm * current_time_info;
    char time_buf[11];
    char date_buf[11];        
        
    string query;
    string jsonstring;
    string response;        
    int i;

    Json::FastWriter fw;
    Json::Reader jsonReader;
    Json::Value jsonResponse;
    Json::Value jsondata;
    Json::Value jsonrow;
    Json::Value jsoninput;
    

    string log_file = "/opt/parcx/Logs/Daemons/PX_UploadToServer_20210706";    
    string data[45];

    CURL *curl;
    CURLcode res;

    MYSQL* mysql_con;
    MYSQL_RES* result;
    MYSQL_ROW row;

public:
    string database;
    string url;
    int s;
    void get_settings();
    void get_tables_for_upload();
    void set_database();
    int mysql_connection();
    void close_function();    
    
    void mysql_process_query();
    void write_log(string, int);
    void PostParkingMovements();
    void PostAccessMovements();
    void PostAlarms();
    void PostBlacklistedTickets();
    void PostValidation();
    void PostPayments();
    void PostCashLevels(int type);
    void PostCashPayout();
    void PostCashRefill();
    void PostParkingReservationMovements();
    void PostManualMovements();
    void PostRevenueShifts();
    void PostPhysicalCashCollected();
    void PostParkingMovementExceptions();
    void PostContractParkingSubscription();
    void PostRevenueCouponUsage();
    void callUploadToServer();
};


/* WRITE LOG FILE */

void UploadToServer::write_log(std::string logdata, int review) {

    if (review <= stoi(settings["review_mode"].asString())) {        
        time(&current_time);
        current_time_info = localtime(&current_time);

        strftime(time_buf, 11, "%H:%M:%S", current_time_info); // Data in format - Day Month  Date Time 
        strftime(date_buf, 11, "%Y-%m-%d", current_time_info); // Date Only for log file name 				
        
        logfile.open(log_file + "_" + std::string(date_buf) + ".log", ios::app);
        logfile << std::string(time_buf) + "  " + logdata<<endl;      
        //cout<<logdata<<endl;
        logfile.close();                     
    }        
}// End . WriteLog



/* MYSQL PROCESS local QUERY */
void UploadToServer::mysql_process_query() {
    if (mysql_query(mysql_con, query.c_str())) {
        write_log(mysql_error(mysql_con), 0);
        result = NULL;

    } else {
        result = mysql_store_result(mysql_con);

    }
}

/* END.  MYSQL PROCESS local QUERY */

void UploadToServer::close_function()
    {  
    try
        {
        mysql_free_result(result);    
        mysql_close(mysql_con);          
        query.clear();    
        response.clear();    
        jsonstring.clear();      
        jsonResponse.clear();   
        }
    catch(const std::exception& e)
        {
        write_log("close_function",0);
        }                             
    }



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

void UploadToServer::set_database()
    {
    try
        {  
        database="";
        mysql_con=mysql_init(NULL);
        if(mysql_real_connect(mysql_con, SERVER, USER_CENTOS,PASSWORD,"information_schema", 0, NULL, 0)!=mysql_con)						
            {
            if(mysql_real_connect(mysql_con, SERVER, USER_TERMINAL,TERMINAL_PASSWORD,"information_schema", 0, NULL, 0)!=mysql_con)
                {
                write_log("DATABASE username password not set  ",0);
                return;	
                }
            }

       
        query="select SCHEMA_NAME from SCHEMATA where SCHEMA_NAME in('"+string(DATABASE_APM)+"','"+string(DATABASE_CASHIER)+"','"+string(DATABASE_TERMINAL)+"')";	
        mysql_process_query();
        if(row = mysql_fetch_row(result))
            {
            database=row[0];
            }
        query.clear();
        mysql_free_result(result);
        mysql_close(mysql_con);		
            							

        write_log("DATABASE: "+database,0);
        }
    catch(const std::exception& e)
        {
        write_log("set_database exception",0);
        write_log(e.what(),0);
        }		
    }

int UploadToServer::mysql_connection()
    {	
    try
        {        			
        mysql_con=mysql_init(NULL);	
        if(database==DATABASE_TERMINAL)			
            {			
            if(mysql_real_connect(mysql_con, SERVER, USER_TERMINAL,TERMINAL_PASSWORD,DATABASE_TERMINAL, 0, NULL, 0)!=mysql_con)
                {                
                write_log(std::string("local db error: ")+mysql_error(mysql_con),0);
                return 0;
                }
            }
        else
            {			
            if(mysql_real_connect(mysql_con, SERVER, USER_CENTOS,PASSWORD,database.c_str(), 0, NULL, 0)!=mysql_con)	
                {                
                write_log(std::string("local db error: ")+mysql_error(mysql_con),0);
                return 0;
                }	
            }       
                
        return 1;				    			 	
        }
    catch(exception &e)
        {
        write_log("mysql_connection exception",0);
        write_log(e.what(),0);
        return 0;
        }
    }/* END . MYSQL CONNECT */



void UploadToServer::get_settings() {
    try {
        if (mysql_connection()) {
            query = "select setting_name,setting_value from device_settings where setting_name in('review_mode','entry_grace_period','device_number','device_name','carpark_number','carpark_name','facility_number','mode_of_operation','parking_zone','post_to_server_webservice_primary','post_to_server_webservice_secondary','device_type','server_ip','secondary_server_ip','upload_to_server_delay') and setting_status=1";
            mysql_process_query();
            while (row = mysql_fetch_row(result)) {
                settings[row[0]] = row[1];
            }
            mysql_free_result(result);
            mysql_close(mysql_con);
        }

        jsonstring = fw.write(settings);
        write_log("Settings: " + jsonstring, 0);
    } catch (exception &e) {
        write_log("get_settings Exception", 0);
        write_log(e.what(), 0);
    }

} // End . Function Get Settings 


void UploadToServer::get_tables_for_upload() {
    try {
        if (mysql_connection()) {
            query = "select table_name,upload_row_limit,enabled from pxcloud_upload_settings";
            mysql_process_query();
            while (row = mysql_fetch_row(result)) {
		table_settings[row[0]]["limit"] = row[1];
                table_settings[row[0]]["enabled"] = row[2];
            }
            mysql_free_result(result);
            mysql_close(mysql_con);
        }

        jsonstring = fw.write(settings);
        write_log("Settings: " + jsonstring, 0);
    } catch (exception &e) {
        write_log("get_settings Exception", 0);
        write_log(e.what(), 0);
    }

} // End . Function Get Table Settings 


void UploadToServer::callUploadToServer() {
    response = "";
    write_log("Request:" + jsonstring, 1);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstring.c_str()); /* data goes here */
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            response = "error";
        } else {
            curl_easy_cleanup(curl);
        }
    }
    write_log("Response:" + response, 1); 
    
    jsonrow.clear();
    jsoninput.clear();
    jsondata.clear();       
    if(jsonReader.parse(response, jsonResponse) && jsonResponse.size()>0)
        mysql_free_result(result);          
}

//Post Parking Movements to Server

void UploadToServer::PostParkingMovements() {
    try {                                                
        if (mysql_connection()) {
            query = "select id,ticket_id,date_time,plate_number,chip_utid,DATE_ADD(date_time,INTERVAL " + settings["entry_grace_period"].asString() + " MINUTE) as entry_grace_period,plate_captured_id,movement_type from parking_movements where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 8; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["operation_mode"] = settings["mode_of_operation"].asString();
                    //jsonrow["movement_type"] = settings["movement_type"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["date_time"] = data[2];
                    jsonrow["plate_number"] = data[3];
                    jsonrow["chip_utid"] = data[4];
                    jsonrow["entry_grace_period"] = data[5];
                    jsonrow["plate_captured_id"] = data[6];
                    jsonrow["movement_type"]=data[7];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "1";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostParkingMovements to Server " + to_string(s), 1);
                callUploadToServer();

                if(jsonReader.parse(response, jsonResponse)){
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_movements set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Movements uploaded", 0);
                } else {
                    write_log("Parking Movements: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostMovements", 1);
                       
        close_function();    
        } // local db connection  				
    } catch (exception &e) {
        write_log("PostParkingMovements :Exception:", 0);
        write_log(e.what(), 0);
    }
} // END Function. Post Parking Movements to Server

//Post Access Movements to Server
void UploadToServer::PostAccessMovements() {
    try {
        if (mysql_connection()) {
            query = "select id,date_time,tag,status_message,tag_serial,tid,epc,epc_list,epc_count,plate_captured_id,ticket_id,plate_number,movement_type from parking_movements_access where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 13; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["operation_mode"] = settings["mode_of_operation"].asString();
                    jsonrow["parking_zone"] = settings["parking_zone"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["date_time"] = data[1];
                    jsonrow["tag"] = data[2];
                    jsonrow["status_message"] = data[3];
                    jsonrow["tag_serial"] = data[4];
                    jsonrow["tid"] = data[5];
                    jsonrow["epc"] = data[6];
                    jsonrow["epc_list"] = data[7];
                    jsonrow["epc_count"] = data[8];
                    jsonrow["plate_captured_id"] = data[9];
                    jsonrow["ticket_id"] = data[10];
                    jsonrow["plate_number"] = data[11];
                    jsonrow["movement_type"] = data[12];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "2";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostAccessMovements to Server " + to_string(s), 1);
                callUploadToServer();
                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_movements_access set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Access Movements  uploaded", 0);
                } else {
                    write_log("Parking Access Movements: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostAccessMovements", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostAccessMovements Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Device Alarms to Server

void UploadToServer::PostAlarms() {
    try {
        if (mysql_connection()) {
            query = "select id,alarm_severity,alarm_code,alarm_description,alarm_date_time,alarm from watchdog_device_alarms where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 6; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_type"] = settings["device_type"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["alarm_severity"] = data[1];
                    jsonrow["alarm_code"] = data[2];
                    jsonrow["alarm_description"] = data[3];
                    jsonrow["alarm_date_time"] = data[4];
                    jsonrow["alarm"] = data[5];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "3";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostAlarms to Server " + to_string(s), 1);
                callUploadToServer();
                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update watchdog_device_alarms set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Watchdog device alarms  uploaded", 0);
                } else {
                    write_log("Watchdog device alarms: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostAlarms", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostAlarms Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Blacklisted tickets  to Server

void UploadToServer::PostBlacklistedTickets() {
    try {
        if (mysql_connection()) {
            query = "select id,ticket_id,date_time,plate_number,chip_utid,blacklisting_reason,blacklisting_description from parking_blacklist where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 7; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["operation_mode"] = settings["mode_of_operation"].asString();
                    jsonrow["movement_type"] = settings["movement_type"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["date_time"] = data[2];
                    jsonrow["plate_number"] = data[3];
                    jsonrow["chip_utid"] = data[4];
                    jsonrow["blacklisting_reason"] = data[5];
                    jsonrow["blacklisting_description"] = data[6];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }

                jsoninput["task"] = "4";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostBlacklistedTickets to Server " + to_string(s), 1);
                callUploadToServer();
                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_blacklist set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Blacklist  uploaded", 0);
                } else {
                    write_log("Parking Blacklist: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostBlacklist", 1);


           close_function();
        }
    } catch (exception &e) {
        write_log("PostBlacklist Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Validations to Server

void UploadToServer::PostValidation() {
    try {
        if (mysql_connection()) {
            query = "select id,ticket_id,validation_value,validation_type,validation_provider,date_time from parking_validation where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 6; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["validation_value"] = data[2];
                    jsonrow["validation_type"] = data[3];
                    jsonrow["validation_provider"] = data[4];
                    jsonrow["date_time"] = data[5];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "5";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostValidation to Server " + to_string(s), 1);
                callUploadToServer();


                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_validation set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Validation  uploaded", 0);
                } else {
                    write_log("Validation: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostValidation", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostValidation Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Payments to Server

void UploadToServer::PostPayments() {
    try {
        if (mysql_connection()) {
            query = "select id,operator_id,operator_name,shift_id,parking_rate_label,parking_rate_name,entry_grace_period,exit_grace_period,vat_type,vat_percentage,ticket_id,entry_date_time,payment_date_time,max_exit_date_time,parking_duration,payment_category,parking_fee,vat_amount,lost_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,gross_amount,amount_received,discount_id,discount_category,discount_name,coupon_id,coupon_category,coupon_source,payment_type,validation_value,validation_id,receipt_number,balance_returned,bank_notes,credit_note,authentication_code,entry_plate_number,exit_plate_number,wallet_points,wallet_username,coupon_value,validator_id,validation_type from revenue_payments where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {

                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 45; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();
                    jsonrow["central_cashier"] = settings["central_cashier"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["operator_id"] = data[1];
                    jsonrow["operator_name"] = data[2];
                    jsonrow["shift_id"] = data[3];
                    jsonrow["parking_rate_label"] = data[4];
                    jsonrow["parking_rate_name"] = data[5];
                    jsonrow["entry_grace_period"] = data[6];
                    jsonrow["exit_grace_period"] = data[7];
                    jsonrow["vat_type"] = data[8];
                    jsonrow["vat_percentage"] = data[9];
                    jsonrow["ticket_id"] = data[10];
                    jsonrow["entry_date_time"] = data[11];
                    jsonrow["payment_date_time"] = data[12];
                    jsonrow["max_exit_date_time"] = data[13];
                    jsonrow["parking_duration"] = data[14];
                    jsonrow["payment_category"] = data[15];
                    jsonrow["parking_fee"] = data[16];
                    jsonrow["vat_amount"] = data[17];
                    jsonrow["lost_fee"] = data[18];
                    jsonrow["admin_fixed_charges"] = data[19];
                    jsonrow["ticket_replacement_fee"] = data[20];
                    jsonrow["discount_amount"] = data[21];
                    jsonrow["gross_amount"] = data[22];
                    jsonrow["amount_received"] = data[23];
                    jsonrow["discount_id"] = data[24];
                    jsonrow["discount_category"] = data[25];
                    jsonrow["discount_name"] = data[26];
                    jsonrow["coupon_id"] = data[27];
                    jsonrow["coupon_category"] = data[28];
                    jsonrow["coupon_source"] = data[29];
                    jsonrow["payment_type"] = data[30];
                    jsonrow["validation_value"] = data[31];
                    jsonrow["validation_id"] = data[32];
                    jsonrow["receipt_number"] = data[33];
                    jsonrow["balance_returned"] = data[34];
                    jsonrow["bank_notes"] = data[35];
                    jsonrow["credit_note"] = data[36];
                    jsonrow["authentication_code"] = data[37];
                    jsonrow["entry_plate_number"] = data[38];
                    jsonrow["exit_plate_number"] = data[39];
                    jsonrow["wallet_points"] = data[40];
                    jsonrow["wallet_username"] = data[41];
                    jsonrow["coupon_value"] = data[42];
                    jsonrow["validator_id"] = data[43];
                    jsonrow["validation_type"] = data[44];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }

                jsoninput["task"] = "6";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;

                jsonstring = fw.write(jsoninput);


                write_log("PostPayments to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update revenue_payments set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Payment  uploaded", 0);
                } else {
                    write_log("Payment: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostPayments", 1);

            close_function();
        }
    } catch (exception &e) {
        write_log("PostPayments Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Cash Levels to Server

void UploadToServer::PostCashLevels(int type) {
    try {
        if (mysql_connection()) {
            if (type == 1) {
                query = "select denomination,current_level,float_level,max_threshold from cashbox_levels where upload_status_server" + to_string(s) + "=0";
                jsoninput["task"] = "7";
            }
            if (type == 2) {
                query = "select denomination,current_level,float_level,max_threshold from recycle_levels where upload_status_server" + to_string(s) + "=0";
                jsoninput["task"] = "8";
            }

            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {

                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 4; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();


                    jsonrow["denomination"] = data[0];
                    jsonrow["current_level"] = data[1];
                    jsonrow["float_level"] = data[2];
                    jsonrow["max_threshold"] = data[3];


                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostCashLevels to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        if (type == 1)
                            query = "update cashbox_levels set upload_status_server" + to_string(s) + "=1 where denomination=" + jsonResponse[i].asString();
                        if (type == 2)
                            query = "update recycle_levels set upload_status_server" + to_string(s) + "=1 where denomination=" + jsonResponse[i].asString();

                        mysql_process_query();
                        mysql_free_result(result);
                    }
                } else {
                    write_log("PostCashLevels: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostCashLevels", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostCashLevels Exception", 0);
        write_log(e.what(), 0);
    }
}


//Post Cash Payout to Server

void UploadToServer::PostCashPayout() {
    try {
        if (mysql_connection()) {
            query = "select id,user,datetime,operation,cashlevel_before_payout,cashlevel_after_payout,notes_payout,total_payout_amount from service_mode_payout where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 8; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();



                    jsonrow["id"] = data[0];
                    jsonrow["user"] = data[1];
                    jsonrow["datetime"] = data[2];
                    jsonrow["operation"] = data[3];
                    jsonrow["cashlevel_before_payout"] = data[4];
                    jsonrow["cashlevel_after_payout"] = data[5];
                    jsonrow["notes_payout"] = data[6];
                    jsonrow["total_payout_amount"] = data[7];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "9";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostCashPayout to Server " + to_string(s), 1);
                callUploadToServer();
                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update service_mode_payout set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " payout  uploaded", 0);
                } else {
                    write_log("PostCashPayout: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostCashPayout", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostCashPayout Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Cash Refill to Server

void UploadToServer::PostCashRefill() {
    try {
        if (mysql_connection()) {
            query = "select id,user,datetime,cashlevel_before_refill,cashlevel_after_refill,notes_refilled,total_amount_refilled from service_mode_refill where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {

                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 7; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["user"] = data[1];
                    jsonrow["datetime"] = data[2];
                    jsonrow["cashlevel_before_refill"] = data[3];
                    jsonrow["cashlevel_after_refill"] = data[4];
                    jsonrow["notes_refilled"] = data[5];
                    jsonrow["total_amount_refilled"] = data[6];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }

                jsoninput["task"] = "10";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostCashRefill to Server " + to_string(s), 1);
                callUploadToServer();
                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update service_mode_refill set upload_status_server" + to_string(s) + "=1 where id=" + jsonResponse[i].asString();
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Cash Refill  uploaded", 0);
                } else {
                    write_log("PostCashRefill: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostCashRefill", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostCashRefill Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Parking Reservation to Server

void UploadToServer::PostParkingReservationMovements() {
    try {
        if (mysql_connection()) {
            query = "select id,ticket_id,date_time,plate_number,plate_captured_id from parking_movements_reservation where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {

                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 6; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();


                    jsonrow["movement_type"] = settings["movement_type"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["date_time"] = data[2];
                    jsonrow["plate_number"] = data[3];
                    jsonrow["plate_captured_id"] = data[4];
                    jsonrow["movement_type"]=data[5];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "11";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostParkingReservationMovements to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_movements_reservation set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Movements Reservation  uploaded", 0);
                } else {
                    write_log("PostParkingReservationMovements: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostReservationMovements", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostParkingReservationMovements Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Manual Movements to Server

void UploadToServer::PostManualMovements() {
    try {
        if (mysql_connection()) {
            query = "select id,date_time,description,operator_id,operator_name,action,reason from parking_movements_manual where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {

                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 7; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["movement_type"] = settings["movement_type"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["date_time"] = data[1];
                    jsonrow["description"] = data[2];
                    jsonrow["operator_id"] = data[3];
                    jsonrow["operator_name"] = data[4];
                    jsonrow["action"] = data[5];
                    jsonrow["reason"] = data[6];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "16";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostManualMovements to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_movements_manual set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Movements Manual  uploaded", 0);
                } else {
                    write_log("PostManualMovements: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostManualMovements", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostManualMovements Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Revenue Shifts to Server

void UploadToServer::PostRevenueShifts() {
    try {
        if (mysql_connection()) {
            query = "select id,shift_id,operator_id,operator_name,login_time,logout_time,shift_duration,parking_fees,vat_amount,lost_fees,admin_fixed_charges,ticket_replacement_fee,discount_amount,shift_earnings,payin_amount,payout_amount,shift_opening_amount,shift_closing_amount,physical_cash_collected,shift_status,wallet_points,credit_card_amount_collected from revenue_shifts where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 23; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();
                    jsonrow["operation_mode"] = settings["mode_of_operation"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["shift_id"] = data[1];
                    jsonrow["operator_id"] = data[2];
                    jsonrow["operator_name"] = data[3];
                    jsonrow["login_time"] = data[4];
                    jsonrow["logout_time"] = data[5];
                    jsonrow["shift_duration"] = data[6];
                    jsonrow["parking_fees"] = data[7];
                    jsonrow["vat_amount"] = data[8];
                    jsonrow["lost_fees"] = data[9];
                    jsonrow["admin_fixed_charges"] = data[10];
                    jsonrow["ticket_replacement_fee"] = data[11];
                    jsonrow["discount_amount"] = data[12];
                    jsonrow["shift_earnings"] = data[13];
                    jsonrow["payin_amount"] = data[14];
                    jsonrow["payout_amount"] = data[15];
                    jsonrow["shift_opening_amount"] = data[16];
                    jsonrow["shift_closing_amount"] = data[17];
                    jsonrow["physical_cash_collected"] = data[18];
                    jsonrow["shift_status"] = data[19];
                    jsonrow["wallet_points"] = data[20]; 
                    jsonrow["credit_card_amount_collected"] = data[21]; 

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "13";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostRevenueShifts to Server " + to_string(s), 1);

                callUploadToServer();


                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update revenue_shifts set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Revenue Shifts  uploaded", 0);
                } else {
                    write_log("PostRevenueShifts: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostRevenueShifts", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostRevenueShifts Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Revenue Physical cash to Server

void UploadToServer::PostPhysicalCashCollected() {
    try {
        if (mysql_connection()) {
            query = "select id,shift_id,user_id,user_name,denomination1,denomination2,denomination3,denomination4,denomination5,denomination6,denomination6,denomination8,denomination9,denomination10,total,date_time from revenue_physical_cash where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 16; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["shift_id"] = data[1];
                    jsonrow["user_id"] = data[2];
                    jsonrow["user_name"] = data[3];
                    jsonrow["denomination1"] = data[4];
                    jsonrow["denomination2"] = data[5];
                    jsonrow["denomination3"] = data[6];
                    jsonrow["denomination4"] = data[7];
                    jsonrow["denomination5"] = data[8];
                    jsonrow["denomination6"] = data[9];
                    jsonrow["denomination7"] = data[10];
                    jsonrow["denomination8"] = data[11];
                    jsonrow["denomination9"] = data[12];
                    jsonrow["denomination10"] = data[13];
                    jsonrow["total"] = data[14];
                    jsonrow["date_time"] = data[15];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "14";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostPhysicalCashCollected to Server " + to_string(s), 1);

                callUploadToServer();


                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update revenue_physical_cash set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Revenue Physical cash  uploaded", 0);
                } else {
                    write_log("PostPhysicalCashCollected: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostPhysicalCashCollected", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostPhysicalCashCollected Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Revenue Subscription to Server

void UploadToServer::PostContractParkingSubscription() {
    try {

        if (mysql_connection()) {
            query = "select id,product_id,product_name,validity_start_date,validity_end_date,product_price,customer_name,company_name,customer_email,customer_mobile,description,validity_days,ticket_id,plate_number,tag,carpark_number,facility_number,date_time from contract_parking_transactions where upload_status_server" + to_string(s) + "=0 limit 25";

            mysql_process_query();
            if (mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 18; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();


                    jsonrow["id"] = data[0];
                    jsonrow["product_id"] = data[1];
                    jsonrow["product_name"] = data[2];
                    jsonrow["validity_start_date"] = data[3];
                    jsonrow["validity_end_date"] = data[4];
                    jsonrow["product_price"] = data[5];
                    jsonrow["customer_name"] = data[6];
                    jsonrow["company_name"] = data[7];
                    jsonrow["customer_email"] = data[8];
                    jsonrow["customer_mobile"] = data[9];
                    jsonrow["description"] = data[10];
                    jsonrow["validity_days"] = data[11];
                    jsonrow["ticket_id"] = data[12];
                    jsonrow["plate_number"] = data[13];
                    jsonrow["tag"] = data[14];
                    jsonrow["carpark_number"] = data[15];
                    jsonrow["facility_number"] = data[16];
                    jsonrow["date_time"] = data[17];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "18";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostContractParkingSubscription to Server " + to_string(s), 1);

                callUploadToServer();


                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update contract_parking_transactions set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Revenue subscription  uploaded", 0);
                } else {
                    write_log("PostContractParkingSubscription: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostContractParkingSubscription", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostContractParkingSubscription Exception", 0);
        write_log(e.what(), 0);
    }
}

//Post Parking Movement Exception to Server

void UploadToServer::PostParkingMovementExceptions() {
    try {

        if (mysql_connection()) {
            query = "select id,ticket_id,date_time,chip_utid,plate_number,exception from parking_movement_exceptions where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 6; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";

                    jsonrow["device_number"] = settings["device_number"].asString();
                    jsonrow["device_name"] = settings["device_name"].asString();
                    jsonrow["carpark_number"] = settings["carpark_number"].asString();
                    jsonrow["carpark_name"] = settings["carpark_name"].asString();
                    jsonrow["facility_number"] = settings["facility_number"].asString();

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["date_time"] = data[2];
                    jsonrow["chip_utid"] = data[3];
                    jsonrow["plate_number"] = data[4];
                    jsonrow["exception"] = data[5];

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "17";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostParkingMovementExceptions to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update parking_movement_exceptions set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Movements Exception  uploaded", 0);
                } else {
                    write_log("PostParkingMovementExceptions: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostParkingMovementExceptions", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostParkingMovementExceptions Exception", 0);
        write_log(e.what(), 0);
    }
}


void UploadToServer::PostRevenueCouponUsage() {
    try {

        if (mysql_connection()) {
            query = "select id,ticket_id,date_time,coupon_id from revenue_coupon_usage where upload_status_server" + to_string(s) + "=0 limit 25";
            mysql_process_query();
            if (result != NULL && mysql_num_rows(result) > 0) {
                while (row = mysql_fetch_row(result)) {
                    for (i = 0; i < 4; i++)
                        row[i] ? data[i] = std::string(row[i]) : data[i] = "";                  

                    jsonrow["id"] = data[0];
                    jsonrow["ticket_id"] = data[1];
                    jsonrow["date_time"] = data[2];
                    jsonrow["coupon_id"] = data[3];                    

                    jsondata.append(jsonrow);
                    jsonrow.clear();
                }
                jsoninput["task"] = "19";
                jsoninput["authentication_key"] = "";
                jsoninput["device_number"] = settings["device_number"].asString();
                jsoninput["data"] = jsondata;
                jsonstring = fw.write(jsoninput);

                write_log("PostRevenueCouponUsage to Server " + to_string(s), 1);
                callUploadToServer();

                if (jsonReader.parse(response, jsonResponse)) {
                    for (i = 0; i < jsonResponse.size(); i++) {
                        query = "update revenue_coupon_usage set upload_status_server" + to_string(s) + "=1 where id='" + jsonResponse[i].asString() + "'";
                        mysql_process_query();
                        mysql_free_result(result);
                    }
                    write_log(to_string(jsonResponse.size()) + " Parking Movements Exception  uploaded", 0);
                } else {
                    write_log("PostRevenueCouponUsage: Could not parse HTTP data as JSON", 0);
                }
            } else
                write_log("No data to PostRevenueCouponUsage", 1);


            close_function();
        }
    } catch (exception &e) {
        write_log("PostRevenueCouponUsage Exception", 0);
        write_log(e.what(), 0);
    }
}


//create daemon

void createDaemon() {
    pid_t pid;
    pid = fork();
    pid_t sid = 0;
    if (pid < 0) {
        cout << "Fork failed";
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        cout << "PX_UploadToServer,Process id=" << pid << endl;
        exit(EXIT_SUCCESS);
    }
    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(void) {
    createDaemon();
    UploadToServer main_obj;       
    settings["review_mode"] = "0";
    settings["movement_type"]=1;
    main_obj.write_log("Daemon Started :  PX_UploadToServer_20210706", 0);
    main_obj.write_log("**************************************************", 0);    
    settings["upload_to_server_delay"] = "5"; 
    main_obj.set_database();
    if(main_obj.database=="")
        {
        main_obj.write_log("Cant set database exiting application",0);
        return 1;
        }
    main_obj.get_settings();
    

    int s = 0, n = 0;
    if ((settings["post_to_server_webservice_primary"].asString() != "0")&&(settings["post_to_server_webservice_primary"].asString() != ""))
        n = 1;
    if ((settings["post_to_server_webservice_secondary"].asString() != "0")&&(settings["post_to_server_webservice_secondary"].asString() != ""))
        n = 2;
    
    if (settings.size() > 1) {
        int device_type = stoi(settings["device_type"].asString());
        if (device_type != 1)
            settings["movement_type"] = 2;
        
        while (1) {
            for (s = 1; s <= n; s++) {
                main_obj.s=s;
                if (s == 1)
                    main_obj.url = "http://" + settings["server_ip"].asString() + settings["post_to_server_webservice_primary"].asString();
                else
                    main_obj.url = "http://" + settings["secondary_server_ip"].asString() + settings["post_to_server_webservice_secondary"].asString();

                //Entry exit	                
         
                    main_obj.PostParkingMovements();
                    sleep(1);
                    main_obj.PostAccessMovements();
                    sleep(1);
                    main_obj.PostParkingReservationMovements();
                    sleep(1);
                    main_obj.PostManualMovements();
                    sleep(1);
                    main_obj.PostParkingMovementExceptions();
                    sleep(1);
                    main_obj.PostAlarms();
                    sleep(1);
                    if (device_type == 1)
                        main_obj.PostBlacklistedTickets();
                    else
                        {
                        main_obj.PostValidation();
                        sleep(1);
                        main_obj.PostRevenueCouponUsage();
                        }
                
            main_obj.url.clear();
            }
            sleep(stoi(settings["upload_to_server_delay"].asString()));
        }
    }
    return 0;
} // END  MAIN 




