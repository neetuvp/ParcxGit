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

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title"> Detailed Shift Information </h3>
          </div>
          <!-- /.card-header -->
          <div class="card-body p-0">
            <table class="table table-condensed" id="shift-detail-content">



            </table>
          </div>
          <!-- /.card-body -->
        </div>
      </div>
      <div class="modal-footer">
        <!-- <button type="button" class="btn btn-secondary" data-dismiss="modal">Export PDF</button> -->
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
      </div>
    </div>
  </div>
</div>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <!-- carparks -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
            <?php $reports->get_carparks();?>
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
        <div class="card-body p-0" id="manual-movements-report-content">

          <?php 
                  $current_date=date("Y-m-d");  
                   $reports->get_manual_movements_report($current_date." ".DAY_CLOSURE_START,$current_date." ".DAY_CLOSURE_END,[],[]);?>

        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php'); ?>

<script>

  $('#view-report-button').click(function (event) { // close the home page banner : control button

    var carpark = $("#multiselect").val();
    var operation = $("#operation").val();

    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        carpark: carpark,
        operation: operation,
        toDate: to,
        fromDate: from
      };
      var temp = JSON.stringify(data);
      //alert(temp);
      $.post("../../ajax/parking.php?task=1", temp)
        .done(function (result) {

          $("#manual-movements-report-content").html(result);
          reportSuccess()
        }, "json");
    } // end if 

    event.preventDefault();

  }); // end traffic report by day 

  $('#export_excel_report').click(function (event) {

    export_to_excel("#manual-movements-report-content", "PMS_Manual_Movements_Report")

  }); // end click event function
</script>