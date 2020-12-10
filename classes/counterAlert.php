<?php
class counterAlert
{
    function counterCondition($json)
        {
        $data=json_decode($json); 
        $id=$data->{'counter_id'};
        $counterName=$data->{'counter_name'};
        $condition=$data->{'condition'};
        $dataIdMsg=$data->{'datamsgId'};
        $dataMsg=$data->{'datamsg'};
        
        $dataMsg= str_replace("<br>", ",", $dataMsg);
        
        $dataMsg= rtrim($dataMsg,',');
        
        $dataIdMsg= rtrim($dataIdMsg,',');
        
        $email=$data->{'email'};
        $email= rtrim($email,',');
        $mobile=$data->{'mobile'};
        $mobile= rtrim($mobile,',');
        if(isset($data->hour))
            $hour=$data->{'hour'};
            
        if(isset($data->minute))
            $minute=$data->{'minute'};
        
        $startDateTime=$data->{'startdateTime'};
        $msg=addCounterCondition($id,$counterName,$condition,$startDateTime,$hour,$minute,$dataIdMsg,$dataMsg,$email,$mobile);        
        $response['status']=200;
        $response['msg']=$msg;        
        $json_response=json_encode($response);
        return $json_response;
        }
    function counterTimeInterval($json)
        {
         $data=json_decode($json); 
         
        if(isset($data->hour))
            $hour=$data->{'hour'};
            
        if(isset($data->minute))
            $minute=$data->{'minute'};
        
        $startDateTime=$data->{'startdateTime'};
        
        $dataMsg=$data->{'datamsg'};
        $dataIdMsg=$data->{'datamsgId'};
        $dataMsg= str_replace("<br>", ",", $dataMsg);
        $dataMsg= rtrim($dataMsg,',');
        $dataIdMsg= rtrim($dataIdMsg,',');
        
        $email=$data->{'email'};
        $email= rtrim($email,',');
        $mobile=$data->{'mobile'};
        $mobile= rtrim($mobile,',');
        $msg=addTimeInterval($startDateTime,$hour,$minute,$dataIdMsg,$dataMsg,$email,$mobile);        
            
        //return data as json
        $response['status']=200;
        $response['msg']=$msg;        
        $json_response=json_encode($response);
        return $json_response;
        }
    function networkStatus($json)
        {
        $data=json_decode($json); 
         
        if(isset($data->hour))
            $hour=$data->{'hour'};
            
        if(isset($data->minute))
            $minute=$data->{'minute'};
        
        $startDateTime=$data->{'startdateTime'};
        
        $email=$data->{'email'};
        $email= rtrim($email,',');
        $mobile=$data->{'mobile'};
        $mobile= rtrim($mobile,',');
        
        $msg=addNetworkStatusInterval($startDateTime,$hour,$minute,$email,$mobile);                    
        //return data as json
        $response['status']=200;
        $response['msg']=$msg;        
        $json_response=json_encode($response);
        return $json_response;
        
        }
}
