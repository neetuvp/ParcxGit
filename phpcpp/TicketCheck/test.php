<?php

$json='{"transaction_id":"20200913095510","device_number":31,"plate_captured_id":99}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>