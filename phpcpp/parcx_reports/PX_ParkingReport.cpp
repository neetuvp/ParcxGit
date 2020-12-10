#include "PX_GeneralOperations.h"
#include "PX_ParkingReport.h"
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
string entry_type,parking_duration;      
string image,croppedImage,sceneImage,datefolder; 
string current_date_time;   
sql::ResultSet *res,*img,*res2;  
size_t n;
int i=0;

sql::Connection *rCon,*con;
sql::Statement *rStmt,*stmt;

void writeParkingReportLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-ParkingReport-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeParkingReportException(string function,string message)
    {
    Php::out<<message<<endl;   
    Php::out<<query<<endl;     
    General.writeLog("WebApplication/ExceptionLogs/PX-ParkingReport-"+General.currentDateTime("%Y-%m-%d"),function,message);        
    }

inline string toString(Php::Value param)
	{
	string value=param;
	return value;
	}

void ParkingReport::openTransactionReport(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];
        string device=json["device"];
        string val=json["plate_number"];
        string delay=json["parking_duration"];
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from open_transactions where entry_date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"' and movement_type=1";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            query=query+" AND device_number in("+device+")";
        if(delay!="")
            {
            if(delay=="0")
                query=query+" AND entry_date_time>=DATE_SUB(NOW(),INTERVAL 1 Day)";
            else
                query=query+" AND entry_date_time<=DATE_SUB(NOW(),INTERVAL "+delay+" Day)";            
                
            }
        if(val.length()>0)
            query=query+" AND(plate_number like '%"+val+"%' OR ticket_id like '%"+val+"%')";

        query=query+" ORDER BY  entry_date_time desc";

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th>Device Name</th>"<<endl;	
             Php::out<<"<th>Plate Number</th>"<<endl;
            Php::out<<"<th>Ticket Id</th>"<<endl;                       
            Php::out<<"<th>Entry type</th>"<<endl;		            
            Php::out<<"<th>Entry DateTime</th>"<<endl;
            Php::out<<"<th>Parking Duration</th>"<<endl;
            Php::out<<"<th>Entry Grace Period</th>"<<endl;
            Php::out<<"<th>Total Amount Paid</th>"<<endl;
            Php::out<<"<th>Payment Date Time</th>"<<endl;
            Php::out<<"<th>Exit Grace Period</th>"<<endl;
            Php::out<<"<th>IMAGE</th>"<<endl;
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            while(res->next())
                {
                if(res->getInt("entry_type")==1)
                    entry_type="Short Term";
                if(res->getInt("entry_type")==2)
                    entry_type="Access";
                if(res->getInt("entry_type")==3)
                    entry_type="Reservation";	

                croppedImage="";
                sceneImage="";
                
                if(res->getString("plate_captured_id")!="")
                    {
                    query="select * from plates_captured where id="+res->getString("plate_captured_id");
                    img= rStmt->executeQuery(query);
                    
                    if(img->next())
                        {
                        image=img->getString("plate_image_name");                   
                        while((n=image.find(" "))!=std::string::npos)
                            image.replace(n,1,"%20");
                        while((n=image.find("#"))!=std::string::npos)
                            image.replace(n,1,"%23");  

                        datefolder=img->getString("capture_date_time");
                        datefolder=datefolder.substr(0,10);
                        
                        croppedImage=img->getString("camera_device_number")+"/"+datefolder+"/Crop_"+image;
                        sceneImage=img->getString("camera_device_number")+"/"+datefolder+"/Scene_"+image;
                        delete img;
                        }	

                    }
                

                int *duration=General.differenceDateTime(current_date_time,res->getString("entry_date_time"),dateTimeFormat);
                if(duration[1]>0)	
                    parking_duration=to_string(duration[1])+" D "+to_string(duration[2])+" H "+to_string(duration[3])+" M";			
                else
                    parking_duration=to_string(duration[2])+" H "+to_string(duration[3])+" M";			
                
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td> "+res->getString("device_name")+" </td>"<<endl;   
                Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;                
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;    
                Php::out<<"<td>"+entry_type+"</td>"<<endl;               
                Php::out<<"<td>"+res->getString("entry_date_time")+"</td>"<<endl;                                
                Php::out<<"<td>"+parking_duration+"</td>"<<endl;                 
                Php::out<<"<td>"+res->getString("entry_grace_period")+"</td>"<<endl;
                Php::out<<"<td>"+res->getString("total_amount_paid")+"</td>"<<endl;
                Php::out<<"<td>"+res->getString("last_payment_date_time")+"</td>"<<endl;
                Php::out<<"<td>"+res->getString("max_exit_date_time")+"</td>"<<endl;
                Php::out<<"<td><img src='"+toString(json["image_location"])+"/"+croppedImage+"' width='100' height='50' data-value ='"+sceneImage+"' data-toggle='modal' data-target='#image-modal' alt='"+res->getString("plate_number")+"'></td>"<<endl;                
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
        writeParkingReportException("openTransactionReport",e.what());        
        }
    }

