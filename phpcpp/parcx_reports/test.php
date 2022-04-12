<?php
$json='{"toDate":"2022-03-06","fromDate":"2022-03-05","weekdays":"","carpark":"","language":"English","task":23}';
$data=json_decode($json,true);
echo parcxReport($data);
?>