//JSON.h install :   sudo apt-get install libjsoncpp-dev
//make -B
//sudo make install
//service apache2 restart
/*
Added parking_ewallet in download
Add download_flag=0 in parking_ewallet
Updated downloaddata based on pxcloud_download_settings in local server


Added Validation features 
Added parking_movements,parking_movements_reservation for IOT
Added valet_parking posting


Added plates_captured - Updated staging but not production
Added carpark_id insert/update in parking_revenue - Updated staging but not updated on production 

Change parking_movements to parking_movements_iot on staging
Added ipaddress in the php page for parking_movements_iot on staging
Added posting for watchdog_device_alarms_iot on staging
Added posting for access_request_iot on staging

On production DB:
-Add plate_captured,watchdog_device_alarms_iot,access_request_iot,change parking_movements to parking_movements_iot and add the extra columns
 */

#include <phpcpp.h>
#include<sstream>
#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<jsoncpp/json/json.h>
#include "validation.h"
#define DBDashboard "pxcloud_server"
#define DBServer "pxcloud_server"
#define DBReporting "pxcloud_server"
using namespace std;

sql::Connection *conn;
sql::Driver *driver;
Json::FastWriter fw;
GeneralOperations general;
Validation validation;
string operator_id;

void WriteToLog(string function, string txt) {
    cout<<function<<" : "<<txt<<endl;
    string path = "Services/ApplicationLogs/PX_DaemonService_" + general.currentDateTime("%Y-%m-%d");
    general.writeLog(path, function, txt);
}

void WriteException(string function, string txt) {    
    string path = "Services/ExceptionLogs/PX_DaemonService_Exception_" + general.currentDateTime("%Y-%m-%d");
    general.writeLog(path, function, txt);
    WriteToLog("****Exception:"+function+"*****",txt);
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> splits;
    std::string split;
    std::istringstream ss(s);
    while (std::getline(ss, split, delimiter)) {
        splits.push_back(split);
    }
    return splits;
}


string mysqldate(string date)
    {
     if (date == "")
        return "NULL";
    else
        return "'" + date + "'";
    }

int GetNextDashboardCount(int carpark_number, int facility_number) 
    {
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int order = 0;
    try 
        {
        con = general.DBConnectionRead(DBDashboard);
        stmt = con->createStatement();
        res = stmt->executeQuery("Select dashboard_order from counters where carpark_number=" + to_string(carpark_number) + "   and facility_number=" + to_string(facility_number));
        if (res->next()) 
            {
            order = res->getInt("dashboard_order");
            } 
        else 
            {
            res = stmt->executeQuery("Select max(dashboard_order) as dashboard_order from counters");
            if (res->next()) 
                {
                order = res->getInt("dashboard_order") + 1;
                }
            }
        delete res;
        delete stmt;
        delete con;
        } 
    catch (const std::exception& e) 
        {        
        WriteException("GetNextDashboardCount", e.what());
        }
    WriteToLog("GetNextDashboardCount", "Dashboard Order:" + to_string(order));

    return order;
    }

