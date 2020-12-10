#ifndef PARKINGREPORT_H
#define PARKINGREPORT_H

#include <phpcpp.h>
using namespace std;
class ParkingReport
	{
    public:    
    void openTransactionReport(Php::Value json);    
    void parkingMovementsReport(Php::Value json);    
    void validationReport(Php::Value json);    
    void accessReport(Php::Value json);  
    void trackTicket(Php::Value json);   
    void visitorFrequencyReport(Php::Value json);
	void visitorFrequencyRealTimeReport(Php::Value json); 
    void ticketDetails(Php::Value json);   
    void blacklistReport(Php::Value json);     
	};

#endif    