void ParkingReport::parkingMovementsReport(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];
        string device=json["device"];
        string val=json["plate_number"];
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from parking_movements where date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"'";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            query=query+" AND device_number in("+device+")";
        if(val.length()>0)
            query=query+" AND(plate_number like '%"+val+"%' OR ticket_id like '%"+val+"%')";

        query=query+" ORDER BY  date_time desc";

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th></th>"<<endl;             
            Php::out<<"<th>DateTime</th>"<<endl;             
            Php::out<<"<th>Plate Number</th>"<<endl;
            Php::out<<"<th>Ticket Id</th>"<<endl;    
            //Php::out<<"<th>Entry Grace Period</th>"<<endl;                                            
            Php::out<<"<th>Device Name</th>"<<endl;	  
            //Php::out<<"<th>Carpark Name</th>"<<endl;	  
                    
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            i=0;
            while(res->next())
                { 
                i++;                   
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("date_time")+"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;                
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;  
                //Php::out<<"<td>"+res->getString("entry_grace_period")+"</td>"<<endl;                     
                Php::out<<"<td> "+res->getString("device_name")+" </td>"<<endl;                                                             
                //Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;                
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
        writeParkingReportException("parkingMovementsReport",e.what());        
        }
    }

void ParkingReport::blacklistReport(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];       
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from parking_blacklist where date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"'";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";        
        query=query+" ORDER BY  date_time desc";

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th></th>"<<endl;             
            Php::out<<"<th>DateTime</th>"<<endl;                         
            Php::out<<"<th>Ticket Id</th>"<<endl;    
            Php::out<<"<th>Device Name</th>"<<endl;	  
            Php::out<<"<th>Blacklisting Reason</th>"<<endl;             
            Php::out<<"<th>Desription</th>"<<endl;	                      
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            i=0;
            while(res->next())
                { 
                i++;                   
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("date_time")+"</td>"<<endl;                              
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;                    
                Php::out<<"<td> "+res->getString("device_name")+" </td>"<<endl;                                                             
                Php::out<<"<td>"+res->getString("blacklisting_reason")+"</td>"<<endl;                   
                Php::out<<"<td>"+res->getString("blacklisting_description")+"</td>"<<endl;                
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
        writeParkingReportException("blacklistReport",e.what());        
        }
    }

void ParkingReport::validationReport(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];
        string validator=json["validator"];
        string product=json["product"];
        string val=json["plate_number"];
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from parking_validation where date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"'";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(validator.length()>0)
            query=query+" AND validator_id in("+validator+")";
        if(product.length()>0)
            query=query+" AND product_id in("+product+")";
        if(val.length()>0)
            query=query+" AND(plate_number like '%"+val+"%' OR ticket_id like '%"+val+"%')";

        query=query+" ORDER BY  date_time desc";

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;           
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th></th>"<<endl;             
            Php::out<<"<th>Entry DateTime</th>"<<endl;             
            Php::out<<"<th>Plate Number</th>"<<endl;
            Php::out<<"<th>Ticket Id</th>"<<endl;    
            Php::out<<"<th>Validator</th>"<<endl;                                            
            Php::out<<"<th>Product</th>"<<endl;	                                  
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            i=0;
            while(res->next())
                { 
                i++;                   
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("date_time")+"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;                
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;  
                Php::out<<"<td>"+res->getString("validator_name")+"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("product_name")+"</td>"<<endl; 
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
        writeParkingReportException("parkingMovementsReport",e.what());        
        }
    }

