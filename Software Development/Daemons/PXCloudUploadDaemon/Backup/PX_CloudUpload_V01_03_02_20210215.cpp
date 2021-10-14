//sudo g++ -std=c++11 -Wall -o PX_CloudUpload_V01_03_02_20210215 PX_CloudUpload_V01_03_02_20210215.cpp PX_GeneralOperations.cpp -ljsoncpp -lmysqlcppconn -lpthread -lcurl -I/opt/lampp/include/ -L/opt/parcx/lib/
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
        conn = general.mysqlConnect("parcx_server");
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
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select  setting_name,setting_value from system_settings where setting_name in('cloud_operator_id','facility_number','facility_name','cloud_upload_webservice')");
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
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        stmt->executeUpdate("Update pxcloud_upload_settings set cloud_upload_date_time=CURRENT_TIMESTAMP() where table_name = '" + table_name + "'");
        delete stmt;
        delete conn;
    } catch (sql::SQLException &e) {
        WriteException("UpdatePXCloudUploadTime", e.what());
    }
}

Json::Value PostParkingOccupancy(int rowlimit, int task) 
    {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;  
    Json::Value json;
    try 
        {        
        conn = general.mysqlConnect("parcx_dashboard");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from counters where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;                       
            while (res->next()) 
                {
                row["id"] = res->getInt("parking_counter_id");
                row["pgs_counter_id"] = res->getInt("pgs_counter_id");
                row["facility_number"] = stoi(facility_number);
                row["count_category"] = res->getInt("count_category");
                row["counter_type"] = res->getInt("counter_type");
                row["counter_name"] = res->getString("counter_name").asStdString();
                row["carpark_name"] = res->getString("carpark_name").asStdString();
                row["dashboard_order"] = res->getInt("dashboard_order");
                row["carpark_number"] = res->getInt("carpark_number");
                row["device_number"] = res->getInt("device_number");

                row["correction_factor"] = res->getInt("correction_factor");
                row["total_spaces"] = res->getInt("total_spaces");
                row["current_level"] = res->getInt("current_level");
                row["occupancy_threshold"] = res->getInt("occupancy_threshold");
                row["free_spaces"] = res->getInt("free_spaces");
                row["reservation_current_level"] = res->getInt("reservation_current_level");
                row["access_current_level"] = res->getInt("access_current_level");
                row["shortterm_current_level"] = res->getInt("shortterm_current_level");
                row["total_shortterm_spaces"] = res->getInt("total_shortterm_spaces");
                row["total_reservation_spaces"] = res->getInt("total_reservation_spaces");

                row["total_access_spaces"] = res->getInt("total_access_spaces");
                row["total_entries"] = res->getInt("total_entries");
                row["total_exits"] = res->getInt("total_exits");
                row["total_manual_entry"] = res->getInt("total_manual_entry");
                row["total_manual_exit"] = res->getInt("total_manual_exit");
                row["blacklisted_tickets"] = res->getInt("blacklisted_tickets");
                row["server_manual_opens"] = res->getInt("server_manual_opens");
                row["external_manual_open"] = res->getInt("external_manual_open");
                row["access_transactions"] = res->getInt("access_transactions");
                row["reservation_transactions"] = res->getInt("reservation_transactions");

                row["occupancy_percentage"] = res->getString("occupancy_percentage").asStdString();
                row["shortterm_entry"] = res->getInt("shortterm_entry");
                row["shortterm_exit"] = res->getInt("shortterm_exit");
                row["access_entry"] = res->getInt("access_entry");
                row["access_exit"] = res->getInt("access_exit");
                row["reservation_entry"] = res->getInt("reservation_entry");
                row["reservation_exit"] = res->getInt("reservation_exit");
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;            
            }
        delete res;
        delete stmt;
        delete conn;

        } 
    catch (const std::exception& e) 
        {        
        WriteException("PostParkingOccupancy", e.what());       
        }
    return json;
    }

