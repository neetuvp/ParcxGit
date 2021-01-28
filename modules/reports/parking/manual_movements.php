<?php

$page_title="Application";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Manual Movements</div>

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_parking.php');
$reports=new reporting_parking();
?>



<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <!-- carparks -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
          
		   <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!-- operation methods -->
        <div class="col-md-2">
          <select class="form-control" id="operation">
            <option value="0">All Operation Methods</option>
            <option value="cashier">Manual Open from Cashier</option>
            <option value="pushbutton">Remote/Push Button Open</option>
          </select>
        </div>

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off" placeholder="Choose Date and Time Range">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1">
        <button type="button" class="btn  btn-secondary" id="view-report-button" onclick="manual_movement()">View Report</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div>

      <div class="additional-menu-right">
        <div id="action-buttons">
          <div class="btn-group">
            <button type="button" class="btn btn-warning">Export</button>
            <button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">
              <span class="caret"></span>
              <span class="sr-only">Toggle Dropdown</span>
            </button>
            <div class="dropdown-menu" role="menu">
              <a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>

              <a class="dropdown-item" href="#" id="export_pdf_report">
                Export to PDF
              </a>
            </div>
          </div>
        </div>
      </div>

    </div>
  </div>
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">

      <div class="card">
        <div class="card-body" id="report-content">

          <?php 
                  //$current_date=date("Y-m-d");  
                   //$reports->get_manual_movements_report($current_date." ".DAY_CLOSURE_START,$current_date." ".DAY_CLOSURE_END,[],[]);
				 $current_date=date("Y-m-d");    
				$data["from"]=$current_date." ".DAY_CLOSURE_START;
				$data["to"]=$current_date." ".DAY_CLOSURE_END;           
				$data["carpark"]="";  
				$data["operation"]="";  
				$data["task"]=11;                          
				echo parcxReport($data);
			?>
				   

        </div>
      </div>

    </div>
</section>
</div>

<?php include('../../../includes/footer.php'); ?>

<script>

 // $('#view-report-button').click(function (event) { // close the home page banner : control button
	function manual_movement(){
    var carpark =$("#multiselect").val().toString(); 
    var operation = $("#operation").val();
	var language =$("#language").val();
    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        carpark: carpark,
        operation: operation,
        to: to,
        from: from,
		language:language,
		task:11
      };
      var jsondata = JSON.stringify(data);  	  
    $.post("../../ajax/reports.php",jsondata,function(data)
		  {      
      loadReport(data);   
		//$("#report-content").html(data);
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 
		} // end if 
  event.preventDefault();
	//}); //manual movements-report-content
	}

  $('#export_excel_report').click(function (event) {

    export_to_excel("#report-content", "PMS_Manual_Movements_Report")

  }); // end click event function
  
  function loadPage()
  {
	loadheadingreport("manual_movement");
	manual_movement(); 
  }
$("#language").change(function(){
  loadPage();
}); 
</script>