void ParkingReport::accessReport(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];
        string device=json["device"];
        string val=json["plate_number"];
        string option=json["option"];       
        string user_role=json["user_role"];
        string user_name="";
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from parking_movements_access where date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"'";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";
        if(device.length()>0)
            query=query+" AND device_number in("+device+")";              
        if(option.length()>0) 
            query=query+" AND status_message like '%"+option+"%'";
        if(val.length()>0)                          
            query=query+" AND(plate_number like '%"+val+"%' OR ticket_id like '%"+val+"%' OR tag like '%"+val+"%')";
           
            
        query=query+" ORDER BY  date_time desc";

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            con=General.mysqlConnect(ServerDB);
            stmt=con->createStatement();
            query="select * from access_whitelist";
            res2=stmt->executeQuery(query);

            delete stmt;           
            con->close();
            delete con;

            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;           
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th></th>"<<endl;             
            Php::out<<"<th>DateTime</th>"<<endl;    
            Php::out<<"<th>Customer Name</th>"<<endl;
            Php::out<<"<th>Plate Number</th>"<<endl;
            Php::out<<"<th>Ticket Id</th>"<<endl;                   
            Php::out<<"<th>Tag</th>"<<endl;                                                   
            Php::out<<"<th>Status</th>"<<endl;                                            
            Php::out<<"<th>Device Name</th>"<<endl;	  
            Php::out<<"<th>Carpark Name</th>"<<endl;            	  
            Php::out<<"<th></th>"<<endl;
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            i=0;
            string status,tag;
            int whitelist_added;
            while(res->next())
                { 
                i++;  
                status=res->getString("status_message");                                
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("date_time")+"</td>"<<endl; 
                res2->beforeFirst();
                user_name="";
                while(res2->next())
                    {
                    if(res->getString("ticket_id")==res2->getString("ticket_id"))
                        {
                        user_name=res2->getString("customer_name");
                        break;
                        }
                    }
                Php::out<<"<td>"+user_name+"</td>"<<endl;                                        
                Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;                
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;                   
                Php::out<<"<td>"+res->getString("tag")+"</td>"<<endl;                                                                       
                Php::out<<"<td>"+res->getString("status_message")+"</td>"<<endl;                     
                Php::out<<"<td> "+res->getString("device_name")+" </td>"<<endl;                                                             
                Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;  
                tag=res->getString("tag"); 
                whitelist_added=res->getInt("whitelist_added");
                if(status=="TAG UNKNOWN" && whitelist_added==0 && tag.length()>2)
                    {                                                                                                                         
                    //Php::out<<"<td><input data-toggle='modal' data-target='#addToWhiteList' type='botton' class='btn btn-warning btnWhitelistAdd' value='Add to whitelist' data-tag='"<<res->getString("tag")<<"' data-id="<<res->getInt("id")<<"></td>";                        
                     Php::out << "<td><button type='button' class='col btn btn-warning btnWhitelistAdd' data-tag='"<<res->getString("tag")<<"' data-id="<<res->getInt("id")<<">Add to whitelist</button></td>"<< std::endl;           
                    }     
                else
                    Php::out<<"<td></td>"<<endl; 
                     
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
        delete res2;       
        rCon->close();
        delete rCon;
        }
    catch(const std::exception& e)
        {
        writeParkingReportException("accessReport",e.what());        
        }
    }

