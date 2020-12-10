<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];
$data_obj=json_decode(file_get_contents("php://input"));

include('../../classes/reporting_valet.php');
$reporting_valet=new reporting_valet();

switch($task)
{

case 1:   // Valet Parking Transactions
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};    
     $reporting_valet->get_valet_parking_transactions($from,$to,$carpark);
     break;
 
 case 2:   // Driver Utilization
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $carpark=$data_obj->{'carpark'};
     $reporting_valet->get_valet_driver_utilization($from,$to,$carpark);
     
     break;

  case 3:   // car pictures
     $id=$data_obj->{'id'};
     $reporting_valet->get_valet_car_pictures($id);
     break;
//Neetu
 case 4:   // Edit Transaction
    $id=$_GET['id'];
    $reporting_valet->edit_valet($id,$data_obj->{'plateno'},$data_obj->{'country'},$data_obj->{'emirate'},$data_obj->{'carbrand'},$data_obj->{'carcolor'},$data_obj->{'carmodel'},$data_obj->{'frontplate'},$data_obj->{'rightplate'},$data_obj->{'leftplate'},$data_obj->{'backplate'});
    break;
case 5:   // Get  Car Details
   $id=$_GET['id'];
   $reporting_valet->get_valet_car_details($id);
   break;
   //Neetu
case 6:   //edit car pictures
   $id=$data_obj->{'id'};
   $reporting_valet->edit_valet_car_pictures($id);
   break;
case 7:   //upload car pictures
   //$id=$_GET['id'];
   $reporting_valet->upload_valet_car_pictures();
   break;
   //Neetu


} // End Switch 

exit();

