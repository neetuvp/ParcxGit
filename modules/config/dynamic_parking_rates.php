<?php 
$page_title="Device Dashboard";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>
<div class="header text-dark" id="pdf-report-header">Dynamic Parking Rate</div>
<div class="row">
  <div class="col tab-header d-flex justify-content-center">
    <div class="tab-link active" data-target="active_rates">Active Parking Rates</div>
    <div class="tab-link" data-target="disabled_rates">Disabled Parking Rates</div>
    <div class="tab-link" data-target="add-rate">Add Parking Rates</div>
    
  </div>
</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- add Dynamic Rates -->
<div class="content-wrapper block-data" data-status="add-rate" style="display:none;">
  <section class="content">
    <div class="container-wide">
  

      <div class="card card-primary mb-4 col-md-4" id="add-rate-div">
        <div class="card-body">
          <div class="row">
            <div class="col">
              <div class="form-group">
                <label>Parking Rate Name</label>
                <input type="text" class="form-control" id="dynamicRateName" placeholder="Dynamic Rate Name">
                <input type="radio" name="rdbDate" value="date" id="rdbDate" checked="checked">
                <label class="col-form-label">Date</label>
                <input type="date" class="form-control" id="dynamicRateDate" />
                <input type="radio" name="rdbDate" value="day" id="rdbDay">
                <label class="col-form-label">Day</label>
                <select id="dynamicRateDay" class="form-control">
                  <option value="Sunday">Sunday</option>
                  <option value="Monday">Monday</option>
                  <option value="Tuesday">Tuesday</option>
                  <option value="Wednesday">Wednesday</option>
                  <option value="Thursday">Thursday</option>
                  <option value="Friday">Friday</option>
                  <option value="Saturday">Saturday</option>
                </select>              
              
              <input type="radio" name="rateType" value="2" id="rdbFixedRate" checked="checked">
              <label class="col-form-label">Fixed Rate</label>              
              <input type="radio" name="rateType" value="1" id="rdbVariableRate">
              <label class="col-form-label">Variable Rate</label>
              <br>
              <label class="col-form-label">Parking Rate</label>
              <select id="dynamicRate" class="form-control">
                <?php
                parkingRatesLabels();
                ?>
              </select>
              <br>
              <button type="submit" class="btn btn-primary" id="add_dynamic_rate">Add Dynamic Rate</button>
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
<div class="content-wrapper block-data" data-status="active_rates">
  <section class="content">
    <div class="container-wide">

 
<div class="card card-primary mb-4" id="active-rates-table">
  <div class="card-header">
    <div class="d-flex justify-content-between align-items-center">
      <div class="col" >Parking Rate Name</div>
      <div class="col">Date</div>
      <div class="col">Day</div>
      <div class="col">Rate Type</div>
      <div class="col">Parking Rate</div>
      <div class="col"></div>
    </div>      
  </div>  
  <?php
  activeDynamicParkingRates();
  ?>
  

</div>
</div>
</section>
</div><!-- /.row -->

<!-- Disabled rates-->
<div class="content-wrapper block-data" data-status="disabled_rates" style="display:none;">
  <section class="content">
    <div class="container-wide">    
      <div class="card card-primary mb-4" id="active-rates-table">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
            <div class="col">Parking Rate Name</div>
            <div class="col">Date</div>
            <div class="col">Day</div>
            <div class="col">Rate Type</div>
            <div class="col">Parking Rate</div>
            <div class="col"></div>
          </div>
        </div>
        <?php
        disabledDynamicParkingRates();
        ?>
      </div>
    </div>
  </section>
</div><!-- /.row -->
</div>

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
<?php include('../../includes/footer.php');?>