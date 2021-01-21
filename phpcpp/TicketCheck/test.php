<?php
// $data["validation_enabled"]="0";
// $data["ticket_id"]="000001010b0058cbeabb";
// $data["plate_number"]="";
// $data["carpark_number"]="1";
// $data["anpr_check_only"]="0";
// $data["reservation_enabled"]="0";
// $data["facility_number"]="1";
// $data["device_type"]="2";
// $data["access_enabled"]="1";
// $data["wallet_enabled"]="1";
// $data["device_name"]="exit";
// $data["carpark_name"]="AFME";
// $data["facility_name"]="AFME";
// $data["device_number"]="31";
// $data["plate_captured_id"]="123";
//echo json_encode($data);
$json='{"transaction_id":"20200913095510","carpark_number":1,"reservation_entry_grace_period":5,"reservation_enabled":1,"device_number":11,"facility_number":200005,"device_type":1,"ticket_id":"20210104081427","access_enabled":1,"reservation_exit_grace_period":5}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>