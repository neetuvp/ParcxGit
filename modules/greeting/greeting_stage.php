<?php
//Important Note : upload_max_filesize = 200M  and post_max_size = 200M in php.ini for upload size upto 200MB
$page_title = "Video Upload";

include('../../includes/header.php');
?>
<script src="js/lottie-player.js"></script>
<link href="css/parcx.css" rel="stylesheet">
<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>

</ul>
<div class="header text-dark" id="pdf-report-header">Greeting Stages</div>


<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>
<!-- Info modal -->
<div class="modal fade" id="info-modal" tabindex="-1" role="dialog" aria-labelledby="edit-preview" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Info</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body" id='info-message'> 
           </div>
        </div>
    </div>
</div>
<!-- Info modal -->


<!-- Preview modal -->
<div class="modal fade" id="preview-modal" tabindex="-1" role="dialog" aria-labelledby="preview" aria-hidden="true" style="z-index:1999;">
    <div class="modal-dialog modal-xl" style="width:100%;height:100%;overflow:hidden;">
        <div class="modal-content" style="width:100%;height:100%;overflow:hidden;">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Preview</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body" style="width:100%;height:100%;overflow:hidden;">        
                <embed type="text/html" id="preview-webpage" src="http://192.168.1.72/parcx" style="width:100%;height:100%;">            
            </div>
        </div>
    </div>
</div>
<!-- Preview modal -->


<!-- Edit modal -->
<div class="modal fade" id="edit-modal"  tabindex="-1" aria-labelledby="exampleModalLabel"
     aria-hidden="true" style="z-index:1999;">
    <div class="modal-dialog modal-xl" style="width:100%;">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Edit</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close" id="close-playlist-modal">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body" style="margin:15px;"> 
                <div class="container">
                    <div class="row justify-content-start">
                        <div class="col-3">               
                            <button class='btn btn-block btn-success btn-schedule' data-schedule='1'>Morning</button>
                        </div>
                        <div class="col-3"> 
                            <button class='btn btn-block btn-info btn-schedule' data-schedule='2'>Afternoon</button>
                        </div>
                        <div class="col-3"> 
                            <button class='btn btn-block btn-info btn-schedule' data-schedule='3'>Evening</button>
                        </div>
                        <div class="col-3">
                            <button class='btn btn-block btn-info btn-schedule' data-schedule='4'>Night</button>
                        </div>
                    </div>
                </div>
                <br>
                <div class="alert-danger" id="error-upload" style="display:none;"></div>
                <form name="form1" class="block-data card card-body p-5" data-status="form"  id="form">
                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Title</label>
                            <input type="text" class="form-control" id="title" name="title" readonly>
                        </div>
                    </div>
                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Description</label>
                            <input type="text" class="form-control" id="description" required name="description">
                        </div>
                    </div>

                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Message Line 1</label>
                            <textarea class="form-control" id="message_line1"></textarea>
                        </div>
                    </div>

                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Message Line 2</label>
                            <textarea class="form-control" id="message_line2"></textarea>
                        </div> 
                    </div>

                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Message Line 3</label>
                            <textarea class="form-control" id="message_line3"></textarea>
                        </div> 
                    </div>


                    <div class="row">                         
                        <div class="col form-group">
                            <label for="">Timeout</label>
                            <input type="number" class="form-control" id="timeout" required name="timeout">
                        </div> 
                    </div>


                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Backgroud Video</label>
                            <input type="hidden" id="bgfile_hidden" value="">
                            <br>
                            <input type='file'  id='bg_file' accept ="video/mp4"  />
                            <video width="25%" height="200" controls>
                                Your browser does not support the video tag.
                            </video>
                        </div> 
                    </div>

                    <div class="row"> 
                        <div class="col form-group">
                            <label for="">Status Animation</label></br>
                            <div class="form-check col-2">
                                <input class="form-check-input radiobtn" type="radio" name="animation-radio" data-name="gif" id="file_gif" value=".gif" checked>
                                <label class="form-check-label" for="gifradio">
                                  Gif
                                </label>
                              </div>
                              <div class="form-check col-2">
                                <input class="form-check-input radiobtn" type="radio" name="animation-radio" data-name="lottie" id="file_lottie" value=".lottie,.json" >
                                <label class="form-check-label" for="lottieradio">
                                  Lottie
                                </label>
                              </div>


                            
                            <input type="file"  id="animation_file" name="animationfile" accept=".gif"/>
                            <input type="hidden" id="animationfile_hidden" value="">
                            <input type="hidden" id="animationtype_hidden" value="">

                            <br>
                            <img src ="" width='25%' height="200" id="image_preview"/>
                            <div id="lottie_preview" style="width:25%;height:200px;"></div>
                        </div>                       
                    </div>  


                    <input type="submit" class="save-stage-details btn btn-block btn-info mt-2 btn-lg" value="Save" id="save-details">

                </form>             
            </div>
        </div>
    </div>
