//g++ -std=c++11 -Wall -o PX_CloudDownload_V01_03_02_20201213 PX_CloudDownload_V01_03_02_20201213.cpp PX_GeneralOperations.cpp -ljsoncpp -lmysqlcppconn -lcurl -I/opt/lampp/include/ -L/opt/parcx/lib/ -pthread

/*
Updates:
Added parking_ewallet in download daemon
Download based on pxcloud_download_settings table
Add cloud_operator_id in system_settings table
Download plates_ewallet and parking_ewallet based on operator_id
//Updated table name to pxcloud_upload_settings - 24-09-2020
//Added Version number and pid in Log and daemon_status table
*/


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
#include<iomanip>
#include <sys/file.h>
#include <errno.h>
#include <thread>
#include "PX_GeneralOperations.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

#include<jsoncpp/json/json.h>



#define DATABASE_SERVER "parcx_server"
#define DATABASE_REPORTING "parcx_reporting"

//#define URL "http://192.168.1.207/PXCloudService/services.php"
string url;
using namespace std;

GeneralOperations general;
int carpark_no,facility_no,device_no,cloud_operator_id;
string carpark_name,facility_name,device_name;
int i;
string start_time="";
string startdate="";
string end_time = "";
string enddate="";

void WriteToLog(string function,string msg)
{
    general.writeLog("Daemons/PXCloudDownload/ApplicationLog_PX_Cloud_Download_"+general.currentDateTime("%Y-%m-%d"),function,msg);
}

void WriteException(string function,string msg)
{
    general.writeLog("Daemons/PXCloudDownload/ExceptionLog_PX_Cloud_Download_"+general.currentDateTime("%Y-%m-%d"),function,msg);
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string intToHex(int val)
{
    std::stringstream ss;
    ss << std::hex << val;
    std::string result(ss.str());
    return result;
}

void GetDayclosuretime()
{ 
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    try{
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select setting_name,setting_value from system_settings where setting_name in ('dayclosure_start_time','dayclosure_end_time')");
        while(rs->next())
        {
            if(rs->getString("setting_name")=="dayclosure_start_time")
            {
            start_time = rs->getString("setting_value");
            WriteToLog("getDayclosuretime","dayclosure_start_time :"+start_time);
            }
            else
            {
            end_time = rs->getString("setting_value");
            WriteToLog("getDayclosuretime","dayclosure_end_time :"+end_time);
            }
        
        }
        delete rs;
        delete stmt;
        delete conn;
    }
   catch (sql::SQLException &e) {
        WriteToLog("getDayclosuretime",e.what());
    }

}

int GetInterval()
{
  int seconds;
  char out[30],out1[30];
  string stime = general.currentDateTime("%Y-%m-%d")+" "+start_time;  //Start Time
  struct tm tm,tm1;
  strptime(stime.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
  time_t e = mktime(&tm);
  time_t n = time(NULL);
 ////current_time in time_t
  seconds = difftime(e,n);

  startdate = stime;
  if(seconds < 0)
  {
    
      tm.tm_mday += 1;
      time_t e = mktime(&tm);
      time_t now = time(NULL);
      seconds = difftime(e,now);
	  strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm);
	  string stime(out);
	  startdate = stime;
    
  }

  strptime(startdate.c_str(),"%Y-%m-%d %H:%M:%S",&tm1);
  tm1.tm_mday += 1;
  tm1.tm_sec  -= 1;
  mktime(&tm1);
  strftime(out1,30,"%Y-%m-%d %H:%M:%S",&tm1);
  string etime(out1);
  enddate = etime;
 
 WriteToLog("Time","start:"+startdate+" end:"+enddate);
  cout<<"Start:"<<startdate<<endl;
  cout<<"End:"<<enddate<<endl;
  cout <<"Seconds:"<<seconds<<endl;
  return seconds;
}
	
string generateTicketNumber(string facility, string carpark,string device, string ticketType)
{
    try
    {						       
        switch(facility.length()){
            case 1:
                facility="00000"+facility;
                break;
            case 2:
                facility="0000"+facility;
                break;
            case 3:
                facility="000"+facility;
                break;
            case 4:
                facility="00"+facility;
                break;
            case 5:
                facility="0"+facility;
                break;
                
        }
    
        string hexCarpark= intToHex(stoi(carpark));       
        if(hexCarpark.length()==1)
            hexCarpark="0"+hexCarpark;                  
        string hexDevice = intToHex(stoi(device));      
        if(hexDevice.length()==1)
            hexDevice="0"+hexDevice;
        string hexTicketType = intToHex(stoi(ticketType));        
        if(hexTicketType.length()==1)
            hexTicketType="0"+hexTicketType;               
        string currentDateTime=general.currentDateTime("%Y-%m-%d %H:%M:%S");
        string finalCurrentDateTime=currentDateTime.substr(0,currentDateTime.size()-3);
        struct tm tm;
        string startDate="2018-01-01 00:00:00";
        string	end="2030-12-00 00:00:00";       
        strptime(startDate.c_str(),"%Y-%m-%d %H:%M:%S",&tm);		
        time_t t=mktime(&tm);               
        time_t now=time(NULL);
        long seconds=difftime(now,t);   
        seconds=seconds+1415926535;			    
        std::stringstream ss;	
        int mode =0;	
        string ticketNo;		
        if(mode)//parktron
        {
            seconds=seconds%1000000;								
            ss << std::setw(6) << std::setfill('0') << seconds;
            string ticket = ss.str();
            ticketNo=device+ticket+"1";							
        }
        else	//parcx
        {
            ss << std::hex << seconds;
            std::string ticket(ss.str());									
            ticketNo=facility+hexCarpark+hexDevice+hexTicketType+ticket;	
        }									
       // WriteToLog("generateTicketNumber",ticketNo);
        return ticketNo;	
    }
    catch(exception &e)
    {
        WriteToLog("Generate Ticket number",e.what());			
        return "";
    }
}


string CallWebservice(string jsonstring)
{
    cout<<"URL:"<<url<<endl;
    string readBuffer="";
    CURL *curl;
    CURLcode res;
    try{
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST"); 
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonstring.c_str());
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);            
          //  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);            
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            if(res==CURLE_OK)
            {
               // WriteToLog("CallWebservice","Curl Successful");
               // WriteToLog("CallWebservice","Request:"+jsonstring);
                //WriteToLog("CallWebservice","Response:"+readBuffer);
            }
            else{
                WriteToLog("CallWebservice","Curl Failed");
				WriteToLog("CallWebservice","Request:"+jsonstring);
				WriteToLog("CallWebservice","Return Code:"+to_string(res));
            }
            curl_easy_cleanup(curl);
         }
    }
    catch(exception &e)
    {
        WriteToLog("CallWebservice",e.what());
    }
    return readBuffer;
}

