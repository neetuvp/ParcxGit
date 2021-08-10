//JSON.h install :   sudo apt-get install libjsoncpp-dev
//make -B
//sudo make install
//service apache2 restart
/*
Added parking_ewallet in download
Add download_flag=0 in parking_ewallet
Updated downloaddata based on pxcloud_download_settings in local server

 */

#include <phpcpp.h>
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
#include "PX_GeneralOperations.h"


#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "PX_GeneralOperations.h"
#include<jsoncpp/json/json.h>

#define DBServer "pxcloud_server"

using namespace std;

sql::Connection *conn;
Json::FastWriter fw;
GeneralOperations general;

void WriteToLog(string function, string txt) 
    {
    cout<<txt<<endl;
    string path = "Services/ApplicationLogs/PX_DaemonService_Valet_" + general.currentDateTime("%Y-%m-%d");
    general.writeLog(path, function, txt);
    }

void WriteException(string function, string txt) 
    {    
    WriteToLog(function, txt); 
    string path = "Services/ExceptionLogs/PX_DaemonService_Valet_Exception_" + general.currentDateTime("%Y-%m-%d");
    general.writeLog(path, function, txt);
    }

string mysqldate(string date)
    {
     if (date == "")
        return "NULL";
    else
        return "'" + date + "'";
    }

Json::Value postValetParking(Json::Value json,string facility_number,string facility_name,Php::Value cloud_carpark_id,Php::Value cloud_carpark_name) 
    {
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value data, jsonresponse;    
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
        con = general.DBConnection(DBServer);
        stmt = con->createStatement();               
       
        data = json["data"];
        for (int i = 0; i < (signed)data.size(); i++) 
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

void UpdateFacilityDateTime(int type,string facility_number) 
    {
    sql::Connection *conn;
    sql::Statement *stmt;
    try 
        {
        conn = general.DBConnection(DBServer);
        stmt = conn->createStatement();
        if (type == 1)                         
            stmt->executeUpdate("Update system_facility set last_upload_datetime=CURRENT_TIMESTAMP() where facility_number=" + facility_number);
        else if (type == 2)                         
            stmt->executeUpdate("Update system_facility set last_download_datetime=CURRENT_TIMESTAMP() where facility_number=" + facility_number);
        
        delete stmt;
        delete conn;
        } 
    catch (sql::SQLException &e) 
        {    
        WriteException("UpdateFacilityDateTime", e.what());
        }
    }

Php::Value PostValetDataToServer(Php::Parameters &params) 
    {
    Json::Reader reader;
    Json::Value json;
    int task;
    Json::Value response, array, result;

    string jsondata = params[0];    
    reader.parse(jsondata, json, true);
    
    string facility_number = json["facility_number"].asString();    
    WriteToLog("PostValetDataToServer", "Facility number: " + facility_number);        
    
    
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;       
    string facility_name;
    Php::Value cloud_carpark_id,cloud_carpark_name;
    try 
        {
        con = general.DBConnection(DBServer);
        stmt = con->createStatement();
        string carpark_id;
                    
        res = stmt->executeQuery("Select facility_name from system_facility where facility_number=" + facility_number);
        if (res->next()) 
            {
            facility_name = res->getString("facility_name");
            WriteToLog("setFaciltyDetails", "Facility name:" + facility_name);                
            delete res;  


            res=stmt->executeQuery("select carpark_id,carpark_name,carpark_number from system_carparks where facility_number="+facility_number);
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
            }                                                                                                                                                                                                      
        delete stmt;
        delete con;
        } 
    catch (const std::exception& e) 
        {        
        WriteException("setFaciltyDetails", e.what());       
        }  
    
    for (int i = 0; i < (signed)json["table"].size(); i++) 
        {
        response = "";
        task = json["table"][i]["task"].asInt();
        switch (task) 
            {
            case 1:
                response = postValetParking(json["table"][i],facility_number,facility_name,cloud_carpark_id,cloud_carpark_name);              
                break;            
            }
        array.append(response);
        }
    result["response"] = array;

    UpdateFacilityDateTime(1,facility_number);
    WriteToLog("PostValetDataToServer", "Final Response:" + fw.write(result));
    return fw.write(result);
    }

extern "C" 
    {
    PHPCPP_EXPORT void *get_module() 
        {
        static Php::Extension extension("PX_Valet_Cloud_Daemon", "1.0");
        extension.add<PostValetDataToServer>("PostValetDataToServer");        
        return extension;
        }
    }