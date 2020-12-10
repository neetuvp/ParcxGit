<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];
$json=file_get_contents("php://input");
$data_obj=json_decode($json);

include('../../classes/reporting_parking.php');
$reporting_parking=new reporting_parking();

switch($task)
{

case 1:   // manual movements
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};
     $operation=$data_obj->{'operation'};
     $reporting_parking->get_manual_movements_report($from,$to,$carpark,$operation);
     break;
 
 case 2:   // parking duration
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};
     $weekdays=$data_obj->{'weekdays'};
     $reporting_parking->show_parking_duration_report($from,$to,$carpark,$weekdays);
     break;

  case 3:   // open transactions
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $reporting_parking->open_transaction_report($from,$to);
     break;

  case 4:   // Traffic Summary Report
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $weekdays=$data_obj->{'weekdays'};
     $carpark=$data_obj->{'carpark'};
     $reporting_parking->traffic_summary_report($from,$to,$carpark,$weekdays);
     break;
 
 case 5:   // access movements
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};
     $serial=$data_obj->{'serial'};
     $acces_option=$data_obj->{'acces_option'};
     $reporting_parking->get_access_movements($from,$to,$carpark,$serial,$acces_option);
     break;
case 6:   // visitor frequency
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};
     $serial=$data_obj->{'serial'};
     $weekdays=$data_obj->{'weekdays'};
     $reporting_parking->visitor_frequency_report($from,$to,$carpark,$serial,$weekdays);
     break; 
case 7: // Validation 
      $from=$data_obj->{'fromDate'};
      $to=$data_obj->{'toDate'};
      $validator=$data_obj->{'validator'};
      $ticket=$data_obj->{'ticketid'};
      $reporting_parking->validation_report($from,$to,$validator,$ticket);
      break;
case 8: // BlackList 
      $from=$data_obj->{'fromDate'};
      $to=$data_obj->{'toDate'};
      $carpark=$data_obj->{'carpark'};      
      $reporting_parking->blacklist_report($from,$to,$carpark);
      break;
	  
case 9:
    $reporting_parking->get_anpr_image_platescaptured($json);
    break;
	
case 10:
    $reporting_parking->report_plates_captured($json);
    break;
case 11:
    $from=$data_obj->{'fromDate'};
    $to=$data_obj->{'toDate'};
    $device=$data_obj->{'device'};
    $reporting_parking->parking_movements($from,$to,$device);
    break;	
	

} // End Switch 

exit();

