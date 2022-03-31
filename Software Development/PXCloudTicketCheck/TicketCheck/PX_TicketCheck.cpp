#include "PX_GeneralOperations.h"
#include "PX_Reservation.h"
#include "PX_Access.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <curl/curl.h>
#include<jsoncpp/json/json.h>
#include <sstream>
#include <iomanip>
#include "PX_Validation.h"
#define CloudDB "pxcloud_server"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
GeneralOperations General;
Reservation ReservationObj;
Access AccessObj;
int entry_grace_period_reservation,exit_grace_period_reservation,rate_plan_id=0;
int entry_grace_period_minutes, exit_grace_period_minutes,parking_rate,parking_duration,parking_fee_duration;
double parking_fee;
Validation validation;
Json::FastWriter fw;
/*int couponEnabled,paymentExit, deductMoneyFromWallet, carparkNumber, facilityNumber, deviceNumber, deviceType, task, entryGrace, exitGrace, reservationEnabled, accessEnabled, deviceFunction, seconds, anprEnabled, cameraId, plateCapturedInterval, plateCapturedId, validationEnabled, walletEnabled, entryType;
string current_coupon_id,deviceName, carpakName, facilityName, ticketId, plateNumber, parkingZone, query, entryDateTime, maxEntryGrace, maxExitGrace, currentDateTime, plateType, plateArea, plateCountry;
string timeValidationId, percentageValidationId, validationDuration, userId, accessExpiry, reservationExpiry, paymentDateTime, accessResult, accessResultDescription, reservationResult, reservationStart, reservationResultDescription;
int test,offlineTimeValidation, offlinePercentageValidation, timeValidation, percentageValidation, parkingDuration, validationSeconds, validationHours, rate_type = 0;
double startparkingFee, parkingFee, vat_percentage = 0, amountPaid = 0, grossAmount, vatAmount, reservationFee, walletBalance;
sql::Connection *reportCon, *serverCon;
sql::Statement *stmt, *serverStmt;
sql::ResultSet *res, *validation, *settings, *device, *rate,*coupon;
CURL *curl;
CURLcode curl_res;
string wallet_webservice, webservice_response, vat_type = "", parking_duration;

Json::Reader jsonReader;
Json::Value jsonResponse;
string currency,coupon_value,couponId,valid_coupon_id,validation_value, parking_rate = "", parking_rate_label = "", entry_grace_period, entry_grace_period_mins, entry_grace_period_hours, exit_grace_period_mins, exit_grace_period_hours, exit_grace_period;
int entry_grace_period_minuts, exit_grace_period_minuts;
int parkingFeeDuration = 0;
int anpr_mismatch_check = 0;
int cooperate_parker = 0;
int short_term_entry_after_contract_parking_space_exceeded = 0;
int no_entry_contract_parking_allow_exit=1;
int coupon_percentage,coupon_amount;
string coupon_array;*/

void writeLog(string function, string message) {
    General.writeLog("Services/PXCloudTicketCheck/ApplicationLog-PX-TicketCheck-" + General.currentDateTime("%Y-%m-%d"), function, message);
}

void writeException(string function, string message) {
    General.writeLog("Services/PXCloudTicketCheck/ExceptionLog-PX-TicketCheck-" + General.currentDateTime("%Y-%m-%d"), function, message);
    writeLog(function, "Exception: " + message);
}

string toString(Php::Value param) {
    string value = param;
    return value;
}
string secondsToParkingDuration(int n) {
    int day = 0, hour = 0, minutes = 0, seconds = 0;
    if (n > 0) {
        day = n / (24 * 3600);
        n = n % (24 * 3600);
        hour = n / 3600;
        n %= 3600;
        minutes = n / 60;
        n %= 60;
        seconds = n;
    }
    string duration;
    if (day > 0)
        duration = to_string(day) + "D " + to_string(hour) + "H " + to_string(minutes) + "M";
    else if (hour > 0)
        duration = to_string(hour) + "H " + to_string(minutes) + "M";
    else
        duration = to_string(minutes) + "M " + to_string(seconds) + "S";
    return duration;
}


string convertToString(int number,int length,int hex)
    {
    stringstream ss;
    if(hex==1)        
		ss << setw(length) << setfill('0') <<std::hex << number;		
    else
        ss << setw(length) << setfill('0') << number;
    return ss.str();
    }



