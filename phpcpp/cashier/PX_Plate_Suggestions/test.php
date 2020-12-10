<?php
//$json={"device_number":2,"transaction_id":123};
//$response=Payment_TicketCheck($json);
//php$response=isOpenTransaction('000001010b0058651513',1,0);
$response=PaymentCheck_GetPlateSuggestions('812');
echo json_encode($response);
?>