void GetSystemSettings()
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    try{
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select  setting_name,setting_value from system_settings where setting_name in('carpark_number','carpark_name','facility_number','facility_name','device_number','device_name','cloud_download_webservice','cloud_operator_id')");
        while(res->next())
        {
            if(res->getString("setting_name")=="carpark_number")
            {
                carpark_no = res->getInt("setting_value");
            }
            if(res->getString("setting_name")=="carpark_name")
            {
                carpark_name = res->getString("setting_value");
            }
            if(res->getString("setting_name")=="facility_number")
            {
                facility_no = res->getInt("setting_value");
            }
            if(res->getString("setting_name")=="facility_name")
            {
                facility_name = res->getString("setting_value");
            }
            if(res->getString("setting_name")=="device_number")
            {
                device_no = res->getInt("setting_value");
            }
            if(res->getString("setting_name")=="device_name")
            {
                device_name = res->getString("setting_value");
            }
            if(res->getString("setting_name")=="cloud_download_webservice")
            {
                url = res->getString("setting_value");
            }
			if(res->getString("setting_name")=="cloud_operator_id")
            {
                cloud_operator_id = res->getInt("setting_value");
            }
        }
        delete res;
        delete stmt;
        delete conn;

    }
    catch(sql::SQLException &e)
    {
        WriteException("GetSettings",e.what());
    }

}

void UpdatePXCloudDownloadTime(string table_name)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    try{
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        stmt->executeUpdate("Update pxcloud_download_settings set cloud_download_date_time=CURRENT_TIMESTAMP() where table_name = '"+table_name+"'");
        delete stmt;
        delete conn;

    }
    catch(sql::SQLException &e)
    {
        WriteException("UpdatePXCloudDownloadTime",e.what());
    }
    
}