int getCarparkID(int carpark_number,int facility_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int carpark_id = 0;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select carpark_id,carpark_name from system_carparks where carpark_number="+to_string(carpark_number)+" and facility_number="+to_string(facility_number));
        if(res->next())
        {
            carpark_id = res->getInt("carpark_id");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("getCarparkID", e.what());
    }
    return carpark_id;
}

int getDeviceCategory(int device_number,int carpark_id)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int device_category = 0;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        //writeLog("writeLog","Select device_category from system_devices where device_number="+to_string(device_number)+" and carpark_id="+to_string(carpark_id));
        res = stmt->executeQuery("Select device_category from system_devices where device_number="+to_string(device_number)+" and carpark_id="+to_string(carpark_id));
        if(res->next())
        {
            device_category = res->getInt("device_category");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("getDeviceCategory", e.what());
    }
    return device_category;
}

void setReservationRateConfiguration(int carpark_id) {
    try 
    {
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res;
        conn = General.mysqlConnect(CloudDB);
        
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            res = stmt->executeQuery("SELECT rate_plan_id FROM parking_rate_reservation where special_rate_date = date(NOW()) and carpark_id = "+to_string(carpark_id)+" and special_rate_status=1");
            if(res->next())

            {
                rate_plan_id = res->getInt("rate_plan_id");
            }
            else
            {
                res = stmt->executeQuery("Select rate_plan_id from parking_rate_reservation where day_of_the_week = DayName(NOW()) and carpark_id = "+to_string(carpark_id));
                if(res->next())
                {
                    rate_plan_id = res->getInt("rate_plan_id");
                }

            }
            
            res = stmt->executeQuery("SELECT reservation_entry_grace_period_hours,reservation_entry_grace_period_mins,reservation_exit_grace_period_hours,reservation_exit_grace_period_mins FROM revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id));
            //writeLog("setReservationRateConfiguration","SELECT reservation_entry_grace_period_hours,reservation_entry_grace_period_mins,reservation_exit_grace_period_hours,reservation_exit_grace_period_mins FROM revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id));
            if(res->next())
            {
                entry_grace_period_reservation = res->getInt("reservation_entry_grace_period_hours")*60 + res->getInt("reservation_entry_grace_period_mins");
                exit_grace_period_reservation = res->getInt("reservation_exit_grace_period_hours")*60 + res->getInt("reservation_exit_grace_period_mins");
            }
            
            writeLog("setReservationRateConfiguration", "Entry grace period=" + to_string(entry_grace_period_reservation));
            writeLog("setReservationRateConfiguration", "Exit grace period=" + to_string(exit_grace_period_reservation));
        }
    }
    catch(const std::exception& e)
    {
        writeException("setReservationRateConfiguration",e.what());
    }

}

void setParkingRateConfiguration(int carpark_id) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    //int rate_plan_id = 1,entry_grace_period_minutes,exit_grace_period_minutes;
    string parking_rate = "", parking_rate_label = "",query;
    try {
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select rate_plan_id from system_carparks where carpark_id = "+to_string(carpark_id));
        if(res->next())
        {
            rate_plan_id = res->getInt("rate_plan_id");
        }
       
        query = "select entry_grace_period_mins,entry_grace_period_hours,exit_grace_period_hours,exit_grace_period_mins from  revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id);
        res = stmt->executeQuery(query);
        if (res->next()) {
            

            entry_grace_period_minutes = res->getInt("entry_grace_period_hours")*60 + res->getInt("entry_grace_period_mins");
            exit_grace_period_minutes = res->getInt("exit_grace_period_hours")*60 + res->getInt("exit_grace_period_mins");
        }
        writeLog("setParkingRateConfiguration", "Entry grace period=" + to_string(entry_grace_period_minutes));
        writeLog("setParkingRateConfiguration", "Exit grace period=" + to_string(exit_grace_period_minutes));
        delete res;
    } catch (const std::exception& e) {
        writeException("setParkingRateConfiguration", e.what());
    }

}
string GetCarparkName(int carpark_id)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string carpark_name;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select carpark_name from system_carparks where carpark_id="+to_string(carpark_id));
        if(res->next())
        {
            carpark_name = res->getString("carpark_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("GetCarparkName", e.what());
    }
    return carpark_name;
}

