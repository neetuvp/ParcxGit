<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">VMS Device</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="active_vms_device">Active VMS Device</div>
    <div class="tab-link" data-target="disabled_vms_device">Disabled VMS Device</div>
    <div class="tab-link" data-target="add_vms_device">Add VMS Device</div>
    
  </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- add Dynamic Rates -->
<div class="content-wrapper block-data" data-status="add_vms_device" style="display:none;">
  <section class="content">
    <div class="container-wide">
 

      <div class="card card-primary mb-4 col-md-4" id="add-vms-device-div">
        <div class="card-body">
          <div class="row">
            <div class="col">
              <div class="form-group">
                <label>VMS Name</label>                              
              <input type="text" class="form-control" id="vms-name" />               
              <label class="col-form-label">Location</label>              
              <input type="text" class="form-control" id="location" />
              <label class="col-form-label">Port name</label>
              <select class="form-control" id="port-name">
              <?php
              for($i=0;$i<10;$i++)
                echo "<option value='/dev/ttyr0".$i."'>/dev/ttyr0".$i."</option>'";
                for($i=10;$i<16;$i++)
                echo "<option value='/dev/ttyr".$i."'>/dev/ttyr".$i."</option>'";
              ?>
              </select>
              <!-- <input type="text" class="form-control" id="port-name" /> -->
              <label class="col-form-label">Hold status</label> 
              <input type="checkbox" class="form-control" id="hold-status" />
              <br>
              <button type="submit" class="btn btn-primary" id="add_vms_device">Save</button>
              <button type="submit" class="btn btn-danger" id="cancel">Cancel</button>
              </div>
            </div>                        
          </div><!--row-->
       
        </div><!--card body-->
      </div>
    </div>
  </section>
</div>
<!-- End dynamic rates -->

<!-- Active rates-->
<div class="content-wrapper block-data" data-status="active_vms_device">
  <section class="content">
    <div class="container-wide">

 
<div class="card card-primary mb-4" id="active-rates-table">
  <div class="card-header">
    <div class="d-flex justify-content-between align-items-center">
      <div class="col" >Vms name</div>      
      <div class="col">Location</div>
      <div class="col">Port Name</div>
      <div class="col-2">Hold Status</div>      
      <div class="col"></div>
      <div class="col"></div>
    </div>      
  </div>  
  <?php
  activeVmsDevices();
  ?>
  

</div>
</div>
</section>
</div><!-- /.row -->

<!-- Disabled rates-->
<div class="content-wrapper block-data" data-status="disabled_vms_device" style="display:none;">
  <section class="content">
    <div class="container-wide">
    

      <div class="card card-primary mb-4" id="active-rates-table">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
             <div class="col">Vms name</div>
             <div class="col">Location</div>
             <div class="col">Port Name</div>
             <div class="col-2">Hold Status</div>
            <div class="col"></div>
            
          </div>
        </div>
        <?php
       disabledVmsDevices();
        ?>
        
      </div>
    </div>
  </section>
</div><!-- /.row -->
</div>


<?php include('../../includes/footer.php');?>

<script>
  /* Table Show - Hide */

  $(document).ready(function () {
    $('.tab-link').on('click', function () {
      var $target = $(this).data('target');
      if ($target != 'all') {
        $('.block-data').css('display', 'none');
        $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
        
      } else {
        $('.block-data').css('display', 'none').fadeIn('slow');
      }
     
    });
  });

  /* Change Active Tab Styling */

  $('.tab-link').on('click', function () {
    $('.tab-link').removeClass('active');
    $(this).addClass('active');
  });
</script>