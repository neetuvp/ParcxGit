<?php include('includes/header.php');?>
<div class="header text-dark" id="pdf-report-header">Validation Transaction Report</div>
<?php include('includes/sidebar.php');?>
<div class="content-wrapper desktop-container">

    <div class="additional-menu row m-0 bg-white border-bottom report-sort">
        <div class="col d-flex align-items-center">
            <div class="flex-grow-1 row">

                <!-- date and time -->
                <div class="datepicker-container menu-item">
                    <div class="input-group">
                        <div class="input-group-prepend">
                            <span class="input-group-text"><i class="far fa-clock"></i></span>
                        </div>
                        <input type="text" class="form-control float-right" id="report-date-time" autocomplete="off" placeholder="Choose Date and Time Range">
                    </div>
                </div>                

                <!-- weekdays -->
                <div class="menu-item">
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

                <!-- carpark -->
                <div class="menu-item">
                    <select id="carpark" multiple="multiple" class="">
                        <?php parcxValidation(array("option-type"=>"5"));?>?>
                    </select>
                </div>

                <!-- product -->
                <div class="menu-item">
                    <select id="products" multiple="multiple" class="">
                    <?php parcxValidation(array("option-type"=>"8"));?>
                    </select>
                </div>

                <!-- search -->
                <div class="menu-item">
                    <button type="button" class="btn btn-block btn-secondary" id="view-report-button">Search</button>
                </div>
               

            </div>

        </div>
    </div>

    <section class="content">
        <div class="container-wide">

            <div class="validation-table p-0" id="report-table">
                
            </div> 

        </div>
    </section>

</div>

<?php include('includes/footer.php');?>

<script>

    /* === report show / hide failed transactions === */

    $(document).ready(function() {              

        // default - hide failed transactions
        var failedElement = $('[data-type="failed-transaction"]');
        failedElement.hide();

        // on change event
        $("#failed-checkbox").change(function() {
            if ($(this).is(':checked')) {
                // show failed transactions
                $(".validation-table table tr").hide();
                $(".validation-table table thead tr").show();
                failedElement.show();
            } else {
                // hide failed transactions
                $(".validation-table table tr").show();
                failedElement.hide();
            }
        });

    });

    /* === report button click === */

    var daterange, from, to;
    
    $("#view-report-button").click(function() {       
        if ((!daterange)) 
            {
            alert("choose date range");
            return;
            }
        var data={};
        data["from"]=from;
        data["to"]=to;

       
        data["validator_id"]=$("#operator_id").val(); 
        data["days"]=$("#days").val().toString();        
        data["carparks"]=$("#carpark").val().toString();                                  
        data["products"]=$("#products").val().toString();                
        data["option-type"]=15;
        var jsondata = JSON.stringify(data);  
        
        $.post("ajax/validation.php",jsondata,function(data){                         
               $("#report-table").html(data);             
           })
           .fail(function(jqxhr,status,error){
               alert("Error: "+error);

           });  
    });


    /* === datepicker === */

    $('#report-date-time').daterangepicker({
        timePicker: true,
        timePickerIncrement: 1,
        format: 'YYYY-MM-DD HH:mm'
    })

    // save from/to values for report
    $('#report-date-time').on('apply.daterangepicker', function (ev, picker) {
        daterange = $("#report-date-time").val();
        from = daterange.substring(0, 16);
        to = daterange.substring(19, 41);
    });

    /* === multiselect === */

    $(document).ready(function() {

        $('#days').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "Everyday",
            nonSelectedText: 'Select Day of the Week',
            selectAllNumber: false,
            allSelectedText: "Everyday",
        });

        $('#validator').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "All Accounts",
            nonSelectedText: 'Select Account',
            selectAllNumber: false,
            allSelectedText: "All Accounts",
        });

        $('#carpark').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "All Carparks",
            nonSelectedText: 'Select Carparks',
            selectAllNumber: false,
            allSelectedText: "All Carparks",
        });

        $('#products').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "All Products",
            nonSelectedText: 'Select Products',
            selectAllNumber: false,
            allSelectedText: "All Products",
        });

        // select all on load

        var multiselectElements = [ '#days', '#validator', '#carpark', '#products' ]

        for (var i in multiselectElements) {
            $(multiselectElements[i] + " option").each(function() {
                this.selected = true;
            });
            $(multiselectElements[i]).multiselect("refresh");
        }

    });

</script>