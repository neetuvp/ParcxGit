#include "PX_GeneralOperations.h"
#include "PX_Access.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include<sstream>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateFormat "%Y-%m-%d"

using namespace std;
extern GeneralOperations General;
extern string query;

void writeAccessLog(string function,string message)
    {
    General.writeLog("Services/PXTicketCheck/ApplicationLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeAccessException(string function,string message)
    {
    General.writeLog("Services/PXTicketCheck/ExceptionLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message); 
	writeAccessLog(function,"Exception: "+message);   
    }


bool presentInString(string str,string find)
    {
    if(find.length()==0)
        return true;    
    stringstream s_stream(str);
    string substr;
    while(s_stream.good()) 
        {       
        getline(s_stream, substr, ',');         
            if(substr==find)
                return true;
        }
    return false;
    }    

Php::Value Access::checkAccess(string ticketId,string parkingZone,int carpark,int facility,int deviceType,string plateNumber,string plateType,string plateArea,string plateCountry,Php::Value anprSettings)
    {
    writeAccessLog("checkAccess","TicketID:"+ticketId+"\tPlateNumber: "+plateNumber);	    
    Php::Value response;
	response["whitelist_present"]="false";	
	response["access_allowed"]="false";	    
    try
        {
        sql::Connection *serverCon;
		sql::Statement *stmt;
		sql::ResultSet *res;           
        serverCon=General.mysqlConnect(ServerDB);
        if(serverCon!=NULL)
            {
            if(plateNumber!="")
                query="select * from access_whitelist where plate_number='"+plateNumber+"'";		                
            else
			    query="select * from access_whitelist where  ticket_id='"+ticketId+"'";
                                					
            stmt = serverCon->createStatement();
            res=stmt->executeQuery(query);
		    if(res->next())
			    {
			    writeAccessLog("checkAccess","Present in access whitelist");
                response["plate_number"]=string(res->getString("plate_number"));	
                response["tag_serial"]=string(res->getString("tag_serial"));
                response["tag"]=string(res->getString("tag"));
                response["tag_tid"]=string(res->getString("tag_tid"));
                response["ticket_id"]=string(res->getString("ticket_id"));
                response["whitelist_present"]="true";
			
			    string zone=res->getString("access_zones");
                string expiry_date=res->getString("validity_expiry_date");
                string start_date=res->getString("validity_start_date");
                string currentDate=General.currentDateTime(dateFormat);
                string carparkNumber=res->getString("carpark_number");
                response["expiry_date"]=expiry_date;
                if(res->getInt("status")==0)
                    {
                    response["result_description"]="Access disabled";    
                    response["result"]="access_disabled";
                    }
                else if(!presentInString(parkingZone,zone))                 
                    {
                    response["result_description"]="Invalid parking zone";    
                    response["result"]="invalid_access_parking_zone";
                    }
                else if(!presentInString(carparkNumber,to_string(carpark)))                  
                    {
                    response["result_description"]="Invalid access carpark";    
                    response["result"]="invalid_access_carpark";
                    }
                else if(facility!=res->getInt("facility_number"))
                    {
                    response["result_description"]="Invalid access facility";    
                    response["result"]="invalid_access_facility";
                    }
                else if(expiry_date.compare(currentDate)<0)
                    {
                    response["result_description"]="Access expired";    
                    response["result"]="access_expired";
                    }
                else if(start_date.compare(currentDate)>0)
                    {
                    response["result_description"]="Access not started";    
                    response["result"]="access_not_started";
                    } 
                else if(res->getInt("antipassback_enabled")==1 && deviceType<3)
                    {                    
                    query="select * from parcx_reporting.open_transactions where  ticket_id='"+ticketId+"'";
                    res=stmt->executeQuery(query);
		            if(res->next())
                        {
                        if(deviceType==1)
                            {
                            response["result_description"]="Entry exists";    
                            response["result"]="antipassback_entry_exist";   
                            }
                        else if(res->getInt("movement_type")==2)
                            {
                            response["result_description"]="Already Exited";    
                            response["result"]="antipassback_already_exited";      
                            }
                        
                        }
                    else
                        {
                        if(deviceType==1)
                            {
                            response["result_description"]="Entry not exists";    
                            response["result"]="antipassback_entry_not_exist";   
                            }   
                        
                        }
                    
                    }                 
                else							
                    {
                    response["access_allowed"]="true";		
                    response["result"]= "allow_access";		
                    response["result_description"]="Access allowed-whitelist available";			
                    }
                                
                }
            else
                {			
                response["result"]="not_in_access_whitelist";
                response["result_description"]="Not in access whitelist";
                }
            string output=	response["result"];
            writeAccessLog("checkAccess",output+"\n");
            serverCon->close();       
            delete serverCon;   
            }
        }
    catch(const std::exception& e)
        {
            writeAccessException("checkAccess",e.what());
        }
    return response;        
    }
