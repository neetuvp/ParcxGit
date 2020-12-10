/* Standard C++ includes */
//for compile
//g++ -Wall -I/usr/include/cppconn -o test3 test3.cpp -L/usr/lib -lmysqlcppconn
#include <stdlib.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include<ctime>
#include<phpcpp.h>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define SERVER "DBServer"
#define USER "parcxservice"
#define PASSWORD "1fromParcx!19514"
#define DATABASE "parcx_server"
#define DATABASE_DASHBOARD "parcx_dashboard"

using namespace std;

sql::Connection *con;	
sql::Statement *stmt;
sql::ResultSet *res;
string query;
		
//current date
string currentDateMysql()
	{
	time_t now=time(NULL);	
	std::tm* timeinfo;
	char buffer[80];
	std::time(&now);
	timeinfo=std::localtime(&now);
	std::strftime(buffer,80,"%Y-%m-%d ",timeinfo);		
	string datetime(buffer);
	return datetime;
	}  
//current date
string currentDate()
	{
	time_t now=time(NULL);	
	std::tm* timeinfo;
	char buffer[80];
	std::time(&now);
	timeinfo=std::localtime(&now);
	std::strftime(buffer,80,"%d-%m-%Y",timeinfo);		
	string datetime(buffer);
	return datetime;
	}//end of .currentDate

//current dateTime
string currentDateTime()
	{
	time_t now=time(NULL);	
	std::tm* timeinfo;
	char buffer[80];
	std::time(&now);
	timeinfo=std::localtime(&now);
	std::strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);		
	string datetime(buffer);
	return datetime;
	}//end of .currentDateTime

//write exception
void writeException(string function,string txt)
	{
	cout<<currentDateTime()<<" from:"<<function<<" "<<txt<<endl;	
	ofstream fp;
	fp.open("/opt/parcx/Logs/WebApplication/ExceptionLogs/PX-Alert-"+currentDate()+".log",ios::app);
	fp<<currentDateTime()<<" from:"<<function<<" "<<txt<<endl;
	fp<<currentDateTime()<<" from:"<<function<<" "<<query<<endl;
	fp.close();
	}//end of .writeException


//mysql connection
sql::Connection* mysqlConnect()
	{
	sql::Connection *con=NULL;
	try
		{
		sql::Driver *driver;				
		driver = get_driver_instance();							
		con = driver->connect(SERVER, USER,PASSWORD);		
		}
	catch(exception &e)
		{				
		writeException("mysqlconnect",e.what());			
		}	
	return con;
	}//end of .mysql_Connect
	
sql::Connection* mysqlConnectServer()
	{
	sql::Connection *con=NULL;
	try
		{				
		con = mysqlConnect();		
		con->setSchema(DATABASE);
		}
	catch(exception &e)
		{				
		writeException("mysqlconnectServer",e.what());			
		}	
	return con;
	}//end of .mysql_Connect	

//mysql connection
sql::Connection* mysqlConnectDashboard()
	{
	sql::Connection *con=NULL;
	try
		{			
		con = mysqlConnect();		
		con->setSchema(DATABASE_DASHBOARD);
		}
	catch(exception &e)
		{				
		writeException("mysqlConnectDashboard",e.what());			
		}	
	return con;
	}//end of .mysql_Connect

string ToString(Php::Value param)
	{
	string value=param;
	return value;
	}
	
void facilityDropdown()
	{	
	try
		{								
        query="select facility_number,facility_name from counters group by facility_number";
		res=stmt->executeQuery(query);		
		while(res->next())
			{		
			Php::out<<"<option value='"<<res->getInt(1)<<"'>"<<res->getString(2)<<"</option>"<<std::endl;
			}	
		}
	catch(exception &e)
		{				
		writeException("facilityDropdown",e.what());			
		}
	}
void carparkDropdown(Php::Value json)
	{	
	try
		{								
        query="select carpark_number,carpark_name from counters where facility_number in("+ToString(json["facility"])+") group by carpark_number";
		res=stmt->executeQuery(query);		
		while(res->next())
			{		
			Php::out<<"<option value='"<<res->getInt(1)<<"'>"<<res->getString(2)<<"</option>"<<std::endl;
			}	
		}
	catch(exception &e)
		{				
		writeException("carparkDropdown",e.what());			
		}
	}
	