void ParkingReport::trackTicket(Php::Value json)
    {
    try
        {        
        string carpark=json["carpark"];       
        string val=json["plate_number"];
        string type=json["type"];
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        query="select * from parking_movements where movement_type=1 and date_time between '"+toString(json["from"])+"' and '"+toString(json["to"])+"'";
        if(carpark.length()>0)
            query=query+" AND carpark_number in("+carpark+")";

        if(val.length()>0)
            query=query+" AND(plate_number like '%"+val+"%' OR ticket_id like '%"+val+"%')";

        query=query+" ORDER BY  date_time desc";

        //Php::out<<query<<endl;

        res= rStmt->executeQuery(query);
        if(res->rowsCount()>0)
            {
            Php::out<<"<div class='row mb-4 jspdf-graph'>"<<endl;
            Php::out<<"<div class='col-lg-3 col-6'>"<<endl;                       
            Php::out<<"<div class='small-box bg-success'>"<<endl;
            Php::out<<"<div class='inner'>"<<endl;
            Php::out<<"<h3>"<<res->rowsCount()<<"</h3>"<<endl;
            Php::out<<"<h6>Transactions</h6>"<<endl;
            Php::out<<"</div>"<<endl;            
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;
            Php::out<<"</div>"<<endl;

            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th></th>"<<endl; 
            Php::out<<"<th>Ticket Id</th>"<<endl;   
            Php::out<<"<th>Plate Number</th>"<<endl;             
            Php::out<<"<th>Entry Date Time</th>"<<endl; 
            Php::out<<"<th>Entry Device</th>"<<endl;
             Php::out<<"<th>Exit Date Time</th>"<<endl; 
            Php::out<<"<th>Exit Device</th>"<<endl;                                                
            Php::out<<"<th>Carpark Name</th>"<<endl;	  
                    
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;
            i=0;
            while(res->next())
                { 
                i++;                   
				Php::out<<"<tr>"<<endl; 
                Php::out<<"<td>"<<i<<"</td>"<<endl; 
                Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<<endl;  
                Php::out<<"<td>"+res->getString("plate_number")+"</td>"<<endl;
                Php::out<<"<td>"+res->getString("date_time")+"</td>"<<endl; 
                Php::out<<"<td> "+res->getString("device_name")+" </td>"<<endl; 
                if(res->getString("ticket_id")!="")
                    {
                    query="Select * from parking_movements where movement_type=2 and ticket_id = '" + res->getString("ticket_id") + "' and date_time>'"+res->getString("date_time")+"'";
                    res2= rStmt->executeQuery(query);
                    if(res2->next())
                        {
                        Php::out<<"<td>"+res2->getString("date_time")+"</td>"<<endl; 
                        Php::out<<"<td> "+res2->getString("device_name")+" </td>"<<endl;
                        delete res2;
                        }
                    else
                        {
                        Php::out<<"<td></td><td></td>"<<endl;
                        }
                    
                    }                                                                                                                                                                       
                Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;                
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
        writeParkingReportException("parkingMovementsReport",e.what());        
        }
    }

void ParkingReport::visitorFrequencyReport(Php::Value json)
    {	
	string visit_type =json["visit_type"];
	try
        {
        int n=0;
        string id = "0";		
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
		
		if(visit_type=="all")		    
			query = "Select * from visitor_frequency";			
		else            
			query = "Select * from visitor_frequency where category = '"+visit_type+"'";					    		
        res = rStmt->executeQuery(query);
        if(res->rowsCount()>0)			
		    {
            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th>No</th>"<<endl;	
            Php::out<<"<th>Plate Number</th>"<<endl;                               
            Php::out<<"<th>Ticket</th>"<<endl;	
            if(visit_type!="shortterm")	            
                Php::out<<"<th>Tag</th>"<<endl;

            Php::out<<"<th>Last 7 days</th>"<< std::endl;
			Php::out<<"<th>Last 30 days</th>"<< std::endl;
			Php::out<< "<th>Last 60 days</th>"<< std::endl;
			Php::out<<"<th>Last 90 days</th>"<< std::endl;
			Php::out<<"<th>Last 120 days</th>"<< std::endl;
			Php::out<<"<th>Last 150 days</th>"<< std::endl;
			Php::out<<"<th>Last 180 days</th>"<< std::endl;
			if(visit_type=="all")			    
				Php::out<<"<th>Category</th>"<< std::endl;			    
			Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;                                    
			n=1;
			
			while (res->next())
			    {		
				Php::out<<"<tr>"<< std::endl;                
                Php::out<< "<td>"+to_string(n)+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("plate_number")+"</td>"<< std::endl;			
				Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<< std::endl;
				if(visit_type!="shortterm")				    
					Php::out<<"<td>"+res->getString("tag_id")+"</td>"<< std::endl;
				    
                Php::out<< "<td>"+res->getString("d7")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d30")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d60")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d90")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d120")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d150")+"</td>"<< std::endl;
				Php::out<< "<td>"+res->getString("d180")+"</td>"<< std::endl;
				if(visit_type=="all")				    
					Php::out<< "<td>"+res->getString("category")+"</td>"<< std::endl;				    
                Php::out<< "</tr>"<< std::endl;               
                n++;		
			    }
			Php::out<<"</tbody></table>"<<endl;
		    }
        else
            {
            Php::out<<"<div class='p-0'>No records/transactions available for the current search criteria</div>"<<endl;
            }
		
		delete res;
		delete rStmt;					
		delete rCon;

		
    }
    catch(const std::exception& e)
    {
        writeParkingReportException("visitorFrequencyReport",e.what());
    }
}

