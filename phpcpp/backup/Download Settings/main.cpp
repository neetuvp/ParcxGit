//CURL-OPENSSL Required to access https  : sudo apt-get install libcurl4-openssl-dev
//JSON.h install :   sudo apt-get install libjsoncpp-dev
//make -B
//sudo make install
//service apache2 restart
#include <phpcpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <fstream>
#include <ctime>
#include <time.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sstream>
#include <string.h>
#include <unistd.h>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <jsoncpp/json/json.h>
#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define DATABASE "parcx_server"
#define DATABASE_DASHBOARD "parcx_dashboard"
#define DATABASE_REPORTING "parcx_reporting"
using namespace std;

sql::Connection *conn;
sql::Driver *driver;

Php::Value response;

string currentTime()
{
	time_t now = time(NULL);
	std::tm *timeinfo;
	char buffer[80];
	std::time(&now);
	timeinfo = std::localtime(&now);
	std::strftime(buffer, 80, "%H:%M:%S", timeinfo);
	string datetime(buffer);
	return datetime;
}
string currentDate()
{
	time_t now = time(NULL);
	std::tm *timeinfo;
	char buffer[80];
	std::time(&now);
	timeinfo = std::localtime(&now);
	std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
	string date(buffer);
	return date;
}
void WriteToLog(string msg)
{
	Php::out << msg << endl;
	ofstream myfile;
	myfile.open("/opt/parcx/Logs/Services/ExceptionLogs/PX_download_settings_error_" + currentDate() + ".log", fstream::app);
	myfile << currentTime() + "   " << msg << endl;
	myfile.close();
}

sql::Connection *DBConnection(string db)
{
	driver = get_driver_instance();
	conn = driver->connect(DBServer, DBUsername, DBPassword);
	conn->setSchema(db);
	return conn;
}

