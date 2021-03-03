#include "PX_GeneralOperations.h"
#include "PX_RevenueReport.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include<vector>
#include <sstream>
#include<iomanip>
#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
extern GeneralOperations General;
string sql_query; 
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
    Php::out<<sql_query<<endl;     
    General.writeLog("WebApplication/ExceptionLogs/PX-RevenueReport-"+General.currentDateTime("%Y-%m-%d"),function,message);        
    }

inline string toString(Php::Value param)
	{
	string value=param;
	return value;
	}

string getLabelRevenue(string label,string lang)
    {
    try
        {
		con = General.mysqlConnect(ServerDB);
		stmt = con->createStatement();
        sql_query= "select "+lang+" from web_application_labels where message='"+label+"'";
		//writeRevenueReportLog("",sql_query);
        res2=stmt->executeQuery(sql_query);
        if(res2->next())
            label=res2->getString(lang);
        delete res2;
		delete stmt;
		delete con;
		//writeRevenueReportLog("",label);
        return label;
        }
    catch(const std::exception& e)
        {
        std::cerr << e.what() << '\n';
		writeRevenueReportException("getLabelRevenue",e.what());
        } 
    return "";       
    }

string getCurrency()
{
	sql::ResultSet *rs;
	string currency="";
    try
	{        
        con=General.mysqlConnect(ServerDB);
        stmt=con->createStatement();
        //rs = stmt->executeQuery("select setting_name,setting_value from system_settings where setting_name in('currency','decimal_places','vat_percentage','facility_name','address_line1','address_line2','vat_id')");
		rs = stmt->executeQuery("select setting_value from system_settings where setting_name ='currency'");
		if(rs->next())
		{
			currency = rs->getString("setting_value");
		}
		delete rs;
        delete stmt;
        delete con;        
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getCurrency",e.what());
		std::cerr << e.what() << '\n';
	}
    return currency;  
}

int getDecimal()
{
	sql::ResultSet *rs;
	int decimal=0;
    try
	{        
        con=General.mysqlConnect(ServerDB);
        stmt=con->createStatement();
        //rs = stmt->executeQuery("select setting_name,setting_value from system_settings where setting_name in('currency','decimal_places','vat_percentage','facility_name','address_line1','address_line2','vat_id')");
		rs = stmt->executeQuery("select setting_value from system_settings where setting_name ='decimal_places'");
		if(rs->next())
		{
			decimal = rs->getInt("setting_value");
		}
		delete rs;
        delete stmt;
        delete con;        
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getDecimal",e.what());
		std::cerr << e.what() << '\n';
	}
    return decimal;  
}

string getVat()
{
	sql::ResultSet *rs;
	string vat="";
    try
	{        
        con=General.mysqlConnect(ServerDB);
        stmt=con->createStatement();
		rs = stmt->executeQuery("select setting_value from system_settings where setting_name ='vat_percentage'");
		if(rs->next())
		{
			vat = rs->getString("setting_value");
		}
		delete rs;
        delete stmt;
        delete con;        
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getVat",e.what());
		std::cerr << e.what() << '\n';
	}
    return vat;  
}

string SetDoublePrecision(double amt)
{
	int precision=getDecimal();
	std::string amt_string="0";
	std::ostringstream streamObj;
	// Set Fixed -Point Notation
	streamObj << std::fixed;
	
	// Set precision to 2 digits
	streamObj << std::setprecision(precision);
	
	//Add double to stream
	streamObj << amt;
	// Get string from output string stream
	amt_string = streamObj.str();
	std::cout << amt_string << std::endl;
	
	return amt_string;
	
}