string GetFacilityName(int facility_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string facility_name;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select facility_name from system_facility where facility_number="+to_string(facility_number));
        if(res->next())
        {
            facility_name = res->getString("facility_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("GetFacilityName", e.what());
    }
    return facility_name;
}


void insertUpdateOpenTransaction(string ticket_id,int carpark,int facility,int device,int movement_type,int carpark_id,string result) {
    sql::Connection *conn;
    sql::Statement *stmt;
    int entryType;
    string query;
    try {                        
        if(result=="allow_access")
            entryType=2;
        else if(result=="reservation_allow_access")
            entryType=3;
        else
            entryType=1;
            
        conn = General.mysqlConnect(CloudDB);
        
        if (conn != NULL) {
            string carpark_name = GetCarparkName(carpark_id);
            string facility_name = GetFacilityName(facility);
            stmt = conn->createStatement(); 
            if(movement_type==1)
                query = "INSERT into open_transactions(device_number,ticket_id,carpark_id,carpark_number,facility_number,entry_date_time,operation_mode,entry_grace_period,entry_type,movement_type,carpark_name,facility_name) VALUES(" + to_string(device) + ",'" + ticket_id + "'," + to_string(carpark_id) + "," + to_string(carpark) + "," + to_string(facility) + ",CURRENT_TIMESTAMP,'', DATE_ADD(CURRENT_TIMESTAMP,INTERVAL " + to_string(entry_grace_period_minutes) + " MINUTE) ,"+to_string(entryType)+",1,'"+carpark_name+"','"+facility_name+"')";
            else
                query = "Delete from open_transactions where ticket_id = '"+ticket_id+"' and carpark_id="+to_string(carpark_id)+" order by id desc limit 1";
            int n = stmt->executeUpdate(query);
            if(n>0 && movement_type==2)
            {
                stmt->executeUpdate("Insert into parking_movements(date_time,ticket_id,device_number,movement_type,carpark_number,facility_number,carpark_id,carpark_name)values(NOW(),'"+ticket_id+"',"+to_string(device)+",2,"+to_string(carpark)+","+to_string(facility)+","+to_string(carpark_id)+",'"+carpark_name+"')");
            }
            delete stmt;
            delete conn;
        }
    } catch (const std::exception& e) {
        writeException("insertUpdateOpenTransaction", e.what());
    }
}

float calculateParkingFee(int duration)
{
    float parking_fee=0;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int hours = 0, minutes = 0, seconds = 0;
    hours = duration / 3600;
    minutes = (duration % 3600) / 60;
    seconds = (duration % 3600) % 60;
    writeLog("calculateParkingFee", to_string(hours) + "H " + to_string(minutes) + "M " + to_string(seconds) + "S ");
    try 
    {
        conn = General.mysqlConnect(CloudDB);
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            if(hours>0){
                writeLog("calculateParkingFee", "SELECT parking_rate,time_duration from  revenue_parking_rates where Lower(time_unit)='hours' and time_duration>"+to_string(hours)+" and parking_rate_id = "+to_string(rate_plan_id)+" and status=1 order by time_duration limit 1");
                res = stmt->executeQuery("SELECT parking_rate,time_duration from  revenue_parking_rates where Lower(time_unit)='hours' and time_duration>"+to_string(hours)+" and parking_rate_id = "+to_string(rate_plan_id)+" and status=1 order by time_duration limit 1");
                if(res->next())
                {
                    parking_fee = res->getDouble("parking_rate");
                    parking_fee_duration = res->getInt("time_duration")*60;
                }
                else
                {
                    //Take max parking rate
                }
            }
            else
            {
                res = stmt->executeQuery("SELECT parking_rate,time_duration from  revenue_parking_rates where Lower(time_unit)='minutes' and time_duration>"+to_string(minutes)+" and parking_rate_id = "+to_string(rate_plan_id)+" and status=1 order by time_duration limit 1");
                if(res->next())
                {
                    parking_fee = res->getDouble("parking_rate");
                    parking_fee_duration = res->getInt("time_duration");
                }
                else
                {
                    //Take first hour parking rate?
                }
            }
        }
        
    }
    catch(const std::exception& e)
    {
        writeException("calculateParkingFee",e.what());
    }
    return parking_fee;
}




