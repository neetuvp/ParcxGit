var task, device_ip, device_number, carpark_number, device_name, description,description_text, movement_type, device_type, controller_task;
var operation_mode=-1;
var reason,reason_text;


//$(".btn-open-barrier, .btn-close-barrier, .btn-restart-machine, .btn-barrier-status, .btn-close-lane, .btn-open-lane, .btn-free-passage,.btn-standard-operation").click(function () {
$(document).on("click", ".btn-open-barrier, .btn-close-barrier, .btn-restart-machine, .btn-barrier-status, .btn-close-lane, .btn-open-lane, .btn-free-passage,.btn-standard-operation", function (){
    var value = $(this).attr('data-value'); 
    var operation = $(this).attr('value'); 
    device_number = $(this).attr('id');
    operation_mode=-1;
    description_text = $(this).attr('data-description'); 
    reason_text = $(this).attr('data-reason');

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
                reason="Open barrier for "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;
            case "Close Barrier":
                task = "S02";
                controller_task = 2;
                movement_type = 4;
                description = "Barrier Close From Server"
                reason="Close barrier for "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;
            case "Open Barrier1":
                controller_task = 1;
                movement_type = 3;
                description = "Barrier Open1 From Server";
                reason_text = reason_text +" "+ device_name;
                break;
            case "Close Barrier1":
                controller_task = 2;
                movement_type = 4;
                description = "Barrier Close1 From Server";
                reason_text = reason_text +" "+ device_name;;
                break;
            case "Open Barrier2":
                controller_task = 3;
                movement_type = 3;
                description = "Barrier Open2 From Server";
                reason_text = reason_text +" "+ device_name;
                break;
            case "Close Barrier2":
                controller_task = 4;
                movement_type = 4;
                description = "Barrier Close2 From Server";
                reason_text = reason_text +" "+ device_name;
                break;

            case "Restart":
                controller_task = 3;
                task = "S03";
                movement_type = 0;
                description = "Restart Machine From Server";
                reason="Restart "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;


            case "Lane Closed Mode":
                controller_task = 5;
                task = "S05";
                movement_type = 0;
                description = "Lane Closed Mode From Server";
                operation_mode=2;
                reason="Change operation mode to Lane closed for "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;


            case "Free Passage Mode":
                controller_task = 5;
                task = "S04";
                movement_type = 0;
                description = "Free Passage Mode From Server";
                operation_mode=1;
                reason="Change operation mode to Free Passage for "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;

            case "Standard Operation Mode":
                controller_task = 5;
                task = "S06";
                movement_type = 0;
                description = "Standard Operation Mode From Server";
                operation_mode=0;
                reason="Change operation mode to Standard operation mode for "+device_name;
                reason_text = reason_text +" "+ device_name;
                break;
        }
        $('#reasonempty').html("");
        //$("#reason_heading").html(reason);
        $("#reason_heading").html(reason_text);
        $("#message-modal-heading").html(operation);
        $('#detailModal').modal('show');
        
    

});


$(function ()
{
    //modal cancel	
    $(document).on('click', '#cancel_reason', function (){
        $('#reason_text').val("");
        $('#reasonempty').html("");
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
                movement_type: movement_type,
                operation_mode:operation_mode};
            var jsontemp = JSON.stringify(data);
            
            console.log(jsontemp);

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
                                                alertMessage("Insert to Db failed");
                                            else
                                                alertMessage(device_name + " " + description + " Success");
                                        }, "json");
                            } else
                                alertMessage(device_name + " " + description + " Failed");
                        }, "json")
                        .fail(function () {
                            alertMessage("Not reachable");
                        });
            } else
            {
                $.post("../ajax/operations.php?task=1", jsontemp)
                        .done(function (result) {
                            console.log(result);
                               // alertMessage(device_name + " " + description +"  "+ result);
                                alertMessage(device_name + " " + description_text +"  "+ result);
                            
                        }, "json");
            }
            $('#reason_text').val("");
            $('#reasonempty').html("");
        } else
        {
           // $("#reasonempty").html("Please enter a valid reason");
           $("#reasonempty").html(valid_reason_message);
        }
    });
    
$("#language").change(function()
{	 
    loadReportLabels(); 
    loadManualOperationReport();
    loadOperationList();
    
});
function loadManualOperationReport()
{
    var data={};
    data["carpark"] = "";
    data["operation"] = "";
    data["language"] = $("#language").val();  
    data["task"] = 11;
    data["limit"] = 10;
    var json = JSON.stringify(data);
    console.log(json);
    $.post("../ajax/reports.php",json,function(data)
    {
        $("#latestmanualreport").html(data);
    });   
	
}
function loadOperationList()
{
    var data={};
    data["language"] = $("#language").val();  
    var json = JSON.stringify(data);
    console.log(json);
    $.post("../ajax/operations.php?task=2",json,function(data)
    {
        $("#manualoperationlist").html(data);
    });   
	
}
function loadReportLabels()    
{
    
	var data={};
	data["task"]=29;
	data["language"]=$("#language").val();    
	data["page"]=26;
	var json = JSON.stringify(data);
	//console.log(json);
	$.post("../ajax/reports.php",json,function(data)
	{	
            //console.log(data);	            
            var json=JSON.parse(data);
            $("#pdf-report-header").html(json.manual_operation);  
            $("#device_name_label").html(json.device_name);  
            $("#device_ip_label").html(json.device_ip);  
            $("#pdf-report-header").html(json.manual_operation);  
            $("#logout").html(json.logout); 
            $("#modal_title").html(json.manual_operation); 
            $("#reason_label").html(json.reason); 
            $("#ok_reason").html(json.ok);
            $("#ok_sidebar_modal").html(json.ok);
            $("#cancel_reason").html(json.cancel); 
            valid_reason_message = json.enter_valid_reason;
	});   
}
});