<?php 
 
require_once('../../includes/common.php');

$task=$_GET['task'];
$data_obj=json_decode(file_get_contents("php://input"));

include('../../classes/reporting_revenue.php');
$reporting_revenue=new reporting_revenue();

switch($task)
{    
    case 1: $reporting_revenue->list_revenue_summary_reports();
    break;
 
  case 8:  
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $device=$data_obj->{'device'};   
     $reporting_revenue->apm_refill_report($from,$to,$device);    
     break;
 
 
   case 9:  
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $device=$data_obj->{'device'}; 
    // $operator=$data_obj->{'operator'};
     $carpark=$data_obj->{'carpark'};   
     $type= $data_obj->{'type'};
     $category= $data_obj->{'category'};
     $discount=$data_obj->{'discount'};
     $showvoid = $data_obj->{'showvoid'};//Neetu 
     $reporting_revenue->revenue_payment_transactions($from,$to,$device,$carpark,$type,$category,$discount,$showvoid);//Neetu Added void
     break;
 
     case 10:  
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $reporting_revenue->revenue_validation_transactions($from,$to);
  
     break;
 
    case 11:       
         $from=$data_obj->{'fromDate'};
        $to=$data_obj->{'toDate'};
        $carpark=$data_obj->{'carpark'};
        $reporting_revenue->revenue_discount_usage_report($from,$to,$carpark);
        break;    
        //Neetu 
    case 13:
        $id=$_GET['id'];
        $reporting_revenue->VoidRevenuePayment($id,$data_obj->{'reason'});
        break;
       
	case 15:  
     $from=$data_obj->{'fromDate'};
     $to=$data_obj->{'toDate'};
     $device=$data_obj->{'device'};   
     $reporting_revenue->apm_payout_report($from,$to,$device);    
     break;
	 
	 case 16:      
     $device=$data_obj->{'device'};   
     $reporting_revenue->cash_levels($device);    
     break;
	 
	

} // End Switch 

exit();

