/* Standard C++ includes */
//for compile
//g++ -Wall -I/usr/include/cppconn -o test3 test3.cpp -L/usr/lib -lmysqlcppconn
#include <stdlib.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include<ctime>
#include<phpcpp.h>

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
	ofstream fp;
	fp.open("/opt/parcx/Logs/WebApplication/ExceptionLogs/PX-Configuration-"+currentDate()+".log",ios::app);
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

	//vms
void vmsDevices()
	{
		try
			{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
				prep_stmt=con->prepareStatement("select * from system_vms_devices where device_enabled=1");
			res=prep_stmt->executeQuery();	
			while(res->next())
				{
					Php::out << "<option value='" << res->getString("vms_id") << "'>" << res->getString("vms_name") << "</option>" << std::endl;				
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
			writeException("dynamicParkingRates", e.what());
			cout<<currentDateTime()<<e.what();								
			}	
	}

	//dynamicParkingRates
void parkingRatesLabels()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select * from revenue_parking_rate_label");
			res = prep_stmt->executeQuery();
			if (res->next())
			{
				Php::out << "<option value='parking_rates1'>" << res->getString("parking_rate1") << "</option>" << std::endl;
				Php::out << "<option value='parking_rates2'>" << res->getString("parking_rate2") << "</option>" << std::endl;
				Php::out << "<option value='parking_rates3'>" << res->getString("parking_rate3") << "</option>" << std::endl;
				Php::out << "<option value='parking_rates4'>" << res->getString("parking_rate4") << "</option>" << std::endl;
				Php::out << "<option value='parking_rates5'>" << res->getString("parking_rate5") << "</option>" << std::endl;
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
			//write to log file
			writeException("dynamicParkingRates", e.what());
			cout << currentDateTime() << e.what();
		}
	}

void activeDynamicParkingRates()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select id,parking_rate_label,datetime,day,rate_type,parking_rate from revenue_dynamic_parking_rates where status=1");
			res = prep_stmt->executeQuery();
			while (res->next())
			{
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("parking_rate_label") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("datetime") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("day") << "</div>" << std::endl;
				std::string type;
				if (res->getInt("rate_type")==1)
					type="Variable Rate";
				else
					type="Fixed Rate";
				Php::out << "<div class='col'>" <<type<<"</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("parking_rate") << "</div>" << std::endl;				
				Php::out << "<div class='col'>";				
				Php::out << "<input class='btn btn-danger btn-disable-dynamic-rate btn-block' type='submit'  value='Disable' id='" << res->getString("id") << "'>";
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
			//write to log file
			writeException("activeDynamicParkingRates", e.what());
			cout << currentDateTime() << e.what();
		}
	}

void activeVmsShedule()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select id,vms_name,message,colour,action,shedule_time,shedule_interval,hold_time,start_datetime from vms_device_shedule where status=1");
			res = prep_stmt->executeQuery();
			while (res->next())
			{
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col' id='name-" + res->getString("id") +"' >" << res->getString("vms_name") << "</div>" << std::endl;
				Php::out << "<div class='col' id='colour-" + res->getString("id") + "'>" << res->getString("colour") << "</div>" << std::endl;
				Php::out << "<div class='col' id='action-" + res->getString("id") + "'>" << res->getString("action") << "</div>" << std::endl;
				Php::out << "<div class='col-2' id='message-" + res->getString("id") + "'>" << res->getString("message") << "</div>" << std::endl;
				Php::out << "<div class='col' id='time-" + res->getString("id") + "'>" << res->getString("shedule_time") << "</div>" << std::endl;
				Php::out << "<div class='col-2' id='interval-" + res->getString("id") + "'>" << res->getString("shedule_interval") << "</div>" << std::endl;
				Php::out << "<div class='col' id='hold-time-" + res->getString("id") + "'>" << res->getString("hold_time") << "</div>" << std::endl;
				Php::out << "<div class='col' id='start-datetime-" + res->getString("id") + "'>" << res->getString("start_datetime") << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-danger btn-disable-vms-shedule btn-block' type='submit'  value='Disable' id='" << res->getString("id") << "'>";
				Php::out << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-info btn-edit-vms-shedule btn-block' type='submit'  value='Edit' id='" << res->getString("id") << "'>";
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
			//write to log file
			writeException("activeVmsShedule", e.what());
			Php::out << currentDateTime() << e.what();
		}
	}

