<?php            	        
$data["user_name"]="parcx";
$data["password"]="Parcx123!"; 
$data["task"]=12;                          
$r=parcxUserManagement($data);
if(is_array($r))
print_r($r);
else
echo $r;
             

?>
