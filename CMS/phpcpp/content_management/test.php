<?php            	        
//$json='{"device_id":"20","task":"3","playlists":[{"id":"1","repeat":1,"start":"2022-03-06 00:00:00","end":"2022-03-15 23:59:59","name":"Playlist 1"}]}';
$json = '{"device_id":"21","task":"8","playlists":[{"id":"31","repeat":0,"start":"2022-03-01 00:00:00","end":"2022-03-30 23:59:59","name":"Test"}]}';
$data = json_decode($json,true);
$r=parcxContentManagement($data);
if(is_array($r))
print_r($r);
else
echo $r;
             

?>