int GetCarparkFromDevice(int device_number,string facility_number)
{
    sql::Connection *conn=NULL;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int carpark_number= 0;
    try 
    {
        conn = general.DBConnectionWrite(DBServer);
        stmt = conn->createStatement();  
        res = stmt->executeQuery("SELECT b.carpark_number FROM system_devices as a inner join system_carparks as b on a.carpark_id = b.carpark_id where facility_number = '"+facility_number+"' and device_number = "+to_string(device_number));
        while(res->next())
        {
            carpark_number = res->getInt("carpark_number");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (const std::exception &e) 
    {
        WriteException("GetCarparkFromDevice", e.what());             
        delete conn;        
    }
    return carpark_number;
}

Json::Value GetTableRules(string table)
{
    sql::Connection *conn=NULL;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value table_rules;
    //Json::Value row;
    string field;
    string schema = DBServer;
 try 
    {
        conn = general.DBConnectionWrite(DBReporting);
        stmt = conn->createStatement();  
        res = stmt->executeQuery("SELECT column_name as 'field', data_type as 'type', character_maximum_length as 'length' FROM information_schema.columns WHERE table_name = '"+table+"'  AND table_schema = '"+schema+"'");
        while(res->next())
        {
            field = res->getString("field").asStdString();
            table_rules[field+"_type"] = res->getString("type").asStdString();
            table_rules[field+"_length"] = res->getInt("length");

            //table_rules.append(row);
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch (const std::exception &e) 
    {
        WriteException("GetTableRules", e.what());             
        delete conn;        
    }
    return table_rules;    
}

Json::Value UpdateParkingOccupancy(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {  
    Json::Value response;
    sql::Connection *con_read;
    sql::Connection *con_write;
    sql::Statement *stmt;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid;
    int carpark_number;
    
    
    int result = 0;
   
    int id, j = 0;
    string counter_name,carpark_number_string;
    int correction_factor;
    int count_category, device_number, pgs_counter_id, counter_type, parking_counter_id;
    int total_spaces, current_level, occupancy_threshold, free_spaces, reservation_current_level, access_current_level, shortterm_current_level;
    int total_shortterm_spaces, total_reservation_spaces, total_access_spaces, total_entries, total_exits, total_manual_entry;
    int total_manual_exit, blacklisted_tickets, server_manual_opens, external_manual_open, access_transactions, reservation_transactions;
    double occupancy_percentage, shortterm_entry, shortterm_exit, access_entry, access_exit, reservation_entry, reservation_exit;
    Json::Value table_rules;
    string sql = "";
    table_rules = GetTableRules("counters");
    try 
        {
        con_read = general.DBConnectionRead(DBDashboard);
        stmt = con_read->createStatement();
        con_write = general.DBConnectionWrite(DBDashboard);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            WriteToLog("UpdateParkingOccupancy", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingOccupancy", fw.write(validation_response));
                response["validation"] = "failed";
                response["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                parking_counter_id = data[i]["id"].asInt();
                carpark_number_string = data[i]["carpark_number"].asString();            
                carpark_number = stoi(carpark_number_string);
                string carpark_name =cloud_carpark_name[carpark_number];          
                int carpark_id = cloud_carpark_id[carpark_number];                                      

                device_number = data[i]["device_number"].asInt();
                pgs_counter_id = data[i]["pgs_counter_id"].asInt();
                counter_type = data[i]["counter_type"].asInt();
                counter_name = data[i]["counter_name"].asString();
                count_category = data[i]["count_category"].asInt();
                correction_factor = data[i]["correction_factor"].asInt();
                total_spaces = data[i]["total_spaces"].asInt();
                current_level = data[i]["current_level"].asInt();
                occupancy_threshold = data[i]["occupancy_threshold"].asInt();
                free_spaces = data[i]["free_spaces"].asInt();
                reservation_current_level = data[i]["reservation_current_level"].asInt();
                access_current_level = data[i]["access_current_level"].asInt();
                shortterm_current_level = data[i]["shortterm_current_level"].asInt();
                total_shortterm_spaces = data[i]["total_shortterm_spaces"].asInt();
                total_reservation_spaces = data[i]["total_reservation_spaces"].asInt();
                total_access_spaces = data[i]["total_access_spaces"].asInt();
                total_entries = data[i]["total_entries"].asInt();
                total_exits = data[i]["total_exits"].asInt();
                total_manual_entry = data[i]["total_manual_entry"].asInt();
                total_manual_exit = data[i]["total_manual_exit"].asInt();
                blacklisted_tickets = data[i]["blacklisted_tickets"].asInt();
                server_manual_opens = data[i]["server_manual_opens"].asInt();
                external_manual_open = data[i]["external_manual_open"].asInt();
                access_transactions = data[i]["access_transactions"].asInt();
                reservation_transactions = data[i]["reservation_transactions"].asInt();
                occupancy_percentage = stod(data[i]["occupancy_percentage"].asString());
                shortterm_entry = data[i]["shortterm_entry"].asInt();
                shortterm_exit = data[i]["shortterm_exit"].asInt();
                access_entry = data[i]["access_entry"].asInt();
                access_exit = data[i]["access_exit"].asInt();
                reservation_entry = data[i]["reservation_entry"].asInt();
                reservation_exit = data[i]["reservation_exit"].asInt(); 


                res = stmt->executeQuery("Select parking_counter_id from counters where carpark_number = " + to_string(carpark_number) + " and facility_number =" + facility_number + " and counter_type=" + to_string(counter_type) + " and count_category=" + to_string(count_category));
                if (res->next()) 
                    {
                    id = res->getInt("parking_counter_id");
                    sql = "Update counters set pgs_counter_id=" + to_string(pgs_counter_id) + ",correction_factor=" + to_string(correction_factor) + ",total_spaces=" + to_string(total_spaces) + ",current_level=" + to_string(current_level) + ",occupancy_threshold=" + to_string(occupancy_threshold) + ",free_spaces=" + to_string(free_spaces) + ",reservation_current_level=" + to_string(reservation_current_level) + ",access_current_level=" + to_string(access_current_level) + ",shortterm_current_level=" + to_string(shortterm_current_level) + ",total_shortterm_spaces=" + to_string(total_shortterm_spaces) + ",total_reservation_spaces=" + to_string(total_reservation_spaces) + ",total_access_spaces=" + to_string(total_access_spaces) + ",total_entries=" + to_string(total_entries) + ",total_exits=" + to_string(total_exits) + ",total_manual_entry=" + to_string(total_manual_entry) + ",total_manual_exit=" + to_string(total_manual_exit) + ",blacklisted_tickets=" + to_string(blacklisted_tickets) + ",server_manual_opens=" + to_string(server_manual_opens) + ",external_manual_open=" + to_string(external_manual_open) + ",access_transactions=" + to_string(access_transactions) + ",reservation_transactions=" + to_string(reservation_transactions) + ",occupancy_percentage=" + to_string(occupancy_percentage) + ",shortterm_entry=" + to_string(shortterm_entry) + ",shortterm_exit=" + to_string(shortterm_exit) + ",access_entry=" + to_string(access_entry) + ",access_exit=" + to_string(access_exit) + ",reservation_entry=" + to_string(reservation_entry) + ",reservation_exit=" + to_string(reservation_exit) + ",last_updated_datetime=NOW() where parking_counter_id = " + to_string(id);               
                    } 
                else 
                    {
                    int order = GetNextDashboardCount(carpark_number, stoi(facility_number));
                    sql = "INSERT INTO counters(carpark_number,carpark_name,carpark_id,facility_name,facility_number,device_number,dashboard_order,pgs_counter_id, count_category, counter_type,counter_name, correction_factor, total_spaces, current_level, occupancy_threshold,free_spaces,reservation_current_level,access_current_level,shortterm_current_level,total_shortterm_spaces, total_reservation_spaces, total_access_spaces, total_entries,total_exits,total_manual_entry,total_manual_exit,blacklisted_tickets,server_manual_opens,external_manual_open,access_transactions,reservation_transactions,occupancy_percentage,shortterm_entry,shortterm_exit,access_entry,access_exit,reservation_entry,reservation_exit) VALUES(" + to_string(carpark_number) + ",'" + carpark_name + "'," + to_string(carpark_id) + ",'" + facility_name + "'," + facility_number + "," + to_string(device_number) + "," + to_string(order) + "," + to_string(pgs_counter_id) + ", " + to_string(count_category) + ", " + to_string(counter_type) + ",'" + counter_name + "'," + to_string(correction_factor) + ", " + to_string(total_spaces) + ", " + to_string(current_level) + ", " + to_string(occupancy_threshold) + "," + to_string(free_spaces) + "," + to_string(reservation_current_level) + "," + to_string(access_current_level) + "," + to_string(shortterm_current_level) + "," + to_string(total_shortterm_spaces) + ", " + to_string(total_reservation_spaces) + ", " + to_string(total_access_spaces) + ", " + to_string(total_entries) + "," + to_string(total_exits) + "," + to_string(total_manual_entry) + "," + to_string(total_manual_exit) + "," + to_string(blacklisted_tickets) + "," + to_string(server_manual_opens) + "," + to_string(external_manual_open) + "," + to_string(access_transactions) + "," + to_string(reservation_transactions) + "," + to_string(occupancy_percentage) + "," + to_string(shortterm_entry) + "," + to_string(shortterm_exit) + "," + to_string(access_entry) + "," + to_string(access_exit) + "," + to_string(reservation_entry) + "," + to_string(reservation_exit) + ")";                                
                    }
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                    {
                    jsonresponseid[j] = parking_counter_id;
                    j++;
                    }

                delete res;
            }
        }
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con_read;                       
        } 
    catch (const std::exception &e) 
        {
        WriteException("Updatecounters", e.what());        
        response["message"] = "failed";    
        response["error"] = e.what();            
        }
    response["data"] = jsonresponseid;
    response["table"] = "counters";
    return response;
    }


Json::Value UpdateDeviceAlarms(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {   
    sql::Connection *con_write;
    con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data;
    string sql = "";
    int id;
    string description, datetime, create_datetime, device_name, code,validation_response;
    int dismiss, severity, carpark_number,device_number;
    
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;    
    try 
        {
        table_rules = GetTableRules("watchdog_device_alarms");
        
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();                
        data = json["data"];
        
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateWatchdogAlarms", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateWatchdogAlarms", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateWatchdogAlarms", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                carpark_number = data[i]["carpark_number"].asInt();           
                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                                                                     
                severity = data[i]["alarm_severity"].asInt();
                code = data[i]["alarm_code"].asString();
                description = data[i]["alarm_description"].asString();
                datetime = data[i]["alarm_date_time"].asString();
                create_datetime = data[i]["create_date_time"].asString();
                dismiss = data[i]["dismiss"].asInt();
        
                sql = "INSERT into watchdog_device_alarms(carpark_name,carpark_number,carpark_id,facility_name,facility_number,device_name,device_number,alarm_severity,alarm_code,alarm_description,alarm_date_time,create_date_time,dismiss) VALUES('" + carpark_name + "'," + to_string(carpark_number) + "," + to_string(carpark_id) + ",'" + facility_name + "'," + facility_number + ",'" + device_name + "'," + to_string(device_number) + "," + to_string(severity) + ",'" + code + "','" + description + "'," + mysqldate(datetime) + "," + mysqldate(create_datetime) + "," + to_string(dismiss) + ")";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }   
            }
        }                
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateWatchdogAlarms", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();        
        delete con_write;        
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "watchdog_device_alarms";
    return jsonresponse;
    }

Json::Value UpdateDeviceStatus(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) 
    {    
    Json::Value response;
    sql::Connection *con_read;
    sql::Connection *con_write;
    sql::Statement *stmt;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid,table_rules;
    int carpark_number;       
    int device_number;
    string device_name;
    int device_type;
    string device_ip;
    int device_network_status;
   
    int id, watchdog_device_id = 0, j = 0;
    int result = 0;
    string sql = "";
    try 
        {
        table_rules = GetTableRules("watchdog_device_status");
        con_read = general.DBConnectionRead(DBDashboard);
        stmt = con_read->createStatement();
        con_write = general.DBConnectionWrite(DBDashboard);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateDeviceStatus", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateDeviceStatus", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateDeviceStatus", fw.write(validation_response));
                response["validation"] = "failed";
                response["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                watchdog_device_id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];                       


                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                device_type = data[i]["device_type"].asInt();
                device_ip = data[i]["device_ip"].asString();
                device_network_status = data[i]["device_network_status"].asInt();

                res = stmt->executeQuery("Select id from watchdog_device_status where device_number = " + to_string(device_number) + " and facility_number = " + facility_number);
                if (res->next()) 
                    {
                    id = res->getInt("id");
                    sql = "Update watchdog_device_status set device_name='" + device_name + "',device_ip='" + device_ip + "',device_network_status=" + to_string(device_network_status) + ",carpark_number=" + to_string(carpark_number) + ",last_updated_datetime=CURRENT_TIMESTAMP() where id = " + to_string(id);

                    } 
                else 
                    {               
                    sql = "INSERT INTO watchdog_device_status (device_type, device_name, device_number, device_ip, device_network_status, carpark_name, carpark_number, facility_name, facility_number , last_updated_datetime) VALUES(" + to_string(device_type) + ",'" + device_name + "'," + to_string(device_number) + ",'" + device_ip + "'," + to_string(device_network_status) + ",'" + carpark_name + "'," + to_string(carpark_number) + ",'" + facility_name + "'," + facility_number + ",CURRENT_TIMESTAMP())";               
                    }
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                    {
                    jsonresponseid[j] = watchdog_device_id;
                    j++;
                    }
                delete res;
            
            }           
            

        }
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con_read;
        
        
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateDeviceStatus", e.what());
        response["message"] = "failed";    
        response["error"] = e.what();                
        }
    response["data"] = jsonresponseid;
    response["table"] = "watchdog_device_status";
    return response;
    }

Json::Value UpdateOpenTransactions(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write, *con_read;
    sql::Statement *stmt_write, *stmt_read;
    sql::ResultSet *res;
    Json::Value data, jsonresponse;
    int carpark_number;   
    int device_number;
    string device_name;
    string sql = "";
    int id;
   
    string ticket_id;
    string entry_grace_period;
    string chip_utid;
    string plate_number;
    string operation_mode;
    string creation_date_time;
    double total_amount_paid;
    string last_payment_date_time;
    string max_exit_date_time;
    string entry_date_time;
    int entry_type;
    int total_validation_hours;
    string parking_rate;
    int movement_type;
    Json::Value jsonresponseid, jsonresponsedelete,table_rules;
    int result = 0;
    int j = 0, k = 0, rowid;
    
    WriteToLog("OPEN", fw.write(json));

    try 
        {
        table_rules = GetTableRules("open_transactions");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();

        con_read = general.DBConnectionRead(DBReporting);
        stmt_read = con_read->createStatement();

        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateOpenTransactions", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateOpenTransactions", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateOpenTransactions", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                entry_grace_period = data[i]["entry_grace_period"].asString();
                chip_utid = data[i]["chip_utid"].asString();
                plate_number = data[i]["plate_number"].asString();
                total_amount_paid = stod(data[i]["total_amount_paid"].asString());
                last_payment_date_time = data[i]["last_payment_date_time"].asString();
                max_exit_date_time = data[i]["max_exit_date_time"].asString();
                entry_date_time = data[i]["entry_date_time"].asString();
                entry_type = data[i]["entry_type"].asInt();
                total_validation_hours = data[i]["total_validation_hours"].asInt();
                parking_rate = data[i]["parking_rate"].asString();
                operation_mode = data[i]["operation_mode"].asString();
                creation_date_time = data[i]["creation_date_time"].asString();
                movement_type = data[i]["movement_type"].asInt();

                last_payment_date_time=mysqldate(last_payment_date_time);
                max_exit_date_time=mysqldate(max_exit_date_time);                        

                if (movement_type == 2) 
                {                                
                    res = stmt_read->executeQuery("Select id from open_transactions where carpark_id=" + to_string(carpark_id) + " and ticket_id='" + ticket_id + "' and movement_type=1");
                    if (res->next()) 
                    {
                        rowid = res->getInt("id");                    
                        result = stmt_write->executeUpdate("Delete from open_transactions where id = " + to_string(rowid));
                        if (result == 1) 
                            {
                            jsonresponsedelete[k] = id;
                            k++;
                            }
                    } 
                    else//delete the record locally even if there is no record in cloud
                    {
                        jsonresponsedelete[k] = id;
                        k++;
                    }
                    delete res;
                } 
                else if (movement_type == 1) 
                {
                    sql = "INSERT into open_transactions(device_number,device_name,carpark_number,carpark_name,facility_number,facility_name,carpark_id,ticket_id,entry_grace_period,chip_utid,plate_number,total_amount_paid,last_payment_date_time,max_exit_date_time,entry_date_time,entry_type,total_validation_hours,parking_rate,operation_mode,creation_date_time,movement_type) VALUES(" + to_string(device_number) + ",'" + device_name + "'," + to_string(carpark_number) + ",'" + carpark_name + "'," + facility_number+ ",'" + facility_name + "'," + to_string(carpark_id) + ",'" + ticket_id + "','" + entry_grace_period + "','" + chip_utid + "','" + plate_number + "'," + to_string(total_amount_paid) + "," + last_payment_date_time + "," + max_exit_date_time + "," + mysqldate(entry_date_time) + "," + to_string(entry_type) + "," + to_string(total_validation_hours) + ",'" + parking_rate + "','" + operation_mode + "'," + mysqldate(creation_date_time) + "," + to_string(movement_type) + ")";

                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                        {
                        jsonresponseid[j] = id;
                        j++;
                        }
                }
            }
        }
        delete stmt_write;
        delete con_write;
        delete stmt_read;
        delete con_read;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateOpenTransactions", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["del"] = jsonresponsedelete;
    jsonresponse["table"] = "open_transactions";
    return jsonresponse;
    }

Json::Value UpdateParkingMovementsAccess(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {    
    sql::Connection *con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data, jsonresponse;
    string sql = "";
    string date_time;
    int id;
    string tag_serial;
    string plate_number;
    string tag;
    string status_message;
    string epc;
    string tid;
    int epc_count;
    string epc_list;
    int device_number;
    string device_name;
    int movement_type;
    string parking_zone;
    int carpark_number;    
       
    Json::Value jsonresponseid,table_rules;
    int result = 0;
    int j = 0;
   
    try 
        {
        table_rules = GetTableRules("parking_movements_access");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateParkingMovementsAccess", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingMovementsAccess", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingMovementsAccess", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                date_time = data[i]["date_time"].asString();
                tag_serial = data[i]["tag_serial"].asString();
                plate_number = data[i]["plate_number"].asString();
                tag = data[i]["tag"].asString();
                status_message = data[i]["status_message"].asString();
                epc = data[i]["epc"].asString();
                tid = data[i]["tid"].asString();
                epc_count = data[i]["epc_count"].asInt();
                epc_list = data[i]["epc_list"].asString();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                movement_type = data[i]["movement_type"].asInt();
                parking_zone = data[i]["parking_zone"].asString();
                carpark_number = data[i]["carpark_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                sql = "INSERT INTO parking_movements_access (date_time, tag_serial, plate_number, tag, status_message, epc, tid, epc_count, epc_list, device_number, device_name, movement_type, parking_zone, carpark_number, carpark_name, facility_number, facility_name,carpark_id) VALUES('" + date_time + "','" + tag_serial + "','" + plate_number + "','" + tag + "','" + status_message + "','" + epc + "','" + tid + "'," + to_string(epc_count) + ",'" + epc_list + "'," + to_string(device_number) + ",'" + device_name + "'," + to_string(movement_type) + ",'" + parking_zone + "'," + to_string(carpark_number) + ",'" + carpark_name + "'," + facility_number+ ",'" + facility_name + "'," + to_string(carpark_id) + ")";
                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                    {
                    jsonresponseid[j] = id;
                    j++;
                    }                                    
            }        
        }
        
        delete stmt_write;
        delete con_write;

    } 
    catch (const std::exception &e) 
    {
        WriteException("UpdateParkingMovementsAccess", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();        
        delete con_write;
    }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_movements_access";
    return jsonresponse;
    }

Json::Value UpdateRevenuePayments(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write=NULL;
    con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data;
    string sql = "";
    
    int id;
    int device_number;
    string device_name;
    
    int carpark_number;   
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;
   
    int operator_id, vat_percentage, receipt_number, product_id, discount_id, product_quantity;
    double vat_amount, lost_fee, parking_fee, admin_fixed_charges, balance_returned, credit_note, amount_received, gross_amount, ticket_replacement_fee, discount_amount;
    string validation_id,validation_value,operator_name, shift_id, parking_rate_label, bank_notes, customer_card_id, void_datetime, void_reason, void_user, anpr_comparison_number, exit_plate_number, entry_plate_number, record_creation_time, payment_type, void_fld, authentication_code, coupon_source, coupon_id, coupon_category, discount_name, discount_category, payment_category, payment_date_time, parking_duration, entry_date_time, max_exit_date_time, ticket_id, vat_type, exit_grace_period, parking_rate_name, entry_grace_period;
   
    try {
        table_rules = GetTableRules("revenue_payments");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateRevenuePayments", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateRevenuePayments", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateRevenuePayments", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                                     

                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                operator_id = data[i]["operator_id"].asInt();
                operator_name = data[i]["operator_name"].asString();
                shift_id = data[i]["shift_id"].asString();
                parking_rate_label = data[i]["parking_rate_label"].asString();
                parking_rate_name = data[i]["parking_rate_name"].asString();
                entry_grace_period = data[i]["entry_grace_period"].asString();
                exit_grace_period = data[i]["exit_grace_period"].asString();
                vat_type = data[i]["vat_type"].asString();
                vat_percentage = data[i]["vat_percentage"].asInt();
                ticket_id = data[i]["ticket_id"].asString();
                entry_date_time = data[i]["entry_date_time"].asString();
                payment_date_time = data[i]["payment_date_time"].asString();
                max_exit_date_time = data[i]["max_exit_date_time"].asString();
                parking_duration = data[i]["parking_duration"].asString();
                payment_category = data[i]["payment_category"].asString();
                parking_fee = stod(data[i]["parking_fee"].asString());
                vat_amount = stod(data[i]["vat_amount"].asString());
                lost_fee = stod(data[i]["lost_fee"].asString());
                admin_fixed_charges = stod(data[i]["admin_fixed_charges"].asString());
                ticket_replacement_fee = stod(data[i]["ticket_replacement_fee"].asString());
                discount_amount = stod(data[i]["discount_amount"].asString());
                gross_amount = stod(data[i]["gross_amount"].asString());
                amount_received = stod(data[i]["amount_received"].asString());
                discount_id = data[i]["discount_id"].asInt();
                discount_category = data[i]["discount_category"].asString();

                discount_name = data[i]["discount_name"].asString();
                coupon_id = data[i]["coupon_id"].asString();
                coupon_category = data[i]["coupon_category"].asString();
                coupon_source = data[i]["coupon_source"].asString();
                product_id = data[i]["product_id"].asInt();
                product_quantity = data[i]["product_quantity"].asInt();
                payment_type = data[i]["payment_type"].asString();
                authentication_code = data[i]["authentication_code"].asString();
                void_fld = data[i]["void"].asString();

                record_creation_time = data[i]["record_creation_time"].asString();
                entry_plate_number = data[i]["entry_plate_number"].asString();
                exit_plate_number = data[i]["exit_plate_number"].asString();
                anpr_comparison_number = data[i]["anpr_comparison_number"].asString();
                validation_id = data[i]["validation_id"].asString();
                validation_value = data[i]["validation_value"].asString();
                void_user = data[i]["void_user"].asString();
                void_reason = data[i]["void_reason"].asString();
                void_datetime = data[i]["void_datetime"].asString();
                receipt_number = data[i]["receipt_number"].asInt();
                customer_card_id = data[i]["customer_card_id"].asString();
                bank_notes = data[i]["bank_notes"].asString();
                balance_returned = stod(data[i]["balance_returned"].asString());
                credit_note = stod(data[i]["credit_note"].asString());

                entry_date_time=mysqldate(entry_date_time);


                sql = "INSERT INTO revenue_payments (device_number, device_name, operator_id, operator_name, shift_id, carpark_name, carpark_number,carpark_id, parking_rate_label, parking_rate_name, entry_grace_period, exit_grace_period, vat_type, vat_percentage, ticket_id, entry_date_time, payment_date_time, max_exit_date_time, parking_duration, payment_category, parking_fee, vat_amount, lost_fee, admin_fixed_charges, ticket_replacement_fee, discount_amount, gross_amount, amount_received, discount_id, discount_category, discount_name, coupon_id, coupon_category, coupon_source, product_id, product_quantity, payment_type, authentication_code, void, record_creation_time, facility_number, entry_plate_number, exit_plate_number, anpr_comparison_number, validation_id, validation_value, void_user, void_reason, void_datetime, receipt_number, customer_card_id, bank_notes, balance_returned, credit_note)Values(" + to_string(device_number) + ",'" + device_name + "'," + to_string(operator_id) + ",'" + operator_name + "','" + shift_id + "','" + carpark_name + "'," + to_string(carpark_number) + "," + to_string(carpark_id) + ",'" + parking_rate_label + "','" + parking_rate_name + "','" + entry_grace_period + "','" + exit_grace_period + "','" + vat_type + "'," + to_string(vat_percentage) + ",'" + ticket_id + "'," + entry_date_time + "," + mysqldate(payment_date_time) + "," + mysqldate(max_exit_date_time) + ",'" + parking_duration + "','" + payment_category + "'," + to_string(parking_fee) + "," + to_string(vat_amount) + "," + to_string(lost_fee) + "," + to_string(admin_fixed_charges) + "," + to_string(ticket_replacement_fee) + "," + to_string(discount_amount) + "," + to_string(gross_amount) + "," + to_string(amount_received) + "," + to_string(discount_id) + ",'" + discount_category + "', '" + discount_name + "','" + coupon_id + "','" + coupon_category + "','" + coupon_source + "', " + to_string(product_id) + ", " + to_string(product_quantity) + ",'" + payment_type + "','" + authentication_code + "','" + void_fld + "'," + mysqldate(record_creation_time) + "," + facility_number + ",'" + entry_plate_number + "','" + exit_plate_number + "','" + anpr_comparison_number + "','" + validation_id + "','" + validation_value + "','" + void_user + "','" + void_reason + "'," + mysqldate(void_datetime) + "," + to_string(receipt_number) + ",'" + customer_card_id + "','" + bank_notes + "'," + to_string(balance_returned) + "," + to_string(credit_note) + ")";

                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }   
            }
        }                
        delete stmt_write;
        delete con_write;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateRevenuePayments", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        delete con_write;        
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "revenue_payments";
    return jsonresponse;
    }

Json::Value UpdateParkingDuration(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {   
    sql::Connection *con_write=NULL;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data;
    int carpark_number;       
    string sql = "";
    int id,row_id;
    string report_date;
    string report_day;
    string category;
    
    int m0to30;
    int m30to60;
    int h[24];
    int d1to2;
    int d2to3;            
    int maximum_parking_duration;
    int minimum_parking_duration;
    string average_parking_duration;
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0, k = 0;
    string response = "";
    try 
        {
        table_rules = GetTableRules("parking_duration");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();                                        
        data = json["data"];
        //cout<<fw.write(table_rules)<<endl;
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateParkingDuration", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingDuration", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingDuration", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                report_date = data[i]["report_date"].asString();
                report_day = data[i]["report_day"].asString();
                m0to30 = data[i]["m0to30"].asInt();           
                m30to60 = data[i]["m30to60"].asInt();           
                carpark_number = data[i]["carpark_number"].asInt();                        

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                category = data[i]["category"].asString();
                j = 1;
                for (j = 1; j < 24; j++) 
                    {
                    h[j] = data[i]["h" + to_string(j) + "to" + to_string((j + 1))].asInt();
                    }
                d1to2 = data[i]["d1to2"].asInt();
                d2to3 = data[i]["d2to3"].asInt();

                maximum_parking_duration = data[i]["maximum_parking_duration"].asInt();
                minimum_parking_duration = data[i]["minimum_parking_duration"].asInt();
                average_parking_duration = data[i]["average_parking_duration"].asString();

                res = stmt_write->executeQuery("Select statistics_id as id from parking_duration where category ='"+category+"' and carpark_id = " + to_string(carpark_id) + " and facility_number =" + facility_number + " and report_date='" + report_date + "'");
                if (res->next()) 
                {
                    row_id = res->getInt("id");
                    sql = "Update parking_duration set m0to30 = " + to_string(m0to30) + ",m30to60=" + to_string(m30to60) + ",d1to2=" + to_string(d1to2) + ",d2to3=" + to_string(d2to3) + ",h1to2=" + to_string(h[1]) + ",h2to3=" + to_string(h[2]) + ",h3to4=" + to_string(h[3]) + ",h4to5=" + to_string(h[4]) + ",h5to6=" + to_string(h[5]) + ",h6to7=" + to_string(h[6]) + ",h7to8=" + to_string(h[7]) + ",h8to9=" + to_string(h[8]) + ",h9to10=" + to_string(h[9]) + ",h10to11=" + to_string(h[10]) + ",h11to12=" + to_string(h[11]) + ",h12to13=" + to_string(h[12]) + ",h13to14=" + to_string(h[13]) + ",h14to15=" + to_string(h[14]) + ",h15to16=" + to_string(h[15]) + ",h16to17=" + to_string(h[16]) + ",h17to18=" + to_string(h[17]) + ",h18to19=" + to_string(h[18]) + ",h19to20=" + to_string(h[19]) + ",h20to21=" + to_string(h[20]) + ",h21to22=" + to_string(h[21]) + ",h22to23=" + to_string(h[22]) + ",h23to24=" + to_string(h[23]) + ",maximum_parking_duration=" + to_string(maximum_parking_duration) + ",minimum_parking_duration=" + to_string(minimum_parking_duration) + ",average_parking_duration=" + average_parking_duration + " where statistics_id = "+to_string(row_id);            
                } 
                else 
                {
                    sql = "INSERT into parking_duration(report_date,report_day,carpark_id,carpark_number,carpark_name,facility_name,facility_number,m0to30,m30to60,d1to2,d2to3,h1to2,h2to3,h3to4,h4to5,h5to6,h6to7,h7to8,h8to9,h9to10,h10to11,h11to12,h12to13,h13to14,h14to15,h15to16,h16to17,h17to18,h18to19,h19to20,h20to21,h21to22,h22to23,h23to24,maximum_parking_duration,minimum_parking_duration,average_parking_duration,category) values ('" + report_date + "','" + report_day + "'," + to_string(carpark_id) + "," + to_string(carpark_number) + ",'" + carpark_name + "','" + facility_name + "'," + facility_number+ "," + to_string(m0to30) + "," + to_string(m30to60) + "," + to_string(d1to2) + "," + to_string(d2to3) + "," + to_string(h[1]) + "," + to_string(h[2]) + "," + to_string(h[3]) + "," + to_string(h[4]) + "," + to_string(h[5]) + "," + to_string(h[6]) + "," + to_string(h[7]) + "," + to_string(h[8]) + "," + to_string(h[9]) + "," + to_string(h[10]) + "," + to_string(h[11]) + "," + to_string(h[12]) + "," + to_string(h[13]) + "," + to_string(h[14]) + "," + to_string(h[15]) + "," + to_string(h[16]) + "," + to_string(h[17]) + "," + to_string(h[18]) + "," + to_string(h[19]) + "," + to_string(h[20]) + "," + to_string(h[21]) + "," + to_string(h[22]) + "," + to_string(h[23]) + "," + to_string(maximum_parking_duration) + "," + to_string(minimum_parking_duration) + "," + average_parking_duration + ",'" + category + "')";            
                }
                delete res;
                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                {
                    jsonresponseid[k] = id;
                    k++;
                } 
            }
        }        
        response = fw.write(jsonresponse);
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {                
        WriteException("UpdateParkingDuration", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();            
        delete con_write;
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_duration";
    return jsonresponse;
    }

Json::Value UpdateRevenueSummary(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {   

    sql::Connection *con_write=NULL;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data;
    int carpark_number;            
    int device_number;
    string device_name;
    string sql = "";
    int id,row_id;
    string report_date;
    string report_day;
    int network_status;
    string gross_amount;
    string parking_fee;
    string vat_amount;
    string lost_ticket_fee;
    string admin_fixed_charges;
    string ticket_replacement_charges;
    string discount_amount;
    string sms_payment_amount;
    string creditcard_payment_amount;
    string rfid_prepaid;
    int complimentary_coupons_count;
    int complimentary_handicap_count;
    int payable_entries_count;
    int lost_ticket_count;
    int discount_count;
    int sms_payment_count;
    int creditcard_payment_count;
    string comments;
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int k = 0;
    
    try {
        table_rules = GetTableRules("summary_daily_revenue");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();               
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateRevenueSummary", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateRevenueSummary", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateRevenueSummary", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                report_date = data[i]["report_date"].asString();
                report_day = data[i]["report_day"].asString();
                network_status = data[i]["network_status"].asInt();
                gross_amount = data[i]["gross_amount"].asString();
                parking_fee = data[i]["parking_fee"].asString();
                vat_amount = data[i]["vat_amount"].asString();
                lost_ticket_fee = data[i]["lost_ticket_fee"].asString();
                admin_fixed_charges = data[i]["admin_fixed_charges"].asString();
                ticket_replacement_charges = data[i]["ticket_replacement_charges"].asString();
                discount_amount = data[i]["discount_amount"].asString();
                sms_payment_amount = data[i]["sms_payment_amount"].asString();
                creditcard_payment_amount = data[i]["creditcard_payment_amount"].asString();
                rfid_prepaid = data[i]["rfid_prepaid"].asString();
                complimentary_coupons_count = data[i]["complimentary_coupons_count"].asInt();
                complimentary_handicap_count = data[i]["complimentary_handicap_count"].asInt();
                payable_entries_count = data[i]["payable_entries_count"].asInt();
                lost_ticket_count = data[i]["lost_ticket_count"].asInt();
                discount_count = data[i]["discount_count"].asInt();
                sms_payment_count = data[i]["sms_payment_count"].asInt();
                creditcard_payment_count = data[i]["creditcard_payment_count"].asInt();
                comments = data[i]["comments"].asString();
                carpark_number = data[i]["carpark_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                res = stmt_write->executeQuery("Select id from summary_daily_revenue where device_number=" + to_string(device_number) + " and carpark_id = " + to_string(carpark_id) + " and facility_number =" + facility_number + " and report_date='" + report_date + "'");
                if (res->next()) 
                {
                    row_id = res->getInt("id");
                    sql = "Update summary_daily_revenue set network_status = "+to_string(network_status)+",gross_amount=" + gross_amount + ",parking_fee=" + parking_fee + ",vat_amount=" + vat_amount + ",lost_ticket_fee=" + lost_ticket_fee + ",admin_fixed_charges=" + admin_fixed_charges + ",ticket_replacement_charges=" + ticket_replacement_charges + ",discount_amount=" + discount_amount + ",sms_payment_amount=" + sms_payment_amount + ",creditcard_payment_amount =" + creditcard_payment_amount + ",rfid_prepaid = " + rfid_prepaid + ",complimentary_coupons_count=" + to_string(complimentary_coupons_count) + ",complimentary_handicap_count=" + to_string(complimentary_handicap_count) + ",payable_entries_count=" + to_string(payable_entries_count) + ",lost_ticket_count=" + to_string(lost_ticket_count) + ",discount_count=" + to_string(discount_count) + ",sms_payment_count=" + to_string(sms_payment_count) + ",creditcard_payment_count=" + to_string(creditcard_payment_count) + ",comments='" + comments + "',last_updated_date_time=NOW() where id = "+to_string(row_id);            
                } 
                else 
                {
                    sql = "INSERT into summary_daily_revenue(report_date,report_day,carpark_id,carpark_number,carpark_name,facility_name,facility_number,device_number,device_name,network_status,gross_amount,parking_fee,vat_amount,lost_ticket_fee,admin_fixed_charges,ticket_replacement_charges,discount_amount,sms_payment_amount,creditcard_payment_amount,rfid_prepaid,complimentary_coupons_count,complimentary_handicap_count,payable_entries_count,lost_ticket_count,discount_count,sms_payment_count,creditcard_payment_count,comments) values ('" + report_date + "','" + report_day + "'," + to_string(carpark_id) + "," + to_string(carpark_number) + ",'" + carpark_name + "','" + facility_name + "'," + facility_number+ "," + to_string(device_number) + ",'" + device_name + "'," + to_string(network_status) + "," + gross_amount + "," + parking_fee + "," + vat_amount + "," + lost_ticket_fee + "," + admin_fixed_charges + "," + ticket_replacement_charges + "," + discount_amount + "," + sms_payment_amount + "," + creditcard_payment_amount + "," + rfid_prepaid + "," + to_string(complimentary_coupons_count) + "," + to_string(complimentary_handicap_count) + "," + to_string(payable_entries_count) + "," + to_string(lost_ticket_count) + "," + to_string(discount_count) + "," + to_string(sms_payment_count) + "," + to_string(creditcard_payment_count) + ",'" + comments + "')";            
                }
                delete res;
                //cout<<sql<<endl;
                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                {
                    jsonresponseid[k] = id;
                    k++;
                }                                    
            }    
        }
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {       
        WriteException("UpdateRevenueSummary", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        delete con_write;
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "summary_daily_revenue";
    return jsonresponse;
    }

Json::Value UpdateParkingMovementsSummary(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {   

    sql::Connection *con_write=NULL;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data;
    int carpark_number;      
    int device_number;    
    string sql = "";
    int id,row_id;
    string report_date;
    string report_day;
    int category;
    int movement_type;
    int h[24];
    int reproccessing_flag;
    int total_count;    
    string comment;    
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0, k = 0;
    
    try 
    {
        table_rules = GetTableRules("summary_parking_movements");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();                     
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingMovementsSummary", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingMovementsSummary", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingMovementsSummary", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                report_date = data[i]["report_date"].asString();
                report_day = data[i]["report_day"].asString();
                category = stoi(data[i]["category"].asString());
                movement_type = data[i]["movement_type"].asInt();
                carpark_number = data[i]["car_park_number"].asInt();            

                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                device_number = data[i]["device_number"].asInt();
                for (j = 0; j < 24; j++) 
                    {
                    h[j] = data[i]["h" + to_string(j) + "to" + to_string((j + 1))].asInt();
                    }
                reproccessing_flag = data[i]["reproccessing_flag"].asInt();
                total_count = data[i]["total_count"].asInt();            
                comment = data[i]["comment"].asString();
                //cout<<"Select id from summary_parking_movements where category = "+to_string(category)+" and  movement_type = "+to_string(movement_type)+" and device_number="+ to_string(device_number) + " and carpark_id = " + to_string(carpark_id) + " and facility_number =" + facility_number + " and report_date='" + report_date + "'"<<endl;
                res = stmt_write->executeQuery("Select id from summary_parking_movements where category = "+to_string(category)+" and  movement_type = "+to_string(movement_type)+" and device_number="+ to_string(device_number) + " and carpark_id = " + to_string(carpark_id) + " and facility_number =" + facility_number + " and report_date='" + report_date + "'");
                if (res->next()) 
                {
                    row_id = res->getInt("id");
                    sql = "Update summary_parking_movements set reproccessing_flag = "+to_string(reproccessing_flag)+",comment='" + comment + "',total_count=" + to_string(total_count) + ",h0to1=" + to_string(h[0]) + ",h1to2=" + to_string(h[1]) + ",h2to3=" + to_string(h[2]) + ",h3to4=" + to_string(h[3]) + ",h4to5=" + to_string(h[4]) + ",h5to6=" + to_string(h[5]) + ",h6to7=" + to_string(h[6]) + ",h7to8=" + to_string(h[7]) + ",h8to9=" + to_string(h[8]) + ",h9to10=" + to_string(h[9]) + ",h10to11=" + to_string(h[10]) + ",h11to12=" + to_string(h[11]) + ",h12to13=" + to_string(h[12]) + ",h13to14=" + to_string(h[13]) + ",h14to15=" + to_string(h[14]) + ",h15to16=" + to_string(h[15]) + ",h16to17=" + to_string(h[16]) + ",h17to18=" + to_string(h[17]) + ",h18to19=" + to_string(h[18]) + ",h19to20=" + to_string(h[19]) + ",h20to21=" + to_string(h[20]) + ",h21to22=" + to_string(h[21]) + ",h22to23=" + to_string(h[22]) + ",h23to24=" + to_string(h[23]) + " where id = "+to_string(row_id); 

                } 
                else 
                {
                    sql = "INSERT into summary_parking_movements(reproccessing_flag,report_date,report_day,carpark_id,carpark_number,carpark_name,facility_name,facility_number,device_number,category,movement_type,comment,total_count,h0to1,h1to2,h2to3,h3to4,h4to5,h5to6,h6to7,h7to8,h8to9,h9to10,h10to11,h11to12,h12to13,h13to14,h14to15,h15to16,h16to17,h17to18,h18to19,h19to20,h20to21,h21to22,h22to23,h23to24) values ("+to_string(reproccessing_flag)+",'" + report_date + "','" + report_day + "'," + to_string(carpark_id) + "," + to_string(carpark_number) + ",'" + carpark_name + "','" + facility_name + "'," + facility_number + "," + to_string(device_number) + "," + to_string(category) + "," + to_string(movement_type) + ",'" + comment + "'," + to_string(total_count) + "," + to_string(h[0]) + "," + to_string(h[1]) + "," + to_string(h[2]) + "," + to_string(h[3]) + "," + to_string(h[4]) + "," + to_string(h[5]) + "," + to_string(h[6]) + "," + to_string(h[7]) + "," + to_string(h[8]) + "," + to_string(h[9]) + "," + to_string(h[10]) + "," + to_string(h[11]) + "," + to_string(h[12]) + "," + to_string(h[13]) + "," + to_string(h[14]) + "," + to_string(h[15]) + "," + to_string(h[16]) + "," + to_string(h[17]) + "," + to_string(h[18]) + "," + to_string(h[19]) + "," + to_string(h[20]) + "," + to_string(h[21]) + "," + to_string(h[22]) + "," + to_string(h[23]) + ")";           
                }
                //cout<<sql<<endl;
                delete res;
                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                {
                    jsonresponseid[k] = id;
                    k++;
                } 
            }
        }            
        delete stmt_write;
        delete con_write;

    } 
    catch (const std::exception &e) 
    {        
        WriteException("UpdateParkingMovementsSummary", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        delete con_write;
    }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "summary_parking_movements";   
    return jsonresponse;
    }

Json::Value UpdateHourlyOccupancy(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {   
    Json::Value response;
    sql::Connection *con_read;
    sql::Connection *con_write;
    sql::Statement *stmt;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid,table_rules;
    int carpark_number;
    string type;   
    
    int h[24], j, k = 0;    
    int id, occupancy_id, result;

    string sql = "";
    try 
        {
        table_rules = GetTableRules("hourly_occupancy");
        con_read = general.DBConnectionRead(DBDashboard);
        stmt = con_read->createStatement();
        con_write = general.DBConnectionWrite(DBDashboard);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateHourlyOccupancy", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateHourlyOccupancy", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateHourlyOccupancy", fw.write(validation_response));
                response["validation"] = "failed";
                response["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                occupancy_id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();                                    
                string carpark_name =cloud_carpark_name[carpark_number];                       


                type = data[i]["type"].asString();


                for (j = 0; j < 24; j++) 
                    {
                    h[j] = data[i]["h" + to_string(j) + "to" + to_string((j + 1))].asInt();
                    }            
                res = stmt->executeQuery("Select id from hourly_occupancy where carpark_number = " + to_string(carpark_number) + " and facility_number =" + facility_number + " and type='" + type + "'");
                if (res->next()) 
                    {
                    id = res->getInt("id");
                    sql = "Update hourly_occupancy set ";
                    for (j = 0; j < 24; j++) 
                        {
                        sql = sql + "h" + to_string(j) + "to" + to_string(j + 1) + "=" + to_string(h[j]);
                        if (j < 23) 
                            {
                            sql = sql + ",";
                            }
                        }
                    sql = sql + " where id = " + to_string(id);                                
                    } 
                else 
                    {
                    sql = "INSERT INTO hourly_occupancy(carpark_number,carpark_name,facility_name,facility_number,type,data,h0to1,h1to2,h2to3,h3to4,h4to5,h5to6,h6to7,h7to8,h8to9,h9to10,h10to11,h11to12,h12to13,h13to14,h14to15,h15to16,h16to17,h17to18,h18to19,h19to20,h20to21,h21to22,h22to23,h23to24)values(" + to_string(carpark_number) + ",'" + carpark_name + "','" + facility_name + "'," + facility_number + ",'" + type + "','realtime'," + to_string(h[0]) + "," + to_string(h[1]) + "," + to_string(h[2]) + "," + to_string(h[3]) + "," + to_string(h[4]) + "," + to_string(h[5]) + "," + to_string(h[6]) + "," + to_string(h[7]) + "," + to_string(h[8]) + "," + to_string(h[9]) + "," + to_string(h[10]) + "," + to_string(h[11]) + "," + to_string(h[12]) + "," + to_string(h[13]) + "," + to_string(h[14]) + "," + to_string(h[15]) + "," + to_string(h[16]) + "," + to_string(h[17]) + "," + to_string(h[18]) + "," + to_string(h[19]) + "," + to_string(h[20]) + "," + to_string(h[21]) + "," + to_string(h[22]) + "," + to_string(h[23]) + ")";                                
                    }
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[k] = occupancy_id;
                    k++;
                }
                delete res; 
            }
        }
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con_read;               
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateHourlyOccupancy", e.what());
        response["message"] = "failed";
        response["error"] = e.what();    
        }    
    response["data"] = jsonresponseid;
    response["table"] = "hourly_occupancy";
    return response;
    }

Json::Value UpdateParkingRevenue(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {    
    Json::Value response;
    sql::Connection *con_read;
    sql::Connection *con_write;
    sql::Statement *stmt;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid,table_rules;
    int carpark_number;        
    double gross_amount, vat_amount, creditcard_payment_amount, sms_payment_amount, parking_fee, lost_ticket_fee, product_sale_amount, discount_amount, admin_fixed_charges, ticket_replacement_fee;
    int device_type, device_number, product_sale_count, creditcard_payment_count, sms_payment_count, network_status, discount_count, lost_ticket_count, payable_entries_count;
    string last_transaction, last_updated_date_time, device_name;
    
    int id, result, k = 0, parking_id,carpark_id;

    string sql = "";
    try 
        {
        table_rules = GetTableRules("parking_revenue");
        con_read = general.DBConnectionRead(DBDashboard);
        stmt = con_read->createStatement();
        con_write = general.DBConnectionWrite(DBDashboard);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingRevenue", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingRevenue", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingRevenue", fw.write(validation_response));
                response["validation"] = "failed";
                response["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                parking_id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();


                string carpark_name =cloud_carpark_name[carpark_number];                                   
                carpark_id = cloud_carpark_id[carpark_number];
                device_type = data[i]["device_type"].asInt();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                network_status = data[i]["network_status"].asInt();
                gross_amount = stod(data[i]["gross_amount"].asString());
                parking_fee = stod(data[i]["parking_fee"].asString());
                vat_amount = stod(data[i]["vat_amount"].asString());
                lost_ticket_fee = stod(data[i]["lost_ticket_fee"].asString());
                admin_fixed_charges = stod(data[i]["admin_fixed_charges"].asString());
                ticket_replacement_fee = stod(data[i]["ticket_replacement_fee"].asString());
                discount_amount = stod(data[i]["discount_amount"].asString());
                product_sale_amount = stod(data[i]["product_sale_amount"].asString());
                sms_payment_amount = stod(data[i]["sms_payment_amount"].asString());
                creditcard_payment_amount = stod(data[i]["creditcard_payment_amount"].asString());
                payable_entries_count = data[i]["payable_entries_count"].asInt();
                lost_ticket_count = data[i]["lost_ticket_count"].asInt();
                discount_count = data[i]["discount_count"].asInt();
                sms_payment_count = data[i]["sms_payment_count"].asInt();
                creditcard_payment_count = data[i]["creditcard_payment_count"].asInt();
                product_sale_count = data[i]["product_sale_count"].asInt();
                last_transaction = data[i]["last_transaction"].asString();
                last_updated_date_time = data[i]["last_updated_date_time"].asString();

                last_updated_date_time=mysqldate(last_updated_date_time);
                last_transaction=mysqldate(last_transaction);                        

                res = stmt->executeQuery("Select id from parking_revenue where carpark_number = " + to_string(carpark_number) + " and facility_number =" + facility_number + " and device_number=" + to_string(device_number));
                if (res->next()) 
                    {
                    id = res->getInt("id");
                    sql = "Update parking_revenue set network_status=" + to_string(network_status) + ",gross_amount=" + to_string(gross_amount) + ",parking_fee = " + to_string(parking_fee) + ",vat_amount=" + to_string(vat_amount) + ",lost_ticket_fee=" + to_string(lost_ticket_fee) + ",admin_fixed_charges=" + to_string(admin_fixed_charges) + ",ticket_replacement_fee=" + to_string(ticket_replacement_fee) + ",discount_amount=" + to_string(discount_amount) + ",product_sale_amount=" + to_string(product_sale_amount) + ",sms_payment_amount=" + to_string(sms_payment_amount) + ",creditcard_payment_amount=" + to_string(creditcard_payment_amount) + ",payable_entries_count=" + to_string(payable_entries_count) + ",lost_ticket_count=" + to_string(lost_ticket_count) + ",discount_count=" + to_string(discount_count) + ",sms_payment_count=" + to_string(sms_payment_count) + ",creditcard_payment_count=" + to_string(creditcard_payment_count) + ",product_sale_count=" + to_string(product_sale_count) + ",last_transaction=" + last_transaction + ",last_updated_date_time=" + last_updated_date_time + ",carpark_id="+to_string(carpark_id)+" where id=" + to_string(id);                                
                    } 
                else 
                    {
                    sql = "INSERT INTO parking_revenue(carpark_number,carpark_name,facility_number,device_type,device_number,device_name,network_status,gross_amount,parking_fee,vat_amount,lost_ticket_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,product_sale_amount,sms_payment_amount,creditcard_payment_amount,payable_entries_count,lost_ticket_count,discount_count,sms_payment_count,creditcard_payment_count,product_sale_count,last_transaction,last_updated_date_time,carpark_id)values(" + to_string(carpark_number) + ",'" + carpark_name + "'," + facility_number + "," + to_string(device_type) + "," + to_string(device_number) + ",'" + device_name + "'," + to_string(network_status) + "," + to_string(gross_amount) + "," + to_string(parking_fee) + "," + to_string(vat_amount) + "," + to_string(lost_ticket_fee) + "," + to_string(admin_fixed_charges) + "," + to_string(ticket_replacement_fee) + "," + to_string(discount_amount) + "," + to_string(product_sale_amount) + "," + to_string(sms_payment_amount) + "," + to_string(creditcard_payment_amount) + "," + to_string(payable_entries_count) + "," + to_string(lost_ticket_count) + "," + to_string(discount_count) + "," + to_string(sms_payment_count) + "," + to_string(creditcard_payment_count) + "," + to_string(product_sale_count) + "," + last_transaction + "," + last_updated_date_time + ","+to_string(carpark_id)+")";                                
                    }
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                    {
                    jsonresponseid[k] = parking_id;
                    k++;
                    }
                delete res;  
            }
        }
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con_read;                
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingRevenue", e.what());
        response["message"] = "failed";
        response["error"] = e.what();    
        }    
    response["data"] = jsonresponseid;
    response["table"] = "parking_revenue";
    return response;
    }

Json::Value UpdateParkingMovementsManual(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data;
    string sql = "";
    string date_time;
    int id;
    int device_number;
    string device_name;    
    int carpark_number;        
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;    
    int movement_type;
    string operator_id, operator_name, create_date_time, action, reason, description;

    try 
        {
        table_rules = GetTableRules("parking_movements_manual");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingMovementsManual", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingMovementsManual", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingMovementsManual", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();


                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                operator_id = data[i]["operator_id"].asString();
                operator_name = data[i]["operator_name"].asString();
                movement_type = data[i]["movement_type"].asInt();
                date_time = data[i]["date_time"].asString();
                create_date_time = data[i]["create_date_time"].asString();
                action = data[i]["action"].asString();
                reason = data[i]["reason"].asString();
                description = data[i]["description"].asString();
                if(create_date_time=="")
                    create_date_time = date_time;

                sql = "INSERT INTO parking_movements_manual (carpark_id,device_number, device_name, operator_id, operator_name, carpark_name, carpark_number,facility_name,facility_number,movement_type,date_time,create_date_time,action,reason,description) Values (" + to_string(carpark_id) + "," + to_string(device_number) + ",'" + device_name + "','" + operator_id + "','" + operator_name + "','" + carpark_name + "'," + to_string(carpark_number) + ",'" + facility_name + "'," + facility_number + "," + to_string(movement_type) + ",'" + date_time + "','" + create_date_time + "','" + action + "','" + reason + "','" + description + "')";            
                result = stmt_write->executeUpdate(sql);

                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                } 
            }
        }                
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingMovementsManual", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        delete con_write;        
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_movements_manual";
    return jsonresponse;
    }

Json::Value UpdateRevenueShifts(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    sql::ResultSet *res;
    Json::Value data;
    string sql = "";    
    int id;
    int device_number;
    string device_name;    
    int carpark_number;        
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;    
    int  operator_id, shift_status;
    string operator_name, shift_id, login_time, logout_time, shift_duration, last_updated_date_time;
    double wallet_points,parking_fees, vat_amount, lost_fee, admin_fixed_charges, ticket_replacement_fee, physical_cash_collected, shift_closing_amount, discount_amount, shift_earnings, payin_amount, payout_amount, shift_opening_amount;

    try 
        {
        table_rules = GetTableRules("revenue_shifts");
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateRevenueShifts", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateRevenueShifts", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateRevenueShifts", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();


                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                         


                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                shift_id = data[i]["shift_id"].asString();
                operator_id = data[i]["operator_id"].asInt();
                operator_name = data[i]["operator_name"].asString();
                login_time = data[i]["login_time"].asString();
                logout_time = data[i]["logout_time"].asString();
                shift_duration = data[i]["shift_duration"].asString();
                parking_fees = stod(data[i]["parking_fees"].asString());
                vat_amount = stod(data[i]["vat_amount"].asString());
                lost_fee = stod(data[i]["lost_fee"].asString());
                admin_fixed_charges = stod(data[i]["admin_fixed_charges"].asString());
                ticket_replacement_fee = stod(data[i]["ticket_replacement_fee"].asString());
                discount_amount = stod(data[i]["discount_amount"].asString());
                shift_earnings = stod(data[i]["shift_earnings"].asString());
                payin_amount = stod(data[i]["payin_amount"].asString());
                payout_amount = stod(data[i]["payout_amount"].asString());
                shift_opening_amount = stod(data[i]["shift_opening_amount"].asString());
                shift_closing_amount = stod(data[i]["shift_closing_amount"].asString());
                physical_cash_collected = stod(data[i]["physical_cash_collected"].asString());
                shift_status = data[i]["shift_status"].asInt();
                last_updated_date_time = data[i]["last_updated_date_time"].asString();
                wallet_points = stod(data[i]["wallet_points"].asString());

                login_time=mysqldate(login_time);
                logout_time=mysqldate(logout_time);


                res = stmt_write->executeQuery("Select id from revenue_shifts where shift_id = '" + shift_id + "'");
                if (res->next()) 
                    {
                    sql = "Update revenue_shifts set wallet_points="+to_string(wallet_points)+",carpark_id=" + to_string(carpark_id) + ",device_number=" + to_string(device_number) + ", device_name='" + device_name + "',operator_id=" + to_string(operator_id) + ", operator_name='" + operator_name + "', carpark_name='" + carpark_name + "', carpark_number=" + to_string(carpark_number) + ",facility_name='" + facility_name + "',facility_number=" + facility_number + ",login_time=" + login_time + ",logout_time=" + logout_time + ",shift_duration='" + shift_duration + "',parking_fees=" + to_string(parking_fees) + ",vat_amount=" + to_string(vat_amount) + ",lost_fee=" + to_string(lost_fee) + ",admin_fixed_charges=" + to_string(admin_fixed_charges) + ",ticket_replacement_fee=" + to_string(ticket_replacement_fee) + ",discount_amount =" + to_string(discount_amount) + ",shift_earnings=" + to_string(shift_earnings) + ",payin_amount=" + to_string(payin_amount) + ",payout_amount=" + to_string(payout_amount) + ",shift_opening_amount=" + to_string(shift_opening_amount) + ",shift_closing_amount=" + to_string(shift_closing_amount) + ",physical_cash_collected=" + to_string(physical_cash_collected) + ",shift_status=" + to_string(shift_status) + ",last_updated_date_time='" + last_updated_date_time + "' where id= "+res->getString("id");
                    } 
                else 
                    {
                    sql = "INSERT INTO revenue_shifts(wallet_points,carpark_id,device_number, device_name, shift_id,operator_id, operator_name, carpark_name, carpark_number,facility_name,facility_number,login_time,logout_time,shift_duration,parking_fees,vat_amount,lost_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,shift_earnings,payin_amount,payout_amount,shift_opening_amount,shift_closing_amount,physical_cash_collected,shift_status,last_updated_date_time) Values ("+to_string(wallet_points)+"," + to_string(carpark_id) + "," + to_string(device_number) + ",'" + device_name + "','" + shift_id + "'," + to_string(operator_id) + ",'" + operator_name + "','" + carpark_name + "'," + to_string(carpark_number) + ",'" + facility_name + "'," + facility_number + "," + login_time + "," + logout_time + ",'" + shift_duration + "'," + to_string(parking_fees) + "," + to_string(vat_amount) + "," + to_string(lost_fee) + "," + to_string(admin_fixed_charges) + "," + to_string(ticket_replacement_fee) + "," + to_string(discount_amount) + "," + to_string(shift_earnings) + "," + to_string(payin_amount) + "," + to_string(payout_amount) + "," + to_string(shift_opening_amount) + "," + to_string(shift_closing_amount) + "," + to_string(physical_cash_collected) + "," + to_string(shift_status) + ",'" + last_updated_date_time + "')";
                    }
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                    {
                    jsonresponseid[j] = id;
                    j++;
                    }  
            }
        }                
        delete stmt_write;
        delete con_write;
    } 
    catch (const std::exception &e) 
    {
        WriteException("UpdateRevenueShifts", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();        
    }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "revenue_shifts";
    return jsonresponse;
    }

Json::Value UpdateAccessWhitelist(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) 
    {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";
    
    int id, result = 0, j = 0, rowid = 0;
    int access_id, wallet_user_id, antipassback, status, wallet_payment, season_card, corporate_parker, operator_id;
    string carparklist,carparkstring, country, plate_number, ticket_id, access_zones, creation_date, last_visited, start_date, expiry_date, company_name, customer_name, tag, tag_serial, tag_tid;
    try 
        {
        table_rules = GetTableRules("access_whitelist");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        con = general.DBConnectionWrite(DBServer);
        stmt = con->createStatement();
        data = json["data"];        
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateAccessWhitelist", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateAccessWhitelist", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateAccessWhitelist", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                carparkstring = data[i]["carpark_number"].asString();


                carparklist = "";
                auto list = split(carparkstring, ',');
                bool first = true;
                for (auto carpark_number : list) 
                    {                                  
                    int carpark_id = cloud_carpark_id[stoi(carpark_number)];                   
                    if (carpark_id > 0) {
                        if (!first && carpark_id > 0) {
                            carparklist = carparklist + ",";
                        }
                        carparklist = carparklist + to_string(carpark_id);
                        first = false;
                    }
                }

                plate_number = data[i]["plate_number"].asString();
                country = data[i]["country"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                access_id = data[i]["access_id"].asInt();
                wallet_user_id = data[i]["wallet_user_id"].asInt();
                operator_id = data[i]["operator_id"].asInt();
                start_date = data[i]["start_date"].asString();
                expiry_date = data[i]["expiry_date"].asString();
                wallet_payment = data[i]["wallet_payment"].asInt();
                season_card = data[i]["season_card"].asInt();
                corporate_parker = data[i]["corporate_parker"].asInt();
                antipassback = data[i]["antipassback"].asInt();
                access_zones = data[i]["access_zones"].asString();
                creation_date = data[i]["creation_date"].asString();
                status = data[i]["status"].asInt();
                last_visited = data[i]["last_visited"].asString();
                company_name = data[i]["company_name"].asString();
                customer_name = data[i]["customer_name"].asString();
                tag = data[i]["tag"].asString();
                tag_serial = data[i]["tag_serial"].asString();
                tag_tid = data[i]["tag_tid"].asString();

                last_visited=mysqldate(last_visited);
                start_date=mysqldate(start_date);
                expiry_date=mysqldate(expiry_date);


                res = stmt->executeQuery("Select id from access_whitelist where access_id = " + to_string(access_id));
                if (res->next()) 
                    {
                    rowid = res->getInt("id");
                    sql = "Update access_whitelist set plate_number = '" + plate_number + "',country = '" + country + "',ticket_id = '" + ticket_id + "',last_sync = CURRENT_TIMESTAMP(),wallet_user_id = " + to_string(wallet_user_id) + ",start_date  = " + start_date + ",expiry_date=" + expiry_date + ",wallet_payment = " + to_string(wallet_payment) + ",season_card = " + to_string(season_card) + ",corporate_parker=" + to_string(corporate_parker) + ",antipassback=" + to_string(antipassback) + ",access_zones='" + access_zones + "',creation_date = '" + creation_date + "',status=" + to_string(status) + ",last_visited=" + last_visited + ",company_name='" + company_name + "',customer_name='" + customer_name + "',tag='" + tag + "',download_flag=1 where id = " + to_string(rowid);
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                        {
                        jsonresponseid[j] = id;
                        j++;
                        }
                    } 
                else 
                    {
                    sql = "Insert into access_whitelist (carpark_number,facility_numbers,plate_number,access_id,country,ticket_id,wallet_user_id,start_date ,expiry_date,wallet_payment,season_card,corporate_parker,antipassback,access_zones,creation_date,status,last_visited,company_name,last_sync,customer_name,tag,operator_id,download_flag) values('" + carparklist + "','" + facility_number + "','" + plate_number + "'," + to_string(access_id) + ",'" + country + "','" + ticket_id + "'," + to_string(wallet_user_id) + "," + start_date + "," + expiry_date + "," + to_string(wallet_payment) + "," + to_string(season_card) + "," + to_string(corporate_parker) + "," + to_string(antipassback) + ",'" + access_zones + "','" + creation_date + "'," + to_string(status) + "," + last_visited + ",'" + company_name + "',CURRENT_TIMESTAMP(),'" + customer_name + "','" + tag_tid + "'," + to_string(operator_id) + ",1)";
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                        {
                        jsonresponseid[j] = id;
                        j++;
                        }
                    }
                delete res;
            }
        }
        jsonresponse["message"] = "success";               
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateAccessWhitelist", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "access_whitelist";
    return jsonresponse;
    }

Json::Value UpdateParkingReservation(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";    
    int id, result = 0, j = 0, rowid = 0;
    int carpark_number, payment_flag, cancel_flag, mobile_app_user_id,operator_id;
    string country, plate_number, ticket_id, name, address, phone, email, parking_bay_number, from_time, to_time, reserved_parking_duration, reserved_parking_fee, payment_reference_id, transaction_id, creation_date_time;
    try 
        {
        table_rules = GetTableRules("parking_reservation");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        con = general.DBConnectionWrite(DBServer);
        stmt = con->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingReservation", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingReservation", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingReservation", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                transaction_id = data[i]["transaction_id"].asString();
                mobile_app_user_id = data[i]["mobile_app_user_id"].asInt();
                operator_id=data[i]["operator_id"].asInt();
                plate_number = data[i]["plate_number"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                name = data[i]["name"].asString();
                address = data[i]["address"].asString();
                country = data[i]["country"].asString();
                phone = data[i]["phone"].asString();
                email = data[i]["email"].asString();
                carpark_number = data[i]["carpark_number"].asInt();

                parking_bay_number = data[i]["parking_bay_number"].asString();
                from_time = data[i]["from_time"].asString();
                to_time = data[i]["to_time"].asString();
                creation_date_time = data[i]["creation_date_time"].asString();
                reserved_parking_duration = data[i]["reserved_parking_duration"].asString();
                reserved_parking_fee = data[i]["reserved_parking_fee"].asString();
                payment_reference_id = data[i]["payment_reference_id"].asString();
                payment_flag = data[i]["payment_flag"].asInt();
                cancel_flag = data[i]["cancel_flag"].asInt();

                from_time=mysqldate(from_time);
                to_time=mysqldate(to_time);

                res = stmt->executeQuery("Select reservation_id from parking_reservation where transaction_id = '" + transaction_id+"'");
                if (res->next()) 
                    {
                    rowid = res->getInt("reservation_id");
                    sql = "Update parking_reservation set download_flag=1,plate_number = '" + plate_number + "',ticket_id = '" + ticket_id + "',name = '" + name + "',address = '" + address + "',country  = '" + country + "',phone='" + phone + "',email = '" + email + "',carpark_number = " + to_string(carpark_number) + ",facility_number=" + facility_number + ",parking_bay_number='" + parking_bay_number + "',from_time=" + from_time + ",to_time = " + to_time + ",reserved_parking_duration='" + reserved_parking_duration + "',reserved_parking_fee='" + reserved_parking_fee + "',payment_reference_id='" + payment_reference_id + "',payment_flag=" + to_string(payment_flag) + ",cancel_flag=" + to_string(cancel_flag) + " where reservation_id = " + to_string(rowid);
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                        {
                        jsonresponseid[j] = id;
                        j++;
                        }
                    } 
                else 
                    {
                    sql = "Insert into parking_reservation (transaction_id,mobile_app_user_id,operator_id,carpark_number,facility_number,plate_number,ticket_id,country,name,address ,phone,email,parking_bay_number,from_time,to_time,reserved_parking_duration,reserved_parking_fee,payment_reference_id,payment_flag,cancel_flag,creation_date_time,download_flag) values('" + transaction_id + "'," + to_string(mobile_app_user_id) + ","+to_string(operator_id)+",'" + to_string(carpark_number) + "','" + facility_number + "','" + plate_number + "','" + ticket_id + "','" + country + "','" + name + "','" + address + "','" + phone + "','" + email + "','" + parking_bay_number + "'," + from_time + "," + to_time + ",'" + reserved_parking_duration + "'," + reserved_parking_fee + ",'" + payment_reference_id + "'," + to_string(payment_flag) + "," + to_string(payment_flag) + ",'" + creation_date_time + "',1)";
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                        {
                        jsonresponseid[j] = id;
                        j++;
                        }
                    }
                delete res;
            }
        }
        jsonresponse["message"] = "success";                
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingReservation", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_reservation";
    return jsonresponse;
    }

Json::Value UpdateParkingSubscription(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";
    int id, result = 0, j = 0;    
    int user_id, operator_id, product_id;
    string user_name, product_name, validity_start_date, validity_end_date, product_price, date_time, customer_name, company_name, customer_email, customer_mobile, description, plate_number, tag, ticket_id;
    try 
        {
        table_rules = GetTableRules("product_transactions");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        con = general.DBConnectionWrite(DBServer);
        stmt = con->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
             data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingSubscription", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingSubscription", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingSubscription", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                plate_number = data[i]["plate_number"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                tag = data[i]["tag"].asString();
                customer_name = data[i]["customer_name"].asString();
                company_name = data[i]["company_name"].asString();
                customer_mobile = data[i]["customer_mobile"].asString();
                customer_email = data[i]["customer_email"].asString();                  
                product_name = data[i]["product_name"].asString();
                validity_start_date = data[i]["validity_start_date"].asString();
                validity_end_date = data[i]["validity_end_date"].asString();
                product_price = data[i]["product_price"].asString();
                date_time = data[i]["date_time"].asString();
                description = data[i]["description"].asString();            
                product_id = data[i]["product_id"].asInt();
                operator_id = data[i]["cloud_operator_id"].asInt();

                sql = "select * from application_users where user_qrcode='" + ticket_id + "'";
                res = stmt->executeQuery(sql);
                if (res->next()) 
                    {
                    user_id = res->getInt("user_id");
                    user_name = res->getString("username");
                    } 
                else 
                    {
                    user_id = 0;
                    user_name = "";
                    }



                sql = "Insert into product_transactions (product_name,product_type,validity_start_date,validity_end_date,price,user_id,product_id,status,date_time,user_name,operator_id)values('" + product_name + "','Contract Parking','" + validity_start_date + "','" + validity_end_date + "','" + product_price + "'," + to_string(user_id) + "," + to_string(product_id) + ",1,'" + date_time + "','" + user_name + "'," + to_string(operator_id) + ")";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                    {
                    jsonresponseid[j] = id;
                    j++;
                    }
            }

        }
        jsonresponse["message"] = "success";        
        
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingReservation", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "contract_parking_transactions";
    return jsonresponse;
    }

Json::Value UpdateParkingMovementsIOT(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name,string ipaddress) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    

    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";
    int id, result = 0, j = 0;    
    int device_number,movement_type,plate_capture_id,carpark_number,parking_id,status_code,iot_mode;
    string date_time, device_name, plate_number,ticket_id,utc_date_time,user_id,iot_mode_name,secure_key,decoded_ticket_id;
    try 
        {
        table_rules = GetTableRules("parking_movements_iot");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
             data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingSubscription", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingSubscription", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingMovements", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                date_time = data[i]["date_time"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                movement_type = data[i]["movement_type"].asInt();
                plate_number = data[i]["plate_number"].asString();                  
                plate_capture_id = data[i]["plate_capture_id"].asInt();
                utc_date_time =data[i]["utc_date_time"].asString();
                parking_id =data[i]["parking_id"].asInt();
                user_id =data[i]["user_id"].asString();
                status_code =data[i]["status_code"].asInt();
                iot_mode =data[i]["iot_mode"].asInt();
                iot_mode_name=data[i]["iot_mode_name"].asString();
                secure_key=data[i]["secure_key"].asString();
                decoded_ticket_id = data[i]["decoded_ticket_id"].asString();
                carpark_number = data[i]["carpark_number"].asInt();
                int carpark_id = cloud_carpark_id[carpark_number];                         
                string carpark_name =cloud_carpark_name[carpark_number];
                


                sql = "Insert into parking_movements_iot(date_time,ticket_id,device_number,device_name,movement_type,carpark_number,facility_number,plate_number,create_date_time,plate_captured_id,carpark_name,carpark_id,utc_date_time,parking_id,user_id,status_code,iot_mode,iot_mode_name,secure_key,decoded_ticket_id,ip_address) Values ('"+date_time+"','"+ticket_id+"',"+to_string(device_number)+",'"+device_name+"',"+to_string(movement_type)+","+to_string(carpark_number)+","+facility_number+",'"+plate_number+"','"+date_time+"',"+to_string(plate_capture_id)+",'"+carpark_name+"',"+to_string(carpark_id)+",'"+utc_date_time+"',"+to_string(parking_id)+",'"+user_id+"',"+to_string(status_code)+","+to_string(iot_mode)+",'"+iot_mode_name+"','"+secure_key+"','"+decoded_ticket_id+"','"+ipaddress+"')";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }
            }

        }
        jsonresponse["message"] = "success";        
        
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingMovementsIOT", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_movements";
    return jsonresponse;
    }

