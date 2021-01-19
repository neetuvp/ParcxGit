<?php
$data["validation_enabled"]="0";
$data["ticket_id"]="000001010b0058cbeabb";
$data["plate_number"]="";
$data["carpark_number"]="1";
$data["anpr_check_only"]="0";
$data["reservation_enabled"]="0";
$data["facility_number"]="1";
$data["device_type"]="2";
$data["access_enabled"]="1";
$data["wallet_enabled"]="1";
$data["device_name"]="exit";
$data["carpark_name"]="AFME";
$data["facility_name"]="AFME";
$data["device_number"]="31";
$data["plate_captured_id"]="123";
//echo json_encode($data);
//$json='{"ticket_id":"200005010b0059219122","carpark_number":"1","anpr_check_only":"0","reservation_enabled":"1","facility_number":"1","device_type":"3","qrcode_access_enabled":"1","device_name":"exit","carpark_name":"AFME","facility_name":"AFME","device_number":"31","transaction_id":"212121","camera_id":"71","plate_captured_interval":"10"}';
//$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>