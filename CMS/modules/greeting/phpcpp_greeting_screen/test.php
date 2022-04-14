<?php            	        
//$json='{"device_id":"20","task":"3","playlists":[{"id":"1","repeat":1,"start":"2022-03-06 00:00:00","end":"2022-03-15 23:59:59","name":"Playlist 1"}]}';
$json = '{"id":"","schedule":1,"file_name":"71586114_Video2.mp4","start_date":"2022-04-08","expiry_date":"2022-04-30","task":6}';
$data = json_decode($json,true);
$r=parcxGreetingScreen($data);
if(is_array($r))
print_r($r);
else
echo $r;
             

?>