Json::Value UpdateParkingMovementsReservation(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;

    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";
    int id, result = 0, j = 0;    
    int device_number,movement_type,plate_capture_id,carpark_number;
    string date_time, device_name, plate_number,ticket_id,chip_utid;
    try 
        {
        table_rules = GetTableRules("parking_movements_reservation");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
             data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingSubscription", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingSubscription", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingMovementsReservation", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                date_time = data[i]["date_time"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                movement_type = data[i]["movement_type"].asInt();
                plate_number = data[i]["plate_number"].asString();                  
                plate_capture_id = data[i]["plate_capture_id"].asInt();
                carpark_number = data[i]["carpark_number"].asInt();
                int carpark_id = cloud_carpark_id[carpark_number];                         
                string carpark_name =cloud_carpark_name[carpark_number];



                sql = "Insert into parking_movements_reservation(date_time,ticket_id,device_number,device_name,movement_type,carpark_number,facility_number,plate_number,plate_captured_id,carpark_name,carpark_id) Values ('"+date_time+"','"+ticket_id+"',"+to_string(device_number)+",'"+device_name+"',"+to_string(movement_type)+","+to_string(carpark_number)+","+facility_number+",'"+plate_number+"',"+to_string(plate_capture_id)+",'"+carpark_name+"',"+to_string(carpark_id)+")";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }
            }

        }
        jsonresponse["message"] = "success";        
        
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingMovementsReservation", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_movements_reservation";
    return jsonresponse;
    }

