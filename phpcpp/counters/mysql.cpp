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
	ofstream fp;
	fp.open("/var/log/parcx/WebApplication/ExceptionLogs/CounterModule"+currentDate()+".log",ios::app);
	fp<<currentDateTime()<<" from:"<<function<<" "<<txt<<endl;
	fp.close();
	}//end of .writeException

//mysql connection
sql::Connection* mysqlConnect()
	{
	sql::Connection *con=NULL;
	try
		{
		sql::Driver *driver;		
		/* Create a connection */
		driver = get_driver_instance();		
		/* Create a connection */	
		con = driver->connect(SERVER, USER,PASSWORD);		
		}
	catch(exception &e)
		{		
		//write to log file
		writeException("mysqlconnect",e.what());	
		cout<<currentDateTime()<<e.what();								
		}	
	return con;
	}//end of .mysql_Connect
	
sql::Connection* mysqlConnectServer()
	{
	sql::Connection *con=NULL;
	try
		{			
		con = mysqlConnect();
		/* Connect to the MySQL ticket database */
		con->setSchema(DATABASE);
		}
	catch(exception &e)
		{		
		//write to log file
		writeException("mysqlconnectServer",e.what());	
		cout<<currentDateTime()<<e.what();								
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
		/* Connect to the MySQL ticket database */
		con->setSchema(DATABASE_DASHBOARD);
		}
	catch(exception &e)
		{		
		//write to log file
		writeException("mysqlConnectDashboard",e.what());	
		cout<<currentDateTime()<<e.what();								
		}	
	return con;
	}//end of .mysql_Connect	


//carpark details
void  carparkCorrectionFactorEdit()
	{
	try
		{
		sql::Connection *con;
		con=mysqlConnectDashboard();
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *res;
        prep_stmt=con->prepareStatement("select * from counting_carpark");
		res=prep_stmt->executeQuery();	
		while(res->next())
			{
			string countType;
			if(res->getInt("count_type")==1)
				countType="Short term";
			if(res->getInt("count_type")==2)
				countType="Contract";
			if(res->getInt("count_type")==3)
				countType="Manual";
			if(res->getInt("count_type")==4)
				countType="Reservation";
			if(res->getInt("count_type")==5)
				countType="Total";
			Php::out<<"<div class='table-view'>"<<std::endl;
			Php::out<<"<div class='card-text'>"<<std::endl;
			Php::out<<"<div class='d-flex justify-content-between align-items-center'>"<<std::endl;		
			Php::out<<"<div class='col'>"<<res->getString("carpark_name")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<countType<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("totalcapacity")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("CurrentLevel")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("FreeSpaces")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("Entries")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("Exits")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("OccupancyPercentage")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>"<<res->getString("last_updated_datetime")<<"</div>"<<std::endl;
			Php::out<<"<div class='col' id='val"<<res->getString("id")<<"'>"<<res->getString("correction_factor")<<"</div>"<<std::endl;
			Php::out<<"<div class='col'>";
			if(res->getInt("count_type")!=3)
				Php::out<<"<input class='edit-correction-factor btn btn-info btn-block' type='button'  value='Edit' id='btn"<<res->getString("id")<<"'>";
			Php::out<<"</div>"<<std::endl;
			Php::out<<"</div>"<<std::endl;
			Php::out<<"</div>"<<std::endl;
			Php::out<<"</div>"<<std::endl;
			}

		con->close();
		res->close();
		prep_stmt->close();
		delete prep_stmt;
		delete con;
		delete res;	
		}
	catch(exception &e)
		{		
		//write to log file
		writeException("carparkTotal",e.what());	
		cout<<currentDateTime()<<e.what();								
		}	
	}


//update correction factor
Php::Value UpdateCorrectionFactor(Php::Parameters &params) //id,value
	{
	string msg="Failed";
	try
		{
			sql::Connection *con;
			con=mysqlConnectDashboard();
			sql::PreparedStatement *prep_stmt;	
			int id=params[0];
			int value=params[1];
			prep_stmt=con->prepareStatement("update counting_carpark set correction_factor=?,CurrentLevel=(Entries-Exits+correction_factor),FreeSpaces=totalcapacity-CurrentLevel,OccupancyPercentage=(CurrentLevel/totalcapacity)*100,last_updated_datetime=CURRENT_TIMESTAMP where id=?");
			prep_stmt->setInt(1,value);
			prep_stmt->setInt(2,id);
			
			if(!prep_stmt->execute())							
				msg="Edited successfully";

			con->close();		
			prep_stmt->close();				
			delete prep_stmt;
			delete con;						
		}	
	catch(exception &e)
		{		
		//write to log file
		writeException("UpdateCorrectionFactor",e.what());	
		cout<<currentDateTime()<<e.what();	
		return e.what();							
		}	
	return msg;			
	}//end of .addCounterCondition

extern "C" {    
    PHPCPP_EXPORT void *get_module() 
    {        
    static Php::Extension extension("counter", "1.0");        					
	extension.add<carparkCorrectionFactorEdit>("carparkCorrectionFactorEdit");				
	extension.add<UpdateCorrectionFactor>("UpdateCorrectionFactor");				
    return extension;
    }
}

