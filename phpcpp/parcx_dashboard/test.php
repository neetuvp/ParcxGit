<?php 
//$json='{"task":8,"id":10,"plate_number1":"DUB Q 94286","plate_number2":"DUB Q 94286","corrected_plate_number1":"DUB Q 94286","corrected_plate_number2":"DUB Q 94286","user_id":"30","user_name":"parcx"}';
$json = '{"task":21}';
$data=json_decode($json,TRUE);
echo parcxDashboard($data);
?>
