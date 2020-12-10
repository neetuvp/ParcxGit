//make -B
//sudo make install
//service apache2 restart

#include <phpcpp.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <ctime>
#include <dlfcn.h>
#include "jsoncpp/json/json.h"

#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "pxheader.h"


using namespace std;
sql::Connection *server_con,*reporting_con;
sql::Driver *driver;
sql::Statement *reporting_stmt,*server_stmt;
sql::ResultSet *reporting_res,*server_res;
sql::PreparedStatement *prep;
sql::ResultSetMetaData *rm;

#define DBServer "DBServer"
#define DBUsername "parcxservice"
#define DBPassword "1fromParcx!19514"
#define REPORTING_DATABASE "parcx_reporting"
#define SERVER_DATABASE "parcx_server"

pxheader p;
string plate_numnber,ticket_number,query,app_key,transaction_id,device_ip,device_name,app_user_id,json_response,ticket_max_exit_date_time,ticket_payment_date_time,grace_time_remaining,grace_time_exeeded,blacklist_status;
int operation_mode,anpr_enable,camera_id,device_number,device_type,task,facilityNo,entryGarce,exitGrace,carparkNo;
Json::Reader reader;
Json::FastWriter fastWriter;
Json::Value json,response,res;
/*
 * opertaion_mode values
 * 1-QRCode 
 * 2-Chipcoin (blacklist and validation check)
 * 3-Ticketless
 */


string log_file_path = "/var/log/parcx/Services/ApplicationLogs/reservation_check_phpcpp_"+ p.currentDate()+ "_log.log";
string error_file_path = "/var/log/parcx/Services/ExceptionLogs/reservation_check_phpcpp_"+ p.currentDate()+ "_exception.log";



extern "C" Json::Value ReservationCheck_GetReservationDetails(string ,int ,int ,int ,int ,int ,int,string);
extern "C" int ReservationCheck_CalculateParkingFee(string,int,int,int);