void counterDropdown(Php::Value json)
	{	
	try
		{								
        query="select parking_counter_id,counter_name from counters where facility_number in("+ToString(json["facility"])+") and carpark_number in("+ToString(json["carpark"])+")";
		res=stmt->executeQuery(query);			
		while(res->next())
			{		
			Php::out<<"<option value='"<<res->getInt(1)<<"'>"<<res->getString(2)<<"</option>"<<std::endl;
			}	
		}
	catch(exception &e)
		{				
		writeException("counterDropdown",e.what());			
		}
	}
	
string insertAlert(Php::Value json)
	{
	string msg="Failed";	
	try
		{
		if(ToString(json["type"])=="condition")	
			query="insert into alerts_counter(facility_number,carpark_number,counter_id,counter_name,counter_condition,start_date_time,hour,minute,counters_in_message,email,mobile,alert_title,alert_description)Values('"+ToString(json["facility_number"])+"','"+ToString(json["carpark_number"])+"','"+ToString(json["counter_id"])+"','"+ToString(json["counter_name"])+"','"+ToString(json["counter_condition"])+"','"+ToString(json["start_date_time"])+"','"+ToString(json["hour"])+"','"+ToString(json["minute"])+"','"+ToString(json["counters_in_message"])+"','"+ToString(json["email"])+"','"+ToString(json["mobile"])+"','"+ToString(json["alert_title"])+"','"+ToString(json["alert_description"])+"')"	;
		else if(ToString(json["type"])=="time")	
			query="insert into alerts_counter(facility_number,carpark_number,counter_id,counter_name,counter_condition,start_date_time,hour,minute,counters_in_message,email,mobile,alert_title,alert_description)Values('"+ToString(json["facility_number"])+"','"+ToString(json["carpark_number"])+"',NULL,NULL,NULL,'"+ToString(json["start_date_time"])+"','"+ToString(json["hour"])+"','"+ToString(json["minute"])+"','"+ToString(json["counters_in_message"])+"','"+ToString(json["email"])+"','"+ToString(json["mobile"])+"','"+ToString(json["alert_title"])+"','"+ToString(json["alert_description"])+"')"	;
		else
			query="insert into alerts_alarms(facility_number,carpark_number,start_date_time,email,mobile,alert_title,alert_description,alarm_code)Values('"+ToString(json["facility_number"])+"','"+ToString(json["carpark_number"])+"','"+ToString(json["start_date_time"])+"','"+ToString(json["email"])+"','"+ToString(json["mobile"])+"','"+ToString(json["alert_title"])+"','"+ToString(json["alert_description"])+"','"+ToString(json["watchdog_alarm"])+"')"	;
		writeException("insertAlert",query);
		if(stmt->executeUpdate(query)>0)
			msg="Success";
		}
	catch(exception &e)
		{				
		writeException("insertAlert",e.what());	
		writeException("insertAlert",query);					
		}
	return msg;
	}

Php::Value alertSettings(Php::Parameters &params) 
	{	
	try	
		{
		Php::Value json=params[0];		
		int task=json["task"];		
		string response="";
		if(task!=4)
			con=mysqlConnectDashboard();
		else
			con=mysqlConnectServer();
		stmt=con->createStatement();
		switch(task)
			{
			case 1:facilityDropdown();
					break;	
			case 2:carparkDropdown(json);
					break;
			case 3:counterDropdown(json);
					break;
			case 4:response=insertAlert(json);					
					break;
			}
		con->close();		
		stmt->close();
		delete stmt;
		delete con;		
		return response;			
		}
	catch (exception &e)
		{			
		writeException("alertSettings", e.what());	
		return "";
		}	
	}


extern "C" {    
    PHPCPP_EXPORT void *get_module() 
    {        
    static Php::Extension extension("PX_Alert", "1.0");        
	extension.add<alertSettings>("alertSettings");		
    return extension;
    }
}

