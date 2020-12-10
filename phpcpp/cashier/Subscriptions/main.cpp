#include "PX_GeneralOperations.h"
#include "PX_ContractParking.h"

using namespace std;
ContractParking obj;

Php::Value parcxContractParking(Php::Parameters &params)
	{
	Php::Value data=params[0];      
    int task=data["task"];   
    //cout<<task<<endl;
    Php::Value response;
    switch (task)
        {
        case 1: response=obj.getProductDetails();
		        break;           
        case 2:  response=obj.getPuchasedProductDetails(data);
		        break;    
        case 3:  response=obj.addProductSubscription(data);
                break;  
        case 4:  response=obj.renewProductSubscription(data);
                break;   
        case 5:  response=obj.verifyPurchase(data);
                break;       
		}
	return response;
	}


extern "C" 
	{    
    PHPCPP_EXPORT void *get_module()
		{        
		static Php::Extension extension("PX_ContractParking", "1.0");
        extension.add<parcxContractParking>("parcxContractParking");               
        return extension;
		}
	}
