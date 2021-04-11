//make -B
//sudo make install
//service apache2 restart

#include <phpcpp.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <dlfcn.h>
#include "jsoncpp/json/json.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "pxheader.h"


using namespace std;
sql::Connection *server_con, *reporting_con, *reporting_con1;
sql::Driver *driver;
sql::Statement *reporting_stmt, *server_stmt, *reporting_stmt1;
sql::ResultSet *reporting_res, *server_res, *reporting_res1;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define REPORTING_DATABASE "parcx_reporting"
#define SERVER_DATABASE "parcx_server"

pxheader p;
string entry_date_time, ticket_number, query, app_key, transaction_id, device_ip, device_name, app_user_id, json_response, ticket_max_exit_date_time, ticket_payment_date_time, grace_time_remaining, grace_time_exeeded, blacklist_status;
int operation_mode, anpr_enable, camera_id, device_number, device_type, task, online_validation_check, entry_type;
string opentransaction_status, max_entry_grace, plate_number, entry_image_name, entry_plate_number, entry_plate_type, entry_capture_datetime, entry_plate_area, entry_plate_country, exit_image_name, exit_plate_number, exit_plate_type, exit_capture_datetime, exit_plate_area, exit_plate_country, so_file_path = "/opt/lampp/lib/php/extensions/no-debug-non-zts-20160303/";
Json::Reader reader;
Json::FastWriter fastWriter;
Json::Value json, response, ticketless_response, paid_response, opentransaction_response, res, validation_response, plate_suggestions, suggestions_response, suggestion_status, platescaptured_response, platescaptured_status, whitelist_response, reservation_response;
/*
 * opertaion_mode values
 * 1-QRCode 
 * 2-Chipcoin (blacklist and validation check)
 * 3-Ticketless
 */


/*
entry_type in open_transaction
1-normal
2-reservation
3-whitelist
 */

string log_file_path = "/opt/parcx/Logs/Services/ApplicationLogs/PX_Plate_Suggestions_phpcpp_" + p.currentDate() + "_log.log";
string error_file_path = "/opt/parcx/Logs/Services/ExceptionLogs/PX_Plate_Suggestions_phpcpp_" + p.currentDate() + "_exception.log";

Json::Value PX_Plate_Suggestions_GetPlateSuggestions(string plateNo, int suggestion) {

    Json::Value jsondata(Json::arrayValue);
    Json::Value error;
    try {

        plate_number = plateNo;
        p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "Getting suggestions for plate no: " + plate_number);
       

        reporting_con = p.mysqlConnect(REPORTING_DATABASE);
        reporting_stmt = reporting_con->createStatement();


        error["error"] = false;
        if (suggestion == 1)
            query = "SELECT plate_captured_id,device_number,plate_number,ticket_id,entry_type,entry_date_time from open_transactions where plate_number like '%" + plate_number + "%'";
        else
            query = "SELECT plate_captured_id,device_number,plate_number,ticket_id,entry_type,entry_date_time from open_transactions where plate_number like '%" + plate_number + "%' order by id desc limit 1";

        reporting_stmt = reporting_con->createStatement();
        reporting_res = reporting_stmt->executeQuery(query);

        if (reporting_res->rowsCount() > 0) {


            p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "Suggestions exists " + to_string(reporting_res->rowsCount()));
            suggestion_status["suggestion_status"] = true;
            reporting_res->first();
            for (int i = 0; (unsigned) i < reporting_res->rowsCount(); i++) {

                //p.writeLog(log_file_path,"PaymentCheck_GetPlateSuggestions","in for loop "+reporting_res->getString("device_number"));
                //query="select camera_id from system_devices where device_number="+reporting_res->getString("device_number");
                //server_res=server_stmt->executeQuery(query);

                //server_res->first();
                //if(server_res->first()){
                p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "getting details from plates captured");
                //query="select * from plates_captured where plate_number like '"+reporting_res->getString("plate_number")+"' and camera_device_number ="+server_res->getString("camera_id")+" ORDER by id DESC LIMIT 1";
                query = "select * from plates_captured where id = '" + reporting_res->getString("plate_captured_id") + "'";


                reporting_con1 = p.mysqlConnect(REPORTING_DATABASE);
                reporting_stmt1 = reporting_con1->createStatement();
                reporting_res1 = reporting_stmt1->executeQuery(query);



                if (reporting_res1->first()) {
                    ticket_number = reporting_res->getString("ticket_id");
                    entry_date_time = reporting_res->getString("entry_date_time");
                    entry_image_name = reporting_res1->getString("plate_image_name");
                    entry_plate_type = reporting_res1->getString("plate_type");
                    entry_capture_datetime = reporting_res1->getString("capture_date_time");
                    entry_plate_area = reporting_res1->getString("plate_area");
                    entry_plate_country = reporting_res1->getString("plate_country");
                    camera_id = reporting_res1->getInt("camera_device_number");
                    plate_number = reporting_res1->getString("plate_number");
                    entry_type = reporting_res->getInt("entry_type");
                    suggestions_response["entry_date_time"] = entry_date_time;
                    suggestions_response["entry_plate_type"] = entry_plate_type;
                    suggestions_response["entry_capture_datetime"] = entry_capture_datetime;
                    suggestions_response["entry_plate_area"] = entry_plate_area;
                    suggestions_response["entry_plate_country"] = entry_plate_country;
                    suggestions_response["entry_plate_image_name"] = entry_image_name;
                    suggestions_response["entry_plate_number"] = plate_number;
                    suggestions_response["entry_camera_id"] = camera_id;
                    suggestions_response["ticket_number"] = ticket_number;
                    suggestions_response["entry_type"] = entry_type;
                    jsondata.append(suggestions_response);

                    reporting_res1->close();
                    reporting_stmt1->close();
                    reporting_con1->close();
                } else {
                    p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "Not presentin plates Captured");
                    suggestion_status["suggestion_status"] = false;
                }
                //}
                reporting_res->next();

            }





        } else {
            p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "Suggestions doesnt exists ");
            error["error"] = false;
            suggestion_status["suggestion_status"] = false;
            p.writeLog(log_file_path, "PaymentCheck_GetPlateSuggestions", "Suggestions doesnt exists wrrote sugesstion status");
        }

        // delete server_res;
        // delete server_stmt;
        // delete server_con;		

        delete reporting_res;
        delete reporting_stmt;
        delete reporting_con;



        jsondata.append(error);
        jsondata.append(suggestion_status);


    } catch (exception &e) {

        suggestion_status["suggestion_status"] = false;
        error["error"] = true;
        jsondata.append(error);
        p.writeException(error_file_path, "PaymentCheck_GetPlateSuggestions", e.what());
    }

    return jsondata;
}

