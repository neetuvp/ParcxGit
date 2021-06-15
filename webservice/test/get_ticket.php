<?php
define ("DB_HOST", "localhost");
define ("DB_USER", "parcxservice");
define ("DB_PASSWORD","1fromParcx!19514");
define ("DB_NAME_REPORTING","parcx_reporting");
$con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_NAME_REPORTING);
$query_string = "select ticket_id from open_transactions UNION SELECT ticket_id from parking_blacklist ORDER BY RAND() LIMIT 1;";
$result = mysqli_query($con, $query_string) or die(mysqli_error($con));
$ticket="";
if ($data = mysqli_fetch_assoc($result)) 
    $ticket=$data["ticket_id"];

mysqli_close($con);
echo $ticket;

?>