Json::Value UpdateValetParking(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponse,table_rules;   
    Json::Value jsonresponseid;
    int result = 0;
    int j = 0, rowid;
    string sql;
    string id,customer_name,mobile_no,plate_number,plate_prefix,plate_country,plate_emirate;
    string front_plate,left_plate,right_plate,back_plate,ticket_number,customer_card_number,customer_card_id;
    string key_card_id,key_card_number,key_position,parking_bay,paid_status,payment_date_time,valet_out,parking_in;
    string parking_out,dropoff_datetime,ready_for_delivery,valet_in_datetime,valet_out_datetime,parking_in_datetime;
    string parking_out_datetime, pickup_driver_id,pickup_driver_name,delivery_driver_id,delivery_driver_name;
    string duration_pickup_parking,duration_parking,duration_payment_parking,duration_parking_dropoff,duration_dropoff_delivery;
    string device_number,operator_id,operator_name,car_brand,car_color,car_model,record_creation_datetime;
    string carpark_number,disable_flag,disable_reason,disable_datetime;    
    
    try 
    {
        table_rules = GetTableRules("valet_parking");
        con = general.DBConnectionWrite(DBServer);
        stmt = con->createStatement();               
       
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingSubscription", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingSubscription", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateValetParking", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                device_number = data[i]["device_number"].asString();
                pickup_driver_name = data[i]["pickup_driver_name"].asString();
                delivery_driver_id = data[i]["delivery_driver_id"].asString();
                delivery_driver_name = data[i]["delivery_driver_name"].asString();
                duration_pickup_parking = data[i]["duration_pickup_parking"].asString();
                duration_parking = data[i]["duration_parking"].asString();
                duration_payment_parking = data[i]["duration_payment_parking"].asString();
                duration_parking_dropoff = data[i]["duration_parking_dropoff"].asString();
                duration_dropoff_delivery = data[i]["duration_dropoff_delivery"].asString();
                operator_id = data[i]["operator_id"].asString();
                operator_name = data[i]["operator_name"].asString();
                car_brand = data[i]["car_brand"].asString();
                car_color = data[i]["car_color"].asString();
                car_model = data[i]["car_model"].asString();
                record_creation_datetime = data[i]["record_creation_datetime"].asString();


                parking_out = data[i]["parking_out"].asString();
                dropoff_datetime = data[i]["dropoff_datetime"].asString();
                valet_in_datetime = data[i]["valet_in_datetime"].asString();
                valet_out_datetime = data[i]["valet_out_datetime"].asString();
                ready_for_delivery = data[i]["ready_for_delivery"].asString();
                parking_in_datetime = data[i]["parking_in_datetime"].asString();
                parking_out_datetime = data[i]["parking_out_datetime"].asString();
                pickup_driver_id = data[i]["pickup_driver_id"].asString();

                key_card_id = data[i]["key_card_id"].asString();
                key_card_number = data[i]["key_card_number"].asString();
                key_position = data[i]["key_position"].asString();
                parking_bay = data[i]["parking_bay"].asString();
                paid_status = data[i]["paid_status"].asString();
                payment_date_time = data[i]["payment_date_time"].asString();
                valet_out = data[i]["valet_out"].asString();
                parking_in = data[i]["parking_in"].asString();

                id = data[i]["id"].asString();
                carpark_number = data[i]["carpark_number"].asString(); 

                string carpark_name =cloud_carpark_name[stoi(carpark_number)];           
                string carpark_id = cloud_carpark_id[stoi(carpark_number)];                        

                customer_name = data[i]["customer_name"].asString();
                mobile_no = data[i]["mobile_no"].asString();
                plate_number = data[i]["plate_number"].asString();            
                plate_prefix = data[i]["plate_prefix"].asString();
                plate_country = data[i]["plate_country"].asString();
                plate_emirate = data[i]["plate_emirate"].asString();            
                front_plate = data[i]["front_plate"].asString();            
                left_plate = data[i]["left_plate"].asString();
                right_plate = data[i]["right_plate"].asString();
                back_plate = data[i]["back_plate"].asString();
                ticket_number = data[i]["ticket_number"].asString();            
                customer_card_number = data[i]["customer_card_number"].asString();
                customer_card_id = data[i]["customer_card_id"].asString();
                disable_flag = data[i]["disable_flag"].asString();
                disable_reason = data[i]["disable_reason"].asString();
                disable_datetime = data[i]["disable_datetime"].asString();


                rowid=0;   
                result=1;
                sql="Select * from valet_parking where carpark_id=" + carpark_id + " and ticket_number='" + ticket_number + "'";            
                res = stmt->executeQuery(sql);
                if (res->next())                 
                    rowid = res->getInt("id");  


                if (stoi(valet_out)==1) 
                    {
                    if(rowid>0 && res->getInt("valet_out")==0)
                        {
                        sql="Delete from live_valet_parking where carpark_id=" + carpark_id + " and ticket_number='" + ticket_number + "'";
                        stmt->executeUpdate(sql);                
                        }
                    }

                if(rowid==0)                
                    {
                    if (stoi(valet_out)==0 && stoi(disable_flag)==0) 
                        {
                        sql="INSERT INTO live_valet_parking(carpark_number,carpark_name,facility_number,facility_name,carpark_id,customer_name,mobile_no,plate_number,front_plate,left_plate,right_plate,back_plate,customer_card_id,customer_card_number,ticket_number,plate_country,plate_emirate,device_number,operator_id,operator_name,plate_prefix,key_card_id,valet_in_datetime,car_brand,car_color,car_model,key_card_number) VALUES('"+carpark_number+"','"+carpark_name+"','"+facility_number+"','"+facility_name+"','"+carpark_id+"','"+customer_name+"','"+mobile_no+"','"+plate_number+"','"+front_plate+"','"+left_plate+"','"+right_plate+"','"+back_plate+"','"+customer_card_id+"','"+customer_card_number+"','"+ticket_number+"','"+plate_country+"','"+plate_emirate+"','"+device_number+"','"+operator_id+"','"+operator_name+"','"+plate_prefix+"','"+key_card_id+"','"+valet_in_datetime+"','"+car_brand+"','"+car_color+"','"+car_model+"','"+key_card_number+"')"; 
                        result = stmt->executeUpdate(sql);
                        }
                    sql="INSERT INTO valet_parking(carpark_number,carpark_name,facility_number,facility_name,carpark_id,customer_name,mobile_no,plate_number,front_plate,left_plate,right_plate,back_plate,customer_card_id,customer_card_number,ticket_number,plate_country,plate_emirate,device_number,operator_id,operator_name,plate_prefix,key_card_id,valet_in_datetime,car_brand,car_color,car_model,key_card_number) VALUES('"+carpark_number+"','"+carpark_name+"','"+facility_number+"','"+facility_name+"','"+carpark_id+"','"+customer_name+"','"+mobile_no+"','"+plate_number+"','"+front_plate+"','"+left_plate+"','"+right_plate+"','"+back_plate+"','"+customer_card_id+"','"+customer_card_number+"','"+ticket_number+"','"+plate_country+"','"+plate_emirate+"','"+device_number+"','"+operator_id+"','"+operator_name+"','"+plate_prefix+"','"+key_card_id+"','"+valet_in_datetime+"','"+car_brand+"','"+car_color+"','"+car_model+"','"+key_card_number+"')"; 
                    result = stmt->executeUpdate(sql);                
                    }

                sql="";
                if(stoi(parking_in)==1)                                                                   
                    sql=sql+"key_position='"+key_position+"',parking_bay='"+parking_bay+"',parking_in=1,parking_in_datetime="+mysqldate(parking_in_datetime)+",pickup_driver_id='"+pickup_driver_id+"',pickup_driver_name='"+pickup_driver_name+"',duration_pickup_parking='"+duration_pickup_parking+"'";

                if(stoi(paid_status)==1)
                    {
                    if(sql!="")
                        sql=sql+",";                                       

                    sql=sql+"paid_status=1,payment_date_time="+mysqldate(payment_date_time);                
                    } 

                if(stoi(parking_out)==1)
                    {
                    if(sql!="")
                        sql=sql+",";                                        

                    sql=sql+"parking_out=1,parking_out_datetime="+mysqldate(parking_out_datetime)+",delivery_driver_id='"+delivery_driver_id+"',delivery_driver_name='"+delivery_driver_name+"',duration_payment_parking='"+duration_payment_parking+"'";                
                    }

                if(stoi(ready_for_delivery)==1)
                    {
                    if(sql!="")
                        sql=sql+",";                                        

                    sql=sql+"ready_for_delivery=1,dropoff_datetime="+mysqldate(dropoff_datetime)+",duration_parking_dropoff='"+duration_parking_dropoff+"'";                
                    } 

                if(stoi(valet_out)==1)
                    {
                    if(sql!="")
                        sql=sql+",";                                       

                    sql=sql+"valet_out=1,duration_dropoff_delivery='"+duration_dropoff_delivery+"',valet_out_datetime="+mysqldate(valet_out_datetime);                    
                    }

                if(stoi(disable_flag)==1)
                    {
                    if(sql!="")
                        sql=sql+",";                                       

                    sql=sql+"disable_flag=1,disable_reason='"+disable_reason+"',disable_datetime="+mysqldate(disable_datetime);                    
                    }

                if(sql!="")
                    {
                    sql=sql+" where ticket_number='"+ticket_number+"' and carpark_id='"+carpark_id+"'" ;  
                    result = stmt->executeUpdate("update valet_parking set plate_number='"+plate_number+"',plate_prefix='"+plate_prefix+"',front_plate='"+front_plate+"',left_plate='"+left_plate+"',right_plate='"+right_plate+"',back_plate='"+back_plate+"',plate_country='"+plate_country+"',plate_emirate='"+plate_emirate+"',"+sql);

                    if (stoi(valet_out)==0) 
                        result = stmt->executeUpdate("update live_valet_parking set "+sql);
                    }

                result=1;
                if (result == 1) 
                    {
                    jsonresponseid[j] = id;
                    j++;
                    }
                delete res;    
            }
        }
        
        
        delete stmt;
        delete con;        
    } 
    catch (const std::exception &e) 
    {
        WriteException("postValetParking", e.what());
        WriteException("postValetParking", sql);
        
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();
        jsonresponse["table"] = "valet_parking";        
    }
    jsonresponse["data"] = jsonresponseid;        
    jsonresponse["table"] = "valet_parking";
    return jsonresponse;
    }

