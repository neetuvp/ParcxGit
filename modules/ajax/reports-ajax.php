<?php
if($_SERVER["REQUEST_METHOD"]=='POST')
    {
	require_once('../../includes/common.php');		
	include('../../classes/reporting_revenue.php');
	$obj=new reporting_revenue();
    $json= file_get_contents("php://input");
    $data=json_decode($json,TRUE);
    $case=$data["option-type"];    	
    switch($case)
		{		
		case 1:   
				$obj->revenue_report($data);
				break;
		case 2:
				$obj->revenue_payment_transactions($data);
				break;
		case 3:
				$obj->get_payment_details($data);
				break;	
		case 4: 
				$obj->void_revenue_payment($data);
				break;
		case 5: 
				$obj->get_pdf_reciept($data);
				break;
		case 6: 
				$obj->shift_report($data);
				break;
		case 7: 
				$obj->get_shift_details($data["shift_id"]);
				break;
		case 8: 
				$obj->revenue_creditcard_payments_report($data);
				break;
		case 9: 
				$obj->revenue_payment_exceptions($data);
				break;
		case 10: 
			$obj->apm_refill_report($data);
			break;
		case 11: 
			$obj->apm_payout_report($data);
			break;
		case 12: 
			$obj->revenue_vat_report($data);
			break;	
		
		}                    
    }
?>
