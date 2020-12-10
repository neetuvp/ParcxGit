#include "PX_GeneralOperations.h"
#include "PX_ContractParking.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
GeneralOperations General;
string query;
string current_date_time;   
sql::ResultSet *res,*res2;  
size_t n;
int i=0;

sql::Connection *rCon,*con;
sql::Statement *rStmt,*stmt;


void writeLog(string function,string message)
    {
    General.writeLog("Services/ApplicationLogs/PX-ContractParking-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("Services/ExceptionLogs/PX-ContractParking-"+General.currentDateTime("%Y-%m-%d"),function,message); 
	writeLog(function,"Exception: "+message);   
    }


inline string toString(Php::Value param)
	{
	string value=param;
	return value;
	}

Php::Value ContractParking::getPuchasedProductDetails(Php::Value json)
    {
    Php::Value response; 
    Php::Value row;   
    try
        { 
        response["available"]="false";        
        string ticket_id=json["ticket_id"];
        string plate_number=json["plate_number"];
        string name=json["customer_name"];
        string tag=json["tag"];
        string mobile=json["mobile"];
        string condition,column;

        
        
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();        
        if(ticket_id.length()>0)
            {
            condition="ticket_id like '%"+ticket_id+"%'";
            column="ticket_id";
            }
        else if(plate_number.length()>0)
            {
            condition="plate_number like '%"+plate_number+"%'";
            column="plate_number";
            }
        else if(name.length()>0)
            {
            condition="customer_name like '%"+name+"%'";
            column="customer_name";
            }
        else if(tag.length()>0)
            {
            condition="tag like '%"+tag+"%'";
            column="tag";
            }
        else if(mobile.length()>0)
            {
            condition="customer_mobile like '%"+mobile+"%'";   
            column="customer_mobile";     
            }

        query="SELECT p.* FROM contract_parking_transactions p INNER JOIN( SELECT "+column+", MAX(id) AS latest FROM contract_parking_transactions where "+condition+" GROUP BY "+column+") AS groupedp ON  groupedp."+column+" = p."+column+" AND groupedp.latest = p.id";
        //cout<<query<<endl;        
        res= rStmt->executeQuery(query);   
        i=0;   
        if(res->rowsCount()>0)
            response["available"]="true";   

        Php::Value purachase;
        while(res->next())
            {            
            row["id"]=string(res->getString("id"));    

            row["customer_name"]=string(res->getString("customer_name"));
            row["customer_mobile"]=string(res->getString("customer_mobile"));
            row["company_name"]=string(res->getString("company_name"));
            row["customer_email"]=string(res->getString("customer_email"));
            row["description"]=string(res->getString("description"));

            row["ticket_id"]=string(res->getString("ticket_id"));
            row["tag"]=string(res->getString("tag"));
            row["plate_number"]=string(res->getString("plate_number"));

            row["product_name"]=string(res->getString("product_name"));
            row["product_price"]=string(res->getString("product_price"));
            row["validity_days"]=string(res->getString("validity_days"));				
            row["product_id"]=string(res->getString("product_id"));
            row["carpark"]=string(res->getString("carpark_number"));
            row["validity_start_date"]=string(res->getString("validity_start_date"));
            row["validity_end_date"]=string(res->getString("validity_end_date"));
            row["purchased_date_time"]=string(res->getString("date_time"));
            purachase[i]=row;
            i++;    
            }

        response["subscription"]=purachase;

        delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
        }
    catch(const std::exception& e)
        {
        writeException("getPuchasedProductDetails",e.what());        
        }
    return response;
    }

Php::Value ContractParking::verifyPurchase(Php::Value json)
    {
    Php::Value response; 
    Php::Value row;   
    try
        { 
        response["available"]="false";                
        string plate_number=json["plate_number"];       
        string tag=json["tag"];        
        bool flag=false;
        string condition,column;

        
        
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();  

        if(plate_number.length()>0)
            {
            condition="plate_number='"+plate_number+"'";
            column="plate_number";
            flag=true;
            }
        if(tag.length()>0)
            {
            if(flag==true)
                {
                condition=condition+" or tag='"+tag+"'";
                column=column+",tag";
                }   
            else
                {
                condition="tag='"+tag+"'";
                column="tag";
                }
            
            }

        query="SELECT t1.* FROM contract_parking_transactions t1 JOIN (SELECT  MAX(id) as id  FROM contract_parking_transactions where  "+condition+" GROUP BY "+column+") t2 ON t1.id = t2.id"; 

       
                      
        res= rStmt->executeQuery(query);   
        i=0;   
        if(res->rowsCount()>0)
            response["available"]="true";   

        Php::Value purachase;
        while(res->next())
            {            
            row["id"]=string(res->getString("id"));    

            row["customer_name"]=string(res->getString("customer_name"));
            row["customer_mobile"]=string(res->getString("customer_mobile"));
            row["company_name"]=string(res->getString("company_name"));
            row["customer_email"]=string(res->getString("customer_email"));
            row["description"]=string(res->getString("description"));

            row["ticket_id"]=string(res->getString("ticket_id"));
            row["tag"]=string(res->getString("tag"));
            row["plate_number"]=string(res->getString("plate_number"));

            row["product_name"]=string(res->getString("product_name"));
            row["product_price"]=string(res->getString("product_price"));
            row["validity_days"]=string(res->getString("validity_days"));				
            row["product_id"]=string(res->getString("product_id"));
            row["carpark"]=string(res->getString("carpark_number"));
            row["validity_start_date"]=string(res->getString("validity_start_date"));
            row["validity_end_date"]=string(res->getString("validity_end_date"));
            row["purchased_date_time"]=string(res->getString("date_time"));
            purachase[i]=row;
            i++;    
            }

        response["subscription"]=purachase;

        delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
        }
    catch(const std::exception& e)
        {
        writeException("getPuchasedProductDetails",e.what());        
        }
    return response;
    }

Php::Value ContractParking::getProductDetails() 
	{	
	Php::Value response;
	Php::Value row;
	i=0;
	try
		{
        response["available"]="false";    
		rCon=General.mysqlConnect(ServerDB);
        rStmt=rCon->createStatement();								         
        query="select * from system_products where CURRENT_DATE between start_date and expiry_date and product_type='contract_parking'";
        i=0;    
        res= rStmt->executeQuery(query);   
        if(res->rowsCount()>0)
            response["available"]="true";   
        Php::Value subscription; 
        while(res->next())            
            {
            row["product_name"]=string(res->getString("product_name"));
            row["product_price"]=string(res->getString("product_price"));
            row["validity_days"]=string(res->getString("product_value"));				
            row["product_id"]=string(res->getString("product_id"));				            
            subscription[i]=row;
            i++;
            }
        response["subscription"]=subscription;
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;			
		}
	catch (exception &e)
		{			
		writeException("getProductDetails", e.what());			
		}
	return response;
	} 

Php::Value ContractParking::addProductSubscription(Php::Value json) 
	{	
	Php::Value response;	
    response["result"]="failed";	
	try
		{
        string name=json["customer_name"];        
        string mobile=json["mobile"];
        string email=json["email"];
        string company_name=json["company_name"];
        string description=json["description"];
        string plate_number=json["plate_number"];
        string tag=json["tag"];
        string ticket_id=json["ticket_id"];
        string product_id=json["product_id"];
        

		rCon=General.mysqlConnect(ServerDB);
        rStmt=rCon->createStatement();	       

        query="select * from system_products where product_id="+product_id;
        //cout<<query<<endl;
        res= rStmt->executeQuery(query);   
        if(res->next())
            {
            string product_name=res->getString("product_name");
            //string product_type=res->getString("product_type");
            string product_price=res->getString("product_price");
            string validity_days=res->getString("product_value");
            string carpark_number=res->getString("carparks");
            string facility_number=res->getString("facility_number");     
            
            
            query="insert into parcx_reporting.contract_parking_transactions(product_id,product_name,product_price,carpark_number,facility_number,validity_days,validity_start_date,validity_end_date,customer_name,company_name,customer_email,customer_mobile,description,plate_number,tag,ticket_id,date_time)values("+product_id+",'"+product_name+"','"+product_price+"','"+carpark_number+"','"+facility_number+"','"+validity_days+"',CURRENT_DATE() + interval 1 day,CURRENT_DATE() + interval "+validity_days+"+1 day,'"+name+"','"+company_name+"','"+email+"','"+mobile+"','"+description+"','"+plate_number+"','"+tag+"','"+ticket_id+"',current_timestamp())";
            //cout<<query<<endl;
            rStmt->executeUpdate(query);   
            query="insert into access_whitelist(facility_number,carpark_number,access_zones,ticket_id,plate_number,tag,validity_start_date,validity_expiry_date,customer_name,status)values('"+facility_number+"','"+carpark_number+"','','"+ticket_id+"','"+plate_number+"','"+tag+"',CURRENT_DATE() + interval 1 day,CURRENT_DATE() + interval "+validity_days+"+1 day,'"+name+"',1)";
            //cout<<query<<endl;
            rStmt->executeUpdate(query);   
           
            }               
		delete rStmt;       
        rCon->close();
        delete rCon;	
        response["result"]="success";			
		}
	catch (exception &e)
		{			
		writeException("addProductSubscription", e.what());	
        writeException("addProductSubscription", query);			
		}
	return response;
	} 

Php::Value ContractParking::renewProductSubscription(Php::Value json) 
	{	
	Php::Value response;	
    response["result"]="failed";	
	try
		{
        string id=json["purchase_id"]; 
        string product_id=json["product_id"];            
        
		rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();	       

        query="select * from contract_parking_transactions where id="+id;       
        res= rStmt->executeQuery(query);   
        if(res->next())
            {                       
            string name=res->getString("customer_name"); 
            string mobile=res->getString("customer_mobile"); 
            string email=res->getString("customer_email"); 
            string company_name=res->getString("company_name");
            string description=res->getString("description"); 
            string plate_number=res->getString("plate_number"); 
            string tag=res->getString("tag"); 
            string ticket_id=res->getString("ticket_id"); 

            query="select * from parcx_server.system_products where product_id="+product_id;      
            res= rStmt->executeQuery(query);   
            res->next();                
            string product_name=res->getString("product_name");
            //string product_type=res->getString("product_type");
            string product_price=res->getString("product_price");
            string validity_days=res->getString("product_value");
            string carpark_number=res->getString("carparks");
            string facility_number=res->getString("facility_number");                                           
            
            query="insert into contract_parking_transactions(product_id,product_name,product_price,carpark_number,facility_number,validity_days,validity_start_date,validity_end_date,customer_name,company_name,customer_email,customer_mobile,description,plate_number,tag,ticket_id,date_time)values("+product_id+",'"+product_name+"','"+product_price+"','','"+facility_number+"','"+validity_days+"',CURRENT_DATE() + interval 1 day,CURRENT_DATE() + interval "+validity_days+"+1 day,'"+name+"','"+company_name+"','"+email+"','"+mobile+"','"+description+"','"+plate_number+"','"+tag+"','"+ticket_id+"',current_timestamp())";           
            rStmt->executeUpdate(query);   
            string current_date=General.currentDateTime("%Y-%m-%d");
            query="select * from parcx_server.access_whitelist where plate_number='"+plate_number+"' and tag='"+tag+"' and ticket_id='"+ticket_id+"'";
            res= rStmt->executeQuery(query);   
            if(res->next())
                {
                string expiry= res->getString("validity_expiry_date");   
                id=res->getString("access_whitelist_id");
                if(expiry.compare(current_date)>0)    
                    query="update parcx_server.access_whitelist set validity_start_date= CURRENT_DATE() + interval 1 day,validity_expiry_date=validity_expiry_date + interval "+validity_days+" day where access_whitelist_id="+id;           
                else                
                    query="update parcx_server.access_whitelist set validity_start_date= CURRENT_DATE() + interval 1 day,validity_expiry_date=CURRENT_DATE() + interval "+validity_days+"+1 day where access_whitelist_id="+id;           
                rStmt->executeUpdate(query);     
                }                       
            }               
		delete rStmt; 
        delete res;      
        rCon->close();
        delete rCon;	
        response["result"]="success";			
		}
	catch (exception &e)
		{			
		writeException("renewProductSubscription", e.what());	
        //cout<<query<<endl;		
		}
	return response;
	} 
