<?php
$page_title = "Interface Settings";

include('../../includes/header.php');
?>

<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>


</ul>
<div class="header text-dark" id="pdf-report-header">Interface Settings</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <div class="tab-link" data-target="form">Add Interface</div>
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<div class="content-wrapper">
    <section class="content">
        <div class="container-wide">    
            <!-- add/update interface form --> 
            <input type="hidden" id="row_id" value="0" />
            <form class="block-data card card-body col-md-5" data-status="form" style="display:none;" id="form">  
                <div class="row">
                    <div class="col form-group">
                        <label for="">Interface Name</label>
                        <input type="text" class="form-control" id="name" aria-describedby="" placeholder="" name="interface-name">
                    </div>
                </div>
                                               				
                <div class="row">
                    <div class="col form-group">
                        <label for="">Type</label>
                        <select id = "interface_type">
                            <option value="ssh">SSH</option>
                            <option value="ftp">FTP</option>
                        </select>
                    </div>
                </div>

                <div class="row">	
                    <div class="col form-group">
                            <label for="">Host</label>
                            <input type="text" class="form-control" id="host" autocomplete="off" placeholder="" name="host">
                    </div>
                </div> 
                
                <div class="row">	
                    <div class="col form-group">
                            <label for="">Username</label>
                            <input type="text" class="form-control" id="username" autocomplete="off" placeholder="" name="username">
                    </div>
                </div> 

		<div class="row">	
                    <div class="col form-group">
                            <label for="">Password</label>
                            <input type="text" class="form-control" id="password" autocomplete="off" placeholder="" name="password">
                    </div>
                </div> 

		<div class="row" id = "show_ftp" style="display:none">	
                    <div class="col form-group">
                            <label for="" id="ftp_label">FTP Url</label>
                            <input type="text" class="form-control" id="url" autocomplete="off" placeholder="" name="url">
                    </div>
                </div> 

		<div class="row" id = "show_ssh">	
                    <div class="col form-group">
                            <label for="" id="ssh_label">SSH Folder Path</label>
                            <input type="text" class="form-control" id="path" autocomplete="off" placeholder="" name="path">
                    </div>
                </div> 
                

                <input type="submit" class="btn btn-block btn-info mt-2 btn-lg" value="Submit" id="add-edit-button">
            </form>             

            <!-- carpark table -->
            <div class="block-data" data-status="overview">
                <div class="card" >               
                    <div class="card-body">     
                        <table id="RecordsTable" class="table table-blue table-bordered table-striped">                    
<?php echo parcxSettings(array("task" => "37")); ?>
                        </table>
                    </div>                                                  
                </div>             
            </div>             

        </div>
    </section>
