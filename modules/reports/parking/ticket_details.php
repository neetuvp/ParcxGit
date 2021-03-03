<?php

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

$data=array();
$data["task"]=29;     
$data["language"]=$_SESSION["language"];
$data["page"]=14;
$json=parcxReport($data);
?>

</ul>

<div class="header text-dark" id="pdf-report-header"><?=$json["ticket_details"]?></div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes

?>
<!-- Modal -->
<div class="modal fade" id="image-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
    aria-hidden="true">
    <div class="modal-dialog modal-xl" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">ANPR Image</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body p-4" id="image-content-modal">                                           
              
            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<div class="content-wrapper">
  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">
        

        <!-- plate -->
        <div class="col-md-2">
          <input type="text" id="ticket_id" class="form-control" placeholder="<?=$json["ticket_id"]?>">
        </div>        
       

        <!-- search -->
        <div class="col-md-1">
          <button type="button" class="btn  btn-secondary" id="view-details-button" onclick="callReport()">View details</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div>



    </div>
  </div>
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">      
      <div class="p-0" id="page-content">       
        
       
      </div>   
    </div>
  </section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
//$('#view-report-button').click(function (event) 
var load_report = 0;
function callReport()
{ 

    var data={};            
    data["ticket_id"]=$("#ticket_id").val();   
	data["language"] = $("#language").val();	
    data["task"]=9;
    var temp = JSON.stringify(data);    
    console.log(temp);  
    $.post("../../ajax/reports.php", temp)
      .done(function (result) {        
        $("#page-content").html(result);
        reportSuccess();
		load_report=1;

      }, "json");


    event.preventDefault();
  }
  //}); 

function loadReportLabels()    
    {
    var data={};
    data["task"]=29;
    data["language"]=$("#language").val();    
    data["page"]=14;
    var json = JSON.stringify(data);
    $.post("../../ajax/reports.php",json,function(data)
        {		
        var json=JSON.parse(data);
        date_range_message=json.choose_datetime_range;
        $("#reservationtime").attr('placeholder',json.choose_datetime_range);        
        $("#pdf-report-header").html(json.ticket_details);   
        $("#view-report-button").html(json.view_report);   
        $("#export").html(json.export);   
        $("#export_excel_report").html(json.export_to_excel);           
        $("#export_pdf_report").html(json.export_to_pdf); 
        $("#logout").html(json.logout); 
        $("#view-details-button").html(json.view_details);
        search_label=json.search;   
        entries_label= json.entries_label;
        info_label=json.info_label;
        previous_label=json.previous;
        next_label=json.next;    
        $("#ticket_id").attr('placeholder',json.ticket_id);  
        }); 
        
        
    }


$("#language").change(function()
{	  
    loadReportLabels();    
    callReport();		
}); 

$('#export_excel_report').click(function (event) 
  {
  export_to_excel("#page-content", "Ticket_Details")
  });

$( document ).ready(function() {
	loadheadingreport("ticket_details");
});
$('body').on('click', "[data-target='#image-modal']", function () 
  {
  var name = $(this).data('value');      
  var image="<img src='http://localhost/ANPR/Images/Scene/"+name+"' width='1000' height='700'>";    
  $("#image-content-modal").html(image);      
  });

</script>