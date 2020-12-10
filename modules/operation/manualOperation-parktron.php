<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Manual Operation</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
//# App Function Classes
include('../../classes/ParkingOperations.php');
$ParkingOperations=new ParkingOperations();
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
        $ParkingOperations->ListSystemDevices();     
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
var task,device_ip,device_number,carpark_number,device_name,description,movement_type;
$(".btn-open-barrier, .btn-close-barrier, .btn-lock-barrier, .btn-unlock-barrier, .btn-enable-lane, .btn-disable-lane, .btn-reset-power").click(function(){
  var value=$(this).attr('value'); 
  device_number=$(this).attr('id'); 
  var retVal = confirm("Confirm : "+value+" ?"); 
  if (retVal == true)
  { 
    
    device_name=$("#device_details_"+device_number).attr('device_name');
    carpark_number=$("#device_details_"+device_number).attr('carpark_number');
    device_ip=$("#device_details_"+device_number).attr('device_ip'); 
    switch(value)
      {
      case "Open Barrier":
        task=1;
        movement_type=3;
        description="Barrier Open From Server"
        break;
      case "Close Barrier":
        task=2;
        movement_type=4;
        description="Barrier Close From Server"
        break;
      case "Lock Barrier":
        task=3;
        movement_type=0;
        description="Lock Barrier From Server"
        break;
      case "Unlock Barrier":
        task=4;
        movement_type=0;
        description="Unlock Barrier From Server"
        break;
      case "Enable Lane":
        task=5;
        movement_type=0;
        description="Enable Lane From Server"
        break;
      case "Disable Lane":
        task=6;
        movement_type=0;
        description="Disable Lane From Server"
        break; 
	case "Reset Power":
        task=7;
        movement_type=0;
        description="Reset Power From Server"
        break; 		
      }
  $('#detailModal').modal('show');  
  }
  
}); 


$(function(){
  //modal ok
  $(document).on('click', '#ok_reason', function() 
    {         
    var reason = $('#reason_text').val(); 
    if (reason != "") 
      {   
      $('#detailModal').modal('hide');
      var operator=$("#username2").text();      
	  var url="http://" + device_ip + "/services/ParkingLaneManagement.php";
	  //call operation
	  var data={
		  carpark:carpark_number,
		  task:task,
		  operator:operator,
		  url:url
	  };
	  temp = JSON.stringify(data);  
	console.log(temp);
	   $.post(url, temp)
         .done(function (result) {
           alert(description+" success");
         }, "json");
		 
	//insert to db	 
      data = {
        device_number:device_number,         
        device_name:device_name,
        description: description,
        carpark_number: carpark_number,
        task: task,
        operator: operator,        
        reason:reason,
        movement_type:movement_type
       }; 
	   
       temp = JSON.stringify(data);          
       console.log(temp);
       $.post("../ajax/manualoperations.php?task=1", temp)
       .done(function (result) {
        console.log(result);
         if(result==1)
            alert("Operation inserted to db success");
          else
            alert("Operation inserted to db failed");
       }, "json");

    $('#reason_text').val("");  
    } else {
      alert("Please enter a valid reason");

    }

  });
});
</script>