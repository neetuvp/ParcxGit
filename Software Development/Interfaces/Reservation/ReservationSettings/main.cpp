//make -B
//sudo make install
//service apache2 restart

#include <phpcpp.h>
#include<iostream>
#include<stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <dlfcn.h>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <mysql.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<cppconn/prepared_statement.h>

#include "validation.h"

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
#define DATABASE "parcx_reporting"


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
string CurrentDateTime()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);		
    string datetime(buffer);
    return datetime;
}
string CurrentDate()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d-%m-%Y",timeinfo);		
    string datetime(buffer);
    return datetime;
}
void CreateLogcpp(string data)
{
    ofstream f;
    f.open("/opt/parcx/Logs/Services/ApplicationLogs/PX_Reservation_"+CurrentDate()+".log",fstream::app);
    f << CurrentDateTime()<<":";
    f << data<< endl;
}
string GenerateTransactionID()
{
    time_t now=time(NULL);	
    std::tm* timeinfo;
    char buffer[80];
    std::time(&now);
    timeinfo=std::localtime(&now);
    std::strftime(buffer,80,"%d%m%Y%H%M%S",timeinfo);		
    string datetime(buffer);
    return datetime;
}

Php::Value CreateReservation(Php::Value data)
{
    CreateLogcpp("********************************Create********************************");

    Php::Value response;
    Validation m;
    string result="";
    sql::Connection *con;

    string transaction_id =  data["TransactionID"];
   try{  
    con = DBConnection();
    string start_time = data["ReservationStart"];
    string end_time = data["ReservationEnd"];
    string name = data["CustomerName"];
    string email = data["CustomerEmail"];
    string phone = data["CustomerTelephone"];
    string plate_number = data["PlateNumber"];
    string ticket_id = data["TicketID"];
    string zone_id = data["ParkingZoneID"];
    string zone_name = data["ParkingZoneName"];
   //CreateLogcpp("Result:"+to_string(m.DataValidation(email,20,5,1)));
    CreateLogcpp("Transaction ID:"+transaction_id);
    if(m.DataValidation(transaction_id,25,3,1) && m.DataValidation(start_time,25,4,1) && m.DataValidation(end_time,25,4,1) && m.DataValidation(name,50,2,1) && m.DataValidation(email,254,5,1) && m.DataValidation(phone,20,1,1) && m.DataValidation(plate_number,20,3,1) && m.DataValidation(ticket_id,20,3,1) && m.DataValidation(zone_id,20,3,1) && m.DataValidation(zone_name,20,3,1))
    //no email validation due to regex error
	//if(m.DataValidation(transaction_id,25,3,1) && m.DataValidation(start_time,25,4,1) && m.DataValidation(end_time,25,4,1) && m.DataValidation(name,50,2,1) && m.DataValidation(phone,20,1,1) && m.DataValidation(plate_number,20,3,1) && m.DataValidation(ticket_id,20,3,1) && m.DataValidation(zone_id,20,3,1) && m.DataValidation(zone_name,20,3,1))
	{
        CreateLogcpp("Valid Data");
		stmt = con->createStatement();
		rs = stmt->executeQuery("Select reservation_id from parking_reservation where (email = '"+email+"' or ticket_id = '"+ticket_id+"') and ((from_time	 <= '"+start_time+"' and to_time >= '"+end_time+"' and from_time<= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time >= '"+start_time+"' and to_time >= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time >= '"+start_time+"' and to_time <= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time <= '"+start_time+"' and to_time <= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"'))");
		if(rs->next())
		{
			CreateLogcpp("Catch: 701 Unable To Process transaction.Duplicate Entry");
			response["status"] = 701;
			response["message"]= "Unable To Process transaction.Duplicate Entry";
			response["TransactionID"] = transaction_id;
		}
		else{
			prep = con->prepareStatement("Insert IGNORE into parking_reservation(transaction_id,from_time,to_time,name,email,phone,plate_number,ticket_id,parking_bay_number) values(?,?,?,?,?,?,?,?,?)");
        

			prep->setString(1,transaction_id);
			prep->setDateTime(2,start_time);
			prep->setDateTime(3,end_time);
			prep->setString(4,name);
			prep->setString(5,email);
			prep->setString(6,phone);
			prep->setString(7,plate_number);
			prep->setString(8,ticket_id);
			prep->setString(9,zone_id);

			if(prep->executeUpdate())
			{
				CreateLogcpp("Response : 200 Success");
				response["status"] = 200;
				response["message"]= "Success";
				response["TransactionID"] = transaction_id;
				//return response;
			}
			else{
				CreateLogcpp("Response : 701 Unable To Process transaction");
				response["status"] = 701;
				response["message"]= "Unable To Process transaction";
				response["TransactionID"] = transaction_id;
				//return response;
			}
			delete prep;
			
		}
		delete rs;
		delete stmt;
    }
    else
    {
        CreateLogcpp("Invalid Data");
        CreateLogcpp("Response : 702 Invalid Data");
        response["status"] = 702;
        response["message"]= "Invalid Data";
        response["TransactionID"] = transaction_id;
    }
    //prep->close();
    delete con;
    }
    catch (sql::SQLException &e)
    {
        CreateLogcpp(e.what());
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        delete con;
        response["status"] = 701;
        response["message"]= "Unable To Process transaction";
        response["TransactionID"] = transaction_id;
     }
    catch (exception &e)
    {
        CreateLogcpp(e.what());
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        response["status"] = 701;
        response["message"]= "Unable To Process transaction";
        response["TransactionID"] = transaction_id;
     }
    return response;

}

Php::Value UpdateReservation(Php::Value data)
{
    CreateLogcpp("********************************Update********************************");
    Php::Value response;
    Validation m;
    
    sql::Connection *con;
    sql::ResultSet *res;
    string query = "";
    try{  

  
    con = DBConnection();
    stmt = con->createStatement();
    string transaction_id = data["TransactionID"];
    string start_time = data["ReservationStart"];
    string end_time = data["ReservationEnd"];
    string name = data["CustomerName"];
    string email = data["CustomerEmail"];
    string phone = data["CustomerTelephone"];
    string plate_number = data["PlateNumber"];
    string ticket_id = data["TicketID"];
    string zone_id = data["ParkingZoneID"];
    string zone_name = data["ParkingZoneName"];
    CreateLogcpp("Transaction ID:"+transaction_id);
    if(m.DataValidation(transaction_id,25,3,1) && m.DataValidation(start_time,25,4,0) && m.DataValidation(end_time,25,4,0) && m.DataValidation(name,50,2,0) && m.DataValidation(email,254,5,1) && m.DataValidation(phone,20,1,0) && m.DataValidation(plate_number,20,3,1) && m.DataValidation(ticket_id,20,3,1) && m.DataValidation(zone_id,20,3,0) && m.DataValidation(zone_name,20,3,0))
    {
        CreateLogcpp("Valid Data");
        res=stmt->executeQuery("Select * From parking_reservation where ticket_id = '"+ticket_id+"'");
        if(res->next()){
            query = "Update parking_reservation set email='"+email+"',plate_number='"+plate_number+"'";
            if(start_time>"")
                query = query + ",from_time ='"+start_time+"'";
            if(end_time>"")
                query = query + ",to_time='"+end_time+"'";
            if(name>"")
                query = query + ",name='"+name+"'";
            if(phone>"")
                query = query + ",phone='"+phone+"'";
            if(zone_id>"")
                query = query + ",parking_bay_number='"+zone_id+"'";

            query = query + " where ticket_id = '"+ticket_id+"'";
            //int result = stmt->executeUpdate("Update parking_reservation set from_time ='"+start_time+"',to_time='"+end_time+"',name='"+name+"',email='"+email+"',phone='"+phone+"',plate_number='"+plate_number+"',parking_bay_number='"+zone_id+"' where ticket_id = '"+ticket_id+"'");
            int result = stmt->executeUpdate(query);
            if(result >= 0)
            {
                CreateLogcpp("Response : 200 Success");
                response["status"] = 200;
                response["message"]= "Success";
                response["TransactionID"] = transaction_id;
            }
            else
            {
                CreateLogcpp("Catch: 701 Unable To Process transaction");
                response["status"] = 701;
                response["message"]= "Unable To Process transaction";
                response["TransactionID"] = transaction_id;
            }
            
        }
        else
        {
            CreateLogcpp("Response : 704 Transaction does not exist");
            response["status"] = 704;
            response["message"]= "Transaction does not exist";
            response["TransactionID"] = transaction_id;
        }
        delete res;
    }
    else
    {
        CreateLogcpp("Invalid Data");
        CreateLogcpp("Response : 702 Invalid Data");
        response["status"] = 702;
        response["message"]= "Invalid Data";
        response["TransactionID"] = transaction_id;
    }    
    //con->close();
 
    delete stmt;
    delete con;
    }
    catch (exception &e)
    {
        CreateLogcpp(e.what());
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        response["status"] = 701;
        response["message"]= "Unable To Process transaction";
        response["TransactionID"] = data["TransactionID"];
    }
    return response;

}


Php::Value CancelReservation(Php::Value data)
{
    CreateLogcpp("********************************Cancel********************************");
    //Php::Value assoc = params[0];
    Php::Value response;
    Validation m;
    
    sql::Connection *con;
    sql::ResultSet *res;

    try{  
        con = DBConnection();
        stmt = con->createStatement();
        string transaction_id = data["TransactionID"];
        string plate_number = data["PlateNumber"];
        string ticket_id = data["TicketID"];
        string email = data["CustomerEmail"];
        CreateLogcpp("Transaction ID:"+transaction_id);
        if(m.DataValidation(transaction_id,25,3,1) && m.DataValidation(email,254,5,1) && m.DataValidation(plate_number,20,3,1) && m.DataValidation(ticket_id,20,3,1))
        {
            res=stmt->executeQuery("Select * From parking_reservation where ticket_id = '"+ticket_id+"'");
            if(res->next()){
                int result = stmt->executeUpdate("Update parking_reservation set cancel_flag =1 where ticket_id = '"+ticket_id+"'");
                if(result > 0)
                {
                    CreateLogcpp("Response : 200 Success");
                    response["status"] = 200;
                    response["message"]= "Success";
                    response["TransactionID"] = transaction_id;
                }
                else
                {
                    CreateLogcpp("Catch: 701 Unable To Process transaction");
                    response["status"] = 701;
                    response["message"]= "Unable To Process transaction";
                    response["TransactionID"] = transaction_id;
                }

            }
            else
            {
                CreateLogcpp("Response : 704 Transaction does not exist");
                response["status"] = 704;
                response["message"]= "Transaction does not exist";
                response["TransactionID"] = transaction_id;
            }
            delete res;
            //con->close();
        }
        else
        {
            CreateLogcpp("Invalid Data");
            CreateLogcpp("Response : 702 Invalid Data");
            response["status"] = 702;
            response["message"]= "Invalid Data";
            response["TransactionID"] = transaction_id;
        }  
        delete stmt;
        delete con;
  
    }
    catch (exception &e)
    {
        CreateLogcpp(e.what());
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        response["status"] = 701;
        response["message"]= "Unable To Process transaction";
        response["TransactionID"] = data["TransactionID"];
    }
    return response;

}

Php::Value CountReservation(Php::Value data)
{
    CreateLogcpp("********************************Count********************************");
    //Php::Value assoc = params[0];
    Php::Value response;
    Validation m;
    
    sql::Connection *con;
    sql::ResultSet *res;
    string transaction_id = GenerateTransactionID();

    try{  
        con = DBConnection();
        stmt = con->createStatement();
        string zone_id = data["ParkingZoneID"];
        string start_time = data["StartDateTime"];
        string end_time = data["EndDateTime"];
        if(m.DataValidation(start_time,25,4,1) && m.DataValidation(end_time,25,4,1) && m.DataValidation(zone_id,20,3,1))
        {
            string sql = "Select count(reservation_id) as count From parking_reservation where cancel_flag = 0 and ((from_time <= '"+start_time+"' and to_time >= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time >= '"+start_time+"' and to_time >= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time >= '"+start_time+"' and to_time <= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"') or (from_time <= '"+start_time+"' and to_time <= '"+end_time+"' and from_time <= '"+end_time+"' and to_time >= '"+start_time+"')) and parking_bay_number = '"+zone_id+"'";
           // CreateLogcpp(sql);
            res=stmt->executeQuery(sql);
            if(res->next()){
                int result = res->getInt("count");
                CreateLogcpp("Response : 200 Success");
                CreateLogcpp("Count  : "+to_string(result));
                response["status"] = 200;
                response["message"]= "Success";
                response["TransactionID"] = transaction_id;
                response["ParkingZone"] = zone_id;
                response["ReservationCount"] = result;
            }
            else
            {
                CreateLogcpp("Response : 704 No data available");
                response["status"] = 704;
                response["message"]= "No data available";
                response["TransactionID"] = transaction_id;
            }
            delete res;
        }
        else
        {
            CreateLogcpp("Invalid Data");
            CreateLogcpp("Response : 702 Invalid Data");
            response["status"] = 702;
            response["message"]= "Invalid Data";
            response["TransactionID"] = transaction_id;
        }
        
        //con->close();
        delete stmt;
        delete con;
    }
    catch (exception &e)
    {
        CreateLogcpp(e.what());
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        response["status"] = 701;
        response["message"]= "Unable To Process transaction";
        response["TransactionID"] = transaction_id;
    }
    return response;
}
Php::Value SearchReservation(Php::Value data)
{
    CreateLogcpp("********************************Search********************************");
   //Php::Value assoc = params[0];
    Php::Value search;
    Php::Value response;
    Validation m;
    
    sql::Connection *con;
    sql::ResultSet *res;

    //MYSQL *conn;
	//MYSQL_RES *res;
	//MYSQL_ROW row;
	//MYSQL_FIELD *field;
    string transaction_id = data["TransactionID"];
    try{

        string start_time = data["StartDateTime"];
        string end_time = data["EndDateTime"];
        string name = data["CustomerName"];
        string email = data["CustomerEmail"];
        string phone = data["CustomerTelephone"];
        string plate_number = data["PlateNumber"];
        string ticket_id = data["TicketID"];
        string zone_id = data["ParkingZoneID"];
        string zone_name = data["ParkingZoneName"];

        
        con = DBConnection();
        stmt = con->createStatement();
        CreateLogcpp("Result:"+to_string(m.DataValidation(email,20,5,0)));

        if(m.DataValidation(transaction_id,25,3,0) && m.DataValidation(start_time,25,4,0) && m.DataValidation(end_time,25,4,0) && m.DataValidation(name,50,2,0) && m.DataValidation(email,254,5,0) && m.DataValidation(phone,20,1,0) && m.DataValidation(plate_number,20,3,0) && m.DataValidation(ticket_id,20,3,0) && m.DataValidation(zone_id,20,3,0) && m.DataValidation(zone_name,20,3,0))
        {
            string sql = "Select * from parking_reservation where transaction_id > ''";
            if(start_time!="")
            {
                sql = sql + " and from_time>='"+start_time+"'";
            }
            if(end_time!="")
            {
                sql = sql + " and to_time<='"+end_time+"'";
            }
            if(name!="")
            {
                sql = sql + " and name like '%"+name+"%'";
            }
            if(email!="")
            {
                sql = sql + " and email like '%"+email+"%'";
            }
            if(phone!="")
            {
                sql = sql + " and phone like '%"+phone+"'";
            }
            if(ticket_id!="")
            {
                sql = sql + " and ticket_id like '%"+ticket_id+"'";
            }
            if(zone_id!="")
            {
                sql = sql + " and parking_bay_number='"+zone_id+"'";
            }
            res=stmt->executeQuery(sql);
            if(res->next()){
                res->beforeFirst();
                int i = 0;
                while(res->next())
                {
                    CreateLogcpp("hereee"+res->getString("transaction_id"));
                    
                    search["Transaction Id"] = res->getString("transaction_id").c_str();
                    search["Reservation Start Date and Time"] = res->getString("from_time").c_str();
                    search["Reservation End Date and Time"] = res->getString("to_time").c_str();
                    search["Customer Name"] = res->getString("name").c_str();
                    search["CustomerEmail"] = res->getString("email").c_str();
                    search["CustomerTelephone"] = res->getString("phone").c_str();

                    search["Plate Number"] = res->getString("plate_number").c_str();
                    search["Parking Zone Id"] = res->getString("parking_bay_number").c_str();
                    search["Parking Zone Name"] = res->getString("parking_bay_number").c_str();

                    search["Ticket ID"] = res->getString("ticket_id").c_str();
                    if(res->getInt("cancel_flag")==1)
                       search["ReservationStatus"] = 1;
                    else
                        search["ReservationStatus"] = 0;
                    response[i]=search;
                    i++;
                }
            }
            else
            {
                CreateLogcpp("Response : 704 No data available");
                response["status"] = 704;
                response["message"]= "No data available";
                response["TransactionID"] = transaction_id;
            }
            delete res;
            
        }
        else{
            CreateLogcpp("Invalid Data");
            CreateLogcpp("Response : 702 Invalid Data");
            response["status"] = 702;
            response["message"]= "Invalid Data";
            response["TransactionID"] = transaction_id;
        }
        delete stmt;
        delete con;
    }
    catch (exception &e)
    {
        
        CreateLogcpp("Catch: 701 Unable To Process transaction");
        CreateLogcpp(e.what());
        response["status"] = 701;
        response["message"]= "Unable To Process transactionss";
        response["TransactionID"] = transaction_id;
    }
    return response;

}

// Php::Value SearchReservation(Php::Parameters &params)
// {
//    int result=0;
//     MYSQL *conn;
// 	MYSQL_RES *res;
// 	MYSQL_ROW row;
// 	MYSQL_FIELD *field;
//     try{

//     string transaction_id = params[0];
//     string start_time = params[1];
//     string end_time = params[2];
//     string name = params[3];
//     string email = params[4];
//     string phone = params[5];
//     string plate_number = params[6];
//     string ticket_id = params[7];
//     string zone_id = params[8];
//     string zone_name = params[9];

//     string sql = "Select * from parking_reservation where transaction_id > ''";
//     if(transaction_id!="")
//     {
//         sql = sql + " and transaction_id like '%"+transaction_id+"%'";
//     }
//     if(start_time!="")
//     {
//         sql = sql + " and fromTime>='"+start_time+"'";
//     }
//     if(end_time!="")
//     {
//         sql = sql + " and toTime<='"+end_time+"'";
//     }
//     if(name!="")
//     {
//         sql = sql + " and name like '%"+name+"%'";
//     }
//     if(email!="")
//     {
//         sql = sql + " and email like '%"+email+"%'";
//     }
//     if(phone!="")
//     {
//         sql = sql + " and phone like '%"+phone+"'";
//     }
//     if(ticket_id!="")
//     {
//         sql = sql + " and ticket_id like '%"+ticket_id+"'";
//     }
//     if(zone_id!="")
//     {
//         sql = sql + " and parkingSlot='"+zone_id+"'";
//     }
//     CreateLogcpp(sql);
//     conn = mysql_init (NULL);	  
//     if(!conn)
//     {
//         Php::out<<"Mysql Init Failed"<<endl;
//     }  
	
// 	conn = mysql_real_connect(conn,DBServer,DBUsername,DBPassword,DATABASE,0,NULL,0); 
//     if(conn)
//     {
//         mysql_query(conn,(sql.c_str()));
//         res = mysql_store_result(conn);
//         int num_fields = mysql_num_fields(res);
//         Php::Array array;
//         Php::Value rows;
//         int i = 0;

//         while((field = mysql_fetch_field(res))){
//             array[i] = field->name;
//             i++;
//         }

//         int x = 0;
//         while ((row = mysql_fetch_row(res)))
//         {

//             for (int z=0;z<num_fields;z++) 
//             {
//                 std::string fieldname = array[z];
//                 array[fieldname] = row[z];
//                 rows[x]=array;
//             }

//             x++;
//         }
//         if(res != NULL)
//             mysql_free_result(res);
//         return rows;

//     }
//     else{
//         //Php::out<<"Connection Failed"<<endl;
//         return "Failed";
//     }

    
// 	mysql_close (conn);

//     }
//     catch (exception &e)
//     {
//         Php::out<<e.what()<<endl;
//     }
//     return result;

// }

 void CreateLog(Php::Parameters &params)
    {
       ofstream f;
        f.open("ReservationLog.log",fstream::app);
        f << "Date : "<<CurrentDateTime()<<endl;
        f << params[0]<< endl;
        
 
    }

Php::Value parcxReservation(Php::Parameters &params) {
    Php::Value data = params[0];
    int task = data["Action"];
    Php::Value response;
    switch (task) {
        case 1:response = CreateReservation(data);
            break;
        case 2:response = SearchReservation(data);
            break;
        case 3:response = UpdateReservation(data);
            break;
        case 4:response = CountReservation(data);
            break;
        case 5:response = CancelReservation(data);
            break;
        

    }
    return response;
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
        static Php::Extension extension("PX_Reservation", "1.0");
        extension.add<parcxReservation>("parcxReservation");
        
        
        // @todo    add your own functions, classes, namespaces to the extension
        
        // return the extension
        return extension;
    }
}