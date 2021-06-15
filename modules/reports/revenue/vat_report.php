<?php
$page_title="Application Home";
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');




$data=array();
$data["task"]=29;     
$data["language"]=$_SESSION["language"];
$data["page"]=5;
$json=parcxReport($data);

?>
</ul>

<div class="header text-dark" id="pdf-report-header"><?=$json["vat_report"]?></div>
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
            <h3 class="card-title d-inline" id="modal_payment_heading"><?=$json["detailed_payment"]?></h3>
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
          <?=$json["receipt_details"]?>
        </h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">×</span>
        </button>
      </div>
      <div class="modal-body">
        <h5 class="modal-title" id="tax-invoice"><?=$json["tax_invoice"]?></h5>
        <div id="pdf-receipt">         
        </div>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal" id="pdf-modal-close"><?=$json["close"]?></button>
        <button type="button" class="btn btn-primary" id="print-pdf-receipt"><?=$json["print"]?></button>
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
        <div class="col-md-2 mb-4">
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
            <option value="'Cash'" id="paymentTypeCash"><?=$json["cash"]?></option>
            <option value="'CreditCard'" id="paymentTypeCreditCard"><?=$json["credit_card"]?></option>             
          </select>
        </div>

        <!-- payment category multiple select -->
        <div class="col-md-2">
          <select class="form-control" id="paymentCategory" multiple="multiple">
            <!-- <option value=0>All Payment Types</option> -->
            <option value=1 id="paymentCategory1"><?=$json["parking_fee"]?></option>
            <option value=2 id="paymentCategory2"><?=$json["lost_ticket"]?></option>
            <option value=3 id="paymentCategory3"><?=$json["discount"]?></option>
            <option value=4 id="paymentCategory4"><?=$json["grace_period"]?></option>
            <option value=5 id="paymentCategory5"><?=$json["product_sales"]?></option>
          </select>
        </div>
               
        <!-- discounts multiple -->
        <div class="col-md-2">
          <select class="form-control" id="discounts" multiple="multiple">
          <?php echo parcxSettings(array("task"=>"35"));?>
          </select>
        </div>
      
    <?php include('../../../includes/additional-menu-report.php');?>       

  <section class="content">
    <div class="container-wide"> 
      <div class="card">
        <div class="card-body" id="report-content">          
          <?php 
          $current_date=date("Y-m-d");    
          $data["from"]=$current_date;
          $data["to"]=$current_date;           
          $data["carpark"]="";    
          $data["device"]="";	
          $data["payment-category"]=[];	
          $data["payment-type"]="";	
          $data["discount"]="";          
          $data["showvoid"]=0;   
          $data["task"]=26;  
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
var date_range_message="choose date range";
from="<?=$current_date." ".DAY_CLOSURE_START?>";
to="<?=$current_date." ".DAY_CLOSURE_END?>";

$(function() 
    {
    $('#deviceNumber').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "<?=$json["all_devices"]?>",               
        nonSelectedText:"<?=$json["select_devices"]?>",       
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_devices"]?>"  
        });
        
    $('#multiselect').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,      
        selectAllText: "<?=$json["all_carparks"]?>",
        nonSelectedText: "<?=$json["select_carparks"]?>",
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_carparks"]?>",       
        });    
    $('#paymentCategory').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "<?=$json["all_category"]?>",               
        nonSelectedText:"<?=$json["select_category"]?>",       
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_category"]?>"      
        });       
    $('#paymentType').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "<?=$json["all_payment"]?>",               
        nonSelectedText:"<?=$json["select_payment"]?>",       
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_payment"]?>"   
        });     
    $('#discounts').multiselect(
        {
        buttonWidth: '100%',
        includeSelectAllOption: true,
        selectAllText: "<?=$json["all_discount"]?>",               
        nonSelectedText:"<?=$json["select_discount"]?>",       
        selectAllNumber: false,
        allSelectedText: "<?=$json["all_discount"]?>"       
        }); 
    });

