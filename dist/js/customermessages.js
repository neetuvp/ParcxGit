var message_limit;
var upload_limit;
var upload_path;
$(document).ready(function () {
    $('#device1').removeClass('btn-info');
    $('#device1').addClass('btn-success');
    $('#language_English').removeClass('btn-info');
    $('#language_English').addClass('btn-success');
    var data = {};
    data["type"] = 1;
    data["language"] = "English";
    data["task"] = 1;
    var jsondata = JSON.stringify(data);
    $.post("../../modules/ajax/customer_messages.php?", jsondata, function (data) {
        $("#messagedata").html(data);
    });   
                   
    data = {}
    data["task"] = 5;
    jsondata = JSON.stringify(data);
    $.post("../../modules/ajax/customer_messages.php", jsondata, function (data) {
        var json = JSON.parse(data);
        message_limit = json.message_limit;
        upload_limit = json.upload_limit;
        upload_path = json.media_path;
    });            
});


function ShowMessagesByDeviceType(type)
    {
    $('.btn-device_type').removeClass('btn-success');
    $('.btn-device_type').addClass('btn-info');
    $('#device' + type).addClass('btn-success');
    $('#device' + type).removeClass('btn-info');
    $('.btn-cancel-parking').css("visibility", "hidden");
    $('#device_type').val(type);
    var data = {};
    data["type"] = type;
    data["language"] = $('#message_language').val();
    data["task"] = 1;

    var jsondata = JSON.stringify(data);
    $.post("../../modules/ajax/customer_messages.php", jsondata, function (data) {
        $("#messagedata").html(data);
    });
    }

function ShowMessagesByLanguage(language)
{
    $('.btn-language').removeClass('btn-success');
    $('.btn-language').addClass('btn-info');
    $('#language_' + language).addClass('btn-success');
    $('#language_' + language).removeClass('btn-info');
    $('.btn-cancel-parking').css("visibility", "hidden");
    $('#message_language').val(language);
    var data = {};
    data["language"] = language;
    data["type"] = $('#device_type').val();
    data["task"] = 1;

    var jsondata = JSON.stringify(data);
    $.post("../../modules/ajax/customer_messages.php", jsondata, function (data) {
        $("#messagedata").html(data);
    });           
}

function CancelEdit(id)
    {
    var device_type = $('#device_type').val();    
    ShowMessagesByDeviceType(device_type);
    }

function EditMessage(id)
    {
    var language = $('#message_language').val();
    $('#' + language + '1' + id).html("<input class='w-100' type = 'text' id = '" + language + "1text" + id + "' name = '" + language + "1text" + id + "' value = '" + $('#' + language + '1' + id).html() + "'>");
    $('#' + language + '2' + id).html("<input class='w-100' type = 'text' id = '" + language + "2text" + id + "' name = '" + language + "2text" + id + "' value = '" + $('#' + language + '2' + id).html() + "'>");
    $('#' + language + '3' + id).html("<input class='w-100' type = 'text' id = '" + language + "3text" + id + "' name = '" + language + "3text" + id + "' value = '" + $('#' + language + '3' + id).html() + "'>");
    $('#editdivfixed' + id).html("<input class='btn btn-info btn-block btn-edit-parking' id ='editmessage" + id + "' type='submit'  value='Save' onclick='EditMessageRecord(" + id + ")' />");  
    $('#cancelbtn' + id).css("visibility", "visible");
    }
    
function EditMessageRecord(id)
    {
    var device_type = $('#device_type').val();
    var language = $('#message_language').val();
    if ($('#' + language + '1text' + id).val().length <= message_limit && $('#' + language + '2text' + id).val().length <= message_limit && $('#' + language + '3text' + id).val().length <= message_limit)
        {
        var data = {};
        data["language"] = language;
        data["line1"] = $('#' + language + '1text' + id).val();
        data["line2"] = $('#' + language + '2text' + id).val();
        data["line3"] = $('#' + language + '3text' + id).val();
        data['id'] = id;
        data["task"] =2;
        var jsondata = JSON.stringify(data);
        $.post("../../modules/ajax/customer_messages.php", jsondata, function (data) {
            ShowMessagesByDeviceType(device_type);
        });
                
        } 
    else 
        {
        alert("Message must be less than " + message_limit + " characters");
        }
    }