Json::Value PostAlarms(int rowlimit, int task) {   
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value json;  
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from watchdog_device_alarms where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;                                   
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();               
                row["alarm_severity"] = res->getInt("alarm_severity");
                row["alarm_code"] = res->getString("alarm_code").asStdString();
                row["alarm_description"] = res->getString("alarm_description").asStdString();               
                row["alarm_date_time"] = res->getString("alarm_date_time").asStdString();
                row["create_date_time"] = res->getString("create_date_time").asStdString();
                row["dismiss"] = res->getInt("dismiss");
                row["carpark_number"] = res->getInt("carpark_number");                
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;            
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostAlarms", e.what());       
        }
    return json;
    }

Json::Value PostDeviceStatus(int rowlimit, int task) 
    {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res; 
    Json::Value json;
    try 
        {        
        conn = general.mysqlConnect("parcx_dashboard");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from watchdog_device_status where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;            
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["carpark_name"] = res->getString("carpark_name").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_name"] = res->getString("facility_name").asStdString();
                row["facility_number"] = stoi(facility_number);
                row["device_name"] = res->getString("device_name").asStdString();
                row["device_number"] = res->getInt("device_number");
                row["device_type"] = res->getInt("device_type");
                row["device_ip"] = res->getString("device_ip").asStdString();
                row["device_network_status"] = res->getInt("device_network_status");               
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {        
        WriteException("PostDeviceStatus", e.what());        
        }
    return json;
    }

Json::Value PostOpenTransactions(int rowlimit, int task) 
    {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;  
    Json::Value json;
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from open_transactions where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;            
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_number"] = stoi(facility_number);
                row["ticket_id"] = res->getString("ticket_id").asStdString();
                row["chip_utid"] = res->getString("chip_utid").asStdString();
                row["plate_number"] = res->getString("plate_number").asStdString();
                row["entry_grace_period"] = res->getString("entry_grace_period").asStdString();
                row["total_amount_paid"] = res->getString("total_amount_paid").asStdString();
                row["last_payment_date_time"] = res->getString("last_payment_date_time").asStdString();
                row["max_exit_date_time"] = res->getString("max_exit_date_time").asStdString();
                row["entry_date_time"] = res->getString("entry_date_time").asStdString();
                row["entry_type"] = res->getInt("entry_type");
                row["total_validation_hours"] = res->getInt("total_validation_hours");
                row["parking_rate"] = res->getString("parking_rate").asStdString();
                row["operation_mode"] = res->getString("operation_mode").asStdString();
                row["movement_type"] = res->getInt("movement_type");
                row["creation_date_time"] = res->getString("creation_date_time").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                                   
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {       
        WriteException("PostOpenTransactions", e.what());       
        }
    return json;
    }

Json::Value PostParkingMovementsAccess(int rowlimit, int task) {   
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value json;    
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from parking_movements_access where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;            
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["date_time"] = res->getString("date_time").asStdString();
                row["tag_serial"] = res->getString("tag_serial").asStdString();
                row["tag"] = res->getString("tag").asStdString();
                row["plate_number"] = res->getString("plate_number").asStdString();
                row["status_message"] = res->getString("status_message").asStdString();
                row["epc"] = res->getString("epc").asStdString();
                row["tid"] = res->getString("tid").asStdString();
                row["epc_count"] = res->getInt("epc_count");
                row["epc_list"] = res->getString("epc_list").asStdString();
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["movement_type"] = res->getInt("movement_type");
                row["parking_zone"] = res->getString("parking_zone").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["carpark_name"] = res->getString("carpark_name").asStdString();
                row["facility_number"] = stoi(facility_number);               
                row["plate_captured_id"] = res->getInt("plate_captured_id");
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 

        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostParkingMovementsAccess", e.what());        
        }
    return json;
    }

