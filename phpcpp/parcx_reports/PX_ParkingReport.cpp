#include "PX_GeneralOperations.h"
#include "PX_ParkingReport.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <phpcpp.h>
#include <cmath>
#include <iostream>
#include <sstream>

#define ServerDB "parcx_server"
#define ReportingDB "parcx_reporting"
#define DashboardDB "parcx_dashboard"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
extern GeneralOperations General;
string query;
string entry_type, parking_duration;
string image, croppedImage, sceneImage, datefolder;
string current_date_time;
sql::ResultSet *res, *img, *res2;
size_t n;
int i = 0;

sql::Connection *rCon, *con;
sql::Statement *rStmt, *stmt;

void writeParkingReportLog(string function, string message) {
    General.writeLog("WebApplication/ApplicationLogs/PX-ParkingReport-" + General.currentDateTime("%Y-%m-%d"), function, message);
}

void writeParkingReportException(string function, string message) {
    Php::out << message << endl;
    Php::out << query << endl;
    General.writeLog("WebApplication/ExceptionLogs/PX-ParkingReport-" + General.currentDateTime("%Y-%m-%d"), function, message);
}

inline string toString(Php::Value param) {
    string value = param;
    return value;
}

string getLabel(string label, string lang) {
    sql::ResultSet *res_label;
    try {
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();
        query = "select " + lang + ",english from web_application_labels where message='" + label + "'";
        res_label = stmt->executeQuery(query);

        if (res_label->next()) {
            if (res_label->getString(lang) != "")
                label = res_label->getString(lang);
            else
                label = res_label->getString("english");
        }
        delete res_label;
        delete stmt;
        delete con;
        return label;
    }    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        writeParkingReportException("getLabel", e.what());
    }
    return "";
}

string ParkingReport::getApplicationLabel(Php::Value data) {
    string lang = data["language"];
    string label = data["label"];
    try {
        label = getLabel(label, lang);
    }    catch (const std::exception& e) {
        writeParkingReportException("getLabel", e.what());
    }
    return label;
}

/*Php::Value getLabels(string lang,string label)
    {     
    Php::Value result;
    try
        {  
        if (label.find(',') != std::string::npos)
            {
            stringstream ss(label);
            string substr;
            label="";
            while (getline(ss, substr, ',')) 
                {                        
                label=label+"'"+substr+"'"+",";
                }
            label=label.substr(0,label.size()-1);
            query= "select "+lang+",message,english from web_application_labels where message in("+label+")";            
            }
        else
            query= "select "+lang+",message,english from web_application_labels where message='"+label+"'";	
            
        sql::ResultSet *res; 
        con = General.mysqlConnect(ServerDB);
        stmt = con->createStatement();
        
        res=stmt->executeQuery(query);
        
        if(res->rowsCount()==1)
            {
            if(res->next())
                {
                if(res->getString(lang)!="")
                    label=res->getString(lang);
                else
                    res->getString("english");                
                }
            result=label;
            }
        else
            {
            while(res->next())                
                {
                if(res->getString(lang)!="")
                    result[res->getString("message").asStdString()]=res->getString(lang).asStdString();                
                else
                    result[res->getString("message").asStdString()]=res->getString("english").asStdString();                
                }
            }
        delete res;
        delete stmt;
        delete con;	
        return result;
        }
    catch(const std::exception& e)
        {
        writeParkingReportException("getApplicationLabels",e.what());	
        }
    return result;  
    }

Php::Value ParkingReport::getApplicationLabels(Php::Value data)
    {
    string lang=data["language"];  
    int page=data["page"];                
    string label=data["label"];    
    if(page>0)
        {
        string reportlabels="choose_datetime_range,view_report,export,export_to_excel,export_to_pdf,logout,search,entries_label,info_label,previous,next,";
        switch(page)
            {
            case 1://"watchdog_device_alarms"
                    label=reportlabels+"watchdog_device_alarms,select_severity,low,high,medium,all_devices,select_devices";
                    break;
            case 2 ://payment_transactions
                label=reportlabels+"payment_transactions,detailed_payment,receipt_details,close,print,tax_invoice,cash,credit_card,parking_fee,lost_ticket,discount,grace_period,product_sales,all_devices,select_devices,all_carparks,select_carparks,all_category,select_category,all_payment,select_payment,all_discount,select_discount";
                    break;
            case 3 ://revenue_summary
                label = reportlabels+"sunday,monday,tuesday,wednesday,thursday,friday,saturday,total_revenue";
            }
        }
    return  getLabels(lang,label);
    }*/

