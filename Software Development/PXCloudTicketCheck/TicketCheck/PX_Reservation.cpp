#include "PX_GeneralOperations.h"
#include "PX_Reservation.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>

#define CloudDB "pxcloud_server"
using namespace std;
extern GeneralOperations General;
extern string query;

void writeReservationLog(string function,string message)
    {
    General.writeLog("Services/PXCloudTicketCheck/ApplicationLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeReservationException(string function,string message)
    {
    General.writeLog("Services/PXCloudTicketCheck/ExceptionLog-PX-TicketCheck-"+General.currentDateTime("%Y-%m-%d"),function,message); 
	writeReservationLog(function,"Exception: "+message);       
    }

Php::Value Reservation::checkReservation(string ticketId,int entryGrace,int exitGrace,int carpark,int facility,int movement_type)
   {
	writeReservationLog("checkReservation","TicketID:"+ticketId);	    	   
	Php::Value response;    
	response["reservation_present"] = "false";
	response["access_allowed"] = "false";		   
   	try
        {        
            sql::Connection *conn;
            sql::Statement *stmt;
            sql::ResultSet *res;
            conn=General.mysqlConnect(CloudDB);
            if(conn!=NULL)
            {
                stmt=conn->createStatement();  
                string parking_to,parking_from;  
                int reservation_facility,reservation_carpark;
                            double fee;
                struct tm tm;	
                time_t now = time(NULL);	
                time_t start,end;

                //query="select * from parking_reservation where ticket_id='"+ticketId+"' and cancel_flag=0 and used_flag=0 and to_time>NOW() limit 1";
               /* if(movement_type==1)
                    query = "select * from parking_reservation where ticket_id='"+ticketId+"' and cancel_flag=0 and used_flag=0 and to_time > NOW() order by to_time limit 1";
                else if(movement_type==2)
                    query = "select * from parking_reservation where ticket_id='"+ticketId+"' and cancel_flag=0 and used_flag=0 and from_time < NOW() order by from_time desc limit 1";
                */
                query = "select * from parking_reservation where ticket_id='"+ticketId+"' and cancel_flag=0 and used_flag=0 limit 1";
                res=stmt->executeQuery(query);   
                if(res->next())
                {
                    int id = res->getInt("reservation_id");
                    writeReservationLog("checkReservation","Reservation present");
                    writeReservationLog("checkReservation","movement type:"+to_string(movement_type));		
                    response["reservation_present"] = "true";
                    response["plate_number"]=string(res->getString("plate_number"));	
                    parking_to=res->getString("to_time");
                    parking_from=res->getString("from_time");
                    response["parking_from"]=parking_from;
                    response["parking_to"]=parking_to;			
                    reservation_carpark=res->getInt("carpark_number");			
                    reservation_facility=res->getInt("facility_number");			
                    response["reservation_carpark"] = reservation_carpark;
                    response["reservation_facility"] = reservation_facility;				
                    response["reservation_used"]=res->getInt("used_flag");
                    fee=res->getDouble("reserved_parking_fee"); 	
                    response["reservation_fee"] = fee;             
                    ticketId=res->getString("ticket_id");
                    response["ticket_id"]=ticketId;
                    if(reservation_carpark!=carpark)
                    {
                        response["result"]= "invalid_reservation_carpark";
                        response["result_description"]="Invalid reservation carpark";
                    }
                    else if(reservation_facility!=facility)
                    {
                        response["result"]= "invalid_reservation_facility";
                        response["result_description"]="Invalid reservation facility";
                    }
                    else
                    {						
                        if(movement_type==1)	//Entry
                        {
                            if(res->getInt("used_flag")>0)
                            {					
                                response["result"]= "already_used_reservation";
                                response["result_description"]="Already used reservation";
                            }
                            if(res->getInt("cancel_flag")>0)
                            {					
                                response["result"]= "cancelled_reservation";
                                response["result_description"]="Cancelled reservation";
                            }
                            else
                            {																							
                                strptime(parking_from.c_str(),"%Y-%m-%d %H:%M:%S",&tm);	
                                tm.tm_min-=entryGrace;			
                                mktime(&tm);
                                start=mktime(&tm); 

                                strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);							
                                mktime(&tm);
                                end=mktime(&tm);  

                               // if(difftime(now,start)>=0 && difftime(now,end)>0)
                               if(difftime(now,start)>=0 && difftime(end,now)>0)
                                {
                                    response["result_description"]="Reservation access allowed";	
                                    response["result"]= "reservation_allow_access";	
                                    response["access_allowed"] = "true";	
                                }
                                else
                                {
                                    response["result_description"]="Not in reservation time slot";	
                                    response["result"]= "not_in_reservation_time_slot";	
                                }														
                            }
                        }//devicetype 1
                        else
                        {        
                            writeReservationLog("checkReservation","To:"+parking_to);
                            if(res->getInt("used_flag")==2)
                            {
                                response["result"]= "already_exited_reservation";
                                response["result_description"]="Already exited reservation";
                            }
                            else
                            {
                                writeReservationLog("checkReservation","Not exited");							
                                strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);														
                                tm.tm_min+=exitGrace;
                                mktime(&tm);	
                                end=mktime(&tm);
                                double seconds=difftime(end,now);								
                                if(seconds>=0)
                                {
                                    strptime(parking_from.c_str(),"%Y-%m-%d %H:%M:%S",&tm);																	
                                    start=mktime(&tm); 
                                    if(difftime(now,start)>=0)
                                    {	
                                        stmt->executeUpdate("Update parking_reservation set used_flag=1 where reservation_id="+to_string(id));
                                        response["result_description"]="Reservation access allowed";									
                                        response["result"]= "reservation_allow_access";
                                        response["access_allowed"] = "true";	
                                        response["exit_grace_time_remaining"]=to_string(int(seconds));
                                        writeReservationLog("checkReservation","Exit grace time remaining: "+to_string(seconds));
                                    }
                                    else
                                    {
                                        
                                        response["result_description"]="Not in reservation timeslot";	
                                        response["result"]= "not_in_reservation_time_slot";	
                                    }
                                }
                                else
                                {                            
                                    response["result"]= "reservation_exit_grace_period_exceeded";	
                                    response["result_description"]="Reservation exit grace period exceeded";
                                    seconds=seconds*-1;
                                    response["parking_duration"]=to_string(int(seconds));   
                                    char out[30];                             
                                    strftime(out,30,"%Y-%m-%d %H:%M:%S",&tm);
                                    string expiry_date(out);
                                    response["max_exit_grace_period"]=expiry_date;
                                }                                                                                                           
                            }

                        }
                    }//valid carpark,facility						                
                }
                else
                {
                    response["result"]= "no_reservation";	
                    response["result_description"]="No reservation";
                }

                string output=	response["result"];
                writeReservationLog("checkReservation",output+"\n");

                delete res;
                delete stmt;	
                delete conn;	
            }

        }
   catch(exception &e)
        {
        writeReservationException("checkReservation",e.what());    
        }
    return response;   
   } 