Php::Value checkAppUser(string ticket_id,int carpark,int facility,int device,int movement_type,int carpark_id)
{
    //writeLog("checkAppUser","In checkAppUser:movement type = "+to_string(movement_type)); 
    Php::Value response;
    string entryDateTime;
    try 
    {
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res,*res1;
        conn = General.mysqlConnect(CloudDB);
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            res = stmt->executeQuery("SELECT wallet_balance,a.user_id from  parking_ewallet as a join application_users as b on a.user_id = b.user_id where b.user_qrcode = '"+ticket_id+"' and a.wallet_enabled = 1");
            if(res->next())
            {
                setParkingRateConfiguration(carpark_id);
                if(movement_type==1)
                {
                    if(res->getDouble("wallet_balance")>=0)
                    {
                        //allow entry and add  in the open transaction table
                        response["access_allowed"]="true";
                        response["result"] = "shortterm_entry";
                        response["result_description"] = "Shortterm Entry";   
                        response["customer_message_line1"]="Access allowed";
                        response["customer_message_line2"]="Shortterm Entry";
                                           
                    }
                    else
                    {
                        response["access_allowed"]="false";
                        response["result_description"]="Access disabled.Wallet balance is negative";    
                        response["result"]="access_disabled";
                        response["customer_message_line1"]="Access denied";
                        response["customer_message_line2"]="Wallet balance negative";
                    }
                }
                else
                {
                    res1 = stmt->executeQuery("Select * from open_transactions where ticket_id = '"+ticket_id+"' and movement_type = 1 order by id desc");
                    if(res1->next())
                    {
                        int id = res1->getInt("id");
                        entryDateTime = res1->getString("entry_date_time");
                        int *duration = General.differenceDateTime(General.currentDateTime(dateTimeFormat), entryDateTime, dateTimeFormat);
                        parking_duration = duration[0];
                        response["parking_duration"] = secondsToParkingDuration(parking_duration);
                        writeLog("checkAppUser","parking duration in seconds:"+to_string(parking_duration)); 
                        parking_fee = calculateParkingFee(parking_duration);
                        response["parking_fee"] = parking_fee;
                        response["parking_fee_duration"] = parking_fee_duration;
                        int conversion_factor = 1; //get it from db based on country
                        double parking_wallet_points = parking_fee*conversion_factor;
                        response["parking_wallet_points"] = parking_wallet_points;
                        double current_balance = res->getDouble("wallet_balance") - parking_wallet_points;
                        string user_id = res->getString("user_id");
                        int n = stmt->executeUpdate("Update parking_ewallet set wallet_balance = "+to_string(current_balance)+",last_deducted_amount = "+to_string(parking_wallet_points)+",last_payment_date_time=NOW(),download_flag=0 where user_id = "+user_id);
                        if(n>0)
                        {
                            stmt->executeUpdate("Insert into ewallet_transactions(date_time,transaction_type,transaction_description,user_id,ticket_id,amount,wallet_point,wallet_conversion_factor,previous_wallet_balance,current_wallet_balance,device_number,carpark_number,facility_number)Values(NOW(),100,'Parking Fee','"+user_id+"','"+ticket_id+"',"+to_string(parking_fee)+","+to_string(parking_wallet_points)+","+to_string(conversion_factor)+","+to_string(res->getDouble("wallet_balance"))+","+to_string(current_balance)+","+to_string(device)+","+to_string(carpark)+","+to_string(facility)+")");
                            stmt->executeUpdate("Update open_transactions set movement_type = 2 where id = "+to_string(id));
                            response["access_allowed"]="true";
                            
                        }
                        
                    }
                    else
                    {
                        response["access_allowed"]="false";
                        response["result_description"]="Not in Open Transaction";    
                        response["result"]="access_disabled";
                        response["customer_message_line1"]="Access denied";
                        response["customer_message_line2"]="Shortterm Entry";
                    }
                    delete res1;
                }
            }
            else
            {
                response["access_allowed"]="false";
                response["result_description"]="Access disabled.No ewallet record found";    
                response["result"]="access_disabled";
                response["customer_message_line1"]="Access denied";
                response["customer_message_line2"]="No ewallet record";
            }
            delete res;
            delete stmt;
        }
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("checkAppUser",e.what());
    }
    return response;
}