void ParkingReport::openTransactionReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string device = json["device"];
        string val = json["plate_number"];
        string delay = json["parking_duration"];
        string lang = json["language"];
        current_date_time = General.currentDateTime(dateTimeFormat);

        string labels = "device_name,transactions,date_time,plate_number,ticket_id,exit_grace_period,image,entry_type,payment_date_time,entry_date_time,no_records,duration,amount_paid,entry_grace_period";
        Php::Value label = General.getLabels(lang, labels);

        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from open_transactions where entry_date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "' and movement_type=1";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";
        if (device.length() > 0)
            query = query + " AND device_number in(" + device + ")";
        if (delay != "") {
            if (delay == "0")
                query = query + " AND entry_date_time>=DATE_SUB(NOW(),INTERVAL 1 Day)";
            else
                query = query + " AND entry_date_time<=DATE_SUB(NOW(),INTERVAL " + delay + " Day)";

        }
        if (val.length() > 0)
            query = query + " AND(plate_number like '%" + val + "%' OR ticket_id like '%" + val + "%')";

        query = query + " ORDER BY  entry_date_time desc";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {

            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << label["transactions"] << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;



            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>" << label["device_name"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_number"] << "</th>" << endl;
            Php::out << "<th>" << label["ticket_id"] << "</th>" << endl;
            Php::out << "<th>" << label["entry_type"] << "</th>" << endl;
            Php::out << "<th>" << label["entry_date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["duration"] << "</th>" << endl;
            Php::out << "<th>" << label["entry_grace_period"] << "</th>" << endl;
            Php::out << "<th>" << label["amount_paid"] << "</th>" << endl;
            Php::out << "<th>" << label["payment_date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["exit_grace_period"] << "</th>" << endl;
            Php::out << "<th>" << label["image"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            while (res->next()) {
                if (res->getInt("entry_type") == 1)
                    entry_type = "Short Term";
                if (res->getInt("entry_type") == 2)
                    entry_type = "Access";
                if (res->getInt("entry_type") == 3)
                    entry_type = "Reservation";

                croppedImage = "";
                sceneImage = "";

                if (res->getString("plate_captured_id") != "") {
                    query = "select * from plates_captured where id=" + res->getString("plate_captured_id");
                    img = rStmt->executeQuery(query);

                    if (img->next()) {
                        image = img->getString("plate_image_name");
                        while ((n = image.find(" ")) != std::string::npos)
                            image.replace(n, 1, "%20");
                        while ((n = image.find("#")) != std::string::npos)
                            image.replace(n, 1, "%23");

                        datefolder = img->getString("capture_date_time");
                        datefolder = datefolder.substr(0, 10);

                        croppedImage = img->getString("camera_device_number") + "/" + datefolder + "/Crop_" + image;
                        sceneImage = img->getString("camera_device_number") + "/" + datefolder + "/Scene_" + image;
                        delete img;
                    }

                }


                int *duration = General.differenceDateTime(current_date_time, res->getString("entry_date_time"), dateTimeFormat);
                if (duration[1] > 0)
                    parking_duration = to_string(duration[1]) + " D " + to_string(duration[2]) + " H " + to_string(duration[3]) + " M";
                else
                    parking_duration = to_string(duration[2]) + " H " + to_string(duration[3]) + " M";

                Php::out << "<tr>" << endl;
                Php::out << "<td> " + res->getString("device_name") + " </td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td>" + entry_type + "</td>" << endl;
                Php::out << "<td>" + res->getString("entry_date_time") + "</td>" << endl;
                Php::out << "<td>" + parking_duration + "</td>" << endl;
                Php::out << "<td>" + res->getString("entry_grace_period") + "</td>" << endl;
                Php::out << "<td>" + res->getString("total_amount_paid") + "</td>" << endl;
                Php::out << "<td>" + res->getString("last_payment_date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("max_exit_date_time") + "</td>" << endl;
                Php::out << "<td><img src='" + toString(json["image_location"]) + "/" + croppedImage + "' width='100' height='50' data-value ='" + sceneImage + "' data-toggle='modal' data-target='#image-modal' alt='" + res->getString("plate_number") + "'></td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" << label["no_records"] << "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("openTransactionReport", e.what());
    }
}

void ParkingReport::parkingMovementsReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string device = json["device"];
        string val = json["plate_number"];
        string lang = json["language"];
        current_date_time = General.currentDateTime(dateTimeFormat);

        string labels = "transactions,date_time,plate_number,ticket_id,device_name,no_records";
        Php::Value label = General.getLabels(lang, labels);


        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_movements where date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";
        if (device.length() > 0)
            query = query + " AND device_number in(" + device + ")";
        if (val.length() > 0)
            query = query + " AND(plate_number like '%" + val + "%' OR ticket_id like '%" + val + "%')";

        query = query + " ORDER BY  date_time desc";
        //Php::out<<query<<endl;
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << toString(label["transactions"]) << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;

            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << toString(label["date_time"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["plate_number"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["ticket_id"]) << "</th>" << endl;
            //Php::out<<"<th>Entry Grace Period</th>"<<endl;                                            
            Php::out << "<th>" << toString(label["device_name"]) << "</th>" << endl;
            //Php::out<<"<th>Carpark Name</th>"<<endl;	  

            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                //Php::out<<"<td>"+res->getString("entry_grace_period")+"</td>"<<endl;                     
                Php::out << "<td> " + res->getString("device_name") + " </td>" << endl;
                //Php::out<<"<td>"+res->getString("carpark_name")+"</td>"<<endl;                
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("parkingMovementsReport", e.what());
    }
}

void ParkingReport::blacklistReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string lang = json["language"];
        current_date_time = General.currentDateTime(dateTimeFormat);

        string labels = "transactions,date_time,device_name,blacklisting_reason,description,no_records";
        Php::Value label = General.getLabels(lang, labels);


        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_blacklist where date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";
        query = query + " ORDER BY  date_time desc";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << toString(label["transactions"]) << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;

            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th></th>" << endl;
            Php::out << "<th>" << toString(label["date_time"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["ticket_id"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["device_name"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["blacklisting_reason"]) << "</th>" << endl;
            Php::out << "<th>" << toString(label["description"]) << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td> " + res->getString("device_name") + " </td>" << endl;
                Php::out << "<td>" + res->getString("blacklisting_reason") + "</td>" << endl;
                Php::out << "<td>" + res->getString("blacklisting_description") + "</td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("blacklistReport", e.what());
    }
}

void ParkingReport::validationReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string validator = json["validator"];
        string product = json["product"];
        string val = json["plate_number"];
        string lang = json["language"];

        string labels = "transactions,entry_date_time,ticket_id,validator,product,plate_number,no_records";
        Php::Value label = General.getLabels(lang, labels);


        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_validation where date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";
        if (validator.length() > 0)
            query = query + " AND validator_id in(" + validator + ")";
        if (product.length() > 0)
            query = query + " AND product_id in(" + product + ")";
        if (val.length() > 0)
            query = query + " AND(plate_number like '%" + val + "%' OR ticket_id like '%" + val + "%')";

        query = query + " ORDER BY  date_time desc";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << label["transactions"] << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;


            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["entry_date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_number"] << "</th>" << endl;
            Php::out << "<th>" << label["ticket_id"] << "</th>" << endl;
            Php::out << "<th>" << label["validator"] << "</th>" << endl;
            Php::out << "<th>" << label["product"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td>" + res->getString("validator_name") + "</td>" << endl;
                Php::out << "<td>" + res->getString("product_name") + "</td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" << label["no_records"] << "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("parkingMovementsReport", e.what());
    }
}

void ParkingReport::accessReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string device = json["device"];
        string val = json["plate_number"];
        string option = json["option"];
        string user_role = json["user_role"];
        string lang = json["language"];
        string user_name = "";
        current_date_time = General.currentDateTime(dateTimeFormat);

        string labels = "date_time,customer_name,plate_number,tag,status,device_name,carpark_name,ticket_id,no_records,transactions";
        Php::Value label = General.getLabels(lang, labels);

        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_movements_access where date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";
        if (device.length() > 0)
            query = query + " AND device_number in(" + device + ")";
        if (option.length() > 0)
            query = query + " AND status_message like '%" + option + "%'";
        if (val.length() > 0)
            query = query + " AND(plate_number like '%" + val + "%' OR ticket_id like '%" + val + "%' OR tag like '%" + val + "%')";


        query = query + " ORDER BY  date_time desc";
        //Php::out<<query<<endl;
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            con = General.mysqlConnect(ServerDB);
            stmt = con->createStatement();
            query = "select * from access_whitelist";
            res2 = stmt->executeQuery(query);

            delete stmt;
            con->close();
            delete con;

            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << label["transactions"] << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;


            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["customer_name"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_number"] << "</th>" << endl;
            Php::out << "<th>" << label["ticket_id"] << "</th>" << endl;
            Php::out << "<th>" << label["tag"] << "</th>" << endl;
            Php::out << "<th>" << label["status"] << "</th>" << endl;
            Php::out << "<th>" << label["device_name"] << "</th>" << endl;
            Php::out << "<th>" << label["carpark_name"] << "</th>" << endl;
            Php::out << "<th></th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            string status, tag;
            int whitelist_added;
            while (res->next()) {
                i++;
                status = res->getString("status_message");
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                res2->beforeFirst();
                user_name = "";
                while (res2->next()) {
                    if (res->getString("ticket_id") == res2->getString("ticket_id")) {
                        user_name = res2->getString("customer_name");
                        break;
                    }
                }
                Php::out << "<td>" + user_name + "</td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td>" + res->getString("tag") + "</td>" << endl;
                Php::out << "<td>" + res->getString("status_message") + "</td>" << endl;
                Php::out << "<td> " + res->getString("device_name") + " </td>" << endl;
                Php::out << "<td>" + res->getString("carpark_name") + "</td>" << endl;
                tag = res->getString("tag");
                whitelist_added = res->getInt("whitelist_added");
                if (status == "TAG UNKNOWN" && whitelist_added == 0 && tag.length() > 2) {
                    //Php::out<<"<td><input data-toggle='modal' data-target='#addToWhiteList' type='botton' class='btn btn-warning btnWhitelistAdd' value='Add to whitelist' data-tag='"<<res->getString("tag")<<"' data-id="<<res->getInt("id")<<"></td>";                        
                    Php::out << "<td><button type='button' class='col btn btn-warning btnWhitelistAdd' data-tag='" << res->getString("tag") << "' data-id=" << res->getInt("id") << ">Add to whitelist</button></td>" << std::endl;
                }
                else
                    Php::out << "<td></td>" << endl;

                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" << label["no_records"] << "</div>" << endl;
        }

        delete rStmt;
        delete res;
        delete res2;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("accessReport", e.what());
    }
}

void ParkingReport::trackTicket(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string val = json["plate_number"];
        string type = json["type"];
        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_movements where movement_type=1 and date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";

        if (val.length() > 0)
            query = query + " AND(plate_number like '%" + val + "%' OR ticket_id like '%" + val + "%')";

        query = query + " ORDER BY  date_time desc";

        //Php::out<<query<<endl;

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>Transactions</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;


            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th></th>" << endl;
            Php::out << "<th>Ticket Id</th>" << endl;
            Php::out << "<th>Plate Number</th>" << endl;
            Php::out << "<th>Entry Date Time</th>" << endl;
            Php::out << "<th>Entry Device</th>" << endl;
            Php::out << "<th>Exit Date Time</th>" << endl;
            Php::out << "<th>Exit Device</th>" << endl;
            Php::out << "<th>Carpark Name</th>" << endl;

            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td> " + res->getString("device_name") + " </td>" << endl;
                if (res->getString("ticket_id") != "") {
                    query = "Select * from parking_movements where movement_type=2 and ticket_id = '" + res->getString("ticket_id") + "' and date_time>'" + res->getString("date_time") + "'";
                    res2 = rStmt->executeQuery(query);
                    if (res2->next()) {
                        Php::out << "<td>" + res2->getString("date_time") + "</td>" << endl;
                        Php::out << "<td> " + res2->getString("device_name") + " </td>" << endl;
                        delete res2;
                    } else {
                        Php::out << "<td></td><td></td>" << endl;
                    }

                }
                Php::out << "<td>" + res->getString("carpark_name") + "</td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>No record found</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("parkingMovementsReport", e.what());
    }
}

void ParkingReport::visitorFrequencyReport(Php::Value json) {
    string visit_type = json["visit_type"];
    string lang = json["language"];
    try {
        int n = 0;
        string id = "0";
        string labels = "plate_number,ticket_id,no_records,transactions,tag,category,last_7_days,last_30_days,last_60_days,last_90_days,last_120_days,last_150_days,last_180_days";
        Php::Value label = General.getLabels(lang, labels);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();

        if (visit_type == "all")
            query = "Select * from visitor_frequency";
        else
            query = "Select * from visitor_frequency where category = '" + visit_type + "'";
        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << toString(label["transactions"]) << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" + toString(label["plate_number"]) + "</th>" << endl;
            Php::out << "<th>" + toString(label["ticket_id"]) + "</th>" << endl;
            if (visit_type != "shortterm")
                Php::out << "<th>" + toString(label["tag"]) + "</th>" << endl;

            Php::out << "<th>" + toString(label["last_7_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_30_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_60_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_90_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_120_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_150_days"]) + "</th>" << std::endl;
            Php::out << "<th>" + toString(label["last_180_days"]) + "</th>" << std::endl;
            if (visit_type == "all")
                Php::out << "<th>" + toString(label["category"]) + "</th>" << std::endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            n = 1;

            while (res->next()) {
                Php::out << "<tr>" << std::endl;
                Php::out << "<td>" + to_string(n) + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << std::endl;
                if (visit_type != "shortterm")
                    Php::out << "<td>" + res->getString("tag_id") + "</td>" << std::endl;

                Php::out << "<td>" + res->getString("d7") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d30") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d60") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d90") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d120") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d150") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("d180") + "</td>" << std::endl;
                if (visit_type == "all")
                    Php::out << "<td>" + res->getString("category") + "</td>" << std::endl;
                Php::out << "</tr>" << std::endl;
                n++;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete res;
        delete rStmt;
        delete rCon;


    }    catch (const std::exception& e) {
        writeParkingReportException("visitorFrequencyReport", e.what());
    }
}

