<?php
//$json='{"transaction_id":"123456789","device_number":31,"plate_number":"","ticket_id":"000001010b00583b1f28","coupon_ticket_id":"200001010b015aa14061","plate_captured_id":0,"coupon_id":"000001010b0057dae8f6"}';
$json='{"transaction_id":"123456789","device_number":31,"plate_number":"","ticket_id":"200001010b015aa14061","plate_captured_id":0,"coupon_id":""}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>