void activeVmsDevices()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select * from system_vms_devices where device_enabled=1");
			res = prep_stmt->executeQuery();
			while (res->next())
			{
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col' id='name-" + res->getString("vms_id") + "' >" << res->getString("vms_name") << "</div>" << std::endl;
				Php::out << "<div class='col' id='location-" + res->getString("vms_id") + "'>" << res->getString("location") << "</div>" << std::endl;
				Php::out << "<div class='col' id='port-name-" + res->getString("vms_id") + "'>" << res->getString("port_name") << "</div>" << std::endl;
				Php::out << "<div class='col' id='hold-status-" + res->getString("vms_id") + "'>" << res->getString("hold_status") << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-danger btn-disable-vms-device btn-block' type='submit'  value='Disable' id='" << res->getString("vms_id") << "'>";
				Php::out << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-info btn-edit-vms-device btn-block' type='submit'  value='Edit' id='" << res->getString("vms_id") << "'>";
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
			//write to log file
			writeException("activeVmsDevices", e.what());
			Php::out << currentDateTime() << e.what();
		}
	}

void disabledVmsDevices()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			string msg;
			prep_stmt = con->prepareStatement("select * from system_vms_devices where device_enabled=0");
			res = prep_stmt->executeQuery();
			while (res->next())
			{			
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col'  >" << res->getString("vms_name") << "</div>" << std::endl;
				Php::out << "<div class='col' >" << res->getString("location") << "</div>" << std::endl;
				Php::out << "<div class='col' >" << res->getString("port_name") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("hold_status") << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-success btn-block btn-enable-vms-device' type='submit'  value='Enable' id='" << res->getString("vms_id") << "'>";
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
			//write to log file
			writeException("disabledVmsDevices", e.what());
			Php::out << currentDateTime() << e.what();
		}
	}

	void disabledVmsShedule()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			string msg;
			prep_stmt = con->prepareStatement("select id,vms_name,message,colour,action,shedule_time,shedule_interval,hold_time,start_datetime from vms_device_shedule where status=0");
			res = prep_stmt->executeQuery();
			while (res->next())
			{
				msg=res->getString("message");				
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("vms_name") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("colour") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("action") << "</div>" << std::endl;
				Php::out << "<div class='col-2'>" << msg << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("shedule_time") << "</div>" << std::endl;
				Php::out << "<div class='col-2'>" << res->getString("shedule_interval") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("hold_time") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("start_datetime") << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-success btn-block btn-enable-vms-shedule' type='submit'  value='Enable' id='" << res->getString("id") << "'>";
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
			//write to log file
			writeException("disabledVmsShedule", e.what());
			Php::out << currentDateTime() << e.what();
		}
	}

void disabledDynamicParkingRates()
	{
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			sql::ResultSet *res;
			prep_stmt = con->prepareStatement("select id,parking_rate_label,datetime,day,rate_type,parking_rate from revenue_dynamic_parking_rates where status=0");
			res = prep_stmt->executeQuery();
			while (res->next())
			{
				Php::out << "<div class='table-view'>" << std::endl;
				Php::out << "<div class='card-text'>" << std::endl;
				Php::out << "<div class='d-flex justify-content-between align-items-center'>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("parking_rate_label") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("datetime") << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("day") << "</div>" << std::endl;
				std::string type;
				if (res->getInt("rate_type") == 1)
					type = "Variable Rate";
				else
					type = "Fixed Rate";
				Php::out << "<div class='col'>" << type << "</div>" << std::endl;
				Php::out << "<div class='col'>" << res->getString("parking_rate") << "</div>" << std::endl;
				Php::out << "<div class='col'>";
				Php::out << "<input class='btn btn-success btn-block btn-enable-dynamic-rate' type='submit'  value='Enable' id='" << res->getString("id") << "'>";
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
			//write to log file
			writeException("activeDynamicParkingRates", e.what());
			cout << currentDateTime() << e.what();
		}
	}

