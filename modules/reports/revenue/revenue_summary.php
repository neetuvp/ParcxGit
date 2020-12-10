<?php
$page_title="Application Home";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');

?>

</ul>

<div class="header text-dark" id="pdf-report-header">Revenue Summary</div>

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

        <!-- carparks -->
        <div class="col-md-2">
          <select class="form-control" id="multiselect" multiple="multiple">
            <?php $reports->get_carparks();?>
          </select>
        </div>

        <!--weekdays -->
        <div class="col-md-2">
          <select id="days" multiple="multiple" class="weekdays">
            <option value="'Sunday'">Sunday</option>
            <option value="'Monday'">Monday</option>
            <option value="'Tuesday'">Tuesday</option>
            <option value="'Wednesday'">Wednesday</option>
            <option value="'Thursday'">Thursday</option>
            <option value="'Friday'">Friday</option>
            <option value="'Saturday'">Saturday</option>
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

      <!-- revenue data chart -->
      <div class="d-none" id="chart_container">
        <div class="row jspdf-graph">
          <div class="col-lg-6 pl-0">
            <div class="card barchart-box mt-0">
              <p class="text-center chart-header text-dark justify-content-middle">
                Earnings
              </p>
              <div class="position-relative">
                <div><canvas id="revenue-chart" height="300"></canvas></div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div class="card-body p-0" id="revenue-summary-content">       
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
$('#view-report-button').click(function (event) 
  { 
  if ((!daterange)) 
    {
    alert("choose date range");
    } 
  else 
    {
    var data={};
    data["from"]=from;
    data["to"]=to;         
    data["carpark"]=$("#multiselect").val().toString(); 
    data["weekdays"]=$("#days").val().toString();  
    data["option-type"]=1;   
    var jsondata = JSON.stringify(data);    
    console.log(jsondata); 
    $.post("../../ajax/reports-ajax.php",jsondata,function(data)
      {		
      $("#revenue-summary-content").html(data);
      reportSuccess();
      createChart();	
      })
    .fail(function(jqxhr,status,error)
      {
      alert("Error: "+error);
      });  
    
  } // end if 

  event.preventDefault();
});

  //////////////////////////////
  // excel export
  //////////////////////////////

$('#export_excel_report').click(function (event) 
  {
  export_to_excel("#revenue-summary-content", "PMS_Revenue_Summary")  
  }); // end click event function

//////////////////////////////
// notification accordion box
//////////////////////////////

// change icon ui based on whether box is open/closed
$(document).on("click", ".notification-box .card-header", function () {
  // $('.notification-box .card-header').click(function (event) {
  $("#notification-icon").toggleClass("fa-angle-down fa-angle-up");
});

//////////////////////////////
// revenue earnings chart
//////////////////////////////

var parking_fee;
var lost_ticket_fee;
var product_sale_amount;
var vat_amount;
var currency;

function createChart() 
  {
  if($("#revenue-summary-content").find('table').length!=0) 
    {    
    $("#chart_container").removeClass("d-none");
    parking_fee = 0;
    lost_ticket_fee = 0;
    product_sale_amount = 0;
    vat_amount = 0;
    getChartData();
    if (click_count === 0) 
      {            
      revenueEarningsChart();
      click_count += 1;
      } 
    else 
      {      
      updateRevenueEarningsChart()
      }
    } 
  else 
    {
    $("#chart_container").addClass("d-none");
    }
  }

function getChartData() 
  {
  parking_fee = $("#chart-data").attr('data-parking-fee');
  lost_ticket_fee = $("#chart-data").attr('data-lost-ticket');
  product_sale_amount = $("#chart-data").attr('data-product-sale');
  //vat_amount = $("#chart-data").attr('data-vat');
  currency=$("#chart-data").attr('data-currency');   
  }

function updateRevenueEarningsChart() 
  {  
  revenue_earnings_chart.data.datasets[0].data[0] = parking_fee;
  revenue_earnings_chart.data.datasets[0].data[1] = lost_ticket_fee;
  revenue_earnings_chart.data.datasets[0].data[2] = product_sale_amount;
  //revenue_earnings_chart.data.datasets[0].data[3] = vat_amount;
  revenue_earnings_chart.update();
  }

var revenue_earnings_chart

function revenueEarningsChart() 
  {
  $(document).ready(function () 
    {
    $(function () 
      {
      var default_data = 
        {
        labels: 
          [
          'Parking Fee',
          'Lost Ticket Fee',
          'Product Sales',
          //'VAT'
          ],
        datasets: 
            [
              {
              data: 
                [
                parking_fee,
                lost_ticket_fee,
                product_sale_amount,
                //vat_amount,
                ],
              borderWidth: 1.5,
              backgroundColor: 
                [
                //"rgba(40, 167, 69, 0.5)",
                "#0bc581",
                "#36bea9",
                "#3cb767",
                //"rgba(0, 123, 255, 0.5)",
                //"rgba(69, 40, 167, 0.5)",
                //"rgba(11, 79, 108, 0.5)",
                ],
            borderColor: 
              [
              "rgba(40, 167, 69, 0.8)",
              "rgba(40, 167, 69, 0.8)",
              "rgba(40, 167, 69, 0.8)",
              //"rgba(11, 79, 108, 0.8)",
              ],
            }, 
          ]
        }

      var ticksStyle = 
        {
        fontColor: '#000',
        }

        var mode = 'index'
        var intersect = true

        var $revenue_earnings_chart = $('#revenue-chart')
        revenue_earnings_chart = new Chart($revenue_earnings_chart, {
          type: 'bar',

          data: default_data,
          options: {
            layout: {
              padding: {
                // add padding so text on tallest bar doesn't get cut off
                top: 25
              }
            },
            legend: {
              display: false
            },
            maintainAspectRatio: false,
            tooltips: {
              mode: mode,
              intersect: intersect
            },
            hover: {
              mode: mode,
              intersect: intersect
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

        // Define a plugin to provide data labels
        Chart.plugins.register({
          afterDatasetsDraw: function (chart) {
            var ctx = chart.ctx;

            chart.data.datasets.forEach(function (dataset, i) {
              var meta = chart.getDatasetMeta(i);
              if (!meta.hidden) {
                meta.data.forEach(function (element, index) {
                  // Draw the text in black, with the specified font
                  ctx.fillStyle = 'rgb(0, 0, 0)';

                  var fontSize = 16;
                  var fontStyle = 'normal';
                  var fontFamily = 'Helvetica Neue';
                  ctx.font = Chart.helpers.fontString(
                    fontSize, fontStyle, fontFamily
                  );

                  // Just naively convert to string for now
                  var dataString = currency+" "  + dataset.data[
                    index].toString();

                  // Make sure alignment settings are correct
                  ctx.textAlign = 'center';
                  ctx.textBaseline = 'middle';

                  var padding = 5;
                  var position = element.tooltipPosition();
                  ctx.fillText(dataString, position.x,
                    position.y - (fontSize / 2) -
                    padding);
                });
              }
            });
          }
        });

      }) // end func

    }); // end doc ready

  } // end chart func
</script>