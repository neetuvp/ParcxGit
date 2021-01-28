<?php
$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">APM Cash Level</div>
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
       

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php echo parcxSettings(array("task"=>"38"));?>
          </select>
        </div>


        
        <!-- search -->
        <div class="col-md-1">
          <button type="button" class="btn btn-block btn-secondary" id="view-report-button" onclick="cash_levels()">Search</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div></div></div>
 

  <section class="content">
    <div class="container-wide"> 
      <div class="card">
        <div class="card-body" id="report-content">
          <?php                    
          //$reports->cash_levels([]);
			$data["device"]="";	
			$data["language"] = $_SESSION["language"];
			$data["task"]=18;                          
			echo parcxReport($data);
          ?>

          </div>
        </div>
      </div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
 // $('#view-report-button').click(function (event) { 
 var load_report=0;
 function cash_levels(){
console.log("Button.click");
    var device = $("#deviceNumber").val().toString();
    language = $("#language").val();
      var data = {        
        device: device,
		task:18,
		language:language
      };
      var temp = JSON.stringify(data);
      alert(temp);
      $.post("../../ajax/reports.php", temp)
        .done(function (result) {
alert(result);
          $("#report-content").html(result);
          reportSuccess();
		  load_report=1;
        }, "json");   

    event.preventDefault();
 }
  //}); 

 function loadPage()
  {
  loadheadingreport("cash_levels");
  if(load_report==1)
	  cash_levels(); 
  }
$("#language").change(function(){
  loadPage();
});

$( document ).ready(function() {
	loadheadingreport("cash_levels");
});

$('#export_excel_report').click(function (event) 
  {  
  export_to_excel("#report-content", "APM Cash Level Report")
  }); // end click event function 
  
</script>


