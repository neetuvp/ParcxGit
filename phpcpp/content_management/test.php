<?php            	        
//$json='{"device_id":"20","task":"3","playlists":[{"id":"1","repeat":1,"start":"2022-03-06 00:00:00","end":"2022-03-15 23:59:59","name":"Playlist 1"}]}';
$json = '{"task":"3","id":27,"type":0}';
$data = json_decode($json,true);
$r=parcxContentManagement($data);
if(is_array($r))
print_r($r);
else
echo $r;
             

?>
