<?php
$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Payment Transactions</div>
<!-- <input type="checkbox" name="showvoid" value="showvoid" id="showvoid"> 
Show Void Transactions-->

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_revenue.php');
$reports=new reporting_revenue();
?>

<!-- Modal -->
<div class="modal fade text-dark" id="detailModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-body">
        <div class="card">
          <div class="card-header">
            <h3 class="card-title d-inline">Detailed Payment Information</h3>
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

<!-- Modal > Void Reason -->
<div class="modal fade text-dark" id="voidReasonModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
  aria-hidden="true">
  <div class="modal-dialog" role="document" id="void-reason-content">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Void Reason</h5>
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
        <button type='button' class='btn btn-info'  id='ok_reason' value='OK'>Ok</button>
        <button type='button' class='btn btn-info' value='Cancel' data-dismiss="modal">Cancel</button>
      </div>
    </div>
  </div>
</div>
<!-- end / Modal -->

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
            <?php $reports->get_payment_devices();?>
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
            <?php $reports->get_discounts();?>
          </select>
        </div>
      
        <!-- date and time -->
        <div class="col-md-3 mt-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off"
              placeholder="Choose Date and Time Range">
          </div>
        </div>

        <!-- search -->
        <div class="col-md-1 mt-3">
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
        <div class="card-body p-0" id="report-content">          
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
          $reports->revenue_payment_transactions($data);
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
$('body').on('click', '#payment_detail', function () 
	{
	if(voidClicked==false)	
		{
    var data={};
    data["payment_id"]=$(this).attr('payment_id');	
    data["option-type"]=3;	
	  var jsondata = JSON.stringify(data);  
    console.log(jsondata);
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
		  {		
        $("#payment-detail-content").html(data);
        $('#detailModal').modal('show');
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 		
		}	
  }); // end click event function 
  
$('#view-report-button').click(function (event) 
	{     	
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
    data["option-type"]=2;
    var jsondata = JSON.stringify(data);  
	  console.log(jsondata);
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
		  {
      $("#report-content").html(data);   		
      reportSuccess();
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 
		} // end if 
  event.preventDefault();
	}); // end traffic report by day 

$('#export_excel_report').click(function (event) 
	{    
  export_to_excel("#report-content", "PMS_Payment_transaction")
	});

$('body').on('click', '.btn-void-transaction', function (e) 
	{	
	voidClicked=true;
  id = $(this).attr("data-id"); 
	$('#voidReasonModal').modal('show');  
	e.preventDefault();	
	});
  
$('#voidReasonModal').on('hide.bs.modal', function (e) 
	{
	voidClicked=false;
	});
	
$('#pdfReceiptModal').on('hide.bs.modal', function (e) 
	{
	voidClicked=false;
	});

$('body').on('click', '#ok_reason', function () 
	{
  var reason = $('#reason_text').val();
  if (reason != "") 
		{
    var data={};
    data["id"]= id;
    data["reason"]=reason; 
    data["option-type"]=4;
    var jsondata = JSON.stringify(data);  
    console.log(jsondata);
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
		  {		
      $('#voidReasonModal').modal('hide');
      $('#reason_text').val("");	
			  if ((!daterange)) 					
					{						
					location.reload();
					}
				else
					$('#view-report-button').click();  
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      });
		} 
	else 
		{
		$('#reasonempty').html("Please enter a valid reason");
		}
	});


     
$('body').on('click', '.btn-show-pdf-reciept', function () 
	{
  id = $(this).attr("data-id");   
	voidClicked=true;
  var data={};
  data["payment_id"]= id;   
  data["option-type"]=5;
  var jsondata = JSON.stringify(data);  
  console.log(jsondata);
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
		  {	
      $("#pdf-receipt").html(data);  	
      $('#pdfReceiptModal').modal('show');  
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      });
	
			
    });
  
</script>