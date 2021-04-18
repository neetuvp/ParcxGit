<?php

$json='{"transaction_id":"123456789","device_number":31,"plate_number":"","ticket_id":"2000050154005a8ef87f","plate_captured_id":199,"anpr_enable":1}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>