int ReservationCheck_CalculateParkingFee(string entry_date,int parkingduration,int device_number,int reservation)
	{
	int fee=0;	
	try
		{
		p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Entry date: "+entry_date+"\tParking duration: "+to_string(parkingduration)+"\tDevice:"+to_string(device_number));	
		int rate_type=0;
		string sql;
		string parking_rate="",parking_rate_label="";
		server_con=p.mysqlConnect(SERVER_DATABASE);
		server_stmt = server_con->createStatement();
		if(reservation==1)
			{
			sql="select reservation_rate_plan,rate_type from system_devices where device_number="+to_string(device_number);	
			server_res=server_stmt->executeQuery(sql);
			if(server_res->next())
				{
				rate_type=server_res->getInt("rate_type");
				parking_rate=server_res->getString("reservation_rate_plan");																	
				p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Reservation parking: Type:"+to_string(rate_type)+"\t"+parking_rate);
				}	
			}
		else
			{
			sql="select rate_type,parking_rate,parking_rate_label from revenue_dynamic_parking_rates where datetime =date('"+entry_date+ "') and status=1";        
			server_res=server_stmt->executeQuery(sql);
			if(server_res->first())
				{ 
				rate_type=server_res->getInt("rate_type");
				parking_rate=server_res->getString("parking_rate");
				parking_rate_label=server_res->getString("parking_rate_label");
				p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Dynamic parking rate For day: Type:"+to_string(rate_type)+"\t"+parking_rate+"\t"+parking_rate_label);			
				}
			else
				{
				sql = "select rate_type,parking_rate,parking_rate_label from revenue_dynamic_parking_rates where day=dayname('"+entry_date+"') and status=1";            
				res=server_stmt->executeQuery(sql);
				if(server_res->first())
					{                
					rate_type=server_res->getInt("rate_type");
					parking_rate=server_res->getString("parking_rate");
					parking_rate_label=server_res->getString("parking_rate_label");
					p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Dynamic parking rate For date: Type:"+to_string(rate_type)+"\t"+parking_rate+"\t"+parking_rate_label);			
					}
				else
					{
					sql="select rate_plan,rate_type from system_devices where device_number="+to_string(device_number);	
					server_res=server_stmt->executeQuery(sql);
					if(server_res->next())
						{
						rate_type=server_res->getInt("rate_type");
						parking_rate=server_res->getString("rate_plan");
						
						/*sql="select "+parking_rate+" from revenue_parking_rate_label";	
						res=stmt->executeQuery(sql);
						if(res->next())
							parking_rate_label=res->getString("parking_rate");
						writeLog("calculateParkingFee","Normal parking: Type:"+to_string(rate_type)+"\t"+parking_rate+"\t"+parking_rate_label);	*/
						
						p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Normal parking: Type:"+to_string(rate_type)+"\t"+parking_rate);
						}
					}
				}
			}
			
		//Calculate duration
		int hours=0,minutes=0,seconds=0;
		hours=parkingduration/3600;
		minutes=(parkingduration%3600)/60;
		seconds=(parkingduration%3600)%60;
		p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee",to_string(hours)+"H "+to_string(minutes)+"M "+to_string(seconds)+"S ");
		response["parking_duration"]=to_string(hours)+"H "+to_string(minutes)+"M "+to_string(seconds)+"S";
		//Calculate parking Fee
		
		if(rate_type==1)
			{
            if(hours>0)
				{
				sql = "select " + parking_rate + " from  revenue_parking_rates "+ "where time_unit_" + parking_rate + "='hours'  "+ "and time_duration_" + parking_rate + ">" + to_string(hours) + " "+ "ORDER BY  time_duration_" + parking_rate + " limit 1";				
				server_res=server_stmt->executeQuery(sql);

				if(server_res->next())
					{
					fee= server_res->getInt(parking_rate);	
					}
				else
					{
                    sql = "select time_duration_" + parking_rate + "," + parking_rate + " from  revenue_parking_rates "+ "where time_unit_" + parking_rate + "='hours'  "+ "ORDER BY  time_duration_" + parking_rate + " DESC limit 1";                    
                    server_res=server_stmt->executeQuery(sql);
                    if(server_res->next())
						{
                        int maxhours = server_res->getInt(1);
                        fee= server_res->getInt(parking_rate);
                     	p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Max hours:"+to_string(maxhours)+" Fee:"+to_string(fee));;
                        sql = "select " + parking_rate + " from revenue_fixed_parking_rates where fixed_fees='recurring_fee'";
						p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Query to get recurring fee: "+sql);;
                        server_res=server_stmt->executeQuery(sql);

                        if(server_res->first())
							{
                            int recurringFee = server_res->getInt(1);
							p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Recurring fee:"+to_string(recurringFee));                            
                            fee = fee + (hours + 1 - maxhours) * recurringFee;
							}
						}
					}
							
				}
            else 
				{//minutes only
                sql = "select " + parking_rate + " from  revenue_parking_rates "+ "where time_unit_" + parking_rate + "='mins'  "+ "and time_duration_" + parking_rate + ">" + to_string(minutes) + " "+ "ORDER BY  time_duration_" + parking_rate + " limit 1";
                server_res = server_stmt->executeQuery(sql);
                if(server_res->next())
					{
					fee= server_res->getInt(parking_rate);	
					}
				else
					{								
                    sql = "select " + parking_rate + " from  revenue_parking_rates "+ "where time_unit_" + parking_rate + "='hours'  " + "ORDER BY  time_duration_" + parking_rate + " limit 1";
                    server_res = server_stmt->executeQuery(sql);
                    server_res->first();
                    fee = server_res->getInt(1);
					} 				
                }
			}
        else if (rate_type == 2) 
			{
            sql = "select " + parking_rate + " from  revenue_fixed_parking_rates where fixed_fees='fixed_parking_rate'";
            server_res = server_stmt->executeQuery(sql);
            server_res->first();
            fee = server_res->getInt(1);
            }
		p.writeLog(log_file_path,"ReservationCheck_CalculateParkingFee","Fee: "+to_string(fee));	

		server_con->close();
		server_stmt->close()       ;
		server_res->close();

		delete server_res;
		delete server_stmt;
        delete server_con;
		}
	catch(exception &e)
		{
		p.writeException(error_file_path,"ReservationCheck_CalculateParkingFee",e.what()); 	
		}
	return fee;
	}

