<?php include('../../includes/header.php'); ?>
<div class="navbar-has-tablink">
    <?php include('../../includes/navbar-start.php'); ?>
</ul>
<div class="header text-dark" id="pdf-report-header">ANPR Review</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- Modal Update Plate Information-->
<div class="modal fade" id="UpdatePlateModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Update Plate Number</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">        
                <img  id="plate-image" class="img-responsive" src="plateimage.jpg" class="img-rounded" alt="PlateNumber" style="width:100%">
                <br/><br/>
                <label for="PlateNumber">Current PlateNumber</label>
                <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="plate-number" disabled="">
                <br/><br/>
                <label for="PlateNumber">Enter Corrected PlateNumber</label>
                <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="corrected-plate-number">            
            </div>
            <div class="modal-footer">                
                <button type="button" class="btn btn-primary" id="btn-save">Save changes</button>
                <button type="button" class="btn btn-danger" data-dismiss="modal">Cancel</button>      
            </div>
        </div>
    </div>
</div>    
<!-- End . Modal for Plate Update -->

<!-- Modal POP to get the operator Attention -->
<div class="modal fade" id="PopUpAlertModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel"></h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">        
                <div class="alert alert-danger" role="alert">
                    <h4 class="alert-heading" id="alert-heading"></h4>
                    <p>Please review the list of number plates which require corrections or confirmation</p>
                    <hr>
                    <p class="mb-0"> Click update in the list and make the required required correction after reviewing the image of the entry plate </p>
                </div>                    
            </div> <!-- End . Modal Body -->
            <div class="modal-footer">
                <button type="button" class="btn btn-secondary" data-dismiss="modal">OK</button>      
            </div>
        </div>
    </div>
</div>    
<!-- End . Modal for Plate Update -->

<!-- Modal -->
<div class="modal fade text-dark" id="plate-mismatch-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel"
     aria-hidden="true">
    <div class="modal-dialog modal-xl" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Manual plate correction</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body p-4">      
                <div id="alert-div-edit-plate" class="alert bg-danger d-none">
                    <h5> Plates are not matching!</h5>                                            
                </div>                 

                <div class="row mb-4">
                    <div class="col">
                        <div class="border-simple p-1" id="plate_image1">

                        </div>
                    </div>
                    <div class="col">
                        <div class="border-simple p-1" id="plate_image2">

                        </div>
                    </div>

                </div>
                <!-- end / info -->

                <!-- info -->
                <div class="row mb-4" id="plate_secondary_modal">
                    <div class="col">
                        <div class="border-simple h-100 p-3">                                                
                            <label for="PlateNumber">Exit PlateNumber</label>
                            <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="plate_number1" disabled="">
                            <br/>
                            <label for="PlateNumber">Enter Corrected PlateNumber</label>
                            <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="corrected_plate_number1">
                        </div>
                    </div>
                    <div class="col">
                        <div class="border-simple h-100 p-3">
                            <label for="PlateNumber">Entry PlateNumber</label>
                            <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="plate_number2" disabled="">
                            <br/>
                            <label for="PlateNumber">Enter Corrected PlateNumber</label>
                            <input class="form-control form-control-lg" type="text" placeholder="Plate Number" id="corrected_plate_number2">                                                        
                        </div>
                    </div>
                </div>                 
            </div>
            <div class="modal-footer">   
                <!--                <button type="button" class="btn btn-success" id="btn-allow">Allow</button>-->
                <button type="button" class="btn btn-primary" id="btn-save-plate-mismatch">Save changes</button>                
            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<div class="content-wrapper">  
    <section class="content">
        <div class="container-wide"> 
            <input type="hidden" id="user-id" value="<?php echo $_SESSION['userId']; ?>">
            <input type="hidden" id="user-name" value="<?php echo $_SESSION['user']; ?>">  
            <input type="hidden" id="image-location" value="<?php echo ImageURL; ?>">  

            <div class="card">
                <div class="card-header"><h3 class="card-title col">Plate Corrections Required</h3></div>                       
                <div class="card-body">
                    <table id="plate-correction-required" class="table table-bordered table-hover"></table>
                </div>         
            </div>

            <br>

            <div class="card">
                <div class="card-header"><h3 class="card-title col">Plate Mismatch</h3></div>                       
                <div class="card-body">
                    <table id="plate-mismatch" class="table table-bordered table-hover"></table>
                </div>         
            </div>
            <br>

            <div class="card">
                <div class="card-header"><h3 class="card-title col">Plates Corrected</h3></div>            
                <div class="card-body">
                    <table id="plate-correction-complete" class="table table-bordered table-hover"></table>
                </div>            
            </div>                   
        </div>             
    </section>
