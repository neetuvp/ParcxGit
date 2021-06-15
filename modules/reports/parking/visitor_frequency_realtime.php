
<?php 
$page_title="Device Dashboard";

include('../../../includes/header.php');

$data=array();
$data["task"]=29;     
$data["language"]=$_SESSION["language"];
$data["page"]=15;
$json=parcxReport($data);
?>
<div class="navbar-has-tablink">

<?php
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header"><?=$json["visitor_frequency_realtime"]?></div>

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
            <option value="shortterm" id="shortterm"><?=$json["short_term"]?></option>
            <option value="contract" id="contract"><?=$json["contract"]?></option>
          </select>
        </div>
		
		
        <div class="col-md-2 d-none">          
		      <select class="form-control" id="contract_type">
	    <option value="" id="sel"><?=$json["select"]?></option>
            <option value="anpr" id="anpr">ANPR</option>
            <option value="ticketid" id="ticketid"><?=$json["ticket_id"]?></option>
            <option value="tag" id="tag"><?=$json["tag"]?></option>
          </select>
        </div>
				
        <div class="col-md-2">
          <input type="text" id="search_text" class="form-control" placeholder="<?=$json["search"]?>">
        </div>

         <?php include('../../../includes/additional-menu-report.php');?>        
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
          $data["language"]=$_SESSION["language"];  
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
var id;
var date_range_message="choose date range";
from="<?=$current_date." ".DAY_CLOSURE_START?>";
to="<?=$current_date." ".DAY_CLOSURE_END?>";
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


  //$('#view-report-button').click(function (event) 
function callReport()
{ 
    var visit_type = $("#visit_type").val();
    var contract_type = $("#contract_type").val();
	  var search_text = $("#search_text").val();
    
      var data={};
      data["visit_type"]=visit_type;
      data["contract_type"]=contract_type;           
      data["search_text"]=search_text;
      data["fromDate"]=from;
      data["toDate"]=to;
      data["language"] = $("#language").val();
      data["task"]=7;      
      var temp = JSON.stringify(data);     

      $.post("../../ajax/reports.php", temp)
        .done(function (result) {
          loadReport(result);
		  load_report=1;
        }, "json");

    event.preventDefault();
}
 // });
$('#view-report-button').click(function (event) 
{ 	
    if (!daterange)		
        alert(date_range_message);        		
    else 
        callReport();	    
});

function loadReportLabels()    
    {
    var data={};
    data["task"]=29;
    data["language"]=$("#language").val();    
    data["page"]=15;
    var json = JSON.stringify(data);
    $.post("../../ajax/reports.php",json,function(data)
        {	
        var json=JSON.parse(data);
        date_range_message=json.choose_datetime_range;
        $("#reservationtime").attr('placeholder',json.choose_datetime_range);        
        $("#pdf-report-header").html(json.visitor_frequency_realtime);   
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
 
 
            
        $("#shortterm").html(json.short_term);
        $("#contract").html(json.contract);
        $("#sel").html(json.select);
        $("#ticketid").html(json.ticket_id);
        $("#tag").html(json.tag);
        $("#search_text").attr('placeholder',json.search);
    
        
        });    
    }

$("#language").change(function()
    {	  
    update_session();
    loadReportLabels();    
    callReport();		
    });   

  $('#export_excel_report').click(function (event) 
    {
    export_to_excel("#page-content", "PMS_VisitorFrequency")
    }); 
</script>
<?php include('../../../includes/footer.php');?>
