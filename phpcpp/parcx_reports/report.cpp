#include "PX_GeneralOperations.h"
#include "PX_ParkingReport.h"
#include "PX_RevenueReport.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;
GeneralOperations General;
ParkingReport pObj;
RevenueReport rObj;


void writeLog(string function,string message)
    {
    General.writeLog("WebApplication/ApplicationLogs/PX-Report-"+General.currentDateTime("%Y-%m-%d"),function,message);    
    }

void writeException(string function,string message)
    {
    General.writeLog("WebApplication/ExceptionLogs/PX-Report-"+General.currentDateTime("%Y-%m-%d"),function,message); 
	writeLog(function,"Exception: "+message);   
    }



Php::Value parcxReport(Php::Parameters &params)
	{
	Php::Value data=params[0];      
    int task=data["task"];   
    Php::Value response;
    switch (task)
        {
        case 1:  pObj.openTransactionReport(data);
		        break;
        case 2:  pObj.parkingMovementsReport(data);
		        break;
        case 3:  pObj.validationReport(data);
		        break;
        case 4:pObj.accessReport(data);
                break;
        case 5:pObj.trackTicket(data);
                break;
        case 6:pObj.visitorFrequencyReport(data);
                break;
        case 7:pObj.visitorFrequencyRealTimeReport(data);
                break;
        case 8:rObj.paymentTransactions(data);
                break;
        case 9:pObj.ticketDetails(data);
                break;
        case 10:  pObj.blacklistReport(data);
		        break;
	}
	return response;
	}


extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_Reports", "1.0");
        extension.add<parcxReport>("parcxReport");               
        return extension;
		}
	}