Json::Value PostRevenuePayments(int rowlimit, int task) {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;   
    Json::Value json;    
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from revenue_payments where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;    
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["operator_id"] = res->getInt("operator_id");
                row["operator_name"] = res->getString("operator_name").asStdString();
                row["shift_id"] = res->getString("shift_id").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["carpark_name"] = res->getString("carpark_name").asStdString();
                row["parking_rate_label"] = res->getString("parking_rate_label").asStdString();
                row["parking_rate_name"] = res->getString("parking_rate_name").asStdString();
                row["entry_grace_period"] = res->getString("entry_grace_period").asStdString();
                row["exit_grace_period"] = res->getString("exit_grace_period").asStdString();
                row["vat_type"] = res->getString("vat_type").asStdString();
                row["vat_percentage"] = res->getInt("vat_percentage");
                row["ticket_id"] = res->getString("ticket_id").asStdString();
                row["entry_date_time"] = res->getString("entry_date_time").asStdString();
                row["payment_date_time"] = res->getString("payment_date_time").asStdString();
                row["max_exit_date_time"] = res->getString("max_exit_date_time").asStdString();
                row["parking_duration"] = res->getString("parking_duration").asStdString();
                row["payment_category"] = res->getString("payment_category").asStdString();
                row["parking_fee"] = res->getString("parking_fee").asStdString();
                row["vat_amount"] = res->getString("vat_amount").asStdString();
                row["lost_fee"] = res->getString("lost_fee").asStdString();
                row["admin_fixed_charges"] = res->getString("admin_fixed_charges").asStdString();
                row["ticket_replacement_fee"] = res->getString("ticket_replacement_fee").asStdString();
                row["discount_amount"] = res->getString("discount_amount").asStdString();
                row["gross_amount"] = res->getString("gross_amount").asStdString();
                row["amount_received"] = res->getString("amount_received").asStdString();
                row["discount_id"] = res->getInt("discount_id");
                row["discount_category"] = res->getString("discount_category").asStdString();
                row["discount_name"] = res->getString("discount_name").asStdString();
                row["coupon_id"] = res->getString("coupon_id").asStdString();
                row["coupon_category"] = res->getString("coupon_category").asStdString();
                row["coupon_source"] = res->getString("coupon_source").asStdString();
                row["product_id"] = res->getInt("product_id");
                row["product_quantity"] = res->getInt("product_quantity");
                row["payment_type"] = res->getString("payment_type").asStdString();
                row["authentication_code"] = res->getString("authentication_code").asStdString();
                row["void"] = res->getString("void").asStdString();
                row["record_creation_time"] = res->getString("record_creation_time").asStdString();
                row["facility_number"] = stoi(facility_number);
                row["entry_plate_number"] = res->getString("entry_plate_number").asStdString();
                row["exit_plate_number"] = res->getString("exit_plate_number").asStdString();
                row["anpr_comparison_number"] = res->getInt("anpr_comparison_number");
                row["validation_id"] = res->getString("validation_id").asStdString();
                row["validation_value"] = res->getString("validation_value").asStdString();
                row["void_user"] = res->getString("void_user").asStdString();
                row["void_reason"] = res->getString("void_reason").asStdString();
                row["void_datetime"] = res->getString("void_datetime").asStdString();
                row["receipt_number"] = res->getInt("receipt_number");
                row["customer_card_id"] = res->getInt("customer_card_id");
                row["bank_notes"] = res->getString("bank_notes").asStdString();
                row["balance_returned"] = res->getString("balance_returned").asStdString();
                row["credit_note"] = res->getString("credit_note").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 

        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostRevenuePayments", e.what());        
        }
    return json;
    }

Json::Value Post_hourly_occupancy(int rowlimit, int task) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value json;
         
    try 
        {        
        conn = general.mysqlConnect("parcx_dashboard");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from hourly_occupancy where data='realtime' and cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;  
            int j;  
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["facility_number"] = stoi(facility_number);
                row["carpark_number"] = res->getInt("carpark_number");
                row["type"] = res->getString("type").asStdString();
                row["data"] = "realtime";
                for (j = 0; j < 24; j++) 
                    {
                    row["h" + to_string(j) + "to" + (to_string(j + 1))] = res->getInt("h" + to_string(j) + "to" + (to_string(j + 1)));
                    }
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;            
            } 
        delete res;
        delete stmt;
        delete conn;

        } 
    catch (const std::exception& e) 
        {
        WriteException("Post_hourly_occupancy", e.what());        
        }
    return json;
    }

