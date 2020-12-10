<?php
require_once('../../../includes/common.php');

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_revenue.php');
$reports=new reporting_revenue();
?>

<div class="content-wrapper">
  <section class="content">
    <div class="container-wide">
      <div class="row mb-3">
        <div class="col-sm-6">
          <h4 class="text-dark" id="pdf-report-header">Parking Validation</h4>
        </div><!-- /.col -->
      </div><!-- /.row -->
      <!-- Shift FIlter Form -->
      <div class="card card-primary mb-4">
        <div class="card-body">
          <div class="row">


            <div class="col-3">

              <!-- Date and time range -->

              <label>Date and Time Range</label>

              <div class="input-group">
                <div class="input-group-prepend">
                  <span class="input-group-text"><i class="far fa-clock"></i></span>
                </div>
                <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off">
              </div>
              <!-- /.input group -->

            </div><!-- /col3-->

            <div class="col-1">
              <label class="d-block">&nbsp;</label>
              <button type="button" class="btn btn-block btn-secondary" id="view-report-button">Search</button>
            </div> <!-- /.Search button -->

            <div class="col-auto" id="action-buttons">
              <label class="d-block">&nbsp;</label>
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
            </div><!-- /col1-->


          </div>
          <!-- /.row -->
        </div>
      </div>

      <div class="col-md-6 padding-reset">
        <div class="card" id="report-content">
          <?php 
          $current_date=date("Y-m-d");
          $reports->revenue_validation_transactions($current_date." ".DAY_CLOSURE_START, $current_date." ".DAY_CLOSURE_END);?>
        </div>
      </div>

    </div>

    <!-- /.row (main row) -->
</div><!-- /.container-fluid -->
</section>
<!-- /.content -->
</div>
<!-- /.content-wrapper -->

<?php include('../../../includes/footer.php');?>

<script>
  $('#view-report-button').click(function (event) { // close the home page banner : control button

    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        toDate: to,
        fromDate: from
      };
      var temp = JSON.stringify(data);
      //  alert(temp);
      $.post("../../ajax/revenue.php?task=10", temp)
        .done(function (result) {

          $("#report-content").html(result);
          reportSuccess();
        }, "json");
    } // end if 

    event.preventDefault();

  }); // end traffic report by day 

  $('#export_excel_report').click(function (event) {

    export_to_excel("#report-content", "PMS_Validation_Report")

  }); // end click event function
</script>