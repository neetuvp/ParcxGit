<?php
$json= '';
$data=json_decode($json,TRUE);
print_r($data);
echo uploadToServer($data);
?>