void ParkingReport::visitorFrequencyRealTimeReport(Php::Value json)
    {		
	string from_date = json["fromDate"];
	string to_date = json["toDate"];
	string visit_type = json["visit_type"];
	string contract_type = json["contract_type"];
	string search_text = json["search_text"];	
    try
        {
        int n=0;
        string id = "0";
        current_date_time=General.currentDateTime(dateTimeFormat); 
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
		if(visit_type=="shortterm")
		    {
			if(search_text=="")			    
				query = "Select count(plate_number) as count,plate_number,ticket_id from parking_movements where date_time>='"+from_date+"' and date_time<='"+to_date+"' group by plate_number";			    
			else                
				query = "Select count(plate_number) as count,plate_number,ticket_id from parking_movements where date_time>='"+from_date+"' and date_time<='"+to_date+"' and plate_number='"+search_text+"'";			    
		    }
		else
            {
			if(contract_type=="anpr")
			    {
				if(search_text=="")				    
					query = "Select count(plate_number) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"'  group by ticket_id,tag";							    
				else                    
					query = "Select count(plate_number) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"' and plate_number='"+search_text+"' group by ticket_id,tag";												    
			    }
			else if(contract_type=="ticketid")
			    {
				if(search_text=="")    
	    			query = "Select count(ticket_id) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"' group by ticket_id,plate_number";				                
				else                    
					query = "Select count(ticket_id) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"' and ticket_id='"+search_text+"' group by ticket_id,plate_number";				    
			    }
			else if(contract_type=="tag")
			    {
				if(search_text=="")				
					query = "Select count(tag) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"'  and tag>'' group by tag,plate_number";							
				else
					query = "Select count(tag) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"' and tag='"+search_text+"' and tag>'' group by tag,plate_number";							
			    }
			else
                {				
				query = "Select count(*) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='"+from_date+"' and date_time<='"+to_date+"' group by plate_number,ticket_id,tag";
			    }
		    }
		
        res = rStmt->executeQuery(query);
        if(res->rowsCount()>0)			
		    {				
            
            Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
			Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th>No</th>"<< std::endl;				
            Php::out<<"<th>Plate</th>"<< std::endl;
            Php::out<<"<th>Ticket</th>"<< std::endl;			
			if(visit_type=="contract")			
				Php::out<<"<th>Tag</th>"<< std::endl;
			
            Php::out<<"<th>No of Visits</th>"<< std::endl;
            Php::out<<"</tr></thead>"<<endl;
			Php::out<<"<tbody>"<<endl;  
			n=1;
			
			while (res->next())
			    {		
				Php::out<<"<tr>"<< std::endl;               
                Php::out<< "<td>"+to_string(n)+"</td>"<< std::endl;				
			    Php::out<< "<td>"+res->getString("plate_number")+"</td>"<< std::endl;
				Php::out<<"<td>"+res->getString("ticket_id")+"</td>"<< std::endl;
				
				if(visit_type=="contract")				    
					Php::out<<"<td>"+res->getString("tag")+"</td>"<< std::endl;				
                Php::out<< "<td>"+res->getString("count")+"</td>"<< std::endl;                       
                Php::out<< "</tr>"<< std::endl;                
                n++;		
			    }
			 Php::out<<"</tbody></table>"<<endl;
            }
        else
            {
            Php::out<<"<div class='p-0'>No records/transactions available for the current search criteria</div>"<<endl;
            }
		
		delete res;
		delete rStmt;
		delete rCon;		
        }
    catch(const std::exception& e)
        {
        writeParkingReportException("visitorFrequencyRealTimeReport",e.what());
        }    
    }