function EditMedia(id)
{


    $('#type' + id).html("<input class='w-100' type = 'text' id = 'english1text" + id + "' name = 'typetext" + id + "' value = '" + $('#type' + id).html() + "'>");
    $('#path' + id).html("<input class='w-100' type = 'text' id = 'english2text" + id + "' name = 'pathtext" + id + "' value = '" + $('#path' + id).html() + "'>");
    $('#editdivfixed' + id).html("<input class='btn btn-info btn-block btn-edit-parking' id ='editmessage" + id + "' type='submit'  value='Save' onclick='EditMessageRecord(" + id + ")' />");    
    $('#cancelbtn' + id).css("visibility", "visible");
}


function AddMessage(language)
    {
    var name = "";
    if ($('#message').val() == "")
        {
        alert("Message field is mandatory");
        return;
        }

    for (var j = 0; j < language.length; j++) {
        if ($('#' + language[j] + '_line1').val().length > 25 || $('#' + language[j] + '_line2').val().length > 25 || $('#' + language[j] + '_line3').val().length > 25)
            {
            alert("Message must be less than " + message_limit + " characters");
            return;
            }
        }

    /////////Image  Upload
    if ($("#image_file").val() > '')
        {
        var file_data = $("#image_file").prop("files")[0];
        var form_data = new FormData();
        form_data.append("file", file_data);
        form_data.append("name", file_data.name);
        var size = file_data.size;
        if (size <= upload_limit * 1000)
            {
            name = file_data.name;
            $.ajax({
                url: '../../modules/ajax/uploadfile.php',
                method: 'POST',
                data: form_data,
                contentType: false,
                processData: false,
                success: function (response) {
                    var json = JSON.parse(response);
                    if (json.status == 200) {
                        alert("Upload Successful");
                    } else
                        alert(json.Message);
                },
                error: function (jqXHR, textStatus, errorThrown) {

                    alert(textStatus);
                    alert(errorThrown);
                } //end of error
            });

            } 
        else 
            {
            alert("File size should be less than " + upload_limit + "KB");
            }
        }


    //Preparing data for insertion
    var data = {};
    data['device_type'] = $('#device_type').val();
    data['message'] = $('#message').val();
    if (name == "")    
        data['media_path'] = "";
    else
        data['media_path'] = upload_path + "/" + name;
    
    for (var i = 0; i < language.length; i++) {
        data[language[i] + '_line1'] = $('#' + language[i] + '_line1').val();
        data[language[i] + '_line2'] = $('#' + language[i] + '_line2').val();
        data[language[i] + '_line3'] = $('#' + language[i] + '_line3').val();
    }
    data["languages"] = language;
    data["task"] = 3;
    var jsondata = JSON.stringify(data);   
    $.post("../../modules/ajax/customer_messages.php", jsondata, function (data) {       
        location.reload(true);
     }); 
}

$('#cancel_upload').click(function () {
    $('#uploadModal').modal('hide');
});



$('body').on('click', '.openModal', function () {
    id = $(this).attr("data-id");    
    var img_src = $('.openImageDialog' + id).attr('src');    
    $(".modal-body #imgmodal").attr("src", img_src);    
});



$('body').on('click', '#confirm_upload', function () {
    //alert("here");
    /////////Image  Upload
    if ($("#upload_file").val() > '')
    {
        var file_data = $("#upload_file").prop("files")[0];
        var form_data = new FormData();
        form_data.append("file", file_data);
        form_data.append("name", file_data.name);
        var size = file_data.size;
        name = file_data.name;
        if (size <= upload_limit * 1000)//if size < 100 kb
        {
            $.ajax({
                url: '../../modules/ajax/uploadfile.php',
                method: 'POST',
                data: form_data,
                contentType: false,
                processData: false,
                success: function (response) {                    
                    var json = JSON.parse(response);
                    
                    if (json.status == 200) {
                        alert("Upload Successful");

                        //Preparing data for insertion
                        var data = {};
                        data['message'] = id;
                        data['media_path'] = upload_path + "/" + file_data.name;

                        var jsondata = JSON.stringify(data);  
                        $.post("../../modules/ajax/customer_messages.php?type=5", jsondata, function (data) {
                           location.reload(true);
                        });                          
                    } else
                        alert(json.Message);
                },
                error: function (jqXHR, textStatus, errorThrown) {

                    alert(textStatus);
                    alert(errorThrown);
                } //end of error
            });
        } else {
            alert("File size should be less than " + upload_limit + "KB");
        }

    } else {
        alert("No file selected")
    }

});