</div>
<!-- / end edit modal -->


<div class="content-wrapper container-wide">
    <section class="content">
           <div class="container-fluid">
                    <div class="container bootstrap snippets bootdeys">
                        <div class="col-md-12">
                            <div class="timeline-centered timeline-sm">
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-violet stage"><i class="fa fa-exclamation"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="1">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="1">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label"><h4 class="timeline-title" id="title-1">Standby</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-green stage"><i class="fa fa-car"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="2">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="2">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-green"><h4 class="timeline-title" id="title-2">Vehicle Arrives</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-orange stage"><i class="fa fa-paper-plane"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="3">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="3">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-orange"><h4 class="timeline-title" id="title-3">App Promotion</h4>

                                            <p></p></div>
                                    </div>
                                    
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-red stage"><i class="fa fa-arrow-up"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="4">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="4">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-red"><h4 class="timeline-title" id="title-4">Fueling Started</h4>

                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-pink stage"><i class="fa fa-paper-plane"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="5">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="5">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-pink"><h4 class="timeline-title" id="title-5">Advertising</h4>

                                            <p></p></div>
                                    </div>
                                    
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-blue stage"><i class="fa fa-thumbs-up"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="6">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="6">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-blue"><h4 class="timeline-title" id="title-6">Fueling Completed</h4>
                                            <p></p></div>
                                    </div>
                                </article>
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-dark stage"><i class="fa fa-calculator"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="7">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="7">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-dark"><h4 class="timeline-title" id="title-7">Fueling AmountToPay</h4>
                                            <p></p></div>
                                    </div>
                                                                 
                                </article>
                                <article class="timeline-entry left-aligned">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-green stage"><i class="fa fa-money-bill"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="8">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="8">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-green"><h4 class="timeline-title" id="title-8">Payment Collection</h4>
                                            <p></p></div>
                                    </div> 
                                </article>
                                
                                <article class="timeline-entry">
                                    <div class="timeline-entry-inner">
                                        
                                        <div class="dropdown">
                                            <div class="timeline-icon bg-yellow stage"><i class="fa fa-circle"></i>
                                                <div class="dropdown-content">
                                                    <div class="btn btn-block btn-default edit-btn" data-id="9">Edit</div>
                                                    <div class="btn btn-block btn-default preview-btn" data-id="9">Preview</div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="timeline-label bg-yellow"><h4 class="timeline-title" id="title-9">Thank You</h4>
                                            <p></p></div>
                                    </div>
                                    <div class="timeline-entry-inner">
                                        <div style="-webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg);" class="timeline-icon"><i class="fa fa-plus"></i></div>
                                    </div>                             
                                </article>
                            </div>
                        </div>
                    </div>
                </div> 
   
    </section>
    
              

</div>

<?php include('../../includes/footer.php'); ?>

<script type="text/javascript">
var stage_id;
var schedule = 1;
$(document).on("click", ".preview-btn", function ()
{
    stage_id = $(this).attr("data-id");
    $("#preview-modal").modal("show");
    $("#preview-webpage").attr("src", "http://192.168.1.72/PGEGreeting");
});

function initialize()
{
    $("#form").trigger("reset");
    $("#edit-preview-modal").modal("hide");
    $("#edit-modal").modal("show");
    $("#error-upload").hide();

    $("#image_preview").attr("src", "");
    $("#image_preview").hide();
    $("#lottie_preview").html("");
    $("#lottie_preview").hide();
    document.querySelector("video").src = "";
}

function showform()
{
    initialize();
    var data = {};
    data["stage_id"] = stage_id;//$("#stage-id").val();
    data["schedule"] = schedule;
    data["task"] = 3;
    var jsondata = JSON.stringify(data);
    console.log(jsondata);
    $.post("ajax/greeting_screen.php?task=3", jsondata, function (result) {
        console.log(result);
        var json = JSON.parse(result);
        $("#title").val(json.title);
        $("#description").val(json.description);
        $("#message_line1").val(json.message_line1);
        $("#message_line2").val(json.message_line2);
        $("#message_line3").val(json.message_line3);
        $("#timeout").val(json.timeout_period);
        $("#bgfile_hidden").val(json.bg_video_file);
        $("#animationfile_hidden").val(json.animation_file);
        $("#animationtype_hidden").val(json.animation_type);
        document.querySelector("video").src = "Media/" + json.bg_video_file + "#t=0.5";

        if (json.animation_type == "image/gif")
        {
            $("#file_gif").prop("checked", true);
            $("#file_lottie").prop("checked", false);
            $("#image_preview").show();
            $("#lottie_preview").hide();
            $("#image_preview").attr("src", "Media/" + json.animation_file);
        } else if (json.animation_type == "lottie" || json.animation_type == "application/json")
        {
            $("#file_lottie").prop("checked", true);
            $("#file_gif").prop("checked", false);
            $('#animation_file').attr("accept", ".json")
            $("#image_preview").hide();
            $("#lottie_preview").show();
            $("#lottie_preview").html("<lottie-player autoplay='true' loop='' src='Media/" + json.animation_file + "' speed='1'  width='200' height='150' background='transparent'></lottie-player>");
        }

    });
}


