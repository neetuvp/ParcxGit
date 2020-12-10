<?php
$json= file_get_contents("php://input");
$obj=new counter();
$case=$_GET["type"];
switch($case)
{
    case 1:                     
            $data=json_decode($json); 
			$id=$data->{'counter_id'};
			$value=$data->{'value'};        
			$msg=UpdateCorrectionFactor($id,$value);       
			$response['status']=200;
			$response['msg']=$msg;        
			$json_response=json_encode($response);   
            break;                    
}
echo $json_response;
?>