std::vector<std::string> split(const std::string& s, char delimiter)                                                                                                                       
{                                                                                                                                                                                             
   std::vector<std::string> splits;                                                                                                                                                           
   std::string split;                                                                                                                                                                         
   std::istringstream ss(s);                                                                                                                                                                  
   while (std::getline(ss, split, delimiter))                                                                                                                                                 
   {                                                                                                                                                                                          
      splits.push_back(split);                                                                                                                                                                
   }                                                                                                                                                                                          
   return splits;                                                                                                                                                                             
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
	string lang = json["language"];

        string labels="total_revenue,transactions,ticket_id,device_name,payment_date_time,duration,category,payment_type,discount_name,discount_amount,gross_amount,no_records";
        Php::Value label=General.getLabels(lang,labels);


        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        sql_query="select * from revenue_payments where payment_date_time between '"+startdate+"' AND '"+enddate+"'"; 
        if(showvoid ==0)    
            sql_query=sql_query+" and (void is null or void='')";      
        else      
            sql_query=sql_query+" and void is not null";

        if(carpark.length()>0)
            sql_query=sql_query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            sql_query=sql_query+" AND device_number in("+device+")";

        if(type.length()>0)	           
           sql_query=sql_query+" AND payment_type IN("+type+")";

        if(discount.length()>0)	  		           
             sql_query=sql_query+ " AND discount_id IN("+discount+")";
        
        if(category.length()>0)
            {
            for (auto &iter : category)
                {                                
                if(iter.first==0)
                    sql_query=sql_query+" and (";        
                else
                    sql_query=sql_query+" Or ";
                if(iter.second==1)
                    sql_query=sql_query+" (parking_fee>0 and lost_fee=0 and discount_amount=0)";
                if(iter.second==2)
                    sql_query=sql_query+" lost_fee>0";
                if(iter.second==3)
                    sql_query=sql_query+" discount_amount>0";
                if(iter.second==4)
                    sql_query=sql_query+" (discount_amount=0 AND gross_amount=0)";
                if(iter.second==5)  
                    sql_query=sql_query+" payment_category='ProductSale'";              
                }
            sql_query=sql_query+" )";                
            }

        sql_query=sql_query+" ORDER BY  payment_date_time desc";        
        res= rStmt->executeQuery(sql_query);
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
            Php::out<<"<h6>"+toString(label["total_revenue"])+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"+toString(label["transactions"])+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"+toString(label["ticket_id"])+"</th>"<<endl; 
            Php::out<<"<th>"+toString(label["device_name"])+"</th>"<<endl;        
            Php::out<<"<th>"+toString(label["payment_date_time"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["duration"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["category"])+"</th>"<<endl;    
            Php::out<<"<th>"+toString(label["payment_type"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["discount_name"])+"</th>"<<endl;                                               
            Php::out<<"<th>"+toString(label["discount_amount"])+"</th>"<<endl; 
            Php::out<<"<th>"+toString(label["gross_amount"])+"</th>"<<endl;                                                        
            
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
            Php::out<<"<div class='p-0'>"+toString(label["no_records"])+"</div>"<<endl;
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

void RevenueReport::vatReport(Php::Value json)
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
	string lang = json["language"];


        string labels="total_revenue,total_vat,transactions,ticket_id,device_name,payment_date_time,duration,category,payment_type,discount_name,vat_amount,discount_amount,gross_amount,no_records";
        Php::Value label=General.getLabels(lang,labels);
   
        

        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        sql_query="select * from revenue_payments where payment_date_time between '"+startdate+"' AND '"+enddate+"'"; 
        if(showvoid ==0)    
            sql_query=sql_query+" and (void is null or void='')";      
        else      
            sql_query=sql_query+" and void is not null";

        if(carpark.length()>0)
            sql_query=sql_query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            sql_query=sql_query+" AND device_number in("+device+")";

        if(type.length()>0)	           
           sql_query=sql_query+" AND payment_type IN("+type+")";

        if(discount.length()>0)	  		           
             sql_query=sql_query+ " AND discount_id IN("+discount+")";
        
        if(category.length()>0)
            {
            for (auto &iter : category)
                {                                
                if(iter.first==0)
                    sql_query=sql_query+" and (";        
                else
                    sql_query=sql_query+" Or ";
                if(iter.second==1)
                    sql_query=sql_query+" (parking_fee>0 and lost_fee=0 and discount_amount=0)";
                if(iter.second==2)
                    sql_query=sql_query+" lost_fee>0";
                if(iter.second==3)
                    sql_query=sql_query+" discount_amount>0";
                if(iter.second==4)
                    sql_query=sql_query+" (discount_amount=0 AND gross_amount=0)";
                if(iter.second==5)  
                    sql_query=sql_query+" payment_category='ProductSale'";              
                }
            sql_query=sql_query+" )";                
            }

        sql_query=sql_query+" ORDER BY  payment_date_time desc";        
        res= rStmt->executeQuery(sql_query);
        if(res->rowsCount()>0)
            {
            float total=0;
            float vat=0;
            while(res->next())
                {
                total=total+res->getDouble("gross_amount");    
                vat=vat+res->getDouble("vat_amount");
                }
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<total<<"</h3>"<<endl;
            Php::out<<"<h6>"+toString(label["total_revenue"])+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
                        
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<vat<<"</h3>"<<endl;
            Php::out<<"<h6>"+toString(label["total_vat"])+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"+toString(label["transactions"])+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"+toString(label["ticket_id"])+"</th>"<<endl; 
            Php::out<<"<th>"+toString(label["device_name"])+"</th>"<<endl;        
            Php::out<<"<th>"+toString(label["payment_date_time"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["duration"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["category"])+"</th>"<<endl;    
            Php::out<<"<th>"+toString(label["payment_type"])+"</th>"<<endl;  
            Php::out<<"<th>"+toString(label["discount_name"])+"</th>"<<endl;    
            Php::out<<"<th>"+toString(label["vat_amount"])+"</th>"<<endl; 
            Php::out<<"<th>"+toString(label["discount_amount"])+"</th>"<<endl; 
            Php::out<<"<th>"+toString(label["gross_amount"])+"</th>"<<endl;                                                                                                             
            
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
                Php::out<<"<td> "+res->getString("vat_amount")+" </td>"<<endl;
                Php::out<<"<td> "+res->getString("discount_amount")+" </td>"<<endl;
                Php::out<<"<td>"+res->getString("gross_amount")+"</td>"<<endl;                
		Php::out<<"</tr>"<<endl;                
                }  
            Php::out<<"</tbody></table>"<<endl;
            }
        else                
            {
            Php::out<<"<div class='p-0'>"+toString(label["no_records"])+"</div>"<<endl;
            }
        delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
       
        }
    catch(const std::exception& e)
        {
        writeRevenueReportException("vatReport",e.what());        
        }
    }

Php::Value getFacilityfeatures()
{
	sql::ResultSet *rs;
	Php::Value arr;
    try
	{        
        con=General.mysqlConnect(ServerDB);
        stmt=con->createStatement();
		rs = stmt->executeQuery("select setting_name,setting_value from system_settings where setting_name in('currency','decimal_places','vat_percentage','facility_name','address_line1','address_line2','vat_id')");
		while(rs->next())
		{
			arr[rs->getString("setting_name")] = rs->getString("setting_value").asStdString();
		}
		delete rs;
        delete stmt;
        delete con;        
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getVat",e.what());
		std::cerr << e.what() << '\n';
	}
    return arr;  
}



string str_replace_string(string str,string find_str,string replace_str)
{
	int length = str.size();
	int x = 0;
	string str_result;

	while (x != length) {
		string letter = str.substr(x, 1);
		if (letter == find_str) {
		   letter = replace_str;
		   }
		str_result = str_result + letter;
		x = x + 1;
	}

	return str_result;
}

void RevenueReport::getPaymentDetails(Php::Value json)
{
	Php::Value arr;
    try
	{        
		string id=json["payment_id"];
		string vat=getVat();
		string currency=getCurrency();
		string carpark=json["carpark"];
		string lang = json["language"];
		arr = getFacilityfeatures();
		string html_data = "";

        string labels="ticket_id,device_name,parking_rate,entry_grace_period,exit_grace_period,vat,entry_date_time,no_records,payment_date_time,maximum_exit_date_time,duration,category,validations,parking_fee,lost_fee,discounts,discount_name,gross_amount,validation_hours,amount_received,balance_returned,credit_note,banknotes,pdf_receipt";
        Php::Value label=General.getLabels(lang,labels);


		rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        sql_query="select * from revenue_payments where id= '"+id+"'"; 
        res= rStmt->executeQuery(sql_query);
		arr = getFacilityfeatures();
        if(res->next())
        {
            html_data+="<tr><td>"+toString(label["ticket_id"])+"</td><td> "+res->getString("ticket_id")+" </td></tr>";  
			html_data+="<tr><td>"+toString(label["device_name"])+"</td><td> "+res->getString("device_name")+" </td></tr>";  
			html_data+="<tr><td>"+toString(label["parking_rate"])+"</td><td>"+res->getString("parking_rate_label")+"("+res->getString("parking_rate_name")+")</td></tr>";    
			html_data+="<tr><td>"+toString(label["entry_grace_period"])+"</td><td>"+res->getString("entry_grace_period")+"</td></tr>";
			html_data+="<tr><td>"+toString(label["exit_grace_period"])+"</td><td>"+res->getString("exit_grace_period")+"</td></tr>";
			if(stoi(vat)!=0)
				html_data+="<tr><td>"+toString(label["vat"])+"</td><td>"+res->getString("vat_percentage")+"% "+res->getString("vat_type")+"</td></tr>";
			html_data+="<tr><td>"+toString(label["entry_date_time"])+"</td><td>"+res->getString("entry_date_time")+"</td></tr>";        
			html_data+="<tr><td>"+toString(label["payment_date_time"])+"</td><td>"+res->getString("payment_date_time")+"</td></tr>";
			html_data+="<tr><td>"+toString(label["maximum_exit_date_time"])+"</td><td>"+res->getString("max_exit_date_time")+"</td></tr>";        
			html_data+="<tr><td>"+toString(label["duration"])+"</td><td>"+res->getString("parking_duration")+"</td></tr>";
			html_data+="<tr><td>"+toString(label["category"])+"</td><td>"+res->getString("payment_category")+"</td></tr>"; 
			
			if(res->getString("validation_id")!="")
			{
				std::vector<std::string> outputArr = split (res->getString("validation_value"),','); 
				//html_data+="<tr><td>Validations</td><td>";           
				html_data+="<tr><td>"+toString(label["validations"])+"</td><td>"+outputArr[0]+" Hour "+outputArr[1]+" %</td></tr>";                  
			}
			html_data+="<tr><td>"+toString(label["parking_fee"])+"</td><td>"+SetDoublePrecision(res->getDouble("parking_fee"))+" "+currency+"</td></tr>";  
			html_data+="<tr><td>"+toString(label["lost_fee"])+"</td><td>"+SetDoublePrecision(res->getDouble("lost_fee"))+" "+currency+"</td></tr>"; 
			if(stoi(vat)!=0) 
				html_data+="<tr><td>"+toString(label["vat"])+"</td><td>"+SetDoublePrecision(res->getDouble("vat_amount"))+" "+currency+"</td></tr>"; 

			html_data+="<tr><td>"+toString(label["discounts"])+"</td><td>"+SetDoublePrecision(res->getDouble("discount_amount"))+" "+currency+"</td></tr>"; 		
			if(res->getDouble("discount_amount")>0)
				html_data+="<tr><td>"+toString(label["discount_name"])+"</td><td>"+res->getString("discount_name")+"</td></tr>";
			html_data+="<tr><td>"+toString(label["gross_amount"])+"</td><td class='font-weight-bold'>"+SetDoublePrecision(res->getDouble("gross_amount"))+" "+currency+"</td></tr>";          
			if(stoi(res->getString("validation_hours"))>0)
				html_data+="<tr><td>"+toString(label["validation_hours"])+"</td><td>"+res->getString("validation_hours")+"</td></tr>";                                
									
			/*notes=data['bank_notes'];
			total=0;
			if(strlen(notes)>0)
				{
				notes=substr(notes,0,-1);				
				notes=str_replace(";",":",notes);
				array=explode(":",notes);
				notes="";
				
				for(i=0;i<count(array);i+=2)
					{
					a=array[i];
					b=array[i+1];
					sum=a." ".currency." x ".b." = ".(a*b)." ".currency;
					if(count(array)>0)
						notes=sum."<br>".notes;
					total+=(a*b);
					}	
				}
				*/
			int total=0;
			string notes="",sum="",a="",b="";
			notes=res->getString("bank_notes");
			notes = notes.substr(0, notes.size()-1);
			notes=str_replace_string(notes,";",",");
			std::vector<std::string> outputArr =split(notes,',');
			for ( int k=0;k<(signed)outputArr.size();k+=2){
				a = outputArr[k];
				b = outputArr[k+1];
				sum = a +" "+currency+" x "+b+"="+to_string((stoi(a)*stoi(b)))+" "+currency;
				//Php::out<<sum<<"\n";
				if(outputArr.size()>0)
					notes=sum+"<br>"+notes;
				total+= stoi(a)*stoi(b);
			
			}
			
			if(notes!="")    
				html_data+="<tr><td>"+toString(label["amount_received"])+"</td><td>"+to_string(total)+"  "+currency+"</td></tr>";		
			html_data+="<tr><td>"+toString(label["balance_returned"])+"</td><td>"+SetDoublePrecision(res->getDouble("balance_returned"))+" "+currency+" </td></tr>";
			html_data+="<tr><td>"+toString(label["credit_note"])+"</td><td>"+SetDoublePrecision(res->getDouble("credit_note"))+" "+currency+" </td></tr>";		
			if(notes!="")
				html_data+="<tr><td>"+toString(label["banknotes"])+"</td><td>"+notes+"</td></tr>";
			html_data+= "<tr><td><input class='btn btn-info btn-show-pdf-reciept' value='"+toString(label["pdf_receipt"])+"' type='button' data-id='"+res->getString("id")+"'></td></tr>";
			
			Php::out<<html_data<<endl;
		}

	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getPaymentDetails",e.what());        
	}
}

void RevenueReport::getPdfReceipt(Php::Value json)
{
	Php::Value arr;
    try
	{        
		string id=json["payment_id"];
		string vat=getVat();
		string currency=getCurrency();
		string carpark=json["carpark"];
		string lang = json["language"];
		arr = getFacilityfeatures();
		string html_data = "";

        string labels="trn_no,device_name,operator_name,payment_type,ticket_id,vat,entry_date_time,payment_date_time,duration,parking_duration,validations,parking_fee,payment_date_time,maximum_exit_date_time,duration,category,validations,parking_fee,lost_fee,discounts,discount_name,gross_amount,validation_hours,amount_received,balance_returned,credit_note,banknotes,pdf_receipt";
        Php::Value label=General.getLabels(lang,labels);


		rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        sql_query="select * from revenue_payments where id= '"+id+"'"; 
        res= rStmt->executeQuery(sql_query);
		arr = getFacilityfeatures();
        if(res->next())
        {
            html_data+="<div class='row mb-4'><div class='col text-center'>";    
			html_data+="<div>"+toString(arr["facility_name"])+"</div>"; 
			html_data+="<div>"+toString(arr["address_line1"])+"</div>"; 
			html_data+="<div>"+toString(arr["address_line2"])+"</div>"; 
			if(vat!="0")
				html_data+="<div>"+toString(label["trn_no"])+" "+toString(arr["vat_id"])+"</div>";                    
			
			html_data+="<hr><div>"+toString(label["device_name"])+": "+res->getString("device_name")+"</div>";
			html_data+="<div>"+toString(label["operator_name"])+": "+res->getString("operator_name")+"</span></div>";
			html_data+="<div>"+toString(label["payment_type"])+": "+res->getString("payment_type")+"</div></div></div>";
			html_data+="<div class='table-responsive-sm'>";
			html_data+="<table class='table table-striped table-receipt'>";
			html_data+="<thead><tr><th></th><th></th><th></th></tr></thead>";
			html_data+="<tbody><tr><td></td><td>";    
			html_data+="<div>"+res->getString("payment_category")+"</div>";
			html_data+="<span>"+toString(label["ticket_id"])+": "+res->getString("ticket_id")+"</span>";
			html_data+="<span>"+toString(label["entry_date_time"])+": "+res->getString("entry_date_time")+"</span>";
			html_data+="<span>"+toString(label["payment_date_time"])+": "+res->getString("payment_date_time")+"</span> ";   
			html_data+="</td>";
			html_data+="<td></td></tr>";
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+toString(label["duration"])+"</strong></td>";
			html_data+="<td><span>"+res->getString("parking_duration")+"</span></td></tr>";
			
			if(res->getString("validation_id")!="")
			{
				std::vector<std::string> outputArr = split (res->getString("validation_value"),','); 
				html_data+="<tr><td></td>";           
				html_data+="<td><strong>"+toString(label["validations"])+"</strong></td>";
				html_data+="<td><span>"+outputArr[0]+" H "+outputArr[1]+" %</span></td></tr>";              
			}
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+toString(label["parking_fee"])+"</strong></td>";
			html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("parking_fee"))+"</span></td></tr>";
			if(res->getDouble("discount_amount")>0)
			{
				html_data+="<tr><td></td>";
				html_data+="<td><strong>"+toString(label["discount_amount"])+"</strong><span>"+to_string(res->getDouble("discount_name"))+"</span></td>";
				html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("discount_amount"))+"</span></td></tr>";
			}
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+toString(label["net_amount"])+"</strong></td>";
			double net=res->getDouble("gross_amount")-res->getDouble("vat_amount");
			html_data+="<td><span>"+currency+" "+SetDoublePrecision(net)+"</span></td></tr>";
			if(vat!="0")
			{
				html_data+="<tr><td></td>";
				html_data+="<td><strong>"+toString(label["vat"])+" ("+vat+"%)</strong></td>";
				html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("vat_amount"))+"</span></td></tr>";
			}
			
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+toString(label["gross_amount"])+"</strong></td>";
			html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("gross_amount"))+"</span></td></tr>";
			html_data+="</tbody></table> ";
			
			Php::out<<html_data<<endl;
		}

	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("getPdfReceipt",e.what());        
	}
}
	