void ParkingReport::ticketDetails(Php::Value json)
    {
    try
        {
        sql::ResultSet *entry,*exit,*access,*reservation,*payment;    
        string   ticket=json["ticket_id"];  
        string status="Exited";
        string type="";
        int entry_type=0;
        string entry_date_time="",exit_date_time="",parking_duration="",entry_grace_period="",payment_date_time="",exit_grace_period="",amount_paid="",plate_number="";
        rCon=General.mysqlConnect(ReportingDB);
        rStmt=rCon->createStatement();
        current_date_time=General.currentDateTime(dateTimeFormat);  
        query="select * from open_transactions where movement_type=1 and ticket_id='"+ticket+"'";  
        entry= rStmt->executeQuery(query);
        if(entry->next())    
            {
            entry_type=entry->getInt("entry_type");    
            plate_number=entry->getString("plate_number");
            if(entry_type==1)
                type="Shortterm entry";    
            else if(entry_type==2)            
                type="Access parking";
            else
                type="Reservation parking";    

            if(entry_type==2)
                {
                query="select * from parcx_server.access_whitelist where ticket_id='"+ticket+"'";  
                access= rStmt->executeQuery(query);   
                if(access->next())
                    entry_grace_period=access->getString("validity_expiry_date");
                }   
            if(entry_type==3)
                {
                query="select * from parking_reservation where ticket_id='"+ticket+"'";  
                reservation= rStmt->executeQuery(query);  
                if(reservation->next())
                    entry_grace_period=reservation->getString("to_time");
                }                     
                          
            entry_date_time=entry->getString("entry_date_time"); 
               
            int *duration=General.differenceDateTime(current_date_time,entry_date_time,dateTimeFormat);
            if(duration[1]>0)	
                parking_duration=to_string(duration[1])+" D "+to_string(duration[2])+" H "+to_string(duration[3])+" M";			
            else
                parking_duration=to_string(duration[2])+" H "+to_string(duration[3])+" M"; 
            
            if(entry_type==1)
                entry_grace_period=entry->getString("entry_grace_period"); 

            int *entry_grace=General.differenceDateTime(entry_grace_period,current_date_time,dateTimeFormat);
            if(entry_grace[0]>0)	
                {
                status="Within entry grace period";    
                }
            else
                {
                status="Unpaid";    
                payment_date_time=entry->getString("last_payment_date_time");                
                if(payment_date_time!="")
                    {
                    exit_grace_period=entry->getString("max_exit_date_time");
                    amount_paid=entry->getString("total_amount_paid");
                    int *exit_grace=General.differenceDateTime(exit_grace_period,current_date_time,dateTimeFormat);
                    if(exit_grace[0]>0)	
                        {
                        status="Within exit grace period";    
                        }
                    else
                        {
                        status="Maximum exit grace period exceeded";
                        }
                    query="select * from revenue_payments where ticket_id='"+ticket+"' and payment_date_time between '"+entry_date_time+"' and current_timestamp";  
                    payment= rStmt->executeQuery(query);                    
                    }                          
                }
            }  
        else
            {                
            query="select * from parking_movements where movement_type=1 and ticket_id='"+ticket+"'";        
            entry= rStmt->executeQuery(query);
            if(entry->next())
                {
                type="Shortterm entry";  
                entry_type=1;  
                }
            else
                {
                query="select * from parking_movements_access where movement_type=1 and ticket_id='"+ticket+"' and status_message='Access Allowed'";        
                entry= rStmt->executeQuery(query);  
                if(entry->next())
                    {
                    type="Access parking";    
                    entry_type=2;    
                    }
                else
                    {                                                        
                    query="select * from parking_movements_reservation where movement_type=1 and ticket_id='"+ticket+"'";        
                    entry= rStmt->executeQuery(query);    
                    if(entry->next())
                        {
                        type="Reservation parking";
                        entry_type=3;  
                        }
                    }
                }
            if(entry_type!=0)
                {                   
                entry_date_time=entry->getString("date_time");
                plate_number=entry->getString("plate_number");

                if(entry_type==2)
                    {
                    query="select * from parcx_server.access_whitelist where ticket_id='"+ticket+"'";  
                    access= rStmt->executeQuery(query);   
                    if(access->next())
                        entry_grace_period=access->getString("validity_expiry_date");
                    }   
                if(entry_type==3)
                    {
                    query="select * from parking_reservation where ticket_id='"+ticket+"'";  
                    reservation= rStmt->executeQuery(query);  
                    if(reservation->next())
                        entry_grace_period=reservation->getString("to_time");
                    } 

                if(entry_type==1)
                    query="select * from parking_movements where movement_type=2 and ticket_id='"+ticket+"'";      
                else if(entry_type==2)
                    query="select * from parking_movements_access where movement_type=2 and ticket_id='"+ticket+"' and status_message='Access Allowed'";  
                else 
                    query="select * from parking_movements_reservation where movement_type=2 and ticket_id='"+ticket+"'";     
                exit= rStmt->executeQuery(query);
                if(exit->next())
                    {
                    exit_date_time=exit->getString("date_time");
                    int *duration=General.differenceDateTime(exit_date_time,entry_date_time,dateTimeFormat);
                    if(duration[1]>0)	
                        parking_duration=to_string(duration[1])+" D "+to_string(duration[2])+" H "+to_string(duration[3])+" M";			
                    else
                        parking_duration=to_string(duration[2])+" H "+to_string(duration[3])+" M"; 
                    }

                query="select * from revenue_payments where ticket_id='"+ticket+"' and payment_date_time between '"+entry_date_time+"' and '"+exit_date_time+"'";  
                payment= rStmt->executeQuery(query);   
                if(payment->rowsCount()>0)
                    amount_paid="0"; 
                }
                         
            }
                    
        if(entry_type!=0)
            {               
            Php::out<<"Status: "<<status<<"<br>"<<endl;    
            Php::out<<"<h3>Entry details</h3>"<<endl;
            Php::out<<"Entry type: "<<type<<"<br>"<<endl;           
            Php::out<<"Plate number: "<<plate_number<<"<br>"<<endl;
            Php::out<<"Entry date time: "<<entry_date_time<<"<br>"<<endl;    
            Php::out<<"Entry grace period: "<<entry_grace_period<<"<br>"<<endl;       
            Php::out<<"Entry device name: "<<entry->getString("device_name")<<"<br>"<<endl;          
            Php::out<<"Entry carpark name: "<<entry->getString("carpark_name")<<"<br>"<<endl;

            delete entry;

            if(entry_type==2 && access->rowsCount()>0)
                {
                Php::out<<"<h3>Access details</h3>"<<endl; 
                Php::out<<"Customer name: "<<access->getString("customer_name")<<"<br>"<<endl;   
                Php::out<<"Validity start date: "<<access->getString("validity_start_date")<<"<br>"<<endl;   
                Php::out<<"Validity expiry rate: "<<access->getString("validity_expiry_date")<<"<br>"<<endl;    
                delete access;               
                }

            if(entry_type==3 && reservation->rowsCount()>0)
                {
                Php::out<<"<h3>Reservation details</h3>"<<endl; 
                Php::out<<"Customer name: "<<reservation->getString("name")<<"<br>"<<endl;   
                Php::out<<"Validity start date: "<<reservation->getString("from_time")<<"<br>"<<endl;   
                Php::out<<"Validity expiry rate: "<<reservation->getString("to_time")<<"<br>"<<endl;    
                delete reservation;               
                }

            if(amount_paid!="")
                {
                Php::out<<"<h3>Payment details</h3>"<<endl;
                if(payment->next())
                    {
                    Php::out<<"Payment date time: "<<payment->getString("payment_date_time")<<"<br>"<<endl;    
                    Php::out<<"Maximum exit date time: "<<payment->getString("max_exit_date_time")<<"<br>"<<endl;                   
                    Php::out<<"Payment device name: "<<payment->getString("device_name")<<"<br>"<<endl;          
                    Php::out<<"Payment carpark name: "<<payment->getString("carpark_name")<<"<br>"<<endl;   
                    Php::out<<"Amount paid: "<<payment->getString("gross_amount")<<"<br>"<<endl;   
                    if(payment->getInt("discount_id")!=0)
                        {
                        Php::out<<"Discount name: "<<payment->getString("discount_name")<<"<br>"<<endl;       
                        }
                    if(payment->getString("validation_id")!="" && payment->getString("validation_id")!="0")
                        {
                        Php::out<<"Validation value: "<<payment->getString("validation_value")<<"<br>"<<endl;       
                        }
                    delete payment;
                    }
                }

            if(exit_date_time!="")
                {
                Php::out<<"<h3>Exit details</h3>"<<endl;
                Php::out<<"Exit date time: "<<exit_date_time<<"<br>"<<endl;                   
                Php::out<<"Exit device name: "<<exit->getString("device_name")<<"<br>"<<endl;          
                Php::out<<"Exit carpark name: "<<exit->getString("carpark_name")<<"<br>"<<endl;
                delete exit;
                }

            }
        else
            Php::out<<"Invalid ticket"<<endl;            
                                                   
        delete res;
		delete rStmt;
		delete rCon;
       
        }
    catch(const std::exception& e)
        {
        writeParkingReportException("visitorFrequencyRealTimeReport",e.what());
        }       
    }