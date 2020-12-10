
<?php
$json='{"task":4,"facility_number":"1001","carpark_number":"1","alert_title":"test","alert_description":"description","hour":"3","minute":"20","start_date_time":"2020-04-27 11:59:00","counter_id":"5","counter_condition":"= 10","counters_in_message":"5","email":"jeenavolga@gmail.com,,,,,","mobile":",,,,,"}';
$data=json_decode($json,TRUE);
echo alertsettings($data);
?>