void DownloadCloudData(string response)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::Value array,accessidlist,updatejson,validationlist,reservationlist,platesewalletlist,parkingwalletlist;
    string jsonstring;
    Json::FastWriter fw;
    bool parse;
    Json::Reader reader;
    string query;
    int rowid,result,k;
    int id,access_id,wallet_user_id,operator_id,status,antipassback,corporate_parker,wallet_payment,season_card;
	string plate_number,description,carpark_number,country,start_date,expiry_date,access_zones,creation_date,last_visited,company_name,customer_name,tag,tag_serial,tag_tid;

    string date_time,ticket_id,chip_utid,chip_serial_number,validation_type,product_name,validator_name;
    int validator_id,product_id,validation_value,carpark_no;

    int mobile_app_user_id,payment_flag,cancel_flag,used_flag,reservation_carpark_number;
    string transaction_id,name,address,phone,email,payment_reference_id,reserved_parking_fee,parking_bay_number,reserved_parking_duration,creation_date_time,from_time,to_time;

	int user_id,wallet_operator_id;
    string create_datetime;

    string user_name,last_payment_date_time,last_top_up_datetime,last_top_up_amount,last_deducted_amount,wallet_balance;
    int wallet_enabled;


    try
    {
			//k=0;

        parse = reader.parse(response,array,true);
        if(parse)
        {
            conn = general.mysqlConnect(DATABASE_SERVER);
            stmt =conn->createStatement();
            //For Access Whitelist
            k=0;
            for(i = 0;i<(signed)array["access_whitelist"].size();i++)
            {
                result = 0;
                id = array["access_whitelist"][i]["id"].asInt();
                plate_number = array["access_whitelist"][i]["plate_number"].asString();
                country = array["access_whitelist"][i]["country"].asString();
                ticket_id = array["access_whitelist"][i]["ticket_id"].asString();
                access_id = array["access_whitelist"][i]["access_id"].asInt();
                wallet_user_id = array["access_whitelist"][i]["wallet_user_id"].asInt();
                operator_id = array["access_whitelist"][i]["operator_id"].asInt();
                start_date = array["access_whitelist"][i]["start_date"].asString();
                expiry_date = array["access_whitelist"][i]["expiry_date"].asString();
                wallet_payment = array["access_whitelist"][i]["wallet_payment"].asInt();
                if(wallet_payment>0)
                    antipassback=1;
                else    
                    antipassback = array["access_whitelist"][i]["antipassback"].asInt();
                season_card = array["access_whitelist"][i]["season_card"].asInt();
                corporate_parker = array["access_whitelist"][i]["corporate_parker"].asInt();                
                access_zones = array["access_whitelist"][i]["access_zones"].asString();
                creation_date = array["access_whitelist"][i]["creation_date"].asString();
                status = array["access_whitelist"][i]["status"].asInt();
                last_visited = array["access_whitelist"][i]["last_visited"].asString();
                company_name = array["access_whitelist"][i]["company_name"].asString();	
                customer_name = array["access_whitelist"][i]["customer_name"].asString();	
                tag = array["access_whitelist"][i]["tag"].asString();
                carpark_number = array["access_whitelist"][i]["carpark_number"].asString();
                tag_serial = "";
                tag_tid = "";
                description = array["access_whitelist"][i]["description"].asString();

                if(last_visited=="")
                    last_visited="NULL";
                else
                    last_visited="'"+last_visited+"'";
                
                if(start_date=="")
                    start_date="NULL";
                else
                    start_date="'"+start_date+"'";
                
                if(expiry_date=="")
                    expiry_date="NULL";
                else
                    expiry_date="'"+expiry_date+"'";

                query="Select access_whitelist_id,cloud_upload_status from access_whitelist where access_id = "+to_string(access_id)+" and facility_number = "+to_string(facility_no);			               
                rs  = stmt->executeQuery(query);
                if(rs->next())
                {						
                    rowid = rs->getInt("access_whitelist_id"); 
                    if(rs->getInt("cloud_upload_status")==1)
                    {
                        query="Update access_whitelist set carpark_number='"+carpark_number+"',plate_number = '"+plate_number+"',country = '"+country+"',ticket_id = '"+ticket_id+"',last_sync = CURRENT_TIMESTAMP(),wallet_user_id = "+to_string(wallet_user_id)+",validity_start_date  = "+start_date+",validity_expiry_date="+expiry_date+",wallet_payment = "+to_string(wallet_payment)+",season_card = "+to_string(season_card)+",corporate_parker="+to_string(corporate_parker)+",antipassback_enabled="+to_string(antipassback)+",access_zones='"+access_zones+"',creation_date_time = '"+creation_date+"',status="+to_string(status)+",last_visited_date_time="+last_visited+",company_name='"+company_name+"',customer_name='"+customer_name+"',tag='"+tag+"',tag_serial='"+tag_serial+"',tag_tid='"+tag_tid+"',description='"+description+"',cloud_upload_status=1 where access_whitelist_id = "+to_string(rowid);    
                        // WriteToLog("DownloadCloudData","Query:"+query);
                        result = stmt->executeUpdate(query);
                    }                       
                }
                else
                {
                    query="Insert into access_whitelist (carpark_number,facility_number,plate_number,access_id,country,ticket_id,wallet_user_id,validity_start_date ,validity_expiry_date,wallet_payment,season_card,corporate_parker,antipassback_enabled,access_zones,creation_date_time,status,last_visited_date_time,company_name,last_sync,customer_name,tag,tag_serial,tag_tid,description,cloud_upload_status) values('"+carpark_number+"',"+to_string(facility_no)+",'"+plate_number+"',"+to_string(access_id)+",'"+country+"','"+ticket_id+"',"+to_string(wallet_user_id)+","+start_date+","+expiry_date+","+to_string(wallet_payment)+","+to_string(season_card)+","+to_string(corporate_parker)+","+to_string(antipassback)+",'"+access_zones+"','"+creation_date+"',"+to_string(status)+","+last_visited+",'"+company_name+"',CURRENT_TIMESTAMP(),'"+customer_name+"','"+tag+"','"+tag_serial+"','"+tag_tid+"','"+description+"',1)";                        
                    // WriteToLog("DownloadCloudData","Query:"+query);
                    result = stmt->executeUpdate(query);
                }
                delete rs;   
                if(result==1)
                {
                    accessidlist[k] = id;
                    k++;
                }
            }
            delete stmt;
            delete conn;

            conn = general.mysqlConnect(DATABASE_REPORTING);
            stmt =conn->createStatement();
            k=0;
            for(i = 0;i<(signed)array["validation"].size();i++)
            {
                result = 0;
                id = array["validation"][i]["id"].asInt();
                date_time = array["validation"][i]["date_time"].asString();
                ticket_id = array["validation"][i]["ticket_id"].asString();
                chip_utid = array["validation"][i]["chip_utid"].asString();
                plate_number = array["validation"][i]["plate_number"].asString();
                chip_serial_number = array["validation"][i]["chip_serial_number"].asString();
                validator_id = array["validation"][i]["validator_id"].asInt();
                validator_name = array["validation"][i]["validator_name"].asString();
                product_id = array["validation"][i]["product_id"].asInt();
                product_name = array["validation"][i]["product_name"].asString();
                validation_type = array["validation"][i]["validation_type"].asString();
                validation_value = array["validation"][i]["validation_value"].asInt();
                carpark_no = array["validation"][i]["carpark_number"].asInt();
                query="INSERT INTO parking_validation (date_time, ticket_id, chip_utid, plate_number, chip_serial_number, validator_id, validator_name, product_id, product_name, validation_type, validation_value, carpark_number) VALUES('"+date_time+"', '"+ticket_id+"', '"+chip_utid+"', '"+plate_number+"', '"+chip_serial_number+"', "+to_string(validator_id)+", '"+validator_name+"', "+to_string(product_id)+", '"+product_name+"', '"+validation_type+"', "+to_string(validation_value)+", "+to_string(carpark_no)+")";
 
                // WriteToLog("DownloadCloudData","Query:"+query);
                result = stmt->executeUpdate(query);
   
                if(result==1)
                {
                    validationlist[k] = id;
                    k++;
                }
            }


            k=0;
            for(i = 0;i<(signed)array["reservation"].size();i++)
            {
                result = 0;
                id = array["reservation"][i]["reservation_id"].asInt();
                mobile_app_user_id  = array["reservation"][i]["mobile_app_user_id"].asInt();
				operator_id = array["reservation"][i]["operator_id"].asInt();
				transaction_id = array["reservation"][i]["transaction_id"].asString();
				plate_number = array["reservation"][i]["plate_number"].asString();
				ticket_id = array["reservation"][i]["ticket_id"].asString();
				name = array["reservation"][i]["name"].asString();
				address = array["reservation"][i]["address"].asString();
				country = array["reservation"][i]["country"].asString();
				phone = array["reservation"][i]["phone"].asString();
				email = array["reservation"][i]["email"].asString();
				reservation_carpark_number = array["reservation"][i]["carpark_number"].asInt();
				parking_bay_number = array["reservation"][i]["parking_bay_number"].asString();
				from_time = array["reservation"][i]["from_time"].asString();
				to_time = array["reservation"][i]["to_time"].asString();
				creation_date_time = array["reservation"][i]["creation_date_time"].asString();
				reserved_parking_duration = array["reservation"][i]["reserved_parking_duration"].asString();
				reserved_parking_fee = array["reservation"][i]["reserved_parking_fee"].asString();
				payment_reference_id = array["reservation"][i]["payment_reference_id"].asString();
				payment_flag = array["reservation"][i]["payment_flag"].asInt();
				cancel_flag = array["reservation"][i]["cancel_flag"].asInt();
				used_flag = array["reservation"][i]["used_flag"].asInt();
                // WriteToLog("DownloadCloudData","Query:"+query);
				query="Select reservation_id,cloud_upload_status from parking_reservation where mobile_app_user_id = "+to_string(mobile_app_user_id)+" and transaction_id = '"+transaction_id+"'";			
                 //WriteToLog("DownloadCloudData","Query:"+query);
                rs  = stmt->executeQuery(query);
                if(rs->next())
                {						
                    rowid = rs->getInt("reservation_id"); 
                    if(rs->getInt("cloud_upload_status")==1)
                    {
                        query="Update parking_reservation set plate_number = '"+plate_number+"',ticket_id = '"+ticket_id+"',name = '"+name+"',address = '"+address+"',country  = '"+country+"',phone='"+phone+"',email = '"+email+"',carpark_number = "+to_string(reservation_carpark_number)+",parking_bay_number='"+parking_bay_number+"',from_time='"+from_time+"',to_time = '"+to_time+"',reserved_parking_duration='"+reserved_parking_duration+"',reserved_parking_fee='"+reserved_parking_fee+"',payment_reference_id='"+payment_reference_id+"',payment_flag="+to_string(payment_flag)+",cancel_flag="+to_string(cancel_flag)+" where reservation_id = "+to_string(rowid);      
                       // WriteToLog("DownloadCloudData","Query:"+query);
                        result = stmt->executeUpdate(query);
                    }                       
                }
                else
                {
					query = "INSERT INTO parking_reservation (mobile_app_user_id, operator_id, transaction_id, plate_number, ticket_id, name, address, country, phone, email, carpark_number, facility_number, parking_bay_number, from_time, to_time, creation_date_time, reserved_parking_duration, reserved_parking_fee, payment_reference_id, payment_flag, cancel_flag, used_flag,cloud_upload_status) VALUES("+to_string(mobile_app_user_id)+", "+to_string(operator_id)+", '"+transaction_id+"', '"+plate_number+"', '"+ticket_id+"', '"+name+"', '"+address+"', '"+country+"', '"+phone+"', '"+email+"', "+to_string(reservation_carpark_number)+", "+to_string(facility_no)+", '"+parking_bay_number+"', '"+from_time+"', '"+to_time+"', '"+creation_date_time+"', '"+reserved_parking_duration+"', '"+reserved_parking_fee+"', '"+payment_reference_id+"', "+to_string(payment_flag)+", "+to_string(cancel_flag)+", "+to_string(used_flag)+",1)";
					// WriteToLog("DownloadCloudData","Query:"+query);
					result = stmt->executeUpdate(query);
				}
   
                if(result==1)
                {
                    reservationlist[k] = id;
                    k++;
                }
            }
			delete stmt;
            delete conn;


			conn = general.mysqlConnect(DATABASE_SERVER);
            stmt =conn->createStatement();

            k=0;
            for(i = 0;i<(signed)array["plates_ewallet"].size();i++)
            {
                result = 0;
                id = array["plates_ewallet"][i]["id"].asInt();
                user_id  = array["plates_ewallet"][i]["user_id"].asInt();
				wallet_operator_id = array["plates_ewallet"][i]["operator_id"].asInt();
				country = array["plates_ewallet"][i]["country"].asString();
				plate_number = array["plates_ewallet"][i]["plate_number"].asString();
				ticket_id = array["plates_ewallet"][i]["ticket_id"].asString();
				status = array["plates_ewallet"][i]["status"].asInt();
				create_datetime = array["plates_ewallet"][i]["create_datetime"].asString();
                // WriteToLog("DownloadCloudData","Query:"+query);
                query = "INSERT INTO plates_ewallet (user_id, operator_id, country, plate_number, ticket_id, status, create_datetime) VALUES("+to_string(user_id)+", "+to_string(wallet_operator_id)+", '"+country+"', '"+plate_number+"', '"+ticket_id+"', "+to_string(status)+", '"+create_datetime+"')";
                result = stmt->executeUpdate(query);
   
                if(result==1)
                {
                    platesewalletlist[k] = id;
                    k++;
                }
            }
            delete stmt;
            delete conn;

            conn = general.mysqlConnect(DATABASE_SERVER);
            stmt =conn->createStatement();

            k=0;
            for(i = 0;i<(signed)array["parking_ewallet"].size();i++)
            {
                result = 0;
                id = array["parking_ewallet"][i]["id"].asInt();
                user_id  = array["parking_ewallet"][i]["user_id"].asInt();
				user_name = array["parking_ewallet"][i]["user_name"].asString();
				plate_number = array["parking_ewallet"][i]["plate_number"].asString();
				last_top_up_amount = array["parking_ewallet"][i]["last_top_up_amount"].asString();
				last_deducted_amount = array["parking_ewallet"][i]["last_deducted_amount"].asString();
				wallet_balance = array["parking_ewallet"][i]["wallet_balance"].asString();
                last_payment_date_time = array["parking_ewallet"][i]["last_payment_date_time"].asString();
                last_top_up_datetime = array["parking_ewallet"][i]["last_top_up_datetime"].asString();
                expiry_date  = array["parking_ewallet"][i]["expiry_date"].asString();
                wallet_enabled = array["parking_ewallet"][i]["wallet_enabled"].asInt();
                
                // WriteToLog("DownloadCloudData","Query:"+query);
                rs = stmt->executeQuery("Select id from parking_ewallet where user_id = "+to_string(user_id));
                if(rs->next())
                {
                    query = "Update parking_ewallet set last_top_up_amount="+last_top_up_amount+", last_deducted_amount="+last_deducted_amount+", wallet_balance="+wallet_balance+",last_payment_date_time='"+last_payment_date_time+"',last_top_up_datetime='"+last_top_up_datetime+"',expiry_date='"+expiry_date+"',wallet_enabled="+to_string(wallet_enabled)+" where user_id = "+to_string(user_id);
                }
                else
                {
                    query = "INSERT INTO parking_ewallet (user_id, user_name, plate_number, last_top_up_amount, last_deducted_amount, wallet_balance,last_payment_date_time,last_top_up_datetime,expiry_date,wallet_enabled) VALUES("+to_string(user_id)+", '"+user_name+"', '"+plate_number+"',"+last_top_up_amount+", "+last_deducted_amount+", "+wallet_balance+",'"+last_payment_date_time+"','"+last_top_up_datetime+"','"+expiry_date+"',"+to_string(wallet_enabled)+")";
                }
				delete rs;
                result = stmt->executeUpdate(query);
   
                if(result==1)
                {
                    parkingwalletlist[k] = id;
                    k++;
                }
            }
            delete stmt;
            delete conn;



            if(accessidlist.size()>0)
            {
                UpdatePXCloudDownloadTime("access_whitelist");
            }
            if(validationlist.size()>0)
            {
                UpdatePXCloudDownloadTime("parking_validation");
            }
            if(reservationlist.size()>0)
            {
                UpdatePXCloudDownloadTime("parking_reservation");
            }
            if(platesewalletlist.size()>0)
            {
                UpdatePXCloudDownloadTime("plates_ewallet");
            }
            if(parkingwalletlist.size()>0)
            {
                UpdatePXCloudDownloadTime("parking_ewallet");
            }

//WriteToLog("DownloadCloudData","heree"+to_string(parkingwalletlist.size()));
            if(accessidlist.size()>0||validationlist.size()>0||reservationlist.size()>0||platesewalletlist.size()>0 || parkingwalletlist.size()>0)
            {
//WriteToLog("DownloadCloudData","heree2");
                updatejson["task"]=2;
                updatejson["transaction_id"] = generateTicketNumber(to_string(facility_no),to_string(carpark_no),to_string(device_no),"00");
                updatejson["accessid"]=accessidlist;
                updatejson["validationid"]=validationlist;
                updatejson["reservationid"]=reservationlist;
				updatejson["platesewalletid"]=platesewalletlist;
                updatejson["parkingwalletid"]=parkingwalletlist;
                cout<<"Ids:"<<fw.write(updatejson)<<endl;
                WriteToLog("DownloadCloudData","Update ID Request:"+fw.write(updatejson));
                CallWebservice(fw.write(updatejson));
            }
            accessidlist="";
        }
        else{
            WriteToLog("DownloadCloudData","Parse Error");
        }
    
        
        
    }
    catch(const std::exception& e)
    {
        WriteToLog("DownloadCloudData",e.what());
        WriteToLog("DownloadCloudData",query);
        WriteException("DownloadCloudData",e.what());
		delete stmt;
		delete conn;

    }

}