Json::Value UpdateParkingValidation(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";    
    int id, result = 0, j = 0, rowid = 0;
    int validation_value,product_id,validator_id;
    string date_time,carpark_number, plate_number, ticket_id, product_name, validator_name,validation_type;
    try 
        {
        table_rules = GetTableRules("parking_validation");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        con = general.DBConnectionWrite(DBServer);
        stmt = con->createStatement();
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingReservation", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingReservation", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateParkingValidation", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                date_time = data[i]["date_time"].asString();
                plate_number = data[i]["plate_number"].asString();
                ticket_id = data[i]["ticket_id"].asString();
                validator_id = data[i]["validator_id"].asInt();
                validator_name = data[i]["validator_name"].asString();
                product_id = data[i]["product_id"].asInt();
                product_name = data[i]["product_name"].asString();
                validation_type = data[i]["validation_type"].asString();
                validation_value = data[i]["validation_value"].asInt();
                carpark_number = data[i]["carpark_number"].asString();
                string carpark_name =cloud_carpark_name[stoi(carpark_number)];           
                string carpark_id = cloud_carpark_id[stoi(carpark_number)];   
                
                res = stmt->executeQuery("Select id from parking_validation where ticket_id = '" + ticket_id+"' and carpark_id = "+carpark_id);
                if (res->next()) 
                {
                    rowid = res->getInt("id");
                    sql = "Update parking_validation set download_flag=1,plate_number = '" + plate_number + "',ticket_id = '" + ticket_id + "',validator_id = "+to_string(validator_id)+",validator_name = '"+validator_name+"',product_id="+to_string(product_id)+",product_name = '"+product_name+"',validation_type='"+validation_type+"',validation_value="+to_string(validation_value)+" where id = " + to_string(rowid);
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                    {
                        jsonresponseid[j] = id;
                        j++;
                    }
                } 
                else 
                {
                    sql = "Insert into parking_validation (ticket_id,date_time,plate_number,validator_id,validator_name,product_id,product_name,validation_type,validation_value,carpark_id,carpark_number,carpark_name,facility_number,facility_name,download_flag) values('" + ticket_id + "','" +date_time+ "','"+plate_number+"'," + to_string(validator_id) + ",'" + validator_name + "'," + to_string(product_id) + ",'" + product_name + "','" + validation_type + "'," + to_string(validation_value) + "," +carpark_id + ","+carpark_number+",'" + carpark_name + "'," +facility_number + ",'" + facility_name + "',1)";
                    result = stmt_write->executeUpdate(sql);
                    if (result == 1) 
                    {
                        jsonresponseid[j] = id;
                        j++;
                    }
                }
                delete res;
            }
        }
        jsonresponse["message"] = "success";                
        delete stmt_write;
        delete con_write;
        delete stmt;
        delete con;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateParkingValidation", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "parking_validation";
    return jsonresponse;
    }