Json::Value Post_parking_revenue(int rowlimit, int task) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value json;
       
    try 
        {        
        conn = general.mysqlConnect("parcx_dashboard");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from parking_revenue where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;            
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["facility_number"] = stoi(facility_number);
                row["carpark_number"] = res->getInt("carpark_number");
                row["device_type"] = res->getInt("device_type");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["network_status"] = res->getInt("network_status");
                row["gross_amount"] = res->getString("gross_amount").asStdString();
                row["parking_fee"] = res->getString("parking_fee").asStdString();
                row["vat_amount"] = res->getString("vat_amount").asStdString();
                row["lost_ticket_fee"] = res->getString("lost_ticket_fee").asStdString();
                row["admin_fixed_charges"] = res->getString("admin_fixed_charges").asStdString();
                row["ticket_replacement_fee"] = res->getString("ticket_replacement_fee").asStdString();
                row["discount_amount"] = res->getString("discount_amount").asStdString();
                row["product_sale_amount"] = res->getString("product_sale_amount").asStdString();
                row["sms_payment_amount"] = res->getString("sms_payment_amount").asStdString();
                row["creditcard_payment_amount"] = res->getString("creditcard_payment_amount").asStdString();
                row["payable_entries_count"] = res->getInt("payable_entries_count");
                row["lost_ticket_count"] = res->getInt("lost_ticket_count");
                row["discount_count"] = res->getInt("discount_count");
                row["sms_payment_count"] = res->getInt("sms_payment_count");
                row["creditcard_payment_count"] = res->getInt("creditcard_payment_count");
                row["product_sale_count"] = res->getInt("product_sale_count");
                row["last_transaction"] = res->getString("last_transaction").asStdString();
                row["last_updated_date_time"] = res->getString("last_updated_date_time").asStdString();

                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                       
            } 

        delete res;
        delete stmt;
        delete conn;

        } 
    catch (const std::exception& e) 
        {
        WriteException("Post_parking_revenue", e.what());       
        }
    return json;
    }

Json::Value PostParkingDuration(int rowlimit, int task) {             
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::Value json;        
    try 
        {        
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select * from parking_duration where cloud_upload_status = 0 limit " + to_string(rowlimit));
        
        if (rs->rowsCount() > 0) 
            {
            Json::Value jsonarray(Json::arrayValue);
            Json::Value duration;
            int j=0;
            while (rs->next()) 
                {
                duration["id"] = rs->getInt("id");
                duration["report_date"] = rs->getString("report_date").asStdString();
                duration["report_day"] = rs->getString("report_day").asStdString();
                duration["category"] = rs->getString("category").asStdString();
                duration["m0to30"] = rs->getInt("m0to30");
                duration["m30to60"] = rs->getInt("m30to60");
                for (j = 1; j < 24; j++) {
                    duration["h" + to_string(j) + "to" + (to_string(j + 1))] = rs->getInt("h" + to_string(j) + "to" + (to_string(j + 1)));
                }
                duration["d1to2"] = rs->getInt("d1to2");
                duration["d2to3"] = rs->getInt("d2to3");
                duration["maximum_parking_duration"] = rs->getInt("maximum_parking_duration");
                duration["minimum_parking_duration"] = rs->getInt("minimum_parking_duration");
                duration["average_parking_duration"] = stod(rs->getString("average_parking_duration"));
                duration["carpark_number"] = rs->getInt("carpark_number");
                duration["facility_number"]=stoi(facility_number); 
                jsonarray.append(duration);
                }
            json["data"] = jsonarray;
            json["task"] = task;                      

            } 
        delete rs;
        delete stmt;
        delete conn;

        } 
    catch (std::exception &e) 
        {       
        WriteException("PostParkingDuration", e.what());       
        }
    return json;
    }

