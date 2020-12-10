<?php
header('Access-Control-Allow-Origin: *');
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Manual Operation</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Modal > Manual open Reason -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document" id="manual-reason-content">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Manual Operation Reason</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body pt-4 pl-4 pr-4">
        <p>Reason:</p>
        <textarea name='reason_text' id='reason_text' class="form-control mb-4"></textarea>
        <span id="reasonempty"></span>
      </div>
      <div class="modal-footer">
        <button type='button' class='btn btn-info' name='ok_reason' id='ok_reason' value='OK'>Ok</button>
        <button type='button' class='btn btn-info' name='cancel_reason' id='cancel_reason'
          value='Cancel'>Cancel</button>
      </div>
    </div>
  </div>
</div>
<!-- end / Modal -->

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
            <div class="col"></div>
            <div class="col"></div>
            <div class="col"></div>
          </div>      
        </div>  
        <?php 
        listManualOperation();
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
var task,device_ip,device_number,carpark_number,device_name,description,movement_type,device_type,controller_task;
$(".btn-open-barrier, .btn-close-barrier, .btn-lock-barrier, .btn-unlock-barrier, .btn-enable-lane, .btn-disable-lane").click(function(){
  var value=$(this).attr('value'); 
  device_number=$(this).attr('id'); 
  var retVal = confirm("Confirm : "+value+" ?"); 
  if (retVal == true)
  { 
    
    device_name=$("#device_details_"+device_number).attr('device_name');
    carpark_number=$("#device_details_"+device_number).attr('carpark_number');
    device_ip=$("#device_details_"+device_number).attr('device_ip'); 
	device_type=$("#device_details_"+device_number).attr('device_type'); 
    switch(value)
      {
      case "Open Barrier":
        task="S01";
		controller_task=1;
        movement_type=3;
        description="Barrier Open From Server"
        break;
      case "Close Barrier":
        task="S02";
		controller_task=2;
        movement_type=4;
        description="Barrier Close From Server"
        break;
	  case "Open Barrier1":        
		controller_task=1;
        movement_type=3;
        description="Barrier Open1 From Server"
        break;
      case "Close Barrier1":       
		controller_task=2;
        movement_type=4;
        description="Barrier Close1 From Server"
        break;
		case "Open Barrier2":        
		controller_task=3;
        movement_type=3;
        description="Barrier Open2 From Server"
        break;
      case "Close Barrier2":        
		controller_task=4;
        movement_type=4;
        description="Barrier Close2 From Server"
        break;
      case "Lock Barrier":
		controller_task=3;
        task="S03";
        movement_type=0;
        description="Lock Barrier From Server"
        break;
      case "Unlock Barrier":
		controller_task=4;
        task="S04";
        movement_type=0;
        description="Unlock Barrier From Server"
        break;
      case "Enable Lane":
		controller_task=5;
        task="S05";
        movement_type=0;
        description="Enable Lane From Server"
        break;
      case "Disable Lane":
		controller_task=6;
        task="S06";
        movement_type=0;
        description="Disable Lane From Server"
        break;      
      }
  $('#detailModal').modal('show');  
  }
  
}); 


$(function()
	{
	//modal cancel	
	$(document).on('click', '#cancel_reason', function() 
		{
		$('#reason_text').val("");		
		$('#detailModal').modal('hide');	
		}); 	
	//modal ok
	$(document).on('click', '#ok_reason', function() 
		{         
		var reason = $('#reason_text').val(); 
		if (reason != "") 
			{   
			$('#detailModal').modal('hide');
			var operator=$("#username2").text();  
			var data = {
				device_number:device_number,
				device_ip:device_ip,
				device_name:device_name,
				description: description,
				carpark_number: carpark_number,
				task: task,
				operator: operator,        
				reason:reason,
				device_type:device_type,
				movement_type:movement_type
				};       
			var jsontemp = JSON.stringify(data);	            

			if(device_type==6 || device_type==7)
				{
				var url="http://" +device_ip + "/services/ParkingLaneManagement.php";								
				var data={task:controller_task};
				var temp = JSON.stringify(data);
				$.post(url, temp)
					.done(function (result) 
						{
						console.log(result);
						if(result==1)	
							{							
							$.post("../ajax/operations.php?task=1", jsontemp)
								.done(function (result) {  			
									if(result==0)
										alert("Insert to Db failed");
									else
										alert(device_name+" "+description+" Success");
									}, "json");
							}
						else
							alert(device_name+" "+description+" Failed");
						}, "json")
					.fail(function() {
						alert( "Not reachable" );
						});
				}
			else
				{
				$.post("../ajax/operations.php?task=1", jsontemp)
				.done(function (result) {  			
				if(result==1)
					alert(device_name+" "+description+" Success");
				else
					alert(device_name+" "+description+" Failed");
					}, "json");	
				}	
		$('#reason_text').val("");			
		} 
	else 
		{
		alert("Please enter a valid reason");
		}
		});
	});
</script>