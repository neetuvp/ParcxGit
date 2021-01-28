<?php
$json= file_get_contents("php://input");
$data=json_decode($json);
$_SESSION["language"]=$data->{"language"};
echo $_SESSION["language"];
?>
