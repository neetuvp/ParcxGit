<?php
$page_title="Application Home";
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>
</ul>

<div class="header text-dark" id="pdf-report-header">VAT Report</div>
<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title d-inline" id="modal_payment_heading">Detailed Payment Information</h3>
            <button type="button" class="close text-white" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">×</span></button>
          </div>
          <!-- /.card-header -->
          <div class="card-body p-0">
            <table class="table table-condensed " id="payment-detail-content">
            </table>
          </div>
          <!-- /.card-body -->
        </div>
      </div>
    </div>
  </div>
</div>


<!-- pdf receipt modal -->
<div class="modal fade" id="pdfReceiptModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"aria-hidden="true">
  <div class="modal-dialog modal-lg" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="exampleModalLabel">
          Receipt Details
        </h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">×</span>
        </button>
      </div>
      <div class="modal-body">
        <h5 class="modal-title">Tax Invoice</h5>
        <div id="pdf-receipt">         
        </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
        <button type="button" class="btn btn-primary" id="print-pdf-receipt">Print</button>
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
            <?php echo parcxSettings(array("task"=>"12"));?>
          </select>
        </div>

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"14","type"=>"3,4,5"));?>
          </select>
        </div>

        <!-- payment type multiple select -->
        <div class="col-md-2">
          <select class="form-control" id="paymentType" multiple="multiple">
            <!-- <option value=0>All Payment Types</option> -->
            <option value="'Cash'">Cash</option>
            <option value="'CreditCard'">Credit Card</option>             
          </select>
        </div>

        <!-- payment category multiple select -->
        <div class="col-md-2">
          <select class="form-control" id="paymentCategory" multiple="multiple">
            <!-- <option value=0>All Payment Types</option> -->
            <option value=1>Parking Fee</option>
            <option value=2>Lost </option>
            <option value=3>Discount</option>
            <option value=4>Grace Period</option>
            <option value=5>Product Sale</option>
          </select>
        </div>

        <!-- discounts multiple -->
        <div class="col-md-2">
          <select class="form-control" id="discounts" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"35"));?>
          </select>
        </div>
      
        <!-- date and time -->
        <div class="col-md-3 mt-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off"
              placeholder="<?php echo parcxReport(array("task"=>"13","language"=>$_SESSION["language"],"label"=>"choose_date_range"));?>">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1 mt-3">
        <button type="button" class="btn  btn-secondary" id="view-report-button" onclick="payment_transactions()">View Report</button>
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
          $data["payment-category"]=[];	
          $data["payment-type"]="";	
          $data["discount"]="";          
          $data["showvoid"]=0;   
          $data["task"]=8;  
          $data["language"]=$_SESSION["language"];
          echo parcxReport($data);
          ?>
          </div>
        </div>
      </div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
var id;
var voidClicked=false;
var load_report = 0;
$('body').on('click', '#payment_detail', function () 
    {
    if(voidClicked==false)	
        {
        var data={};
        data["payment_id"]=$(this).attr('payment_id');	
        data["task"]=24;
        data["language"]=$("#language").val();  
        var jsondata = JSON.stringify(data);      
        $.post("../../ajax/reports.php",jsondata,function(data)
            {		
        $("#payment-detail-content").html(data);
        $('#detailModal').modal('show');
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 

	var heading = {};
		heading["task"]=13;
		heading["language"]=$("#language").val(); 
		heading["label"]="detailed_payment";
		jsondata = JSON.stringify(heading);    
		$.post("../../ajax/reports.php",jsondata,function(data)
		  {		
		$("#modal_payment_heading").html(data);
	  })
	.fail(function(jqxhr,status,error)
		  {
	  alert("Error: "+error);
	  });
		}	
  }); // end click event function 
  
//$('#view-report-button').click(function (event) 
function payment_transactions(){     	
  if ((!daterange)) 
		{
		alert("choose date range");
    return ;
		} 
	else 
		{
    var data={};
    data["from"]=from;
    data["to"]=to;           
    data["carpark"]=$("#multiselect").val().toString();    
	  data["device"]=$("#deviceNumber").val().toString();	
    data["payment-category"]=$("#paymentCategory").val();	
    data["payment-type"]=$("#paymentType").val().toString();	
    data["discount"]=$("#discounts").val().toString();
    if ($('#showvoid').is(':checked')) 
      data["showvoid"]=1;
    else
      data["showvoid"]=0;    
    data["task"]=26;
	data["language"]=$("#language").val();   
    var jsondata = JSON.stringify(data);  	  
    $.post("../../ajax/reports.php",jsondata,function(data)
		  {      
      loadReport(data);    
	  load_report=1;
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 
} // end if 
  event.preventDefault();
	}
	//}); // 

$('#export_excel_report').click(function (event) 
	{    
  export_to_excel("#report-content", "PMS_Payment_transaction")
	});
	

$('body').on('click', '.btn-show-pdf-reciept', function () 
    {
    id = $(this).attr("data-id");   
    voidClicked=true;
    var data={};
    data["payment_id"]= id;   
    data["task"]=25;
    data["language"]=$("#language").val();   
    var jsondata = JSON.stringify(data);      
    $.post("../../ajax/reports.php",jsondata,function(data)
        {
        $("#pdf-receipt").html(data);  	
        $('#pdfReceiptModal').modal('show');  
        })
    .fail(function(jqxhr,status,error)
        {
        alert("Error: "+error);
        });
			
    });
	


$("#language").change(function()
    {	  
    changeLanguage();    
    loadheadingreport("vat_report");
    loadMultiselect();
    if(load_report==1)
        payment_transactions(); 		
    }); 
  
$( document ).ready(function() 
    {
    loadheadingreport("vat_report");
    });
  
</script>