<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];
$data_obj=json_decode(file_get_contents("php://input"));

include('../../classes/reporting_system.php');
$reporting_system=new reporting_system();

switch($task)
{    
    case 1: 
    $from=$data_obj->{'fromDate'};
    $to=$data_obj->{'toDate'};
    $reporting_system->watchdog_network($from,$to);
    break;

    case 2: 
        $from=$data_obj->{'fromDate'};
        $to=$data_obj->{'toDate'};
        $device=$data_obj->{'deviceNumber'};
        $severity=$data_obj->{'severity'};
        $reporting_system->watchdog_device_alarms($from,$to,$device,$severity);
        break;
    case 3:
    $from=$data_obj->{'fromDate'};
    $to=$data_obj->{'toDate'};
    $reporting_system->TicketBlacklist($from,$to);
    break;
  

} // End Switch 

exit();