void CollectData(int interval)
{
    
    sql::ResultSet *res;
    sql::Connection *conn;
    sql::Statement *stmt;
    Json::FastWriter fw;
	Json::Value request;
    Json::Value json,response;
    Json::Value data(Json::arrayValue);
    Json::Value rowdata;
	string jsonstring,webservice_response;
	request["facility_number"] = facility_no;
	request["cloud_operator_id"] = cloud_operator_id;
	request["transaction_id"] = generateTicketNumber(to_string(facility_no),to_string(00),to_string(device_no),"00");
	request["task"] = 1;
	

    try
    {
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        //WriteToLog("","Select table_name,task,upload_row_limit from pxcloud_settings where day_closure = 0 and enabled=1 and time_interval = "+to_string(interval));
        res = stmt->executeQuery("Select table_name,task,download_row_limit from pxcloud_download_settings where enabled=1 and day_closure=0 and time_interval = "+to_string(interval));
        //int n = res->rowsCount();
		Json::Value tb,tbrow;
        //int i=0;
        while(res->next())
        {
        
            tb["table_name"] = res->getString("table_name").asStdString();
            //tb["task"] = res->getInt("task");
            tb["rowlimit"]=res->getInt("download_row_limit");
            tbrow.append(tb);
        }
        delete res;
        delete stmt;
        delete conn;
        //WriteToLog("","here"+to_string(n));
		request["table"] = tbrow;
        while(true)
        {
            jsonstring = fw.write(request);    
			WriteToLog("CollectData","Webservice Request:"+jsonstring);
            cout<<"req:"<<jsonstring<<endl;
			webservice_response = CallWebservice(jsonstring);
			WriteToLog("CollectData","Webservice Response:"+webservice_response);
            DownloadCloudData(webservice_response);
            sleep(interval);
        }
    }
    catch(const std::exception& e)
    {
        WriteException("CollectData",e.what());
		WriteToLog("CollectData",e.what());
		WriteToLog("CollectData","Full req:   "+fw.write(json));
    }

}

