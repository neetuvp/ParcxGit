<?php
$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Credit Card Payment Transactions</div>
<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
//include('../../../classes/reporting_revenue.php');
//$reports=new reporting_revenue();
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

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php echo parcxSettings(array("task"=>"37"));?>
          </select>
        </div>

       

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off"
              placeholder="<?php echo parcxReport(array("task"=>"13","language"=>$_SESSION["language"],"label"=>"choose_datetime_range"));?>">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1">
        <button type="button" class="btn  btn-secondary" id="view-report-button" onclick="creditcard_transactions()">View Report</button>
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
          $data["from"]=$current_date." ".DAY_CLOSURE_START;
          $data["to"]=$current_date." ".DAY_CLOSURE_END;           
          $data["carpark"]="";    
          $data["device"]="";	 
		  $data["task"]=21;  
		  $data["language"]=$_SESSION["language"];
          echo parcxReport($data); 
          ?>         
        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
//$('#view-report-button').click(function (event) 
var load_report = 0;
function creditcard_transactions()
  {   
  if ((!daterange)) 
    {
    alert("choose date range");
    } 
  else 
    {
    var data={};
    data["from"]=from;
    data["to"]=to;         
    data["carpark"]=$("#multiselect").val().toString(); 
    data["device"]=$("#deviceNumber").val().toString();      
	data["language"] = $("#language").val();	
    data["task"]=21;
    var jsondata = JSON.stringify(data);      
    console.log(jsondata);
    $.post("../../ajax/reports.php",jsondata,function(data)
      {		
      $("#report-content").html(data);
	  load_report=1;
    reportSuccess();      
      })
  .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      });     
    } // end if 

  event.preventDefault();
  }
 // }); // end traffic report by day 

function loadPage()
  {
  loadheadingreport("creditcard_transactions");
  if(load_report==1)
	creditcard_transactions(); 
  }
$("#language").change(function(){
  loadPage();
});

$( document ).ready(function() {
	loadheadingreport("creditcard_transactions");
});

$('#export_excel_report').click(function (event) 
  {  
  export_to_excel("#report-content", "PMS_CreditCard_Payment_transaction")
  }); // end click event function



 
  
</script>