$('body').on('click', '#payment_detail', function () 
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
      });              
    }); // end click event function 
    
function callReport()
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
        });
        
    }
  
$('#view-report-button').click(function (event) 
    { 	
    if (!daterange)		
        alert(date_range_message);        		
    else 
	callReport();	    
    });


$('#export_excel_report').click(function (event) 
    {    
    export_to_excel("#report-content", "PMS_Payment_transaction")
    });

     
$('body').on('click', '.btn-show-pdf-reciept', function () 
    {
    id = $(this).attr("data-id");       
    var data={};
    data["payment_id"]= id;   
    data["task"]=25;
    data["language"]=$("#language").val();   
    var jsondata = JSON.stringify(data);      
    $.post("../../ajax/reports.php",jsondata,function(data)
        {
        $("#pdf-receipt").html(data);  	
        $('#pdfReceiptModal').modal('show');  
        });    			
    });
	
function loadReportLabels()    
    {
    var data={};
    data["task"]=29;
    data["language"]=$("#language").val();    
    data["page"]=5;
    var json = JSON.stringify(data);
    $.post("../../ajax/reports.php",json,function(data)
        {		
        var json=JSON.parse(data);
        date_range_message=json.choose_datetime_range;
        $("#reservationtime").attr('placeholder',json.choose_datetime_range);        
        $("#pdf-report-header").html(json.vat_report);   
        $("#view-report-button").html(json.view_report);   
        $("#export").html(json.export);   
        $("#export_excel_report").html(json.export_to_excel);           
        $("#export_pdf_report").html(json.export_to_pdf); 
        $("#logout").html(json.logout); 
        search_label=json.search;   
        entries_label= json.entries_label;
        info_label=json.info_label;
        previous_label=json.previous;
        next_label=json.next;        
        
        $("#modal_payment_heading").html(json.detailed_payment); 
        $("#exampleModalLabel").html(json.receipt_details); 
        $("#print-pdf-receipt").html(json.print); 
        $("#pdf-modal-close").html(json.close);  
        $("#tax-invoice").html(json.tax_invoice);
        $("#paymentTypeCash").html(json.cash);
        $("#paymentTypeCreditCard").html(json.credit_card);
        $("#paymentCategory1").html(json.parking_fee);
        $("#paymentCategory2").html(json.lost_ticket);
        $("#paymentCategory3").html(json.discount);
        $("#paymentCategory4").html(json.grace_period);
        $("#paymentCategory5").html(json.product_sales);
        
                                        
        $('#deviceNumber').multiselect('destroy');
        $('#deviceNumber').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: json.all_devices,                                    
            nonSelectedText:json.select_devices,                   
            selectAllNumber: false,
            allSelectedText: json.all_devices             
            });  
            
        $('#multiselect').multiselect('destroy');
        $('#multiselect').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,      
            selectAllText: json.all_carparks,
            nonSelectedText: json.select_carparks,
            selectAllNumber: false,
            allSelectedText: json.all_carparks
            }); 
    
        $('#paymentCategory').multiselect('destroy');
        $('#paymentCategory').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: json.all_category,
            nonSelectedText:json.select_category,
            selectAllNumber: false,
            allSelectedText: json.all_category
            });   
            
        $('#paymentType').multiselect('destroy');
        $('#paymentType').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: json.all_payment,
            nonSelectedText:json.select_payment,
            selectAllNumber: false,
            allSelectedText:json.all_payment 
            }); 
            
        $('#discounts').multiselect('destroy');
        $('#discounts').multiselect(
            {
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: json.all_discount,
            nonSelectedText:json.select_discount,
            selectAllNumber: false,
            allSelectedText: json.all_discount
            }); 
        });    
    }

$("#language").change(function()
{
    update_session();	  	  
    loadReportLabels();    
    callReport();		
}); 
  
</script>