void DCDaemon()
{
	WriteToLog("DCDaemon","DCDaemon Starts");
	string webservice_response;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *rs;
    Json::FastWriter fw;
    Json::Value request,response;
    Json::Value data(Json::arrayValue);
    Json::Value rowdata;
	request["facility_number"] = facility_no;
	request["cloud_operator_id"] = cloud_operator_id;
	request["transaction_id"] = generateTicketNumber(to_string(facility_no),to_string(00),to_string(device_no),"00");
	request["task"] = 1;

    //string table_name;
    //int task,rowlimit;
    i=0;
    try{
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        rs = stmt->executeQuery("Select table_name,download_row_limit from pxcloud_download_settings where enabled=1 and day_closure = 1");
        if(rs->rowsCount()>0)
        {
            Json::Value tb,tbrow;
			//int i=0;
			while(rs->next())
			{
			
				tb["table_name"] = rs->getString("table_name").asStdString();
				//tb["task"] = rs->getInt("task");
				tb["rowlimit"]=rs->getInt("download_row_limit");
				tbrow.append(tb);
			}
			request["table"]=tbrow;
            string jsonstring = fw.write(request);
            if(request["table"].size()>0)
            {
                cout<<"Full req:"<<fw.write(request)<<endl;
                //WriteToLog("DCDaemon","Full req:   "+fw.write(request));
                webservice_response = CallWebservice(jsonstring);
				//WriteToLog("DCDaemon","Webservice Response:"+webservice_response);
				DownloadCloudData(webservice_response);
            }
            else if(request["table"].size()==0)
            {
                WriteToLog("DCDaemon","No Records Found");
            }
            
        }
        
        delete rs;
        delete stmt;
        delete conn;
        sleep(1);
    }
    catch(sql::SQLException &e)
    {
		WriteToLog("DCDaemon",e.what());
        WriteException("DCDaemon",e.what());
    }
    catch(exception &e)
    {		
		WriteToLog("DCDaemon",e.what());
        WriteException("DCDaemon",e.what());
    }

}


