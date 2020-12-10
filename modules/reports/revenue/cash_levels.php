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
include('../../../classes/reporting_revenue.php');
$reports=new reporting_revenue();
?>


<div class="content-wrapper">
 <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">
       

        <!-- payment devices multiselect-->
        <div class="col-md-2">
          <select class="form-control" id="deviceNumber" multiple="multiple">
            <?php $reports->get_apm_devices();?>
          </select>
        </div>


        
        <!-- search -->
        <div class="col-md-1">
          <button type="button" class="btn btn-block btn-secondary" id="view-report-button">Search</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div></div></div>
 

  <section class="content">
    <div class="container-wide"> 
      <div class="card">
        <div class="card-body p-0" id="report-content">
          <?php                    
          $reports->cash_levels([]);
          ?>

          </div>
        </div>
      </div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
  $('#view-report-button').click(function (event) { 
console.log("Button.click");
    var device = $("#deviceNumber").val().toString();
    
      var data = {        
        device: device        
      };
      var temp = JSON.stringify(data);
      //alert(temp);
      $.post("../../ajax/revenue.php?task=16", temp)
        .done(function (result) {

          $("#report-content").html(result);
          reportSuccess();

        }, "json");   

    event.preventDefault();

  }); 

  
  
</script>