Json::Value ReservationCheck_CheckRevenuePaymentDetails(string ticket_number)
	{
	//Php::Value response;
    string ticket_max_exit_date_time,ticket_payment_date_time;  	
    try
		{                       
		p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Ticket: "+ticket_number);
        string sql="SELECT max(payment_date_time) as payment_date_time,max(max_exit_date_time) as max_exit_date_time,sum(gross_amount) as gross_amount from revenue_payments where ticket_id ='"+ticket_number+"' group by ticket_id";        
        //writeLog("check_revenue_payment_details","SQL: "+sql);
        reporting_stmt = reporting_con->createStatement();
        reporting_res=reporting_stmt->executeQuery(sql);
		
        if(reporting_res->next())
			{
            p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Already paid");   
            p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","paid amount: "+to_string(reporting_res->getInt("gross_amount"))); 
			response["paid_status"]="true";
			response["payment_date_time"]=string(reporting_res->getString("payment_date_time"));
			response["amount_paid"]=reporting_res->getInt("gross_amount");

            ticket_payment_date_time=reporting_res->getString("payment_date_time");
            ticket_max_exit_date_time=reporting_res->getString("max_exit_date_time");

            p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Payment DateTime : "+ticket_payment_date_time);
            p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Max exit DateTime : "+ticket_max_exit_date_time);
			p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Amount paid : "+to_string(response["amount_paid"].asInt()));

            struct tm tm;
            strptime(ticket_max_exit_date_time.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
            time_t t = mktime(&tm);
            time_t now = time(NULL);
            int seconds = difftime(t,now);

            if(seconds>0)
				{
				p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Exit grace time remianing: "+to_string(seconds)+" seconds");				
				response["exit_grace_time_remaining"]=to_string(seconds);
				response["result"]="Allow Access";
				}
              else
				{
				response["exit_grace_time_remaining"]="0";				
				strptime(ticket_payment_date_time.c_str(),"%Y-%m-%d %H:%M:%S",&tm);
				t = mktime(&tm);				
				seconds = difftime(now,t);
				p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Parking duration after payment: "+to_string(seconds)+" seconds");				
				//response["parking_duration"]=to_string(seconds);
				response["result"]="Reservation exit Grace period exceeded";
				p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Exit grace time exceeded");
				}
                            
			p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Paid");
			}
        else
			{				
            p.writeLog(log_file_path,"ReservationCheck_CheckRevenuePaymentDetails","Not paid");            
			response["paid_status"]="false";
			response["result"]="Not paid";
			}             
		}
    catch(const std::exception& e)
		{        
        p.writeException(error_file_path,"ReservationCheck_CheckRevenuePaymentDetails",e.what());        
		}	
	return response;	
	}

Json::Value ReservationCheck_GetReservationDetails(string ticketId,int entryGrace,int exitGrace,int carparkNo,int facilityNo,int device_type,int deviceNo,string plate_number)
	{
    //Php::Value response;
    string parking_to,parking_from;    
	struct tm tm;	
	time_t now = time(NULL);
	int reservation_carpark,reservation_facility,seconds,amount_paid,amount_to_pay;
	time_t start,end;
	response["reservation_status"]="false";	
    try
		{       
        reporting_con= p.mysqlConnect(REPORTING_DATABASE);
        string ticket_number=ticketId;
		int entry_grace=entryGrace;
		int exit_grace=exitGrace;
		int carpark=carparkNo;
		int facility=facilityNo;
		int type=device_type;
		int device_number = deviceNo;
		
		p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","ticket_number: "+ticket_number+"\tentry_grace: "+to_string(entry_grace)+"\texit_grace: "+to_string(exit_grace)+"\tcarpark: "+to_string(carpark)+"\tfacility: "+to_string(facility)+"\ttype: "+to_string(type)+"\tDevice Number: "+to_string(device_number)+"\tPlate Number: "+plate_number);

		if(!ticket_number.empty())
        query="SELECT * from parking_reservation where ticket_id = '"+ticket_number+"'";

		else
		{
		query="SELECT * from parking_reservation where plate_number = '"+plate_number+"'";    
		}

		        //writeLog("getReservationDetails",sql);
		p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails",query);
        reporting_stmt = reporting_con->createStatement();
        reporting_res=reporting_stmt->executeQuery(query);

        if(reporting_res->next())
			{			            
			parking_to=reporting_res->getString("to_time");
			parking_from=reporting_res->getString("from_time");
			response["parking_from"]=parking_from;
			response["parking_to"]=parking_to;			
			reservation_carpark=reporting_res->getInt("carpark_number");			
			reservation_facility=reporting_res->getInt("facility_number");			
            response["carpark"] = reservation_carpark;
			response["facility"] = reservation_facility;
		
			response["used"]=reporting_res->getInt("used_flag");
			response["ticket_id"]=(string)reporting_res->getString("ticket_id");
			ticket_number=reporting_res->getString("ticket_id");
			p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Parking from: "+parking_from+"\tParking to: "+parking_to);
			p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Carpark: "+to_string(reservation_carpark)+"\tFacility:"+to_string(reservation_facility)+"\tUsed: "+reporting_res->getString("used_flag"));
			if(type==1)	//Entry
				{
				if(reporting_res->getInt("used_flag")>0)
					{					
					response["result"]= "Already Used Reservation";
					}
				else
					{					
					if(reservation_carpark!=carpark)
						response["result"]= "Invalid carpark";
					else if(reservation_facility!=facility)
						response["result"]= "Invalid facility";
					else
						{						
						strptime(parking_from.c_str(),"%Y-%m-%d %H:%M:%S",&tm);	
						tm.tm_min-=entry_grace;			
						mktime(&tm);
						start=mktime(&tm); 
			
						strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);							
						mktime(&tm);
						end=mktime(&tm);  

						if(difftime(now,start)>=0 && difftime(end,now)>0){
							response["result"]= "Allow Access";	
							response["reservation_status"]="true";
						}
						else
							response["result"]= "Not in reservation time slot";	
						}
						
					}
				}
			
			else//Exit,APm,cashier
				{


	p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails"," "+json["anpr_enable"].asString()+" "+json["operation_mode"].asString());

					if(json["anpr_enable"].asInt()==1 || json["operation_mode"].asInt()==3){

						p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","in if ");

						void* handle = dlopen("/opt/lampp/lib/php/extensions/no-debug-non-zts-20160303/PaymentTicketCheck.so", RTLD_LAZY);
								if (!handle) {
									cout<<dlerror()<<endl;
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Unable to load SO file ");

								}
								else{
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Loadede SO file succsessfully");
									Json::Value (*getPlatesCapturedDetails)(int);
									*(void **)(&getPlatesCapturedDetails) = dlsym(handle, "PaymentTicketCheck_GetPlateCapturedDetails");
									Json::Value platescaptured_reponse =getPlatesCapturedDetails(json["device_number"].asInt());
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","closing the sloaded SO file");
									dlclose(handle);
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Response: "+fastWriter.write(platescaptured_reponse));

									//response=platescaptured_reponse;	
									if(!json.isMember("isPlateNoSearch") || json["isPlateNoSearch"].asString().compare("false")==0){
										response["entry_plate_type"]=platescaptured_reponse["entry_plate_type"];
									response["entry_capture_datetime"]=platescaptured_reponse["entry_capture_datetime"];
									response["entry_capture_datetime"]=platescaptured_reponse["entry_capture_datetime"];
									response["entry_plate_country"]=platescaptured_reponse["entry_plate_country"];
									response["entry_plate_number"]=platescaptured_reponse["entry_plate_number"];
									response["entry_camera_id"]=platescaptured_reponse["entry_camera_id"];
									response["entry_plate_image_name"]=platescaptured_reponse["entry_plate_image_name"];
									}
									
									response["exit_plate_type"]=platescaptured_reponse["exit_plate_type"];
									response["exit_capture_datetime"]=platescaptured_reponse["exit_capture_datetime"];
									response["exit_capture_datetime"]=platescaptured_reponse["exit_capture_datetime"];
									response["exit_plate_country"]=platescaptured_reponse["exit_plate_country"];
									response["exit_plate_number"]=platescaptured_reponse["exit_plate_number"];
									response["exit_camera_id"]=platescaptured_reponse["exit_camera_id"];
									response["exit_plate_image_name"]=platescaptured_reponse["exit_plate_image_name"];
								
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Response: "+response["exit_plate_image_name"].asString());		

								}
					}


					response["error"]=false;
				if(reporting_res->getInt("used_flag")==2){
					
						response["plate_number"]=(string)reporting_res->getString("plate_number");
					response["result"]= "Already Exited";
				}
				else
					{
					p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Not exited");
					if(reservation_carpark!=carpark)
						response["result"]= "Invalid carpark";
					else if(reservation_facility!=facility)
						response["result"]= "Invalid facility";
					else
						{
						response["plate_number"]=(string)reporting_res->getString("plate_number");
						strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);							
						mktime(&tm);
						tm.tm_min+=exit_grace;	
						end=mktime(&tm);
						seconds=difftime(end,now);					
						if(seconds>=0)
							{
							strptime(parking_from.c_str(),"%Y-%m-%d %H:%M:%S",&tm);	
							//tm.tm_min-=entry_grace;			
							mktime(&tm);
							start=mktime(&tm); 
							if(difftime(now,start)>=0)
								{
								response["result"]= "Allow Access";
								response["reservation_status"]="true";
								response["exit_grace_time_remaining"]=to_string(seconds);
								p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Exit grace time remaining: "+to_string(response["exit_grace_time_remaining"].asInt()));
								}
							else
								{
								response["result"]= "Not in reservation time slot";	
								}
							}
						else
							{
							
							//check payment exist for overstay
							query="SELECT payment_date_time,max_exit_date_time from revenue_payments where ticket_id = '"+ticket_number+"' ORDER by id DESC LIMIT 1";
							p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails",query);
							reporting_res=reporting_stmt->executeQuery(query);	
							if(reporting_res->next())
								{
								p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Payment for overstay exist,Payment time: "+reporting_res->getString("payment_date_time"));
								parking_to=reporting_res->getString("max_exit_date_time");
								strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);							
								//mktime(&tm);
								//tm.tm_min+=exit_grace;	
								end=mktime(&tm);  
								seconds=difftime(end,now);	
								p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Difference in seconds after reservation uptil now: "+to_string(seconds))				;
								if(seconds>=0)
									{
									response["result"]= "Allow Access";
									response["exit_grace_time_remaining"]=to_string(seconds);
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Exit grace time remaining: "+response["exit_grace_time_remaining"].asString());
									}
								else
									{
									parking_to=reporting_res->getString("payment_date_time");
									strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);																
									end=mktime(&tm);  	
									seconds=difftime(now,end);		
									response["result"]= "Reservation exit grace period exceeded";
									response["parking_to"]=parking_to;
									//calculation overstayed fee
									amount_to_pay=ReservationCheck_CalculateParkingFee(parking_from,seconds,device_number,1);
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Total Amount to pay recaluclating from: "+to_string(amount_to_pay));
									Json::Value payment_details=ReservationCheck_CheckRevenuePaymentDetails(ticket_number);
									amount_paid=payment_details["amount_paid"].asInt();	
								
									amount_to_pay= amount_to_pay-amount_paid;
									response["amount_to_pay"]= amount_to_pay;

									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Amount to be paid finally: "+to_string(amount_to_pay));
											
									}
								}
							else
								{
									strptime(parking_to.c_str(),"%Y-%m-%d %H:%M:%S",&tm);							
									mktime(&tm);
									end=mktime(&tm);
									seconds=difftime(now,end);	
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Payment for overstay not exist calculating 1st payment");
									response["result"]= "Reservation exit grace period exceeded";
									amount_to_pay=ReservationCheck_CalculateParkingFee(parking_from,seconds,device_number,1);
									p.writeLog(log_file_path,"ReservationCheck_GetReservationDetails","Total Amount to pay : "+to_string(amount_to_pay));
									response["amount_to_pay"]= amount_to_pay;								
								}
							}
						}
					}
				}
			}
		else{
			response["result"]= "No Reservation";	
			response["error"]=false;
		}

		//string output=response["result"];
		//p.writeLog(log_file_path,"getReservationDetails",response+"\n");
		reporting_res->close();
		reporting_stmt->close();
		reporting_con->close();

		delete reporting_res;
		delete reporting_stmt;
		delete reporting_con;					
		}
    catch(const std::exception& e)
		{	       
        p.writeException(error_file_path,"ReservationCheck_GetReservationDetails",e.what());        
		}
		return response;
	}