Php::Value parcxTicketCheck(Php::Parameters &params) {
int device_number,carpark_number,facility_number,movement_type=1;
string qr_code,parking_zone;
Json::Reader reader;
Php::Value response;
Json::Value json,validation_rules;
response["customer_message_line1"]="";
response["customer_message_line2"]="";
response["customer_message_line3"]="";
response["customer_message_line4"]="";
try {
    string request = params[0];
    reader.parse(request,json,true);
    writeLog("parcxTicketCheck","--------------START-----------------");
    device_number = json["device_number"].asInt();
    carpark_number = json["carpark_number"].asInt();
    facility_number = json["facility_number"].asInt();
    parking_zone = json["parking_zone"].asString();
    qr_code = json["qr_code"].asString();

    /*validation_rules["device_number_type"] = "int";
    validation_rules["device_number_length"] = 0;
    validation_rules["carpark_number_type"] = "int";
    validation_rules["carpark_number_length"] = 0;
    validation_rules["facility_number_type"] = "int";
    validation_rules["facility_number_length"] = 0;
    validation_rules["parking_zone_type"] = "varchar";
    validation_rules["parking_zone_length"] = 10;
    validation_rules["qr_code_type"] = "varchar";
    validation_rules["qr_code_length"] = 30;

    Json::Value validation_response = validation.checkValidation(json,validation_rules);

    if(validation_response["result"]=="failed")
    {
        writeLog("parcxTicketCheck", fw.write(validation_response));
        response["access_allowed"]="false";
        response["result_description"]="Validation failed";
        response["result"]="validation_failed";
        response["validation_details"] = fw.write(validation_response);
    }
    else
    {*/
        int carpark_id = getCarparkID(carpark_number,facility_number);
        int device_type = getDeviceCategory(device_number,carpark_id);
        writeLog("parcxTicketCheck","device category:"+to_string(device_type));
        if(device_type==1)
        {
            movement_type = 1;
        }
        else if(device_type==2 || device_type==3)
        {
            movement_type = 2;
        }
        //Check Access Whitelist
        Php::Value whitelist_details = AccessObj.checkAccess(qr_code, parking_zone,carpark_number,facility_number,movement_type);
        writeLog("parcxTicketCheck","checkAccess Response:"+toString(whitelist_details["result_description"]));
        if (toString(whitelist_details["access_allowed"]) == "true")
        {
            for (auto &iter : whitelist_details)
                response[iter.first] = iter.second;
            response["customer_message_line1"]="Access allowed";
            response["customer_message_line2"]="Whitelist available";

        }
        else //Check Reservation
        {
            setReservationRateConfiguration(carpark_id);
            Php::Value reservation_details = ReservationObj.checkReservation(qr_code,entry_grace_period_reservation,exit_grace_period_reservation,carpark_number,facility_number,movement_type);
            writeLog("parcxTicketCheck","checkReservation Response:"+toString(reservation_details["result_description"]));
            if (toString(reservation_details["access_allowed"]) == "true")
            {
                for (auto &iter : reservation_details)
                    response[iter.first] = iter.second;
                response["customer_message_line1"]="Access allowed";
                response["customer_message_line2"]="Reservation available";
                    
            }
            else if(toString(reservation_details["access_allowed"]) == "false" && toString(reservation_details["result"]) =="reservation_exit_grace_period_exceeded")
            {
                //Handle exit grace period exceeded in case of reservation
            }
            else //App User
            {
                Php::Value app_user_details = checkAppUser(qr_code,carpark_number,facility_number,device_number,movement_type,carpark_id); 
                writeLog("parcxTicketCheck","checkAppUser Response:"+toString(app_user_details["result_description"]));
                for (auto &iter : app_user_details)
                    response[iter.first] = iter.second;
                
            }
        }
        string result = response["result"];
        if(movement_type==1 && toString(response["access_allowed"])=="true")
        {
            insertUpdateOpenTransaction(qr_code,carpark_number,facility_number,device_number,movement_type,carpark_id,result); 
        }
        else if(movement_type==2 && toString(response["access_allowed"])=="true")
        {
            insertUpdateOpenTransaction(qr_code,carpark_number,facility_number,device_number,movement_type,carpark_id,result); 
        }
        /*if (toString(response["access_allowed"]) == "false" && device_type != 1) {
            writeLog("exitTicketCheck", "Exit ticket check: " + ticket_id);
            Php::Value ticket_details = openTransactionCheck();
            for (auto &iter : ticket_details)
                response[iter.first] = iter.second;
        }*/
    //}
    
    //writeLog("parcxTicketCheck", "Result: " + result + "\tDescription:" + result_description + "\n");
    writeLog("parcxTicketCheck","--------------END-----------------"); 
    
} catch (exception &e) {
        writeException("parcxTicketCheck", e.what());
    }
    return response;

}


extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PX_TicketCheck", "1.0");
        extension.add<parcxTicketCheck>("parcxTicketCheck");
        return extension;
    }
}