#include "PX_GeneralOperations.h"
#include "PX_Access.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include<sstream>

#define CloudDB "pxcloud_server"
#define dateFormat "%Y-%m-%d"

using namespace std;
extern GeneralOperations General;
extern string query;

void writeAccessLog(string function,string message)
    {
    General.writeLog("Services/PXCloudTicketCheck/ApplicationLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeAccessException(string function,string message)
    {
    General.writeLog("Services/PXCloudTicketCheck/ExceptionLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message); 
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

Php::Value Access::checkAccess(string ticketId,string parkingZone,int carpark,int facility,int movementType)
    {
    string plateNumber;
    writeAccessLog("checkAccess","TicketID:"+ticketId);	    
    Php::Value response;
	response["whitelist_present"]="false";	
	response["access_allowed"]="false";	    
    try
        {
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res;                   
        conn=General.mysqlConnect(CloudDB);
        if(conn!=NULL)
        {
            query="select * from access_whitelist where  ticket_id='"+ticketId+"'";                         					
            stmt = conn->createStatement();
            res=stmt->executeQuery(query);
            if(res->next())
            {
                writeAccessLog("checkAccess","Present in access whitelist");
                plateNumber=res->getString("plate_number");
                ticketId=res->getString("ticket_id");
                response["plate_number"]=string(res->getString("plate_number"));	
                //response["tag_serial"]=string(res->getString("tag_serial"));
                response["tag"]=string(res->getString("tag"));
                //response["tag_tid"]=string(res->getString("tag_tid"));
                response["ticket_id"]=string(res->getString("ticket_id"));
                response["whitelist_present"]="true";
                int corporate_parker=res->getInt("corporate_parker");
                response["corporate_parker"]=corporate_parker;
                response["entry_type_contract_parking_space_exceeded"]=2;
			
                string zone=res->getString("access_zones");
                string expiry_date=res->getString("expiry_date");
                //string start_date=res->getString("validity_start_date");
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
                        response["customer_message_line1"]="Invalid Parking Zone";
                        }
                    else if(!presentInString(carparkNumber,to_string(carpark)))                  
                        {
                        response["result_description"]="Invalid access carpark";    
                        response["result"]="invalid_access_carpark";
                        response["customer_message_line1"]="Invalid Access Carpark";
                        }
                    else if(facility!=res->getInt("facility_numbers"))
                        {
                        response["result_description"]="Invalid access facility";    
                        response["result"]="invalid_access_facility";
                        response["customer_message_line1"]="";
                        
                        }
                    else if(expiry_date.compare(currentDate)<0)
                        {
                        response["result_description"]="Access expired";    
                        response["result"]="access_expired";
                        }
                    /*else if(start_date.compare(currentDate)>0)
                        {
                        response["result_description"]="Access not started";    
                        response["result"]="access_not_started";
                        }  */
                    /*else if(res->getInt("antipassback_enabled")==1 && deviceType<3)
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
                            else
                                {
                                response["access_allowed"]="true";		
                                response["result"]= "allow_access";		
                                response["result_description"]="Access allowed.whitelist available";
                                }                                                
                            }
                        else
                            {
                            if(deviceType==2)
                                {
                                response["result_description"]="Entry not exists";    
                                response["result"]="antipassback_entry_not_exist";   
                                }
                            else
                                {
                                response["access_allowed"]="true";		
                                response["result"]= "allow_access";		
                                response["result_description"]="Access allowed.whitelist available";
                                }                                                   
                            }

                        } */             
                    else							
                        {
                            if(movementType==1)
                            {
                                response["access_allowed"]="true";		
                                response["result"]= "allow_access";		
                                response["result_description"]="Access allowed.whitelist available";
                                
                            }
                            else
                            {
                                response["access_allowed"]="true";		
                                response["result"]= "allow_access";		
                                response["result_description"]="Access allowed at exit.whitelist available";
                            }
                        }
                
                    
                /*writeAccessLog("checkAccess","cooperate_parker : "+to_string(cooperate_parker));
                response["short_term_entry_after_contract_parking_space_exceeded"]=0;
                
                string access_allowed=response["access_allowed"];
                                        
                if(access_allowed=="true" && cooperate_parker>1)
                    {
                    query="SELECT setting_value from system_settings where setting_name='short_term_entry_after_contract_parking_space_exceeded'";
                    res=stmt->executeQuery(query);
                    res->next();
                    response["short_term_entry_after_contract_parking_space_exceeded"]=int(res->getInt("setting_value"));
                    writeAccessLog("checkAccess","short_term_entry_after_contract_parking_space_exceeded : "+string(res->getString("setting_value")));

                    if(res->getInt("setting_value")==1 && deviceType==1)
                        {        
                        query="select * from cooperate_users where  user_id="+to_string(cooperate_parker);
                        res=stmt->executeQuery(query);
                        if(res->next())
                            {
                            int parking_spaces=res->getInt("parking_spaces");
                            writeAccessLog("checkAccess","Cooperate parker under "+to_string(cooperate_parker)+". Alloted spaces : "+to_string(parking_spaces));
                            query="SELECT count(*) as count FROM parcx_server.access_whitelist a,parcx_reporting.open_transactions b where a.ticket_id=b.ticket_id  and a.plate_number=b.plate_number and b.entry_type=2 and a.corporate_parker="+to_string(cooperate_parker);
                            res=stmt->executeQuery(query);
                            res->next();
                            int occupied=res->getInt("count");
                            writeAccessLog("checkAccess","Currently occupied for the cooperate user :"+to_string(occupied));
                            if(occupied>=parking_spaces)
                                {                                    
                                writeAccessLog("checkAccess","short term access after cooperate parking space occupied is enabled");
                                response["access_allowed"]="false";		
                                response["result"]= "contract_parking_spaces_occupied";		
                                response["result_description"]="Contract parking spaces occupied";                                        
                                }
                            }                        
                        }   
                    } */                			                                                    
                }
            else
                {			
                response["result"]="not_in_access_whitelist";
                response["result_description"]="Not in access whitelist";
                }
            string output=	response["result"];
            writeAccessLog("checkAccess",output+"\n");
            //conn->close();
            delete res;
            delete stmt;
            delete conn;   
            }
        }
    catch(const std::exception& e)
        {
            writeAccessException("checkAccess",e.what());
        }
    return response;        
    }
