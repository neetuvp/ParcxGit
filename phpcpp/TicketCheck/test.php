<?php

$json='{"transaction_id":"123456789","device_number":31,"plate_number":"OMN BR 1211","ticket_id":"","plate_captured_id":0}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>