void RevenueReport::shiftReport(Php::Value json)
    {
        string difference="";
    try
        {        
        string startdate=json["from"];
        string enddate=json["to"];
        string device=json["device"];
        string carpark=json["carpark"];
        string operator_id=json["operator"];
        string lang = json["language"];

        string labels="transactions,device_name,operator_name,shift_number,shift_start,shift_end,shift_earnings,over_short,last_updated,status,no_records";
        Php::Value label=General.getLabels(lang,labels);


        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        sql_query="select * from revenue_shifts where login_time between '"+startdate+"' AND '"+enddate+"'"; 

        if(carpark.length()>0)
            sql_query=sql_query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            sql_query=sql_query+" AND device_number in("+device+")";
		if(operator_id.length()>0)
            sql_query=sql_query+" AND operator_id in("+operator_id+")";

        

        sql_query=sql_query+" ORDER BY  device_number ASC"; 
			
        res= rStmt->executeQuery(sql_query);
        if(res->rowsCount()>0)
            {
            difference="";
            
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"<<toString(label["transactions"])<<"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"<<toString(label["shift_number"])<<"</th>"<<endl; 
            Php::out<<"<th>"<<toString(label["device_name"])<<"</th>"<<endl;        
            Php::out<<"<th>"<<toString(label["operator_name"])<<"</th>"<<endl;  
            Php::out<<"<th>"<<toString(label["shift_start"])<<"</th>"<<endl;  
            Php::out<<"<th>"<<toString(label["shift_end"])<<"</th>"<<endl;    
            Php::out<<"<th>"<<toString(label["shift_earnings"])<<"</th>"<<endl;  
            Php::out<<"<th>"<<toString(label["over_short"])<<"</th>"<<endl;                                               
            Php::out<<"<th>"<<toString(label["last_updated"])<<"</th>"<<endl; 
            Php::out<<"<th>"<<toString(label["status"])<<"</th>"<<endl;                                                        
            
            Php::out<<"</tr>"<<endl; 
            Php::out<<"</thead><tbody>"<<endl; 

            i=0;
            while(res->next())
                { 
                i++;    
               
				if(res->getDouble("physical_cash_collected")>0)                    
                    difference=to_string(res->getDouble("physical_cash_collected")- res->getDouble("shift_earnings"));                
                else                    
                    difference="-"; 
				Php::out<<"<tr class='pointer' data-toggle='modal' data-target='#detailModal' id='shift_detail' shift_id='"+res->getString("shift_id")+"'>"<<endl;                
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                 
                Php::out<<"<td>"+res->getString("shift_id")+"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("device_name")+"</td>"<<endl;   
                Php::out<<"<td>"+res->getString("operator_name")+"</td>"<<endl;  
                Php::out<<"<td>"+res->getString("login_time")+"</td>"<<endl;                     
                Php::out<<"<td> "+res->getString("logout_time")+" </td>"<<endl;                                                             
                Php::out<<"<td>"+res->getString("shift_earnings")+"</td>"<<endl;   
				Php::out<<"<td>"+(difference)+"</td>"<<endl; 
				Php::out<<"<td>"+res->getString("last_updated_date_time")+"</td>"<<endl; 
				if(res->getString("shift_status") == "1")                 
                    Php::out<<"<td><span class='badge bg-danger shift_status_closed'>Closed</span></td>"<<endl;                              
                else 
                    Php::out<<"<td><span class='badge bg-success'>Open</span></td>"<<endl;    
             
				Php::out<<"</tr>"<<endl;                
                }  
            Php::out<<"</tbody></table>"<<endl;
            }
        else                
            {
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
            }
        delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
       
        }
    catch(const std::exception& e)
        {
        writeRevenueReportException("shiftReport",e.what());        
        }
    }

