<?php
include('../../includes/header.php');
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Correction Factor</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
  <section class="content">
    <div class="container-wide">

      <div class="card-body p-0">
        <div class="card-header">
          <div class="d-flex justify-content-between align-items-center">
            <div class="col">Carpark name</div>
            <div class="col">Count type</div>
            <div class="col">Total capacity</div>
            <div class="col">Current Level</div>
            <div class="col">Free spaces</div>
            <div class="col">Entries</div>
            <div class="col">Exits</div>
            <div class="col">Occupancy percentage</div>
            <div class="col">Last updated time</div>
            <div class="col">Correction factor</div>
            <div class="col"></div>
          </div>
        </div>
        <?php
         carparkCorrectionFactorEdit();
         ?>
      </div>


    </div>
  </section>
</div>

<?php

include('../../includes/footer.php');
?>