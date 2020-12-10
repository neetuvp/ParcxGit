
<?php 
$page_title="Device Dashboard";

include('../../../includes/header.php');
?>
<div class="navbar-has-tablink">

<?php
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Realtime Visitor Frequency</div>

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
		      <select class="form-control" id="visit_type">
            <option value="shortterm">Short Term</option>
            <option value="contract">Contract</option>
          </select>
        </div>
		
		
        <div class="col-md-2">          
		      <select class="form-control" id="contract_type">
			      <option value="">Select</option>
            <option value="anpr">ANPR</option>
            <option value="ticketid">Ticket ID</option>
            <option value="tag">Tag</option>
          </select>
        </div>
				
        <div class="col-md-2">
          <input type="text" id="search_text" class="form-control" placeholder="SEARCH">
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
          <?php
          $current_date=date("Y-m-d");    
          $data["fromDate"]=$current_date." ".DAY_CLOSURE_START;
          $data["toDate"]=$current_date." ".DAY_CLOSURE_END;           
          $data["visit_type"]="shortterm";            
          $data["contract_type"]="anpr";            
          $data["search_text"]="";            
          $data["task"]=7;          
          echo parcxReport($data);
          ?>  
        </div>
      </div>
    </div>

</section>
</div>

<script>
  /* Table Show - Hide */

$(document).ready(function()
  {
	if($('#visit_type').val()=="shortterm")
	  {
		$('#contract_type').attr("disabled", true); 
		$('#contract_type').val("anpr");
	  }                        
  });

$('#visit_type').change(function() 
  {
  if($('#visit_type').val()=="shortterm")
	  {
		$('#contract_type').attr("disabled", true); 
		$('#contract_type').val("anpr");
	  }
	else if($('#visit_type').val()=="contract")
	  {
		$('#contract_type').attr("disabled", false); 
		$('#contract_type').val("");
	  }
});


  $('#view-report-button').click(function (event) 
    { 
    var visit_type = $("#visit_type").val();
    var contract_type = $("#contract_type").val();
	  var search_text = $("#search_text").val();

    if ((!daterange)) 
      {
      alert("choose date range");
      } 
    else 
      {     
      var data={};
      data["visit_type"]=visit_type;
      data["contract_type"]=contract_type;           
      data["search_text"]=search_text;
      data["fromDate"]=from;
      data["toDate"]=to
      data["task"]=7;      
      var temp = JSON.stringify(data);     

      $.post("../../ajax/reports.php", temp)
        .done(function (result) {
          loadReport(result);

        }, "json");

      } 

    event.preventDefault();

  });

  $('#export_excel_report').click(function (event) 
    {
    export_to_excel("#page-content", "PMS_VisitorFrequency")
    }); 
</script>
<?php include('../../../includes/footer.php');?>