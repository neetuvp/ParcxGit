#include "PX_GeneralOperations.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include<phpcpp.h>
#include <netdb.h>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateFormat "%Y-%m-%d"

using namespace std;
GeneralOperations General;
sql::Connection *rCon, *con;
sql::Statement *rStmt, *stmt;
;
sql::ResultSet *res, *res2;
string query;
string image, croppedImage, sceneImage, datefolder;
size_t n;
string plate_review_confidence_rate = "0";
int update;

void writeLog(string function, string message) {
    General.writeLog("WebApplication/ApplicationLogs/PX-Dashboard-" + General.currentDateTime("%Y-%m-%d"), function, message);
}

void writeException(string function, string message) {
    General.writeLog("WebApplication/ExceptionLogs/PX-Dashboard-" + General.currentDateTime("%Y-%m-%d"), function, message);
    Php::out << message << endl;
    writeLog(function, "Exception: " + message);
}

string toString(Php::Value param) {
    string value = param;
    return value;
}


void getPlateCorrectionRequiredTable() {
    try {
        if (plate_review_confidence_rate == "0") {
            con = General.mysqlConnect(ServerDB);
            stmt = con->createStatement();
            query = "select setting_value from system_settings where setting_name='plate_review_confidence_rate'";
            res = stmt->executeQuery(query);
            if (res->next())
                plate_review_confidence_rate = res->getString("setting_value");
            writeLog("plate_review_confidence_rate", plate_review_confidence_rate);
            delete res;
            delete stmt;
            delete con;
        }
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select a.id,a.plate_number,plate_image_name,capture_date_time,camera_device_number,confidence_rate,entry_date_time from plates_captured a INNER JOIN open_transactions b ON a.id=b.plate_captured_id where initial_plate_number is null and (b.plate_number like '%no plate%'  or confidence_rate<=" + plate_review_confidence_rate + " or b.plate_number like '%noplate%')";
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<thead>" << endl;
            Php::out << "<tr>" << endl;
            Php::out << "<th>Plate Number</th>" << endl;
            Php::out << "<th>Entry Date Time</th>" << endl;
            Php::out << "<th>Confidence Rate</th>" << endl;
            //Php::out<<"<th>Plate Image</th>"<<endl;		
            Php::out << "<th>Action</th>" << endl;
            Php::out << "</tr>" << endl;
            Php::out << "</thead>" << endl;
            Php::out << "<tbody>" << endl;
            while (res->next()) {
                image = res->getString("plate_image_name");
                while ((n = image.find(" ")) != std::string::npos)
                    image.replace(n, 1, "%20");
                while ((n = image.find("#")) != std::string::npos)
                    image.replace(n, 1, "%23");

                datefolder = res->getString("capture_date_time");
                datefolder = datefolder.substr(0, 10);

                croppedImage = res->getString("camera_device_number") + "/" + datefolder + "/Crop_" + image;
                sceneImage = res->getString("camera_device_number") + "/" + datefolder + "/Scene_" + image;

                Php::out << "<tr'>" << endl;
                Php::out << "<td >" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td >" + res->getString("entry_date_time") + "</td>" << endl;
                Php::out << "<td ><span class='badge bg-danger'>" + res->getString("confidence_rate") + "</span></td>" << endl;
                //Php::out<<"<td ><img src='http://localhost/ANPR/Images/Cropped/"+croppedImage+"' width='100' height='50'></td>"<<endl;                
                Php::out << "<td><button type='button' class='btn  btn-info col' data-toggle='modal' data-target='#UpdatePlateModal' data-plate='" + res->getString("plate_number") + "' data-id='" + res->getString("id") + "' data-update='0' data-value ='" + sceneImage + "'>Update</button></td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody>" << endl;
        } else {
            Php::out << "<div class='p-3'>No plates available for correction</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeException("getPlateCorrectionRequiredTable", e.what());
        writeException("getPlateCorrectionRequiredTable", query);
    }
}

void getPlateMismatchTable() {
    try {      
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "SELECT  * FROM plates_mismatch T1 JOIN (Select device_number, max(id) id from plates_mismatch where date_time >= DATE_SUB(NOW(),INTERVAL 1 HOUR) and dismiss=0 and entry_plate_captured_id>0 and exit_plate_captured_id>0 group by device_number) T2 on T1.device_number = T2.device_number and T1.id = T2.id ORDER by T1.id desc";
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<thead>" << endl;
            Php::out << "<tr>" << endl;
            Php::out << "<th>Date Time</th>" << endl;           
            Php::out << "<th>Device name</th>" << endl;           
            Php::out << "<th>Ticket id</th>" << endl;           
            Php::out << "<th>Entry Plate Number</th>" << endl;
            Php::out << "<th>Exit Plate Number</th>" << endl;            
            Php::out << "<th>Action</th>" << endl;
            Php::out << "</tr>" << endl;
            Php::out << "</thead>" << endl;
            Php::out << "<tbody>" << endl;
            while (res->next()) {                

                Php::out << "<tr'>" << endl;
                Php::out << "<td >" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td >" + res->getString("device_name") + "</td>" << endl;
                Php::out << "<td >" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td >" + res->getString("entry_plate_number") + "</td>" << endl;
                Php::out << "<td >" + res->getString("exit_plate_number") + "</td>" << endl;                
                Php::out << "<td><button type='button' class='btn  btn-info col btn-plate-mismatch'  data-id='" + res->getString("id") + "'>Update</button></td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody>" << endl;
        } else {
            Php::out << "<div class='p-3'>No Plate Mismatch</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeException("getPlateMismatchTable", e.what());
        writeException("getPlateMismatchTable", query);
    }
}

void correctPlateNumber(Php::Value data) {
    try {
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        update = data["update"];
        if (update == 0)
            query = "update plates_captured set plate_corrected_date_time=CURRENT_TIMESTAMP,initial_plate_number='" + toString(data["plate_number"]) + "',plate_number='" + toString(data["corrected_plate_number"]) + "',user_id='" + toString(data["user_id"]) + "',user_name='" + toString(data["user_name"]) + "' where id='" + toString(data["id"]) + "'";
        else if (update == 1)
            query = "update plates_captured set plate_corrected_date_time=CURRENT_TIMESTAMP,plate_number='" + toString(data["corrected_plate_number"]) + "',user_id='" + toString(data["user_id"]) + "',user_name='" + toString(data["user_name"]) + "' where id='" + toString(data["id"]) + "'";
        else
            query = "update plates_captured set plate_corrected_date_time=CURRENT_TIMESTAMP,initial_plate_number='" + toString(data["plate_number"]) + "',user_id='" + toString(data["user_id"]) + "',user_name='" + toString(data["user_name"]) + "' where id='" + toString(data["id"]) + "'";
        rStmt->executeUpdate(query);

        if (update == 0 || update == 1) {
            query = "update open_transactions set plate_number='" + toString(data["corrected_plate_number"]) + "' where plate_captured_id='" + toString(data["id"]) + "'";
            rStmt->executeUpdate(query);
        }
        delete rStmt;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeException("correctPlateNumber", e.what());
    }

}

void getPlateCorrectedTable() {
    try {
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "SELECT capture_date_time,a.id,initial_plate_number,a.plate_number,plate_corrected_date_time,user_name,plate_image_name,camera_device_number,confidence_rate,entry_date_time FROM plates_captured a,open_transactions b where a.id=b.plate_captured_id and initial_plate_number is not null";
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<thead>" << endl;
            Php::out << "<tr>" << endl;
            Php::out << "<th>Plate Number</th>" << endl;
            Php::out << "<th>Corrected Plate Number</th>" << endl;
            Php::out << "<th>Entry Date Time</th>" << endl;
            Php::out << "<th>Confidence Rate</th>" << endl;
            //Php::out<<"<th>Plate Image</th>"<<endl;	
            Php::out << "<th>User name</th>" << endl;
            Php::out << "<th>Action</th>" << endl;
            Php::out << "</tr>" << endl;
            Php::out << "</thead>" << endl;
            Php::out << "<tbody>" << endl;
            while (res->next()) {
                image = res->getString("plate_image_name");
                while ((n = image.find(" ")) != std::string::npos)
                    image.replace(n, 1, "%20");
                while ((n = image.find("#")) != std::string::npos)
                    image.replace(n, 1, "%23");

                datefolder = res->getString("capture_date_time");
                datefolder = datefolder.substr(0, 10);

                croppedImage = res->getString("camera_device_number") + "/" + datefolder + "/Crop_" + image;
                sceneImage = res->getString("camera_device_number") + "/" + datefolder + "/Scene_" + image;


                Php::out << "<tr'>" << endl;
                Php::out << "<td >" + res->getString("initial_plate_number") + "</td>" << endl;
                Php::out << "<td >" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td >" + res->getString("entry_date_time") + "</td>" << endl;
                Php::out << "<td ><span class='badge bg-danger'>" + res->getString("confidence_rate") + "</span></td>" << endl;
                // Php::out<<"<td ><img src='http://localhost/ANPR/Images/Cropped/"+croppedImage+"' width='100' height='50'></td>"<<endl;                
                Php::out << "<td >" + res->getString("user_name") + "</td>" << endl;
                Php::out << "<td><button type='button' class='col btn btn-info' data-toggle='modal' data-target='#UpdatePlateModal' data-plate='" + res->getString("plate_number") + "' data-update='1' data-id='" + res->getString("id") + "' data-value ='" + sceneImage + "'>Update</button></td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody>" << endl;
        } else {
            Php::out << "<div class='p-3'>No plates corrected</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeException("getPlateCorrectedTable", e.what());
        writeException("getPlateCorrectedTable", query);
    }
}

int sendDataToPort(string ip,int port,string message) {
    
    try {        
        int sockfd; // socket file descriptor 	
        struct sockaddr_in serv_addr;
        struct hostent *server;

        sockfd = socket(AF_INET, SOCK_STREAM, 0); // generate file descriptor 
            if (sockfd < 0) 
                    {
                    writeException("sendDataToPort","ERROR opening socket "+ip+":"+to_string(port));
                    return 0;
                    }

            server = gethostbyname(ip.c_str()); //the ip address (or server name) of the listening server.
            if (server == NULL) 
                    {
                    writeException("sendDataToPort","ERROR, no such host "+ip+":"+to_string(port));
                    return 0;
                    }

            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
            serv_addr.sin_port = htons(port);
            struct timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

            if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
                    {
                    writeException("sendDataToPort","ERROR connecting "+ip+":"+to_string(port));                     
                    return 0;
                    } 

            /* write to port*/
            int n;
            char * wbuff; 
            string str = message+"\n";
            wbuff = (char *)str.c_str(); // convert from string to c string, has to have \0 terminal 

            n = write(sockfd, wbuff, strlen(wbuff));
            if(n < 0) 
                    writeException("sendDataToPort","Cannot write to socket");
            else
                    writeException("sendDataToPort",message +" -command send to "+ip+":"+to_string(port));

            close(sockfd);	
            }
	catch (exception &e)
            {
            writeException("sendDataToPort",e.what());            
            }
        return 1;
	}


void correctPlateMismatch(Php::Value data) {
    try {              
        string id=data["id"];
        con = General.mysqlConnect(ReportingDB);
        stmt = con->createStatement();
        query="select * from plates_mismatch where id="+id;
        res=stmt->executeQuery(query);
        if(res->next())
            {
            string entry_id=res->getString("entry_plate_captured_id");
            string exit_id=res->getString("exit_plate_captured_id"); 
            string device_number=res->getString("device_number");
            string user_id=data["user_id"];
            string user_name=data["user_name"];
            string plate_number1=data["plate_number1"];
            string plate_number2=data["plate_number2"];
            string corrected_plate_number1=data["corrected_plate_number1"];
            string corrected_plate_number2=data["corrected_plate_number2"];
            if(plate_number1!=corrected_plate_number1)
                {
                query = "update plates_captured set plate_corrected_date_time=CURRENT_TIMESTAMP,initial_plate_number='" + plate_number1 + "',plate_number='" + corrected_plate_number1 + "',user_id='" + user_id+ "',user_name='" +user_name + "' where id='" + entry_id + "'";
                stmt->executeUpdate(query);
                query = "update open_transactions set plate_number='" + corrected_plate_number1 + "' where plate_captured_id='" + entry_id+ "'";
                rStmt->executeUpdate(query);
                }
            if(plate_number2!=corrected_plate_number2)
                {
                query = "update plates_captured set plate_corrected_date_time=CURRENT_TIMESTAMP,initial_plate_number='" + plate_number2 + "',plate_number='" + corrected_plate_number2 + "',user_id='" + user_id+ "',user_name='" +user_name + "' where id='" + exit_id + "'";
                stmt->executeUpdate(query);
                }            
            delete res;
            query="select device_ip from parcx_server.system_devices where device_number="+device_number;
            res=stmt->executeQuery(query);
            if(res->next())
                {
                string ip=res->getString("device_ip");
                string message="C0"+exit_id+"C1P0"+corrected_plate_number2+"P1";
                if(sendDataToPort(ip,8091,message)==1)                    
                    query="update plates_mismatch set dismiss=2 where id="+id;
                else
                    query="update plates_mismatch set dismiss=1 where id="+id;
                
                    stmt->executeUpdate(query);
                    
                delete res;
                }
            }
        
                
        delete stmt;
        con->close();
        delete con;
    }    catch (const std::exception& e) {
        writeException("correctPlateMismatch", e.what());
    }

}

Php::Value getMismatchPlateDetails(Php::Value data)
    {
    Php::Value response;
    try
        {        
        string id=data["id"];
        string entry_id,exit_id;        
        con = General.mysqlConnect(ReportingDB);
        stmt = con->createStatement();
        query="select * from plates_mismatch where id="+id;
        res=stmt->executeQuery(query);
        if(res->next())
            {
            entry_id=res->getString("entry_plate_captured_id");
            exit_id=res->getString("exit_plate_captured_id");   
            response["result_description"]=string(res->getString("result_description"));
            }
        query = "select* from plates_captured where id="+entry_id;
        res=stmt->executeQuery(query);
        if(res->next())
            {            
            response["entry_plate_number"]=string(res->getString("plate_number"));
            image = res->getString("plate_image_name");
            while ((n = image.find(" ")) != std::string::npos)
                image.replace(n, 1, "%20");
            while ((n = image.find("#")) != std::string::npos)
                image.replace(n, 1, "%23");

            datefolder = res->getString("capture_date_time");
            datefolder = datefolder.substr(0, 10);
            
            sceneImage = res->getString("camera_device_number") + "/" + datefolder + "/Scene_" + image;
            response["entry_image"]=sceneImage;     
            delete res;
            } 
        query = "select* from plates_captured where id="+exit_id;
        res=stmt->executeQuery(query);
        if(res->next())
            {            
            response["exit_plate_number"]=string(res->getString("plate_number"));
            image = res->getString("plate_image_name");
            while ((n = image.find(" ")) != std::string::npos)
                image.replace(n, 1, "%20");
            while ((n = image.find("#")) != std::string::npos)
                image.replace(n, 1, "%23");

            datefolder = res->getString("capture_date_time");
            datefolder = datefolder.substr(0, 10);
            
            sceneImage = res->getString("camera_device_number") + "/" + datefolder + "/Scene_" + image;
            response["exit_image"]=sceneImage;  
            delete res;
            } 
        delete stmt;
        delete con;        
        }
     catch (const std::exception& e)
        {
        writeException("getMismatchPlateDetails", e.what()); 
        }
    return response;
    }

Php::Value hourlyOccupancyReport(Php::Value data) {
    try {
        string fld = "";
        string carpark = data["carpark"];
        string facility = data["facility"];
        string type = data["type"];
        Php::Array total_count;
        int i = 0;
        int j = 0;
        con = General.mysqlConnect(DashboardDB);
        stmt = con->createStatement();
        query = "select";
        while (i < 24) {
            query += " h" + std::to_string(i) + "to" + std::to_string((i + 1));
            if (i < 23) {
                query += ",";
            }
            i++;
        }


        query += " from hourly_occupancy where type='" + type + "' and data='realtime' and carpark_number=" + carpark + " and facility_number=" + facility;
        res = stmt->executeQuery(query);
        if (res->next()) {
            i = 0;
            j = 0;

            while (i < 24) {
                total_count[j] = res->getInt("h" + std::to_string(i) + "to" + std::to_string((i + 1)));
                i++;
                j++;
            }
        } else {
            i = 0;
            while (i < 24) {
                total_count[i] = 0;
                i++;
            }
        }

        delete res;
        delete stmt;
        delete con;

        return total_count;
    }    catch (const std::exception& e) {
        writeException("hourlyOccupancyReport", e.what());
        writeException("hourlyOccupancyReport", query);
        return "";
    }
}//End Hourly Occupancy

Php::Value averageHourlyOccupancyReport(Php::Value data) {
    try {
        string fld = "";
        string carpark = data["carpark"];
        string facility = data["facility"];
        int option = data["seloption"];

        Php::Array total_count;
        int i = 0;
        int j = 0;
        con = General.mysqlConnect(ReportingDB);
        stmt = con->createStatement();
        query = "select";
        while (i < 24) {
            query += " avg(h" + std::to_string(i) + "to" + std::to_string((i + 1)) + ") as h" + std::to_string(i) + "to" + std::to_string((i + 1));
            if (i < 23)
                query += ",";
            i++;
        }

        query += " from hourly_occupancy where carpark_number=" + carpark;
        if (option == 0)
            query += " and report_date=(DATE(NOW()) + INTERVAL -7 DAY)";
        if (option == 1)
            query += " and report_date between (DATE(NOW()) + INTERVAL -7 DAY) and CURRENT_DATE";
        if (option == 2)
            query += " and report_date<=CURRENT_DATE";


        res = stmt->executeQuery(query);
        if (res->next()) {
            i = 0;
            j = 0;

            while (i < 24) {
                total_count[j] = (int) res->getDouble("h" + std::to_string(i) + "to" + std::to_string((i + 1)));
                i++;
                j++;
            }
        } else {
            i = 0;
            while (i < 24) {
                total_count[i] = 0;
                i++;
            }
        }

        delete res;
        delete stmt;
        delete con;

        return total_count;
    }    catch (const std::exception& e) {
        writeException("averageHourlyOccupancyReport", e.what());
        writeException("averageHourlyOccupancyReport", query);
        return "";
    }
}//End Average Hourly Occupancyz

Php::Value parcxDashboard(Php::Parameters &params) {
    Php::Value data = params[0];
    int task = data["task"];
    Php::Value response;
    switch (task) {
        case 1:getPlateCorrectionRequiredTable();
            break;
        case 2:correctPlateNumber(data);
            break;
        case 3:getPlateCorrectedTable();
            break;
        case 4:response = hourlyOccupancyReport(data);
            break;
        case 5:response = averageHourlyOccupancyReport(data);
            break;
        case 6:getPlateMismatchTable();
            break;
        case 7:response=getMismatchPlateDetails(data);
            break;
        case 8:correctPlateMismatch(data);
        break;
    }
    return response;
}


extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PX_Dashboard", "1.0");
        extension.add<parcxDashboard>("parcxDashboard");
        return extension;
    }
}
