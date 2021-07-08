#include "PX_GeneralOperations.h"
#include "PX_Reservation.h"
#include "PX_Access.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <curl/curl.h>
#include<jsoncpp/json/json.h>
#include <sstream>
#include <iomanip>

#define CloudDB "pxcloud_server"
#define dateTimeFormat "%Y-%m-%d %H:%M:%S"

using namespace std;
GeneralOperations General;
Reservation ReservationObj;
Access AccessObj;
int entry_grace_period_reservation,exit_grace_period_reservation,rate_plan_id=0;
int entry_grace_period_minutes, exit_grace_period_minutes,parking_rate,parking_duration,parking_fee_duration;
double parking_fee;


/*int couponEnabled,paymentExit, deductMoneyFromWallet, carparkNumber, facilityNumber, deviceNumber, deviceType, task, entryGrace, exitGrace, reservationEnabled, accessEnabled, deviceFunction, seconds, anprEnabled, cameraId, plateCapturedInterval, plateCapturedId, validationEnabled, walletEnabled, entryType;
string current_coupon_id,deviceName, carpakName, facilityName, ticketId, plateNumber, parkingZone, query, entryDateTime, maxEntryGrace, maxExitGrace, currentDateTime, plateType, plateArea, plateCountry;
string timeValidationId, percentageValidationId, validationDuration, userId, accessExpiry, reservationExpiry, paymentDateTime, accessResult, accessResultDescription, reservationResult, reservationStart, reservationResultDescription;
int test,offlineTimeValidation, offlinePercentageValidation, timeValidation, percentageValidation, parkingDuration, validationSeconds, validationHours, rate_type = 0;
double startparkingFee, parkingFee, vat_percentage = 0, amountPaid = 0, grossAmount, vatAmount, reservationFee, walletBalance;
sql::Connection *reportCon, *serverCon;
sql::Statement *stmt, *serverStmt;
sql::ResultSet *res, *validation, *settings, *device, *rate,*coupon;
CURL *curl;
CURLcode curl_res;
string wallet_webservice, webservice_response, vat_type = "", parking_duration;
Json::FastWriter fw;
Json::Reader jsonReader;
Json::Value jsonResponse;
string currency,coupon_value,couponId,valid_coupon_id,validation_value, parking_rate = "", parking_rate_label = "", entry_grace_period, entry_grace_period_mins, entry_grace_period_hours, exit_grace_period_mins, exit_grace_period_hours, exit_grace_period;
int entry_grace_period_minuts, exit_grace_period_minuts;
int parkingFeeDuration = 0;
int anpr_mismatch_check = 0;
int cooperate_parker = 0;
int short_term_entry_after_contract_parking_space_exceeded = 0;
int no_entry_contract_parking_allow_exit=1;
int coupon_percentage,coupon_amount;
string coupon_array;*/

void writeLog(string function, string message) {
    General.writeLog("Services/PXCloudTicketCheck/ApplicationLog-PX-TicketCheck-" + General.currentDateTime("%Y-%m-%d"), function, message);
}

void writeException(string function, string message) {
    General.writeLog("Services/PXCloudTicketCheck/ExceptionLog-PX-TicketCheck-" + General.currentDateTime("%Y-%m-%d"), function, message);
    writeLog(function, "Exception: " + message);
}

