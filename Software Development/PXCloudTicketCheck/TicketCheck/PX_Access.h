#ifndef ACCESS_H
#define ACCESS_H

#include <string.h>
#include <phpcpp.h>
using namespace std;
class Access
{
    public:    
    Php::Value checkAccess(string ticket_id,string parkingZone,int carpark,int facility,int movementType);    
};

#endif    
