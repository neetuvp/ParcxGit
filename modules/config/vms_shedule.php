<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">VMS Shedule</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="active_vms_shedule">Active VMS Shedule</div>
    <div class="tab-link" data-target="disabled_vms_shedule">Disabled VMS Shedule</div>
    <div class="tab-link" data-target="add_vms_shedule">Add VMS Shedule</div>
    
  </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- add Dynamic Rates -->
<div class="content-wrapper block-data" data-status="add_vms_shedule" style="display:none;">
  <section class="content">
    <div class="container-wide">


      <div class="card card-primary mb-4 col-md-4" id="add-vms-shedule-div">
        <div class="card-body">
          <div class="row">
            <div class="col">
              <div class="form-group">
                <label>VMS Name</label>
                <select id="vms_name" class="form-control">
                <?php
                vmsDevices();
                ?>
                </select>
                <label class="col-form-label">Message</label>
                <textarea id="message" rows="5" class="form-control"></textarea>                  
                <label class="col-form-label">Colour</label>                
                <select id="colour" class="form-control">
                  <option value="white">White</option>
                  <option value="red">Red</option>
                  <option value="green">Green</option>
                  <option value="yellow">Yellow</option>
                  <option value="blue">Blue</option>
                  <option value="pink">Pink</option>
                  <option value="cyan">Cyan</option>
                </select>              
              <label class="col-form-label">Action</label>
              <select id="action" class="form-control">
                <option value="static">Static</option>
                <option value="flashing">Flashing</option>
                <option value="scrolling">Scrolling</option>                
              </select>
              <label class="col-form-label">Shedule Time</label>
              <input type="time" class="form-control" id="sheduleTime" />               
              <label class="col-form-label">Shedule Interval</label>
              <select id="shedule-interval" class="form-control" multiple="multiple">
                <!-- <option value="daily">Daily</option> -->
                 <option value="Sunday">Sunday</option>
                 <option value="Monday">Monday</option>
                 <option value="Tuesday">Tuesday</option>
                 <option value="Wednesday">Wednesday</option>
                 <option value="Thursday">Thursday</option>
                 <option value="Friday">Friday</option>
                 <option value="Saturday">Saturday</option>
              </select>
              <label class="col-form-label">Hold Time(Minutes)</label>
              <input type="number" class="form-control" id="holdTime" />
              <label class="col-form-label">Start date time</label>
              <input type="datetime-local" class="form-control" id="startDateTime" />
              <br>
              <button type="submit" class="btn btn-primary" id="add_vms_shedule">Save</button>
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
<div class="content-wrapper block-data" data-status="active_vms_shedule">
  <section class="content">
    <div class="container-wide">
  
 
<div class="card card-primary mb-4" id="active-rates-table">
  <div class="card-header">
    <div class="d-flex justify-content-between align-items-center">
      <div class="col" >Vms name</div>      
      <div class="col">colour</div>
      <div class="col">Action</div>
      <div class="col-2">Message</div>
      <div class="col">Shedule time</div>
      <div class="col-2">Shedule Interval</div>
      <div class="col">Hold time</div>
      <div class="col">Start date time</div>
      <div class="col"></div>
      <div class="col"></div>
    </div>      
  </div>  
  <?php
  activeVmsShedule();
  ?>
  

</div>
</div>
</section>
</div><!-- /.row -->

<!-- Disabled rates-->
<div class="content-wrapper block-data" data-status="disabled_vms_shedule" style="display:none;">
  <section class="content">
    <div class="container-wide">
   

      <div class="card card-primary mb-4" id="active-rates-table">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
             <div class="col">Vms name</div>
             <div class="col">colour</div>
             <div class="col">Action</div>
             <div class="col-2">Message</div>
             <div class="col">Shedule time</div>
             <div class="col-2">Shedule Interval</div>
             <div class="col">Hold time</div>
             <div class="col">Start date time</div>
            <div class="col"></div>
            
          </div>
        </div>
        <?php
        disabledVmsShedule();
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
        //$('#shedule-interval').multiselect('selectAll');
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