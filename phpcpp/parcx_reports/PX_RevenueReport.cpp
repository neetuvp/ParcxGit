#include "PX_GeneralOperations.h"
#include "PX_RevenueReport.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
extern GeneralOperations General;
extern string query; 
extern sql::ResultSet *res,*res2;  
extern sql::Connection *rCon,*con;
extern sql::Statement *rStmt,*stmt;
extern int i;


void writeRevenueReportLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-RevenueReport-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeRevenueReportException(string function,string message)
    {
    Php::out<<message<<endl;   
    Php::out<<query<<endl;     
    General.writeLog("WebApplication/ExceptionLogs/PX-RevenueReport-"+General.currentDateTime("%Y-%m-%d"),function,message);        
    }

inline string toString(Php::Value param)
	{
	string value=param;
	return value;
	}

void RevenueReport::paymentTransactions(Php::Value json)
    {
    try
        {        
        string startdate=json["from"];
        string enddate=json["to"];
        string device=json["device"];
        string carpark=json["carpark"];
        string type=json["payment-type"];
        Php::Value category=json["payment-category"];
        string discount=json["discount"];   
        int showvoid=json["showvoid"]; 
        string payment_category;

        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from revenue_payments where payment_date_time between '"+startdate+"' AND '"+enddate+"'"; 
        if(showvoid ==0)    
            query=query+" and (void is null or void='')";      
        else      
            query=query+" and void is not null";

        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            query=query+" AND device_number in("+device+")";

        if(type.length()>0)	           
           query=query+" AND payment_type IN("+type+")";

        if(discount.length()>0)	  		           
             query=query+ " AND discount_id IN("+discount+")";
        
        if(category.length()>0)
            {
            for (auto &iter : category)
                {                                
                if(iter.first==0)
                    query=query+" and (";        
                else
                    query=query+" Or ";
                if(iter.second==1)
                    query=query+" (parking_fee>0 and lost_fee=0 and discount_amount=0)";
                if(iter.second==2)
                    query=query+" lost_fee>0";
                if(iter.second==3)
                    query=query+" discount_amount>0";
                if(iter.second==4)
                    query=query+" (discount_amount=0 AND gross_amount=0)";
                if(iter.second==5)  
                    query=query+" payment_category='ProductSale'";              
                }
            query=query+" )";                
            }

        query=query+" ORDER BY  payment_date_time desc";        
        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            float total=0;
            while(res->next())
                {
                total=total+res->getDouble("gross_amount");    
                }
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<total<<"</h3>"<<endl;
            Php::out<<"<h6>Total Revenue</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Total Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>Ticket Number</th>"<<endl; 
            Php::out<<"<th>Device Name</th>"<<endl;        
            Php::out<<"<th>Payment Time</th>"<<endl;  
            Php::out<<"<th>Parking Duration</th>"<<endl;  
            Php::out<<"<th>Category</th>"<<endl;    
            Php::out<<"<th>Payment Type</th>"<<endl;  
            Php::out<<"<th>Discount Name</th>"<<endl;                                               
            Php::out<<"<th>Discount Amount</th>"<<endl; 
            Php::out<<"<th>Gross Amount</th>"<<endl;                                                        
            
            Php::out<<"</tr>"<<endl; 
            Php::out<<"</thead><tbody>"<<endl; 

            res->beforeFirst();
            i=0;
            while(res->next())
                { 
                i++;    
                if(res->getInt("discount_amount")>0)
                    payment_category="Discount";
                else if(res->getInt("lost_fee")>0)
                    payment_category="Lost";
                else if(res->getInt("discount_amount")==0 &&res->getInt("gross_amount")==0)
                    payment_category="Grace Period";
                else if(res->getString("payment_category")=="ProductSale")
                    payment_category="Product Sale";
                else if(res->getString("payment_category")=="WALLET")                    
                    payment_category="Wallet";  
                else if(res->getString("payment_category")=="Reservation")                    
                    payment_category="Reservation";                   
                else                     
                    payment_category="Parking Fee";

				Php::out<<"<tr class='pointer'  id='payment_detail' payment_id='"<<res->getInt("id")<<"'>"<<endl;                
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                 if(showvoid == 0) 
                    {
                    Php::out<<"<td>" +res->getString("ticket_id")+ " </td>"<<endl;
                    Php::out<<"<td>" +res->getString("device_name")+" </td>"<<endl;
                    } 
                else 
                    {
                    Php::out<<"<td>" +res->getString("ticket_id")+ "<br>Void Reason:" +res->getString("void_reason")+"</td>"<<endl;
                    Php::out<<"<td>" +res->getString("device_name")+" <br>User:" +res->getString("void_user")+"</td>"<<endl;
                    }

                Php::out<<"<td>"+res->getString("payment_date_time")+"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("parking_duration")+"</td>"<<endl;   
                Php::out<<"<td>"+payment_category+"</td>"<<endl;   
                Php::out<<"<td>"+res->getString("payment_type")+"</td>"<<endl;  
                Php::out<<"<td>"+res->getString("discount_name")+"</td>"<<endl;                     
                Php::out<<"<td> "+res->getString("discount_amount")+" </td>"<<endl;                                                             
                Php::out<<"<td>"+res->getString("gross_amount")+"</td>"<<endl;                
				Php::out<<"</tr>"<<endl;                
                }  
            Php::out<<"</tbody></table>"<<endl;
            }
        else                
            {
            Php::out<<"<div class='p-0'>No records/transactions available for the current search criteria</div>"<<endl;
            }
        delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
       
        }
    catch(const std::exception& e)
        {
        writeRevenueReportException("paymentTransactions",e.what());        
        }
    }

