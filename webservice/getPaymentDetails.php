<?php
ini_set("display_errors","1");
function CreateLogs($data)
	{
    date_default_timezone_set('Asia/Dubai');
    $file = "/var/log/parcx/Services/ApplicationLogs/Parcx-Payment-".date("Y-m-d").".log";	
    $fh = fopen($file, 'a') or die("can't open file");    
    fwrite($fh,date('H:i:s'). "\t");
    fwrite($fh,$data);
    fwrite($fh,"\n");
    fclose($fh);
	}
if($_SERVER['REQUEST_METHOD']=='POST')
	{
	$json= file_get_contents("php://input");	
	CreateLogs("Json input: ".$json);
	$data= json_decode($json); 
	$ticketNo= $data->{'ticket_no'};	
	if (strlen($ticketNo)!=0 && strlen($ticketNo)== 20)
		{			
		$blacklistedStatus= isBlacklisted($ticketNo);
		CreateLogs("Blacklist status: ".$blacklistedStatus);
	
		if(strcasecmp($blacklistedStatus, "blacklisted")==0)
			{
			$response["error"]='false';
			$response["result"]='Blacklisted';
			}
		else
			{
			//check in open transaction
			$opentransaction_status=isOpenTransaction($ticketNo);
			CreateLogs("Open transaction check: ".json_encode($opentransaction_status));				
			//Not in open transaction means exited
			if($opentransaction_status["opentransaction_status"]=="false")
				{				
				$response["error"]='false';
				$response["result"]='Exited';			
				}
			else
				{
				//In open transaction,check paid or not
				$entry_grace_time_remaining = (int)$opentransaction_status["entry_grace_time_remaining"];				
				if($entry_grace_time_remaining!=0)
					{							
					$response["error"]='false';
					$response["result"]='entry_grace';
					$response["entry_grace_time_remaining"]=$opentransaction_status["entry_grace_time_remaining"];					
					}
				else
					{	
					$already_paid_status = isAlreadyPaid($ticketNo);
					CreateLogs("Already paid check: ".json_encode($already_paid_status));				
					if($already_paid_status["payment_status"]=="true")
						{					
						$response["error"]='false';
						$response["result"]='paid';
						$response["ticket_payment_date_time"]=$already_paid_status["ticket_payment_date_time"];
						$response["grace_time_remaining"]=$already_paid_status["grace_time_remaining"];						
						$response["max_exit_date_time"]=$already_paid_status["max_exit_date_time"];	
						}
					else
						{					
						$response["error"]='false';
						$response["result"]='unpaid';	
						}
					}
				}
			}
		}
	else
		{		
		$response["error"]='true';
		$response["result"]="ticket number not present or Invalid ticket No";
		}
	}
else
	{	
	$response["error"]='true';
	$response["result"]="Not a post request";
	}
CreateLogs("Json output: ".json_encode($response)."\n");
echo json_encode($response);	
?>