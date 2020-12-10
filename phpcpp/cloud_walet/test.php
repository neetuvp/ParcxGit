<?php
//$response=updateWaletTransaction("H 95943","12345",5,31,"Exit",1,"AFME",1,"Dubai AFME");				
$response=getWalletTransactions(1);
//$response=getPlateCapturedDetails(31,1,10,10,0);
//$response=updateWalletPayment(3,200);
echo json_encode($response);
?>
