#include <phpcpp.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>


#include "mysql_connection.h"
#include "mysql_driver.h"


#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/prepared_statement.h>
#include <jsoncpp/json/json.h>


using namespace std;
sql::Connection *server_conn,*reporting_conn,*con;
sql::Driver *driver;
sql::Statement *stmt;
sql::ResultSet *res;
sql::PreparedStatement *pst;
sql::ResultSetMetaData *res_meta;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"


ifstream fp;
char buf[80];

string currentDateTime()
{
   time_t now = time(0);
   struct tm tstruct;
   tstruct = *localtime(&now);
   strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
   return buf;
}

string currentDate()
{
   time_t now = time(0);
   struct tm tstruct;
                                    //char buf[80];
   tstruct = *localtime(&now);
   strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
   return buf;
}

                //write exception
void writeException(string function,string txt)
{
   ofstream fp;
   string  name="/var/log/parcx/Services/ExceptionLogs/GetDataPhpCpp"+currentDate()+".log";
   fp.open(name.c_str(),ios::app);  
                                    //fp.open("/etc/parcx/Logs/Daemons/ApplicationLogs/Discounts.log");
   fp<<currentDateTime()<<" from:"<<function<<endl<<txt<<endl<<endl;
   fp.close();
}

void writeLog(string function,string txt)
{

   cout<<"In write Log"<<endl;
   std::ofstream fp;
   string  name="/var/log/parcx/Services/ApplicationLogs/GetDataPhpCpp"+currentDate()+".log";    

                                    //fp.open("/var/log/parcx/Logs/Daemons/ApplicationLogs/Discounts.txt");
   fp.open(name.c_str(),ios::app);  
   fp<<currentDateTime()<<" from:"<<function<<endl<<txt<<endl<<endl;
   fp.close();  
}

//sql::Connection* connectReportingDB()
//{
//
  //try
  //  {
        /* code */
        /* Create a connection */
        //driver = get_driver_instance();
        //con = driver->connect(DBServer, DBUsername, DBPassword);
        /* Connect to the MySQL test database */
       // con->setSchema(ReportingDB);

    //}
    //catch(const std::exception& e)
    //{
  //      std::cerr << e.what() << '\n';
  //      writeException("connectReportingDB",e.what());
  //  }
//return con;
    
//}*/

sql::Connection* connectServerDB()
{

    try
    {
        /* code */
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(DBServer, DBUsername, DBPassword);
        /* Connect to the MySQL test database */
        con->setSchema(ServerDB);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        writeException("connectServerDB",e.what());
    }
    return con;
    
}




Php::Value getDiscounts(Php::Parameters &params)
{

    Json::Value discounts(Json::arrayValue);
    Json::Value tempDiscount,status;
    string jsonDiscounts;
	Php::Value response;
    writeLog("getDiscounts","in function");

    try
    {
        /* code */
        connectServerDB();
        string carpark_no=params[0];
		writeLog("getDiscounts","Carpark No: "+carpark_no);
		
        string sql="SELECT * from revenue_discounts where carpark_number ='"+carpark_no+"' and status = 1";
        writeLog("getDiscounts","SQL query to get discounts: "+sql);
        stmt = con->createStatement();
        res=stmt->executeQuery(sql);

		int row_count= static_cast<int>(res->rowsCount());
		
        if(res->first()){
			
			  
			  //discounts.append(status);
              writeLog("getDiscounts","Discounts exists "+to_string(row_count));
              
             for(int i=0;i<row_count;i++){
              		int id=res->getInt("id");
               		int discount_id=res->getInt("discount_id");
               	    string discount_name=res->getString("discount_name");
              		string discount_type=res->getString("discount_type");
              		string discount_option=res->getString("discount_option");
					int discount_category=res->getInt("discount_category");
					int discount_value=res->getInt("discount_value");
					int carpark_number=res->getInt("carpark_number");
					int status=res->getInt("status");
					
					tempDiscount["id"]=id;
					tempDiscount["discount_id"]=discount_id;
					tempDiscount["discount_name"]=discount_name;
					tempDiscount["discount_type"]=discount_type;
					tempDiscount["discount_option"]=discount_option;
					tempDiscount["discount_category"]=discount_category;
					tempDiscount["discount_value"]=discount_value;
					tempDiscount["carpark_number"]=carpark_number;
					tempDiscount["status"]=status;
					
					discounts.append(tempDiscount);
				 
				 	res->next();
              }
			status["error"]="false";
			status["discount"]=discounts;
              
        }
        else{
			
			  status["error"]="true";
			  status["discount"]="No Discounts Available";
			  discounts.append(status);
              writeLog("getDiscounts","Discounts doesnt exists");
        }
       
      
        Json::FastWriter fw;
        jsonDiscounts = fw.write(status);
        writeLog("getDiscounts","Discounts :"+jsonDiscounts);
		response=jsonDiscounts;
        
        res->close();
        stmt->close();
        con->close();


        delete res;
        delete stmt;
        delete con;
        

        return response;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        writeException("getDiscounts",e.what());
        return response;
    }
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
        static Php::Extension extension("discounts", "1.0");
          extension.add<getDiscounts>("getDiscounts");
         
        
        // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}