Json::Value PostRevenueSummary(int rowlimit, int task) {                    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::Value json;   

    try 
        {        
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select * from summary_daily_revenue where cloud_upload_status = 0 limit " + to_string(rowlimit));
       
        if (rs->rowsCount() > 0) 
            {
            Json::Value jsonarray(Json::arrayValue);
            Json::Value revenue;
            while (rs->next()) 
                {
                revenue["id"] = rs->getInt("id");
                revenue["report_date"] = rs->getString("report_date").asStdString();
                revenue["report_day"] = rs->getString("report_day").asStdString();
                revenue["network_status"] = rs->getInt("network_status");
                revenue["gross_amount"] = rs->getString("gross_amount").asStdString();
                revenue["parking_fee"] = rs->getString("parking_fee").asStdString();
                revenue["vat_amount"] = rs->getString("vat_amount").asStdString();
                revenue["lost_ticket_fee"] = rs->getString("lost_ticket_fee").asStdString();
                revenue["admin_fixed_charges"] = rs->getString("admin_fixed_charges").asStdString();
                revenue["ticket_replacement_charges"] = rs->getString("ticket_replacement_charges").asStdString();
                revenue["discount_amount"] = rs->getString("discount_amount").asStdString();
                revenue["sms_payment_amount"] = rs->getString("sms_payment_amount").asStdString();
                revenue["creditcard_payment_amount"] = rs->getString("creditcard_payment_amount").asStdString();
                revenue["rfid_prepaid"] = rs->getString("rfid_prepaid").asStdString();
                revenue["complimentary_coupons_count"] = rs->getInt("complimentary_coupons_count");
                revenue["complimentary_handicap_count"] = rs->getInt("complimentary_handicap_count");

                revenue["payable_entries_count"] = rs->getInt("payable_entries_count");
                revenue["lost_ticket_count"] = rs->getInt("lost_ticket_count");
                revenue["discount_count"] = rs->getInt("discount_count");
                revenue["sms_payment_count"] = rs->getInt("sms_payment_count");
                revenue["creditcard_payment_count"] = rs->getInt("creditcard_payment_count");
                revenue["comments"] = rs->getString("comments").asStdString();
                revenue["carpark_number"] = rs->getInt("carpark_number");
                revenue["facility_number"] = stoi(facility_number);
                revenue["device_name"] = rs->getString("device_name").asStdString();                
                revenue["device_number"] = rs->getInt("sms_payment_count");
                jsonarray.append(revenue);
                }
            json["data"] = jsonarray;
            json["task"] = task;            
        } 
        delete rs;
        delete stmt;
        delete conn;

        } 
    catch (std::exception &e) 
        {              
        WriteException("PostRevenueSummary", e.what());        
        }
    return json;
    }

Json::Value PostParkingMovementsSummary(int rowlimit, int task) {               
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    
    Json::Value json;

    try 
        {        
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select * from summary_parking_movements where cloud_upload_status = 0 limit " + to_string(rowlimit));        
        if (rs->rowsCount() > 0) 
            {
            Json::Value jsonarray(Json::arrayValue);
            Json::Value duration;
            int j;
            while (rs->next()) 
                {

                duration["id"] = rs->getInt("id");
                duration["report_date"] = rs->getString("report_date").asStdString();
                duration["report_day"] = rs->getString("report_day").asStdString();
                duration["category"] = rs->getInt("category");
                for (j = 0; j < 24; j++) {
                    duration["h" + to_string(j) + "to" + (to_string(j + 1))] = rs->getInt("h" + to_string(j) + "to" + (to_string(j + 1)));
                }
                duration["reproccessing_flag"] = rs->getInt("flag");
                duration["total_count"] = rs->getInt("total_count");
                duration["movement_type"] = rs->getInt("movement_type");                
                duration["comment"] = rs->getString("comment").asStdString();                
                duration["carpark_number"] = rs->getInt("car_park_number");
                duration["device_number"] = rs->getInt("device_number");
                duration["facility_number"]=stoi(facility_number);
                jsonarray.append(duration);
                }
            json["data"] = jsonarray;
            json["task"] = task;            
            
            } 
        delete rs;
        delete stmt;
        delete conn;

        } 
    catch (std::exception &e) 
        {                
        WriteException("PostParkingMovementsSummary", e.what());      
        }
    return json;
    }

Json::Value PostParkingMovementsManual(int rowlimit, int task) {   
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Json::Value json;                        
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from parking_movements_manual where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["operator_id"] = res->getString("operator_id").asStdString();
                row["operator_name"] = res->getString("operator_name").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_number"] = stoi(facility_number);
                row["movement_type"] = res->getInt("movement_type");
                row["date_time"] = res->getString("date_time").asStdString();
                row["create_date_time"] = res->getString("create_date_time").asStdString();
                row["action"] = res->getString("action").asStdString();
                row["reason"] = res->getString("reason").asStdString();
                row["description"] = res->getString("description").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                                   
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostParkingMovementsManual", e.what());        
        }
    return json;
    }