$(document).on("click", ".edit-btn", function ()
{
    stage_id = $(this).attr("data-id");
    showform(stage_id, 1);
});

$(document).on("click", ".btn-schedule", function ()
{
    schedule = $(this).attr("data-schedule");
    $('.btn-schedule').removeClass('btn-success');
    $('.btn-schedule').addClass('btn-info');
    $(this).removeClass('btn-info');
    $(this).addClass('btn-success');
    showform(stage_id, schedule);
});


$('.radiobtn').change(function () {
    $('#animation_file').val(""); // before setting
    $('#animation_file').attr("accept", $(this).val())
})

function uploadfile(type)
{
    event.preventDefault();
    var formData = new FormData();
    if (type == 1)
        var file_data = $("#bg_file").prop("files")[0];
    else if (type == 2)
        var file_data = $("#animation_file").prop("files")[0];
    file_data.name = new Date().valueOf() + "_" + file_data.name;
    formData.append('file', file_data);
    
    $.ajax({

        url: "ajax/greeting_screen.php?task=1",
        method: 'POST',
        data: formData,
        async: false,
        contentType: false,
        processData: false,
        success: function (response) {
            try {
                var json = JSON.parse(response);
                if (json.status == 200) {
                    var new_filname = json.file_name;
                    if (type == 1)
                        $("#bgfile_hidden").val(new_filname);
                    else if (type == 2) {
                        $("#animationfile_hidden").val(new_filname);
                        $("#animationtype_hidden").val(file_data.type);
                    }
                    result = "Success"
                    UpdateStageDetails();
                } else
                {
                    result = "Failed"
                    $("#error-upload").show();
                    $("#error-upload").html(json.message);
                }
            } catch (e) {

                $("#error-upload").show();
                $("#error-upload").html(response);
            }
        },
        error: function (jqXHR, textStatus, errorThrown) {
            $("#error-upload").show();
            $("#error-upload").html(textStatus + errorThrown);
        } //end of error
    });

}
$(document).on("click", ".save-stage-details", function ()
{
    var result = "Success";
    if ($('#bg_file').val() || $('#animation_file').val())
    {
        if ($('#bg_file').val()) {
            uploadfile(1);
        }
        if ($('#animation_file').val()) {
            uploadfile(2);
        }
    } else
    {
        UpdateStageDetails();
    }

});

function UpdateStageDetails()
{
    event.preventDefault();
    var data = {};
    data["stage_id"] = stage_id;
    data["schedule"] = schedule;
    data["title"] = $("#title").val();
    data["description"] = $("#description").val();
    data["message_line1"] = $("#message_line1").val();
    data["message_line2"] = $("#message_line2").val();
    data["message_line3"] = $("#message_line3").val();
    data["timeout"] = $("#timeout").val();
    data["bg_file"] = $("#bgfile_hidden").val();
    data["animation_file"] = $("#animationfile_hidden").val();
    data["animation_type"] = $("#animationtype_hidden").val();

    data["task"] = 2;
    var jsondata = JSON.stringify(data);
    console.log(jsondata);
    $.post("ajax/greeting_screen.php?task=2", jsondata, function (result) {
        console.log(result);
        if (result === "Success")
        {
            $("#edit-modal").modal('hide');
            $("#info-message").html("Update Successful");
            $("#info-modal").modal('show');

        }
        else
            alert(result);
    });
}

$('#info-modal').on('hidden.bs.modal', function () {
    location.reload();
   })

document.getElementById("bg_file")
        .onchange = function (event) {
            let file = event.target.files[0];
            let blobURL = URL.createObjectURL(file);
            document.querySelector("video").src = blobURL;
}

document.getElementById("animation_file")
        .onchange = function (event) {
            event.preventDefault();
            let file = event.target.files[0];
            let blobURL = URL.createObjectURL(file);
            if (file.type == "image/gif")
            {
                $("#image_preview").attr("src", blobURL);
                $("#image_preview").show();
                $("#lottie_preview").hide();
            } else if (file.type == "application/json")
            {
                $("#lottie_preview").show();
                $("#lottie_preview").html("<lottie-player autoplay='true' loop='' src='" + blobURL + "' speed='1'   background='transparent'></lottie-player>");
                $("#image_preview").hide();

            }
}
</script>
