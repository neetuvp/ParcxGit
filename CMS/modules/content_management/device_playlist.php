<?php
//Add in footer:  plugins/timepicker/jquery.timepicker.min.js"></script>
//plugins/timepicker/jquery.timepicker.min.css" />
$page_title = "Playlist";

include('../../includes/header.php');

?>



<div class="navbar-has-tablink">

    <?php
    include('../../includes/navbar-start.php');
    ?>

</ul>
<div class="header text-dark" id="pdf-report-header">Device Playlist</div>
<div class="row hidden-sm-down">
    <div class="col tab-header d-flex justify-content-center">
        <div class="tab-link active" data-target="overview">Overview</div>
        <!--<div class="tab-link" data-target="form">Add Playlist to Device</div>-->
    </div>
</div>

<?php
include('../../includes/navbar-end.php');
include('../../includes/sidebar.php');
?>

<!-- Modal  -->
<!-- Modal -->
<div class="modal fade" id="view_playlist_modal"  role="dialog" aria-labelledby="exampleModalLabel"
     aria-hidden="true" style="z-index: 1999;">
    <div class="modal-dialog modal-xl" role="document" style="width:1000px;">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">Manage Playlist</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close" id="close-alarm-modal">
                    <span aria-hidden="true">&times;</span>
                </button>
                
            </div>
            <div class="modal-body p-4" id="playlist-modal-body">  
                               
            </div>
        </div>
    </div>
</div>
<!-- / end modal -->

<div class="content-wrapper container-wide">
    <section class="content">
        
        <div class="block-data col-md-12" data-status="overview">
            <div class="card" >               
                <div class="card-body" id="div-RecordsTable">     
                    <table id="RecordsTable" class="table  table-bordered"> 
                        <?php
                        $data["task"] = 6;
                        $data["edit"] = 1;
                        $data["delete"]=1;
                        parcxContentManagement($data);
                        ?> 
                    </table>
                </div>                                                  
            </div>             
        </div> 
    
    </section>
</div>


<?php include('../../includes/footer.php'); ?>

