//g++ -std=c++11  -o PX_insertDataToServer insertDataToServer.cpp PX_GeneralOperations.cpp -lmysqlcppconn
#include "PX_GeneralOperations.h"
#include "mysql_connection.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<sys/stat.h>
#include<cstdlib>
#include <fstream>
#include <sstream>

#define DATABASE_REPORTING "parcx_reporting"
#define DATABASE_DASHBOARD "parcx_dashboard"
#define DATABASE_SERVER "parcx_server"
#define dateFormat "%Y-%m-%d"

using namespace std;
GeneralOperations general;
sql::Connection *con;
sql::ResultSet *res;
sql::Statement *stmt;
string query;
int delay;
int i=0;
int h;
int n;
string facility_number="100001";
string facility_name="Demo facility 100001";
string carpark_number="1";
string carpark_name="Demo facility 100001";    
string ticket_id,entry_date_time,current_date_time,parking_duration,category,max_exit_date_time;
int parking_fee,vat_amount,gross_amount;
ofstream fp; 						    		
string hexCarpark= "01";
string hexDevice= "0b";
string hexTicketType= "00";
struct tm s_tm;
string startDate="2018-01-01 00:00:00";    


void createDaemon() {
    pid_t pid;
    pid = fork();
    pid_t sid = 0;
    if (pid < 0) {
        cout << "Fork failed";
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        int proccess_id = (int) pid;
        cout  << " : Process id=" << pid << endl;        
        exit(EXIT_SUCCESS);
    }
    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}


void generateTicketNumber()
    {        																				
    //number of seconds from 2018 jan 01 00:00:00 to now+1415926535
    strptime(startDate.c_str(),"%Y-%m-%d %H:%M:%S",&s_tm);		
    time_t t=mktime(&s_tm);

    time_t now=time(NULL);
    long seconds=difftime(now,t);

    seconds=seconds+1415926535;			
			
    std::stringstream ss;				
    ss << std::hex << seconds;
    std::string ticket(ss.str());									
    
    ticket_id=facility_number+hexCarpark+hexDevice+hexTicketType+ticket;
    
    }

