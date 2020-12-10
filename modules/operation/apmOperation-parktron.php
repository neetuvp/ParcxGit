<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">APM Operation</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
//# App Function Classes
include('../../classes/ParkingOperations.php');
$ParkingOperations=new ParkingOperations();
?>

<div class="content-wrapper">
<section class="content">
    <div class="container-wide">

      <div class="col-md-12 padding-reset">
        <div class="card" id="page-content">   
        <div class="card card-primary" id="active-rates-table">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
            <div class="col" >Device Name</div>
            <div class="col" >Device IP</div>
            <div class="col"></div>
            <div class="col"></div>
            <div class="col"></div>                   
          </div>      
        </div>  
        <?php 
        $ParkingOperations->ListAPMDevices();     
        ?>
        

      </div>      
        </div>
      </div>

    </div>
</div>
</section>
</div>

<?php

include('../../includes/footer.php');
?>

<script>
var device_ip,device_number;
$(".btn-issue-lost").click(function(){
  var value=$(this).attr('value'); 
  device_number=$(this).attr('id'); 
  var retVal = confirm("Confirm : "+value+" ?"); 
  if (retVal == true)
	{         
    device_ip=$("#device_details_"+device_number).attr('device_ip');  
	var data = 
		{
        device_ip:device_ip                
		};       
    var temp = JSON.stringify(data);              
    $.post("../ajax/operations.php?task=2", temp)
       .done(function (result) {  
		if(result==1)
			alert("Success");  
		else
			alert("Failed");  
       }, "json");
  
	}
  
}); 


</script>