void ParkingReport::visitorFrequencyRealTimeReport(Php::Value json) {
    string from_date = json["fromDate"];
    string to_date = json["toDate"];
    string visit_type = json["visit_type"];
    string contract_type = json["contract_type"];
    string search_text = json["search_text"];
    string lang = json["language"];
    try {
        int n = 0;
        string id = "0";
        current_date_time = General.currentDateTime(dateTimeFormat);

        string labels = "plate_number,date_time,tag,device_name,ticket_id,no_of_visits,no_records,transactions";
        Php::Value label = General.getLabels(lang, labels);

        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        if (visit_type == "shortterm") {
            if (search_text == "")
                query = "Select count(plate_number) as count,plate_number,ticket_id from parking_movements where date_time>='" + from_date + "' and date_time<='" + to_date + "' group by plate_number";
            else
                query = "Select count(plate_number) as count,plate_number,ticket_id from parking_movements where date_time>='" + from_date + "' and date_time<='" + to_date + "' and plate_number='" + search_text + "'";
        } else {
            if (contract_type == "anpr") {
                if (search_text == "")
                    query = "Select count(plate_number) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "'  group by ticket_id,tag";
                else
                    query = "Select count(plate_number) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "' and plate_number='" + search_text + "' group by ticket_id,tag";
            } else if (contract_type == "ticketid") {
                if (search_text == "")
                    query = "Select count(ticket_id) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "' group by ticket_id,plate_number";
                else
                    query = "Select count(ticket_id) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "' and ticket_id='" + search_text + "' group by ticket_id,plate_number";
            } else if (contract_type == "tag") {
                if (search_text == "")
                    query = "Select count(tag) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "'  and tag>'' group by tag,plate_number";
                else
                    query = "Select count(tag) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "' and tag='" + search_text + "' and tag>'' group by tag,plate_number";
            } else {
                query = "Select count(*) as count,plate_number,ticket_id,tag from parking_movements_access where date_time>='" + from_date + "' and date_time<='" + to_date + "' group by plate_number,ticket_id,tag";
            }
        }

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << toString(label["transactions"]) << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;


            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << std::endl;
            Php::out << "<th>" << toString(label["plate_number"]) << "</th>" << std::endl;
            Php::out << "<th>" << toString(label["ticket_id"]) << "</th>" << std::endl;
            if (visit_type == "contract")
                Php::out << "<th>" << toString(label["tag"]) << "</th>" << std::endl;

            Php::out << "<th>" << toString(label["no_of_visits"]) << "</th>" << std::endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            n = 1;

            while (res->next()) {
                Php::out << "<tr>" << std::endl;
                Php::out << "<td>" + to_string(n) + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("ticket_id") + "</td>" << std::endl;

                if (visit_type == "contract")
                    Php::out << "<td>" + res->getString("tag") + "</td>" << std::endl;
                Php::out << "<td>" + res->getString("count") + "</td>" << std::endl;
                Php::out << "</tr>" << std::endl;
                n++;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete res;
        delete rStmt;
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("visitorFrequencyRealTimeReport", e.what());
    }
}

void ParkingReport::ticketDetails(Php::Value json) {
    try {
        sql::ResultSet *entry, *exit, *access, *reservation, *payment;
        string ticket = json["ticket_id"];
        string status = "Exited";
        string type = "";
        string lang = json["language"];
        int entry_type = 0;
        string entry_date_time = "", exit_date_time = "", parking_duration = "", entry_grace_period = "", payment_date_time = "", exit_grace_period = "", amount_paid = "", plate_number = "";

        string labels = "status,entry_details,entry_type,plate_number,entry_date_time,entry_grace_period,entry_device_name,entry_carpark_name,access_details,customer_name,validity_start_date,validity_expiry_date,reservation_details,customer_name,validity_start_date,validity_expiry_date,payment_details,payment_date_time,maximum_exit_date_time,payment_device_name,payment_carpark_name,amount_paid,discount_name,validation_value,exit_details,exit_time,exit_device_name,exit_carpark_name,invalid_ticket";
        Php::Value label = General.getLabels(lang, labels);
        entry = NULL;
        exit = NULL;
        reservation = NULL;
        access = NULL;
        payment = NULL;


        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        current_date_time = General.currentDateTime(dateTimeFormat);
        query = "select * from open_transactions where movement_type=1 and ticket_id='" + ticket + "'";
        entry = rStmt->executeQuery(query);
        if (entry->next()) {
            entry_type = entry->getInt("entry_type");
            plate_number = entry->getString("plate_number");
            if (entry_type == 1)
                type = "Shortterm entry";
            else if (entry_type == 2)
                type = "Access parking";
            else
                type = "Reservation parking";

            if (entry_type == 2) {
                query = "select * from parcx_server.access_whitelist where ticket_id='" + ticket + "'";
                access = rStmt->executeQuery(query);
                if (access->next())
                    entry_grace_period = access->getString("validity_expiry_date");
            }
            if (entry_type == 3) {
                query = "select * from parking_reservation where ticket_id='" + ticket + "'";
                reservation = rStmt->executeQuery(query);
                if (reservation->next())
                    entry_grace_period = reservation->getString("to_time");
            }

            entry_date_time = entry->getString("entry_date_time");

            int *duration = General.differenceDateTime(current_date_time, entry_date_time, dateTimeFormat);
            if (duration[1] > 0)
                parking_duration = to_string(duration[1]) + " D " + to_string(duration[2]) + " H " + to_string(duration[3]) + " M";
            else
                parking_duration = to_string(duration[2]) + " H " + to_string(duration[3]) + " M";

            if (entry_type == 1)
                entry_grace_period = entry->getString("entry_grace_period");

            int *entry_grace = General.differenceDateTime(entry_grace_period, current_date_time, dateTimeFormat);
            if (entry_grace[0] > 0) {
                status = "Within entry grace period";
            } else {
                status = "Unpaid";
                payment_date_time = entry->getString("last_payment_date_time");
                if (payment_date_time != "") {
                    exit_grace_period = entry->getString("max_exit_date_time");
                    amount_paid = entry->getString("total_amount_paid");
                    int *exit_grace = General.differenceDateTime(exit_grace_period, current_date_time, dateTimeFormat);
                    if (exit_grace[0] > 0) {
                        status = "Within exit grace period";
                    } else {
                        status = "Maximum exit grace period exceeded";
                    }
                    query = "select * from revenue_payments where ticket_id='" + ticket + "' and payment_date_time between '" + entry_date_time + "' and current_timestamp";
                    payment = rStmt->executeQuery(query);
                }
            }
        }
        else {
            query = "select * from parking_movements where movement_type=1 and ticket_id='" + ticket + "'";
            entry = rStmt->executeQuery(query);
            if (entry->next()) {
                type = "Shortterm entry";
                entry_type = 1;
            } else {
                query = "select * from parking_movements_access where movement_type=1 and ticket_id='" + ticket + "' and status_message='Access Allowed'";
                entry = rStmt->executeQuery(query);
                if (entry->next()) {
                    type = "Access parking";
                    entry_type = 2;
                } else {
                    query = "select * from parking_movements_reservation where movement_type=1 and ticket_id='" + ticket + "'";
                    entry = rStmt->executeQuery(query);
                    if (entry->next()) {
                        type = "Reservation parking";
                        entry_type = 3;
                    }
                }
            }
            if (entry_type != 0) {
                entry_date_time = entry->getString("date_time");
                plate_number = entry->getString("plate_number");

                if (entry_type == 2) {
                    query = "select * from parcx_server.access_whitelist where ticket_id='" + ticket + "'";
                    access = rStmt->executeQuery(query);
                    if (access->next())
                        entry_grace_period = access->getString("validity_expiry_date");
                }
                if (entry_type == 3) {
                    query = "select * from parking_reservation where ticket_id='" + ticket + "'";
                    reservation = rStmt->executeQuery(query);
                    if (reservation->next())
                        entry_grace_period = reservation->getString("to_time");
                }

                if (entry_type == 1)
                    query = "select * from parking_movements where movement_type=2 and ticket_id='" + ticket + "'";
                else if (entry_type == 2)
                    query = "select * from parking_movements_access where movement_type=2 and ticket_id='" + ticket + "' and status_message='Access Allowed'";
                else
                    query = "select * from parking_movements_reservation where movement_type=2 and ticket_id='" + ticket + "'";
                exit = rStmt->executeQuery(query);
                if (exit->next()) {
                    exit_date_time = exit->getString("date_time");
                    int *duration = General.differenceDateTime(exit_date_time, entry_date_time, dateTimeFormat);
                    if (duration[1] > 0)
                        parking_duration = to_string(duration[1]) + " D " + to_string(duration[2]) + " H " + to_string(duration[3]) + " M";
                    else
                        parking_duration = to_string(duration[2]) + " H " + to_string(duration[3]) + " M";
                }

                query = "select * from revenue_payments where ticket_id='" + ticket + "' and payment_date_time between '" + entry_date_time + "' and '" + exit_date_time + "'";
                payment = rStmt->executeQuery(query);
                if (payment->rowsCount() > 0)
                    amount_paid = "0";
            }

        }

        if (entry_type != 0) {
            Php::out << toString(label["status"]) << ": " << status << "<br>" << endl;
            Php::out << "<h3>" << toString(label["entry_details"]) << "</h3>" << endl;
            Php::out << "" << toString(label["entry_type"]) << ": " << type << "<br>" << endl;
            Php::out << "" << toString(label["plate_number"]) << ": " << plate_number << "<br>" << endl;
            Php::out << "" << toString(label["entry_date_time"]) << ": " << entry_date_time << "<br>" << endl;
            Php::out << "" << toString(label["entry_grace_period"]) << ": " << entry_grace_period << "<br>" << endl;
            Php::out << "" << toString(label["entry_device_name"]) << ": " << entry->getString("device_name") << "<br>" << endl;
            Php::out << "" << toString(label["entry_carpark_name"]) << ": " << entry->getString("carpark_name") << "<br>" << endl;

            delete entry;

            if (entry_type == 2 && access->rowsCount() > 0) {
                Php::out << "<h3>" << toString(label["access_details"]) << "</h3>" << endl;
                Php::out << "" << toString(label["customer_name"]) << ": " << access->getString("customer_name") << "<br>" << endl;
                Php::out << "" << toString(label["validity_start_date"]) << ": " << access->getString("validity_start_date") << "<br>" << endl;
                Php::out << "" << toString(label["validity_expiry_date"]) << ": " << access->getString("validity_expiry_date") << "<br>" << endl;
                delete access;
            }

            if (entry_type == 3 && reservation->rowsCount() > 0) {
                Php::out << "<h3>" << toString(label["reservation_details"]) << "</h3>" << endl;
                Php::out << "" << toString(label["customer_name"]) << ": " << reservation->getString("name") << "<br>" << endl;
                Php::out << "" << toString(label["validity_start_date"]) << ": " << reservation->getString("from_time") << "<br>" << endl;
                Php::out << "" << toString(label["validity_expiry_date"]) << ": " << reservation->getString("to_time") << "<br>" << endl;
                delete reservation;
            }

            if (amount_paid != "") {
                Php::out << "<h3>" << toString(label["payment_details"]) << "</h3>" << endl;
                if (payment->next()) {
                    Php::out << "" << toString(label["payment_date_time"]) << ": " << payment->getString("payment_date_time") << "<br>" << endl;
                    Php::out << "" << toString(label["maximum_exit_date_time"]) << ": " << payment->getString("max_exit_date_time") << "<br>" << endl;
                    Php::out << "" << toString(label["payment_device_name"]) << ": " << payment->getString("device_name") << "<br>" << endl;
                    Php::out << "" << toString(label["payment_carpark_name"]) << ": " << payment->getString("carpark_name") << "<br>" << endl;
                    Php::out << "" << toString(label["amount_paid"]) << ": " << payment->getString("gross_amount") << "<br>" << endl;
                    if (payment->getInt("discount_id") != 0) {
                        Php::out << "" << toString(label["discount_name"]) << ": " << payment->getString("discount_name") << "<br>" << endl;
                    }
                    if (payment->getString("validation_id") != "" && payment->getString("validation_id") != "0") {
                        Php::out << "" << toString(label["validation_value"]) << ": " << payment->getString("validation_value") << "<br>" << endl;
                    }
                    delete payment;
                }
            }

            if (exit_date_time != "") {
                Php::out << "<h3>" << toString(label["exit_details"]) << "</h3>" << endl;
                Php::out << "" << toString(label["exit_time"]) << ": " << exit_date_time << "<br>" << endl;
                Php::out << "" << toString(label["exit_device_name"]) << ": " << exit->getString("device_name") << "<br>" << endl;
                Php::out << "" << toString(label["exit_carpark_name"]) << ": " << exit->getString("carpark_name") << "<br>" << endl;
                delete exit;
            }

        } else
            Php::out << "" << toString(label["invalid_ticket"]) << "" << endl;

        delete res;
        delete rStmt;
        delete rCon;

    }    catch (const std::exception& e) {
        writeParkingReportException("ticketDetails", e.what());
    }
}

void ParkingReport::watchdogNetworkLogs(Php::Value json) {
    int n = 1;
    try {
        string from = json["from"];
        string to = json["to"];
        string lang = json["language"];
        string labels = "device_name,ip_address,connection_lost,connection_restored,carpark_name,no_records";
        Php::Value label = General.getLabels(lang, labels);
        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from watchdog_network_logs where connection_failure between '" + from + "' AND '" + to + "'";
        query = query + "ORDER BY connection_failure ASC";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["device_name"] << "</th>" << endl;
            Php::out << "<th>" << label["ip_address"] << "</th>" << endl;
            Php::out << "<th>" << label["connection_lost"] << "</th>" << endl;
            Php::out << "<th>" << label["connection_restored"] << "</th>" << endl;
            Php::out << "<th>" << label["carpark_name"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            while (res->next()) {
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << to_string(n) << "</td>" << endl;
                Php::out << "<td>" + res->getString("device_name") + "</td>" << endl;
                Php::out << "<td>" + res->getString("device_ip") + "</td>" << endl;
                Php::out << "<td> " + res->getString("connection_failure") + " </td>" << endl;
                Php::out << "<td> " + res->getString("connection_restored") + " </td>" << endl;
                Php::out << "<td> " + res->getString("carpark_name") + " </td>" << endl;
                Php::out << "</tr>" << endl;
                n++;
            }
            Php::out << "</tbody></table>" << endl;
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("watchdogNetworkLogs", e.what());
    }
}

void ParkingReport::watchDogDeviceAlarms(Php::Value json) {
    try {
        string from = json["from"];
        string to = json["to"];
        string device = json["device"];
        string severity = json["severity"];
        string lang = json["language"];
        int n = 1;
        string labels = "date_time,device_name,severity,description,high,medium,low,no_records";
        Php::Value label = General.getLabels(lang, labels);
        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from watchdog_device_alarms where alarm_date_time between '" + from + "' AND '" + to + "'";
        if (device.length() > 0)
            query = query + " AND device_number in(" + device + ") ";
        if (severity != "0")
            query = query + " AND alarm_severity " + severity + " ";
        query = query + "ORDER BY alarm_date_time DESC";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["device_name"] << "</th>" << endl;
            Php::out << "<th>" << label["severity"] << "</th>" << endl;
            Php::out << "<th>" << label["description"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            string temp = "";
            while (res->next()) {
                if (res->getInt("alarm_severity") >= 1 && res->getInt("alarm_severity") <= 3)
                    temp = "<span class='badge badge-danger'>" + toString(label["high"]) + "</span>";
                else if (res->getInt("alarm_severity") >= 4 && res->getInt("alarm_severity") <= 7)
                    temp = "<span class='badge badge-warning'>" + toString(label["medium"]) + "</span>";
                else if (res->getInt("alarm_severity") >= 8 && res->getInt("alarm_severity") <= 10)
                    temp = "<span class='badge badge-secondary'>" + toString(label["low"]) + "</span>";
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << to_string(n) << "</td>" << endl;
                Php::out << "<td>" + res->getString("alarm_date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("device_name") + "</td>" << endl;
                Php::out << "<td> " + temp + " </td>" << endl;
                Php::out << "<td> " + res->getString("alarm_description") + " </td>" << endl;
                Php::out << "</tr>" << endl;
                n++;
            }
            Php::out << "</tbody></table>" << endl;
            //query="select message,"+lang+" from parcx_server.web_application_messages where message like 'alarm_%'";
            //res2=rStmt->executeQuery(query);
            //while(res2->next())
            //   label[res2->getString("message").asStdString()]=res2->getString(lang).asStdString();

            /*Php::out<<"<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>"<<endl; 
            Php::out<<"<thead><tr>"<<endl;
            Php::out<<"<th>#</th>"<<endl;                                  
            Php::out<<"<th>"<<label["date_time"]<<"</th>"<<endl;                         
            Php::out<<"<th>"<<label["device_name"]<<"</th>"<<endl;	  
            Php::out<<"<th>"<<label["severity"]<<"</th>"<<endl;                         
            Php::out<<"<th>"<<label["description"]<<"</th>"<<endl;	     
            Php::out<<"</tr></thead>"<<endl;
            Php::out<<"<tbody>"<<endl;
                        
            while(res->next())
            {  
                
                
                Php::out<<"<tr>"<<endl;  
                Php::out<<"<td>"<<to_string(n)<<"</td>"<<endl;                              
                Php::out<<"<td>"+res->getString("alarm_date_time")+"</td>"<<endl;                   
                Php::out<<"<td>"+res->getString("device_name")+"</td>"<<endl; 
                
                if(res->getInt("alarm_severity")>=1 && res->getInt("alarm_severity")<=3)
                    Php::out<<"<td><span class='badge badge-danger'>"<<label["high"]<<"</span></td>"<<endl; 
                else if(res->getInt("alarm_severity")>=4 && res->getInt("alarm_severity")<=7)
                    Php::out<<"<td><span class='badge badge-warning'>"<<label["medium"]<<"</span></td>"<<endl; 
                else if(res->getInt("alarm_severity")>=8 && res->getInt("alarm_severity")<=10)
                    Php::out<<"<td><span class='badge badge-secondary'>"<<label["low"]<<"</span></td>"<<endl; 
                
                //string description=label["alarm_"+res->getString("alarm_code")];
                //if(description=="")                    
                    Php::out<<"<td>"<<res->getString("alarm_description")<<"</td>"<<endl;   
                //else
                 //   Php::out<<"<td> "<<description<<" </td>"<<endl;   
                Php::out<<"</tr>"<<endl;     
                n++;
            }  
            Php::out<<"</tbody></table>"<<endl;*/
        } else {
            Php::out << "<div class='p-0'>" + toString(label["no_records"]) + "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("watchDogDeviceAlarms", e.what());
    }
}

void ParkingReport::manualmovementReport(Php::Value json) {
    try {
        string carpark = json["carpark"];
        string operation = json["operation"];
        string lang = json["language"];

        string labels = "date_time,operator_name,action,reason,description,device_name,no_records,transactions";
        Php::Value label = General.getLabels(lang, labels);

        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from parking_movements_manual where date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";
        if (carpark.length() > 0)
            query = query + " AND carpark_number in(" + carpark + ")";

        if (operation.length() > 1)
            query = query + " AND action LIKE '%" + operation + "%'";


        query = query + " ORDER BY  date_time desc";

        res = rStmt->executeQuery(query);
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << label["transactions"] << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;

            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["device_name"] << "</th>" << endl;
            Php::out << "<th>" << label["operator_name"] << "</th>" << endl;
            Php::out << "<th>" << label["action"] << "</th>" << endl;
            Php::out << "<th>" << label["reason"] << "</th>" << endl;
            Php::out << "<th>" << label["description"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;

            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                Php::out << "<td>" + res->getString("date_time") + "</td>" << endl;
                Php::out << "<td>" + res->getString("device_name") + "</td>" << endl;
                Php::out << "<td> " + res->getString("operator_name") + " </td>" << endl;
                Php::out << "<td>" + res->getString("action") + "</td>" << endl;
                Php::out << "<td>" + res->getString("reason") + "</td>" << endl;
                Php::out << "<td>" + res->getString("description") + "</td>" << endl;
                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {

            Php::out << "<div class='p-0'>" << label["no_records"] << "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("manualmovmentReport", e.what());
    }
}

string str_replace(string str, string find_str, string replace_str) {
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

void ParkingReport::platescapturedReport(Php::Value json) {
    Php::Value device_name;
    string lang = json["language"];
    string ImageURL = json["imageurl"];
    string device = json["device"];
    try {


        string labels = "date_time,plate_number,plate_area,plate_country,cropped_image,full_image,camera_name,no_records";
        Php::Value label = General.getLabels(lang, labels);

        current_date_time = General.currentDateTime(dateTimeFormat);
        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from plates_captured where capture_date_time between '" + toString(json["from"]) + "' and '" + toString(json["to"]) + "'";

        if (device.length() > 0)
            query = query + " and camera_device_number in(" + device + ")";

        if (toString(json["plate"]) != "")
            query = query + " and plate_number like '%" + toString(json["plate"]) + "%'";

        if (toString(json["type"]) != "")
            query = query + " and plate_type = '" + toString(json["type"]) + "'";

        if (toString(json["area"]) != "")
            query = query + " and plate_area = '" + toString(json["area"]) + "'";

        if (toString(json["country"]) != "")
            query = query + " and plate_country = '" + toString(json["country"]) + "'";

        query = query + " order by capture_date_time desc";


        res = rStmt->executeQuery(query);
        //Php::out<<query<<endl;
        if (res->rowsCount() > 0) {
            Php::out << "<div class='row mb-4 jspdf-graph'>" << endl;
            Php::out << "<div class='col-lg-3 col-6'>" << endl;
            Php::out << "<div class='small-box bg-success'>" << endl;
            Php::out << "<div class='inner'>" << endl;
            Php::out << "<h3>" << res->rowsCount() << "</h3>" << endl;
            Php::out << "<h6>" << label["transactions"] << "</h6>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;
            Php::out << "</div>" << endl;

            Php::out << "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>" << endl;
            Php::out << "<thead><tr>" << endl;
            Php::out << "<th>#</th>" << endl;
            Php::out << "<th>" << label["camera_name"] << "</th>" << endl;
            Php::out << "<th>" << label["date_time"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_number"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_area"] << "</th>" << endl;
            Php::out << "<th>" << label["plate_country"] << "</th>" << endl;
            Php::out << "<th>" << label["cropped_image"] << "</th>" << endl;
            Php::out << "<th>" << label["full_image"] << "</th>" << endl;
            Php::out << "</tr></thead>" << endl;
            Php::out << "<tbody>" << endl;
            i = 0;
            while (res->next()) {
                i++;
                Php::out << "<tr>" << endl;
                Php::out << "<td>" << i << "</td>" << endl;
                //device_name = getdevicename(res->getInt("camera_device_number"));
                Php::out << "<td>" << endl;


                con = General.mysqlConnect(ServerDB);
                stmt = con->createStatement();
                res2 = stmt->executeQuery("select device_name from system_devices where device_number=" + res->getString("camera_device_number"));
                if (res2->next()) {
                    Php::out << res2->getString("device_name") << endl;
                } else {
                    Php::out << "Unkown" << endl;
                }
                delete res2;
                delete stmt;
                delete con;

                string plate_image = res->getString("plate_image_name");
                plate_image = str_replace(plate_image, " ", "%20");
                plate_image = str_replace(plate_image, "#", "%23");

                string plate_date = res->getString("capture_date_time");
                plate_date = plate_date.substr(0, 10);


                Php::out << "</td>" << endl;
                Php::out << "<td>" + res->getString("capture_date_time") + "<input type='hidden' id = 'date" + to_string(i) + "' value = '" + plate_date + "'><input type='hidden' id = 'camera_no" + to_string(i) + "' value = '" + res->getString("camera_device_number") + "'><input type='hidden' id = 'plate_image" + to_string(i) + "' value = '" + res->getString("plate_image_name") + "'></td>" << endl;
                Php::out << "<td>" + res->getString("plate_number") + "</td>" << endl;
                Php::out << "<td> " + res->getString("plate_area") + " </td>" << endl;
                Php::out << "<td>" + res->getString("plate_country") + "</td>" << endl;
                Php::out << "<td><img src='" + ImageURL + "\\" + res->getString("camera_device_number") + "\\" + plate_date + "\\Crop_" + plate_image + "' width='100' ; height='50' ;></td>" << endl;
                Php::out << "<td><input type='button' data-value = " + to_string(i) + " data-toggle='modal' data-target='#image-modal' value='View' class='btn btn-link'></td>" << endl;

                Php::out << "</tr>" << endl;
            }
            Php::out << "</tbody></table>" << endl;
        } else {

            Php::out << "<div class='p-0'>" << label["no_records"] << "</div>" << endl;
        }

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;
    }    catch (const std::exception& e) {
        writeParkingReportException("platescapturedReport", e.what());
    }
}

void ParkingReport::getanprImage(Php::Value json) {
    string plate_image = json["plate_image"];

    plate_image = str_replace(plate_image, " ", "%20");
    plate_image = str_replace(plate_image, "#", "%23");
    string camera_id = json["camera_no"];
    string plate_date = json["date"];
    string ImageURL = json["imageurl"];
    Php::out << "<div>" << endl;
    //int html_data+='<img src ="'.ImageURL.'/default.jpg" width="100%"; height="500";>';
    Php::out << "<img src ='" + ImageURL + "\\" + camera_id + "\\" + plate_date + "\\Scene_" + plate_image + "' width='700'; height='700';>" << endl;
    ;
    Php::out << "</div>" << endl;


}

string Addtime(string sdate) {
    char out[30];
    string addtime;

    struct tm tm;

    strptime(sdate.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    // time_t e = mktime(&tm);

    tm.tm_sec = tm.tm_sec + 3600;
    //time_t t = mktime(&tm);
    strftime(out, 30, "%Y-%m-%d %H:%M:%S", &tm);
    string stime(out);
    addtime = stime;


    return addtime;


}

string AddDate(string sdate) {
    char out[30];
    string adddate;

    struct tm tm;
    strptime(sdate.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    // time_t e = mktime(&tm);


    tm.tm_sec += 86400;
    // time_t t = mktime(&tm);
    strftime(out, 30, "%Y-%m-%d %H:%M:%S", &tm);
    string stime(out);
    adddate = stime;


    return adddate;


}

void ParkingReport::trafficReport(Php::Value json) {
    int show_date_by_report = 0;
    Php::Value datearr, day_report;
    int entry_count[24];
    int exit_count[24];
    int access_entry[24];
    int access_exit[24];
    int manualopen_count[24];
    int manualclose_count[24];
    //int pushbutton_open_count[24];     
    //int count[24];
    try {
        string from = json["fromDate"]; //"2020-12-01 00:00:00";
        string to = json["toDate"]; //"2020-12-30 00:00:00";
        string carpark = json["carpark"];
        string weekdays = json["weekdays"];
        string dayclosure = "00:00:00"; //json["dayclosureStart"];
        string temp_date;
        string lang = json["language"];
        //Php::out<<from<<endl;
        //Php::out<<to<<endl;
        int *d = General.differenceDateTime(to, from, "%Y-%m-%d");
        string labels = "entries,short_term,exits,manual_operation,entry_exit,report_date,shortterm_entries,shortterm_exits,contract_entries,contract_exits,manual_entry,manual_exit,from,to,total,no_records";
        Php::Value label = General.getLabels(lang, labels);
        //Php::out<<d[1]<<endl;
        //Php::out<<from<<endl;
        //AddDate("2019-06-01 00:00:00");
        datearr[0] = from.substr(0, 10);

        i = 1;
        temp_date = from + " 00:00:00";
        string date_add;
        string from1;
        while (i <= d[1]) {
            //datearr[i] = AddDate(temp_date);
            date_add = AddDate(temp_date);
            from1 = date_add.substr(0, 10);
            datearr[i] = from1;
            //Php::out<<from1<<endl;
            temp_date = date_add;
            i++;

        }
        for (int k = 0; k < datearr.size(); k++) {
            string key = datearr[k];
            day_report[key]["short_term_entry"] = 0;
            day_report[key]["short_term_exit"] = 0;
            day_report[key]["access_entry"] = 0;
            day_report[key]["access_exit"] = 0;
            day_report[key]["manual_entry"] = 0;
            day_report[key]["manual_exit"] = 0;
        }
        //Php::out<<day_report["2020-12-08"]["short_term_entry"]<<endl;
        //Php::out<<day_report["2020-12-06"]["short_term_entry"]<<endl;

        if (d[1] > 1) {
            show_date_by_report = 1;
        }



        string current_date = General.currentDateTime("%Y-%m-%d %H:%M:%S");
        int total_entry = 0;
        int total_exit = 0;
        int total_mopen = 0;
        int total_mclose = 0;
        int access_entry_total = 0;
        int access_exit_total = 0;
        //int total_pushbutton_open = 0;

        for (i = 0; i < 24; i++) {
            entry_count[i] = 0;
            exit_count[i] = 0;
            manualopen_count[i] = 0;
            manualclose_count[i] = 0;
            // pushbutton_open_count[i] = 0;
            access_entry[i] = 0;
            access_exit[i] = 0;
        }





        rCon = General.mysqlConnect(ReportingDB);
        rStmt = rCon->createStatement();
        query = "select * from summary_parking_movements where  report_date between '" + from + "' and '" + to + "'";
        if (carpark.length() > 0)
            query = query + " AND car_park_number IN (" + carpark + ")";

        if (weekdays.length() > 0)
            query = query + " AND report_day IN (" + weekdays + ")";

        res = rStmt->executeQuery(query);
        //Php::out<<query<<endl;
        int category;
        int movement_type;
        string day, field;
        int temp = 0;
        string data = "", summary = "", header = "", html_data = "";
        if (res->rowsCount() > 0) {
            while (res->next()) {
                category = res->getInt("category");
                movement_type = res->getInt("movement_type");
                day = res->getString("report_date");


                switch (category) {
                    case 1:
                        if (movement_type == 1) {
                            temp = day_report[day]["short_term_entry_count"];
                            day_report[day]["short_term_entry_count"] = res->getInt("total_count") + temp;
                        }
                        if (movement_type == 2) {
                            temp = day_report[day]["short_term_exit_count"];
                            day_report[day]["short_term_exit_count"] = res->getInt("total_count") + temp;
                        }
                        break;
                    case 2:
                        if (movement_type == 1) {
                            temp = day_report[day]["access_entry_count"];
                            day_report[day]["access_entry_count"] = res->getInt("total_count") + temp;
                        }
                        if (movement_type == 2) {
                            temp = day_report[day]["access_exit_count"];
                            day_report[day]["access_exit_count"] = res->getInt("total_count") + temp;
                        }
                        break;
                    case 3:
                        if (movement_type == 1 || movement_type == 3) {
                            temp = day_report[day]["manual_entry_count"];
                            day_report[day]["manual_entry_count"] = res->getInt("total_count") + temp;
                        }
                        if (movement_type == 2 || movement_type == 4) {
                            temp = day_report[day]["manual_exit_count"];
                            day_report[day]["manual_exit_count"] = res->getInt("total_count") + temp;
                        }
                        break;
                }


                i = 0;

                while (i < 24) {
                    field = "h" + to_string(i) + "to" + to_string((i + 1));
                    switch (category) {
                        case 1:
                            if (movement_type == 1) {
                                entry_count[i] = res->getInt(field) + entry_count[i];
                                total_entry = res->getInt(field) + total_entry;
                            }
                            if (movement_type == 2) {
                                exit_count[i] = res->getInt(field) + exit_count[i];
                                total_exit = total_exit + res->getInt(field);
                            }
                            break;

                        case 2:
                            if (movement_type == 1) {
                                access_entry[i] = res->getInt(field) + access_entry[i];
                                access_entry_total = access_entry_total + res->getInt(field);
                            }
                            if (movement_type == 2) {
                                access_exit[i] = res->getInt(field) + access_exit[i];
                                access_exit_total = access_exit_total + res->getInt(field);
                            }
                            break;

                        case 3:
                            if (movement_type == 1 || movement_type == 3) {
                                manualopen_count[i] = res->getInt(field) + manualopen_count[i];
                                total_mopen = total_mopen + res->getInt(field);
                            }
                            if (movement_type == 2 || movement_type == 4) {
                                manualclose_count[i] = res->getInt(field) + manualclose_count[i];
                                total_mclose = res->getInt(field) + total_mclose;
                            }
                            break;



                            /* 
                           case 5:
                           pushbutton_open_count[i] = res->getInt(field);
                           total_pushbutton_open = total_pushbutton_open + res->getInt(field);
                           break;                                                                                                   
                            
                           case 10:
                           count[i]=(int)data[field];
                           total=(int)data['total_count'];;
                           break;
                            
                            
                           case 50: // exit counts during grace period 
                           count[i]=(int)data[field];
                           total_exit_during_grace=(int)data['total_count'];;
                           break;
                             */

                    }
                    i++;
                }

            }

            summary = "<div class='row mb-4 jspdf-graph'>";
            summary += "<div class='col-lg-3 col'>";
            summary += "<div class='small-box bg-success'>";
            summary += "<div class='inner'>";
            summary += "<h3>" + to_string(total_entry) + "/" + to_string(access_entry_total) + "</h3>";
            summary += "     <p>" + toString(label["entries"]) + "<br>[" + toString(label["short_term"]) + "]</p>";
            summary += "    </div>";
            summary += "    <div class='icon'>";
            summary += "    <i class='ion ion-pie-graph'></i>";
            summary += "   </div>";
            summary += " </div>";
            summary += " </div>";
            summary += " <div class='col-lg-3 col'>";
            summary += "  <div class='small-box bg-success'>";
            summary += "    <div class='inner'>";
            summary += "     <h3>" + to_string(total_exit) + "/" + to_string(access_exit_total) + "</h3>";
            summary += "    <p>" + toString(label["exits"]) + "<br>[" + toString(label["short_term"]) + "]</p>";
            summary += "  </div>";
            summary += "  <div class='icon'>";
            summary += "  <i class='ion ion-pie-graph'></i>";
            summary += "  </div>";
            summary += "</div>";
            summary += " </div>";

            summary += "   <div class='col-lg-3 col'>";
            summary += "<div class='small-box bg-success'>";
            summary += "  <div class='inner'>";
            summary += "     <h3>" + to_string(total_mopen) + "/" + to_string(total_mclose) + "</h3>";
            summary += "   <p>" + toString(label["manual_operation"]) + "<br>" + toString(label["entry_exit"]) + "</p>";
            summary += " </div>";
            summary += " <div class='icon'>";
            summary += " <i class='ion ion-pie-graph'></i>";
            summary += " </div>";
            summary += "</div>";
            summary += "</div>";

            summary += "</div> <!--End. Row -->";

            Php::out << summary << endl;

            // End . Traffic Summary 

            /*Date by report*/
            if (show_date_by_report == true) {
                //header = "<table width='100%' class='jspdf-table'>";
                //header += "<tr class='card-header d-flex justify-content-between align-items-center'>";
                header += "<table id='TABLE_1' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
                header += "<thead><tr><th>" + toString(label["report_date"]) + "</th>";
                header += "<th>" + toString(label["shortterm_entries"]) + "</th>";
                header += "<th>" + toString(label["shortterm_exits"]) + "</th>";
                header += "<th>" + toString(label["contract_entries"]) + "</th>";
                header += "<th>" + toString(label["contract_exits"]) + "</th>";
                header += "<th>" + toString(label["manual_entry"]) + "</th>";
                header += "<th>" + toString(label["manual_exit"]) + "</th>";
                header += "</tr></thead>";
                //Php::out<<header<<endl;
                html_data += header;
                html_data += "<tbody>";



                //day_report=array_reverse(day_report,true);

                /*foreach(day_report as key=>value)
                    {                
                    html_data += "<tr class='card-text d-flex justify-content-between align-items-center'>";                
                    html_data += "<td>" . key . " </td>";                
                    html_data += "<td>" . day_report[key]["short_term_entry_count"] . " </td>";
                    html_data += "<td>" . day_report[key]["short_term_exit_count"] . " </td>";
                    html_data += "<td>" . day_report[key]["access_entry_count"] . " </td>";
                    html_data += "<td>" . day_report[key]["access_exit_count"] . " </td>";
                    html_data += "<td>" . day_report[key]["manual_entry_count"] . " </td>";
                    html_data += "<td>" .day_report[key]["manual_exit_count"] . " </td>";
                    
                    html_data += "</tr>";
                    }*/
                for (int k = datearr.size() - 1; k >= 0; k--) {
                    html_data += "<tr>";
                    string key = datearr[k];
                    html_data += "<td>" + key + "</td>";
                    if (toString(day_report[key]["short_term_entry_count"]) == "") {
                        day_report[key]["short_term_entry_count"] = 0;
                    }
                    if (toString(day_report[key]["short_term_exit_count"]) == "") {
                        day_report[key]["short_term_exit_count"] = 0;
                    }
                    if (toString(day_report[key]["access_entry_count"]) == "") {
                        day_report[key]["access_entry_count"] = 0;
                    }
                    if (toString(day_report[key]["access_exit_count"]) == "") {
                        day_report[key]["access_exit_count"] = 0;
                    }
                    if (toString(day_report[key]["manual_entry_count"]) == "") {
                        day_report[key]["manual_entry_count"] = 0;
                    }
                    if (toString(day_report[key]["manual_exit_count"]) == "") {
                        day_report[key]["manual_exit_count"] = 0;
                    }
                    html_data += "<td>" + toString(day_report[key]["short_term_entry_count"]) + "</td>";
                    html_data += "<td>" + toString(day_report[key]["short_term_exit_count"]) + " </td>";
                    html_data += "<td>" + toString(day_report[key]["access_entry_count"]) + " </td>";
                    html_data += "<td>" + toString(day_report[key]["access_exit_count"]) + " </td>";
                    html_data += "<td>" + toString(day_report[key]["manual_entry_count"]) + " </td>";
                    html_data += "<td>" + toString(day_report[key]["manual_exit_count"]) + " </td>";
                    html_data += "</tr>";
                }




                html_data += "<tr>";
                html_data += "<td>" + toString(label["total"]) + "</td>";
                html_data += "<td>" + to_string(total_entry) + "</td>";
                html_data += "<td>" + to_string(total_exit) + "</td>";
                html_data += "<td>" + to_string(access_entry_total) + "</td>";
                html_data += "<td>" + to_string(access_exit_total) + "</td>";
                html_data += "<td>" + to_string(total_mopen) + "</td>";
                html_data += "<td>" + to_string(total_mclose) + "</td>";
                html_data += "</tr>";

                html_data += "</table><br>";
                Php::out << html_data << endl;
            }


            // Begin . Hourly Traffic Statistics a

            //html_data = "";

            //header = "<table width='100%' class='jspdf-table'>";
            header = "";
            header += "<table id='TABLE_2' class='table table-blue table-bordered table-striped jspdf-table RecordsTableclass'>";
            header += "<thead><tr>";

            header += "<th>" + toString(label["from"]) + "</th>";
            header += "<th>" + toString(label["to"]) + "</th>";
            header += "<th>" + toString(label["shortterm_entries"]) + "</th>";
            header += "<th>" + toString(label["shortterm_exits"]) + "</th>";
            header += "<th>" + toString(label["contract_entries"]) + "</th>";
            header += "<th>" + toString(label["contract_exits"]) + "</th>";
            header += "<th>" + toString(label["manual_entry"]) + "</th>";
            header += "<th>" + toString(label["manual_exit"]) + "</th>";

            header += "</tr></thead>";

            html_data = header;
            html_data += "<tbody>";

            Php::out << html_data << endl;

            html_data = "";
            i = 0;
            string time_from = "2020-01-01 " + dayclosure;
            string time_to;
            int entryData[24];
            int exitData[24];
            while (i < 24) {
                // data for chart                

                entryData[i] = entry_count[i] + access_entry[i];
                exitData[i] = exit_count[i] + access_exit[i];
                //Php::out<<"Entrydata"<<entryData[i]<<endl;
                //Php::out<<"Exitdata"<<exitData[i]<<endl;
                html_data += "<span display='none' id='chartData_" + to_string(i) + "' data-entry='" + to_string(entryData[i]) + "' data-exit='" + to_string(exitData[i]) + "'></span>";

                // end / data for chart
                time_to = Addtime(time_from);
                //Php::out<<"to:"<<time_to<<endl;

                html_data += "<tr>";

                html_data += "<td>" + time_from.substr(11, 8) + " </td>";
                html_data += "<td>" + time_to.substr(11, 8) + " </td>";
                html_data += "<td>" + to_string(entry_count[i]) + " </td>";
                html_data += "<td>" + to_string(exit_count[i]) + " </td>";
                html_data += "<td>" + to_string(access_entry[i]) + " </td>";
                html_data += "<td>" + to_string(access_exit[i]) + " </td>";
                html_data += "<td>" + to_string(manualopen_count[i]) + " </td>";
                html_data += "<td>" + to_string(manualclose_count[i]) + " </td>";

                html_data += "</tr>";

                time_from = time_to;

                i++;
            }

            html_data += "<tr>";

            html_data += "<td>" + toString(label["total"]) + "</td>";
            html_data += "<td></td>";
            html_data += "<td>" + to_string(total_entry) + "</td>";
            html_data += "<td>" + to_string(total_exit) + "</td>";
            html_data += "<td>" + to_string(access_entry_total) + "</td>";
            html_data += "<td>" + to_string(access_exit_total) + "</td>";
            html_data += "<td>" + to_string(total_mopen) + "</td>";
            html_data += "<td>" + to_string(total_mclose) + "</td>";

            html_data += "</tr></tbody>";

            html_data += "</table>";

            //End . Hourly Traffic Statistics 
            Php::out << html_data << endl;



        } else {
            Php::out << "<div class='card p-3'>" + toString(label["no_records"]) + "</div>" << endl;
        }
        //echo html_data;   

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;

    }    catch (const std::exception& e) {
        writeParkingReportException("trafficReport", e.what());
    }

}

int* get_parking_duration(string fromdate, string todate, string carpark, string category, string weekdays, int* total_count) {
    //static int total_count[30];
    string field, query_string;
    int i = 0;
    int j = 0;
    int k = 1;

    rCon = General.mysqlConnect(ReportingDB);
    rStmt = rCon->createStatement();
    query_string = "select sum(m0to30) as m0to30,sum(m30to60) as m30to60";

    i = 1;
    while (i < 24) {
        query_string += ",sum(h" + to_string(i) + "to" + to_string((i + 1)) + ") as h" + to_string(i) + "to" + to_string((i + 1));
        i++;
    }
    k = 1;
    while (k < 3) {
        query_string += ",sum(d" + to_string(k) + "to" + to_string((k + 1)) + ") as d" + to_string(k) + "to" + to_string((k + 1));
        k++;
    }

    query_string += ",max(maximum_parking_duration) as max_parking_duration,min(nullif(minimum_parking_duration,0)) as min_parking_duration,avg(average_parking_duration) as avg_parking_duration  from parking_duration where report_date between '" + fromdate + "' AND '" + todate + "' and category='" + category + "'";

    if (carpark.length() > 0) {
        if (carpark.length() > 0)
            query_string = query_string + " AND carpark_number in(" + carpark + ")";

    }

    if (weekdays.length() > 0)
        query_string = query_string + " AND report_day IN (" + weekdays + ")";

    res = rStmt->executeQuery(query_string);
    //Php::out<<query_string<<endl;

    if (res->next()) {

        total_count[0] = res->getInt("m0to30");

        total_count[1] = res->getInt("m30to60");
        i = 1;
        j = 2;
        //get count for hours
        while (i < 24) {
            field = "h" + to_string(i) + "to" + to_string((i + 1));
            //Php::out<<field<<":"<<res->getInt(field)<<endl;
            total_count[j] = res->getInt(field);
            i++;
            j++;
        }

        //get count for days      
        k = 1;
        while (k < 3) {
            field = "d" + to_string(k) + "to" + to_string((k + 1));
            total_count[j] = res->getInt(field);
            k++;
            j++;
        }
        total_count[j++] = res->getInt("max_parking_duration");
        total_count[j++] = res->getInt("min_parking_duration");
        total_count[j++] = res->getInt("avg_parking_duration");

        delete rStmt;
        delete res;
        rCon->close();
        delete rCon;

        return total_count;

    } else {
        i = 0;
        while (i < 15) {
            total_count[i] = 0;
            i++;
        }

        return total_count;
    }


} // End get parking duration

void ParkingReport::parkingDuration(Php::Value json) {


    //int* count_shortterm[29];
    //int* count_contract[29];
    string duration_label[29];
    string duration_chart_label[29];
    int contract[30];
    int shortterm[30];
    int max_dur_short_term;
    int max_dur_contract;
    int min_dur_short_term;
    int min_dur_contract;
    int avg_dur_short_term;
    int avg_dur_contract;

    int count1 = 0, count2 = 0;
    int i = 0;
    int j = 0;
    string html_data = "";
    double temp = 0, temp2;
    int temp1, temp3;
    string header = "";

    try {
        string from = json["fromDate"]; //"2020-12-01 00:00:00";
        string to = json["toDate"]; //"2020-12-30 00:00:00";
        string carpark = json["carpark"];
        string weekdays = json["weekdays"];
        string dayclosure = "00:00:00"; //json["dayclosureStart"];
        string temp_date;
        string lang = json["language"];


        string labels = "maximum_parking_duration,minimum_parking_duration,average_parking_duration,shortterm,contract,shortterm_parkers,contract_parkers,duration,no_records";
        Php::Value label = General.getLabels(lang, labels);





        int* count_shortterm = get_parking_duration(from, to, carpark, "shortterm", weekdays, shortterm);
        //Php::out<<"Size of shortterm array:"<<sizeof(count_shortterm)<<endl;
        for (int n = 0; n < 30; n++) {
            //Php::out<<n<<":"<<count_shortterm[n]<<"   "<<endl;
            count1 += count_shortterm[n];
        }

        int* count_contract = get_parking_duration(from, to, carpark, "longterm", weekdays, contract);
        for (n = 0; n < 30; n++) {
            //Php::out<<n<<":"<<count_contract[n]<<"   "<<endl;
            count2 += count_contract[n];
        }
        //Php::out<<"shortterm:"<<count1<<endl;
        //Php::out<<"longterm:"<<count2<<endl;

        //CREATE THE DURATION LABELS 
        duration_label[0] = "0 - 30 Min";
        duration_label[1] = "30 - 60 Min";

        duration_chart_label[0] = "0 - 30\nMin";
        duration_chart_label[1] = "30 - 60\nMin";

        i = 2;

        while (i <= 24) {
            duration_label[i] = to_string((i - 1)) + " - " + to_string(i) + " Hours";
            duration_chart_label[i] = to_string((i - 1)) + " - " + to_string(i) + "\n" + "Hours";
            i++;
        }

        j = 1;
        while (j < 3) {
            duration_label[i] = to_string(j) + " - " + to_string((j + 1)) + " Days";
            duration_chart_label[i] = to_string(j) + " - " + to_string((j + 1)) + "\n" + "Days";
            i++;
            j++;
        }

        if ((count1 > 0) || (count2 > 0)) {

            html_data = "";

            // create display minutes/hours text for info box

            max_dur_short_term = count_shortterm[27];
            max_dur_contract = count_contract[27];
            min_dur_short_term = count_shortterm[28];
            min_dur_contract = count_contract[28];
            avg_dur_short_term = count_shortterm[29];
            avg_dur_contract = count_contract[29];


            /*Php::out<<"max_dur_short_term :"<<count_shortterm[27];
            Php::out<<"   max_dur_contract :"<<count_contract[27];
            Php::out<<"   min_dur_short_term :"<<count_shortterm[28];
            Php::out<<"   min_dur_contract :"<<count_contract[28];
            Php::out<<"   avg_dur_short_term :"<<count_shortterm[29];
            Php::out<<"   avg_dur_contract :"<<count_contract[29];*/

            int display_values[] = {max_dur_short_term, max_dur_contract, min_dur_short_term, min_dur_contract, avg_dur_short_term, avg_dur_contract};

            // print_r($display_values);
            string display_values_str[6];
            for (i = 0; i < 6; i++) {

                if (display_values[i] < 60) {
                    display_values_str[i] = to_string(display_values[i]) + "m";
                } else {
                    // Php::out<<"Value:"<<to_string(display_values[i])<<endl;



                    temp = (double) display_values[i] / 60; // 737/60 = 12.28
                    //Php::out<<" Temp:"<<temp<<endl;

                    temp1 = floor(temp); // 12 hours
                    //Php::out<<" Temp1:"<<temp1<<endl;

                    temp2 = temp - temp1; // 12.28 - 12 = 0.28
                    //Php::out<<" Temp2:"<<temp2<<endl;

                    temp3 = floor(temp2 * 60); // 0.28 * 60 = 17 minutes
                    //Php::out<<" Temp3:"<<temp3<<endl;

                    display_values_str[i] = to_string(temp1) + "h " + to_string(temp3) + "m";
                    //Php::out<<" Temp2:"<<display_values_str[i]<<endl;
                }
            }

            // print_r($display_values);

            header += "<div class='row mb-4 jspdf-graph text-center'>";
            header += "<div class='col-md-4 pl-0'>";
            header += "<!-- small card -->";
            header += "<div class='small-box bg-info'>";
            header += "<h5 class='small-box-footer text-white'>" + toString(label["maximum_parking_duration"]) + "</h5>";
            header += "<div class='inner row'>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[0] + "</h3>";
            header += "<h5>" + toString(label["shortterm"]) + "</h5>";
            header += "</div>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[1] + "</h3>";
            header += "<h5>" + toString(label["contract"]) + "</h5>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "<div class'col-md-4'>";
            header += "<!-- small card -->";
            header += "<div class='small-box bg-info'>";
            header += "<h5 class='small-box-footer text-white'>" + toString(label["minimum_parking_duration"]) + "</h5>";
            header += "<div class='inner row'>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[2] + "</h3>";
            header += "<h5>" + toString(label["shortterm"]) + "</h5>";
            header += "</div>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[3] + "</h3>";
            header += "<h5>" + toString(label["contract"]) + "</h5>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "<div class='col-md-4 pr-0'>";
            header += "<!-- small card -->";
            header += "<div class='small-box bg-info'>";
            header += "<h5 href='#' class='small-box-footer text-white'>" + toString(label["average_parking_duration"]) + "</h5>";
            header += "<div class='inner row'>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[4] + "</h3>";
            header += "<h5>" + toString(label["shortterm"]) + "</h5>";
            header += "</div>";
            header += "<div class='col'>";
            header += "<h3>" + display_values_str[5] + "</h3>";
            header += "<h5>" + toString(label["contract"]) + "</h5>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "</div>";
            header += "</div>";

            header += "<div class='col-md-6 p-0'>";

            // begin header



            header += "<table id='RecordsTable' class='table table-blue table-bordered table-striped jspdf-table'>";
            header += "<thead><tr>";

            header += "<th>" + toString(label["duration"]) + "</th>";
            header += "<th>" + toString(label["shortterm_parkers"]) + "</th>";
            header += "<th>" + toString(label["contract_parkers"]) + "</th>";

            header += "</tr></thead>";

            html_data += header;

            html_data += "<tbody>";

            i = 0;

            while (i <= (30 - 4)) {

                // data for chart

                html_data += "<span display='none' id='chartData_" + to_string(i) + "' data-duration='" + duration_chart_label[i] + "' data-short-term='" + to_string(count_shortterm[i]) + "' data-contract='" + to_string(count_contract[i]) + "'></span>";

                // end / data for chart

                html_data += "<tr>";

                html_data += "<td>" + duration_label[i] + "</td>";
                html_data += "<td>" + to_string(count_shortterm[i]) + "</td>";
                html_data += "<td>" + to_string(count_contract[i]) + "</td>";

                html_data += "</tr>";

                i++;
            }

            html_data += "</tbody</table>";

            html_data += "</div>";

        } else {
            html_data = "<div class='card'><div class='p-3'>" + toString(label["no_records"]) + "</div></div>";
        }
        Php::out << html_data << endl;

    }    catch (const std::exception& e) {
        writeParkingReportException("parkingDuration", e.what());
    }
} //END. Show parking duration report 