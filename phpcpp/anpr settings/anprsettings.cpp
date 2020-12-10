/* Standard C++ includes */
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

using namespace std;
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
	Php::out<<currentDateTime()<<" from:"<<function<<" "<<txt<<endl;
	ofstream fp;
	fp.open("/opt/parcx/Logs/WebApplication/ExceptionLogs/PX-AnprSettings-" + currentDate() + ".log", ios::app);
	fp<<currentDateTime()<<" from:"<<function<<" "<<txt<<endl;
	fp.close();
	}//end of .writeException
	
string ToString(Php::Value param)
	{
	string value=param;
	return value;
	}	

//mysql connection
sql::Connection* mysqlConnect()
	{
	sql::Connection *con=NULL;
	try
		{
		sql::Driver *driver;				
		driver = get_driver_instance();				
		con = driver->connect(SERVER, USER,PASSWORD);
		con->setSchema(DATABASE);		
		}
	catch(exception &e)
		{				
		writeException("mysqlconnect",e.what());			
		}	
	return con;
	}//end of .mysql_Connect
		
void anprSettingsTable()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnect();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select * from access_anpr_checks order by anpr_check_id desc");
			res = prep_stmt->executeQuery();
			if(res->rowsCount()>0)
				{
				Php::out <<"<div class='card-header'>" << std::endl;
				Php::out <<"<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out <<"<div class='col'>Carpark name</div>" << std::endl;			 
				Php::out <<"<div class='col'>Carpark number</div>" << std::endl;			 
				Php::out <<"<div class='col-3'></div>" << std::endl;
				Php::out <<"</div>" << std::endl;
				Php::out <<"</div>" << std::endl;
				}
			while (res->next())
				{				
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("carpark_name") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("carpark_number") << "</div>" << std::endl;
				Php::out << "<div class='col-3'>";
				Php::out << "<input class='btn btn-info btn-edit-anpr-settings btn-block' type='submit'  value='Edit' id='" << res->getString("anpr_check_id") << "'>";
				Php::out << "</div>" << std::endl;
				Php::out << "</div>" << std::endl;
				Php::out << "</div>" << std::endl;
				Php::out << "</div>" << std::endl;
				}

			con->close();
			res->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
			delete res;
		}
		catch (exception &e)
			{			
			writeException("PX_anpr_settings", e.what());			
			}
	}

string updateANPRSettings(Php::Value json)
	{		
	string msg = "Update Failed";	
	string sql;
	try
		{
		sql::Connection *con;
		con = mysqlConnect();
		sql::Statement *stmt;				
		stmt = con->createStatement();
		sql="update access_anpr_checks set check_plate_type="+ToString(json["check_plate_type"])+",check_plate_country="+ToString(json["check_plate_country"])+",check_plate_area="+ToString(json["check_plate_area"])+",check_plate_prefix="+ToString(json["check_plate_prefix"])+",check_plate_number="+ToString(json["check_plate_number"])+",check_partial_plate_number="+ToString(json["check_partial_plate_number"])+",check_plate_number_begining="+ToString(json["check_plate_number_begining"])+",check_plate_number_end="+ToString(json["check_plate_number_end"])+",allow_O_Q_mismatch="+ToString(json["allow_O_Q_mismatch"])+" where anpr_check_id ="+ToString(json["id"]);		
		if (stmt->executeUpdate(sql)>0)
			msg = "Updation Successfull";

		con->close();
		stmt->close();
		delete stmt;
		delete con;
		}
	catch (exception &e)
		{			
		writeException("updateANPRSettings", e.what());			
		writeException("updateANPRSettings", sql);			
		}
	return msg;
}
	
Php::Value getANPRSettingsDetails(Php::Value json) 
	{
	Php::Value response;
	try
		{
		sql::Connection *con;
		con = mysqlConnect();
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *res;		
		prep_stmt = con->prepareStatement("select * from access_anpr_checks where anpr_check_id=?");		
		prep_stmt->setString(1, ToString(json["id"]));				
		res = prep_stmt->executeQuery();
		string sql;		
		if (res->next())
			{				
			sql::ResultSetMetaData *res_meta = res -> getMetaData();
			int columns = res_meta -> getColumnCount();   
			for (int i = 1; i <= columns; i++) 							
				response[res_meta -> getColumnLabel(i)]=string(res->getString(i));				
			}
				
		delete res;
		delete prep_stmt;
		delete con;
		}
	catch (exception &e)
		{			
		writeException("getOperatorDetails", e.what());			
		}
	return response;
	} 

Php::Value anprSettings(Php::Parameters &params) 
	{	
	try	
		{
		Php::Value json=params[0];		
		int task=json["task"];		
		string response="";
		switch(task)
			{
			case 1:anprSettingsTable();
					break;	
			case 2:response=updateANPRSettings(json);
					break;
			case 3:Php::Value res=getANPRSettingsDetails(json);
					return res;
					break;
			}
		return response;			
		}
	catch (exception &e)
		{			
		writeException("anprSettings", e.what());	
		return "";
		}	
	}

extern "C"
	{
		PHPCPP_EXPORT void *get_module()
		{
			static Php::Extension extension("PX_AnprSettings", "1.0");
			extension.add<anprSettings>("anprSettings");			
			return extension;
		}
	}