Json::Value PostRevenueShifts(int rowlimit, int task) {   
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Json::Value json;                  
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from revenue_shifts where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["device_number"] = res->getInt("device_number");
                row["device_name"] = res->getString("device_name").asStdString();
                row["operator_id"] = res->getInt("operator_id");
                row["operator_name"] = res->getString("operator_name").asStdString();
                row["shift_id"] = res->getString("shift_id").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_number"] = stoi(facility_number);
                row["login_time"] = res->getString("login_time").asStdString();
                row["logout_time"] = res->getString("logout_time").asStdString();
                row["shift_duration"] = res->getString("shift_duration").asStdString();
                row["parking_fees"] = res->getString("parking_fees").asStdString();
                row["vat_amount"] = res->getString("vat_amount").asStdString();
                row["lost_fee"] = res->getString("lost_fee").asStdString();
                row["admin_fixed_charges"] = res->getString("admin_fixed_charges").asStdString();
                row["ticket_replacement_fee"] = res->getString("ticket_replacement_fee").asStdString();
                row["discount_amount"] = res->getString("discount_amount").asStdString();
                row["shift_earnings"] = res->getString("shift_earnings").asStdString();
                row["payin_amount"] = res->getString("payin_amount").asStdString();
                row["payout_amount"] = res->getString("payout_amount").asStdString();
                row["shift_opening_amount"] = res->getString("shift_opening_amount").asStdString();
                row["shift_closing_amount"] = res->getString("shift_closing_amount").asStdString();
                row["physical_cash_collected"] = res->getString("physical_cash_collected").asStdString();
                row["wallet_points"] = res->getString("wallet_points").asStdString();
                row["shift_status"] = res->getInt("shift_status");
                row["last_updated_date_time"] = res->getString("last_updated_date_time").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 

        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostRevenueShifts", e.what());       
        }
    return json;
    }

