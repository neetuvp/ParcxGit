<?php            	        
$data["user_role_id"]="105"; 
$data["task"]=1;                          
$r=parcxUserManagement($data);
if(is_array($r))
print_r($r);
else
echo $r;
             

?>