string toString(Php::Value param) {
    string value = param;
    return value;
}
string secondsToParkingDuration(int n) {
    int day = 0, hour = 0, minutes = 0, seconds = 0;
    if (n > 0) {
        day = n / (24 * 3600);
        n = n % (24 * 3600);
        hour = n / 3600;
        n %= 3600;
        minutes = n / 60;
        n %= 60;
        seconds = n;
    }
    string duration;
    if (day > 0)
        duration = to_string(day) + "D " + to_string(hour) + "H " + to_string(minutes) + "M";
    else if (hour > 0)
        duration = to_string(hour) + "H " + to_string(minutes) + "M";
    else
        duration = to_string(minutes) + "M " + to_string(seconds) + "S";
    return duration;
}
/*
void calculateVat() {
    try {
        writeLog("calculateVat", "vat_percentage: " + to_string(vat_percentage));
        if (vat_percentage > 0) {
            if (vat_type == "inclusive") {
                grossAmount = parkingFee;
                parkingFee = grossAmount / (1 + (vat_percentage / 100));
                vatAmount = grossAmount - parkingFee;
            }
            if (vat_type == "exclusive") {
                grossAmount = parkingFee * (1 + (vat_percentage / 100));
                vatAmount = grossAmount - parkingFee;
            }
            writeLog("calculateVat", "parkingFee: " + to_string(parkingFee));
            writeLog("calculateVat", "vatAmount: " + to_string(vatAmount));
        } else
            grossAmount = parkingFee;

        writeLog("calculateVat", "grossAmount: " + to_string(grossAmount));
    } catch (const std::exception& e) {
        writeException("calculateVat", e.what());
    }

}








float calculateParkingFee() {
    stmt = serverCon->createStatement();
    float fee = 0;
    float recurringFee = 0;
    parkingFeeDuration = 0;
    try {
        writeLog("calculateParkingFee", "Entry date: " + entryDateTime + "\tParking duration: " + to_string(parkingDuration) + "\tDevice:" + to_string(deviceNumber));
        //Calculate duration
        int hours = 0, minutes = 0, seconds = 0;
        hours = parkingDuration / 3600;
        minutes = (parkingDuration % 3600) / 60;
        seconds = (parkingDuration % 3600) % 60;
        writeLog("calculateParkingFee", to_string(hours) + " H " + to_string(minutes) + " M " + to_string(seconds) + " S ");
        //Calculate parking Fee

        if (rate_type == 1) {
            if (hours > 0) {
                query = "select " + parking_rate + ",time_duration_" + parking_rate + " from  revenue_parking_rates " + "where time_unit_" + parking_rate + "='hours'  " + "and time_duration_" + parking_rate + ">" + to_string(hours) + " " + "ORDER BY  time_duration_" + parking_rate + " limit 1";
                rate = stmt->executeQuery(query);

                if (rate->next()) {
                    fee = rate->getDouble(parking_rate);
                    parkingFeeDuration = rate->getInt(2)*60;
                } else {
                    query = "select time_duration_" + parking_rate + "," + parking_rate + " from  revenue_parking_rates " + "where time_unit_" + parking_rate + "='hours'  " + "ORDER BY  time_duration_" + parking_rate + " DESC limit 1";
                    rate = stmt->executeQuery(query);
                    if (rate->next()) {
                        parkingFeeDuration = (hours + 1)*60;
                        int maxhours = rate->getInt(1);
                        fee = rate->getDouble(parking_rate);
                        writeLog("calculateParkingFee", "Max hours:" + to_string(maxhours) + " Fee:" + to_string(fee));
                        ;
                        query = "select " + parking_rate + " from revenue_fixed_parking_rates where fixed_fees='recurring_fee'";
                        rate = stmt->executeQuery(query);

                        if (rate->next()) {
                            recurringFee = rate->getDouble(1);
                            writeLog("calculateParkingFee", "Recurring fee:" + to_string(recurringFee));
                            fee = fee + (hours + 1 - maxhours) * recurringFee;
                        }
                    }
                }

            } else {//minutes only
                query = "select " + parking_rate + ",time_duration_" + parking_rate + " from  revenue_parking_rates " + "where time_unit_" + parking_rate + "='mins'  " + "and time_duration_" + parking_rate + ">" + to_string(minutes) + " " + "ORDER BY  time_duration_" + parking_rate + " limit 1";
                rate = stmt->executeQuery(query);
                if (rate->next()) {
                    fee = rate->getDouble(parking_rate);
                    parkingFeeDuration = rate->getInt(2);
                } else {
                    query = "select " + parking_rate + ",time_duration_" + parking_rate + " from  revenue_parking_rates " + "where time_unit_" + parking_rate + "='hours'  " + "ORDER BY  time_duration_" + parking_rate + " limit 1";
                    rate = stmt->executeQuery(query);
                    rate->first();
                    fee = rate->getDouble(1);
                    parkingFeeDuration = rate->getInt(2)*60;
                }
            }
        } else if (rate_type == 2) {
            query = "select " + parking_rate + " from  revenue_fixed_parking_rates where fixed_fees='fixed_parking_rate'";
            rate = stmt->executeQuery(query);
            rate->first();
            fee = rate->getDouble(1);
        }
        writeLog("calculateParkingFee", "Fee: " + to_string(fee));
        writeLog("calculateParkingFee", "parkingFeeDuration: " + to_string(parkingFeeDuration));
        if (amountPaid > 0) {
            writeLog("calculateParkingFee", "amountPaid: " + to_string(amountPaid));
            fee = fee - amountPaid;
            writeLog("calculateParkingFee", "Fee: " + to_string(fee));
        }
        delete rate;
    } catch (exception &e) {
        writeException("calculateParkingFee", e.what());
        writeException("calculateParkingFee", query);
    }
    return fee;
}

void calculateAmountToPay() {
    try {
        parkingFee = startparkingFee;
        if (percentageValidation > 0) {
            parkingFee = parkingFee - (parkingFee * percentageValidation / 100);
            writeLog("calculateAmountToPay", "Parking fee after " + to_string(percentageValidation) + " validation : " + to_string(parkingFee));
        }
        if(coupon_amount>0)
            {
            parkingFee=parkingFee-coupon_amount;
            writeLog("calculateAmountToPay", "Parking fee after " + to_string(coupon_amount) + " amount coupon : " + to_string(parkingFee));
            }
        if (coupon_percentage > 0) {
            parkingFee = parkingFee - (parkingFee * coupon_percentage / 100);
            writeLog("calculateAmountToPay", "Parking fee after " + to_string(coupon_percentage) + "% coupon : " + to_string(parkingFee));
        }
        
        calculateVat();
    } catch (const std::exception& e) {
        writeException("calculateAmountToPay", e.what());
    }

}





Php::Value openTransactionCheck(int getDetails) {
    Php::Value response;
    response["access_allowed"] = "false";
    response["open_transaction_check"] = "false";
    try {
        if (ticketId != "") {
            if (ticketId.length() == 20)
                query = "SELECT * from open_transactions where ticket_id = '" + ticketId + "' and movement_type=1 order by id desc";
            else
                query = "SELECT * from open_transactions where ticket_id like '%" + ticketId + "' and movement_type=1 order by id desc";
        } else
            query = "SELECT * from open_transactions where plate_number = '" + plateNumber + "' and movement_type=1 order by id desc";

        res = stmt->executeQuery(query);
        if (res->next()) {
            response["entry_plate_number"] = "###";            

            if (res->getString("plate_captured_id") != "")
                response = AnprObj.getEntryPlateDetails(res->getInt("plate_captured_id"));

            response["open_transaction_check"] = "true";                

            ticketId = res->getString("ticket_id");
            plateNumber = res->getString("plate_number");
            entryType = res->getInt("entry_type");
            entryDateTime = res->getString("entry_date_time");
            maxEntryGrace = res->getString("entry_grace_period");
            writeLog("openTransactionCheck", "Present in open transaction, id:" + res->getString("id"));
            writeLog("openTransactionCheck", "ticketId: " + ticketId);
            writeLog("openTransactionCheck", "plateNumber: " + plateNumber);
            writeLog("openTransactionCheck", "entryDateTime: " + entryDateTime);
            writeLog("openTransactionCheck", "maxEntryGrace: " + maxEntryGrace);

            response["ticket_id"] = ticketId;
            response["plate_number"] = plateNumber;            
            response["entry_grace_period"] = maxEntryGrace;
            response["entry_date_time"] = entryDateTime;
            response["entry_type"] = entryType;
            response["carpark_number"] = string(res->getString("carpark_number"));
            response["facility_number"] = string(res->getString("facility_number"));
            if (getDetails == 1)
                return response;
            response["access_allowed"] = "false";

            if (carparkNumber != res->getInt("carpark_number")) {
                response["result"] = "invalid_carpark";
                response["result_description"] = "Invalid carpark";
            } else if (facilityNumber != res->getInt("facility_number")) {
                response["result"] = "invalid_facility";
                response["result_description"] = "Invalid facility";
            } else {
                if (entryType == 1 && accessResult == "allow_access") {
                    if (cooperate_parker > 1 && short_term_entry_after_contract_parking_space_exceeded == 1) {
                        response["whitelist_result"] = "access_denied";
                        response["whitelist_result_description"] = "Corporate user enterd as shortterm user";
                    } else {
                        response["whitelist_result"] = "access_denied";
                        response["whitelist_result_description"] = "Antipassback.Access user entered as shortterm";
                    }
                }

                if (entryType == 2 && accessResult == "allow_access") {
                    if (res->getString("pay_upto") != "") {
                        currentDateTime = res->getString("pay_upto");
                        response["entry_type"] = 1;
                    } else {
                        response["access_allowed"] = "true";
                        return response;
                    }
                } else
                    currentDateTime = General.currentDateTime(dateTimeFormat);
                writeLog("openTransactionCheck", "paymentUpto: " + currentDateTime);
                setParkingRateConfiguration();
                if (deviceType > 2 || walletEnabled == 1 || paymentExit == 1) 
                    {                    
                    response["parking_rate"] = parking_rate;
                    response["parking_rate_label"] = parking_rate_label;
                    response["entry_grace_period"] = entry_grace_period;
                    response["exit_grace_period"] = exit_grace_period;
                    response["exit_grace_period_minutes"] = exit_grace_period_minuts;
                    response["entry_grace_period_minutes"] = entry_grace_period_minuts;
                    response["parking_fee"] = 0;
                    response["net_amount"] = 0;
                    response["vat_amount"] = 0;
                    response["gross_amount"] = 0;
                    response["parking_fee_duration_minutes"] = 0;
                    }

                stmt = reportCon->createStatement();

                int *duration = General.differenceDateTime(currentDateTime, entryDateTime, dateTimeFormat);
                parkingDuration = duration[0];
                writeLog("openTransactionCheck", "Parking duration: " + to_string(parkingDuration) + " S");
                parking_duration = secondsToParkingDuration(parkingDuration);
                writeLog("openTransactionCheck", "Parking duration: " + parking_duration);
                response["parking_duration"] = parking_duration;                
                
                timeValidation = 0;
                percentageValidation =0;                       
                if (validationEnabled == 1) {
                    getValidations();
                    validation_value = "";
                    response["validation_id"] = "";
                    response["validation_hours"] = "0";
                    response["validation_percentage"] = "0";
                    timeValidation = timeValidation + offlineTimeValidation;
                    percentageValidation = percentageValidation + offlinePercentageValidation;
                    writeLog("openTransactionCheck", "Offline Validation Hours:" + to_string(offlineTimeValidation));
                    writeLog("openTransactionCheck", "Offline Validation Percentage:" + to_string(offlinePercentageValidation));

                    writeLog("openTransactionCheck", "Total Validation Hours:" + to_string(timeValidation));
                    writeLog("openTransactionCheck", "Total Validation Percentage:" + to_string(percentageValidation));
                    if (timeValidation > 0) {
                        response["validation_id"] = timeValidationId;
                        response["validation_type"] = "Time";
                        response["validation_hours"] = to_string(timeValidation);
                        validation_value = to_string(timeValidation) + " H";
                    }
                    if (percentageValidation > 0) {
                        response["validation_percentage"] = to_string(percentageValidation);
                        response["validation_id"] = (timeValidationId!="") ? timeValidationId + "," + percentageValidationId : percentageValidationId;
                        response["validation_type"] = (timeValidation > 0) ? "Time,Percentage" : "Percentage";
                        validation_value = validation_value + to_string(percentageValidation) + " %";
                    }

                    validationSeconds = timeValidation * 3600;
                }
                
                response["entry_grace_time_remaining"] = "0";
                response["exit_grace_time_remaining"] = "0";

                int *timeDiff = General.differenceDateTime(maxEntryGrace, currentDateTime, dateTimeFormat);
                writeLog("openTransactionCheck", "entry_grace_time_remaining: " + to_string(timeDiff[0]));
                if (timeDiff[0] > 0) {
                    response["entry_grace_time_remaining"] = to_string(timeDiff[0]);
                    response["ticketcheck_result"] = "within_entry_grace_period";
                    response["result"] = "ticketcheck_access_allowed";
                    response["access_allowed"] = "true";
                    response["result_description"] = "Within entry grace period";
                    return response;
                } else {
                    maxExitGrace = res->getString("max_exit_date_time");
                    amountPaid = res->getDouble("total_amount_paid");
                    if (entryType == 1) {
                        paymentDateTime = res->getString("last_payment_date_time");
                        response["payment_date_time"] = paymentDateTime;
                    }
                    if (entryType == 2) {
                        writeLog("openTransactionCheck", "Access Entry: accessResult: " + accessResult);
                        response["result"] = accessResult;
                        response["result_description"] = accessResultDescription;
                        if (accessResult == "access_expired") {
                            entryDateTime = accessExpiry + " 23:59:59";
                            if (maxExitGrace == "")
                                maxExitGrace = accessExpiry + " 23:59:59";

                            if (amountPaid > 0) {
                                response["result"] = "access_exit_grace_period_exceeded";
                                response["result_description"] = "Access exit grace period exceeded";
                            }
                        } else if (accessResult == "allow_access") {
                            if (maxExitGrace != "")
                                maxExitGrace = accessExpiry + " 23:59:59";

                        } else
                            return response;
                    }
                    if (entryType == 3) {
                        writeLog("openTransactionCheck", "Reservation Entry: reservationResult: " + reservationResult);
                        if (reservationResult == "reservation_exit_grace_period_exceeded") {
                            entryDateTime = reservationStart;
                            if (amountPaid == 0) {
                                maxExitGrace = reservationExpiry;
                                amountPaid = reservationFee;
                            } else
                                amountPaid = amountPaid + reservationFee;
                        }
                        else
                            return response;

                    }
                    if (maxExitGrace != "") {
                        writeLog("openTransactionCheck", "maxExitGrace: " + maxExitGrace);
                        response["max_exit_date_time"] = maxExitGrace;
                        response["total_amount_paid"] = amountPaid;
                        writeLog("openTransactionCheck", "amountPaid: " + to_string(amountPaid));
                        int *timeDiff = General.differenceDateTime(maxExitGrace, currentDateTime, dateTimeFormat);
                        writeLog("openTransactionCheck", "exit_grace_time_remaining: " + to_string(timeDiff[0]));
                        if (timeDiff[0] > 0) {
                            response["exit_grace_time_remaining"] = to_string(timeDiff[0]);
                            response["ticketcheck_result"] = "within_exit_grace_period";
                            response["result"] = "ticketcheck_access_allowed";
                            response["access_allowed"] = "true";
                            response["result_description"] = "Within exit grace period";
                            return response;
                        } else {
                            if (entryType == 1) {
                                response["result_description"] = "Exit grace period exceeded";
                                response["result"] = "exit_grace_period_exceeded";
                            }
                            if (entryType == 2 && deviceType == 2) {
                                response["result_description"] = "Access expired";
                                response["result"] = "access_expired";
                                return response;
                            }
                        }
                    } else {
                        if (accessResult == "allow_access") {
                            if (cooperate_parker > 1 && short_term_entry_after_contract_parking_space_exceeded == 1)
                                response["result_description"] = "Unpaid.Corporate user enterd as shortterm user";
                            else
                                response["result_description"] = "Unpaid.Antipassback.Access user entered as shortterm";
                        } else
                            response["result_description"] = "Unpaid";
                        response["result"] = "unpaid";
                    }

                    startparkingFee = calculateParkingFee();
                    response["parking_fee"] = startparkingFee;
                    response["parking_fee_duration_minutes"] = parkingFeeDuration;
                    exitGrace = parkingFeeDuration * 60 - parkingDuration;


                    if (validationSeconds > 0) {
                        parkingDuration = parkingDuration - validationSeconds;
                        writeLog("openTransactionCheck", "Parking duration after validation: " + to_string(parkingDuration) + " S");
                        writeLog("openTransactionCheck", "Parking duration after validation: " + to_string(parkingDuration / 3600) + " H " + to_string((parkingDuration % 3600) / 60) + " M");
                    }
                    if ((validationSeconds > 0 && parkingDuration <= 0) || percentageValidation >= 100) {
                        response["ticketcheck_result"] = "within_validation_hours";
                        response["result"] = "ticketcheck_access_allowed";
                        response["access_allowed"] = "true";
                        response["result_description"] = "Validation Applied";
                        return response;
                    } else {
                        writeLog("openTransactionCheck", "calculate parking fee after validation hours");
                        startparkingFee = calculateParkingFee();
                    }
                    
                    coupon_percentage=0;
                    coupon_amount=0;
                    if(couponEnabled==1)
                        {
                        getCouponDiscounts();
                        response["coupon_percentage"]=coupon_percentage;
                        response["coupon_amount"]=coupon_amount;
                        response["valid_coupons"]=valid_coupon_id;
                        response["coupon_value"]=coupon_value;
                        }
                    
                    if(coupon_percentage>=100 ||(coupon_amount>0 && startparkingFee<=coupon_amount))
                        {
                        response["ticketcheck_result"] = "valid_coupon_applied";
                        response["result"] = "ticketcheck_access_allowed";
                        response["access_allowed"] = "true";
                        response["result_description"] = "coupon Applied";
                        return response;
                        }                   

                    if (deviceType >= 2 || walletEnabled == 1 || paymentExit == 1) {
                        calculateAmountToPay();
                        response["net_amount"] = parkingFee;
                        response["vat_amount"] = vatAmount;
                        response["gross_amount"] = grossAmount;
                        
                        if (grossAmount <= 0) {
                        response["exit_grace_time_remaining"] = to_string(exitGrace);
                        response["ticketcheck_result"] = "within_exit_grace_period";
                        response["result"] = "ticketcheck_access_allowed";
                        response["access_allowed"] = "true";
                        response["result_description"] = "Within exit grace period.Amount to pay =0";
                        return response;
                        }
                    }
                   
                    if (toString(response["access_allowed"]) == "false" && walletEnabled == 1) {
                        if (grossAmount > 0) {
                            Php::Value wallet = walletCheck();
                            if (deviceType == 2) {
                                response["access_allowed"] = toString(wallet["access_allowed"]);
                                if (toString(response["access_allowed"]) == "true") {
                                    response["result"] = "ticketcheck_access_allowed";
                                    string amount_in_points = wallet["amount_in_points"];
                                    response["gross_amount_in_points"] = amount_in_points;
                                    response["wallet_balance"] = toString(wallet["wallet_balance"]);
                                    response["result_description"] = toString(wallet["result_description"]);
                                    stmt = reportCon->createStatement();
                                    string validation_id = response["validation_id"];
                                    string category;
                                    if (entryType == 1)
                                        category = "StandardParkingFee";
                                    if (entryType == 3)
                                        category = "Reservation";
                                    query = "INSERT into revenue_payments(device_number,device_name,carpark_number,carpark_name,facility_number,parking_rate_label,parking_rate_name,entry_grace_period,exit_grace_period,vat_type,vat_percentage,ticket_id,entry_date_time,payment_date_time,max_exit_date_time,parking_duration,payment_category,parking_fee,vat_amount,gross_amount,amount_received,payment_type,validation_value,validation_id,entry_plate_number,exit_plate_number,wallet_points)VALUES('" + to_string(deviceNumber) + "','" + deviceName + "'," + to_string(carparkNumber) + ",'" + carpakName + "','" + to_string(facilityNumber) + "','" + parking_rate_label + "','" + parking_rate + "','" + entry_grace_period + "','" + exit_grace_period + "','" + vat_type + "','" + to_string(vat_percentage) + "','" + ticketId + "','" + entryDateTime + "',CURRENT_TIMESTAMP ,(now() + INTERVAL " + exit_grace_period_mins + " MINUTE),'" + parking_duration + "','" + category + "','" + to_string(parkingFee) + "','" + to_string(vatAmount) + "','" + to_string(grossAmount) + "','" + to_string(grossAmount) + "','E WALLET','" + validation_value + "','" + validation_id + "','" + plateNumber + "','" + plateNumber + "'," + amount_in_points + ")";
                                    stmt->executeUpdate(query);
                                } else {
                                    response["wallet_result"] = toString(wallet["result"]);
                                    response["wallet_result_description"] = toString(wallet["result_description"]);
                                    response["wallet_balance"] = toString(wallet["wallet_balance"]);
                                    response["wallet_expiry_date"] = toString(wallet["wallet_expiry_date"]);
                                }
                            } else {
                                if (toString(wallet["wallet_present"]) == "true") {
                                    response["user_id"] = toString(wallet["user_id"]);
                                    response["user_name"] = toString(wallet["user_name"]);
                                    response["wallet_expiry_date"] = toString(wallet["wallet_expiry_date"]);
                                    response["wallet_present"] = "true";
                                    response["wallet_usable"] = toString(wallet["wallet_usable"]);
                                    response["gross_amount_in_points"] = stod(toString(wallet["gross_amount_in_points"]));
                                    response["wallet_balance"] = stod(toString(wallet["wallet_balance"]));
                                    response["conversion_rate"] = stod(toString(wallet["conversion_rate"]));
                                } else {
                                    response["wallet_present"] = "false";
                                    response["wallet_usable"] = "false";
                                }
                                response["wallet_result_description"] = toString(wallet["result_description"]);
                                response["wallet_result"] = toString(wallet["result"]);
                            }
                        } else {
                            response["result_description"] = "Amount to pay=0";
                        }
                    }
                }
            }
        } else {
            if(accessResult!="" && accessResult!="not_in_access_whitelist")
                {
                if(accessResult=="allow_access" && no_entry_contract_parking_allow_exit==1)
                    response["access_allowed"]="true"; 
                else
                    {
                    response["result"]=accessResult;
                    response["result_description"]=accessResultDescription;
                    }
                }
            else
                {
                response["result"] = "already_exited";
                response["result_description"] = "Already exited";
                }                                                                                                     
        }
       
    }    catch (const std::exception& e) {
        writeException("openTransactionCheck", e.what());
        writeException("openTransactionCheck", query);
    }
    return response;
}



Php::Value exitTicketCheck() {
    Php::Value response;
    try {
        response["access_allowed"] = "false";
        response["coupon_ticket"]="false";
        reportCon = General.mysqlConnect(ReportingDB);
        if (reportCon != NULL) {
            stmt = reportCon->createStatement();
                    
            //blacklist
            if (ticketId.length() == 20)
                query = "SELECT * from parking_blacklist where ticket_id = '" + ticketId + "'";
            else
                query = "SELECT * from parking_blacklist where ticket_id like '%" + ticketId + "'";
            res = stmt->executeQuery(query);
            if (res->next()) 
                {
                response["result"] = "blacklisted";
                response["result_description"] = string(res->getString("blacklisting_description"));
                writeLog("exitTicketCheck", "Blacklisted");
                } 
            else 
                {
                writeLog("exitTicketCheck", "Not blacklisted");
                serverCon = General.mysqlConnect(ServerDB);
                response = openTransactionCheck(0);
                delete serverCon;
                }
                
            delete res;
            delete stmt;
            delete reportCon;
        }
    }    catch (const std::exception& e) {
        writeException("exitTicketCheck", e.what());
    }
    return response;
}

Php::Value exitPlateCheck() {
    Php::Value response;
    try {
        response["access_allowed"] = "false";
        reportCon = General.mysqlConnect(ReportingDB);
        if (reportCon != NULL) {
            stmt = reportCon->createStatement();
            serverCon = General.mysqlConnect(ServerDB);
            response = openTransactionCheck(0);
            delete serverCon;
        }
        delete res;
        delete stmt;
        delete reportCon;
    } catch (const std::exception& e) {
        writeException("exitPlateCheck", e.what());
    }
    return response;
}

void insertIntoPlateMismatch(string entry_plate_number,string exit_plate_number,string id1,string id2,string result,string description,string dismiss) {
    try {
        reportCon = General.mysqlConnect(ReportingDB);
        if (reportCon != NULL) {
            stmt = reportCon->createStatement();
            query="insert into plates_mismatch(date_time,device_number,device_name,entry_plate_number,exit_plate_number,ticket_id,entry_plate_captured_id,exit_plate_captured_id,result,result_description,dismiss)values(CURRENT_TIMESTAMP," + to_string(deviceNumber) + ",'" + deviceName + "','" +entry_plate_number + "','" + exit_plate_number + "','" + ticketId + "',"+id1+","+id2+",'"+result+"','"+description+"','"+dismiss+"')";
            stmt->executeUpdate(query);
            delete stmt;
            delete reportCon;
        }
    } catch (const std::exception& e) {
        writeException("insertIntoPlateMismatch", e.what());
        //writeException("insertIntoPlateMismatch",query);
    }
}


void insertIntoTicketCheck(string result, string description) {
    try {
        reportCon = General.mysqlConnect(CloudDB);
        if (reportCon != NULL) {
            stmt = reportCon->createStatement();
            stmt->executeUpdate("insert into ticket_check(date_time,device_number,device_name,carpark_number,carpark_name,device_type,ticket_id,plate_number,result,facility_number,result_description)values(CURRENT_TIMESTAMP," + to_string(deviceNumber) + ",'" + deviceName + "'," + to_string(carparkNumber) + ",'" + carpakName + "'," + to_string(deviceType) + ",'" + ticketId + "','" + plateNumber + "','" + result + "','" + to_string(facilityNumber) + "','" + description + "')");
            delete stmt;
            delete reportCon;
        }
    } catch (const std::exception& e) {
        writeException("insertIntoTicketCheck", e.what());
    }
}



void getDeviceDetails() {
    try {
        serverCon = General.mysqlConnect(ServerDB);
        if (serverCon != NULL) {
            stmt = serverCon->createStatement();
            res = stmt->executeQuery("select * from system_devices where device_number=" + to_string(deviceNumber));
            if (res->next()) {
                deviceType = res->getInt("device_category");
                writeLog("deviceType", to_string(deviceType));
                carparkNumber = res->getInt("carpark_number");
                writeLog("carpark", to_string(carparkNumber));
                parkingZone = res->getString("parking_zone");
                writeLog("parkingZone", parkingZone);
                reservationEnabled = res->getInt("reservation_enabled");
                writeLog("reservationEnabled", to_string(reservationEnabled));
                accessEnabled = res->getInt("access_enabled");
                writeLog("accessEnabled", to_string(accessEnabled));
                
                deviceName = res->getString("device_name");
                writeLog("deviceName", deviceName);
                carpakName = res->getString("carpark_name");
                writeLog("carpakName", carpakName);
                cameraId = res->getInt("camera_id");
                writeLog("cameraId", to_string(cameraId));
                paymentExit = res->getInt("payment_enabled_exit");
                writeLog("paymentExit", to_string(paymentExit));
                anpr_mismatch_check = res->getInt("anpr_mismatch_check");
                writeLog("anpr_mismatch_check", to_string(anpr_mismatch_check));
                couponEnabled=res->getInt("coupon_enabled");
                writeLog("couponEnabled", to_string(couponEnabled));
               
            }
            delete res;            
            query = "select setting_name,setting_value from system_settings";        
            settings = stmt->executeQuery(query);
            while (settings->next()) {                
                if (settings->getString("setting_name") == "cloud_wallet_webservice")
                    wallet_webservice = settings->getString("setting_value");
                if (settings->getString("setting_name") == "validation_enabled")
                    validationEnabled = settings->getInt("setting_value");
                if (settings->getString("setting_name") == "wallet_enabled")
                    walletEnabled = settings->getInt("setting_value");
                if (settings->getString("setting_name") == "vat_type")
                    vat_type = settings->getString("setting_value");
                if (settings->getString("setting_name") == "vat_percentage")
                    vat_percentage = settings->getDouble("setting_value");                
                if (settings->getString("setting_name") == "facility_number")
                    facilityNumber = settings->getInt("setting_value");
                if (settings->getString("setting_name") == "facility_name")
                    facilityName = settings->getString("setting_value");                   
                if (settings->getString("setting_name") == "no_entry_contract_parking_allow_exit")
                    no_entry_contract_parking_allow_exit = settings->getInt("setting_value"); 
                if (settings->getString("setting_name") == "currency")
                    currency = settings->getString("setting_value");                                 
            }
        delete settings;

        writeLog( "walletEnabled", to_string(walletEnabled));
        writeLog( "wallet_webservice", wallet_webservice);
        writeLog( "validationEnabled", to_string(validationEnabled));
        writeLog( "vat_type", vat_type);
        writeLog( "vat_percentage", to_string(vat_percentage));        
        writeLog("facility",to_string(facilityNumber));
        writeLog("facilityName",facilityName);      
        writeLog("currency",currency);      
        writeLog("no_entry_contract_parking_allow_exit",to_string(no_entry_contract_parking_allow_exit));
        delete stmt;
        delete serverCon;
        }
    } catch (const std::exception& e) {
        writeException("getDeviceDetails", e.what());
    }
}*/