Php::Value PlateSuggestions(Php::Parameters &params) {

    try {

        res["error"] = true;
        string json_request = params[0];
        p.writeLog(log_file_path, "PaymentTicketCheck", "in the function " + json_request);
        bool parseSuccess = reader.parse(json_request, json, false);

        if (parseSuccess) {
            //const Json::Value resultValue = json["device_number"];
            device_number = json["device_number"].asInt();
            device_name = json["device_name"].asString();
            device_type = json["device_type"].asInt();
            device_ip = json["device_ip"].asString();
            transaction_id = json["transaction_id"].asString();
            app_key = json["app_key"].asString();
            app_user_id = json["app_suer_id"].asString();
            task = json["task"].asInt();
            p.writeLog(log_file_path, "Payment_TicketCheck", "device numebr " + to_string(device_number));
            p.writeLog(log_file_path, "Payment_TicketCheck", "device type " + to_string(device_type));

            //PaymentTicketCheck_GetPlateCapturedDetails(device_number,1,5,5,0);

            switch (device_type) {

                    //Entry device
                case 1:
                    break;

                    //Exit or cashier
                case 3:case 5:
                    //p.writeLog(log_file_path,"Payment_TicketCheck","case 3 or case 5");
                    operation_mode = json["operation_mode"].asInt();


                    online_validation_check = json["online_validation"].asInt();
                    res["plate_suggestions"] = "";



                    res["error"] = false;
                    p.writeLog(log_file_path, "PaymentTicketCheck", "Request from cashier to get the plate suggestions for plate_number: " + json["plate_number"].asString());
                    res["plate_suggestions"] = PX_Plate_Suggestions_GetPlateSuggestions(json["plate_number"].asString(), json["get_plate_suggestions"].asInt());

                    res["transaction_id"] = transaction_id;
                    res["app_key"] = app_key;
                    json_response = fastWriter.write(res);
                    p.writeLog(log_file_path, "PaymentTicketCheck", "Final Response " + json_response);
                    break;


            }
        }

        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            p.writeException(error_file_path, "PaymentTicketCheck", e.what());

        }

        return json_response;



    }

    extern "C" {

        PHPCPP_EXPORT void *get_module() {
            static Php::Extension extension("PX_Plate_Suggestions", "1.0");
            extension.add<PlateSuggestions>("PlateSuggestions");
            return extension;
        }
    }

