<?php
if($_FILES["file"]["name"]!='')
    {
    $response["status"]=200;
    $file=$_FILES["file"]["tmp_name"];
    $width=$file[0];
    $height=$file[1];
    if($width>550)
        {
        $response["status"]=400;
        $response["Message"]="Width >550";
        }
    if($height>150)
        {
        $response["status"]=400;
        $response["Message"]="Height >150";
        }
    $extension=pathinfo($_FILES["file"]["name"],PATHINFO_EXTENSION);      
    if($extension!="jpg" && $extension!="jpeg")
        {
        $response["status"]=400;
        $response["Message"]="Only JPEG Images accepting";
        }
    if($response["status"]==200)
        {
        $name=$_POST["name"] ;     
        $target="../dist/img/customer-logo/";   
        $location=$target.$name;
        if(move_uploaded_file($_FILES["file"]["tmp_name"],$location))
            $response["Message"]="Uploaded successfully";
        else
            $response["Message"]="Upload failed";
        }
    echo json_encode($response);
    }
?>