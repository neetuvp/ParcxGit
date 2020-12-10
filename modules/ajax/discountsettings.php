<?php
$json= file_get_contents("php://input");
$case=$_GET["type"];
$data=json_decode($json);
switch($case)
{
    case 1:                     
        $response=DisableDiscountSettings($data->{"id"});
        break; 
    case 2:                     
        $response=EnableDiscountSettings($data->{"id"});
        break; 
    case 3:  
    {
        $id = "";
        $data = json_decode($json);
        //$discount_id = $data -> {'discount_id'};
        $discount_id = GetNextDiscountId();
        $discount_name = $data -> {'discount_name'};
        $discount_type = $data -> {'discount_type'};
        $discount_option = $data -> {'discount_option'};
        $discount_category = $data -> {'discount_category'};
        $discount_value = $data -> {'discount_value'};
        $cp_id = $data -> {'cp_name'};
        $status =  $data -> {'status'};
        $response = PostDiscountSettings($id,$discount_id,$discount_name,$discount_type, $discount_option,$discount_category,$discount_value,$cp_id,$status);
        break;
    }
    case 4:  
    {
        $id = $_GET["id"];       
        $data = json_decode($json);
        $discount_id = $data -> {'discount_id'};
        $discount_name = $data -> {'discount_name'};
        $discount_type = $data -> {'discount_type'};
        $discount_option = $data -> {'discount_option'};
        $discount_category = $data -> {'discount_category'};
        $discount_value = $data -> {'discount_value'};
        $cp_id = $data -> {'cp_name'};
        $status =  1;
        $response = PostDiscountSettings($id,$discount_id,$discount_name,$discount_type, $discount_option,$discount_category,$discount_value,$cp_id,$status);
    break;
    }

}
echo $response;
?>