void RevenueReport::getshiftDetails(Php::Value json)
{
	string difference="";
    try
	{        
		string shift_id=json["shift_id"];
		string lang=json["language"];
                
                string labels="device_name,operator_name,shift_start,shift_end,open_amount,close_amount,payin_amount,payout_amount,duration,parking_fee,lost_fee,vat,physical_cash,shift_earnings,over_short";
                Php::Value label=General.getLabels(lang,labels);

		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select * from revenue_shifts where shift_id='"+shift_id+"'";
		res= rStmt->executeQuery(sql_query);
		
		if(res->next())
        {
			Php::out<<"<tr><td>"<<label["device_name"]<<"</td><td> "+res->getString("device_name")+" </td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<label["operator_name"]<<"</td><td>"+res->getString("operator_name")+"</td></tr>"<<endl;    
			Php::out<<"<tr><td>"<<label["shift_start"]<<"</td><td>"+res->getString("login_time")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<label["shift_end"]<<"</td><td>"+res->getString("logout_time")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<label["open_amount"]<<"</td><td>"+res->getString("shift_opening_amount")+"</td></tr>"<<endl;        
			Php::out<<"<tr><td>"<<label["close_amount"]<<"</td><td>"+res->getString("shift_closing_amount")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<label["payin_amount"]<<"</td><td>"+res->getString("payin_amount")+"</td></tr>"<<endl;        
			Php::out<<"<tr><td>"<<label["payout_amount"]<<"</td><td>"+res->getString("payout_amount")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<label["duration"]<<"</td><td>"+res->getString("shift_duration")+"</td></tr>"<<endl;                   
			Php::out<<"<tr><td>"<<label["parking_fee"]<<"</td><td>"+res->getString("parking_fees")+"</td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<label["lost_fee"]<<"</td><td>"+res->getString("lost_fee")+"</td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<label["vat"]<<"</td><td>"+res->getString("vat_amount")+"</td></tr>"<<endl;               
			Php::out<<"<tr><td>"<<label["shift_earnings"]<<"</td><td class='font-weight-bold'>"+res->getString("shift_earnings")+"</td></tr>"<<endl;          
			Php::out<<"<tr><td>"<<label["physical_cash"]<<"</td><td class='font-weight-bold'>"+res->getString("physical_cash_collected")+"</td></tr>"<<endl;       
			if(res->getDouble("physical_cash_collected")>0)                      
				difference=to_string(res->getDouble("physical_cash_collected")-res->getDouble("shift_earnings"));                
			else            
				difference="-";               
			Php::out<<"<tr><td>"<<label["over_short"]<<"</td><td class='font-weight-bold'>"+difference+"</td></tr>"<<endl;
			Php::out<<"</div>";
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("getshiftDetails",e.what());        
	}

}



void RevenueReport::apmrefillReport(Php::Value json)
{
	string str_array,a,b,sum,currency;
	int total = 0;
    try
	{        
		string startdate=json["from"];
		string enddate=json["to"];
		string device_number = json["device"];
		string carpark = json["carpark"];
		string lang=json["language"];
		string notes="";
                
                string labels="device_name,date_time,operator_name,banknote,amount,transactions,total_amount_refilled,no_records";
                Php::Value label=General.getLabels(lang,labels);

                
		currency = getCurrency();
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select * from revenue_apm_cash_refill where datetime between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            sql_query += " AND device_number IN("+device_number+")";  
		res= rStmt->executeQuery(sql_query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";        
            data+="<th>"+toString(label["date_time"])+"</th>";  
            data+="<th>"+toString(label["operator_name"])+"</th>";  
            data+="<th>"+toString(label["banknote"])+"</th>";    
            data+="<th>"+toString(label["amount"])+"</th>";  
            
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				i++;    
				notes=res->getString("notes_refilled");
				notes = notes.substr(0, notes.size()-1);
                notes=str_replace_string(notes,";",",");
				std::vector<std::string> outputArr =split(notes,',');
				notes="";
				for ( int k=0;k<(signed)outputArr.size();k+=2){
						a = outputArr[k];
						b = outputArr[k+1];
						sum = a +" "+currency+" x "+b+"="+to_string((stoi(a)*stoi(b)))+" "+currency;
						//data+=sum+"\n";
						if(outputArr.size()>0)
							notes=sum+"<br>"+notes;
                    
					}
               				
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+="<td>"+res->getString("device_name")+"</td>"; 
				data+="<td>"+res->getString("datetime")+"</td>";   
				data+="<td>"+res->getString("user")+"</td>";  
				data+="<td>"+notes+"</td>";                     
				data+="<td> "+res->getString("total_amount_refilled")+" </td>";                                                             
				total=total+res->getInt("total_amount_refilled");
				data+="</tr>";            
			}  
			data+="</tbody></table>";
			
			summary+="<div class='row mb-4 jspdf-graph'>";            			
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(res->rowsCount())+"</h3>";
            summary+="<h6>"+toString(label["transactions"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
			
            summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(total)+" "+currency+"</h3>";
            summary+="<h6>"+toString(label["total_amount_refilled"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("apmrefillReport",e.what());        
	}

}


void RevenueReport::apmcashlevelsReport(Php::Value json)
{
	string str_array,a,b,sum,currency;
	int apm_total=0;
	int total=0;
    try
	{        
		string device_number = json["device"];
		string lang=json["language"];

                string labels="banknotes,recycler_level,cashbox_level,total,no_records";
                Php::Value label=General.getLabels(lang,labels);

		rCon=General.mysqlConnect(DashboardDB);
		rStmt=rCon->createStatement();
		sql_query="SELECT a.device_number,a.device_name,a.denomination,a.current_level as recycler_current_level,b.current_level as cashbox_current_level FROM apm_recycle_levels as a join apm_cashbox_levels as b on a.denomination=b.denomination and a.device_number=b.device_number";
		
        if (device_number.length() > 0) 		           
            sql_query += " AND a.device_number IN("+device_number+")";  
		
		sql_query += " order by denomination asc";
		res= rStmt->executeQuery(sql_query);
		string data="";
		if(res->rowsCount()>0)
		{
            data = "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>";
            data = data+"<thead>";
            data = data+"<tr>"; 
            data = data+ "<th>"+toString(label["banknotes"])+"</th>";        
            data = data+ "<th>"+toString(label["recycler_level"])+"</th>";  
            data = data+ "<th>"+toString(label["cashbox_level"])+"</th>";  
            data = data+ "<th>"+toString(label["total"])+"</th>";               
            data = data+ "</tr>"; 
            data = data+ "</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				if(device_number!=res->getString("device_number"))
					{
					if(device_number!="")
						{
						data+="<tr><td></td><td></td><td><h3>"+toString(label["total"])+"</h3></td><td><h3>"+to_string(apm_total)+"</h3></td></tr>";
						apm_total=0;   
						}    
					data+="<tr><td>"+res->getString("device_name")+"</td><td></td><td></td><td></td></tr>";
					device_number=res->getString("device_number");
					}
				data+="<tr>";
				//html_data.="<td class='col'>".data['device_name']."</td>";    
				data+="<td>"+res->getString("denomination")+"</td>";            
				data+="<td>"+res->getString("recycler_current_level")+"</td>";            
				data+="<td>"+res->getString("cashbox_current_level")+"</td>";   
				total=res->getInt("denomination")*res->getInt("cashbox_current_level")+res->getInt("denomination")*+res->getInt("recycler_current_level");			
				data+="<td>"+to_string(total)+"</td>";  		
				data+="</tr>";
				apm_total=apm_total+total;									
			          
			}  
			if(data!="")
           data+="<tr><td></td><td></td><td><h3>"+toString(label["total"])+"</h3></td><td><h3>"+to_string(apm_total)+"</h3></td></tr>";
           Php::out<<data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("apmcashlevelsReport",e.what());        
	}

}


void RevenueReport::apmcashpayoutReport(Php::Value json)
{
	string str_array,a,b,sum,currency,notes;
	int total=0;
    try
	{        
		string startdate=json["from"];
		string enddate=json["to"];
		string device_number = json["device"];
		string carpark = json["carpark"];
		string lang=json["language"];
                
                string labels="device_name,operation,date_time,operator,banknotes,amount,transactions,total_payout,no_records";
                Php::Value label=General.getLabels(lang,labels);

                
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select * from revenue_apm_cash_payout where datetime between '"+startdate+"' AND '"+enddate+"'";
		
        if (device_number.length() > 0) 		           
            sql_query += " AND device_number IN("+device_number+")";  
		
		if (carpark.length() > 0) 		           
            sql_query += " AND device_number IN("+device_number+")";  

		res= rStmt->executeQuery(sql_query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
			
			
			
            data = "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>";
            data = data+"<thead>";
            data = data+"<tr>"; 
            data = data+ "<th>"+toString(label["device_name"])+"</th>";        
            data = data+ "<th>"+toString(label["operation"])+"</th>";  
            data = data+ "<th>"+toString(label["date_time"])+"</th>";  
            data = data+ "<th>"+toString(label["operator"])+"</th>";
            data = data+ "<th>"+toString(label["banknotes"])+"</th>";  
            data = data+ "<th>"+toString(label["amount"])+"</th>";               
            data = data+ "</tr>"; 
            data = data+ "</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				notes=res->getString("notes_payout");
				notes = notes.substr(0, notes.size()-1);
                notes=str_replace_string(notes,";",",");
				std::vector<std::string> outputArr =split(notes,',');
				notes="";
				for ( int k=0;k<(signed)outputArr.size();k+=2){
					a = outputArr[k];
					b = outputArr[k+1];
					sum = a +" "+currency+" x "+b+"="+to_string((stoi(a)*stoi(b)))+" "+currency;
					//Php::out<<sum<<"\n";
					if(outputArr.size()>0)
						notes=sum+"<br>"+notes;
				
				}
				data+="<td>"+res->getString("device_name")+"</td>";            
				data+="<td>"+res->getString("operation")+"</td>";            
				data+="<td>"+res->getString("datetime")+"</td>";   
				data+="<td>"+res->getString("user")+"</td>";            
				data+="<td>"+notes+"</td>";            
				data+="<td>"+res->getString("total_payout_amount")+"</td></tr>";   
				total=total+res->getInt("total_payout_amount");							
			          
			} 

			summary = "<div class='row mb-4 jspdf-graph'>";
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(res->rowsCount())+"</h3>";
            summary+="<h6>"+toString(label["transactions"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";

            summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(total)+"</h3>";
            summary+="<h6>"+toString(label["total_payout"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
           Php::out<<summary + data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("apmcashpayoutReport",e.what());        
	}

}

void RevenueReport::paymentexceptionReport(Php::Value json)
{
	string str_array,a,b,sum,currency,paid;
    try
	{        
		string startdate=json["from"];
		string enddate=json["to"];
		string device_number = json["device"];
		string carpark = json["carpark"];
		string lang=json["language"];
		string ex_option = json["exception_option"];
		string notes="";
		currency = getCurrency();

                string labels="date_time,device_name,ticket_id,exception,amount_received,balance_returned,credit_note,banknotes,transactions,no_records";
                Php::Value label=General.getLabels(lang,labels);



		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select * from revenue_payment_exceptions where date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            sql_query += " AND device_number IN("+device_number+")"; 

		if (ex_option !="0") 
            sql_query += " AND exception LIKE '"+ex_option+"%'";
		res= rStmt->executeQuery(sql_query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+toString(label["date_time"])+"</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";        
            data+="<th>"+toString(label["ticket_id"])+"</th>";  
            data+="<th>"+toString(label["exception"])+"</th>";  
            data+="<th>"+toString(label["amount_received"])+"</th>";    
            data+="<th>"+toString(label["balance_returned"])+"</th>";  
            data+="<th>"+toString(label["credit_note"])+"</th>";    
            data+="<th>"+toString(label["banknotes"])+"</th>";  
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				i++;    
				
				if(res->getInt("paid_status")==1)
                    paid="Not paid";
                else if(res->getInt("paid_status")==2)
                    paid="Paid";
                else if(res->getInt("paid_status")==3)
                    paid="Exited";
				
				notes=res->getString("bank_notes");
				notes = notes.substr(0, notes.size()-1);
                //notes=str_replace_string(notes,";",",");
				notes=str_replace_string(notes,","," "+currency+" x ");
                notes=str_replace_string(notes,";","<br>");
				
               				
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+="<td>"+res->getString("date_time")+"</td>"; 
				data+="<td>"+res->getString("device_name")+"</td>"; 
				data+="<td>"+res->getString("ticket_id")+"</td>";   
				data+="<td>"+res->getString("exception")+"</td>"; 
				data+="<td>"+res->getString("amount_received")+"</td>"; 
				data+="<td>"+res->getString("balance_returned")+"</td>";   
				data+="<td>"+res->getString("credit_note")+"</td>";  
				data+="<td>"+notes+"</td>";                     
				data+="</tr>";            
			}  
			data+="</tbody></table>";
			
			summary+="<div class='row mb-4 jspdf-graph'>";            			
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(res->rowsCount())+"</h3>";
            summary+="<h6>"+toString(label["transactions"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("paymentexceptionReport",e.what());        
	}

}


void RevenueReport::creditcardReport(Php::Value json)
{
	string str_array,a,b,sum,currency,paid;
	double total=0;
    try
	{        
		string startdate=json["from"];
		string enddate=json["to"];
		string device_number = json["device"];
		string carpark = json["carpark"];
		string lang=json["language"];
		string notes="";
		currency = getCurrency();

                string labels="date_time,device_name,merchant_id,terminal_id,ticket_id,comment,card_type,amount,authentication_code,reference_number,status,transactions,total_revenue,no_records";
                Php::Value label=General.getLabels(lang,labels);


		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select * from revenue_creditcard_payments where  date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            sql_query += " AND device_number IN("+device_number+")"; 

		res= rStmt->executeQuery(sql_query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+toString(label["date_time"])+"</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";        
            data+="<th>"+toString(label["merchant_id"])+"</th>";  
            data+="<th>"+toString(label["terminal_id"])+"</th>";  
            data+="<th>"+toString(label["ticket_id"])+"</th>";  
            data+="<th>"+toString(label["comment"])+"</th>";  
            data+="<th>"+toString(label["card_type"])+"</th>";    
            data+="<th>"+toString(label["amount"])+"</th>";  
            data+="<th>"+toString(label["authentication_code"])+"</th>";    
            data+="<th>"+toString(label["reference_number"])+"</th>";  
            data+="<th>"+toString(label["status"])+"</th>";  
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				i++;    
				
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+="<td>"+res->getString("date_time")+"</td>"; 
				data+="<td>"+res->getString("device_number")+"</td>"; 
				data+="<td>"+res->getString("merchant_id")+"</td>"; 
				data+="<td>"+res->getString("terminal_id")+"</td>"; 
				data+="<td>"+res->getString("ticket_number")+"</td>";   
				data+="<td>"+res->getString("comment")+"</td>"; 
				data+="<td>"+res->getString("card_type")+"</td>"; 
				data+="<td>"+res->getString("amount")+"</td>";   
				data+="<td>"+res->getString("authentication_code")+"</td>";  
				data+="<td>"+res->getString("reference_number")+"</td>";  
				data+="<td>"+res->getString("status")+"</td>";  
				data+="</tr>";   

				total = total + res->getDouble("amount");

			}  
			data+="</tbody></table>";
			
			summary+="<div class='row mb-4 jspdf-graph'>";            			
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(res->rowsCount())+"</h3>";
            summary+="<h6>"+toString(label["transactions"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
			
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+SetDoublePrecision(total)+"</h3>";
            summary+="<h6>"+toString(label["total_revenue"])+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
	   }
    catch(const std::exception& e)
	{
		writeRevenueReportException("creditcardReport",e.what());        
	}

}


string revenuesummaryReport(string startdate,string enddate,string weekdays,string carpark,string lang,Php::Value label)
{
	string str_array,a,b,sum,currency,paid;
	string data="";
	string summary="";
	sql::ResultSet *rs;
	sql::Connection *conn;
	sql::Statement *stmt;
try
	{        
		currency = getCurrency();
		conn=General.mysqlConnect(ReportingDB);
		stmt=conn->createStatement();
		sql_query="select sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_fee,sum(gross_amount) as earnings,sum(vat_amount) as vat,sum(product_sale_amount) as product_sale,sum(discount_amount) as discount_amount from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(report_date) IN("+weekdays+")"; 

		sql_query += " AND device_number > 0 ORDER BY device_number ASC";
		rs= stmt->executeQuery(sql_query);
		while(rs->next())
		{

			data+="<span id='chart-data' class='d-none' data-currency="+currency+" data-parking-fee='"+SetDoublePrecision(rs->getDouble("parking_fee"))+"' data-lost-ticket='"+SetDoublePrecision(rs->getDouble("lost_fee"))+"' data-product-sale='"+SetDoublePrecision(rs->getDouble("product_sale"))+"' data-vat='"+SetDoublePrecision(rs->getDouble("vat"))+"'></span>";

			data+="<div class='row mb-4 jspdf-graph'>";
			data+="<div class='col-auto'>";    
			data+="<div class='small-box bg-success'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("earnings"))+"</h3>";
			data+="<p>"+toString(label["total_revenue"])+"</p>";
			data+="</div>";
			data+="<div class='icon'>";
			data+="<i class='ion ion-stats-bars'></i>";
			data+="</div>";		
			data+="</div>";
			data+="</div>";
			
			data+="<div class='col-auto'>";		
			data+="<div class='small-box box-color-parking-fee'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("parking_fee"))+"</h3>";
			data+="<p>"+toString(label["parking_fee"])+"</p>";
			data+="</div>";
			data+="<div class='icon'>";
			data+="<i class='ion ion-stats-bars'></i>";
			data+="</div>";		
			data+="</div>";
			data+="</div>";

			data+="<div class='col-auto'>";    
			data+="<div class='small-box box-color-lost-fee'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("lost_fee"))+"</h3>";
			data+="<p>"+toString(label["lost_fee"])+"</p>";
			data+="</div>";
			data+="<div class='icon'>";
			data+="<i class='ion ion-stats-bars'></i>";
			data+="</div>";		
			data+="</div>";
			data+="</div>";

			data+="<div class='col-auto'>";		
			data+="<div class='small-box box-color-product-fee'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("product_sale"))+"</h3>";
			data+="<p>"+toString(label["product_sale_amount"])+"</p>";
			data+="</div>";
			data+="<div class='icon'>";
			data+="<i class='ion ion-stats-bars'></i>";
			data+="</div>";		
			data+="</div>";
			data+="</div>";

			// if(vat!=0)
			//     {
			//     html_data.='<div class="col-auto">';		
			//     html_data.='<div class="small-box bg-success">';
			//     html_data.='<div class="inner">';
			//     html_data.='<h3>'.currency." ".number_format(data_summary["vat"],d).'</h3>';
			//     html_data.='<p>VAT</p>';
			//     html_data.='</div>';
			//     html_data.='<div class="icon">';
			//     html_data.='<i class="ion ion-stats-bars"></i>';
			//     html_data.='</div>';
			//     html_data.='</div>';
			//     html_data.='</div>';
			//     }

			data+="<div class='col-auto'>";
			data+="<!-- small box -->";
			data+="<div class='small-box box-color-discount-fee'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("discount_amount"))+"</h3>";
			data+="<p>"+toString(label["discount_amount"])+"</p>";
			data+="</div>";
			data+="<div class='icon'>";
			data+="<i class='ion ion-stats-bars'></i>";
			data+="</div>";
			data+="</div>";
			data+="</div>";

			data+="</div><!-- Row End -->";
			data+="</div>";
		}
		
		delete stmt;
        delete rs;       
        conn->close();
        delete conn;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("revenuesummaryReport",e.what());        
	}
		return data;

}

string revenuedayclosurecomment(string startdate,string enddate,string weekdays,string carpark,string lang,Php::Value label)
{
	string str_array,a,b,sum,currency,paid;
	string data="";
	string summary="";
	sql::ResultSet *rs;
	sql::Connection *conn;
	sql::Statement *stmt;
try
	{        
		currency = getCurrency();
		conn=General.mysqlConnect(ReportingDB);
		stmt=conn->createStatement();
		sql_query="select dayname(report_date) as day,report_date,comments,carpark_name,device_name from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(report_date) IN("+weekdays+")"; 

		sql_query += " AND reproccessing_flag=1 ORDER BY device_number ASC";
		rs= stmt->executeQuery(sql_query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["notifications"])+"</div>";
            data+="<table id='TABLE_1' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+toString(label["report_date"])+"</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";        
            data+="<th>"+toString(label["carpark_number"])+"</th>";  
            data+="<th>"+toString(label["comment"])+"</th>";  
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(rs->next())
			{ 
				i++;    
				
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+="<td>"+rs->getString("report_date")+"<small class='form-text text-muted'>"+rs->getString("day")+"</small></td>"; 
				data+="<td>"+rs->getString("device_name")+"</td>"; 
				data+="<td>"+rs->getString("carpark_name")+"</td>"; 
				data+="<td>"+rs->getString("comments")+"</td>"; 
				data+="</tr>";   


			}  
			data+="</tbody></table>";
		}
		
		delete stmt;
        delete rs;       
        conn->close();
        delete conn;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("revenuedayclosurecomment",e.what());        
	}
		return data;

}



string revenuesummaryReportbydate(string startdate,string enddate,string weekdays,string carpark,string lang,Php::Value label)
{
	string str_array,a,b,sum,currency,paid;
	string data="";
	string summary="";
	int cashcount=0;
	sql::ResultSet *rs;
	sql::Connection *conn;
	sql::Statement *stmt;
try
	{        
		currency = getCurrency();
		conn=General.mysqlConnect(ReportingDB);
		stmt=conn->createStatement();
		sql_query="select report_date,dayname(report_date) as day,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as discount_amount,sum(payable_entries_count) as payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(report_date) IN("+weekdays+")"; 

		sql_query += " GROUP BY report_date ORDER BY report_date DESC";
		rs= stmt->executeQuery(sql_query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["report_by_date"])+"</div>";
            data+="<table id='TABLE_2' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+toString(label["report_date"])+"</th>"; 
            data+="<th>"+toString(label["total_earnings"])+"</th>";        
            data+="<th>"+toString(label["parking_fee"])+"</th>";  
            data+="<th>"+toString(label["lost_fee"])+"</th>";  
			data+="<th>"+toString(label["product_sale_amount"])+"</th>"; 
            data+="<th>"+toString(label["cash_amount"])+"</th>";        
            data+="<th>"+toString(label["creditcard_amount"])+"</th>";  
            data+="<th>"+toString(label["wallet_amount"])+"</th>";  
			data+="<th>"+toString(label["payable_entries_count"])+"</th>";  
            data+="<th>"+toString(label["lost_ticket_count"])+"</th>";  
            data+="<th>"+toString(label["discount_count"])+"</th>";  
			data+="<th>"+toString(label["product_sale_count"])+"</th>"; 
            data+="<th>"+toString(label["cash_transaction_count"])+"</th>";        
            data+="<th>"+toString(label["creditcard_transaction_count"])+"</th>";  
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(rs->next())
			{ 
				i++;    
				
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+="<td>"+rs->getString("report_date")+"<small class='form-text text-muted'>"+rs->getString("day")+"</small></td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("gross_amount"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("parking_fee"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("lost_ticket_fee"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("product_sale_amount"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("cash_amount"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("creditcard_amount"))+"</td>"; 
				data+="<td>"+SetDoublePrecision(rs->getDouble("sms_amount"))+"</td>"; 
				cashcount=rs->getInt("payable_entries_count")-rs->getInt("creditcard_count");
				data+="<td>"+rs->getString("payable_entries_count")+"</td>"; 
				data+="<td>"+rs->getString("lost_ticket_count")+"</td>"; 
				data+="<td>"+rs->getString("discount_count")+"</td>"; 
				data+="<td>"+rs->getString("product_sale_count")+"</td>"; 
				data+="<td>"+to_string(cashcount)+"</td>"; 
				data+="<td>"+rs->getString("creditcard_count")+"</td>"; 
				data+="</tr>";   


			}  
			data+="</tbody></table>";
		}
		
		delete stmt;
        delete rs;       
        conn->close();
        delete conn;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("revenuesummaryReportbydate",e.what());        
	}
		return data;

}


string productsaleReport(string startdate,string enddate,string weekdays,string carpark,string lang,Php::Value label)
{
	string str_array,a,b,sum,currency,paid;
	string data="";
	string summary="";
	sql::ResultSet *rs;
	sql::Connection *conn;
	sql::Statement *stmt;
	int carpark_displayed_flag=0;
try
	{        
		currency = getCurrency();
		conn=General.mysqlConnect(ReportingDB);
		stmt=conn->createStatement();
		sql_query="select device_name,product_name,MIN(carpark_number) as carpark_number,carpark_name,sum(product_quantity) as product_quantity,unit_price,sum(product_sale_amount) as product_sale from revenue_product_usage where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(report_date) IN("+weekdays+")"; 

		sql_query += " GROUP BY device_name,product_id ORDER BY carpark_number ASC";
		rs= stmt->executeQuery(sql_query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["product_sales"])+"</div>";
            data+="<table id='TABLE_5' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>"; 
            data+="<th>"+toString(label["product_name"])+"</th>";        
            data+="<th>"+toString(label["quantity"])+"</th>";  
            data+="<th>"+toString(label["unit_price"])+"</th>";  
			data+="<th>"+toString(label["total_amount"])+"</th>"; 
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(rs->next())
			{ 
				i++;    
				if(carpark != rs->getString("carpark_number")) 
                    {
                    carpark=rs->getString("carpark_number");
                    carpark_displayed_flag=0;
                    }
        
                if((carpark != "") && carpark_displayed_flag ==0)
                    {
                    data+="<tr>";
                    data+="<td>"+rs->getString("carpark_name")+"</td><td></td><td></td><td></td><td></td><td></td>";
                    data+="</tr>";
                    carpark_displayed_flag=1;
                    }  


				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+= "<td>"+ rs->getString("device_name")+" </td>";
                data+= "<td>" + rs->getString("product_name")+ "</td>";
                data+= "<td>" + rs->getString("product_quantity")+ "</td>";
                data+= "<td>"  + SetDoublePrecision(rs->getDouble("unit_price"))+ "</td>";   
                data+= "<td>" + SetDoublePrecision(rs->getDouble("product_sale"))+ "</td>";
				data+="</tr>";   


			}  
			data+="</tbody></table>";
		}
		
		delete stmt;
        delete rs;       
        conn->close();
        delete conn;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("productsaleReport",e.what());        
	}
		return data;

}

string discountusagesummary(string startdate,string enddate,string weekdays,string carpark,string lang,Php::Value label)
{
	string str_array,a,b,sum,currency,paid;
	string data="";
	string summary="";
	sql::ResultSet *rs;
	sql::Connection *conn;
	sql::Statement *stmt;
	int carpark_displayed_flag=0;
try
	{        
		currency = getCurrency();
		conn=General.mysqlConnect(ReportingDB);
		stmt=conn->createStatement();
		sql_query="select device_name,discount_name,MIN(carpark_number) as carpark_number,carpark_name,count(id) as discount_usage,sum(discount_amount) as discount_amount from revenue_payments where discount_amount > 0  and payment_date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(payment_date_time) IN("+weekdays+")"; 

		sql_query += " AND device_number > 0 GROUP BY device_name,discount_id ORDER BY carpark_number ASC";
		rs= stmt->executeQuery(sql_query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["discounts"])+"</div>";
            data+="<table id='TABLE_5' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+toString(label["device_name"])+"</th>"; 
            data+="<th>"+toString(label["discount_name"])+"</th>";        
            data+="<th>"+toString(label["discount_count"])+"</th>";  
            data+="<th>"+toString(label["discount_amount"])+"</th>";  
            data+="</tr>"; 
            data+="</thead><tbody>"; 
			i=0;
            while(rs->next())
			{ 
				i++;    
				if(carpark != rs->getString("carpark_number")) 
                    {
                    carpark=rs->getString("carpark_number");
                    carpark_displayed_flag=0;
                    }
        
                if((carpark != "") && carpark_displayed_flag ==0)
                    {
                    data+="<tr>";
                    data+="<td>"+rs->getString("carpark_name")+"</td><td></td><td></td><td></td><td></td>";
                    data+="</tr>";
                    carpark_displayed_flag=1;
                    }  


				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+= "<td>"+ rs->getString("device_name")+" </td>";
                data+= "<td>" + rs->getString("discount_name")+ "</td>";
                data+= "<td>" + rs->getString("discount_usage")+ "</td>";
                data+= "<td>"  + SetDoublePrecision(rs->getDouble("discount_amount"))+ "</td>";   
				data+="</tr>";   


			}  
			data+="</tbody></table>";
		}
		else
		{
			data+="<div class='card table-view'><div class='p-3'>No Discounts In This Time Period</div></div>";
		}
		
		delete stmt;
        delete rs;       
        conn->close();
        delete conn;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("discountusagesummary",e.what());        
	}
		return data;

}

void RevenueReport::revenueReport(Php::Value json)
{
	string str_array,a,b,sum,currency,paid;
	int carpark_displayed_flag=0;
	int cashcount=0;

	try
	{        
		string startdate=json["from"];
		string enddate=json["to"];
		string weekdays = json["weekdays"];
		string carpark = json["carpark"];
		string lang=json["language"];


                string labels="total_revenue,parking_fee,lost_fee,product_sale_amount,discount_amount,notifications,report_date,device_name,carpark_number,comment,report_by_date,report_date,total_earnings,parking_fee,lost_fee,product_sale_amount,cash_amount,creditcard_amount,wallet_amount,payable_entries_count,lost_ticket_count,discount_count,product_sale_count,cash_transaction_count,creditcard_transaction_count,product_sales,product_name,quantity,unit_price,total_amount,discounts,discount_name,discount_count,discount_amount,earnings,earnings_by_payment_type,cash,credit_card,sms,transaction_count,total_earnings,payable_entries,no_records";
                Php::Value label=General.getLabels(lang,labels);


		currency = getCurrency();
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		sql_query="select device_name,carpark_number,carpark_name,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as discount_amount,sum(payable_entries_count) as payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            sql_query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            sql_query += " AND dayname(report_date) IN("+weekdays+")"; 
		
        sql_query+=" AND device_number > 0 GROUP BY device_number ORDER BY carpark_number ASC";   

		res= rStmt->executeQuery(sql_query);
		string data="";
		if(res->rowsCount()>0)
		{
			Php::out<<revenuesummaryReport(startdate,enddate,weekdays,carpark,lang,label)<<endl;
			Php::out<<revenuedayclosurecomment(startdate,enddate,weekdays,carpark,lang,label)<<endl;
			Php::out<<revenuesummaryReportbydate(startdate,enddate,weekdays,carpark,lang,label)<<endl;
			
			/////////////////////////
            //Earnings
            ////////////////////////
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["earnings"])+"</div>";
            data+="<table id='TABLE_3' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";
            data+="<th>"+toString(label["parking_fee"])+"</th>";
            data+="<th>"+toString(label["lost_fee"])+"</th>";
            data+="<th>"+toString(label["product_sale_amount"])+"</th>";
            data+="<th>"+toString(label["total_earnings"])+"</th>";
            data+="</tr>";
			data+="</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				i++;    
				if(carpark != res->getString("carpark_number")) 
                    {
                    carpark=res->getString("carpark_number");
                    carpark_displayed_flag=0;
                    }
        
                if((carpark != "") && carpark_displayed_flag ==0)
                    {
                    data+="<tr>";
                    data+="<td>"+res->getString("carpark_name")+"</td><td></td><td></td><td></td><td></td><td></td>";
                    data+="</tr>";
                    carpark_displayed_flag=1;
                    }  


				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+= "<td>"+ res->getString("device_name")+" </td>";
                data+= "<td>" + SetDoublePrecision(res->getDouble("parking_fee"))+ "</td>";
                data+= "<td>" + SetDoublePrecision(res->getDouble("lost_ticket_fee"))+ "</td>";
                data+= "<td>"  + SetDoublePrecision(res->getDouble("product_sale_amount"))+ "</td>";   
                data+= "<td>" + SetDoublePrecision(res->getDouble("gross_amount"))+ "</td>";
				data+="</tr>";   


			}  
			data+="</tbody></table>";
			Php::out<<data<<endl;

			/////////////////////////
            //Earning by payment type
            ////////////////////////
			data="";
			carpark_displayed_flag=0;
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["earnings_by_payment_type"])+"</div>";
            data+="<table id='TABLE_4' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";
            data+="<th>"+toString(label["cash"])+"</th>";
            data+="<th>"+toString(label["credit_card"])+"</th>";
            data+="<th>"+toString(label["sms"])+"</th>";
            data+="</tr>";
			data+="</thead><tbody>"; 
			i=0;
			res->beforeFirst();
            while(res->next())
			{ 
				i++;    
				if(carpark != res->getString("carpark_number")) 
                    {
                    carpark=res->getString("carpark_number");
                    carpark_displayed_flag=0;
                    }
        
                if((carpark != "") && carpark_displayed_flag ==0)
                    {
                    data+="<tr>";
                    data+="<td>"+res->getString("carpark_name")+"</td><td></td><td></td><td></td><td></td>";
                    data+="</tr>";
                    carpark_displayed_flag=1;
                    }  


				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+= "<td>"+ res->getString("device_name")+"</td>";
                data+= "<td>" + SetDoublePrecision(res->getDouble("cash_amount"))+ "</td>";
                data+= "<td>" + SetDoublePrecision(res->getDouble("creditcard_amount"))+ "</td>";
                data+= "<td>"  + SetDoublePrecision(res->getDouble("sms_amount"))+ "</td>";   
				data+="</tr>";   


			}  
			data+="</tbody></table>";
			Php::out<<data<<endl;

			////////////////////////
            //Prodct sale report
            ////////////////////////
			Php::out<<productsaleReport(startdate,enddate,weekdays,carpark,lang,label)<<endl;

            //////////////////////
            //Discounts report
            ///////////////////
            Php::out<<discountusagesummary(startdate,enddate,weekdays,carpark,lang,label)<<endl;
			
			
            ////////////////////////
            //Transaction Count
            ////////////////////////
			data="";
			carpark_displayed_flag=0;
			data+="<div class='header pl-0 mt-4 mb-3'>"+toString(label["transaction_count"])+"</div>";
            data+="<table id='TABLE_6' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+toString(label["device_name"])+"</th>";
            data+="<th>"+toString(label["payable_entries"])+"</th>";
            data+="<th>"+toString(label["lost_ticket"])+"</th>";
            data+="<th>"+toString(label["discounts"])+"</th>";
            data+="<th>"+toString(label["product_sales"])+"</th>";
            data+="<th>"+toString(label["cash"])+"</th>";
            data+="<th>"+toString(label["credit_card"])+"</th>";
            data+="</tr>";
			data+="</thead><tbody>"; 
			i=0;
			res->beforeFirst();
            while(res->next())
			{ 
				i++;    
				if(carpark != res->getString("carpark_number")) 
                    {
                    carpark=res->getString("carpark_number");
                    carpark_displayed_flag=0;
                    }
        
                if((carpark != "") && carpark_displayed_flag ==0)
                    {
                    data+="<tr>";
                    data+="<td>"+res->getString("carpark_name")+"</td><td></td><td></td><td></td><td></td></td><td></td><td></td><td></td>";
                    data+="</tr>";
                    carpark_displayed_flag=1;
                    }  

				cashcount=res->getInt("payable_entries_count")-res->getInt("creditcard_count");
				data+="<tr><td>"+to_string(i)+"</td>"; 
				data+= "<td>"+ res->getString("device_name")+"</td>";
                data+= "<td>" +res->getString("payable_entries_count")+ "</td>";
                data+= "<td>" +res->getString("lost_ticket_count")+ "</td>";
                data+= "<td>" +res->getString("discount_count")+ "</td>";
                data+= "<td>" +res->getString("product_sale_count")+ "</td>";
                data+= "<td>" +to_string(cashcount)+ "</td>";
                data+= "<td>" +res->getString("creditcard_count")+ "</td>";
				data+="</tr>";   


			}  
			data+="</tbody></table>";
			Php::out<<data<<endl;


		}
		else{
			Php::out<<"<div class='p-0'>"<<toString(label["no_records"])<<"</div>"<<endl;
		}
		delete rStmt;
        delete res;       
        rCon->close();
        delete rCon;
		
	}
    catch(const std::exception& e)
	{
		writeRevenueReportException("revenuesummaryReport",e.what());        
	}

}