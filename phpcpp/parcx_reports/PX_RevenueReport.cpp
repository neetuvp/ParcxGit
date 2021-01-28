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

string getLabelRevenue(string label,string lang)
    {
    try
        {
		con = General.mysqlConnect(ServerDB);
		stmt = con->createStatement();
        query= "select "+lang+" from web_application_labels where message='"+label+"'";
		//writeRevenueReportLog("",query);
        res2=stmt->executeQuery(query);
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
            Php::out<<"<h6>"+getLabelRevenue("total_revenue",lang)+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"+getLabelRevenue("transactions",lang)+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("ticket_id",lang)+"</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("device_name",lang)+"</th>"<<endl;        
            Php::out<<"<th>"+getLabelRevenue("payment_date_time",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("duration",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("category",lang)+"</th>"<<endl;    
            Php::out<<"<th>"+getLabelRevenue("payment_type",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("discount_name",lang)+"</th>"<<endl;                                               
            Php::out<<"<th>"+getLabelRevenue("discount_amount",lang)+"</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("gross_amount",lang)+"</th>"<<endl;                                                        
            
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
            Php::out<<"<div class='p-0'>"+getLabelRevenue("no_records",lang)+"</div>"<<endl;
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
            Php::out<<"<h6>"+getLabelRevenue("total_revenue",lang)+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
                        
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<vat<<"</h3>"<<endl;
            Php::out<<"<h6>"+getLabelRevenue("total_vat",lang)+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"+getLabelRevenue("transactions",lang)+"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("ticket_id",lang)+"</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("device_name",lang)+"</th>"<<endl;        
            Php::out<<"<th>"+getLabelRevenue("payment_date_time",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("duration",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("category",lang)+"</th>"<<endl;    
            Php::out<<"<th>"+getLabelRevenue("payment_type",lang)+"</th>"<<endl;  
            Php::out<<"<th>"+getLabelRevenue("discount_name",lang)+"</th>"<<endl;    
            Php::out<<"<th>"+getLabelRevenue("vat_amount",lang)+"</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("discount_amount",lang)+"</th>"<<endl; 
            Php::out<<"<th>"+getLabelRevenue("gross_amount",lang)+"</th>"<<endl;                                                        
            
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
            Php::out<<"<div class='p-0'>"+getLabelRevenue("no_records",lang)+"</div>"<<endl;
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
		rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from revenue_payments where id= '"+id+"'"; 
        res= rStmt->executeQuery(query);
		arr = getFacilityfeatures();
        if(res->next())
        {
            html_data+="<tr><td>"+getLabelRevenue("ticket_id",lang)+"</td><td> "+res->getString("ticket_id")+" </td></tr>";  
			html_data+="<tr><td>"+getLabelRevenue("device_name",lang)+"</td><td> "+res->getString("device_name")+" </td></tr>";  
			html_data+="<tr><td>"+getLabelRevenue("parking_rate",lang)+"</td><td>"+res->getString("parking_rate_label")+"("+res->getString("parking_rate_name")+")</td></tr>";    
			html_data+="<tr><td>"+getLabelRevenue("entry_grace_period",lang)+"</td><td>"+res->getString("entry_grace_period")+"</td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("exit_grace_period",lang)+"</td><td>"+res->getString("exit_grace_period")+"</td></tr>";
			if(stoi(vat)!=0)
				html_data+="<tr><td>"+getLabelRevenue("vat",lang)+"</td><td>"+res->getString("vat_percentage")+"% "+res->getString("vat_type")+"</td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("entry_date_time",lang)+"</td><td>"+res->getString("entry_date_time")+"</td></tr>";        
			html_data+="<tr><td>"+getLabelRevenue("payment_date_time",lang)+"</td><td>"+res->getString("payment_date_time")+"</td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("maximum_exit_date_time",lang)+"</td><td>"+res->getString("max_exit_date_time")+"</td></tr>";        
			html_data+="<tr><td>"+getLabelRevenue("duration",lang)+"</td><td>"+res->getString("parking_duration")+"</td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("category",lang)+"</td><td>"+res->getString("payment_category")+"</td></tr>"; 
			
			if(res->getString("validation_id")!="")
			{
				std::vector<std::string> outputArr = split (res->getString("validation_value"),','); 
				//html_data+="<tr><td>Validations</td><td>";           
				html_data+="<tr><td>"+getLabelRevenue("validations",lang)+"</td><td>"+outputArr[0]+" Hour "+outputArr[1]+" %</td></tr>";                  
			}
			html_data+="<tr><td>"+getLabelRevenue("parking_fee",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("parking_fee"))+" "+currency+"</td></tr>";  
			html_data+="<tr><td>"+getLabelRevenue("lost_fee",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("lost_fee"))+" "+currency+"</td></tr>"; 
			if(stoi(vat)!=0) 
				html_data+="<tr><td>"+getLabelRevenue("vat",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("vat_amount"))+" "+currency+"</td></tr>"; 

			html_data+="<tr><td>"+getLabelRevenue("discounts",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("discount_amount"))+" "+currency+"</td></tr>"; 		
			if(res->getDouble("discount_amount")>0)
				html_data+="<tr><td>"+getLabelRevenue("discount_name",lang)+"</td><td>"+res->getString("discount_name")+"</td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("gross_amount",lang)+"</td><td class='font-weight-bold'>"+SetDoublePrecision(res->getDouble("gross_amount"))+" "+currency+"</td></tr>";          
			if(stoi(res->getString("validation_hours"))>0)
				html_data+="<tr><td>"+getLabelRevenue("validation_hours",lang)+"</td><td>"+res->getString("validation_hours")+"</td></tr>";                                
									
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
				html_data+="<tr><td>"+getLabelRevenue("amount_received",lang)+"</td><td>"+to_string(total)+"  "+currency+"</td></tr>";		
			html_data+="<tr><td>"+getLabelRevenue("balance_returned",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("balance_returned"))+" "+currency+" </td></tr>";
			html_data+="<tr><td>"+getLabelRevenue("credit_note",lang)+"</td><td>"+SetDoublePrecision(res->getDouble("credit_note"))+" "+currency+" </td></tr>";		
			if(notes!="")
				html_data+="<tr><td>"+getLabelRevenue("banknotes",lang)+"</td><td>"+notes+"</td></tr>";
			html_data+= "<tr><td><input class='btn btn-info btn-show-pdf-reciept' value='"+getLabelRevenue("pdf_receipt",lang)+"' type='button' data-id='"+res->getString("id")+"'></td></tr>";
			
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
		rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from revenue_payments where id= '"+id+"'"; 
        res= rStmt->executeQuery(query);
		arr = getFacilityfeatures();
        if(res->next())
        {
            html_data+="<div class='row mb-4'><div class='col text-center'>";    
			html_data+="<div>"+toString(arr["facility_name"])+"</div>"; 
			html_data+="<div>"+toString(arr["address_line1"])+"</div>"; 
			html_data+="<div>"+toString(arr["address_line2"])+"</div>"; 
			if(vat!="0")
				html_data+="<div>"+getLabelRevenue("trn_no",lang)+" "+toString(arr["vat_id"])+"</div>";                    
			
			html_data+="<hr><div>"+getLabelRevenue("device_name",lang)+": "+res->getString("device_name")+"</div>";
			html_data+="<div>"+getLabelRevenue("operator_name",lang)+": "+res->getString("operator_name")+"</span></div>";
			html_data+="<div>"+getLabelRevenue("payment_type",lang)+": "+res->getString("payment_type")+"</div></div></div>";
			html_data+="<div class='table-responsive-sm'>";
			html_data+="<table class='table table-striped table-receipt'>";
			html_data+="<thead><tr><th></th><th></th><th></th></tr></thead>";
			html_data+="<tbody><tr><td></td><td>";    
			html_data+="<div>"+res->getString("payment_category")+"</div>";
			html_data+="<span>"+getLabelRevenue("ticket_id",lang)+": "+res->getString("ticket_id")+"</span>";
			html_data+="<span>"+getLabelRevenue("entry_date_time",lang)+": "+res->getString("entry_date_time")+"</span>";
			html_data+="<span>"+getLabelRevenue("payment_date_time",lang)+": "+res->getString("payment_date_time")+"</span> ";   
			html_data+="</td>";
			html_data+="<td></td></tr>";
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+getLabelRevenue("duration",lang)+"</strong></td>";
			html_data+="<td><span>"+res->getString("parking_duration")+"</span></td></tr>";
			
			if(res->getString("validation_id")!="")
			{
				std::vector<std::string> outputArr = split (res->getString("validation_value"),','); 
				html_data+="<tr><td></td>";           
				html_data+="<td><strong>"+getLabelRevenue("validations",lang)+"</strong></td>";
				html_data+="<td><span>"+outputArr[0]+" H "+outputArr[1]+" %</span></td></tr>";              
			}
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+getLabelRevenue("parking_fee",lang)+"</strong></td>";
			html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("parking_fee"))+"</span></td></tr>";
			if(res->getDouble("discount_amount")>0)
			{
				html_data+="<tr><td></td>";
				html_data+="<td><strong>"+getLabelRevenue("discount_amount",lang)+"</strong><span>"+to_string(res->getDouble("discount_name"))+"</span></td>";
				html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("discount_amount"))+"</span></td></tr>";
			}
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+getLabelRevenue("net_amount",lang)+"</strong></td>";
			double net=res->getDouble("gross_amount")-res->getDouble("vat_amount");
			html_data+="<td><span>"+currency+" "+SetDoublePrecision(net)+"</span></td></tr>";
			if(vat!="0")
			{
				html_data+="<tr><td></td>";
				html_data+="<td><strong>"+getLabelRevenue("vat",lang)+" ("+vat+"%)</strong></td>";
				html_data+="<td><span>"+currency+" "+SetDoublePrecision(res->getDouble("vat_amount"))+"</span></td></tr>";
			}
			
			html_data+="<tr><td></td>";
			html_data+="<td><strong>"+getLabelRevenue("gross_amount",lang)+"</strong></td>";
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

        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from revenue_shifts where login_time between '"+startdate+"' AND '"+enddate+"'"; 

        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            query=query+" AND device_number in("+device+")";
		if(operator_id.length()>0)
            query=query+" AND operator_id in("+operator_id+")";

        

        query=query+" ORDER BY  device_number ASC"; 
			
        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            difference="";
            
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;            
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>"<<getLabelRevenue("transactions",lang)<<"</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead>"<<endl; 
            Php::out<<"<tr>"<<endl; 
            Php::out<<"<th>#</th>"<<endl; 
            Php::out<<"<th>"<<getLabelRevenue("shift_number",lang)<<"</th>"<<endl; 
            Php::out<<"<th>"<<getLabelRevenue("device_name",lang)<<"</th>"<<endl;        
            Php::out<<"<th>"<<getLabelRevenue("operator_name",lang)<<"</th>"<<endl;  
            Php::out<<"<th>"<<getLabelRevenue("shift_start",lang)<<"</th>"<<endl;  
            Php::out<<"<th>"<<getLabelRevenue("shift_end",lang)<<"</th>"<<endl;    
            Php::out<<"<th>"<<getLabelRevenue("shift_earnings",lang)<<"</th>"<<endl;  
            Php::out<<"<th>"<<getLabelRevenue("over_short",lang)<<"</th>"<<endl;                                               
            Php::out<<"<th>"<<getLabelRevenue("last_updated",lang)<<"</th>"<<endl; 
            Php::out<<"<th>"<<getLabelRevenue("status",lang)<<"</th>"<<endl;                                                        
            
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
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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

		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select * from revenue_shifts where shift_id='"+shift_id+"'";
		res= rStmt->executeQuery(query);
		
		if(res->next())
        {
			Php::out<<"<tr><td>"<<getLabelRevenue("device_name",lang)<<"</td><td> "+res->getString("device_name")+" </td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<getLabelRevenue("operator_name",lang)<<"</td><td>"+res->getString("operator_name")+"</td></tr>"<<endl;    
			Php::out<<"<tr><td>"<<getLabelRevenue("shift_start",lang)<<"</td><td>"+res->getString("login_time")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<getLabelRevenue("shift_end",lang)<<"</td><td>"+res->getString("logout_time")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<getLabelRevenue("open_amount",lang)<<"</td><td>"+res->getString("shift_opening_amount")+"</td></tr>"<<endl;        
			Php::out<<"<tr><td>"<<getLabelRevenue("close_amount",lang)<<"</td><td>"+res->getString("shift_closing_amount")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<getLabelRevenue("payin_amount",lang)<<"</td><td>"+res->getString("payin_amount")+"</td></tr>"<<endl;        
			Php::out<<"<tr><td>"<<getLabelRevenue("payout_amount",lang)<<"</td><td>"+res->getString("payout_amount")+"</td></tr>"<<endl;
			Php::out<<"<tr><td>"<<getLabelRevenue("duration",lang)<<"</td><td>"+res->getString("shift_duration")+"</td></tr>"<<endl;                   
			Php::out<<"<tr><td>"<<getLabelRevenue("parking_fee",lang)<<"</td><td>"+res->getString("parking_fees")+"</td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<getLabelRevenue("lost_fee",lang)<<"</td><td>"+res->getString("lost_fee")+"</td></tr>"<<endl;  
			Php::out<<"<tr><td>"<<getLabelRevenue("vat",lang)<<"</td><td>"+res->getString("vat_amount")+"</td></tr>"<<endl;               
			Php::out<<"<tr><td>"<<getLabelRevenue("shift_earnings",lang)<<"</td><td class='font-weight-bold'>"+res->getString("shift_earnings")+"</td></tr>"<<endl;          
			Php::out<<"<tr><td>"<<getLabelRevenue("physical_cash",lang)<<"</td><td class='font-weight-bold'>"+res->getString("physical_cash_collected")+"</td></tr>"<<endl;       
			if(res->getDouble("physical_cash_collected")>0)                      
				difference=to_string(res->getDouble("physical_cash_collected")-res->getDouble("shift_earnings"));                
			else            
				difference="-";               
			Php::out<<"<tr><td>"<<getLabelRevenue("over_short",lang)<<"</td><td class='font-weight-bold'>"+difference+"</td></tr>"<<endl;
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
		currency = getCurrency();
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select * from revenue_apm_cash_refill where datetime between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            query += " AND device_number IN("+device_number+")";  
		res= rStmt->executeQuery(query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("date_time",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("operator_name",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("banknote",lang)+"</th>";    
            data+="<th>"+getLabelRevenue("amount",lang)+"</th>";  
            
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
            summary+="<h6>"+getLabelRevenue("transactions",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
			
            summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(total)+" "+currency+"</h3>";
            summary+="<h6>"+getLabelRevenue("total_amount_refilled",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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
		rCon=General.mysqlConnect(DashboardDB);
		rStmt=rCon->createStatement();
		query="SELECT a.device_number,a.device_name,a.denomination,a.current_level as recycler_current_level,b.current_level as cashbox_current_level FROM apm_recycle_levels as a join apm_cashbox_levels as b on a.denomination=b.denomination and a.device_number=b.device_number";
		
        if (device_number.length() > 0) 		           
            query += " AND device_number IN("+device_number+")";  
		
		query += " order by denomination asc";
		res= rStmt->executeQuery(query);
		string data="";
		if(res->rowsCount()>0)
		{
            data = "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>";
            data = data+"<thead>";
            data = data+"<tr>"; 
            data = data+ "<th>"+getLabelRevenue("banknotes",lang)+"</th>";        
            data = data+ "<th>"+getLabelRevenue("recycler_level",lang)+"</th>";  
            data = data+ "<th>"+getLabelRevenue("cashbox_level",lang)+"</th>";  
            data = data+ "<th>"+getLabelRevenue("total",lang)+"</th>";               
            data = data+ "</tr>"; 
            data = data+ "</thead><tbody>"; 
			i=0;
            while(res->next())
			{ 
				if(device_number!=res->getString("device_number"))
					{
					if(device_number!="")
						{
						data+="<tr><td></td><td></td><td><h3>"+getLabelRevenue("total",lang)+"</h3></td><td><h3>"+to_string(apm_total)+"</h3></td></tr>";
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
           data+="<tr><td></td><td></td><td><h3>"+getLabelRevenue("total",lang)+"</h3></td><td><h3>"+to_string(apm_total)+"</h3></td></tr>";
           Php::out<<data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select * from revenue_apm_cash_payout where datetime between '"+startdate+"' AND '"+enddate+"'";
		
        if (device_number.length() > 0) 		           
            query += " AND device_number IN("+device_number+")";  
		
		if (carpark.length() > 0) 		           
            query += " AND device_number IN("+device_number+")";  

		res= rStmt->executeQuery(query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
			
			
			
            data = "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>";
            data = data+"<thead>";
            data = data+"<tr>"; 
            data = data+ "<th>"+getLabelRevenue("device_name",lang)+"</th>";        
            data = data+ "<th>"+getLabelRevenue("operation",lang)+"</th>";  
            data = data+ "<th>"+getLabelRevenue("date_time",lang)+"</th>";  
            data = data+ "<th>"+getLabelRevenue("operator",lang)+"</th>";
            data = data+ "<th>"+getLabelRevenue("banknotes",lang)+"</th>";  
            data = data+ "<th>"+getLabelRevenue("amount",lang)+"</th>";               
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
            summary+="<h6>"+getLabelRevenue("transactions",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";

            summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+to_string(total)+"</h3>";
            summary+="<h6>"+getLabelRevenue("total_payout",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
           Php::out<<summary + data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select * from revenue_payment_exceptions where date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            query += " AND device_number IN("+device_number+")"; 

		if (ex_option !="0") 
            query += " AND exception LIKE '"+ex_option+"%'";

		res= rStmt->executeQuery(query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("date_time",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("ticket_id",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("exception",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("amount_received",lang)+"</th>";    
            data+="<th>"+getLabelRevenue("balance_returned",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("credit_note",lang)+"</th>";    
            data+="<th>"+getLabelRevenue("banknotes",lang)+"</th>";  
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
            summary+="<h6>"+getLabelRevenue("transactions",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select * from revenue_creditcard_payments where  date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (device_number.length() > 0) 		           
            query += " AND device_number IN("+device_number+")"; 


		res= rStmt->executeQuery(query);
		string data="";
		string summary="";
		if(res->rowsCount()>0)
		{
            data+="<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("date_time",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("merchant_id",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("terminal_id",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("ticket_id",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("comment",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("card_type",lang)+"</th>";    
            data+="<th>"+getLabelRevenue("amount",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("authentication_code",lang)+"</th>";    
            data+="<th>"+getLabelRevenue("reference_number",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("status",lang)+"</th>";  
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
            summary+="<h6>"+getLabelRevenue("transactions",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
			
			summary+="<div class='col-lg-3 col-6'>";                       
            summary+="<div class='small-box bg-success'>";
            summary+="<div class='inner'>";
            summary+="<h3>"+SetDoublePrecision(total)+"</h3>";
            summary+="<h6>"+getLabelRevenue("total_revenue",lang)+"</h6>";
            summary+="</div>";            
            summary+="</div>";
            summary+="</div>";
            summary+="</div>";
			
			Php::out<<summary+data<<endl;
		}
		else{
            Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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


string revenuesummaryReport(string startdate,string enddate,string weekdays,string carpark,string lang)
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
		query="select sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_fee,sum(gross_amount) as earnings,sum(vat_amount) as vat,sum(product_sale_amount) as product_sale,sum(discount_amount) as discount_amount from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(report_date) IN("+weekdays+")"; 

		query += " AND device_number > 0 ORDER BY device_number ASC";
		rs= stmt->executeQuery(query);
		while(rs->next())
		{
			data+="<span id='chart-data' class='d-none' data-currency="+currency+" data-parking-fee='"+SetDoublePrecision(rs->getDouble("parking_fee"))+"' data-lost-ticket='"+SetDoublePrecision(rs->getDouble("lost_fee"))+"' data-product-sale='"+SetDoublePrecision(rs->getDouble("product_sale"))+"' data-vat='"+SetDoublePrecision(rs->getDouble("vat"))+"'></span>";

			data+="<div class='row mb-4 jspdf-graph'>";
			data+="<div class='col-auto'>";    
			data+="<div class='small-box bg-success'>";
			data+="<div class='inner'>";
			data+="<h3>"+currency+" "+SetDoublePrecision(rs->getDouble("earnings"))+"</h3>";
			data+="<p>"+getLabelRevenue("total_revenue",lang)+"</p>";
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
			data+="<p>"+getLabelRevenue("parking_fee",lang)+"</p>";
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
			data+="<p>"+getLabelRevenue("lost_fee",lang)+"</p>";
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
			data+="<p>"+getLabelRevenue("product_sale_amount",lang)+"</p>";
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
			data+="<p>"+getLabelRevenue("discount_amount",lang)+"</p>";
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

string revenuedayclosurecomment(string startdate,string enddate,string weekdays,string carpark,string lang)
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
		query="select dayname(report_date) as day,report_date,comments,carpark_name,device_name from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(report_date) IN("+weekdays+")"; 

		query += " AND reproccessing_flag=1 ORDER BY device_number ASC";
		rs= stmt->executeQuery(query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("notifications",lang)+"</div>";
            data+="<table id='TABLE_1' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("report_date",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("carpark_number",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("comment",lang)+"</th>";  
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



string revenuesummaryReportbydate(string startdate,string enddate,string weekdays,string carpark,string lang)
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
		query="select report_date,dayname(report_date) as day,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as discount_amount,sum(payable_entries_count) as payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(report_date) IN("+weekdays+")"; 

		query += " GROUP BY report_date ORDER BY report_date DESC";
		rs= stmt->executeQuery(query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("report_by_date",lang)+"</div>";
            data+="<table id='TABLE_2' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("report_date",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("total_earnings",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("parking_fee",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("lost_fee",lang)+"</th>";  
			data+="<th>"+getLabelRevenue("product_sale_amount",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("cash_amount",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("creditcard_amount",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("wallet_amount",lang)+"</th>";  
			data+="<th>"+getLabelRevenue("payable_entries_count",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("lost_ticket_count",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("discount_count",lang)+"</th>";  
			data+="<th>"+getLabelRevenue("product_sale_count",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("cash_transaction_count",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("creditcard_transaction_count",lang)+"</th>";  
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


string productsaleReport(string startdate,string enddate,string weekdays,string carpark,string lang)
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
		query="select device_name,product_name,MIN(carpark_number) as carpark_number,carpark_name,sum(product_quantity) as product_quantity,unit_price,sum(product_sale_amount) as product_sale from revenue_product_usage where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(report_date) IN("+weekdays+")"; 

		query += " GROUP BY device_name,product_id ORDER BY carpark_number ASC";
		rs= stmt->executeQuery(query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("product_sales",lang)+"</div>";
            data+="<table id='TABLE_5' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("device_name",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("product_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("quantity",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("unit_price",lang)+"</th>";  
			data+="<th>"+getLabelRevenue("total_amount",lang)+"</th>"; 
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

string discountusagesummary(string startdate,string enddate,string weekdays,string carpark,string lang)
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
		query="select device_name,discount_name,MIN(carpark_number) as carpark_number,carpark_name,count(id) as discount_usage,sum(discount_amount) as discount_amount from revenue_payments where discount_amount > 0  and payment_date_time between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(payment_date_time) IN("+weekdays+")"; 

		query += " AND device_number > 0 GROUP BY device_name,discount_id ORDER BY carpark_number ASC";
		rs= stmt->executeQuery(query);
		if(rs->rowsCount()>0)
		{
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("discounts",lang)+"</div>";
            data+="<table id='TABLE_5' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>"; 
            data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
			data+="<th>"+getLabelRevenue("device_name",lang)+"</th>"; 
            data+="<th>"+getLabelRevenue("discount_name",lang)+"</th>";        
            data+="<th>"+getLabelRevenue("discount_count",lang)+"</th>";  
            data+="<th>"+getLabelRevenue("discount_amount",lang)+"</th>";  
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
		currency = getCurrency();
		rCon=General.mysqlConnect(ReportingDB);
		rStmt=rCon->createStatement();
		query="select device_name,carpark_number,carpark_name,sum(gross_amount) as gross_amount,sum(parking_fee) as parking_fee,sum(lost_ticket_fee) as lost_ticket_fee,sum(vat_amount) as vat_amount,sum(product_sale_amount) as product_sale_amount,sum(discount_amount) as discount_amount,sum(payable_entries_count) as payable_entries_count,sum(lost_ticket_count) as lost_ticket_count,sum(discount_count) as discount_count,sum(creditcard_payment_count) as creditcard_count,sum(sms_payment_count) as sms_count,sum(creditcard_payment_amount) as creditcard_amount,sum(gross_amount-creditcard_payment_amount) as cash_amount,sum(sms_payment_amount) as sms_amount,sum(product_sale_count) as product_sale_count from summary_daily_revenue where report_date between '"+startdate+"' and '"+enddate+"'";
		if (carpark.length() > 0) 		           
            query += " AND carpark_number IN("+carpark+")";   
  
        if (weekdays.length() > 0) 		           
            query += " AND dayname(report_date) IN("+weekdays+")"; 
		
        query+=" AND device_number > 0 GROUP BY device_number ORDER BY carpark_number ASC";   

		res= rStmt->executeQuery(query);
		string data="";
		if(res->rowsCount()>0)
		{
			Php::out<<revenuesummaryReport(startdate,enddate,weekdays,carpark,lang)<<endl;
			Php::out<<revenuedayclosurecomment(startdate,enddate,weekdays,carpark,lang)<<endl;
			Php::out<<revenuesummaryReportbydate(startdate,enddate,weekdays,carpark,lang)<<endl;
			
			/////////////////////////
            //Earnings
            ////////////////////////
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("earnings",lang)+"</div>";
            data+="<table id='TABLE_3' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";
            data+="<th>"+getLabelRevenue("parking_fee",lang)+"</th>";
            data+="<th>"+getLabelRevenue("lost_fee",lang)+"</th>";
            data+="<th>"+getLabelRevenue("product_sale_amount",lang)+"</th>";
            data+="<th>"+getLabelRevenue("total_earnings",lang)+"</th>";
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
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("earnings_by_payment_type",lang)+"</div>";
            data+="<table id='TABLE_4' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";
            data+="<th>"+getLabelRevenue("cash",lang)+"</th>";
            data+="<th>"+getLabelRevenue("credit_card",lang)+"</th>";
            data+="<th>"+getLabelRevenue("sms",lang)+"</th>";
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
			Php::out<<productsaleReport(startdate,enddate,weekdays,carpark,lang)<<endl;

            //////////////////////
            //Discounts report
            ///////////////////
            Php::out<<discountusagesummary(startdate,enddate,weekdays,carpark,lang)<<endl;
			
			
            ////////////////////////
            //Transaction Count
            ////////////////////////
			data="";
			carpark_displayed_flag=0;
			data+="<div class='header pl-0 mt-4 mb-3'>"+getLabelRevenue("transaction_count",lang)+"</div>";
            data+="<table id='TABLE_6' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            //data+="<tr class='card-header d-flex justify-content-between align-items-center'>";
			data+="<thead>"; 
            data+="<tr>"; 
            data+="<th>#</th>"; 
            data+="<th>"+getLabelRevenue("device_name",lang)+"</th>";
            data+="<th>"+getLabelRevenue("payable_entries",lang)+"</th>";
            data+="<th>"+getLabelRevenue("lost_ticket",lang)+"</th>";
            data+="<th>"+getLabelRevenue("discounts",lang)+"</th>";
            data+="<th>"+getLabelRevenue("product_sales",lang)+"</th>";
            data+="<th>"+getLabelRevenue("cash",lang)+"</th>";
            data+="<th>"+getLabelRevenue("credit_card",lang)+"</th>";
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
			Php::out<<"<div class='p-0'>"<<getLabelRevenue("no_records",lang)<<"</div>"<<endl;
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