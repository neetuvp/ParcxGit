<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];

include('../../classes/dashboard.php');
$dashboard=new dashboard();

include('../../classes/reporting_valet.php');
$valet=new reporting_valet();


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
        $json= file_get_contents("php://input");    
        $data=json_decode($json,TRUE);         
        if($data["device_category"]=="APM")
            $dashboard->cash_levels($data["device_number"]);	
        
        if($data["device_category"]=="Entry Column" || $data["device_category"]=="Exit Column")
            {
            $html='<div class="d-flex justify-content-between align-items-center mb-4">
            <div class="col"><input type="submit" class="btn btn-outline-info btn-open-barrier btn-block" value="Open Barrier" id="'.$data["device_number"].'"></div>
            <div class="col"><input type="submit" class="btn btn-outline-info btn-close-barrier btn-block" value="Close Barrier" id="'.$data["device_number"].'"></div>
            <div class="col"><input type="submit" class="btn btn-outline-info btn-close-lane btn-block" value="Lane Closed Mode" id="'.$data["device_number"].'"></div>
            <div class="col"><input type="submit" class="btn btn-outline-info btn-free-passage btn-block" value="Free Passage Mode" id="'.$data["device_number"].'"></div>
            <div class="col"><input type="submit" class="btn btn-outline-info btn-standard-operation btn-block" value="Standard Operation Mode" id="'.$data["device_number"].'"></div>
            </div>';
            echo $html;
            }
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
    case 22:
       $dashboard->get_device_status_by_facility();
       break;
    case 23:
       $dashboard->get_device_status_by_carpark($_GET['facility_number']);
       break;
    case 24:        
       $dashboard->get_device_status_by_device($_GET['facility_number'],$_GET['carpark_number']);
       break;
    } // End Switch 

exit();
