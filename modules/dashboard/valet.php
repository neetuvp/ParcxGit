<?php 
$page_title="Valet Keys";

require_once ('../../includes/common.php');

include ('../../includes/header.php');
?>

<div class="navbar-has-tablink">

<?php
include ('../../includes/navbar-start.php');

include ('../../classes/reporting_valet.php');
$reports=new reporting_valet();
?>

    </ul>

    <div class="header text-dark" id="pdf-report-header">Valet Dashboard</div>

    <div class="row">
        <div class="col tab-header d-flex justify-content-center">
            <div class="tab-header-link active" data-target="overview">Overview</div>
            <div class="tab-header-link" data-target="keymanager">Key Manager</div>
        </div>
    </div>

    <?php 
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

    <div class="content-wrapper">

        <!-- tab-link header -->
        <div class="additional-menu row m-0 bg-white border-bottom">
            <div class="col d-flex justify-content-between pl-1 align-items-center">

                <div class="flex-grow-1 row additional-menu-left">

                    <div class="nav-details">
                        <div class="nav-details">
                            <span class="text-bold m-0">
                                <!--Service Updated Date and Time:-->
                            </span>
                            <span id="last-updated">
                            </span>
                        </div>
                    </div>

                </div>

                <div class="additional-menu-right row align-items-center">

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
        <!-- end / tab-link header -->

        <section class="content">
            <div class="container-wide">

                <div class="col-md-12 table-view">

                    <div class="row">
                        <div class="col-md-12">

                            <!-- overivew tab -->
                            <div data-status="overview">

                                <div id="valet_overview">
                                    <?php $reports->get_valet_counters();?>
                                </div>

                                <!-- graph content -->

                                <!-- revenue -->
                                <div class="row jspdf-graph">

                                    <!-- rev barchart -->
                                    <div class="col-lg-8 pl-0 pr-3">
                                        <div class="card barchart-box mt-0 h-100">
                                            <p class="text-center chart-header text-dark">Revenue - Last 7 Days</p>
                                            <div class="">
                                                <div class="position-relative">
                                                    <canvas id="revenue-7days" height="269"></canvas>
                                                </div>
                                            </div>

                                        </div>
                                    </div>
                                    <!-- end / rev chart -->

                                    <!-- donut chart - revenue sources -->
                                    <div class="col-lg-4 pr-0">
                                        <div class="card barchart-box mt-0" id="parking-duration">
                                            <p class="text-center chart-header text-dark">Revenue Sources</p>
                                            <div class="">
                                                <div class="position-relative">
                                                    <canvas id="revenue_sources_chart" height="175"></canvas>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <!-- end / donut chart -->

                                </div>

                                <!-- occupancy -->
                                <!--<div class="row jspdf-graph">
                                    <div class="col-lg-12">
                                        <div class="card barchart-box">

                                            <nav class="navbar navbar-light navbar-expand-md bg-faded justify-content-center p-0">
                                                <a href="/" class="d-flex w-50 mr-auto"></a>

                                                <div class="navbar-collapse w-100" id="collapsingNavbar3">

                                                    <ul class="navbar-nav w-100 justify-content-center">
                                                        <p class="text-center chart-header text-dark justify-content-middle">
                                                            Hourly Occupancy -
                                                            <span id="current-carpark">All Carparks</span>
                                                        </p>
                                                    </ul>

                                                    <div class="w-100 row m-0">
                                                        <div class="col-4 ml-auto p-0">
                                                            <select class="form-control" id="linechart-select">                                                                
                                                                 <option value="0">Last week's average</option>
                                                <option value="1">All-time average</option> 
                                                                <option value="0">This day last week</option>

                                                            </select>
                                                        </div>
                                                        <div class="col-4 ml-3 m-0 p-0">
                                                            <select class="form-control" id="carpark-select">                                                                
                                                                <option value="0">All Carparks</option>
                                                                <?php $reports->get_carparks();?>
                                                            </select>
                                                        </div>
                                                    </div>

                                                </div>
                                            </nav>

                                            <div class="">
                                                <div class="position-relative">
                                                    <canvas id="hourly-occ-chart" height="300"></canvas>
                                                </div>
                                            </div>

                                        </div>
                                    </div>
                                </div>-->

                                <!-- / end graph content -->

                                <!-- table content -->
                                <div id="valet_overview" class="row mt-4">

                                    <!-- table 1 -->
                                    <div class="col-md-4 table-scroll">

                                        <div class="card-body p-0">
                                            <table width="100%" class="jspdf-table">
                                                <tbody>
                                                    <tr class="card-header d-flex justify-content-between align-items-center">
                                                        <th class="col">Cashier</th>
                                                        <th class="col">Amount</th>
                                                    </tr>
                                                </tbody>
                                                <tbody class="table-view">
                                                    <?php
                                            $reports->amount_collected_by_cashier();
                                            ?>

                                                </tbody>
                                            </table>
                                        </div>

                                    </div>

                                    <!-- table 2 -->
                                    <div class="col-md-4 table-scroll">

                                        <div class="card-body p-0">
                                            <table width="100%" class="jspdf-table">
                                                <tbody>
                                                    <tr class="card-header d-flex justify-content-between align-items-center">
                                                        <th class="col">Device</th>
                                                        <th class="col">Valet In</th>
                                                        <!--<th class="col">Valet Out</th>-->
                                                    </tr>

                                                </tbody>
                                                <tbody class="table-view">
                                                    <?php
                                            $reports->valet_by_device();
                                            ?>
                                                </tbody>
                                            </table>
                                        </div>

                                    </div>

                                    <!-- table 3 -->
                                    <div class="col-md-4 table-scroll">

                                        <div class="card-body p-0">
                                            <table width="100%" class="jspdf-table">
                                                <tbody>
                                                    <tr class="card-header d-flex justify-content-between align-items-center">
                                                        <th class="col">Driver</th>
                                                        <th class="col">Key In</th>
                                                        <th class="col">Key Out</th>
                                                    </tr>
                                                </tbody>
                                                <tbody class="table-view">
                                                    <?php
                                                    $reports->keyinout_by_drivers();
                                                    ?>
                                                </tbody>
                                            </table>
                                        </div>

                                    </div>

                                </div>

                            </div>

                            <!-- key manager tab -->
                            <div data-status="keymanager" class="hidden">
                                <div id="valet_dashboard">
                                    <?php $reports->valet_parking_keydashboard();?>
                                </div>
                            </div>

                        </div>
                    </div>

                </div>

        </section>
    </div>
