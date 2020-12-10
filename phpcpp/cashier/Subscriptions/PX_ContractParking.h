#ifndef CONRACTPARKING_H
#define CONRACTPARKING_H

#include <phpcpp.h>
using namespace std;
class ContractParking
	{
    public:    
    Php::Value getProductDetails();    
    Php::Value getPuchasedProductDetails(Php::Value json);    
    Php::Value addProductSubscription(Php::Value json);    
    Php::Value renewProductSubscription(Php::Value json); 
    Php::Value verifyPurchase(Php::Value json);        
	};

#endif    