Php::Value insertdynamicParkingRate(Php::Parameters &params)
	{
		string msg = "Insertion Failed";
		try
			{
				sql::Connection *con;
				con = mysqlConnectServer();
				sql::PreparedStatement *prep_stmt;
				string name = params[0];
				string date = params[1];
				string day=params[2];
				int type=params[3];
				string parkingRate=params[4];
				prep_stmt = con->prepareStatement("insert into revenue_dynamic_parking_rates(parking_rate_label,datetime,day,rate_type,parking_rate,status)VALUES(?,?,?,?,?,1)");
				prep_stmt->setString(1, name);
				if(date!="")					
					prep_stmt->setString(2, date);
				else
					prep_stmt->setNull(2,0);
				prep_stmt->setString(3, day);
				prep_stmt->setInt(4, type);
				prep_stmt->setString(5, parkingRate);

				if (!prep_stmt->execute())
					msg = "Insertion Successfull";

				con->close();
				prep_stmt->close();
				delete prep_stmt;
				delete con;
			}
			catch (exception &e)
			{
				//write to log file
				writeException("insertdynamicParkingRate", e.what());
				cout << currentDateTime() << e.what();
			}
		return msg;	
	}
Php::Value insertVmsDevice(Php::Parameters &params)
	{
		string msg = "Insertion Failed";
		try
			{
				sql::Connection *con;
				con = mysqlConnectServer();
				sql::PreparedStatement *prep_stmt;
				string name = params[0];
				string location = params[1];
				string portname=params[2];
				int holdStatus=params[3];				
				prep_stmt = con->prepareStatement("insert into system_vms_devices(vms_name,location,port_name,hold_status,device_enabled)VALUES(?,?,?,?,1)");
				prep_stmt->setString(1, name);
				prep_stmt->setString(2, location);
				prep_stmt->setString(3, portname);
				prep_stmt->setInt(4, holdStatus);

				if (!prep_stmt->execute())
					msg = "Insertion Successfull";

				con->close();
				prep_stmt->close();
				delete prep_stmt;
				delete con;
			}
			catch (exception &e)
			{
				//write to log file
				writeException("insertVmsDevice", e.what());
				cout << currentDateTime() << e.what();
			}
		return msg;	
	}

Php::Value updateVmsDevice(Php::Parameters &params)
	{
		string msg = "Update Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			string name = params[0];
			string location = params[1];
			string portname = params[2];
			int holdStatus = params[3];
			int id = params[4];
			prep_stmt = con->prepareStatement("update system_vms_devices set vms_name=?,location=?,port_name=?,hold_status=? where vms_id=?");
			prep_stmt->setString(1, name);
			prep_stmt->setString(2, location);
			prep_stmt->setString(3, portname);
			prep_stmt->setInt(4, holdStatus);
			prep_stmt->setInt(5, id);

			if (!prep_stmt->execute())
				msg = "Update Successfull";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("updateVmsDevice", e.what());
			cout << currentDateTime() << e.what();
		}
		return msg;
	}
	Php::Value insertVmsShedule(Php::Parameters &params)
	{
		string msg = "Insertion Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			string vms_id = params[0];
			string vms_name = params[1];
			string message = params[2];
			string colour = params[3];
			string action = params[4];
			string shedule_time = params[5];
			string shedule_interval = params[6];
			string hold_time = params[7];
			string start_datetime = params[8];
			prep_stmt = con->prepareStatement("insert into vms_device_shedule(vms_id,vms_name,message,colour,action,shedule_time,shedule_interval,hold_time,start_datetime,status)VALUES(?,?,?,?,?,?,?,?,?,1)");
			prep_stmt->setString(1, vms_id);
			prep_stmt->setString(2, vms_name);
			prep_stmt->setString(3, message);
			prep_stmt->setString(4, colour);
			prep_stmt->setString(5, action);
			prep_stmt->setString(6, shedule_time);
			prep_stmt->setString(7, shedule_interval);
			prep_stmt->setString(8, hold_time);
			prep_stmt->setString(9, start_datetime);
			if (!prep_stmt->execute())
				msg = "Insertion Successfull";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("insertdynamicParkingRate", e.what());
			cout << currentDateTime() << e.what();
		}
		return msg;
	}

