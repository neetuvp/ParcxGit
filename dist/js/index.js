$(function()
{ 
    $("#login").click(function(event)
    { 
    event.preventDefault();
    var data = {};
    data["user_name"] = $("#username").val();
    data["password"] = $("#password").val();
    data["task"] = "12";
    var jsondata = JSON.stringify(data);
    console.log(jsondata);
    $.post("../../parcx/modules/ajax/users.php", jsondata, function (result) {
      console.log(result);
        if (result == "Success")
          window.location="../../parcx/modules/dashboard/device_status_facility.php";
        else
        $("#messagebox").html(result);
      });
    });
});

//End Login