Json::Value UpdatePlatesCaptured(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) {
    sql::Connection *con_write;
    sql::Statement *stmt_write;
    Json::Value data, jsonresponseid, jsonresponse,table_rules;
    string sql = "";    
    int id, result = 0, j = 0;
    int camera_device_number,confidence_rate,user_id,carpark_number ;
    string capture_date_time,initial_plate_number, plate_number,plate_type,plate_image_name, camera_name,plate_area,plate_country,plate_corrected_date_time,user_name;
    try 
        {
        table_rules = GetTableRules("plates_captured");
        con_write = general.DBConnectionWrite(DBServer);
        stmt_write = con_write->createStatement();
        
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
           // WriteToLog("UpdateParkingReservation", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateParkingReservation", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdatePlatesCaptured", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                capture_date_time = data[i]["capture_date_time"].asString();
                plate_number = data[i]["plate_number"].asString();
                plate_type = data[i]["plate_type"].asString();
                plate_image_name = data[i]["plate_image_name"].asString();
                camera_device_number = data[i]["camera_device_number"].asInt();
                camera_name = data[i]["camera_name"].asString();
                plate_area = data[i]["plate_area"].asString();
                plate_country = data[i]["plate_country"].asString();
                initial_plate_number = data[i]["initial_plate_number"].asString();
                confidence_rate = data[i]["confidence_rate"].asInt();
                plate_corrected_date_time = data[i]["plate_corrected_date_time"].asString();
                user_id = data[i]["user_id"].asInt();
                user_name = data[i]["user_name"].asString();
                carpark_number = GetCarparkFromDevice(camera_device_number,facility_number);
                WriteToLog("UpdateParkingReservation", "Carpark :"+to_string(carpark_number));
                string carpark_name =cloud_carpark_name[carpark_number];           
                string carpark_id = cloud_carpark_id[carpark_number];   
                
                
                sql = "INSERT INTO plates_captured (camera_device_number, camera_name, plate_number, plate_type, plate_image_name, capture_date_time, plate_area, plate_country, initial_plate_number, confidence_rate, plate_corrected_date_time, user_id, user_name, carpark_number, facility_number, carpark_id) VALUES("+to_string(camera_device_number)+",'"+camera_name+"', '"+plate_number+"', '"+plate_type+"', '"+plate_image_name+"', '"+capture_date_time+"', '"+plate_area+"', '"+plate_country+"', '"+initial_plate_number+"',"+to_string(confidence_rate)+","+mysqldate(plate_corrected_date_time)+","+to_string(user_id)+",'"+user_name+"',"+to_string(carpark_number)+",'"+facility_number+"',"+carpark_id+")";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }

            }
        }
        jsonresponse["message"] = "success";                
        delete stmt_write;
        delete con_write;

        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdatePlatesCaptured", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();    
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "plates_captured";
    return jsonresponse;
    }