string convertToString(int number,int length,int hex)
    {
    stringstream ss;
    if(hex==1)        
		ss << setw(length) << setfill('0') <<std::hex << number;		
    else
        ss << setw(length) << setfill('0') << number;
    return ss.str();
    }



int getCarparkID(int carpark_number,int facility_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int carpark_id = 0;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select carpark_id,carpark_name from system_carparks where carpark_number="+to_string(carpark_number)+" and facility_number="+to_string(facility_number));
        if(res->next())
        {
            carpark_id = res->getInt("carpark_id");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("getCarparkID", e.what());
    }
    return carpark_id;
}

int getDeviceCategory(int device_number,int carpark_id)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int device_category = 0;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select device_category from system_devices where device_number="+to_string(device_number)+" and carpark_id="+to_string(carpark_id));
        if(res->next())
        {
            device_category = res->getInt("device_category");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("getDeviceCategory", e.what());
    }
    return device_category;
}

void setReservationRateConfiguration(int carpark_id) {
    try 
    {
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res;
        conn = General.mysqlConnect(CloudDB);
        
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            res = stmt->executeQuery("SELECT rate_plan_id FROM parking_rate_reservation where special_rate_date = date(NOW()) and carpark_id = "+to_string(carpark_id)+" and special_rate_status=1");
            if(res->next())

            {
                rate_plan_id = res->getInt("rate_plan_id");
            }
            else
            {
                res = stmt->executeQuery("Select rate_plan_id from parking_rate_reservation where day_of_the_week = DayName(NOW()) and carpark_id = "+to_string(carpark_id));
                if(res->next())
                {
                    rate_plan_id = res->getInt("rate_plan_id");
                }

            }
            
            res = stmt->executeQuery("SELECT reservation_entry_grace_period_hours,reservation_entry_grace_period_mins,reservation_exit_grace_period_hours,reservation_exit_grace_period_mins FROM revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id));
            writeLog("setReservationRateConfiguration","SELECT reservation_entry_grace_period_hours,reservation_entry_grace_period_mins,reservation_exit_grace_period_hours,reservation_exit_grace_period_mins FROM revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id));
            if(res->next())
            {
                entry_grace_period_reservation = res->getInt("reservation_entry_grace_period_hours")*60 + res->getInt("reservation_entry_grace_period_mins");
                exit_grace_period_reservation = res->getInt("reservation_exit_grace_period_hours")*60 + res->getInt("reservation_exit_grace_period_mins");
            }
            
            writeLog("setReservationRateConfiguration", "Entry grace period=" + to_string(entry_grace_period_reservation));
            writeLog("setReservationRateConfiguration", "Exit grace period=" + to_string(exit_grace_period_reservation));
        }
    }
    catch(const std::exception& e)
    {
        writeException("setReservationRateConfiguration",e.what());
    }

}

void setParkingRateConfiguration(int carpark_id) {
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int rate_plan_id = 0,entry_grace_period_minutes,exit_grace_period_minutes;
    string parking_rate = "", parking_rate_label = "",query;
    try {
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select rate_plan_id from system_carparks where carpark_id = "+to_string(carpark_id));
        if(res->next())
        {
            rate_plan_id = res->getInt("rate_plan_id");
        }
        
        
       
        query = "select entry_grace_period_mins,entry_grace_period_hours,exit_grace_period_hours,exit_grace_period_mins from  revenue_fixed_parking_rates where parking_rate_id = "+to_string(rate_plan_id);
        res = stmt->executeQuery(query);
        if (res->next()) {
            

            entry_grace_period_minutes = res->getInt("entry_grace_period_hours")*60 + res->getInt("entry_grace_period_mins");
            exit_grace_period_minutes = res->getInt("exit_grace_period_hours")*60 + res->getInt("exit_grace_period_mins");
        }
        writeLog("setParkingRateConfiguration", "Entry grace period=" + to_string(entry_grace_period_minutes));
        writeLog("setParkingRateConfiguration", "Exit grace period=" + to_string(exit_grace_period_minutes));
        delete res;
    } catch (const std::exception& e) {
        writeException("setParkingRateConfiguration", e.what());
    }

}
string GetCarparkName(int carpark_id)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string carpark_name;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select carpark_name from system_carparks where carpark_id="+to_string(carpark_id));
        if(res->next())
        {
            carpark_name = res->getString("carpark_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("GetCarparkName", e.what());
    }
    return carpark_name;
}

string GetFacilityName(int facility_number)
{
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    string facility_name;
    try{
        conn = General.mysqlConnect(CloudDB);
        stmt = conn->createStatement();
        res = stmt->executeQuery("Select facility_name from system_facility where facility_number="+to_string(facility_number));
        if(res->next())
        {
            facility_name = res->getString("facility_name");
        }
        delete res;
        delete stmt;
        delete conn;
    }
    catch(exception &e) {
        writeException("GetFacilityName", e.what());
    }
    return facility_name;
}

void insertIntoOpenTransaction(string ticketId,int carpark,int facility,int device,int movement_type,int carpark_id) {
    sql::Connection *conn;
    sql::Statement *stmt;
    int entryType;
    string query;
    try {                        
        /*if(result=="allow_access")
            entryType=2;
        else if(result=="reservation_allow_access")
            entryType=3;
        else*/
            entryType=1;
            
        conn = General.mysqlConnect(CloudDB);
        
        if (conn != NULL) {
            string carpark_name = GetCarparkName(carpark_id);
            string facility_name = GetFacilityName(facility);
            stmt = conn->createStatement();                                    
            query = "INSERT into open_transactions(device_number,ticket_id,carpark_id,carpark_number,facility_number,entry_date_time,operation_mode,entry_grace_period,entry_type,movement_type,carpark_name,facility_name) VALUES(" + to_string(device) + ",'" + ticketId + "'," + to_string(carpark_id) + "," + to_string(carpark) + "," + to_string(facility) + ",CURRENT_TIMESTAMP,'', DATE_ADD(CURRENT_TIMESTAMP,INTERVAL " + to_string(entry_grace_period_minutes) + " MINUTE) ,"+to_string(entryType)+",1,'"+carpark_name+"','"+facility_name+"')";
            
            stmt->executeUpdate(query);
            delete stmt;
            delete conn;
        }
    } catch (const std::exception& e) {
        writeException("insertIntoOpenTransaction", e.what());
    }
}

float calculateParkingFee(int duration)
{
    float parking_fee=0;
    sql::Connection *conn;
    sql::Statement *stmt;
    sql::ResultSet *res;
    int hours = 0, minutes = 0, seconds = 0;
    hours = duration / 3600;
    minutes = (duration % 3600) / 60;
    seconds = (duration % 3600) % 60;
    writeLog("calculateParkingFee", to_string(hours) + "H " + to_string(minutes) + "M " + to_string(seconds) + "S ");
    try 
    {
        conn = General.mysqlConnect(CloudDB);
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            if(hours>0){
                res = stmt->executeQuery("SELECT parking_rate,time_duration from  revenue_parking_rates where Lower(time_unit)='hours' and time_duration>"+to_string(hours)+" and parking_rate_id = "+to_string(rate_plan_id)+" and status=1 order by time_duration limit 1");
                if(res->next())
                {
                    parking_fee = res->getDouble("parking_rate");
                    parking_fee_duration = res->getInt("time_duration")*60;
                }
                else
                {
                    //Take max parking rate
                }
            }
            else
            {
                res = stmt->executeQuery("SELECT parking_rate,time_duration from  revenue_parking_rates where Lower(time_unit)='minutes' and time_duration>"+to_string(minutes)+" and parking_rate_id = "+to_string(rate_plan_id)+" and status=1 order by time_duration limit 1");
                if(res->next())
                {
                    parking_fee = res->getDouble("parking_rate");
                    parking_fee_duration = res->getInt("time_duration");
                }
                else
                {
                    //Take first hour parking rate?
                }
            }
        }
        
    }
    catch(const std::exception& e)
    {
        writeException("calculateParkingFee",e.what());
    }
    return parking_fee;
}



