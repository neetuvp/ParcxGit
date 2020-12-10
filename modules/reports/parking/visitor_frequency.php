
<?php 
$page_title="Visitor frequency";

include('../../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Visitor Frequency</div>

<?php 
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>





<!-- Active Text Messages-->
<div class="content-wrapper block-data" data-status="active_messages">
<!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <div class="col-md-2">
          <!--<input type="text" id="serial" class="form-control" placeholder="TAG SERIAL">-->
		  <select class="form-control" id="visit_type">
			<option value="all">All</option>
            <option value="shortterm">Short Term</option>
            <option value="contract">Contract</option>
          </select>
        </div>
		
		
        
        <!-- search -->
        <div class="col-md-1">
          <button type="button" class="btn btn-block btn-secondary" id="view-report-button">Search</button>
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
        <?php echo parcxReport(array("task"=>"6","visit_type"=>"all"));?>
        </div>
        </div>
    </div>
</section>
</div>

<script>

  $('#view-report-button').click(function (event) 
    { 
    var visit_type = $("#visit_type").val();       
    var data={};
    data["visit_type"]=visit_type;
    data["task"]=6;
    var temp = JSON.stringify(data);    

    $.post("../../ajax/reports.php", temp)
      .done(function (result) {
        loadReport(result);
      }, "json");
    event.preventDefault();
  }); 

  $('#export_excel_report').click(function (event) 
    {
    export_to_excel("#report-content", "PMS_VisitorFrequency")
    });
    
</script>
<?php include('../../../includes/footer.php');?>