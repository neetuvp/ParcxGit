<?php
require_once('../../../includes/common.php');

$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Parking Duration</div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');

//# App Function Classes
include('../../../classes/reporting_parking.php');
$reports=new reporting_parking();
?>

<div class="content-wrapper">

  <!-- additional menu -->
  <div class="additional-menu row m-0 bg-white border-bottom">
    <div class="col d-flex pl-1 align-items-center">

      <div class="flex-grow-1 row additional-menu-left">

        <!-- carparks multiselect -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
            <?php $reports->get_carparks();?>
          </select>
        </div>

        <!--weekdays -->
        <div class="col-md-2">
          <select id="days" multiple="multiple" class="weekdays">
            <option value="Sunday">Sunday</option>
            <option value="Monday">Monday</option>
            <option value="Tuesday">Tuesday</option>
            <option value="Wednesday">Wednesday</option>
            <option value="Thursday">Thursday</option>
            <option value="Friday">Friday</option>
            <option value="Saturday">Saturday</option>
          </select>
        </div>

        <!-- date and time -->
        <div class="col-md-3">
          <div class="input-group">
            <div class="input-group-prepend">
              <span class="input-group-text"><i class="far fa-clock"></i></span>
            </div>
            <input type="text" class="form-control float-right" id="reservationtime" autocomplete="off" placeholder="Choose Date Range">
          </div>
        </div>

        

        <!-- search -->
        <div class="col-md-1">
        <button type="button" class="btn  btn-secondary" id="view-report-button">View Report</button>
        </div>

        <!-- loader -->
        <div class='col-1' id='loader'>
          <img src='../../../dist/img/loading.gif'>
        </div>

      </div>

      <div class="additional-menu-right">
        <div id="action-buttons">
          <div class="btn-group">
            <button type="button" class="btn btn-warning">Export</button>
            <button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">
              <span class="caret"></span>
              <span class="sr-only">Toggle Dropdown</span>
            </button>
            <div class="dropdown-menu" role="menu">
              <a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>

              <a class="dropdown-item" href="#" id="export_pdf_report">
                Export to PDF
              </a>
            </div>
          </div>
        </div>
      </div>

    </div>
  </div>
  <!-- end / additional menu -->

  <section class="content">
    <div class="container-wide">

      <!-- parking dur chart -->
      <div class="d-none" id="chart_container">
        <div class="row jspdf-graph">
          <div class="col-lg-12">
            <div class="card barchart-box report-chart">
              <div class="position-relative">
                <div><canvas id="duration-chart" height="300"></canvas></div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div>
        <div class="card-body p-0" id="report-content">
        </div>
      </div>

    </div>

</div>
</section>
</div>

<?php include('../../../includes/footer.php');?>

<script>
  //////////////////////////////
  // load report data
  //////////////////////////////

  var click_count = 0;

  $('#view-report-button').click(function (event) {
    var carpark = $("#multiselect").val();

    var allVals = [];
    $('#days :checked').each(function () {
      allVals.push($(this).val());
    });
    if ((!daterange)) {
      alert("choose date range");
    } else {
      var data = {
        toDate: to,
        fromDate: from,
        carpark: carpark,
        weekdays: allVals
      };
      var temp = JSON.stringify(data);
      //alert(temp);
      $.post("../../ajax/parking.php?task=2", temp)
        .done(function (result) {

          $("#report-content").html(result);

          reportSuccess();

          if (result.indexOf("No records/transactions available for the current search criteria") === -1) {

            // show chart when report first loads
            $("#chart_container").removeClass("d-none");

            // clear previous array data
            count_short_term = [];
            count_contract = [];
            duration_labels = [];

            getChartData();

            // for first click
            if (click_count === 0) {

              // load chart
              // note - chart *must* be visible first
              durationChart();

              click_count += 1;

            } else {

              // for all other clicks
              updateDurationChart()

            }
          }

        }, "json");
    } // end if 

    event.preventDefault();

  }); // end traffic report by day 

  //////////////////////////////
  // excel export
  //////////////////////////////

  $('#export_excel_report').click(function (event) {
    export_to_excel("#report-content", "PMS_Parking_Duration")
  }); // end click event function

  //////////////////////////////
  // bar chart - duration
  //////////////////////////////

  var count_short_term = [];
  var count_contract = [];
  var duration_labels = [];

  function getChartData() {

    // save report data for use in chart
    for (dataCount = 0; dataCount <= 26; dataCount++) {
      var temp_var = "chartData_" + dataCount
      var temp1 = $("#" + temp_var).attr('data-short-term');
      var temp2 = $("#" + temp_var).attr('data-contract');
      var temp3 = $("#" + temp_var).attr('data-duration');

      count_short_term.push(temp1);
      count_contract.push(temp2);
      duration_labels.push(temp3);

    }

  }

  function updateDurationChart() {
    // set barchart data
    duration_chart.data.datasets[0].data = count_short_term;
    duration_chart.data.datasets[1].data = count_contract;
    duration_chart.update();
  }

  var duration_chart;

  function durationChart() {

    $(document).ready(function () {

      $(function () {
        var default_data = {
          labels: duration_labels,
          datasets: [{
              data: count_short_term,
              label: 'Short-Term Parkers',

              // transparent bar with normal border
              backgroundColor: "rgba(40,167,69, 0.5)",
              borderColor: '#28a745',
              borderWidth: 1.5,
            },
            {
              data: count_contract,
              label: 'Contract Parkers',

              // transparent bar with normal border
              backgroundColor: "rgba(0,123,255, 0.5)",
              borderColor: '#007bff',
              borderWidth: 1.5,
            },
          ]
        }

        var ticksStyle = {
          fontColor: '#000',
        }

        var mode = 'index'
        var intersect = true

        var $duration_chart = $('#duration-chart')
        duration_chart = new Chart($duration_chart, {
          type: 'bar',

          data: default_data,
          options: {
            maintainAspectRatio: false,
            tooltips: {
              mode: mode,
              intersect: intersect
            },
            hover: {
              mode: mode,
              intersect: intersect
            },
            legend: {
              display: true,
              text: 'test1'
            },
            scales: {
              yAxes: [{
                gridLines: {
                  display: true,
                  lineWidth: '4px',
                  color: 'rgba(0, 0, 0, .2)',
                  zeroLineColor: 'transparent'
                },
                ticks: $.extend({
                  beginAtZero: true,
                }, ticksStyle)
              }],
              xAxes: [{
                display: true,
                gridLines: {
                  display: false
                },
                ticks: ticksStyle
              }]
            },
          },
          plugins: [{
            beforeInit: function (chart) {
              chart.data.labels.forEach(function (e, i, a) {
                // add linebreak where "\n" occurs
                if (/\n/.test(e)) {
                  a[i] = e.split(/\n/);
                }
              });
            }
          }]
        })

      })

    });

  }
</script>