int main()
    {  
    createDaemon();
      


    fp.open("insert_to_db.log", ios::app);

    con=general.mysqlConnect("parcx_server");
    stmt=con->createStatement();          
    query = "select setting_value from system_settings where setting_name='facility_name'";
    res=stmt->executeQuery(query);
    if(res->next())        
        facility_name=res->getString("setting_value") ; 
    
    fp<<"facility_name: "<<facility_name<<endl;
    carpark_name=facility_name;

    query = "select setting_value from system_settings where setting_name='facility_number'";
    res=stmt->executeQuery(query);
    if(res->next())        
        facility_number=res->getString("setting_value") ;
    
    fp<<"facility_number: "<<facility_number<<endl;

    query = "select device_name,device_category,device_number from system_devices where device_category<=5 order by device_category";
    res=stmt->executeQuery(query);
    n=res->rowsCount();
    string device_number[n];
    string device_name[n];
    string device_type[n];
    
    while(res->next())        
        {
        device_number[i]=res->getString("device_number");
        device_name[i]=res->getString("device_name");
        device_type[i]=res->getString("device_category");

        fp<<i<<"   "<<device_number[i]<<"   "<<device_type[i]<<"   "<<device_name[i]<<endl;        
        i++;
        }
    fp<<"-------------------------"<<endl;
    fp.close();  
    delete res;
    delete stmt;
    delete con;
    
    i=0;
    while(true)
        {        
        fp.open("insert_to_db.log", ios::app);
        current_date_time=general.currentDateTime("%Y-%m-%d %H:%M:%S");
        fp<<"current_date_time :"<<current_date_time<<endl;

        h=stoi(general.currentDateTime("%H"));       
        if(h>0 && h<7)
            delay=rand()%1000;
        else if(h>=7 && h<10)
            delay=rand()%100;
        else if(h>=10 && h<12)
            delay=rand()%250;
        else if(h>=12 && h<15)
            delay=rand()%350;
        else if(h>=15 && h<18)
            delay=rand()%100;
        else if(h>=18 && h<20)
            delay=rand()%500;
        else
            delay=rand()%1000;
        
        fp<<"Delay: "<<delay<<endl;

        i=delay%n;
        fp<<"i :"<<i<<" "<<"device: "<<device_name[i]<<"  device_type: "<<device_type[i]<<endl;        
        try
            {        
            con=general.mysqlConnect(DATABASE_REPORTING);
            stmt=con->createStatement();               
            if (stoi(device_type[i]) == 1) 
                {                
                ticket_id=general.currentDateTime("%Y%m%d%H%M%S");
                generateTicketNumber();
                fp<<"ENTRY ticket_id :"<<ticket_id<<endl;
                query = "INSERT into parking_movements(device_name,device_number,movement_type,ticket_id,carpark_number,facility_number,date_time,plate_number,chip_utid,operation_mode,plate_captured_id,carpark_name) VALUES('" + device_name[i] + "'," + device_number[i] + ",'" + device_type[i] + "','" + ticket_id + "'," + carpark_number + "," + facility_number + ",CURRENT_TIMESTAMP,'','','',0,'" + carpark_name + "')";
                 stmt->executeUpdate(query);
                 query = "INSERT into open_transactions(device_name,device_number,ticket_id,carpark_number,facility_number,entry_date_time,plate_number,chip_utid,operation_mode,entry_grace_period,entry_type,plate_captured_id,carpark_name,movement_type) VALUES('" + device_name[i] + "'," + device_number[i] + ",'" + ticket_id + "'," + carpark_number + "," + facility_number + ",CURRENT_TIMESTAMP,'','','',DATE_ADD(CURRENT_TIMESTAMP,INTERVAL 15 MINUTE),1,0,'" + carpark_name + "',1)";
                 stmt->executeUpdate(query);
                }
            else
                {            
                query = "select ticket_id,entry_date_time,max_exit_date_time from open_transactions ORDER BY RAND() LIMIT 1";
                res=stmt->executeQuery(query);
                if(res->next())
                    {
                    ticket_id=res->getString("ticket_id");                
                    entry_date_time=res->getString("entry_date_time");
                    max_exit_date_time=res->getString("max_exit_date_time");
                    
                    
                    fp<<"EXIT ticket_id :"<<ticket_id<<endl;
                    fp<<"entry_date_time :"<<entry_date_time<<endl;
                    fp<<"max_exit_date_time :"<<entry_date_time<<endl;
                    
                    

                    int *duration = general.differenceDateTime(current_date_time, entry_date_time, "%Y-%m-%d %H:%M:%S");
                    if(duration[1]>0)                    
                        {
                        parking_duration= to_string(duration[1])+" D "+to_string(duration[2])+" H "+to_string(duration[3])+" M";
                        h=duration[1]*24+duration[2]+1;
                        }
                    else
                        {
                        parking_duration= to_string(duration[2])+" H "+to_string(duration[3])+" M";
                        h=duration[2]+1;
                        }
                    fp<<"parking_duration :"<<duration[0]<<endl;
                    fp<<"parking_duration :"<<parking_duration<<endl;
                    parking_fee=0;
                    vat_amount=0;
                    gross_amount=0;
                    category="Entry Grace";
                    if(duration[0]>60*15)
                        {                    
                        gross_amount=h*5;
                        parking_fee = gross_amount / (1 + (5 / 100));
                        vat_amount = gross_amount - parking_fee;                        
                        category="StandardParkingFee";
                        }
                    fp<<"gross_amount :"<<gross_amount<<endl;
                    fp<<"parking_fee :"<<parking_fee<<endl;
                    fp<<"vat_amount :"<<vat_amount<<endl;
                    fp<<"category :"<<category<<endl;


                    if (stoi(device_type[i]) > 2) 
                        { 
                        if(max_exit_date_time=="")                                                       
                            {
                            query = "INSERT into revenue_payments(device_number,device_name,carpark_number,carpark_name,facility_number,operator_id,operator_name,shift_id,parking_rate_label,parking_rate_name,entry_grace_period,exit_grace_period,vat_type,vat_percentage,ticket_id,entry_date_time,payment_date_time,max_exit_date_time,parking_duration,payment_category,parking_fee,vat_amount,lost_fee,admin_fixed_charges,ticket_replacement_fee,discount_amount,gross_amount,amount_received,discount_id,discount_category,discount_name,coupon_id,coupon_category,coupon_source,payment_type,validation_value,validation_id,receipt_number,bank_notes,balance_returned,credit_note,authentication_code,entry_plate_number,exit_plate_number,wallet_points)VALUES('" + device_number[i] + "','" + device_name[i] + "'," + carpark_number + ",'" + carpark_name + "','" + facility_number + "',30,'parcx','afme12345678','General Rate','parking_rates1','0 H 15 M','0 H 15 M','inclusive','5','" + ticket_id + "','" + entry_date_time + "',CURRENT_TIMESTAMP,DATE_ADD(CURRENT_TIMESTAMP,INTERVAL 15 MINUTE),'" + parking_duration + "','" + category + "','" + to_string(parking_fee) + "','" + to_string(vat_amount) + "','0','0','0','0','" + to_string(gross_amount) + "','" + to_string(gross_amount) + "','0','','','','','','Cash','','','100','','0','0','','','',0)";                        
                            stmt->executeUpdate(query);  

                            query="update open_transactions set total_amount_paid="+to_string(gross_amount)+",parking_rate='parking_rates1',last_payment_date_time=CURRENT_TIMESTAMP,max_exit_date_time=DATE_ADD(CURRENT_TIMESTAMP,INTERVAL 5 MINUTE) where ticket_id='"+ticket_id+"'";
                            stmt->executeUpdate(query);  
                            fp<<"create payment"<<endl;                                                  
                            }
                        else
                            {
                            fp<<"Already paid"<<endl;
                            delay=10;
                            }
                        
                        }
                    else
                        {
                        if(max_exit_date_time!="" || gross_amount==0)     
                            {
                            query = "INSERT into parking_movements(device_name,device_number,movement_type,ticket_id,carpark_number,facility_number,date_time,plate_number,chip_utid,operation_mode,plate_captured_id,carpark_name) VALUES('" + device_name[i] + "'," + device_number[i] + ",'" + device_type[i] + "','" + ticket_id + "'," + carpark_number + "," + facility_number + ",CURRENT_TIMESTAMP,'','','',0,'" + carpark_name + "')";
                            stmt->executeUpdate(query);

                            query = "Delete from open_transactions where ticket_id = '" + ticket_id + "' ";
                            stmt->executeUpdate(query);
                            fp<<"create exit"<<endl;                      
                            }
                        else
                            {
                            fp<<"Not paid"<<endl;
                            delay=10;
                            }
                        
                        }
                    delete res;
                    }
                else 
                    delay=10;
                }
            if(delay>500)
                {
                query = "INSERT into watchdog_device_alarms(carpark_number,carpark_name,facility_number,device_name,device_number,alarm_severity,alarm_code,alarm_description,alarm_date_time) VALUES(" + carpark_number + ",'" + carpark_name + "'," + facility_number + ",'" + device_name[i] + "'," + device_number[i] + ",6,1011,'Server Offline',CURRENT_TIMESTAMP)";
                stmt->executeUpdate(query);;
                }
                
            delete stmt;
            delete con;
            
            }        
        catch (sql::SQLException &e) 
            {
            fp<<e.what()<<endl;
            fp<<query<<endl;
            delete con;
            }
        fp<<"-------------------------"<<endl;
        fp.close();       
        sleep(delay);        
        }   
    }

