<?php include('includes/header.php'); ?>
<div class="header text-dark" id="pdf-report-header">Validate Ticket</div>
<img class="img-fluid hidden-lg-up" src="dist/img/parcx-cloud-dark.jpg">
<?php include('includes/sidebar.php'); ?>

<!-- page-specific styles -->
<link href="dist/css/shop-validation.css" rel="stylesheet">

<!-- modals -->

<!-- add ticket number -->
<div class="modal fade" id="addTkt" tabindex="-1" role="dialog" aria-labelledby="" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content card-outline card-danger">
            <div class="modal-header">
                <h5 class="modal-title">Add Ticket Number</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <p>Please enter a ticket number.</p>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
            </div>
        </div>
    </div>
</div>

<!-- choose product -->
<div class="modal fade" id="chooseProduct" tabindex="-1" role="dialog" aria-labelledby="" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content card-outline card-danger">
            <div class="modal-header">
                <h5 class="modal-title">Choose Validation Product</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <p>Please choose a validation product.</p>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
            </div>
        </div>
    </div>
</div>

<!-- confirm validation -->
<div class="modal fade" id="confirmValidation" tabindex="-1" role="dialog" aria-labelledby="" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content card-outline card-success">
            <div class="modal-header">
                <h5 class="modal-title">Confirm Validation</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <p>Please confirm that you want to validate ticket number <span id="tktNum"></span> for <span
                        id="valProd"></span>.</p>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-dismiss="modal">Cancel</button>
                <button type="button" class="btn btn-primary" id="confValidate" data-dismiss="modal">Confirm</button>
            </div>
        </div>
    </div>
</div>

<div class="content-wrapper">

    <!-- mobile tab menu -->
    <div class="mobile-additional-menu hidden-lg-up validation-shop-menu">
        <p class="header">Validation</p>
        <p><?php echo $_SESSION["operator_name"];?></p>
    </div>

    <section class="content">
        <div class="container-wide">

            <div class="shop-validation">
                <div class="row">
                    <div class="validation-counter">
                        <div class="device">

                            <!-- status icons -->
                            <div class="row d-flex justify-content-between mb-2">
                                <div class="col-auto">
                                    <div class="icon-container mb-3">
                                        <i class="fas fa-circle status-icon icon-success"></i>
                                        <i class="fas fa-exclamation-triangle status-icon text-warning"></i>
                                    </div>
                                </div>
                                <div class="col-auto">
                                    <label for="" class="text-dark mr-2" id="validation-count">Validations today</label>
                                    <div id="counter-value" class="message-box bg-dark-gradient d-inline">
                                    </div>
                                </div>
                            </div>

                            <!-- message field -->
                            <div class="message-box message-status bg-dark-gradient">
                                 <?php 
                                 parcxValidation(array("option-type"=>"9","validator_id"=>$_SESSION["operator_id"],"login_time"=>$_SESSION["login_time"]));                               
                                 echo $_SESSION["login_time"]; ?>
                                Login
                            </div>

                            <!-- input field -->
                            <div class="form-group mb-4">
                                <label for="" class="text-dark">Enter Ticket Number</label>
                                <input type="" class="form-control bg-dark-gradient" id="ticketNum" aria-describedby="" placeholder="">
                            </div>

                            <!-- buttons -->
                           <?php parcxValidation(array("option-type"=>"10")); ?>

                            <!-- submit button -->
                            <button type="submit" name="" value="Validate"
                                class="calc-main-btn btn btn-lg btn-block mt-4 btn-info">Validate</button>

                        </div>

                        <!-- table -->
                        <div class="validation-table">
                            <div class="table-responsive card card-table-blue">
                                <table class="table table-blue">
                                <?php parcxValidation(array("option-type"=>"11","validator_id"=>$_SESSION["operator_id"])); ?>
                                </table>
                            </div>
                        </div>

                    </div>

                    <!-- graph -->
                    <div class="graph-container hidden-md-down">
                        <div class="card graph-box text-dark">
                            <div class="row">
                                <img class="img-fluid gif-anim" src="dist/img/gif/validate-steps-plates.gif">
                            </div>
                        </div>
                        <div class="row jspdf-graph">
                            <div class="col-lg-12">
                                <div class="card barchart-box m-0">
                                    <p class="chart-header">Hourly Validations</p>
                                    <div class="position-relative">
                                        <canvas id="overview-graph" height=""></canvas>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>

                </div>
            </div>

        </div>
    </section>
</div>

<!-- page specific scripts -->

<script src="dist/js/chartjs/chart.min.js"></script>
<script src="dist/js/chartjs/custom-graphs.js"></script>