Json::Value UpdateWatchdogAlarmsIOT(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name,string ipaddress) {   
    sql::Connection *con_write;
    con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data;
    string sql = "";
    int id;
    string description, datetime, device_name, code,validation_response,iot_mode_name,alarm;
    int  severity, carpark_number,device_number,iot_mode;
    
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;    
    try 
        {
        table_rules = GetTableRules("watchdog_device_alarms_iot");
        
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();                
        data = json["data"];
        
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateWatchdogAlarms", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateWatchdogAlarms", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateWatchdogAlarmsIOT", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                device_number = data[i]["device_number"].asInt();
                device_name = data[i]["device_name"].asString();
                carpark_number = data[i]["carpark_number"].asInt();           
                string carpark_name =cloud_carpark_name[carpark_number];           
                int carpark_id = cloud_carpark_id[carpark_number];                                                                     
                severity = data[i]["alarm_severity"].asInt();
                code = data[i]["alarm_code"].asString();
                description = data[i]["alarm_description"].asString();
                datetime = data[i]["alarm_date_time"].asString();
                alarm = data[i]["alarm"].asString();
                //create_datetime = data[i]["create_date_time"].asString();
                //dismiss = data[i]["dismiss"].asInt();
                iot_mode =data[i]["iot_mode"].asInt();
                iot_mode_name=data[i]["iot_mode_name"].asString();
        
                sql = "INSERT into watchdog_device_alarms_iot(carpark_name,carpark_number,carpark_id,facility_name,facility_number,device_name,device_number,alarm_severity,alarm_code,alarm_description,alarm,alarm_date_time,iot_mode,iot_mode_name,ip_address) VALUES('" + carpark_name + "'," + to_string(carpark_number) + "," + to_string(carpark_id) + ",'" + facility_name + "'," + facility_number + ",'" + device_name + "'," + to_string(device_number) + "," + to_string(severity) + ",'" + code + "','" + description + "','"+alarm+"'," + mysqldate(datetime) + ","+to_string(iot_mode)+",'"+iot_mode_name+"','"+ipaddress+"')";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }   
            }
        }                
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateWatchdogAlarmsIOT", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();        
        delete con_write;        
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "watchdog_device_alarms";
    return jsonresponse;
    }


Json::Value UpdateAccessRequestIOT(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name,string ipaddress) {   
    sql::Connection *con_write;
    con_write=NULL;
    sql::Statement *stmt_write;
    Json::Value data;
    string sql = "";
    int id;
    string token_request_date_time,token_response_date_time,token_response,api_request_date_time,api_response_date_time,api_request,api_response,response_status,ticket_id,plate_number,iot_mode_name;
    int plate_captured_id,iot_mode,api_response_delay;
    
    Json::Value jsonresponse, jsonresponseid,table_rules;
    int result = 0;
    int j = 0;    
    try 
        {
        table_rules = GetTableRules("access_request_iot");
        
        con_write = general.DBConnectionWrite(DBReporting);
        stmt_write = con_write->createStatement();                
        data = json["data"];
        
        for (int i = 0; i < (signed)data.size(); i++) 
        {
            data[i] = validation.checkSpecialCharacters(data[i],table_rules);
            //WriteToLog("UpdateAccessRequestIOT", fw.write(data[i]));
            Json::Value validation_response = validation.checkValidation(data[i],table_rules);           
            
            //WriteToLog("UpdateAccessRequestIOT", fw.write(validation_response));
            if(validation_response["result"]=="failed")
            {
                WriteException("UpdateAccessRequestIOT", fw.write(validation_response));
                jsonresponse["validation"] = "failed";
                jsonresponse["validation_details"] = validation_response["validation_details"];
            }
            else
            {
                id = data[i]["id"].asInt();
                api_request_date_time = data[i]["api_request_date_time"].asString(); 
                api_response_date_time = data[i]["api_response_date_time"].asString(); 
                api_response_delay =data[i]["api_response_delay"].asInt();
                api_request =data[i]["api_request"].asString();     
                api_response =data[i]["api_response"].asString();
                response_status =data[i]["response_status"].asString();
                ticket_id =data[i]["ticket_id"].asString();
                plate_number =data[i]["plate_number"].asString();
                plate_captured_id = data[i]["plate_captured_id"].asInt();
                iot_mode =data[i]["iot_mode"].asInt();
                iot_mode_name=data[i]["iot_mode_name"].asString();
        
                sql = "INSERT into access_request_iot(api_request_date_time,api_response_date_time,api_response_delay,api_request,api_response,response_status,ticket_id,plate_number,plate_captured_id,iot_mode,iot_mode_name,ip_address) VALUES(" +  mysqldate(api_request_date_time) + "," + mysqldate(api_response_date_time) + ","+to_string(api_response_delay)+",'" + api_request + "','" +api_response + "','" +response_status + "','" + ticket_id + "','" + plate_number + "',"+to_string(plate_captured_id)+","+to_string(iot_mode)+",'"+iot_mode_name+"','"+ipaddress+"')";
                result = stmt_write->executeUpdate(sql);
                if (result == 1) 
                {
                    jsonresponseid[j] = id;
                    j++;
                }   
            }
        }                
        delete stmt_write;
        delete con_write;
        } 
    catch (const std::exception &e) 
        {
        WriteException("UpdateAccessRequestIOT", e.what());
        jsonresponse["message"] = "failed";
        jsonresponse["error"] = e.what();        
        delete con_write;        
        }
    jsonresponse["data"] = jsonresponseid;
    jsonresponse["table"] = "access_request";
    return jsonresponse;
    }




void UpdateFacilityDateTime(string facility_number, int type) {
    sql::Connection *conn;
    sql::Statement *stmt;
    try 
        {
        conn = general.DBConnectionRead(DBServer);
        stmt = conn->createStatement();
        if (type == 1) 
            {        
            stmt->executeUpdate("Update system_facility set last_upload_datetime=CURRENT_TIMESTAMP() where facility_number=" + facility_number);
            } 
        else if (type == 2) 
            {            
            stmt->executeUpdate("Update system_facility set last_download_datetime=CURRENT_TIMESTAMP() where facility_number=" + facility_number);
            }
        delete stmt;
        delete conn;
        } 
    catch (sql::SQLException &e) 
        {       
        WriteException("UpdateFacilityDateTime", e.what());
        }
    }