Php::Value serverHandhShake(Php::Parameters &params)
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Php::Value response;
		int threshold = 0, free_spaces = 0;
		;
		string device_number = params[0];
		string carpark_number = params[1];
		conn = DBConnection(DATABASE_DASHBOARD);
		stmt = conn->createStatement();
		res = stmt->executeQuery("SELECT * FROM  counters where carpark_number=" + carpark_number + " and counter_type=1");
		if (res->next())
		{
			threshold = res->getInt("occupancy_threshold");
			free_spaces = res->getInt("free_spaces");
			if (free_spaces <= threshold)
				response["total_free_spaces"] = 0;
			else
				response["total_free_spaces"] = free_spaces;

			response["reservation_free_spaces"] = res->getInt("total_reservation_spaces") - res->getInt("reservation_current_level");
			response["access_free_spaces"] = res->getInt("total_access_spaces") - res->getInt("access_current_level");
			response["shortterm_free_spaces"] = res->getInt("total_shortterm_spaces") - res->getInt("shortterm_current_level");
		}

		delete res;
		delete stmt;
		delete conn;
		return response;
	}
	catch (sql::SQLException &e)
	{
		WriteToLog("****serverHandhShake*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadSettings(string device_number)
{
    try
	{
	sql::Statement *stmt;
    sql::ResultSet *res;
	Json::Value jsonrow;
    Json::FastWriter fw;
	int category=0;
	string rate_plan,reservation_rate_plan="";    
    conn = DBConnection(DATABASE);
	stmt = conn->createStatement();
	res = stmt->executeQuery("SELECT * FROM system_devices WHERE device_number = "+device_number+" and device_enabled=1");
	if(res->next())
		{ 
		//jsonrow["error"]="FALSE";
		category=res->getInt("device_category");
		rate_plan=res->getString("rate_plan");
		reservation_rate_plan=res->getString("reservation_rate_plan");
		jsonrow["device_name"]=string(res->getString("device_name")); 
		jsonrow["device_type"]=string(res->getString("device_category")); 		
		jsonrow["carpark_name"]=string(res->getString("carpark_name"));
        jsonrow["carpark_number"]=string(res->getString("carpark_number"));		
		jsonrow["camera_id"]=string(res->getString("camera_id"));		
		if(category==3||category==4||category==5)	
			{
			jsonrow["rate_plan"]=string(res->getString("rate_plan"));	
			jsonrow["reservation_rate_plan"]=string(res->getString("reservation_rate_plan")); 		
			jsonrow["customer_receipt_mandatory"]=string(res->getString("customer_receipt_mandatory"));
			jsonrow["shift_receipt_mandatory"]=string(res->getString("shift_receipt_mandatory"));        
			jsonrow["physical_cash_count"]=string(res->getString("shift_physical_cash_count_required"));        
			jsonrow["rate_type"]=string(res->getString("rate_type")); 
			jsonrow["issue_lost"]=string(res->getString("issue_lost"));                   
			}
		}
		
	if(category==1||category==2)
		{
		int entry_grace_mins=0,entry_grace_hours=0,exit_grace_mins=0,exit_grace_hours=0;
		int r_entry_grace_mins=0,r_entry_grace_hours=0,r_exit_grace_mins=0,r_exit_grace_hours=0;
		if(reservation_rate_plan!="")
			res = stmt->executeQuery("SELECT fixed_fees,"+rate_plan+" as rate_plan,"+reservation_rate_plan+" as reservation_rate_plan FROM revenue_fixed_parking_rates");			
		else
			res = stmt->executeQuery("SELECT fixed_fees,"+rate_plan+" as rate_plan FROM revenue_fixed_parking_rates");			
		while(res->next())
			{
			if(res->getString("fixed_fees")=="entry_grace_period_mins")
					entry_grace_mins=res->getInt("rate_plan");
			if(res->getString("fixed_fees")=="entry_grace_period_hours")
					entry_grace_hours=res->getInt("rate_plan");
			if(res->getString("fixed_fees")=="exit_grace_period_mins")
					exit_grace_mins=res->getInt("rate_plan");
			if(res->getString("fixed_fees")=="exit_grace_period_hours")
					exit_grace_hours=res->getInt("rate_plan");
				
			if(reservation_rate_plan!="")
				{
				if(res->getString("fixed_fees")=="reservation_entry_grace_period_mins")
						r_entry_grace_mins=res->getInt("reservation_rate_plan");	
				if(res->getString("fixed_fees")=="reservation_entry_grace_period_hours")
						r_entry_grace_hours=res->getInt("reservation_rate_plan");
				if(res->getString("fixed_fees")=="reservation_exit_grace_period_mins")
						r_exit_grace_mins=res->getInt("reservation_rate_plan");
				if(res->getString("fixed_fees")=="reservation_exit_grace_period_hours")
						r_exit_grace_hours=res->getInt("reservation_rate_plan");
				}
			}
		jsonrow["entry_grace_period"]=to_string(entry_grace_hours*60+entry_grace_mins);
		jsonrow["exit_grace_period"]=to_string(exit_grace_hours*60+exit_grace_mins);
		jsonrow["reservation_entry_grace_period"]=to_string(r_entry_grace_hours*60+r_entry_grace_mins);
		jsonrow["reservation_exit_grace_period"]=to_string(r_exit_grace_hours*60+r_exit_grace_mins);
		}
		
	
		res = stmt->executeQuery("SELECT * FROM system_settings where setting_status=1");
		while(res->next())
			{
			if(res->getString("setting_name")=="facility_name")
				jsonrow["facility_name"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="facility_number")
				jsonrow["facility_number"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="address_line1")
				jsonrow["address_line1"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="address_line2")
				jsonrow["address_line2"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="site_code")
				jsonrow["site_code"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="primary_language")
				jsonrow["primary_language"]=string(res->getString("setting_value"));
			
			if(res->getString("setting_name")=="secondary_language")
				jsonrow["secondary_language"]=string(res->getString("setting_value"));
			
			
			if(category==3||category==4||category==5)	
				{
				if(res->getString("setting_name")=="vat_percentage")
					jsonrow["vat_percentage"]=string(res->getString("setting_value"));
				
				if(res->getString("setting_name")=="vat_type")
					jsonrow["vat_type"]=string(res->getString("setting_value"));
				
				if(res->getString("setting_name")=="vat_id")
					jsonrow["vat_id"]=string(res->getString("setting_value"));
				
				if(res->getString("setting_name")=="ticket_validity")
					jsonrow["ticket_validity"]=string(res->getString("setting_value"));
				
				if(res->getString("setting_name")=="currency")
					jsonrow["currency"]=string(res->getString("setting_value"));
					
				if(res->getString("setting_name")=="dayclosure_endtime")
					jsonrow["dayclosure_endtime"]=string(res->getString("setting_value"));

					if(res->getString("setting_name")=="decimal_places")
					jsonrow["decimal_places"]=string(res->getString("setting_value"));

				}			
			}	
		
    delete res;
    delete stmt;
    conn->close();
    delete conn; 
			
	string result= fw.write(jsonrow);                                
    return result;
    }
    catch(sql::SQLException &e)
    {
       
        WriteToLog("****downloadSettings*****");
        WriteToLog(e.what());
		return "{'error':'true'}";
    }
              
}

Php::Value downloadParkingRateLabel()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("SELECT * from revenue_parking_rate_label where id=1");
		while (res->next())
		{
			jsonrow["parking_rates1"] = string(res->getString("parking_rate1"));
			jsonrow["parking_rates2"] = string(res->getString("parking_rate2"));
			jsonrow["parking_rates3"] = string(res->getString("parking_rate3"));
			jsonrow["parking_rates4"] = string(res->getString("parking_rate4"));
			jsonrow["parking_rates5"] = string(res->getString("parking_rate5"));
		}

		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsonrow);
		return result;
	}
	catch (sql::SQLException &e)
	{

		WriteToLog("****downloadParkingRateLabel*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}
Php::Value downloadDenominations(string currency)
{
    try
	{
	sql::Statement *stmt;
    sql::ResultSet *res;
	Json::Value jsonrow;
    Json::FastWriter fw;    
    conn = DBConnection(DATABASE);
	stmt = conn->createStatement();				
	res = stmt->executeQuery("select * from revenue_banknote_denominations where currency='"+currency+"'");
	if(res->next())
		{
		sql::ResultSetMetaData *res_meta = res -> getMetaData();
		string key;
		int columns = res_meta -> getColumnCount();   
		for (int i = 1; i <= columns; i++) 							
			{
			key=res_meta -> getColumnLabel(i);			
			jsonrow[key]=string(res->getString(i));			
			}
		}
    delete res;
    delete stmt;
    conn->close();
    delete conn; 
			
	string result= fw.write(jsonrow);                                
    return result;
    }
    catch(sql::SQLException &e)
    {
       
        WriteToLog("****downloadDenominations*****");
        WriteToLog(e.what());
		return "{'error':'true'}";
    }
              
}
Php::Value downloadFixedParkingRates()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From revenue_fixed_parking_rates");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["fixed_fees"] = string(res->getString("fixed_fees"));
			jsonrow["parking_rates1"] = string(res->getString("parking_rates1"));
			jsonrow["parking_rates2"] = string(res->getString("parking_rates2"));
			jsonrow["parking_rates3"] = string(res->getString("parking_rates3"));
			jsonrow["parking_rates4"] = string(res->getString("parking_rates4"));
			jsonrow["parking_rates5"] = string(res->getString("parking_rates5"));
			jsonrow["rate_labels"] = string(res->getString("rate_labels"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadFixedParkingRates*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}
Php::Value downloadParkingRates()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From revenue_parking_rates");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["time_unit_parking_rates1"] = string(res->getString("time_unit_parking_rates1"));
			jsonrow["time_duration_parking_rates1"] = string(res->getString("time_duration_parking_rates1"));
			jsonrow["parking_rates1"] = string(res->getString("parking_rates1"));
			jsonrow["time_unit_parking_rates2"] = string(res->getString("time_unit_parking_rates2"));
			jsonrow["time_duration_parking_rates2"] = string(res->getString("time_duration_parking_rates2"));
			jsonrow["parking_rates2"] = string(res->getString("parking_rates2"));
			jsonrow["time_unit_parking_rates3"] = string(res->getString("time_unit_parking_rates3"));
			jsonrow["time_duration_parking_rates3"] = string(res->getString("time_duration_parking_rates3"));
			jsonrow["parking_rates3"] = string(res->getString("parking_rates3"));
			jsonrow["time_unit_parking_rates4"] = string(res->getString("time_unit_parking_rates4"));
			jsonrow["time_duration_parking_rates4"] = string(res->getString("time_duration_parking_rates4"));
			jsonrow["parking_rates4"] = string(res->getString("parking_rates4"));
			jsonrow["time_unit_parking_rates5"] = string(res->getString("time_unit_parking_rates5"));
			jsonrow["time_duration_parking_rates5"] = string(res->getString("time_duration_parking_rates5"));
			jsonrow["parking_rates5"] = string(res->getString("parking_rates5"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadParkingRates*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}
Php::Value downloadDynamicParkingRates()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From revenue_dynamic_parking_rates where status=1");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["parking_rate_label"] = string(res->getString("parking_rate_label"));
			jsonrow["datetime"] = string(res->getString("datetime"));
			jsonrow["day"] = string(res->getString("day"));
			jsonrow["rate_type"] = string(res->getString("rate_type"));
			jsonrow["parking_rate"] = string(res->getString("parking_rate"));
			jsonrow["status"] = "1";
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadDynamicParkingRates*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}
Php::Value downloadDiscounts()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From revenue_discounts where status=1");
		
	
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["discount_id"] = string(res->getString("discount_id"));
			jsonrow["discount_name"] = string(res->getString("discount_name"));
			jsonrow["discount_type"] = string(res->getString("discount_type"));
			jsonrow["discount_option"] = string(res->getString("discount_option"));
			jsonrow["discount_category"] = string(res->getString("discount_category"));
			jsonrow["discount_value"] = string(res->getString("discount_value"));
			jsonrow["carpark_number"] = string(res->getString("carpark_number"));
			jsonrow["status"] = "1";
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadDiscounts*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadUsers()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From system_users where account_status=1");
		while (res->next())
		{
			jsonrow["user_id"] = string(res->getString("user_id"));
			jsonrow["operator_name"] = string(res->getString("operator_name"));
			jsonrow["username"] = string(res->getString("username"));
			jsonrow["company_name"] = string(res->getString("company_name"));
			jsonrow["email"] = string(res->getString("email"));
			jsonrow["phone"] = string(res->getString("phone"));
			jsonrow["password"] = string(res->getString("password"));
			jsonrow["user_role_id"] = string(res->getString("user_role_id"));
			jsonrow["language"] = string(res->getString("language"));
			jsonrow["validity_from_date"] = string(res->getString("validity_from_date"));
			jsonrow["validity_to_date"] = string(res->getString("validity_to_date"));
			jsonrow["account_status"] = string(res->getString("account_status"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadUsers*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadCustomerMessagesText(string device_type)
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From customer_messages_text where device_type=" + device_type + " and status=1");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["message"] = string(res->getString("message"));
			jsonrow["english_line1"] = string(res->getString("english_line1"));
			jsonrow["english_line2"] = string(res->getString("english_line2"));
			jsonrow["english_line3"] = string(res->getString("english_line3"));
			jsonrow["arabic_line1"] = string(res->getString("arabic_line1"));
			jsonrow["arabic_line2"] = string(res->getString("arabic_line2"));
			jsonrow["arabic_line3"] = string(res->getString("arabic_line3"));
			jsonrow["spanish_line1"] = string(res->getString("spanish_line1"));
			jsonrow["spanish_line2"] = string(res->getString("spanish_line2"));
			jsonrow["spanish_line3"] = string(res->getString("spanish_line3"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadCustomerMessagesText*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadCustomerMessagesMedia(string device_type)
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;

		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From customer_messages_media where device_type=" + device_type + " and status=1");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("id"));
			jsonrow["message"] = string(res->getString("message"));
			jsonrow["media_type"] = string(res->getString("media_type"));
			jsonrow["media_path"] = string(res->getString("media_path"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadCustomerMessagesMedia*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadAccessWhitelist()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From access_whitelist");
		while (res->next())
		{
			jsonrow["id"] = string(res->getString("access_whitelist_id"));
			jsonrow["facility_number"] = string(res->getString("facility_number"));
			jsonrow["carpark_number"] = string(res->getString("carpark_number"));
			jsonrow["access_zones"] = string(res->getString("access_zones"));
			jsonrow["tag_serial"] = string(res->getString("tag_serial"));
			jsonrow["tag_tid"] = string(res->getString("tag_tid"));
			jsonrow["customer_name"] = string(res->getString("customer_name"));
			jsonrow["tag"] = string(res->getString("tag"));
			jsonrow["start_date"] = string(res->getString("validity_start_date"));
			jsonrow["expiry_date"] = string(res->getString("validity_expiry_date"));
			jsonrow["antipassback"] = string(res->getString("antipassback_enabled"));
			jsonrow["status"] = string(res->getString("status"));
			jsonrow["plate_number"] = string(res->getString("plate_number"));
			jsonrow["ticket_id"] = string(res->getString("ticket_id"));
			jsonrow["personalized_message_line1"] = string(res->getString("personalized_message_line1"));
			jsonrow["personalized_message_line2"] = string(res->getString("personalized_message_line2"));
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadAccessWhitelist*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadManualMovementsReasons(string carpark_number, string facility_number)
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From manual_movements_reasons where status = 1 and carpark_number ='" + carpark_number + "' and facility_number ='" + facility_number + "'");

		while (res->next())
		{
			jsonrow["reason"] = string(res->getString("reason"));
			jsonrow["carpark_number"] = string(res->getString("carpark_number"));
			jsonrow["facility_number"] = string(res->getString("facility_number"));
			jsonrow["device_number"] = string(res->getString("device_number"));

			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadManualMovementsReasons*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadReservations()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::ResultSetMetaData *res_meta;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE_REPORTING);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From parking_reservation");
		res_meta = res->getMetaData();
		unsigned i = 1;
		while (res->next())
		{
			for (i = 1; i <= res_meta->getColumnCount(); i++)
			{
				jsonrow[res_meta->getColumnName(i)] = string(res->getString(res_meta->getColumnName(i)));
			}
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadReservations*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadApplicationLabels()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::ResultSetMetaData *res_meta;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From device_application_label");
		res_meta = res->getMetaData();
		unsigned i = 1;
		while (res->next())
		{
			for (i = 1; i <= res_meta->getColumnCount(); i++)
			{
				jsonrow[res_meta->getColumnName(i)] = string(res->getString(res_meta->getColumnName(i)));
			}
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadApplicationLabels*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadReceiptLabels()
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::ResultSetMetaData *res_meta;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		res = stmt->executeQuery("select * From receipt_settings");
		res_meta = res->getMetaData();
		unsigned i = 1;
		while (res->next())
		{
			for (i = 1; i <= res_meta->getColumnCount(); i++)
			{
				jsonrow[res_meta->getColumnName(i)] = string(res->getString(res_meta->getColumnName(i)));
			}
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadReceiptLabels*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadProducts(string carpark_number, string facility_number)
{
	try
	{
		sql::Statement *stmt;
		sql::ResultSet *res;
		sql::ResultSetMetaData *res_meta;
		Json::Value jsondata(Json::arrayValue);
		Json::Value jsonrow;
		Json::FastWriter fw;
		conn = DBConnection(DATABASE);
		stmt = conn->createStatement();
		//res = stmt->executeQuery("select * From revenue_products where status = 1 and carpark_number ='" + carpark_number + "' and facility_number ='" + facility_number + "'");

		res = stmt->executeQuery("select * from system_products where product_type='contract_parking' and status = 1 and find_in_set("+carpark_number+",carparks) and facility_number ='" + facility_number + "'");

		res_meta = res->getMetaData();
		unsigned i = 1;
		while (res->next())
		{
			for (i = 1; i <= res_meta->getColumnCount(); i++)
			{
				jsonrow[res_meta->getColumnName(i)] = string(res->getString(res_meta->getColumnName(i)));
			}
			jsondata.append(jsonrow);
		}
		delete res;
		delete stmt;
		conn->close();
		delete conn;

		string result = fw.write(jsondata);
		return result;
	}
	catch (exception &e)
	{
		WriteToLog("****downloadProducts*****");
		WriteToLog(e.what());
		return "{'error':'true'}";
	}
}

Php::Value downloadDeviceSettings(Php::Parameters &params)
{
	Php::Value data = params[0];
	int task = data["task"];

	switch (task)
	{
	case 1:
	{
		string device = data["device_number"];
		response = downloadSettings(device);
		break;
	}
	case 2:
		response = downloadFixedParkingRates();
		break;
	case 3:
		response = downloadParkingRates();
		break;
	case 4:
		response = downloadDynamicParkingRates();
		break;
	case 5:
		response = downloadDiscounts();
		break;
	case 6:
		response = downloadParkingRateLabel();
		break;
	case 7:
	{
		string currency = data["currency"];
		response = downloadDenominations(currency);
		break;
	}
	case 8:
		response = downloadUsers();
		break;
	case 9:
	{
		string device_type = data["device_type"];
		response = downloadCustomerMessagesText(device_type);
		break;
	}
	case 10:
	{
		string device_type = data["device_type"];
		response = downloadCustomerMessagesMedia(device_type);
		break;
	}
	case 11:
		response = downloadAccessWhitelist();
		break;
	case 12:
	{
		string carpark_number = data["carpark_number"];
		string facility_number = data["facility_number"];
		response = downloadManualMovementsReasons(carpark_number, facility_number);
		break;
	}

	case 13:
		response = downloadReservations();
		break;

	case 14:
		response = downloadApplicationLabels();
		break;

	case 15:
		response = downloadReceiptLabels();
		break;

	case 16:
	{
		string carpark_number = data["carpark_number"];
		string facility_number = data["facility_number"];
		response = downloadProducts(carpark_number, facility_number);
		break;
	}
	}

	return response;
}

extern "C"
{
	PHPCPP_EXPORT void *get_module()
	{
		static Php::Extension extension("PX_DownloadSettings", "1.0");
		extension.add<downloadDeviceSettings>("downloadDeviceSettings");
		extension.add<serverHandhShake>("serverHandhShake");
		return extension;
	}
}