</div>
<?php include('../../includes/footer.php'); ?>
<script>
    var status;
    var id;
    /* === multiselect === */

    function multislect()
    {
        $('#multiselect').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "All Carparks",
            nonSelectedText: 'Select Carparks',
            selectAllNumber: false,
            allSelectedText: "All Carparks",
        });


        $('#days').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "Everyday",
            nonSelectedText: 'Select Day of the Week',
            selectAllNumber: false,
            allSelectedText: "Everyday",
        });

        $('#time-slots').multiselect({
            buttonWidth: '100%',
            includeSelectAllOption: true,
            selectAllText: "All Time Slots",
            nonSelectedText: 'Select Time Slots',
            selectAllNumber: false,
            allSelectedText: "All Time Slots",
        });


        // select all on load
        var multiselectElements = ['#days', '#time-slots', '#multiselect']

        for (var i in multiselectElements) {
            $(multiselectElements[i] + " option").each(function () {
                this.selected = true;
            });
            $(multiselectElements[i]).multiselect("refresh");
        }
    }

    $(document).ready(function ()
    {
        $('#RecordsTable').DataTable(
                {
                    "paging": true,
                    "lengthChange": true,
                    "searching": true,
                    "ordering": true,
                    "info": true,
                    "autoWidth": false,
                    "lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]],
                    "aaSorting": []

                });
        $('#multiselect').multiselect(
                {
                    buttonWidth: '100%',
                    includeSelectAllOption: true,
                    selectAllText: "All carparks",
                    nonSelectedText: "Select carparks",
                    selectAllNumber: false,
                    allSelectedText: "Select carparks",
                });

        $('#start_date,#expiry_date,#start_date_validation,#expiry_date_validation').daterangepicker({
            timePicker: false,
            timePickerIncrement: 10,
            format: 'YYYY-MM-DD',
            singleDatePicker: true,
        })



        $("* [data-target]").on('click', function ()
        {
            var $target = $(this).data('target');
            if ($target == "form")
            {
                $("#form").trigger('reset');
                
            }
            $('.block-data').css('display', 'none');
            $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
            $('.tab-link').removeClass('active');
            $(this).addClass('active');
        });

        

        //FormSubmit
        var formElement = $("#form");
        var rules_set = {};

        formElement.find('input[type=text]').each(function ()
        {
            var name = $(this).attr('name');
           // rules_set[name] = 'required';
        });

        formElement.validate({
            rules: rules_set,
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                error.insertAfter(element);
            },
            submitHandler: function ()
            {
                var data = {};
                
                data["id"] = $("#row_id").val(); 
                data["name"] = $("#name").val();
                data["interface_type"] = $("#interface_type").val();
                data["host"]=$("#host").val();
                data["username"] = $("#username").val();
                data["password"] = $("#password").val();
                if($("#interface_type").val()=="ssh")
                    data["folder_path"] = $("#path").val();
                else
                    data["url"] = $("#url").val();
                
                data["task"] = "39";
                var jsondata = JSON.stringify(data);
                console.log(jsondata);
                $.post("../../modules/ajax/settings.php", jsondata, function (result) {
                    if (result == "Successfull")
                        location.reload();
                    else
                        alert(result);
                })
                        .fail(function (jqxhr, status, error) {
                            alert("Error: " + error);

                        });
            }
        });

    });

    /* === enable disable product === */
    var status;
    var id;
    $(document).on("click", ".product-enable-disable-btn", function ()
    {
        id = $(this).parent('td').parent('tr').data('id');
        var status_text = $(this).text();
        if (status_text == "Disable")
            status = 0;
        else
            status = 1;

        var data = {};
        data["id"] = id;
        data["status"] = status;
        data["task"] = "32";
        var jsondata = JSON.stringify(data);
        $.post("../../modules/ajax/settings.php", jsondata, function (result) {
            if (result == "Successfull")
                location.reload();
            else
                alert(result);
        })
                .fail(function (jqxhr, status, error) {
                    alert("Error: " + error);

                });
    });

    /*=====edit======*/
    $(document).on("click", ".interface-edit", function ()
    {
        id = $(this).parent('td').parent('tr').data('id');
        var data = {};
        data["id"] = id;
        data["task"] = "38";
        var jsondata = JSON.stringify(data);
      
        $.post("../../modules/ajax/settings.php", jsondata, function (result) {
            $('.block-data[data-status="overview"]').hide();
            $('.block-data[data-status="form"]').show();
            $('.tab-link').removeClass('active');
            var response = JSON.parse(result);
            $("#row_id").val(id);
            $("#name").val(response.interface_name);
            $("#interface_type").val(response.interface_type);
            $("#host").val(response.host);
            $("#username").val(response.username);
            $("#password").val(response.password);
            $("#url").val(response.url);
            $("#path").val(response.folder_path);
            if(response.interface_type=="ssh")
            {
                $("#show_ftp").hide();
                $("#show_ssh").show();
                /*$("#url").hide();
                $("#ftp_label").hide();
                $("#path").show();
                $("#ssh_label").show();*/
            }
            else if(response.interface_type=="ftp")
            {
                $("#show_ftp").show();
                $("#show_ssh").hide();
                /*$("#url").show();
                $("#ftp_label").show();
                $("#path").hide();
                $("#ssh_label").hide();*/
            }
        })
        .fail(function (jqxhr, status, error) {
            alert("Error: " + error);

        });
    });

    $("#interface_type").change(function ()
    {
        if ($('#interface_type').val() == 'ftp')
        {
            $("#show_ftp").show();
            $("#show_ssh").hide();
        } else
        {
            $("#show_ftp").hide();
            $("#show_ssh").show();
        }
    });

</script>
