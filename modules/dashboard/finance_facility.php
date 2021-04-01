<?php 
include('../../includes/header.php');
?>

<div class="navbar-has-tablink navbar-has-sublink">

<?php
include('../../includes/navbar-start.php');
?>

</ul>

<div class="header text-dark" id="pdf-report-header">Finance</div>

<?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

    <div class="content-wrapper">

        <section class="content">
            <div class="container-wide">
            <div class="row" id="device-status-block" >
                
                <div class="col-md-4">
                    <div class="card ">
                        <div class="card-body box-profile">
                            <h3 class="profile-username text-center">Training Area</h3>
                            <p class="text-muted text-center">200005</p>                                                                
                                <!--<canvas id="revenue_sources_chart" ></canvas>-->
                                <canvas id="donutChart" style="min-height: 250px; height: 250px; max-height: 250px; max-width: 100%;"></canvas>
                                
                            <ul class="list-group list-group-unbordered mb-3 mt-3">                                
                                <li class="list-group-item"><b>Total payment devices</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Total transactions</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Gross Amount</b> <a class="float-right">100 AED</a></li>
                                <li class="list-group-item"><b>Parking Fee</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Lost fee</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Product sale</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Vat amount</b> <a class="float-right">6</a></li>
                                <li class="list-group-item"><b>Discount amount</b> <a class="float-right">6</a></li>
                            </ul>
                            <button type="button" class="show-facility-details btn btn-block bg-secondary-gradient" facility_number="200005">More <i class="fa fa-arrow-circle-right"></i></button>
                        </div>
                    </div>
                </div>
                
            </div> 

            </div>
        </section>
    </div>
</div>

<?php include('../../includes/footer.php');?>

<script>
    
//////////////////////////////
// donut chart - revenue sources
//////////////////////////////

var donutChart;
var donutChartValues;
show_donut_chart();

function show_donut_chart()
{
  var donutChartCanvas = $('#donutChart').get(0).getContext('2d')
    var donutData        = {
      labels: [
          'Parking Fee',           
          'Lost Fee',                   
          'Product Sale', 
          
      ],
      datasets: [
        {
          data: [700,500,400],
          backgroundColor : ['#00a65a','#f56954', '#f39c12'],
        }
      ]
    }
    var donutOptions     = {
      maintainAspectRatio : false,
      responsive : true,
    }
    //Create pie or douhnut chart
    
    donutChart = new Chart(donutChartCanvas, {
      type: 'doughnut',
      data: donutData,
      options: donutOptions      
    });
    
    donutChartValues = donutChart.data.datasets[0].data;
    updateRevenueSources() ;

}

function updateRevenueSources() 
    {
    $.get("../ajax/dashboard.php?task=10", function (data) 
        {
        amount = JSON.parse(data);
        donutChartValues[0] = amount["parking_fee"];
        donutChartValues[1] = amount["lost_fee"];
        donutChartValues[2] = amount["product_sale_amount"];        
        donutChart.update();        
        });
    }

</script>