Json::Value PostAccessWhitelist(int rowlimit, int task) {
    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Json::Value json;        
    try 
        {
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from access_whitelist where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;
            while (res->next()) 
                {
                row["id"] = res->getInt("access_whitelist_id");
                row["access_id"] = res->getInt("access_id");
                row["plate_number"] = res->getString("plate_number").asStdString();
                row["wallet_user_id"] = res->getInt("wallet_user_id");
                row["operator_id"] = cloud_operator_id;
                row["start_date"] = res->getString("validity_start_date").asStdString();
                row["expiry_date"] = res->getString("validity_expiry_date").asStdString();
                row["wallet_payment"] = res->getInt("wallet_payment");
                row["season_card"] = res->getInt("season_card");
                row["corporate_parker"] = res->getInt("corporate_parker");
                row["antipassback"] = res->getInt("antipassback_enabled");
                row["access_zones"] = res->getString("access_zones").asStdString();
                row["creation_date"] = res->getString("creation_date_time").asStdString();
                row["status"] = res->getInt("status");
                row["last_visited"] = res->getString("last_visited_date_time").asStdString();
                row["company_name"] = res->getString("company_name").asStdString();
                row["customer_name"] = res->getString("customer_name").asStdString();
                row["tag"] = res->getString("tag").asStdString();
                row["country"] = res->getString("country").asStdString();
                row["carpark_number"] = res->getString("carpark_number").asStdString();
                row["facility_number"] = stoi(facility_number);
                row["ticket_id"] = res->getString("ticket_id").asStdString();
                row["tag_serial"] = res->getString("tag_serial").asStdString();
                row["tag_tid"] = res->getString("tag_tid").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 

        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostAccessWhitelist", e.what());       
        }
    return json;
    }

Json::Value PostParkingReservation(int rowlimit, int task) {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Json::Value json;       
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from parking_reservation where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;
            while (res->next()) 
                {
                row["id"] = res->getInt("reservation_id");
                row["transaction_id"] = res->getString("transaction_id").asStdString();
                row["mobile_app_user_id"] = res->getInt("mobile_app_user_id");
                row["plate_number"] = res->getString("plate_number").asStdString();
                row["ticket_id"] = res->getString("ticket_id").asStdString();
                row["name"] = res->getString("name").asStdString();
                row["address"] = res->getString("address").asStdString();
                row["country"] = res->getString("country").asStdString();
                row["phone"] = res->getString("phone").asStdString();
                row["email"] = res->getString("email").asStdString();
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_number"] = stoi(facility_number);
                row["parking_bay_number"] = res->getString("parking_bay_number").asStdString();
                row["from_time"] = res->getString("from_time").asStdString();
                row["to_time"] = res->getString("to_time").asStdString();
                row["creation_date_time"] = res->getString("creation_date_time").asStdString();
                row["reserved_parking_duration"] = res->getString("reserved_parking_duration").asStdString();
                row["reserved_parking_fee"] = res->getString("reserved_parking_fee").asStdString();
                row["payment_reference_id"] = res->getString("payment_reference_id").asStdString();
                row["payment_flag"] = res->getInt("payment_flag");
                row["cancel_flag"] = res->getInt("cancel_flag");               
                row["used_flag"] = res->getInt("used_flag");
                row["operator_id"]=cloud_operator_id;
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e) 
        {
        WriteException("PostParkingReservation", e.what());       
        }
    return json;
    }

Json::Value PostProductSubscription(int rowlimit, int task) {    
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Json::Value json;   
    try 
        {
        conn = general.mysqlConnect("parcx_reporting");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from contract_parking_transactions where cloud_upload_status=0 limit " + to_string(rowlimit));
        if (res->rowsCount() > 0) 
            {
            Json::Value array,row;
            while (res->next()) 
                {
                row["id"] = res->getInt("id");
                row["cloud_operator_id"] = cloud_operator_id;
                row["product_id"] = res->getInt("product_id");
                row["product_name"] = res->getString("product_name").asStdString();
                row["validity_start_date"] = res->getString("validity_start_date").asStdString();
                row["validity_end_date"] = res->getString("validity_end_date").asStdString();
                row["product_price"] = res->getString("product_price").asStdString();
                row["date_time"] = res->getString("date_time").asStdString();
                row["customer_name"] = res->getString("customer_name").asStdString();
                row["company_name "] = res->getString("company_name").asStdString();
                row["customer_email"] = res->getString("customer_email").asStdString();
                row["customer_mobile"] = res->getString("customer_mobile").asStdString();
                row["description"] = res->getString("description").asStdString();
                row["validity_days"] = res->getInt("validity_days");
                row["carpark_number"] = res->getInt("carpark_number");
                row["facility_number"] = stoi(facility_number);
                row["plate_number"] = res->getString("plate_number").asStdString();
                row["tag"] = res->getString("tag").asStdString();
                row["ticket_id"] = res->getString("ticket_id").asStdString();
                array.append(row);
                }
            json["data"] = array;
            json["task"] = task;                        
            } 
        delete res;
        delete stmt;
        delete conn;
        } 
    catch (const std::exception& e)     
        {
        WriteException("PostProductSubscription", e.what());        
        }
    return json;
    }

void UpdateUploadStatus(string sql, string db,string table) 
    {
    sql::Connection *conn;
    sql::Statement *stmt;
    try 
        {
        conn = general.mysqlConnect(db);
        stmt = conn->createStatement();
        stmt->executeUpdate(sql);
        sql="Update parcx_server.pxcloud_upload_settings set cloud_upload_date_time=CURRENT_TIMESTAMP() where table_name = '" + table + "'";
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

                    if (array["response"][j]["table"] == "open_transactions") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update open_transactions set cloud_upload_status = 1 where id in (" + id_string + ") and movement_type=1";
                            UpdateUploadStatus(sql, "parcx_reporting","open_transactions");                            
                            }
                        
                        //To delete the records with movement type 2
                        id_string = "";
                        for (i = 0; i < (signed)array["response"][j]["del"].size(); i++) 
                            {
                            id_string = id_string + array["response"][j]["del"][i].asString();
                            if (i < (signed)(array["response"][j]["del"].size() - 1))                                 
                                id_string = id_string + ",";                                
                            }                        
                        
                        if (id_string > "") 
                            {
                            WriteToLog("PostOpenTransactions", "Delete ids:" + id_string);
                            sql = "Delete from open_transactions where id in (" + id_string + ") and movement_type=2";
                            UpdateUploadStatus(sql, "parcx_reporting","open_transactions");                           
                            }
                        } 
                    else if (array["response"][j]["table"] == "counters") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update counters set cloud_upload_status = 1 where parking_counter_id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_dashboard","counters");                            
                            }                            
                        } 
                    else if (array["response"][j]["table"] == "parking_movements_access") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_movements_access set cloud_upload_status = 1,upload_date_time=CURRENT_TIMESTAMP() where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","parking_movements_access");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "revenue_payments") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update revenue_payments set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","revenue_payments");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "parking_duration") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_duration set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","parking_duration");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "summary_daily_revenue") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update summary_daily_revenue set cloud_upload_status = 1,last_updated_date_time = CURRENT_TIMESTAMP() where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","summary_daily_revenue");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "summary_parking_movements") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update summary_parking_movements set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","summary_parking_movements");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "hourly_occupancy") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update hourly_occupancy set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_dashboard","hourly_occupancy");                            
                            }                        
                        } 
                    else if (array["response"][j]["table"] == "parking_revenue") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update parking_revenue set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_dashboard","parking_revenue");                           
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
                    else if (array["response"][j]["table"] == "revenue_shifts") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update revenue_shifts set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","revenue_shifts");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "watchdog_device_status") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update watchdog_device_status set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_dashboard","watchdog_device_status");                            
                            }                        
                        } 
                    else if (array["response"][j]["table"] == "watchdog_device_alarms") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update watchdog_device_alarms set cloud_upload_status = 1 where id in (" + id_string + ")";
                            UpdateUploadStatus(sql, "parcx_reporting","watchdog_device_alarms");                            
                            }
                        } 
                    else if (array["response"][j]["table"] == "access_whitelist") 
                        {
                        if (id_string > "") 
                            {
                            sql = "Update access_whitelist set cloud_upload_status = 1 where access_whitelist_id in (" + id_string + ")";                            
                            UpdateUploadStatus(sql, "parcx_server","access_whitelist");                            
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

Json::Value TableList(string table, int rowlimit, int task) {
    Json::Value response;
    if (table == "counters") 
        response = PostParkingOccupancy(rowlimit, task);
    else if (table == "watchdog_device_alarms") 
        response = PostAlarms(rowlimit, task);
    else if (table == "watchdog_device_status") 
        response = PostDeviceStatus(rowlimit, task);
    else if (table == "open_transactions") 
        response = PostOpenTransactions(rowlimit, task);
    else if (table == "parking_movements_access")
        response = PostParkingMovementsAccess(rowlimit, task);
    else if (table == "revenue_payments") 
        response = PostRevenuePayments(rowlimit, task);
    else if (table == "parking_revenue")
        response = Post_parking_revenue(rowlimit, task);
    else if (table == "hourly_occupancy")
        response = Post_hourly_occupancy(rowlimit, task);
    else if (table == "parking_duration")
        response = PostParkingDuration(rowlimit, task);
    else if (table == "summary_daily_revenue") 
        response = PostRevenueSummary(rowlimit, task);
    else if (table == "summary_parking_movements")         
        response = PostParkingMovementsSummary(rowlimit, task);
    else if (table == "summary_parking_movements") 
        response = PostParkingMovementsSummary(rowlimit, task);
    else if (table == "parking_movements_manual") 
        response = PostParkingMovementsManual(rowlimit, task);
    else if (table == "revenue_shifts") 
        response = PostRevenueShifts(rowlimit, task);
    else if (table == "access_whitelist")
        response = PostAccessWhitelist(rowlimit, task);
    else if (table == "parking_reservation") 
        response = PostParkingReservation(rowlimit, task);
    else if (table == "contract_parking_transactions") 
        response = PostProductSubscription(rowlimit, task);
    
    return response;
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
        conn = general.mysqlConnect("parcx_server");
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
                response = TableList(tb[i].table_name, tb[i].rowlimit, tb[i].task);
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
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select table_name,task,upload_row_limit from pxcloud_upload_settings where enabled=1 and day_closure = 1");
        if (rs->rowsCount() > 0) 
            {
            while (rs->next()) 
                {
                table_name = rs->getString("table_name");
                task = rs->getInt("task");
                rowlimit = rs->getInt("upload_row_limit");
                response = TableList(table_name, rowlimit, task);
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
        conn = general.mysqlConnect("parcx_server");
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
        conn = general.mysqlConnect("parcx_server");
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