</div>


<script src="../../plugins/gauge/raphael-2.1.4.min.js"></script>
<script src="../../plugins/flot/jquery.flot.min.js"></script>
<script src="../../plugins/flot/jquery.flot.categories.min.js"></script>
<script src="../../dist/js/gauge.js"></script>
<script src="../../dist/js/dashboard.js"></script>

<!-- add moment.js -->
<script src="../../plugins/jquery/moment.min.js"></script>

<!-- save day_closure_start as js variable for use in graphs -->
<div id="dom-target" style="display: none;">
    <?php 
        $date = DAY_CLOSURE_START;
        echo htmlspecialchars($date); /* You have to escape because the result will not be valid HTML otherwise */
    ?>
</div>

<?php include('../../includes/footer.php');?>

<script>


    /* Table Show - Hide */

    $(document).ready(function () {

        $('.tab-link').on('click', function () {
            var $target = $(this).data('target');
            if ($target != 'all') {

                $('.card-text').addClass("hidden");
                $('.card-text[data-status="header"]').removeClass("hidden");

                // $('.card-text').closest('.card-text[data-status="header"]').removeClass("hidden").fadeIn('slow').addClass("moi");

                $('.card-text[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');
            } else {
                // all
                $('.card-text').removeClass("hidden").fadeIn('slow');
                $('.card-text[data-status="header"]').removeClass("hidden").fadeIn('slow');
                console.log('else');
            }
        });

        $('.tab-header-link').on('click', function () {
            var $target = $(this).data('target');

            if ($target === 'keymanager') {

                $('[data-status="overview"]').addClass("hidden");
                $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');

            } else {

                $('[data-status="keymanager"]').addClass("hidden");
                $('[data-status="' + $target + '"]').removeClass("hidden").fadeIn('slow');

            }
        });

        addPagination();
    });

    function addPagination() {

        pageSize = 12;

        var pageCount = $(".keybox").length / pageSize;

        for (var i = 0; i < pageCount; i++) {
            $(".pagination").append('<li class="page-item"><a class="page-link" href="#">' + (i + 1) + '</a></li> ');
        }
        $(".pagination li").first().addClass("active")
        showPage = function (page) {
            $(".keybox").hide();
            $(".keybox").each(function (n) {
                if (n >= pageSize * (page - 1) && n < pageSize * page) $(this).show();
            });
        }
        showPage(1);
        $(".pagination li").click(function () {
            $(".pagination li").removeClass("active");
            $(this).addClass("active");
            showPage(parseInt($(this).text()))
        });

        // console.log("added pagination")
    }

    function ValetDashboardUpdate() {

        $.get("../ajax/dashboard.php?task=6", function (data) {
            $('#valet_dashboard').html(data);

            // keymanager pagination
            addPagination();
        });

        $.get("../ajax/dashboard.php?task=11", function (data) {
            $('#valet_overview').html(data);
        });

    }

    /* Change Active Tab Styling */

    $('.tab-link').on('click', function () {
        $('.tab-link').removeClass('active');
        $(this).addClass('active');
    });

    $('.tab-header-link').on('click', function () {
        $('.tab-header-link').removeClass('active');
        $(this).addClass('active');
    });

    $('#export_excel_report').click(function (event) {
        export_to_excel("#live-revenue-summary-content", "Live_Revenue_Report")
    }); // end click event function

    /* begin graphs */

    //////////////////////////////
    // bar chart - hourly occupancy
    //////////////////////////////

    var hourly_occ_chart

    function updateHourlyOccupancy() {

        // reset barchart data
        hourly_occ_chart.data.datasets[0].data = getoccupancy("Min");
        hourly_occ_chart.data.datasets[1].data = getoccupancy("Max");

        // reset linechart data
        hourly_occ_chart.data.datasets[2].data = getaverageoccupancy();

        hourly_occ_chart.update();

    }

    function getoccupancy(type) {
        var tmp = [];
        var dt = {};
        dt['carpark'] = $('#carpark-select').find("option:selected").val();
        dt['type'] = type;

        var jsondata = JSON.stringify(dt);
        $.ajax({
            type: 'post',
            url: "../../modules/ajax/hourlyoccupancy.php?type=1",
            data: jsondata,
            cache: false,
            async: false,
            contentType: 'application/json',
            dataType: 'json',
            success: function (data) {

                for (var i = 0; i < data.length; i++) {
                    tmp.push(data[i]);
                    // console.log("max = " + maxdata)
                }
            },
            error: function (jqXHR, textStatus, errorThrown) {
                //alert("textStatus" + textStatus);
                alert(errorThrown + " " + textStatus);
            }
        });
        return tmp;
    }

    function getaverageoccupancy() {
        var tmp = [];
        var dt = {};
        dt['carpark'] = $('#carpark-select').find("option:selected").val();
        dt['seloption'] = $('#linechart-select').find("option:selected").val();

        var jsondata = JSON.stringify(dt);
        $.ajax({
            type: 'post',
            url: "../../modules/ajax/hourlyoccupancy.php?type=2",
            data: jsondata,
            cache: false,
            async: false,
            contentType: 'application/json',
            dataType: 'json',
            success: function (data) {
                for (var i = 0; i < data.length; i++) {
                    tmp.push(data[i]);
                    // console.log("max = " + maxdata)
                }
            },
            error: function (jqXHR, textStatus, errorThrown) {
                //alert("textStatus" + textStatus);
                alert(errorThrown + " " + textStatus);
            }
        });
        return tmp;
    }

    function hourlyOccupancy() {

        // set up multiselect

        $(document).ready(function () {

            $('#carpark-select').multiselect({
                buttonWidth: '100%',
                includeSelectAllOption: true,
                selectAllText: "null",
                nonSelectedText: 'null',
                onChange: function (element, checked) {

                    // change header text to current carpark

                    var currentCarpark = $('#carpark-select').find("option:selected").text()
                    $("#current-carpark").html(currentCarpark);
                    // console.log("currentCarpark = " + currentCarpark);
                    // console.log("changed");

                    // reset barchart data
                    hourly_occ_chart.data.datasets[0].data = getoccupancy("Min");
                    hourly_occ_chart.data.datasets[1].data = getoccupancy("Max");

                    hourly_occ_chart.update();

                }
            });

            $('#linechart-select').multiselect({
                buttonWidth: '100%',
                includeSelectAllOption: true,
                selectAllText: "null",
                nonSelectedText: 'null',
                onChange: function (element, checked) {

                    hourly_occ_chart.data.datasets[2].data = getaverageoccupancy();

                    hourly_occ_chart.update();

                }
            });

            var maxdata = [];
            var mindata = [];

            /* occupancy data */
            maxdata = getoccupancy("Max");
            mindata = getoccupancy("Min");

            $(function () {
                'use strict'

                var default_data = {
                    labels: hours_label_reorder,
                    datasets: [{
                            data: mindata,
                            label: 'Minimum',

                            // transparent bar with normal border
                            backgroundColor: "rgba(40,167,69, 0.5)",
                            borderColor: '#28a745',
                            borderWidth: 1.5,
                        },
                        {
                            data: maxdata,
                            label: 'Maximum',

                            // transparent bar with normal border
                            backgroundColor: "rgba(0,123,255, 0.5)",
                            borderColor: '#007bff',
                            borderWidth: 1.5,
                        },
                        {
                            type: 'line',
                            label: "This day last week",
                            borderColor: '#28a745',
                            borderWidth: 2,
                            fill: false,
                            data: getaverageoccupancy()
                        }
                    ]
                }

                /* end datasets */

                var ticksStyle = {
                    fontColor: '#000',
                    // fontStyle: 'bold'
                }

                var mode = 'index'
                var intersect = true

                var $hourly_occ_chart = $('#hourly-occ-chart')
                hourly_occ_chart = new Chart($hourly_occ_chart, {
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
                                // scaleLabel: {
                                //   display: true,
                                //   labelString: 'Number of Cars',
                                //   fontStyle: 'inherit',
                                // },
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
                        // animation: false,
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

    hourlyOccupancy();

    //////////////////////////////
    // donut chart - revenue sources
    //////////////////////////////

    var config
    var ctx
    var pieChart
    var pieValues

    function updateRevenueSources() {
        $.get("../ajax/dashboard.php?task=10", function (data) {

            amount = JSON.parse(data);

            pieValues[0] = amount["parking_fee"];
            pieValues[1] = amount["lost_fee"];
            pieValues[2] = amount["product_sale_amount"]

            // console.log("pV = " + pieValues);

            pieChart.update();

        });
    }

    function revenueSources() {

        // alert("revs");

        var pieChartCanvas = $("#revenue_sources_chart").get(0).getContext("2d");
        pieChart = new Chart(pieChartCanvas, {
            type: 'doughnut',
            data: {
                datasets: [{
                    data: [],
                    backgroundColor: [
                        "rgba(40, 167, 69, 0.5)",
                        "rgba(0, 123, 255, 0.5)",
                        "rgba(113, 128, 172, 0.5)"
                    ],
                    borderColor: [
                        "#28a745",
                        "#007bff",
                        "#7180ac"
                    ],
                    label: 'Dataset 1'
                }],
                labels: [
                    'Parking Fee',
                    'Lost Fee',
                    'Product Sale'
                ]
            },
            options: {
                responsive: true,
                legend: {
                    position: 'top',
                },
                animation: {
                    animateScale: true,
                    animateRotate: true
                }
            }
        })

        // get dynamic data

        var parking_fee, vat_amount, lost_fee, amount;
        pieValues = pieChart.data.datasets[0].data

        updateRevenueSources();

        // edits to add margin between legend and chart

        function getBoxWidth(labelOpts, fontSize) {
            return labelOpts.usePointStyle ?
                fontSize * Math.SQRT2 :
                labelOpts.boxWidth;
        };

        Chart.NewLegend = Chart.Legend.extend({
            afterFit: function () {
                this.height = this.height + 10;
            },
        });

        function createNewLegendAndAttach(chartInstance, legendOpts) {
            var legend = new Chart.NewLegend({
                ctx: chartInstance.chart.ctx,
                options: legendOpts,
                chart: chartInstance
            });

            if (chartInstance.legend) {
                Chart.layoutService.removeBox(chartInstance, chartInstance.legend);
                delete chartInstance.newLegend;
            }

            chartInstance.newLegend = legend;
            Chart.layoutService.addBox(chartInstance, legend);
        }

        // Register the legend plugin
        Chart.plugins.register({
            beforeInit: function (chartInstance) {
                var legendOpts = chartInstance.options.legend;

                if (legendOpts) {
                    createNewLegendAndAttach(chartInstance, legendOpts);
                }
            },
            beforeUpdate: function (chartInstance) {
                var legendOpts = chartInstance.options.legend;

                if (legendOpts) {
                    legendOpts = Chart.helpers.configMerge(Chart.defaults.global.legend, legendOpts);

                    if (chartInstance.newLegend) {
                        chartInstance.newLegend.options = legendOpts;
                    } else {
                        createNewLegendAndAttach(chartInstance, legendOpts);
                    }
                } else {
                    Chart.layoutService.removeBox(chartInstance, chartInstance.newLegend);
                    delete chartInstance.newLegend;
                }
            },
            afterEvent: function (chartInstance, e) {
                var legend = chartInstance.newLegend;
                if (legend) {
                    legend.handleEvent(e);
                }
            }
        });

    }

    revenueSources();

    //////////////////////////////
    // bar chart - revenue last 7 days
    //////////////////////////////

    var rev_7_days
    var $result

    function rev7days() {
        var $rev_7_days
        var revenue_amounts = {};
        var weekdays = [];

        $.get("../ajax/revenue.php?task=12", function (data) {

            // console.log(data);
            // get dynamic data

            /* chart variables */

            var weekdates = [];
            var date_string = [];
            var test = 0

            $result = JSON.parse(data);
            console.log($result);

            function addDates() {
                if (test <= 7) {
                    var temp1 = moment().subtract(test, 'days').format("MM-DD");
                    weekdates.unshift(temp1);

                    var temp2 = moment().subtract(test, 'days').format("ddd");
                    weekdays.unshift(temp2);
                    // console.log(temp2);

                    // used for comparing to $result
                    var temp3 = moment().subtract(test, 'days').format("YYYY-MM-DD");
                    date_string.unshift(temp3);

                    test += 1;

                    addDates();
                }
            }

            addDates()

            // console.log(date_string);

            // get revenue amounts
            // if ($result.length !== 7) {
            //     for (var i in weekdays) {
            //         for (var j in $result) {
            //             if ($result[j].day === weekdays[i]) {
            //                 revenue_amounts[weekdays[i]] = $result[j].amount;
            //             }
            //         }
            //         // add data for missing day
            //         if (revenue_amounts[weekdays[i]] === undefined) {
            //             revenue_amounts[weekdays[i]] = "0";
            //             // console.log(weekdays[i] + " is undefined");
            //         } else {
            //             // console.log(weekdays[i] + " is defined");
            //         }
            //     }
            // }

            // create label

            var daylabels = [];
            var revenue_data = [];

            for (var i in weekdays) {
                if (i < 7) {
                    daylabels[i] = (weekdays[i] + "\n" + weekdates[i]);
                } else {
                    daylabels[i] = ("Today" + "\n" + weekdays[i] + " " + weekdates[i]);
                }
            }

            // console.log(weekdates);

            // put revenue data in array
            for (var i in date_string) {
                for (var j in $result) {
                    if (date_string[i] === $result[j].date) {
                        revenue_data[i] = $result[j].amount
                    }
                }
            }

            // console.log("revenue_data = " + revenue_data);

            $(function () {
                'use strict'

                var ticksStyle = {
                    fontColor: '#000',
                    // fontStyle: 'bold'
                }

                var mode = 'index'
                var intersect = true

                // console.log("revd = " + revenue_data);

                $rev_7_days = $('#revenue-7days')
                rev_7_days = new Chart($rev_7_days, {
                    type: 'bar',
                    // type: 'horizontalBar',
                    data: {
                        labels: daylabels,
                        datasets: [{
                            data: revenue_data,
                            label: 'Revenue',

                            // transparent bar with normal border
                            // backgroundColor: "rgba(40,167,69, 0.5)",
                            // borderColor: '#28a745',

                            backgroundColor: [
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(40,167,69, 0.5)",
                                "rgba(69,40,167, 0.5)"
                            ],

                            borderColor: [
                                '#28a745',
                                '#28a745',
                                '#28a745',
                                '#28a745',
                                '#28a745',
                                '#28a745',
                                '#28a745',
                                '#4528a7',
                            ],

                            borderWidth: 2,
                        }]
                    },
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
                            display: false
                        },
                        scales: {
                            yAxes: [{
                                // display: false,
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
                                ticks: $.extend({
                                    beginAtZero: true,
                                }, ticksStyle)
                            }]
                        },
                        animation: {
                            duration: 1,
                            // onComplete: function () {
                            //     var titles = [" ", " ", " ", " ", " ", " ", " ", "Today"];
                            //     // alert("onComplete");
                            //     var chartInstance = this.chart,
                            //         ctx = chartInstance.ctx;
                            //     ctx.textAlign = 'center';
                            //     ctx.fillStyle = "rgba(0, 0, 0, 1)";
                            //     ctx.textBaseline = 'bottom';

                            //     this.data.datasets.forEach(function (dataset, i) {
                            //         var meta = chartInstance.controller.getDatasetMeta(
                            //             i);
                            //         meta.data.forEach(function (bar, index) {
                            //             ctx.fillText(titles[index], bar._model
                            //                 .x,
                            //                 bar._model
                            //                 .y - 5);

                            //         });
                            //     });
                            // }
                        }
                    },
                    // plugin to add line break for long label
                    plugins: [{
                        beforeInit: function (chart) {
                            chart.data.labels.forEach(function (e, i, a) {
                                // add linebreak where "\n" occurs
                                if (/\n/.test(e)) {
                                    a[i] = e.split(/\n/);
                                }
                            });
                        }
                    }],
                })

            })

        });
    }

    rev7days();

    //////////////////////////////
    // update data settimeout
    //////////////////////////////

    setInterval(function () {

        // graphs
        updateHourlyOccupancy();

        // keymanager
        ValetDashboardUpdate();

    }, 10000);
    //
</script>