<script>
    var status;
    var id;
    var table;
    var user_name;
    var page = 0;
    var edit=false;
    table = $('#RecordsTable').DataTable({"lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]], "aaSorting": []});
    
    //Load data
function loadTable()
{
    var data = {};
    data["task"] = "6";
    data["edit"] =1;
    data["delete"]=1;
    var jsondata = JSON.stringify(data);
    $.post("../../modules/ajax/cms.php", jsondata, function (result) {
        $("#div-RecordsTable").html("<table id='RecordsTable' class='table  table-bordered'>" + result + "</table>");
        table = $('#RecordsTable').DataTable({"lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]], "aaSorting": []});
        table.page(page).draw(false);
    });
}
    
    //2022-03-01 23:00:00 - 2022-03-28 23:59:59
    $(document).ready(function ()
    {

        //$("#form-edit-user").validate();
        
        //table = $('#RecordsTable').DataTable({"lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]], "aaSorting": []});
        $("* [data-target]").on('click', function ()
        {
            var $target = $(this).data('target');
            if ($target == "form")
            {
                //alert("here");
                $("#left").html("");
                $("#right").html("");
                $("form[name=form1]").trigger('reset');
                //$('#form')[0].reset();
                var data = {};
 
                data["task"] = "4";
                var jsondata = JSON.stringify(data);
                $.post("../../modules/ajax/cms.php", jsondata, function (result) {
                    $("#left").html(result)
                });
                $("#add-edit-button").val("Submit");
            }
            $('.block-data').css('display', 'none');
            $('.block-data[data-status="' + $target + '"]').fadeIn('slow');
            $('.tab-link').removeClass('active');
            $(this).addClass('active');
        });


        //FormSubmit
        var formElement = $("#form");
        var rules_set = {
            playlist_name: {
                required: true,
                minlength: 3,
                maxlength: 100
            }
        };



        formElement.validate({
            rules: rules_set,
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                if(element.is('input:password') )
                    error.insertAfter(element.parent());
                else
                    error.insertAfter(element);                
            },
            
            submitHandler: function ()
            {
                var data = {};
                var div_id="";
                data["playlist_name"] = $("#playlist_name").val();
                data["description"] = $("#description").val();
                var video_list="";
                $('#right > div').map(function() {
                    div_id = this.id;
                    div_id = div_id.substring(1)
                    video_list = video_list+div_id+",";
                });
                var l = video_list.length;
                video_list = video_list.substring(0,l-1);
                data["video_list"] = video_list;
                data["task"] = "5";
                var jsondata = JSON.stringify(data);
                console.log(jsondata);
                $.post("../../modules/ajax/cms.php", jsondata, function (result) {
                    console.log(result);
                    if (result === "Successfull")
                        location.reload();
                    else
                        alert(result);
                });
            }
        });
     });




    
    $(document).on("click", ".manage-playlist-button", function ()
    {
        var id = $(this).attr("data-id");
        var name = $(this).attr("data-name");
        var data = {};
        data["device_id"] = id;
        data["device_name"]=name;
        data["task"] = "7";
        //data["task"] = "2";
        var jsondata = JSON.stringify(data);
        //console.log(jsondata);
        //$.post("../../modules/ajax/playlist.php", jsondata, function (result) {
        $.post("../../modules/ajax/cms.php", jsondata, function (result) {
            $("#view_playlist_modal").modal('show');
            $("#playlist-modal-body.modal-body").html(result);
            $(".pl-options").hide();
            
        });
        event.preventDefault();
    });
    
   
    
   /* function drag(ev) {
        console.log(ev.target.id);
        ev.dataTransfer.setData("div", ev.target.id);
    }
    
    function drop(ev) {
        var _target = $("#" + ev.target.id);
        if ($(_target).hasClass("nodrop")) {
            ev.preventDefault();
            var src = document.getElementById(ev.dataTransfer.getData("div"));
            var srcParent = src.parentNode;
            var tgt = ev.currentTarget.firstElementChild;

            ev.currentTarget.replaceChild(src, tgt);
            srcParent.appendChild(tgt);
        } else {
            ev.preventDefault();
            var data = ev.dataTransfer.getData("div");
            ev.target.appendChild(document.getElementById(data));
        }
        
    }

    function allowDrop(ev) {
        ev.preventDefault();
    }*/
    
   /* $(document).on("click", ".view-details-button", function ()
    {
        var id = $(this).attr("data-id");
        var data = {};
        data["id"] = id;
        data["task"] = "8";
        var jsondata = JSON.stringify(data);
        console.log(jsondata);
        $.post("../../modules/ajax/cms.php", jsondata, function (result) {
            $("#view_playlist_modal").modal('show');
           // $(".modal-body").html(result);
            $(".modal-body").html("<table id='RecordsTable' class='table  table-bordered'>" + result + "</table>");
            table = $('#RecordsTable').DataTable({"lengthMenu": [[10, 25, 50, -1], [10, 25, 50, "All"]], "aaSorting": []});
            table.page(page).draw(false);
            
        });

    });*/
    
   /* $('#view_playlist_modal').on('shown.bs.modal', function (e) {
     $('.scheduledate').daterangepicker(
        {
            timePicker: false,
            timePickerIncrement: 1,
            format: 'YYYY-MM-DD'
        });
    });*/
    
    $('#view_playlist_modal').on('shown.bs.modal', function (ev) {
        $('.scheduledate').daterangepicker(
        {
            timePicker: false,
            timePickerIncrement: 1,
            format: 'YYYY-MM-DD'
        }).end().on('keypress paste', function (e) {
            e.preventDefault();
            return false;
        });
        
        
        $('.timepicker').timepicker({
            scrollDefault: '00:00',
            timeFormat: 'H:i',
            step: 15,
        });
        
        /*$('.schedulestarttime').daterangepicker({
            timePicker : true,
            singleDatePicker:true,
            timePicker24Hour : true,
            timePickerIncrement : 1,
            timePickerSeconds : false,
            autoApply: false,
            locale : {
                format : 'HH:mm:ss'
            }
        }).on('show.daterangepicker', function(ev, picker) {
            picker.container.find(".calendar-date").hide();
        }).on('showCalendar.daterangepicker', function(ev, picker){
           picker.container.find('.calendar-date').remove();
        }).on('apply.daterangepicker', function (e, picker) {
            var starttime = picker.startDate.format('HH:mm:ss');
            var id = $(this).attr("data-id")
            $('#start-time'+id).val(starttime);
        }).end().on('keypress paste', function (e) {
            e.preventDefault();
            return false;
        });
        
        $('.scheduleendtime').daterangepicker({
            timePicker : true,
            singleDatePicker:true,
            timePicker24Hour : true,
            timePickerIncrement : 1,
            timePickerSeconds : false,
            autoApply: false,
            locale : {
                format : 'HH:mm:ss'
            }
        }).on('show.daterangepicker', function(ev, picker) {
            picker.container.find(".calendar-date").hide();
        }).on('showCalendar.daterangepicker', function(ev, picker){
           picker.container.find('.calendar-date').remove();
        }).on('apply.daterangepicker', function (e, picker) {
            var endtime = picker.endDate.format('HH:mm:ss');
            var id = $(this).attr("data-id")
            $('#end-time'+id).val(endtime);
        }).end().on('keypress paste', function (e) {
            e.preventDefault();
            return false;
        });*/
        
        
        
        
    });
    
    
    $(document).on("click", ".playlist-checkbox", function ()
    {
        edit=false;
        var id = $(this).attr("data-id");
        //alert(id)
        if($(this).is(":checked")) {
            $('.modal-body').find("#pl-options-"+id).show();
            //$("#pl-options-"+id).show();
        } else {
            //$("#pl-options-"+id).hide();
            $('.modal-body').find("#pl-options-"+id).hide();
        }
            
            //$("#expand-div-p1").removeClass("d-none");
        //}
                
    });
    
   /* $(document).on("click", ".edit-playlist", function ()
    {
        var id = $(this).attr("data-id");
        //alert(id)
        if($(this).is(":checked")) {
            $('.modal-body').find("#pl-options-"+id).show();
            //$("#pl-options-"+id).show();
        } else {
            //$("#pl-options-"+id).hide();
            $('.modal-body').find("#pl-options-"+id).hide();
        }
            
            //$("#expand-div-p1").removeClass("d-none");
        //}
                
    });
 */
    function addeditplaylist(id,e)
    {
        e.preventDefault();
        $("#pl-options-"+id).addClass("hidden");
        //$("#v"+id).addClass("hidden");//;.remove();
        $("#span-cbox-"+id).addClass("hidden");
        $("#span-del-"+id).removeClass("hidden");
        $("#span-edit-"+id).removeClass("hidden");
        //$("#right").append("<div class='btn btn-block btn-info btn-lg bg-gray playlist-right' data-playlist="+id+" id='right-v"+id+"'>"+$('#v'+id).html()+"</div>");
        if(edit==false){
            var element = $('#v'+id).clone();
            element.addClass("playlist-right")
            $("#v"+id).remove();
            $("#right").append(element);
        }
        edit=false;
    }
    
    $(document).on("click", ".add-playlist", function (e)
    {
        var id = $(this).attr("data-id");
        addeditplaylist(id,e);
    });
    
   
    
    $(document).on("click", ".del-playlist", function (e)
    {
        var id =$(this).attr("data-id");
        e.preventDefault();
        var element = $('#v'+id).clone();
        element.removeClass("playlist-right");
        
        $("#v"+id).remove();
        $("#left").append(element);
        $("#v"+id).find(".scheduledate").daterangepicker({
            timePicker: false,
            timePickerIncrement: 1,
            format: 'YYYY-MM-DD'
        });
        
        $("#v"+id).find('.schedulestarttime').timepicker({
            //scrollDefault: '00:00',
            timeFormat: 'H:i',
            step: 15,
            
        });
        
        $("#v"+id).find('.scheduleendtime').timepicker({
            //scrollDefault: '00:00',
            timeFormat: 'H:i',
            step: 15,
            
        });
        /*$("#v"+id).find('.schedulestarttime').daterangepicker({
            timePicker : true,
            singleDatePicker:true,
            timePicker24Hour : true,
            timePickerIncrement : 1,
            timePickerSeconds : false,
            autoApply: false,
            locale : {
                format : 'HH:mm:ss'
            }
        }).on('show.daterangepicker', function(ev, picker) {
            picker.container.find(".calendar-date").hide();
            picker.container.find('.hourselect').val("5");
        }).on('showCalendar.daterangepicker', function(ev, picker){
           picker.container.find('.calendar-date').remove();
        }).on('apply.daterangepicker', function (e, picker) {
            var hh = picker.container.find('.hourselect').val();
            var mm = picker.container.find('.minuteselect').val();
            var ampm = picker.container.find('.ampmselect').val();
            const timeformat = moment(hh+":"+mm+" "+ampm, ["h:mm A"]).format("HH:mm");
            $('#start-time'+id).val(timeformat+":00");
        });
        
        $("#v"+id).find('.scheduleendtime').daterangepicker({
            timePicker : true,
            singleDatePicker:true,
            timePicker24Hour : true,
            timePickerIncrement : 1,
            timePickerSeconds : false,
            autoApply: false,
            locale : {
                format : 'HH:mm:ss'
            }
        }).on('show.daterangepicker', function(ev, picker) {
            picker.container.find(".calendar-date").hide();
        }).on('showCalendar.daterangepicker', function(ev, picker){
           picker.container.find('.calendar-date').remove();
        }).on('apply.daterangepicker', function (e, picker) {
            var hh = picker.container.find('.hourselect').val();
            var mm = picker.container.find('.minuteselect').val();
            var ampm = picker.container.find('.ampmselect').val();
            const timeformat = moment(hh+":"+mm+" "+ampm, ["h:mm A"]).format("HH:mm");
            $('#end-time'+id).val(timeformat+":00");
	    if(mm!="0")
	    {
		$('#end-time'+id).val(timeformat+":59");
            }
        });*/
        
        $("#pl-options-"+id).removeClass("hidden");
        //$('#playlist-modal-body').find("#c"+id).removeClass("hidden");//.remove();
        $("#span-cbox-"+id).removeClass("hidden");
        $("#span-del-"+id).addClass("hidden");
        $("#span-edit-"+id).addClass("hidden");
        $("#c"+id).prop('checked', false); // Unchecks it
        $("#pl-options-"+id).hide();
    });
    
    $(document).on("click", ".playlist-details-div", function ()
    {
        id = $(this).attr("data-id");//1;//$(this).parent('td').parent('tr').data('id');
        var data = {};
        data["id"] = id;
        data["task"] = "3";
        data["type"] = "1";
        var jsondata = JSON.stringify(data);
       // console.log("#playlist-details-"+id);
        $.post("../../modules/ajax/cms.php", jsondata, function (result) {
             $('#playlist-modal-body').find("#playlist-details-"+id).html(result);
            
        });
    });
    
    
    $(document).on("click", "#save-screen-playlist", function (e)
    {
        var playlist=[];
        var i=0;
        var id=0;
        var currentElement;
        var repeat_playlist=0;
        var daterange_playlist;
        var from_date;
        var to_date;
        var name;
        screen_id = $(this).attr("data-screen");
        $('.playlist-right').each(function(){
            repeat_playlist=0;
            currentElement = $(this);
            id = currentElement.attr("data-playlist") ;
            name = $('#playlist-modal-body').find("#l"+id).html();
            if ($('#playlist-modal-body').find("#repeat-p"+id).is(":checked"))
            {
                repeat_playlist = 1;  
            }
            
            daterange_playlist = $('#playlist-modal-body').find('#date-p'+id).val();
            from_date = daterange_playlist.substring(0, 10);
            to_date = daterange_playlist.substring(13, 23);
            if(from_date=="")
            {
                from_date="00-00-00";
            }
            if(to_date=="")
            {
                to_date="00-00-00";
            }
            start_time = $('#playlist-modal-body').find('#start-time'+id).val();
            if(start_time=="")
            {
                start_time="00:00:00";
            }
            end_time = $('#playlist-modal-body').find('#end-time'+id).val();
            if(end_time=="")
            {
                end_time="00:00:00";
            }
            
            playlist.push({
                id: id, 
                repeat:repeat_playlist,
                start_date:from_date,
                end_date:to_date,
                start_time:start_time,
                end_time:end_time,
                name:name
            });
         });
        var data = {};
        data["device_id"] = screen_id;
        data["task"] = "8";
        data["playlists"]=playlist;
        var jsondata = JSON.stringify(data);
        console.log(jsondata);
        $.post("../../modules/ajax/cms.php", jsondata, function (result) {
             location.reload();
            
        });
    });
    
    $(document).on("click", ".edit-playlist", function (e)
    {
        var id =$(this).attr("data-id");
        if(edit==true)
        {
            addeditplaylist(id,e);
        }
        else
        {
            edit=true;
            $("#pl-options-"+id).show();
            $("#pl-options-"+id).removeClass("hidden");
        }
        
    });
    
  /*  $("#view_playlist_modal" ).scroll(function() {
        $('.scheduledate').daterangepicker('place')
    });*/
    
    

</script>
