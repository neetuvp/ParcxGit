//sudo g++ -std=c++11 -Wall -o test test.cpp validation.cpp
//      ./test

#include<iostream>
#include<stdlib.h>
#include <time.h>
#include <ctime>
#include "pxheader.h"
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <mysql/mysql.h>
#include<jsoncpp/json/json.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
using namespace std;
int main()
{ 
    pxheader p;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    Json::Value array,json ;
    Json::Value row;
    Json::FastWriter fw;
    string response = "";

    json["task"]=1;
    json["device_no"]=2;
    json["device_name"]="device_name";
    json["facility_no"]=3;
    json["facility_name"]="facility_name";
    json["carpark_no"]=4;
    json["carpark_name"]="carpark_name";
    try
    {
        conn = p.mysqlConnect("parcx_dashboard");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select * from parking_occupancy where upload_status=0");
        while(res->next())
        {
            row["id"]=res->getInt("id");
            row["count_category"] = res->getInt("count_category");
            row["correction_factor"]  = res->getInt("correction_factor");
            row["totalCapacity"] = res->getInt("totalCapacity");
            row["CurrentLevel"] = res->getInt("CurrentLevel");
            row["FreeSpaces"] = res->getInt("FreeSpaces");
            row["Entries"]=res->getInt("Entries");
            row["Exits"] = res->getInt("Exits");
            row["OccupancyPercentage"] = res->getString("OccupancyPercentage").asStdString();
            array.append(row);
        }
        json["data"] = array;
        response = fw.write(json);
        cout<<"Response:"<<response<<endl;
    }
    catch(const std::exception& e)
    {
        //WriteException("GetParkingOccupancy",e.what());
        cout<<e.what()<<endl;
    }
    

}