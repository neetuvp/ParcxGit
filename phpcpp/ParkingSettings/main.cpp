//make -B
//sudo make install
//service apache2 restart

#include <phpcpp.h>
#include<iostream>
#include<stdlib.h>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <mysql.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

using namespace std;
sql::Connection *conn;
sql::Driver *driver;
sql::Statement *stmt;
sql::ResultSet *rs;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define DATABASE "parcx_server"


sql::Connection* mySQLConnect()
{
 
  driver = get_driver_instance();
  conn = driver->connect(DBServer,DBUsername,DBPassword);
  return conn;
}

sql::Connection* DBConnection()
{
  conn = mySQLConnect();
  conn->setSchema("parcx_server");
  stmt = conn->createStatement();
 // Php::out<<"Connection Successs"<<endl;
  return conn;
}
Php::Value GetParkingRateCount(Php::Parameters &params)
{
    MYSQL *conn;
	MYSQL_RES *res;
	conn = mysql_init (NULL);	  
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
    }  
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(conn)
    {
        //Php::out<<"Connection Succeeded"<<endl;
    }
    else{
       // Php::out<<"Connection Failed"<<endl;
    }
    
    mysql_query(conn,("SELECT * FROM revenue_parking_rate_label"));
	res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);

    if(res != NULL)
    {
 	    mysql_free_result(res);
    }

	mysql_close (conn);

    return num_fields;
}
Php::Value GetParkingRateLabels(Php::Parameters &params)
{
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	  
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
    }  
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(conn)
    {
        //Php::out<<"Connection Succeeded"<<endl;
    }
    else{
       // Php::out<<"Connection Failed"<<endl;
    }
    
    mysql_query(conn,("SELECT * FROM revenue_parking_rate_label"));
	res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);
	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		    std::string fieldname = array[z];
            array[fieldname] = row[z];
            rows[x]=array;
	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

    return rows;
}
Php::Value GetParkingRateSettings(Php::Parameters &params)
{
    string rate = params[0];
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	  
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
    }  
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(conn)
    {
        //Php::out<<"Connection Succeeded"<<endl;
    }
    else{
       // Php::out<<"Connection Failed"<<endl;
    }
    string sql = "SELECT id,time_unit_"+rate+",time_duration_"+rate+","+rate+" from revenue_parking_rates";
    
    mysql_query(conn,(sql.c_str()));
	res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);
	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		    std::string fieldname = array[z];
            array[fieldname] = row[z];
            rows[x]=array;
	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

    return rows;
}
Php::Value PostParkingRateSettings(Php::Parameters &params)
{
    int result=0;
    sql::Connection *con;
    con = DBConnection();
    int id = params[0];
    string rateno = params[1];
    string timeunit = params[2];
    int timeduration = params[3];
    int rate = params[4];
    prep = con->prepareStatement("Update revenue_parking_rates set time_unit_"+rateno+"=?,time_duration_"+rateno+"=?,"+rateno+"=? where id = ?");

    prep->setString(1,timeunit);
    prep->setInt(2,timeduration);
    prep->setInt(3,rate);
    prep->setInt(4,id);

    if(prep->executeUpdate())
    {
        result = 1;
    }
    prep->close();
    con->close();
    return result;

}
Php::Value GetFixedParkingRateSettings(Php::Parameters &params)
{
    string rate = params[0];
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);	  
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
    }  
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(conn)
    {
        //Php::out<<"Connection Succeeded"<<endl;
    }
    else{
       // Php::out<<"Connection Failed"<<endl;
    }
    string sql = "SELECT id,rate_labels,"+rate+" from revenue_fixed_parking_rates";
    
    mysql_query(conn,(sql.c_str()));
	res = mysql_store_result(conn);
    int num_fields = mysql_num_fields(res);
	Php::Array array;
	Php::Value rows;
	int i = 0;

	while((field = mysql_fetch_field(res))){
	    array[i] = field->name;
	    i++;
	}

	int x = 0;
	while ((row = mysql_fetch_row(res)))
	{

	    for (int z=0;z<num_fields;z++) 
	    {
		    std::string fieldname = array[z];
            array[fieldname] = row[z];
            rows[x]=array;
	    }

	    x++;
	}
	if(res != NULL)
	mysql_free_result(res);
	mysql_close (conn);

    return rows;
}
Php::Value PostFixedParkingRateSettings(Php::Parameters &params)
{
    int result=0;
    sql::Connection *con;
    con = DBConnection();
    int id = params[0];
    string rateno = params[1];
    string ratelabel = params[2];
    int rate = params[3];
    prep = con->prepareStatement("Update revenue_fixed_parking_rates set rate_labels=?,"+rateno+"=? where id = ?");

    prep->setString(1,ratelabel);
    prep->setInt(2,rate);
    prep->setInt(3,id);

    if(prep->executeUpdate())
    {
        result = 1;
    }
    prep->close();
    con->close();
    return result;

}
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension extension("PX_ParkingSettings", "1.0");
        extension.add<GetParkingRateLabels>("GetParkingRateLabels");
        extension.add<GetParkingRateCount>("GetParkingRateCount");
        extension.add<GetParkingRateSettings>("GetParkingRateSettings");
        extension.add<GetFixedParkingRateSettings>("GetFixedParkingRateSettings");
        extension.add<PostParkingRateSettings>("PostParkingRateSettings");
         extension.add<PostFixedParkingRateSettings>("PostFixedParkingRateSettings");
        // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
