var task, device_ip, device_number, carpark_number, device_name, description, movement_type, device_type, controller_task;
//$(".btn-open-barrier, .btn-close-barrier, .btn-restart-machine, .btn-barrier-status, .btn-close-lane, .btn-open-lane, .btn-free-passage,.btn-standard-operation").click(function () {
$(document).on("click", ".btn-open-barrier, .btn-close-barrier, .btn-restart-machine, .btn-barrier-status, .btn-close-lane, .btn-open-lane, .btn-free-passage,.btn-standard-operation", function (){
    var value = $(this).attr('value');   
    device_number = $(this).attr('id');


        device_name = $("#device_details_" + device_number).attr('device_name');
        carpark_number = $("#device_details_" + device_number).attr('carpark_number');
        device_ip = $("#device_details_" + device_number).attr('device_ip');
        device_type = $("#device_details_" + device_number).attr('device_type');
        switch (value)
        {
            case "Open Barrier":
                task = "S01";
                controller_task = 1;
                movement_type = 3;
                description = "Barrier Open From Server"
                break;
            case "Close Barrier":
                task = "S02";
                controller_task = 2;
                movement_type = 4;
                description = "Barrier Close From Server"
                break;
            case "Open Barrier1":
                controller_task = 1;
                movement_type = 3;
                description = "Barrier Open1 From Server"
                break;
            case "Close Barrier1":
                controller_task = 2;
                movement_type = 4;
                description = "Barrier Close1 From Server"
                break;
            case "Open Barrier2":
                controller_task = 3;
                movement_type = 3;
                description = "Barrier Open2 From Server"
                break;
            case "Close Barrier2":
                controller_task = 4;
                movement_type = 4;
                description = "Barrier Close2 From Server"
                break;

            case "Restart":
                controller_task = 3;
                task = "S03";
                movement_type = 0;
                description = "Restart Machine From Server"
                break;


            case "Lane Closed Mode":
                controller_task = 5;
                task = "S05";
                movement_type = 0;
                description = "Lane Closed Mode From Server"
                break;


            case "Free Passage Mode":
                controller_task = 5;
                task = "S04";
                movement_type = 0;
                description = "Free Passage Mode From Server"
                break;

            case "Standard Operation Mode":
                controller_task = 5;
                task = "S06";
                movement_type = 0;
                description = "Standard Operation Mode From Server"
                break;
        }
        $('#detailModal').modal('show');
    

});


$(function ()
{
    //modal cancel	
    $(document).on('click', '#cancel_reason', function (){
        $('#reason_text').val("");
        $('#detailModal').modal('hide');
    });
    //modal ok
    $(document).on('click', '#ok_reason', function ()
    {
        var reason = $('#reason_text').val();
        if (reason != "")
        {
            $('#detailModal').modal('hide');
            var operator = $("#username2").text();
            var data = {
                device_number: device_number,
                device_ip: device_ip,
                device_name: device_name,
                description: description,
                carpark_number: carpark_number,
                task: task,
                operator: operator,
                reason: reason,
                device_type: device_type,
                movement_type: movement_type
            };
            var jsontemp = JSON.stringify(data);

            if (device_type == 6 || device_type == 7)
            {
                var url = "http://" + device_ip + "/services/ParkingLaneManagement.php";
                var data = {task: controller_task};
                var temp = JSON.stringify(data);
                $.post(url, temp)
                        .done(function (result)
                        {
                            console.log(result);
                            if (result == 1)
                            {
                                $.post("../ajax/operations.php?task=1", jsontemp)
                                        .done(function (result) {
                                            if (result == 0)
                                                alert("Insert to Db failed");
                                            else
                                                alert(device_name + " " + description + " Success");
                                        }, "json");
                            } else
                                alert(device_name + " " + description + " Failed");
                        }, "json")
                        .fail(function () {
                            alert("Not reachable");
                        });
            } else
            {
                $.post("../ajax/operations.php?task=1", jsontemp)
                        .done(function (result) {
                            if (result == 1)
                                alert(device_name + " " + description + " Success");
                            else
                                alert(device_name + " " + description + " Failed");
                        }, "json");
            }
            $('#reason_text').val("");
        } else
        {
            alert("Please enter a valid reason");
        }
    });
});