void DownloadDCData()
{

    int SLEEP_INTERVAL;
    GetDayclosuretime();
    WriteToLog("Main","Url:"+url);
    //SLEEP_INTERVAL = GetInterval();//Add 30 min to dayclosure time
    //WriteToLog("Main","Daemon Sleeps for "+std::to_string(SLEEP_INTERVAL)+" seconds");
    //WriteToLog("Main","StartDate:"+startdate+" EndDate:"+enddate);
    while(true)
    {
        //cout<<"URL:"+url<<endl;
       // WriteToLog("","******************************************Log******************************************");
        DCDaemon();
        SLEEP_INTERVAL = GetInterval();//Add 30 min to dayclosure time;
        WriteToLog("DownloadDCData","Daemon Sleeps for "+std::to_string(SLEEP_INTERVAL)+" seconds");
        WriteToLog("DownloadDCData","StartDate:"+startdate+" EndDate:"+enddate);
		sleep(SLEEP_INTERVAL);
		WriteToLog("DCDaemon","Daemon Sleeps for 30 minutes more");
		sleep(30*60);

        cout<<"Start:"+startdate+" End:"+enddate<<endl;
    }
    
}

void GetCloudDownloadSettings()
{
   sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int i = 0,n=0;
    string table_name;
    int interval;
    try{
        conn = general.mysqlConnect("parcx_server");
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select distinct(time_interval) from pxcloud_download_settings where enabled=1 and day_closure=0");
		n=res->rowsCount();		
        thread updatethread[n],dcthread;

        while(res->next())
        {
            interval = res->getInt("time_interval");
            updatethread[i]=thread(CollectData,interval);
            i++;
        }
        delete res;
        
        res = stmt->executeQuery("select id from pxcloud_download_settings where enabled=1 and day_closure = 1");
        if(res->next())
        {
            dcthread = thread(DownloadDCData);
			dcthread.join();
        }	
        delete res;
        delete stmt;
        delete conn;


        for(i=0;i<n;i++)
			updatethread[i].join();
        
    }
    catch(sql::SQLException &e)
    {
        WriteException("PostData",e.what());
    }
    catch(exception &e)
    {		
        WriteException("PostData",e.what());
    }
}

