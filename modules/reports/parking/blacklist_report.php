<?php

$page_title="Application";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

$data=array();
$data["task"]=29;     
$data["language"]=$_SESSION["language"];
$data["page"]=12;
$json=parcxReport($data);

?>

</ul>

<div class="header text-dark" id="pdf-report-header"><?=$json["blacklist_report"]?></div>

<?php

include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
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

       

  <?php include('../../../includes/additional-menu-report.php');?>           
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
        $data["language"]=$_SESSION["language"];   		
        $data["task"]=10;                          
        echo parcxReport($data);
        ?>  
        </div>
      </div>
    </div>
  </section>
</div>

<?php include('../../../includes/footer.php'); ?>

<script>

//$('#view-report-button').click(function (event) 
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
 });
function callReport()
{    	
    var data={};
    data["from"]=from;
    data["to"]=to;           
    data["carpark"]=$("#multiselect").val().toString(); 
	data["language"] = $("#language").val();
    data["task"]=10;
    var jsondata = JSON.stringify(data);  	  
    $.post("../../ajax/reports.php",jsondata,function(data)
		  {      
      loadReport(data);    
	  load_report = 1;
      })
    .fail(function(jqxhr,status,error)
		  {
      alert("Error: "+error);
      }); 
		
    event.preventDefault();
}

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
    data["page"]=12;
    var json = JSON.stringify(data);
    $.post("../../ajax/reports.php",json,function(data)
        {	
        var json=JSON.parse(data);
        date_range_message=json.choose_datetime_range;
        $("#reservationtime").attr('placeholder',json.choose_datetime_range);        
        $("#pdf-report-header").html(json.blacklist_report);   
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
    
        
        });    
    }

$("#language").change(function()
    {	
    update_session();   
    loadReportLabels();    
    callReport();		
    });       

  $('#export_excel_report').click(function (event) {
    export_to_excel("#report-content", "PMS_Blacklist_Report")
  }); // end click event function
</script>