Php::Value updateVmsShedule(Php::Parameters &params)
	{
		string msg = "Update Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			string vms_id = params[0];
			string vms_name = params[1];
			string message = params[2];
			string colour = params[3];
			string action = params[4];
			string shedule_time = params[5];
			string shedule_interval = params[6];
			string hold_time = params[7];
			string start_datetime = params[8];
			string id = params[9];
			prep_stmt = con->prepareStatement("update  vms_device_shedule set vms_id=?,vms_name=?,message=?,colour=?,action=?,shedule_time=?,shedule_interval=?,hold_time=?,start_datetime=? where id=?");
			prep_stmt->setString(1, vms_id);
			prep_stmt->setString(2, vms_name);
			prep_stmt->setString(3, message);
			prep_stmt->setString(4, colour);
			prep_stmt->setString(5, action);
			prep_stmt->setString(6, shedule_time);
			prep_stmt->setString(7, shedule_interval);
			prep_stmt->setString(8, hold_time);
			prep_stmt->setString(9, start_datetime);
			prep_stmt->setString(10, id);
			if (!prep_stmt->execute())
				msg = "Update Successfull";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("updateVmsShedule", e.what());
			cout << currentDateTime() << e.what();
		}
		return msg;
	}

Php::Value updateStatusDynamicParkingRate(Php::Parameters &params) 
	{
		string msg = "Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			int id = params[0];
			int status=params[1];
			prep_stmt = con->prepareStatement("update revenue_dynamic_parking_rates set status=? where id=?");
			prep_stmt->setInt(1, status);
			prep_stmt->setInt(2, id);

			if (!prep_stmt->execute())
				msg = "Updated successfully";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("updateStatusDynamicParkingRate", e.what());
			cout << currentDateTime() << e.what();
			return e.what();
		}
		return msg;
	} //end of .setActiveDynamicParkingRate

Php::Value updateStatusVmsDevices(Php::Parameters &params) 
	{
		string msg = "Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			int id = params[0];
			int status=params[1];
			prep_stmt = con->prepareStatement("update system_vms_devices set device_enabled=? where vms_id=?");
			prep_stmt->setInt(1, status);
			prep_stmt->setInt(2, id);

			if (!prep_stmt->execute())
				msg = "Updated successfully";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("updateStatusVmsDevices", e.what());
			cout << currentDateTime() << e.what();
			return e.what();
		}
		return msg;
	} //end of .setActiveDynamicParkingRate

Php::Value updateStatusVmsShedule(Php::Parameters &params)
	{
		string msg = "Failed";
		try
		{
			sql::Connection *con;
			con = mysqlConnectServer();
			sql::PreparedStatement *prep_stmt;
			int id = params[0];
			int status = params[1];
			prep_stmt = con->prepareStatement("update vms_device_shedule set status=? where id=?");
			prep_stmt->setInt(1, status);
			prep_stmt->setInt(2, id);
			if (!prep_stmt->execute())
				msg = "Updated successfully";

			con->close();
			prep_stmt->close();
			delete prep_stmt;
			delete con;
		}
		catch (exception &e)
		{
			//write to log file
			writeException("updateStatusVmsShedule", e.what());
			cout << currentDateTime() << e.what();
			return e.what();
		}
		return msg;
	} //end of .setActiveVmsShedule



extern "C"
	{
		PHPCPP_EXPORT void *get_module()
		{
			static Php::Extension extension("PX_Configuration", "1.0");
			extension.add<parkingRatesLabels>("parkingRatesLabels");
			extension.add<activeDynamicParkingRates>("activeDynamicParkingRates");
			extension.add<activeVmsShedule>("activeVmsShedule");
			extension.add<activeVmsDevices>("activeVmsDevices");
			extension.add<disabledVmsDevices>("disabledVmsDevices");
			extension.add<disabledDynamicParkingRates>("disabledDynamicParkingRates");
			extension.add<disabledVmsShedule>("disabledVmsShedule");
			extension.add<insertdynamicParkingRate>("insertdynamicParkingRate");
			extension.add<insertVmsDevice>("insertVmsDevice");
			extension.add<insertVmsShedule>("insertVmsShedule");
			extension.add<updateVmsShedule>("updateVmsShedule");
			extension.add<updateVmsDevice>("updateVmsDevice");
			extension.add<updateStatusDynamicParkingRate>("updateStatusDynamicParkingRate");
			extension.add<updateStatusVmsShedule>("updateStatusVmsShedule");
			extension.add<updateStatusVmsDevices>("updateStatusVmsDevices");
			extension.add<vmsDevices>("vmsDevices");
			return extension;
		}
}

