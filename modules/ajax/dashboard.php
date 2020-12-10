<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];
//$data_obj=json_decode(file_get_contents("php://input"));

include('../../classes/dashboard.php');
$dashboard=new dashboard();
include('../../classes/reporting_valet.php');
$valet=new reporting_valet();

include('../../classes/reporting_system.php');
$system=new reporting_system();

include('../../classes/reporting_revenue.php');
$revenue=new reporting_revenue();

switch($task)
    {
    case 1:   //Get counters for dashboard - carparks list view
        $dashboard->UpdateOccupancyCarparkCounters($_GET['facility_number']);
        
        break;
    case 2:   // basic counters
        $json= file_get_contents("php://input");    
        $data=json_decode($json);   
        $dashboard->OccupancyCarparkDetail($data->{"facility_number"},$data->{"carpark_number"});
        break;

    case 3:   // hourly Occupancy
        $json= file_get_contents("php://input"); 
        $data=json_decode($json);
        $response=$dashboard->HourlyOccupancyReport($data->{'carpark'},$data->{'type'});
        echo json_encode($response);
        break;
    case 4: $dashboard->revenue_lastdays();
    break;

    case 5:
        $dashboard->get_revenue_summary();
        break;

    case 6:
        $valet->valet_parking_keydashboard();
        break;    
    case 7:
        $system->watchdog_device_alarms(0,0);
        break;
   
    case 9:$dashboard->get_device_status();
        break;

    case 10:
        $dashboard->live_revenue();
        break;

    case 11:
        $valet->get_valet_counters();
        break;

    case 12:
        $dashboard->getLastUpdatedDateForOccupancy();
        break;

    case 13:
        $json= file_get_contents("php://input");    
        $data=json_decode($json,TRUE);         
        if($data["device_category"]=="APM")
            $revenue->cash_levels($data["device_number"]);	
        $dashboard->getAlarmList($data["device_number"]);
        break;
    case 14:
        $dashboard->getLatestAlarmsCount();
        break;
    case 15:
        $dashboard->getLatestAlarmList();
        break;
    case 16:
        $json= file_get_contents("php://input");    
        $data=json_decode($json);   
        $dashboard->dismissAlarm($data->{"id"});
        break;

    case 17:
        $dashboard->OccupancyCarparkDetail($carpark);
        break;
        
    case 18:
        $json= file_get_contents("php://input");    
        $data=json_decode($json);    
        $dashboard->OccupancyCarparkCounters($data->{"facility_number"});
        break;

    case 19:     
        $dashboard->UpdateOccupancyCarparkDetail($_GET['facility_number'],$_GET['carpark_number']);
        break;

    case 21:
        $dashboard->UpdateOccupancyFacilityCounters();
        break;
    } // End Switch 

exit();
