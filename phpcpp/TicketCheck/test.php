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
$json='{"transaction_id":"2000050151075a3cd494","online_validation":0,"anpr_enable":0,"carpark_number":1,"reservation_entry_grace_period":5,"anpr_check_only":0,"reservation_enabled":1,"camera_id":72,"device_type":3,"ticket_id":"200005010B005A18483B","central_cashier":0,"reservation_exit_grace_period":5,"device_ip":"192.168.1.164","device_name":"Cashier81","app_key":"PXCashier","app_user_id":30,"device_function":1,"facility_number":200005,"device_number":81,"qrcode_loop_detection":1,"plate_captured_interval":18000,"qrcode_access_enabled":1,"parking_zone":0}';
$data=json_decode($json);
$response=parcxTicketCheck($data);
print_r($response);
//echo json_encode($response);	
?>