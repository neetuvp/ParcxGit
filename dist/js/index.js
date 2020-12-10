$(function()
{ 

    $(".btn.btn-primary.btn-block.btn-flat").click(function(event)
    { 
        event.preventDefault();
        
    var username=$("#username").val();
    var password=$("#password").val();
    var data={username:username,password:password};   
    var temp=JSON.stringify(data); 
		//alert(temp);
    $.ajax({
            type: "post",
            url: "../../parcx/modules/ajax/admin.php?type=6",
            data: temp,
            async:false,
            contentType:"application/json",
            dataType:"json",	
            success:function(data)
                            {   
                              if(data["status"]==200){     
                                  //window.location = "home.php"; 
                                window.location="../../parcx/modules/dashboard/deviceStatus.php";
                              }
                               else
                                   $("#messagebox").html(data["message"]); 

                            },				
            error:function(jqXHR,textStatus,errorThrown){
                        alert("Cant connect to DBServer");
                                                         
                                        }//end of error
                                    });	//end of aajax

    });
    });

//End Login
