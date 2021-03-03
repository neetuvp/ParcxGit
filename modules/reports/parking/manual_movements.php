<?php
$page_title = "Application";

//# Import application layout.
include('../../../includes/header.php');
include('../../../includes/navbar-start.php');
$data = array();
$data["task"] = 29;
$data["language"] = $_SESSION["language"];
$data["page"] = 18;
$json = parcxReport($data);
?>

</ul>

<div class="header text-dark" id="pdf-report-header"><?= $json["manual_movement"] ?></div>

<?php
include('../../../includes/navbar-end.php');
include('../../../includes/sidebar.php');
?>



<div class="content-wrapper">

    <!-- additional menu -->
    <div class="additional-menu row m-0 bg-white border-bottom">
        <div class="col d-flex pl-1 align-items-center">

            <div class="flex-grow-1 row additional-menu-left">

                <!-- carparks -->
                <div class="col-md-2">
                    <select class="form-control" id="multiselect" multiple="multiple">

<?php echo parcxSettings(array("task" => "12")); ?>
                    </select>
                </div>

                <!-- operation methods -->
                <div class="col-md-2">
                    <select class="form-control" id="operation">
                        <option value="0" id="all_operation_methods" ><?= $json["all_operation_methods"] ?></option>
                        <option value="cashier" id="manual_open_from_cashier"><?= $json["manual_open_from_cashier"] ?></option>
                        <option value="pushbutton" id="remote_pushbutton_open"><?= $json["remote_pushbutton_open"] ?></option>
                    </select>
                </div>

<?php include('../../../includes/additional-menu-report.php'); ?>       
                <!-- end / additional menu -->


                <section class="content">
                    <div class="container-wide">
                        <div class="card">
                            <div class="card-body" id="report-content">          
                            <?php
                            $current_date = date("Y-m-d");
                            $data["from"] = $current_date . " " . DAY_CLOSURE_START;
                            $data["to"] = $current_date . " " . DAY_CLOSURE_END;
                            $data["carpark"] = "";
                            $data["operation"] = "";
                            $data["language"] = $_SESSION["language"];
                            $data["task"] = 11;
                            echo parcxReport($data);
                            ?>
                            </div>
                        </div>
                    </div>
                </section>
            </div>


<?php include('../../../includes/footer.php'); ?>

<script>
    var date_range_message = "choose date range";
    from = "<?= $current_date . " " . DAY_CLOSURE_START ?>";
    to = "<?= $current_date . " " . DAY_CLOSURE_END ?>";

    $(function ()
    {
        $('#multiselect').multiselect(
                {
                    buttonWidth: '100%',
                    includeSelectAllOption: true,
                    selectAllText: "<?= $json["all_carparks"] ?>",
                    nonSelectedText: "<?= $json["select_carparks"] ?>",
                    selectAllNumber: false,
                    allSelectedText: "<?= $json["all_carparks"] ?>",
                });
    });
    function callReport()
    {
        var carpark = $("#multiselect").val().toString();
        var operation = $("#operation").val();
        var language = $("#language").val();

        var data = {
            carpark: carpark,
            operation: operation,
            to: to,
            from: from,
            language: language,
            task: 11
        };
        var jsondata = JSON.stringify(data);
        console.log(jsondata);
        $.post("../../ajax/reports.php", jsondata, function (data)
        {
            console.log(data);
            loadReport(data);
            //$("#report-content").html(data);
        })
                .fail(function (jqxhr, status, error)
                {
                    alert("Error: " + error);
                });
        event.preventDefault();
        //}); //manual movements-report-content
    }


    $('#view-report-button').click(function (event)
    {
        if (!daterange)
            alert(date_range_message);
        else
            callReport();
    });

    function loadReportLabels()
    {
        var data = {};
        data["task"] = 29;
        data["language"] = $("#language").val();
        data["page"] = 18;
        var json = JSON.stringify(data);
        $.post("../../ajax/reports.php", json, function (data)
        {
            var json = JSON.parse(data);
            date_range_message = json.choose_datetime_range;
            $("#reservationtime").attr('placeholder', json.choose_datetime_range);
            $("#pdf-report-header").html(json.manual_movement);
            $("#view-report-button").html(json.view_report);
            $("#export").html(json.export);
            $("#export_excel_report").html(json.export_to_excel);
            $("#export_pdf_report").html(json.export_to_pdf);
            $("#logout").html(json.logout);
            search_label = json.search;
            entries_label = json.entries_label;
            info_label = json.info_label;
            previous_label = json.previous;
            next_label = json.next;
            $("#all_operation_methods").html(json.all_operation_methods);
            $("#manual_open_from_cashier").html(json.manual_open_from_cashier);
            $("#remote_pushbutton_open").html(json.remote_pushbutton_open);

            $('#multiselect').multiselect('destroy');
            $('#multiselect').multiselect(
                    {
                        buttonWidth: '100%',
                        includeSelectAllOption: true,
                        selectAllText: json.all_carparks,
                        nonSelectedText: json.select_carparks,
                        selectAllNumber: false,
                        allSelectedText: json.all_carparks
                    });
        });
    }

    $("#language").change(function ()
    {
        loadReportLabels();
        callReport();
    });



    $('#export_excel_report').click(function (event) {

        export_to_excel("#report-content", "PMS_Manual_Movements_Report")

    }); // end click event function


</script>