Php::Value checkAppUser(string ticket_id,int carpark,int facility,int device,int movement_type,int carpark_id)
{
    writeLog("checkAppUser","In checkAppUser:movement type = "+to_string(movement_type)); 
    Php::Value response;
    string entryDateTime;
    try 
    {
        sql::Connection *conn;
        sql::Statement *stmt;
        sql::ResultSet *res,*res1;
        conn = General.mysqlConnect(CloudDB);
        if(conn!=NULL)
        {
            stmt = conn->createStatement();
            res = stmt->executeQuery("SELECT wallet_balance,a.user_id from  parking_ewallet as a join application_users as b on a.user_id = b.user_id where b.user_qrcode = '"+ticket_id+"' and a.wallet_enabled = 1");
            if(res->next())
            {
                setParkingRateConfiguration(carpark_id);
                if(movement_type==1)
                {
                    if(res->getDouble("wallet_balance")>=0)
                    {
                        //allow entry and add  in the open transaction table
                        response["access_allowed"]="true";
                        response["result"] = "shortterm_entry";
                        response["result_description"] = "Shortterm Entry";   
                        insertIntoOpenTransaction(ticket_id,carpark,facility,device,movement_type,carpark_id);                        
                    }
                    else
                    {
                        response["access_allowed"]="false";
                        response["result_description"]="Access disabled.Wallet balance is negative";    
                        response["result"]="access_disabled";
                    }
                }
                else
                {
                    res1 = stmt->executeQuery("Select * from open_transactions where ticket_id = '"+ticket_id+"' and movement_type = 1 order by id desc");
                    if(res1->next())
                    {
                        int id = res1->getInt("id");
                        entryDateTime = res1->getString("entry_date_time");
                        int *duration = General.differenceDateTime(General.currentDateTime(dateTimeFormat), entryDateTime, dateTimeFormat);
                        parking_duration = duration[0];
                        response["parking_duration"] = secondsToParkingDuration(parking_duration);
                        writeLog("checkAppUser","parking duration in seconds:"+to_string(parking_duration)); 
                        parking_fee = calculateParkingFee(parking_duration);
                        response["parking_fee"] = parking_fee;
                        response["parking_fee_duration"] = parking_fee_duration;
                        int conversion_factor = 1; //get it from db based on country
                        double parking_wallet_points = parking_fee*conversion_factor;
                        response["parking_wallet_points"] = parking_wallet_points;
                        double current_balance = res->getDouble("wallet_balance") - parking_wallet_points;
                        string user_id = res->getString("user_id");
                        int n = stmt->executeUpdate("Update parking_ewallet set wallet_balance = "+to_string(current_balance)+",last_deducted_amount = "+to_string(parking_wallet_points)+",last_payment_date_time=NOW(),download_flag=0 where user_id = "+user_id);
                        if(n>0)
                        {
                            stmt->executeUpdate("Insert into ewallet_transactions(date_time,transaction_type,transaction_description,user_id,ticket_id,amount,wallet_point,wallet_conversion_factor,previous_wallet_balance,current_wallet_balance,device_number,carpark_number,facility_number)Values(NOW(),100,'Parking Fee','"+user_id+"','"+ticket_id+"',"+to_string(parking_fee)+","+to_string(parking_wallet_points)+","+to_string(conversion_factor)+","+to_string(res->getDouble("wallet_balance"))+","+to_string(current_balance)+","+to_string(device)+","+to_string(carpark)+","+to_string(facility)+")");
                            stmt->executeUpdate("Update open_transactions set movement_type = 2 where id = "+to_string(id));
                            response["access_allowed"]="true";
                            
                        }
                        
                    }
                    else
                    {
                        response["access_allowed"]="false";
                        response["result_description"]="Not in Open Transaction";    
                        response["result"]="access_disabled";
                    }
                    delete res1;
                }
            }
            else
            {
                response["access_allowed"]="false";
                response["result_description"]="Access disabled.No ewallet record found";    
                response["result"]="access_disabled";
            }
            delete res;
            delete stmt;
        }
        delete conn;
    }
    catch(const std::exception& e)
    {
        writeException("checkAppUser",e.what());
    }
    return response;
}