<script>

    function graphHeight() {
        var value1 = $(".device").css("height")
        var value2 = $(".graph-box").css("height")
        var value3 = $(".validation-table").css("height")
        var height = (parseInt(value1, 10) + parseInt(value3, 10)) - parseInt(value2, 10) - 80;
        $("#overview-graph").css("height", height)
    }

    function menuLink() {
        if (window.innerWidth < 990) {
            $(".fa-bars").parent(".nav-link").hide();
            $("#pdf-report-header").hide();
        } else {
            $(".fa-bars").parent(".nav-link").show();
            $("#pdf-report-header").show();
        }
    }

    $(window).resize(function() {
        graphHeight();
        menuLink();
    });

    $(document).ready(function() {
        $("#ticketNum").focus();
        graphHeight();
        menuLink();
        $(".mobile-container").hide();
    });

    /* === validation scripts === */

    var valCount = 0;
    var selectedProduct;
    var product_id;

    $(document).on("click", ".calc-main-btn", function() {
        // if ticket number is not entered
        if ($("#ticketNum").val() === "") {
            $('#addTkt').modal('show')
        }
        // if validation product is not selected
        else if ($(".calc-btn.btn-success").length === 0) {
            $('#chooseProduct').modal('show')
        }
        // select product
        else {
            $('#confirmValidation').modal('show')
            $("#tktNum").html($("#ticketNum").val());
            $("#valProd").html(selectedProduct);
        }
    });

    $(document).on("click", ".calc-btn", function() {
        $(".calc-btn.btn-success").removeClass("btn-success");
        $(this).addClass("btn-success");
        selectedProduct = $(this).text();
        product_id = $(this).val();
    });

    $(document).on("click", "#confValidate", function() {
        var data = {};		
        data["ticket_id"] = $("#ticketNum").val();
        data["product_id"] = product_id;
        data["validator_id"] = $("#operator_id").val();
        data["option-type"] = 14;
        var jsondata = JSON.stringify(data);
        $.post("ajax/validation.php", jsondata, function(data) 
			{
				console.log(data);
				var json = JSON.parse(data);
                alert(json.message);
                if (json.status === "200")
                    location.reload();
            })
            .fail(function(jqxhr, status, error) {
                alert("Error: " + error);
            });

        // clear selection
        $(".calc-btn.btn-success").removeClass("btn-success");
        selectedProduct = null;
        product_id = null;
        $("#ticketNum").val("");
    });

    /* === set start and end hours for graph === */

    // set start and end hours
    var hours_start = ['0:00', '1:00', '2:00', '3:00', '4:00', '5:00', '6:00', '7:00', '8:00', '9:00', '10:00', '11:00',
        '12:00', '13:00', '14:00', '15:00', '16:00', '17:00', '18:00', '19:00', '20:00', '21:00', '22:00', '23:00'
    ];
    var hours_end = [];

    var i;

    for (i = 0; i < hours_start.length; ++i) {
        // if last value, create string using first value
        if (i != (hours_start.length - 1)) {
            var j;
            j = i + 1;
            var string_value = hours_start[j];
            hours_end.push(string_value);
        } else {
            var string_value = hours_start[0];
            hours_end.push(string_value);
        }
    }

    /* === get graph data from server === */

    var maxdata = [];
    var linedata = [];

    function get_hourly_validations() {
        maxdata = [];
        var data = {};
        data["validator_id"] = $("#operator_id").val();
        data["option-type"] = 12;
        var jsondata = JSON.stringify(data);
        $.post("ajax/validation.php", jsondata, function(data) 
			{
            //console.log(data);     				
			var json = JSON.parse(data);
            for (var i = 0; i < 24; i++) {
                maxdata.push(json[i]);
            }
            $("#counter-value").html(json[24]);
            updateBarChart();
        })
        .fail(function(jqxhr, status, error) {
            alert("Error: " + error);
        });
    }

    function get_average_hourly_validations() {
        linedata = [];
        var data = {};
        data["validator_id"] = $("#operator_id").val();
        data["option-type"] = 13;
        var jsondata = JSON.stringify(data);        
        $.post("ajax/validation.php", jsondata, function(data) 
			{
            //console.log(data);    
			var json = JSON.parse(data);
            for (var i = 0; i < 24; i++) {
                linedata.push(json[i]);
            }
            total = json[24];
            updateLineGraph();
        })
        .fail(function(jqxhr, status, error) {
            alert("Error: " + error);
        });
    }

    /* === update graph === */

    // only replace new data, to prevent the entire chart from animating

    function updateBarChart() {
        for (var i in maxdata) {
            if (Graph.data.datasets[0].data[i] !== maxdata[i]) {
                Graph.data.datasets[0].data[i] = maxdata[i];
            }
        }
        Graph.update();
    }

    function updateLineGraph() {
        for (var i in linedata) {
            if (Graph.data.datasets[1].data[i] !== linedata[i]) {
                Graph.data.datasets[1].data[i] = linedata[i];
            }
        }
        Graph.update();
    }

    /* === load graph === */

    var Graph

    function overviewGraph() {

        var hours_label = [];

        // only display the start time on mobile devices to prevent labels from overlapping
        if (window.innerWidth > 1440) {
            for (var i = 0; i < hours_start.length; i++) {
                hours_label[i] = hours_start[i] + "\n" + hours_end[i];
            }
        } else {
            hours_label = hours_start;
        }

        var default_data = {
            labels: hours_label,
            datasets: [{
                data: get_hourly_validations(),
                label: 'No. of Validations',
                backgroundColor: "rgba(0,123,255, 0.5)",
                borderColor: '#007bff',
                borderWidth: 1.5,
            }, {
                type: 'line',
                label: "Last week Average",
                borderColor: '#28a745',
                borderWidth: 2,
                fill: false,
                data: get_average_hourly_validations(),
            }]
        }

        var ticksStyle = {
            fontColor: '#000',
        }
        var mode = 'index';
        var intersect = true;
        var $overviewGraph = $('#overview-graph');
        Graph = new Chart($overviewGraph, {
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
                beforeInit: function(chart) {
                    chart.data.labels.forEach(function(e, i, a) {
                        // add linebreak where "\n" occurs
                        if (/\n/.test(e)) {
                            a[i] = e.split(/\n/);
                        }
                    });
                }
            }]
        })

    }

    $(document).ready(function() {

        overviewGraph();

        /*setTimeout(() => { 
            get_hourly_validations();
            get_average_hourly_validations();
        }, 100);

        setInterval(() => {
            get_hourly_validations();
            get_average_hourly_validations();
        }, 3000);*/

    });

</script>

<?php include('includes/footer.php'); ?>