<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];

include('../../classes/dashboard.php');
$dashboard=new dashboard();

include('../../classes/reporting_valet.php');
$valet=new reporting_valet();

$json= file_get_contents("php://input");    
$data=json_decode($json,TRUE); 


switch($task)
    {
    case 1:   //Get counters for dashboard - carparks list view
        $dashboard->UpdateOccupancyCarparkCounters($_GET['facility_number']);
        
        break;
    case 2:   // basic counters
       
        $dashboard->OccupancyCarparkDetail($data["facility_number"],$data["carpark_number"]);
        break;

    case 3:   // hourly Occupancy       
        $response=$dashboard->HourlyOccupancyReport($data['carpark'],$data['type']);
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
        $dashboard->watchdog_device_alarms(0,0);
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
        $dashboard->get_alarm_list($data);        
        break;
    case 14:
        $dashboard->get_latest_alarms_count()();
        break;
    case 15:
        $dashboard->get_latest_alarm_list($data);
        break;
    case 16:       
        $dashboard->dismiss_alarm($data["id"]);
        break;

    case 17:
        $dashboard->OccupancyCarparkDetail($carpark);
        break;
        
    case 18:       
        $dashboard->OccupancyCarparkCounters($data["facility_number"]);
        break;

    case 19:     
        $dashboard->UpdateOccupancyCarparkDetail($_GET['facility_number'],$_GET['carpark_number']);
        break;

    case 21:
        $dashboard->UpdateOccupancyFacilityCounters();
        break;
    case 22:
       $dashboard->get_device_status_by_facility();
       break;
    case 23:
       $dashboard->get_device_status_by_carpark($_GET['facility_number']);
       break;
    case 24:        
       $dashboard->get_device_status_by_device($_GET['facility_number'],$_GET['carpark_number']);
       break;
    case 25:        
       $dashboard->device_details($data);
       break;
    case 26:
            $dashboard->cash_levels ($data);
        break;
    } // End Switch 

exit();