Php::Value parcxTicketCheck(Php::Parameters &params) {
int device_number,carpark_number,facility_number,movement_type;
string qr_code,parking_zone;
Php::Value response;
response["customer_message_line1"]="";
response["customer_message_line2"]="";
response["customer_message_line3"]="";
response["customer_message_line4"]="";
try {
    Php::Value json = params[0];
    writeLog("parcxTicketCheck","--------------START-----------------");
    device_number = json["device_number"];
    carpark_number = json["carpark_number"];
    facility_number = json["facility_number"];
    //movement_type = json["movement_type"];
    parking_zone = toString(json["parking_zone"]);
    qr_code = toString(json["qr_code"]);
    int carpark_id = getCarparkID(carpark_number,facility_number);
    int device_category = getDeviceCategory(device_number,carpark_id);
    
    if(device_category==1)
    {
        movement_type = 1;
    }
    else if(device_category==2 || device_category==3)
    {
        movement_type = 2;
    }
    //Check Access Whitelist
    Php::Value whitelist_details = AccessObj.checkAccess(qr_code, parking_zone,carpark_number,facility_number,movement_type);
    
    if (toString(whitelist_details["access_allowed"]) == "true")
    {
        for (auto &iter : whitelist_details)
        response[iter.first] = iter.second;
        //insertIntoTicketCheck(result, result_description);
    }
    else //Check Reservation
    {
        setReservationRateConfiguration(carpark_id);
        Php::Value reservation_details = ReservationObj.checkReservation(qr_code,entry_grace_period_reservation,exit_grace_period_reservation,carpark_number,facility_number,movement_type);
        if (toString(reservation_details["access_allowed"]) == "true")
        {
            for (auto &iter : reservation_details)
            response[iter.first] = iter.second;
           // insertIntoTicketCheck(result, result_description);
        }
        else
        {
            Php::Value app_user_details = checkAppUser(qr_code,carpark_number,facility_number,device_number,movement_type,carpark_id); 
            for (auto &iter : app_user_details)
            response[iter.first] = iter.second;
        }
    }
    //writeLog("parcxTicketCheck", "Result: " + result + "\tDescription:" + result_description + "\n");
    writeLog("parcxTicketCheck","--------------END-----------------"); 
    
} catch (exception &e) {
        writeException("parcxTicketCheck", e.what());
    }
    return response;

}



