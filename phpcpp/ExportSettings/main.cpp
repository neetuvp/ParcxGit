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
  conn->setSchema(DATABASE);
  stmt = conn->createStatement();
 // Php::out<<"Connection Successs"<<endl;
  return conn;
}
Php::Value GetExport(Php::Parameters &params)
{
    Php::Value rows;
    string s = "";
    try
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
        
        string sql = "Select * from DataExportSettings";
        mysql_query(conn,(sql.c_str()));
        
    
        res = mysql_store_result(conn);
        int num_fields = mysql_num_fields(res);
        Php::Array array;

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
    }
    catch (exception &e)
    {
        //write to log file
        //Log("activeDynamicParkingRates", e.what());
        cout << e.what();
    }


    return rows;
}
/*Php::Value GetExport(Php::Parameters &params)
{
    MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;
	conn = mysql_init (NULL);
Php::out<<"heree"<<endl;	
    if(!conn)
    {
        Php::out<<"Mysql Init Failed"<<endl;
    }  
    else
    {
        //Php::out<<"Mysql Init Success"<<endl;
    }
    
	
	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
    if(conn)
    {
        //Php::out<<"Connection Succeeded"<<endl;
    }
    else{
       // Php::out<<"Connection Failed"<<endl;
    }
    string sql = "Select * from DataExportSettings";
    
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
  
}*/
Php::Value GetInterfaceName(Php::Parameters &params)
{
    int id = params[0];
    Php::Value interface_name;
    conn = DBConnection();
    rs = stmt->executeQuery("SELECT interface_name from interface_settings where id="+to_string(id));
	if(rs->next())
    {
        interface_name = rs->getString("interface_name");
    }
    delete rs;
    delete stmt;
    delete conn;
    return interface_name;

}
Php::Value SelectInterfaces(Php::Parameters &params)
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
    string sql = "Select id,interface_name,interface_type from interface_settings";
    const char *sql_char = sql.c_str();
    mysql_query(conn,sql_char);
   
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

Php::Value UpdateExportSettings(Php::Parameters &params)
{
    int result=0;
    sql::Connection *con;
    try{

  
    con = DBConnection();
    int id = params[0];
    int export_flag = params[1];
    int ssh = params[2];
    int ftp = params[3];
    int ssh_interface_id = params[4];
    int ftp_interface_id = params[5];
       
    prep = con->prepareStatement("Update DataExportSettings set export_flag=?,ssh=?,ftp=?,ssh_interface_id=?,ftp_interface_id=? where id = ?");
        
   
    prep->setInt(1,export_flag);
    prep->setInt(2,ssh);
    prep->setInt(3,ftp);
    prep->setInt(4,ssh_interface_id);
    prep->setInt(5,ftp_interface_id);
    prep->setInt(6,id);

    if(prep->executeUpdate())
    {
        result = 1;
    }
    //prep->close();
    con->close();
    }
    catch (sql::SQLException e)
    {
        Php::out<<e.what()<<endl;
    }
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
        static Php::Extension extension("PX_ExportSettings", "1.0");
        extension.add<GetExport>("GetExport");
        extension.add<GetInterfaceName>("GetInterfaceName");
        extension.add<SelectInterfaces>("SelectInterfaces");
        extension.add<UpdateExportSettings>("UpdateExportSettings");
               // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