</div>
<script>

    var eventTime = 0;
    var currentTime = 0;
    var withoutEvent = 0;
    $(document).on('click mouseover keydown', function (evt) {
        eventTime = Date.now();
    });

    function correction_required()
    {
        var data = {};
        data["task"] = 1;
        var json = JSON.stringify(data);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            $("#plate-correction-required").html(data);
            var count = $('#plate-correction-required tr').length;
            if (count > 0)
            {
                $("#alert-heading").html((count - 1) + " number plates captured require attention!");
                $('#PopUpAlertModal').modal('show');
            }
        });
    }


    function plate_mismatch()
    {
        var data = {};
        data["task"] = 6;
        var json = JSON.stringify(data);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            $("#plate-mismatch").html(data);
        });
    }

    function number_plate_corrected()
    {
        var data = {};
        data["task"] = 3;
        var json = JSON.stringify(data);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            $("#plate-correction-complete").html(data);
        });
    }

    $(function ()
    {
        correction_required();
        number_plate_corrected();
        plate_mismatch();

        setInterval(function ()
        {
            currentTime = Date.now();
            withoutEvent = currentTime - eventTime;
            withoutEvent = withoutEvent / 1000; //seconds       
            if ($('#PopUpAlertModal').is(':visible') == false && $('#UpdatePlateModal').is(':visible') == false)
            {
                if (withoutEvent >= 60 * 3)
                {
                    correction_required();
                }
            }
        }, 1000 * 30);

    });

    var id;
    var plate;
    var update;
    $('body').on('click', "[data-target='#UpdatePlateModal']", function ()
    {
        var name = $(this).data('value');
        id = $(this).data('id');
        plate = $(this).data('plate');
        update = $(this).data('update');
        $("#btn-allow").hide();
        var location = $("#image-location").val() + "/";
        $("#plate-image").attr("src", location + name);
        $("#plate-number").val(plate);
    });

    $(document).on('click', '#btn-save', function ()
    {
        var data = {};
        data["task"] = 2;
        data["id"] = id;
        data["plate_number"] = plate;
        data["update"] = update;
        data["corrected_plate_number"] = $("#corrected-plate-number").val();
        data["user_id"] = $("#user-id").val();
        data["user_name"] = $("#user-name").val();
        var json = JSON.stringify(data);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            location.reload();
        });


    });


    $(document).on('click', '.btn-plate-mismatch', function ()
    {
        id = $(this).data('id');
        var data = {};
        data["task"] = 7;
        data["id"]=id;        
        var json = JSON.stringify(data);
        console.log(json);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            var response = JSON.parse(data);
            $("#plate-mismatch-modal").modal("show");
            var image_url = "<?php echo ImageURL ?>";
            $('#plate_image1').html('<img src ="' + image_url + '/' + response["entry_image"] + '" width="100%"; height="100%";>');
            $('#plate_number1').val(response["entry_plate_number"]);
            $('#corrected_plate_number1').val(response["entry_plate_number"]);
            $('#plate_image2').html('<img src ="' + image_url + '/' + response["exit_image"] + '" width="100%"; height="100%";>');
            $('#plate_number2').val(response["exit_plate_number"]);
            $('#corrected_plate_number2').val(response["exit_plate_number"]);
        });


    });

    $(document).on('click', '#btn-allow', function ()
    {
        var data = {};
        data["task"] = 2;
        data["id"] = id;
        data["plate_number"] = plate;
        data["update"] = 2;
        data["user_id"] = $("#user-id").val();
        data["user_name"] = $("#user-name").val();
        var json = JSON.stringify(data);
        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            location.reload();
        });


    });

    $(document).on('click', '#btn-save-plate-mismatch', function ()
    {
        id = $(this).data('id');
        var data = {};
        data["task"] = 8;
        data["entry_id"] = $(this).data('entry-id');
        data["exit_id"] = $(this).data('exit-id');
        data["entry_plate"] = $("#corrected_plate_number1").val();
        data["exit_plate"] = $("#corrected_plate_number2").val();
        var json = JSON.stringify(data);

        $.post("../ajax/dashboard-ajax.php", json, function (data)
        {
            location.reload();
        });


    });

</script>
<?php include '../../includes/footer.php'; ?>