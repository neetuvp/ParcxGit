<?php
//$json={"device_number":2,"transaction_id":123};
//$response=Payment_TicketCheck($json);
//php$response=isOpenTransaction('000001010b0058651513',1,0);
$response=getReservationDetails('000001010b0057d54332',5,5,1,1,3,2);
echo json_encode($response);
?>
