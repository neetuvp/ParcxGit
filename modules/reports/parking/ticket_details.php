<?php
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
?>

<div class="content-wrapper">
  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom" >
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">
        
        <!-- devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber">
          <?php echo parcxSettings(array("task"=>"14","type"=>"1,2,3"));?>
          </select>
        </div>
        
        
        
        <!-- plate -->
        <div class="col-md-2">
          <input type="text" id="ticket_id" class="form-control" placeholder="<?=$json["ticket_id"]?>">
        </div>    
        <div class="col-md-2">
          <input type="text" id="plate_number" class="form-control" placeholder="<?=$json["plate_number"]?>">
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
      <div class="col-md-6" id="page-content">               
       
      </div>   
    </div>
  </section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
var body='<div class="card">'+
              '<div class="card-header">'+
                '<h3 class="card-title p-2">Danger Outline</h3>'+
             '</div>'+
              '<div class="card-body"><dl class="row"></dl></div>'+
            '</div>';
function callReport()
{ 
if($("#ticket_id").val()=="" && $("#plate_number").val()=="")
    alert("Please enter ticket id or plate_number");
else
    {
    var data={};            
    data["ticket_id"]=$("#ticket_id").val();   
    data["plate_number"]=$("#plate_number").val();   
    data["device_number"]=$("#deviceNumber").val();   
    data["test"]="1";
    data["language"] = $("#language").val();	
    data["task"]=9;
    var temp = JSON.stringify(data);        
    $.post("../../ajax/reports.php", temp)
      .done(function (result) {           
        $("#page-content").html(body);           
       var json=JSON.parse(result);
       if(json.access_allowed=="true")
           $(".card").addClass("card-success");
       else
           $(".card").addClass("card-danger");
       
       $(".card-title").html(json.result_description);
       
       for( let prop in json ){
            
            $("dl").append("<dt class='col-sm-6'>"+prop+"</dt><dd class='col-sm-6'>"+json[prop]+"</dd>");
        }

      }, "json");


        }
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



</script>