/*Php::Value parcxTicketCheck(Php::Parameters &params) {
    Php::Value response;
    Php::Value anprSettings;    
    try {
        Php::Value json = params[0];
        
        test=0;
        writeLog("parcxTicketCheck","--------------START-----------------");
        test=json["test"];
        writeLog("test", to_string(test));

        deviceNumber = json["device_number"];                
        writeLog("deviceNumber", to_string(deviceNumber));
        
        plateCapturedId = json["plate_captured_id"];
        writeLog("plateCapturedId", to_string(plateCapturedId));

        if (plateCapturedId > 0 || json["anpr_check_only"] == 1 || json["anpr_enable"] == 1)
            anprEnabled = 1;
        else
            anprEnabled = 0;
        writeLog("anprEnabled", to_string(anprEnabled));

        deductMoneyFromWallet = json["deduct_money_from_wallet"];
        if (json["qrcode_access_enabled"] == 1 || json["anpr_access_enabled"] == 1 || json["access_enabled"] == 1)
            accessEnabled = 1;
        writeLog("accessEnabled", to_string(accessEnabled));
        
        getDeviceDetails();        

        if (deductMoneyFromWallet == 1) {
            grossAmount = json["amount_to_pay"];
            userId = toString(json["user_id"]);
            response = updateWalletPayment();
        } else {
            
            plateCapturedInterval = json["plate_captured_interval"];
            writeLog("plateCapturedInterval", to_string(plateCapturedInterval));
            anprSettings = "";
            ticketId = toString(json["ticket_id"]);
            writeLog("ticketId", ticketId);
            plateNumber = toString(json["plate_number"]);
            writeLog("plateNumber", plateNumber);            
            plateType = "";
            plateCountry = "";
            plateArea = "";

            if (anprEnabled == 1) {                  
                Php::Value plate_details = AnprObj.getPlateDetails(cameraId, plateCapturedInterval, plateCapturedId);
                response = plate_details;
                if (toString(plate_details["result"]) == "plate_available") {
                    plateNumber = toString(plate_details["plate_number"]);
                    plateArea = toString(plate_details["plate_area"]);                    
                    plateCountry = toString(plate_details["plate_country"]);
                    plateType = toString(plate_details["plate_type"]);
                    plateCapturedId=plate_details["plate_captured_id"];
                } 
            } 

            response["access_allowed"] = "false";
            response["ticket_id"] = ticketId;
            response["plate_number"] = plateNumber;
            response["open_transaction_check"] = "false";
            response["whitelist_present"]="false";
            accessExpiry="";
            accessResult="";     
            if(couponEnabled==1 && deviceType!=1)
                {
                coupon_array=toString(json["coupon_id"]);   
                writeLog("coupon_array", coupon_array);
                
                current_coupon_id="";                
                Php::Value coupon =couponCheck();
                for (auto &iter : coupon)
                    response[iter.first] = iter.second;    
                string coupon_ticket=response["coupon_ticket"];                
                if(coupon_ticket=="true")
                    {
                    string result=response["result"];
                    if(result=="valid_coupon")
                        {                        
                        if(toString(json["coupon_ticket_id"])!="")
                            {
                            current_coupon_id=ticketId;                                
                            ticketId=toString(json["coupon_ticket_id"]);
                            writeLog("current_coupon_id",current_coupon_id);
                            writeLog("ticketId",ticketId);
                            }
                        else 
                            return response;
                        }
                    else
                        return response;
                    }
                                
                }
            if (accessEnabled == 1) {
                cooperate_parker = 0;
                short_term_entry_after_contract_parking_space_exceeded = 0;
                writeLog("qrcodeAccessEnabled", "Access check");
                Php::Value whitelist_details = AccessObj.checkAccess(ticketId, parkingZone, carparkNumber, facilityNumber, deviceType, plateNumber, plateType, plateArea, plateCountry, anprSettings);
                for (auto &iter : whitelist_details)
                    response[iter.first] = iter.second;
                accessExpiry = toString(whitelist_details["expiry_date"]);
                accessResult = toString(whitelist_details["result"]);
                accessResultDescription = toString(whitelist_details["result_description"]);

                response["whitelist_result"] = accessResult;
                response["whitelist_result_description"] = accessResultDescription;

                cooperate_parker = response["cooperate_parker"];
                short_term_entry_after_contract_parking_space_exceeded = response["short_term_entry_after_contract_parking_space_exceeded"];
                //if(toString(response["access_allowed"])=="true" && cooperate_parker>1 && short_term_entry_after_contract_parking_space_exceeded==1 && deviceType!=1)

                if (toString(response["access_allowed"]) == "true" && deviceType != 1 ) {                                        
                    Php::Value ticket_details = exitPlateCheck();
                    for (auto &iter : ticket_details)
                        response[iter.first] = iter.second;

                    string result = response["result"];
                    string result_description = response["result_description"];
                    insertIntoTicketCheck(result, result_description);
                    writeLog("parcxTicketCheck", "Result: " + result + "\tDescription:" + result_description + "\n");
                    writeLog("parcxTicketCheck","--------------END-----------------"); 
                    return response;
                }

            }


            if (toString(response["access_allowed"]) == "false" && reservationEnabled == 1) {                
                entryType=3;
                serverCon = General.mysqlConnect(ServerDB);
                setParkingRateConfiguration();
                delete serverCon;
                writeLog("reservationEnabled", "Reservation check");
               
                Php::Value reservation_details = ReservationObj.checkReservation(ticketId, entry_grace_period_minuts, exit_grace_period_minuts, carparkNumber, facilityNumber, deviceType, plateNumber, plateType, plateArea, plateCountry, anprSettings);
                for (auto &iter : reservation_details)
                    response[iter.first] = iter.second;

                reservationExpiry = toString(reservation_details["max_exit_grace_period"]);
                reservationFee = reservation_details["reservation_fee"];
                reservationResult = toString(reservation_details["result"]);
                reservationResultDescription = toString(reservation_details["result_description"]);
                reservationStart = toString(reservation_details["parking_from"]);

                response["reservation_result"] = reservationResult;
                response["reservation_result_description"] = reservationResultDescription;
            }

            if (toString(response["access_allowed"]) == "false" && deviceType == 1) {
                response["result"] = "shortterm_entry";
                response["result_description"] = "Shortterm Entry";
            }
            if (toString(response["access_allowed"]) == "true" && deviceType == 3) {
                reportCon = General.mysqlConnect(ReportingDB);
                if (reportCon != NULL) {
                    stmt = reportCon->createStatement();
                    Php::Value entry_details = openTransactionCheck(1);
                    for (auto &iter : entry_details)
                        response[iter.first] = iter.second;
                }
                delete res;
                delete stmt;
                delete reportCon;
            }

            if (toString(response["access_allowed"]) == "false" && deviceType != 1) {
                
                offlinePercentageValidation = json["offline_validation_percentage"];
                offlineTimeValidation = json["offline_validation_hours"];

                writeLog("offlinePercentageValidation", to_string(offlinePercentageValidation));
                writeLog("offlineTimeValidation", to_string(offlineTimeValidation));

                if (ticketId != "")//ticket check
                {
                    writeLog("exitTicketCheck", "Exit ticket check: " + ticketId);
                    Php::Value ticket_details = exitTicketCheck();
                    for (auto &iter : ticket_details)
                        response[iter.first] = iter.second;
                } else if (plateNumber != "") //plate check
                {
                    writeLog("exitPlateCheck", "Exit plate check: " + plateNumber);
                    Php::Value ticket_details = exitPlateCheck();
                    for (auto &iter : ticket_details)
                        response[iter.first] = iter.second;
                }

                if (toString(response["result"]) == "ticketcheck_access_allowed" && deviceType > 2)
                    response["result"] = toString(response["ticketcheck_result"]);                
            } else
                writeLog("parcxTicketCheck", "No exit ticket check");
        }

        string result = response["result"];
        string result_description = response["result_description"];
        if(test==0)
            insertIntoTicketCheck(result, result_description);
        else
            {            
            if(deviceType==1)
                {
                int create_entry=json["add_test_entry"];                
                if(create_entry==1)
                    {
                    ticketId=toString(response["ticket_id"]);
                    plateNumber = toString(response["plate_number"]);
                    if(ticketId=="")
                        {
                        generateTicketNumber();
                        response["ticket_id"]=ticketId;
                        }
                    insertIntoOpenTransaction(result);
                    }
                }
            insertIntoTicketCheck(result, "Test: "+result_description);                        
            }

        writeLog("parcxTicketCheck", "Result: " + result + "\tDescription:" + result_description);
        
        string open_transaction_check=response["open_transaction_check"];
        
        if (toString(response["access_allowed"]) == "true" && deviceType !=1 && anprEnabled == 1 && test==0 && open_transaction_check=="true") //anpr plate mismatch
            {
                string entry_plate = response["entry_plate_number"];
                string current_plate = response["current_plate_number"];
                
                writeLog("parcxTicketCheck", "Entry plate: " + entry_plate + " Current Plate:" + current_plate);

                if ( (entry_plate.length() >0 && current_plate.length() >0 && entry_plate != current_plate)||(plateCapturedId>0 ))
                    {
                    insertIntoTicketCheck("plate_mismatch", "Plate mismatch");
                    string id1=response["entry_plate_captured_id"];
                    string id2=response["plate_captured_id"];
                    if(id1=="")
                        id1="0";
                    string dismiss="1";
                    if(anpr_mismatch_check==1 && deviceType==2)
                        {
                        dismiss="0";  
                        response["result"]="plate_mismatch";
                        response["result_description"]="Plate mismatch.Entry Plate: "+entry_plate+" Current Plate:"+current_plate; 
                        writeLog("parcxTicketCheck", "Result:plate_mismatch\tDescription:Plate mismatch.Entry Plate: "+entry_plate+" Current Plate:"+current_plate); 
                        }
                    insertIntoPlateMismatch(entry_plate,current_plate,id1,id2,result,result_description,dismiss);
                    
                    }                    
            }

        
        
     writeLog("parcxTicketCheck","--------------END-----------------");   
    } catch (exception &e) {
        writeException("parcxTicketCheck", e.what());
    }
    return response;
}*/

extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("PX_TicketCheck", "1.0");
        extension.add<parcxTicketCheck>("parcxTicketCheck");
        return extension;
    }
}