void UpdateProcessId(int pid)
{
	sql::Connection *conn;
    sql::Statement *stmt;
   // sql::ResultSet *res;
	try{
		conn = general.mysqlConnect("parcx_server");
		stmt = conn->createStatement();
		stmt->executeUpdate("Update daemon_status set pid = "+to_string(pid)+",start_time=CURRENT_TIMESTAMP() where daemon_label = 'Cloud Download Daemon'");
		
	}
	catch(exception &e){
		cout<<"UpdateProcessId: "<<e.what()<<endl;
		WriteException("UpdateProcessId",e.what());
	}
	delete stmt;
	delete conn;
		
}

int main(void)
{
    int pid_file = open("px_cloud_download.lock", O_CREAT | O_RDWR, 0644);
	int rc = flock(pid_file, LOCK_EX | LOCK_NB);
	if(rc) {
		if(EWOULDBLOCK == errno)
		{
			WriteToLog("Main: " , "Another Instance is running");
			cout<<"Another Instance is running"<<endl; // another instance is running
		}
	}
	else 
    {

       pid_t pid,sid;

		pid = fork();

        cout<<pid<<endl;

		if(pid > 0)
		{
			WriteToLog("Main: " , "Cloud Download Daemon Version : PX_CloudDownload_V01_03_02_20201213    Process Id:"+to_string(pid));
			cout<<"Cloud Download Daemon Version : PX_CloudDownload_V01_03_02_20201213"<<"Process Id :"<<pid<<endl;	
			
			UpdateProcessId(pid);
			exit(EXIT_SUCCESS);
		}
		else if(pid < 0)
		{
			exit(EXIT_FAILURE);
		}
		umask(0);
		openlog("daemon-named",LOG_NOWAIT|LOG_PID,LOG_USER);
		syslog(LOG_NOTICE,"Successfully started Cloud download daemon");
		WriteToLog("Main","Successfully started Cloud download daemon");
		sid = setsid();
		if(sid<0)
		{
			exit(EXIT_FAILURE);
		}
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		//const int SLEEP_INTERVAL = 30;

		////while(1)
		//{
			WriteToLog("Main","******************************************Log******************************************");       
			GetSystemSettings();
            WriteToLog("Main","Url:"+url);
			GetCloudDownloadSettings();
		   // sleep(SLEEP_INTERVAL);
	   // }
    }
    return 0;
}
