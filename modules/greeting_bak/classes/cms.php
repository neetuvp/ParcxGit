<?php
define ("DB_HOST", "DBServer");
define ("DB_USER", "parcxservice");
define ("DB_PASSWORD","1fromParcx!19514");
define ("DB_NAME","parcx_server");
define ("DB_NAME_REPORTING","parcx_reporting");
define ("DB_NAME_DASHBOARD","parcx_dashboard");

class cms {        
    
    function db_connect($db)
        {       
        $con = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, $db);
        if (!$con)
            echo "Connection attempt failed";
        else
            return $con;
        } // end

    function get_cms_details()
        {
        $con = $this->db_connect(DB_NAME);        
        if ($con != NULL) {
            $sql="select * from greeting_screen";
            $result = mysqli_query($con, $sql) or die(mysqli_error($con));
            $response = array();
            while ($data = mysqli_fetch_assoc($result)){                
                //$row["id"]=$data["stage_id"];
                $id=$data["stage_id"];
                $row["video"]=$data["bg_video_file"];
                $row["image_type"]=$data["animation_type"];
                $row["image"]=$data["animation_file"];
                $row["message_1"]=$data["message_line1"];
                $row["message_2"]=$data["message_line2"];
                $row["message_3"]=$data["message_line3"];
                $response["stage_".$id]=$row;
                }
            mysqli_close($con);

            echo json_encode($response);
            }
        }
    }
?>