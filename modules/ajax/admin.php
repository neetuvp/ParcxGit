
<?php

require_once '../../classes/adminlogin.php';
$obj=new AdminPanel();
$json= file_get_contents("php://input");

$case=$_GET["type"];
switch($case)
{
    case 1:
        $response=$obj->addUser($json);        
        break;
    case 2:
        $response=$obj->addUserRole($json);        
        break;
    case 3:
        $response=$obj->updateUserRole($json);        
        break;
    case 4:
        $response=$obj->DisablUserRole($json);        
        break;
    case 5:
        $response=$obj->EnableUserRole($json);        
        break;
    case 6:
    $response=$obj->Userlogin($json);        
        break;

    case 7:
    $response=$obj->UserStatusDisable($json);        
        break;

    case 8:
    $response=$obj->UserStatusEnable($json);        
        break;
    
    case 9:
        $response=get_user_roles_menu();
        break;
    
        case 10:
    $response=$obj->UpdateUser($json);        
        break;
    case 11:
        $data=json_decode($json);
        $response=change_password($data->{"id"},$data->{"currentPassword"},$data->{"newPassword"});
        break;
}

echo $response;
?>

        <?php

        if (isset($_POST['sub'])) {
            if (isset($_REQUEST['u_rolecode'])) {
                $u_rollname = $_REQUEST['u_rolecode'];
            }
            
           $msg= add_user_role($u_rollname);
           
           if($msg="success")
                {                
               
               $result= last_inserted_user_roll_id();
               
               $u_rolid= $result[0];
               
                }
            else
                {                                 
                echo "User Roll Insertion Failed";
                }
               
                
         if($u_rolid>0)    
            { 
            $params = "";
            $query = "insert into system_role_rights ( user_role_id , menu_id , rr_view , rr_edit , rr_delete)";
            $values="values ";
            foreach ($_POST as $key => $value) {
                if (strpos($key, "cb_") !== false) {
                    
                    $key = str_replace("cb_", "", $key);
                    $values = $values."(" .$u_rolid . ",";
                    $values = $values . $key . ",";
                    $values = $values . "1,1,1";
                    $values = rtrim($values, ',');
                    $values = $values . " ),";  
                }
            }
            $values = rtrim($values, ',');

            $query = $query . $values;

            $result = add_user_access_right($query);
            
            if($result=='success') 
             {           
               echo'Inserted Successfully'; 
  
             } 
            else 
            {
               echo'Failed To Insert';
            }
            
            }
           
        else
            {
               echo'Failed To Insert';
            }
            
        }
        ?>  
 




<?php

   if (isset($_POST['edit'])) {

    $roleid=$_POST['role_id'];

     foreach ($_POST as $key => $value) {
         if (strpos($key, "cb_") !== false) {

             $key = str_replace("cb_", "", $key);


             $acesR = get_menu_access_by_role_and_menu_id($roleid,$key);

             if(sizeof($acesR)>0)
             {
               $query = "update system_role_rights set" ;
                             $query=$query." rr_view=".$value. " where menu_id=".$key." and  user_role_id=".$roleid.";";  
             }
             // new menu insert time
             else 
             {
//       if($key===1)
//           {
                $query = "insert into system_role_rights (user_role_id , menu_id , rr_view , rr_edit , rr_delete) values ($roleid,$key,1,1,1);";
                 // }
             }


            $update_result= update_user_access_right($query);

         }
     }

         if($update_result=='success') 
             {  
                echo "User Role Updation Successfully"; 
             } 
           else  
             {  
                echo "Failed Updation";  
             }
             
            
        }
       
  ?>  

