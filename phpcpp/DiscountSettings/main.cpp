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
Php::Value GetCarparks(Php::Parameters &params)
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
       // Php::out<<"Connection Succeeded"<<endl;
    }
    else{
        Php::out<<"Connection Failed"<<endl;
    }
    mysql_query(conn,("SELECT * FROM system_carparks"));
   
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
Php::Value GetDiscountSettings(Php::Parameters &params)
{
    string s = params[0];
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
    if(s=="")
    {
        mysql_query(conn,("SELECT * FROM revenue_discounts"));
    }
    else{
        string sql = "SELECT * FROM revenue_discounts where id = "+s;
        mysql_query(conn,(sql.c_str()));
    }
   
   
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

Php::Value GetNextDiscountId(Php::Parameters &params)
{

    int id=1;
    conn = DBConnection();
    rs = stmt->executeQuery("SELECT max(discount_id) as discount_id FROM revenue_discounts");
	while(rs->next())
    {
        if(rs->getString("discount_id") != "")
        {
            id = rs->getInt("discount_id")+1;
        }
    }
    delete conn;

    return id;

}


Php::Value PostDiscountSettings(Php::Parameters &params)
{
   int result=0;
    sql::Connection *con;
    try{

  
    con = DBConnection();
    int id = params[0];

    int discount_id = params[1];

    string discount_name = params[2];

    string discount_type = params[3];

    string discount_option = params[4];

    int discount_category = params[5];

    int discount_value = params[6];

    string carpark_number = params[7];

    string status = params[8];

    if(params[0] == "")
    {
        prep = con->prepareStatement("Insert into revenue_discounts (discount_id,discount_name,discount_type,discount_option,discount_category,discount_value,carpark_number,status) values(?,?,?,?,?,?,?,?)");
    }
    else
    {
       
        prep = con->prepareStatement("Update revenue_discounts set discount_id=?,discount_name=?,discount_type=?,discount_option=?,discount_category=?,discount_value=?,carpark_number=?,status=? where id = ?");
        prep->setInt(9,id);
    }

    prep->setInt(1,discount_id);
    prep->setString(2,discount_name);
    prep->setString(3,discount_type);
    prep->setString(4,discount_option);
    prep->setInt(5,discount_category);
    prep->setInt(6,discount_value);
    prep->setString(7,carpark_number);
    prep->setString(8,status);

    if(prep->executeUpdate())
    {
        result = 1;
  }
    //prep->close();
    con->close();
    }
    catch (exception &e)
    {
        Php::out<<e.what()<<endl;
    }
    return result;

}


Php::Value DisableDiscountSettings(Php::Parameters &params)
{
    sql::Connection *con;
    con = DBConnection();
    int id = params[0];
    int result=0;

    prep = con->prepareStatement("Update revenue_discounts set status = 0 where id = ?");
    prep->setInt(1,id);

    if(prep->executeUpdate())
    {
        result = 1;
    }
   prep->close();
   con->close();
   return result;

}

Php::Value EnableDiscountSettings(Php::Parameters &params)
{
    sql::Connection *con;
    con = DBConnection();
    int id = params[0];
    int result=0;

    prep = con->prepareStatement("Update revenue_discounts set status = 1 where id = ?");
    prep->setInt(1,id);

    if(prep->executeUpdate())
    {
        result = 1;
    }
   prep->close();
   con->close();
   return result;

}
/**
 *  tell the compiler that the get_module is a pure C function
 */
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
        static Php::Extension extension("PX_DiscountSettings", "1.0");
        extension.add<GetDiscountSettings>("GetDiscountSettings");
        extension.add<GetNextDiscountId>("GetNextDiscountId");
        extension.add<PostDiscountSettings>("PostDiscountSettings");
        extension.add<DisableDiscountSettings>("DisableDiscountSettings");
        extension.add<EnableDiscountSettings>("EnableDiscountSettings");
        extension.add<GetCarparks>("GetCarparks");
        // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
