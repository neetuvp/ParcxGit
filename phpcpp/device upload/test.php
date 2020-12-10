<?php
$json= '{"authentication_key":"","device_number":"81","task":"3","data":[{"alarm":"Printer Unavilable","alarm_code":"122","alarm_date_time":"2020-08-31 11:13:10","alarm_description":"Printer unavilable to print receipt","alarm_severity":"10","carpark_name":"P1","carpark_number":"1","device_type":"3","device_name":"Cashier 1","device_number":"81","facility_number":"200005","id":"65"}]}';
$data=json_decode($json,TRUE);
print_r($data);
echo uploadToServer($data);
?>