Php::Value PostDataToServer(Php::Parameters &params) 
    {
    Json::Reader reader;
    Json::Value json;
    int task;
    Json::Value response, array, result;

    string jsondata = params[0];
    WriteToLog("PostDataToServer", "Json:" + jsondata);
    reader.parse(jsondata, json, true);
    
    string facility_number = json["facility_number"].asString();      
    string ipaddress =  json["ipaddress"].asString();
    
    operator_id=json["operator_id"].asString();        
    if(facility_number=="")
        {
        facility_number = json["facility_no"].asString(); 
        if(facility_number=="200005")
            operator_id="43";            
        }
    WriteToLog("PostDataToServer", "Facility number: " + facility_number);
    WriteToLog("PostDataToServer", "Operator id: " + operator_id); 
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;    
    string message="success";
    string facility_name;
    Php::Value cloud_carpark_id,cloud_carpark_name;
    try 
        {
        con = general.DBConnectionRead(DBServer);
        stmt = con->createStatement();
        string carpark_id;
        res = stmt->executeQuery("Select carparks from system_users where user_id=" + operator_id);
        if (res->next()) 
            {
            carpark_id = res->getString("carparks");
            delete res;
            
            WriteToLog("setFaciltyDetails","Carpark id for operator : "+carpark_id);
            
            res = stmt->executeQuery("Select facility_name from system_facility where facility_number=" + facility_number);
            if (res->next()) 
                {
                facility_name = res->getString("facility_name");
                WriteToLog("setFaciltyDetails", "Facility name:" + facility_name);                
                delete res;  
                
                
                res=stmt->executeQuery("select carpark_id,carpark_name,carpark_number from system_carparks where facility_number="+facility_number+ " and carpark_id in("+carpark_id+")");
                if(res->rowsCount()>0)
                    {
                    string carpark_number,carpark_name;                                    
                    while(res->next())
                        {
                        carpark_number=res->getString("carpark_number");
                        carpark_name=res->getString("carpark_name");
                        carpark_id=res->getString("carpark_id");
                        cloud_carpark_id[stoi(carpark_number)]=carpark_id;
                        cloud_carpark_name[stoi(carpark_number)]=carpark_name;
                        WriteToLog("setFaciltyDetails", "carpark name:" + carpark_name+"\t carpark number: "+carpark_number+"\tcarpark id: "+carpark_id);                                                
                        }
                    delete res;
                    }
                else
                    message="Facility is not accesssible for this user";
                }
            else
                message="Facility number is not registered in cloud";
                                        
                                                                                                                     
            }
        else
            message="cloud operator id not present";
                    
        delete stmt;
        delete con;
        } 
    catch (const std::exception& e) 
        {        
        WriteException("setFaciltyDetails", e.what());
        message= "exception";
        }  
            
    WriteToLog("PostDataToServer", "Set details response:"+message);        
    result["response_message"]=message;
    
    if(message=="success")
        {
        for (int i = 0; i < (signed)json["table"].size(); i++) {
            response = "";
            task = json["table"][i]["task"].asInt();
            switch (task) 
                {
                case 1:
                    response = UpdateParkingOccupancy(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                 
                    break;
                case 2:
                    response = UpdateDeviceAlarms(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 3:
                    response = UpdateDeviceStatus(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 4:
                    response = UpdateOpenTransactions(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 5:
                    response = UpdateParkingDuration(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 6:
                    response = UpdateRevenueSummary(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 7:
                    response = UpdateParkingMovementsSummary(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 8:
                    response = UpdateParkingMovementsAccess(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 9:
                    response = UpdateRevenuePayments(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 10:
                    response = UpdateHourlyOccupancy(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 11:
                    response = UpdateParkingRevenue(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 12:
                    response = UpdateParkingMovementsManual(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 13:
                    response = UpdateRevenueShifts(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 14:
                    response = UpdateAccessWhitelist(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);                    
                    break;
                case 15:
                    response = UpdateParkingReservation(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);
                    break;
                case 16:
                    response = UpdateParkingSubscription(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);
                    break;
                case 17:
                    response = UpdateParkingMovementsIOT(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name,ipaddress);
                    break;
                case 18:
                    response = UpdateParkingMovementsReservation(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);
                    break;
                case 19:
                    response = UpdateValetParking(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);              
                    break;
                case 20:
                    response = UpdateParkingValidation(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);              
                    break;
                case 21:
                    response = UpdatePlatesCaptured(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);
                    break;
                case 22:
                    response = UpdateWatchdogAlarmsIOT(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name,ipaddress);
                    break;
                case 23:
                    response = UpdateAccessRequestIOT(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name,ipaddress);
                }
            array.append(response);
            }
        result["response"] = array;
        }
        
    UpdateFacilityDateTime(facility_number, 1);
    WriteToLog("PostDataToServer", "Final Response:" + fw.write(result));
    return fw.write(result);

    }

string DownloadCloudData(Json::Value json,string facility_number,string cloud_operator_id) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value accessdata, validationdata, reservationdata, platesewalletdata, parkingewalletdata, cooperateuserdata, array, result;
    Json::Value accessrow, validationrow, reservationrow, platesewalletrow, parkingewalletrow, cooperateuserrow;    
    string table;

    string response = "", query = "";
            
    string carparkstring;
    string carparklist;
    int carparkno;
    string facility_id;
    try {
        conn = general.DBConnectionRead(DBServer);
        stmt = conn->createStatement();

        query = "Select facility_id from system_facility where facility_number=" + facility_number;
        res = stmt->executeQuery(query);
        if(res->next())
            facility_id=res->getString("facility_id");
        delete res;
        delete conn;
                
        int rowlimit = 0;
        for (int i = 0; i < (signed)json["table"].size(); i++) {
            rowlimit = json["table"][i]["rowlimit"].asInt();
            table=json["table"][i]["table_name"].asString();
            if (json["table"][i]["table_name"] == "access_whitelist") 
                {                
                conn = general.DBConnectionRead(DBServer);
                stmt = conn->createStatement();

                query = "Select * from access_whitelist where facility_numbers='" +facility_number + "' and operator_id=" + cloud_operator_id + " and download_flag=0 limit " + to_string(rowlimit); //+ query;
                res = stmt->executeQuery(query);
                if (res->rowsCount() > 0) 
                    {
                    sql::ResultSet *carpark;
                    carpark=stmt->executeQuery("select carpark_id,carpark_name,carpark_number from system_carparks");
                    
                    Php::Value cloud_carpark_number;
                    string carpark_number,carpark_id;
                    while(carpark->next())
                        {
                        carpark_number=carpark->getString("carpark_number");                        
                        carpark_id=carpark->getString("carpark_id");
                        cloud_carpark_number[stoi(carpark_id)]=carpark_number;                                                        
                        }
                    delete carpark;
                      
                    
                    while (res->next()) 
                        {
                        accessrow["id"] = res->getInt("id");
                        accessrow["access_id"] = res->getInt("access_id");
                        accessrow["plate_number"] = res->getString("plate_number").asStdString();
                        accessrow["wallet_user_id"] = res->getInt("wallet_user_id");
                        accessrow["ticket_id"] = res->getString("ticket_id").asStdString();
                        accessrow["operator_id"] = res->getInt("operator_id");
                        accessrow["start_date"] = res->getString("start_date").asStdString();
                        accessrow["expiry_date"] = res->getString("expiry_date").asStdString();
                        accessrow["wallet_payment"] = res->getInt("wallet_payment");
                        accessrow["season_card"] = res->getInt("season_card");
                        accessrow["corporate_parker"] = res->getInt("corporate_parker");
                        accessrow["antipassback"] = res->getInt("antipassback");
                        accessrow["access_zones"] = res->getString("access_zones").asStdString();
                        accessrow["creation_date"] = res->getString("creation_date").asStdString();
                        accessrow["status"] = res->getInt("status");
                        accessrow["last_visited"] = res->getString("last_visited").asStdString();
                        accessrow["company_name"] = res->getString("company_name").asStdString();
                        accessrow["customer_name"] = res->getString("customer_name").asStdString();
                        accessrow["tag"] = res->getString("tag").asStdString();
                        accessrow["description"] = res->getString("description").asStdString();
                        carparkstring = res->getString("carpark_number");
                        carparklist = "";
                        auto list = split(carparkstring, ',');
                        bool first = true;
                        for (auto carparkid : list) {
                            carparkno = cloud_carpark_number[stoi(carparkid)];                           
                            if (carparkno > 0) {
                                if (!first && carparkno > 0) {
                                    carparklist = carparklist + ",";
                                }
                                carparklist = carparklist + to_string(carparkno);
                                first = false;
                            }

                        }
                        if (carparklist != "") 
                            {
                            accessrow["carpark_number"] = carparklist;
                            accessdata.append(accessrow);
                            }
                        }
                    result["access_whitelist"] = accessdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "access_whitelist Response:" + response);
                    } 
                else 
                    {
                    result["access_whitelist"] = accessdata;
                    WriteToLog("DownloadCloudData", "access_whitelist No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                } 
            else if (json["table"][i]["table_name"] == "parking_validation") 
                {
                conn = general.DBConnectionRead(DBReporting);
                stmt = conn->createStatement();
                res = stmt->executeQuery("Select * from parking_validation where facility_number=" + facility_number + " and download_flag=0 limit " + to_string(rowlimit));
                if (res->rowsCount() > 0) 
                    {
                    while (res->next()) 
                        {
                        validationrow["id"] = res->getInt("id");
                        validationrow["date_time"] = res->getString("date_time").asStdString();
                        validationrow["ticket_id"] = res->getString("ticket_id").asStdString();
                        validationrow["chip_utid"] = res->getString("chip_utid").asStdString();
                        validationrow["plate_number"] = res->getString("plate_number").asStdString();
                        validationrow["chip_serial_number"] = res->getString("chip_serial_number").asStdString();
                        validationrow["validator_id"] = res->getInt("validator_id");
                        validationrow["validator_name"] = res->getString("validator_name").asStdString();
                        validationrow["product_id"] = res->getInt("product_id");
                        validationrow["product_name"] = res->getString("product_name").asStdString();
                        validationrow["validation_type"] = res->getString("validation_type").asStdString();
                        validationrow["validation_value"] = res->getInt("validation_value");
                        validationrow["carpark_number"] = res->getInt("carpark_number");  
                        validationrow["carpark_name"] = res->getString("carpark_name").asStdString();
                        validationdata.append(validationrow);
                        }
                    result["validation"] = validationdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "Validation Response:" + response);
                    } 
                else 
                    {
                    result["validation"] = validationdata;
                    WriteToLog("DownloadCloudData", "Validation:No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                } 
            else if (json["table"][i]["table_name"] == "parking_reservation") 
                {
                conn = general.DBConnectionRead(DBReporting);
                stmt = conn->createStatement();
                res = stmt->executeQuery("Select * from parking_reservation where facility_number=" +facility_number + " and download_flag=0 limit " + to_string(rowlimit));
                if (res->rowsCount() > 0) 
                    {
                    while (res->next()) 
                        {
                        reservationrow["reservation_id"] = res->getInt("reservation_id");
                        reservationrow["mobile_app_user_id"] = res->getInt("mobile_app_user_id");
                        reservationrow["operator_id"] = res->getInt("operator_id");
                        reservationrow["transaction_id"] = res->getString("transaction_id").asStdString();
                        reservationrow["plate_number"] = res->getString("plate_number").asStdString();
                        reservationrow["ticket_id"] = res->getString("ticket_id").asStdString();
                        reservationrow["name"] = res->getString("name").asStdString();
                        reservationrow["address"] = res->getString("address").asStdString();
                        reservationrow["country"] = res->getString("country").asStdString();
                        reservationrow["phone"] = res->getString("phone").asStdString();
                        reservationrow["email"] = res->getString("email").asStdString();
                        reservationrow["carpark_number"] = res->getInt("carpark_number");
                        reservationrow["parking_bay_number"] = res->getString("parking_bay_number").asStdString();
                        reservationrow["from_time"] = res->getString("from_time").asStdString();
                        reservationrow["to_time"] = res->getString("to_time").asStdString();
                        reservationrow["creation_date_time"] = res->getString("creation_date_time").asStdString();
                        reservationrow["reserved_parking_duration"] = res->getString("reserved_parking_duration").asStdString();
                        reservationrow["reserved_parking_fee"] = res->getString("reserved_parking_fee").asStdString();
                        reservationrow["payment_reference_id"] = res->getString("payment_reference_id").asStdString();
                        reservationrow["payment_flag"] = res->getInt("payment_flag");
                        reservationrow["cancel_flag"] = res->getInt("cancel_flag");
                        reservationrow["used_flag"] = res->getInt("used_flag");
                        reservationdata.append(reservationrow);
                        }
                    result["reservation"] = reservationdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "Reservation Response:" + response);
                    } 
                else 
                    {
                    result["reservation"] = reservationdata;
                    WriteToLog("DownloadCloudData", "Reservation:No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                } 
            else if (json["table"][i]["table_name"] == "plates_ewallet") 
                {
                string carpark_id_string = "", operator_id_string = "";
                conn = general.DBConnectionRead(DBServer);
                stmt = conn->createStatement();
                res = stmt->executeQuery("Select * from plates_ewallet where operator_id = " + cloud_operator_id + " and  !find_in_set("+facility_id+",download_flag) limit " + to_string(rowlimit));
                if (res->rowsCount() > 0) 
                    {
                    while (res->next()) 
                        {
                        platesewalletrow["id"] = res->getInt("id");
                        platesewalletrow["user_id"] = res->getInt("user_id");
                        platesewalletrow["operator_id"] = res->getInt("operator_id");
                        platesewalletrow["country"] = res->getString("country").asStdString();
                        platesewalletrow["plate_number"] = res->getString("plate_number").asStdString();
                        platesewalletrow["ticket_id"] = res->getString("ticket_id").asStdString();
                        platesewalletrow["status"] = res->getInt("status");
                        platesewalletrow["create_datetime"] = res->getString("create_datetime").asStdString();
                        platesewalletdata.append(platesewalletrow);
                        }
                    result["plates_ewallet"] = platesewalletdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "Plates Wallet Response:" + response);
                    } 
                else 
                    {
                    result["plates_ewallet"] = platesewalletdata;
                    WriteToLog("DownloadCloudData", "Plates Wallet:No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                } 
            else if (json["table"][i]["table_name"] == "parking_ewallet") 
                {
                conn = general.DBConnectionRead(DBServer);
                stmt = conn->createStatement();
                res = stmt->executeQuery("Select a.* from parking_ewallet as a inner join application_users as b on a.user_id = b.user_id where b.parent_user_id = " + cloud_operator_id + " and a.download_flag=0 limit " + to_string(rowlimit));
                if (res->rowsCount() > 0) 
                    {
                    while (res->next()) 
                        {
                        parkingewalletrow["id"] = res->getInt("id");
                        parkingewalletrow["user_id"] = res->getInt("user_id");
                        parkingewalletrow["user_name"] = res->getString("user_name").asStdString();
                        parkingewalletrow["plate_number"] = res->getString("plate_number").asStdString();
                        parkingewalletrow["last_top_up_amount"] = res->getString("last_top_up_amount").asStdString();
                        parkingewalletrow["last_deducted_amount"] = res->getString("last_deducted_amount").asStdString();
                        parkingewalletrow["wallet_balance"] = res->getString("wallet_balance").asStdString();
                        parkingewalletrow["last_payment_date_time"] = res->getString("last_payment_date_time").asStdString();
                        parkingewalletrow["last_top_up_datetime"] = res->getString("last_top_up_datetime").asStdString();
                        parkingewalletrow["expiry_date"] = res->getString("expiry_date").asStdString();
                        parkingewalletrow["wallet_enabled"] = res->getInt("wallet_enabled");

                        parkingewalletdata.append(parkingewalletrow);
                        }
                    result["parking_ewallet"] = parkingewalletdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "Parking Wallet Response:" + response);
                    } 
                else 
                    {
                    result["parking_ewallet"] = parkingewalletdata;
                    WriteToLog("DownloadCloudData", "parking_ewallet:No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                } 
            else if (json["table"][i]["table_name"] == "cooperate_users") 
                {
                conn = general.DBConnectionRead(DBServer);
                stmt = conn->createStatement();
                //res = stmt->executeQuery("Select * from application_users  where parent_user_id = " + cloud_operator_id + " and download_flag=0 limit " + to_string(rowlimit));
                res = stmt->executeQuery("Select * from application_users  where parent_user_id = " + cloud_operator_id + "  and  !find_in_set("+facility_id+",download_flag) limit " + to_string(rowlimit));
                if (res->rowsCount() > 0) 
                    {
                    while (res->next()) 
                        {
                        cooperateuserrow["user_id"] = res->getInt("user_id");
                        cooperateuserrow["user_name"] = res->getString("email").asStdString();
                        cooperateuserrow["date_validity"] = res->getInt("date_validity");
                        cooperateuserrow["start_date"] = res->getString("start_date").asStdString();
                        cooperateuserrow["expiry_date"] = res->getString("expiry_date").asStdString();
                        cooperateuserrow["parking_spaces"] = res->getInt("corporate_parking_spaces");
                        cooperateuserrow["status"] = res->getInt("status");
                        cooperateuserrow["service_corporate_parker"] = res->getInt("service_corporate_parker");
                        cooperateuserdata.append(cooperateuserrow);
                        }
                    result["cooperate_users"] = cooperateuserdata;
                    response = fw.write(result);
                    WriteToLog("DownloadCloudData", "cooperate_users Response:" + response);
                    } 
                else 
                    {
                    result["cooperate_users"] = cooperateuserdata;
                    WriteToLog("DownloadCloudData", "cooperateuserdata: No Records Found");
                    }
                delete res;
                delete stmt;
                delete conn;
                }
            response = fw.write(result);
            }

        } 
    catch (const std::exception& e) 
        {
        WriteToLog("DownloadCloudData", e.what());
        WriteException("DownloadCloudData", e.what());
        result["message"] = "failed";
        result["error"] = e.what();
        result["error_table"] = table;
        response = fw.write(result);
        delete conn;
        }
    return response;
    }

void UpdateDownloadStatusflag(Json::Value json,string facility_number) {   
   
    int i;
    string accessidstring, validationidstring, reservationidstring, plateswalletstring, parkingwalletstring, cooperateuserstring;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string facility_id;
    try {
        conn = general.DBConnectionWrite(DBServer);
        stmt = conn->createStatement();   
        
        res = stmt->executeQuery("Select facility_id from system_facility where facility_number=" + facility_number);
        if(res->next())
            facility_id=res->getString("facility_id");
        delete res;
        
        for (i = 0; i < (signed)json["accessid"].size(); i++) {
            accessidstring = accessidstring + json["accessid"][i].asString();
            if (i < (signed)(json["accessid"].size() - 1)) {
                accessidstring = accessidstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "Access ids:" + accessidstring);
        if (accessidstring > "")
            stmt->executeUpdate("Update access_whitelist set download_flag = 1 where id in (" + accessidstring + ")");

        delete stmt;
        delete conn;

        conn = general.DBConnectionWrite(DBReporting);
        stmt = conn->createStatement();
        for (i = 0; i < (signed)json["validationid"].size(); i++) {
            validationidstring = validationidstring + json["validationid"][i].asString();
            if (i < (signed)(json["validationid"].size() - 1)) {
                validationidstring = validationidstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "Validation ids:" + validationidstring);
        if (validationidstring > "")
            stmt->executeUpdate("Update parking_validation set download_flag = 1 where id in (" + validationidstring + ")");

        //Update parking_reservation
        for (i = 0; i < (signed)json["reservationid"].size(); i++) {
            reservationidstring = reservationidstring + json["reservationid"][i].asString();
            if (i < (signed)(json["reservationid"].size() - 1)) {
                reservationidstring = reservationidstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "Reservation ids:" + reservationidstring);
        if (reservationidstring > "")
            stmt->executeUpdate("Update parking_reservation set download_flag = 1 where reservation_id in (" + reservationidstring + ")");


        delete stmt;
        delete conn;


        //Update plates_ewallet
        conn = general.DBConnectionWrite(DBServer);
        stmt = conn->createStatement();
        for (i = 0; i < (signed)json["platesewalletid"].size(); i++) {
            plateswalletstring = plateswalletstring + json["platesewalletid"][i].asString();
            if (i < (signed)(json["platesewalletid"].size() - 1)) {
                plateswalletstring = plateswalletstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "Platewallet ids:" + plateswalletstring);
        if (plateswalletstring > "")
            stmt->executeUpdate("Update plates_ewallet set download_flag =concat(download_flag,',"+facility_id+"') where id in (" + plateswalletstring + ")");

        //update cooperate user
        for (i = 0; i < (signed)json["cooperateuserid"].size(); i++) {
            cooperateuserstring = cooperateuserstring + json["cooperateuserid"][i].asString();
            if (i < (signed)(json["cooperateuserid"].size() - 1)) {
                cooperateuserstring = cooperateuserstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "cooperateuserid ids:" + cooperateuserstring);
        if (cooperateuserstring > "")
            stmt->executeUpdate("Update application_users set download_flag =concat(download_flag,',"+facility_id+"') where user_id in (" + cooperateuserstring + ")");


        delete stmt;
        delete conn;

        conn = general.DBConnectionWrite(DBServer);
        stmt = conn->createStatement();
        for (i = 0; i < (signed)json["parkingwalletid"].size(); i++) {
            parkingwalletstring = parkingwalletstring + json["parkingwalletid"][i].asString();
            if (i < (signed)(json["parkingwalletid"].size() - 1)) {
                parkingwalletstring = parkingwalletstring + ",";
            }
        }
        WriteToLog("UpdateDownloadStatusflag", "parking ewallet ids:" + parkingwalletstring);
        if (parkingwalletstring > "")
            stmt->executeUpdate("Update parking_ewallet set download_flag =concat(download_flag,',"+facility_id+"') where id in (" + parkingwalletstring + ")");

        delete stmt;
        delete conn;
    } catch (const std::exception& e) {
        WriteToLog("UpdateDownloadStatusflag", e.what());
        WriteException("UpdateDownloadStatusflag", e.what());
        delete conn;
    }
}

Php::Value DownloadDataFromServer(Php::Parameters &params) {    
    Json::Reader reader;
    Json::Value json;
    string jsondata = params[0];
    WriteToLog("DownloadDataFromServer", "Json:" + jsondata);
    reader.parse(jsondata, json, true);
    int task = json["task"].asInt();
    string response = "";
    
    string facility_number = json["facility_number"].asString();
    string cloud_operator_id = json["cloud_operator_id"].asString();
    
    if(facility_number=="")
        {
        facility_number=json["transaction_id"].asString();
        facility_number= facility_number.substr(0,6);        
        }
    
    WriteToLog("DownloadDataFromServer", "Facility number: " + facility_number);
    WriteToLog("DownloadDataFromServer", "Operator id: " + cloud_operator_id); 
    
    switch (task) {
        case 1:
            response = DownloadCloudData(json,facility_number,cloud_operator_id);
            WriteToLog("DownloadCloudData", "Response:" + response);           
            UpdateFacilityDateTime(facility_number, 2);
            break;
        case 2:
            UpdateDownloadStatusflag(json,facility_number);
            break;
    }

    return response;

}

extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PXCloud_Daemon", "1.0");
        extension.add<PostDataToServer>("PostDataToServer");
        extension.add<DownloadDataFromServer>("DownloadDataFromServer");
        return extension;
    }
}
