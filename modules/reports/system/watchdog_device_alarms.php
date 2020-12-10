<?php
$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Watchdog Device Alarms</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_system.php');
$reports=new reporting_system();
?>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">
      <!-- devices multiselect-->
      <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php $reports->get_devices();?>
          </select>
        </div>
		
		<div class="col-md-2">
          <select class="form-control" id="alarm-severity">
            <option value="0">Select Severity</option>
			<option value="between 8 and 10">Low</option>
			<option value="between 4 and 7">Medium</option>
			<option value="between 1 and 3">High</option>
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
        <button type="button" class="btn  btn-secondary" id="view-report-button">View Report</button>
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
        <div class="card-body p-0" id="shift-report-content">
          <?php 
                   $current_date=date("Y-m-d");  
                   $reports->watchdog_device_alarms($current_date." ".DAY_CLOSURE_START,$current_date." ".DAY_CLOSURE_END,"","0");
                   ?>

        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
  $('#view-report-button').click(function (event) { // close the home page banner : control button

    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        toDate: to,
        fromDate: from,
        deviceNumber:$("#deviceNumber").val().toString(),
		    severity:$("#alarm-severity").val()
      };
      var temp = JSON.stringify(data);
      //alert(temp);
      $.post("../../ajax/system.php?task=2", temp)
        .done(function (result) {

          $("#shift-report-content").html(result);
          reportSuccess();
        }, "json");
    } // end if 

    event.preventDefault();

  }); // end traffic report by day 

  $('#export_excel_report').click(function (event) {

    export_to_excel("#shift-report-content", "PMS_watchdog_device_alarm_Report")

  }); // end click event function
</script>