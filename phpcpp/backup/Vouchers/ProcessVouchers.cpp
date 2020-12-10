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
sql::Connection *server_con,*reporting_con;
sql::Statement *reporting_stmt,*server_stmt;
sql::ResultSet *reporting_res,*server_res;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define REPORTING_DATABASE "parcx_reporting"
#define SERVER_DATABASE "parcx_server"

pxheader p;

Json::Reader reader;
Json::FastWriter fastWriter;
Json::Value json,response,res;

string query,app_key,transaction_id,device_ip,device_name,app_user_id,json_response,couponID,ticketID;
int device_number,device_type,task,carparkNo;

string log_file_path = "/opt/parcx/Logs/Services/PXProcessVoucher/ApplicationLog-PX-ProcessVoucher"+ p.currentDate()+ ".log";
string error_file_path = "/opt/parcx/Logs/Services/PXProcessVoucher/ExceptionLog-PX-ProcessVoucher"+ p.currentDate()+ "_exception.log";


Json::Value ProcessVoucher_ValidateCoupon(string couponId,int carparkNo)
	{
    try
		{        
        server_con= p.mysqlConnect(SERVER_DATABASE);
        string coupon_id=couponId;
        int carpark_no=carparkNo;
		p.writeLog(log_file_path,"ProcessVoucher_ValidateCoupon","coupon id: "+coupon_id);
		p.writeLog(log_file_path,"ProcessVoucher_ValidateCoupon","carpark number: "+to_string(carpark_no));
        query="SELECT carpark_number,DiscountName,coupon_usage from revenue_coupons_whitelist where status = 1 and  coupon_id= '"+coupon_id+"'";
        server_stmt = server_con->createStatement();
        server_res=server_stmt->executeQuery(query);

        if(server_res->first())
			{
			if(server_res->getInt("coupon_usage") == 1)
				{
				response["error"]=true;
                response["message"]="Already used coupon";
				}
			else if(server_res->getInt("carpark_number") != carpark_no)
				{
				response["error"]=true;
                response["message"]="Coupon from another carpark";
				}
			else
				{
				response["error"]=false;
                response["message"]="Valid coupon";
                response["source"]="";
				}
			}
        else
			{
            response["error"]=true;
            response["message"]="Invalid coupon";
			}

		// writeLog("validateCoupon","Final Response: "+response);

        server_res->close();
        server_stmt->close();
        server_con->close();


        delete server_res;
        delete server_stmt;
        delete server_con;

      

		}
    catch(const std::exception& e)
		{
		response["error"]=true;
        response["message"]="Unable to process coupon";
        std::cerr << e.what() << '\n';
        p.writeException(error_file_path,"ProcessVoucher_ValidateCoupon",e.what());
    
		}

	return response;
	}


Json::Value ProcessVoucher_UpdateUsedCoupon(string couponID,string ticketId)
	{
    int result;
    try
		{        
        server_con= p.mysqlConnect(SERVER_DATABASE);
        string coupon_id=couponID;
        string ticket_id=ticketId;
		p.writeLog(log_file_path,"ProcessVoucher_UpdateUsedCoupon","coupon id: "+coupon_id);
		p.writeLog(log_file_path,"ProcessVoucher_UpdateUsedCoupon","ticket id: "+ticket_id);
        query="update revenue_coupons_whitelist set coupon_usage=1,ticket_id='"+ticket_id+"',coupon_usage_datetime=CURRENT_TIMESTAMP where coupon_id= '"+coupon_id+"'";
        server_stmt= server_con->createStatement();
        result=server_stmt->executeUpdate(query);
		p.writeLog(log_file_path,"ProcessVoucher_UpdateUsedCoupon","Update result : "+to_string(result));
        if(result == 1)
			{
			response["error"] = false; 
			response["message"] = "Data updated";
			}
        else
			{
            response["error"]=true;
            response["message"]="updation unsuccessfull";
			}

      

      
        server_stmt->close();
        server_con->close();


        delete server_stmt;
        delete server_con;

       

		}
    catch(const std::exception& e)
		{
		response["error"]=true;
        response["message"]="unable to update coupon";
        std::cerr << e.what() << '\n';
        p.writeException(error_file_path,"ProcessVoucher_UpdateUsedCoupon",e.what());
       
		}
		 return response;
	}

Php::Value ProcessVoucher(Php::Parameters &params)
{
	
	try{
	
	res["error"]=false;
	string json_request=params[0];
	p.writeLog(log_file_path,"ProcessVoucher","inout "+json_request);
	bool parseSuccess = reader.parse(json_request, json, false);
	
	 if (parseSuccess)
	{
    
    device_number =json["device_number"].asInt();
    device_name= json["device_name"].asString();
    device_type = json["device_type"].asInt();
	device_ip=json["device_ip"].asString();
    transaction_id=json["transaction_id"].asString();
    app_key=json["app_key"].asString();
    app_user_id=json["app_suer_id"].asString();
    task= json["task"].asInt();
    p.writeLog(log_file_path,"ProcessVoucher","device numebr "+to_string(device_number));
	p.writeLog(log_file_path,"ProcessVoucher","device type "+to_string(device_type));
	
	
	
	
	
	switch(task){
		
		//Validate Coupon
		case 1:
		p.writeLog(log_file_path,"ProcessVoucher","Validating coupons");
		couponID=json["couponId"].asString();
		carparkNo=json["carpark_number"].asInt();
		p.writeLog(log_file_path,"ProcessVoucher","Coupon ID to validate: "+couponID);
		res=ProcessVoucher_ValidateCoupon(couponID,carparkNo);
		break;
		
		//Update Used Coupon
		case 2:
		p.writeLog(log_file_path,"ProcessVoucher","Updating coupons");
		couponID=json["couponId"].asString();
		ticketID=json["ticket"].asString();
		p.writeLog(log_file_path,"ProcessVoucher","Coupon ID to updated: "+couponID);
		p.writeLog(log_file_path,"ProcessVoucher","ticket ID to updated: "+ticketID);
		res=ProcessVoucher_UpdateUsedCoupon(couponID,ticketID);	
		break;

		
	}
	
	res["transaction_id"]=transaction_id;
    res["app_key"]=app_key;
	
    
  }
  
}
catch(const std::exception& e)
    {
		res["error"]=true;
        std::cerr << e.what() << '\n';
        p.writeException(error_file_path,"ProcessVoucher",e.what());
       
    }
    
	json_response=fastWriter.write(res) ;
	p.writeLog(log_file_path,"ProcessVoucher","Final Response "+json_response);
	return json_response;



}



extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_ProcessVoucher", "1.0");
        extension.add<ProcessVoucher>("ProcessVoucher");        
         
        return extension;
		}
	}
