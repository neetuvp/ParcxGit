<?php
//$json= '{"to_date":"2020-05-23 23:59:59","from_date":"2020-05-10 00:00:00","task":"1"}';
//$data=json_decode($json,TRUE);
//echo parkingReport($data);
//echo parcxReport(array("task"=>"6","visit_type"=>"all"));
//$json= '{"fromDate":"2020-12-01","toDate":"2020-12-28","carpark":"","weekdays":"","option-type":1,"language":"English","task":23}';//'{"device":"","task":18,"language":"English"}';
//'{"from":"2019-06-01 00:00:00","to":"2020-12-22 23:59:59","carpark":"","device":"","plate_number":"","option":"","user_role":"Admin","wiegand":"1","language":"English","task":17}';
//$data=json_decode($json,TRUE);
//echo parcxReport($data);


            	        
             $data["label"]="date_time"; 
             $data["task"]=29;  
             $data["language"]="English";                          
             $r=parcxReport($data);
             if(is_array($r))
                print_r($r);
            else
                echo $r;
             

?>