Php::Value ReservationCheck(Php::Parameters &params)
{
	
	try{
	
	
	string json_request=params[0];
	p.writeLog(log_file_path,"Payment_TicketCheck","in the function "+json_request);
	bool parseSuccess = reader.parse(json_request, json, false);
	res["error"]=true;
	 if (parseSuccess)
  {
    //const Json::Value resultValue = json["device_number"];
    device_number =json["device_number"].asInt();
    device_name= json["device_name"].asString();
    device_type = json["device_type"].asInt();
	device_ip=json["device_ip"].asString();
    transaction_id=json["transaction_id"].asString();
    app_key=json["app_key"].asString();
    app_user_id=json["app_suer_id"].asString();
    task= json["task"].asInt();
	
    p.writeLog(log_file_path,"Reservation_Check","device numebr "+to_string(device_number));
	p.writeLog(log_file_path,"Reservation_Check","device type "+to_string(device_type));

	switch(task){
		
		//get the reservation details
		case 1:
		
			p.writeLog(log_file_path,"Reservation_Check","Getting the reservation details");
			
			entryGarce=json["reservation_entry_grace_period"].asInt();
			exitGrace=json["reservation_exit_grace_period"].asInt();
			facilityNo=json["facility_number"].asInt();
			carparkNo=json["carpark_number"].asInt();

			if(json.isMember("plate_number")){
				plate_numnber=json["plate_number"].asString();
			}
			else{
				plate_numnber="";
			}

			
			if(json.isMember("ticket_no")){
				ticket_number=json["ticket_no"].asString();
			}
			else{
				ticket_number="";
			}
			
			res=ReservationCheck_GetReservationDetails(ticket_number,entryGarce,exitGrace,carparkNo,facilityNo,device_type,device_number,plate_numnber);
			
		break;
		
		
	}
	
	res["transaction_id"]=transaction_id;
    res["app_key"]=app_key;
	json_response=fastWriter.write(res) ;
	p.writeLog(log_file_path,"Reservation_Check","Final Response "+json_response);
	
    
  }
  
}
catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        p.writeException(error_file_path,"Reservation_Check",e.what());
       
    }
	
	return json_response;



}





extern "C" 
	{    
	
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("ReservationCheck", "1.0");
        extension.add<ReservationCheck>("ReservationCheck");        
        return extension;
		}
	}

