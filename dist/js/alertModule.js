$(function()
{    
    var idInMsg="";
$("#containerCondition").hide();
$("#divTimeInterval").hide();
$("#value2").hide(); 


$(".draggable").draggable({
        containment:'#right',
        helper:'clone'  ,       
            });
            
$("#right").droppable({
    accept:".draggable",
    activeClass:"ui-state-default",
    hoverClass:"ui-state-hover",
    drop:function(event,ui){        
        $("#right").append(ui.draggable.text()+"<br>");           
        var id=ui.draggable.attr("id");
        idInMsg=idInMsg+id+",";
        $("#"+id).draggable("disable");
    }
});   


//select time interval
$("#TimeInterval").click(function(){    
     $("#containerCondition").show();
    $("#divCondition").hide(); 
    $("#minuteSelect2").show();
    $("#minuteSelect").hide();
    $("#timeControl").show();    
    $("#divEmail").show();
    $("#divMobile").hide();
    $("#messgeData").show();
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false); 
    var height=$("#left").height();
    $("#right").height(height);
    $("#middle").height(height);
    });
    
//select condition
$("#condition").click(function(){
    $("#containerCondition").show();
    $("#divCondition").show();
    $("#timeControl").show();     
    $("#divEmail").show();
    $("#minuteSelect2").hide();
    $("#minuteSelect").show();
    $("#divMobile").hide();
    $("#messgeData").show();
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false);  
    var height=$("#left").height();
    $("#right").height(height);
    $("#middle").height(height);
    });
    
//select network status
$("#NetworkStatus").click(function(){
    $("#containerCondition").show();
    $("#divCondition").hide(); 
    $("#minuteSelect2").show();
    $("#minuteSelect").hide();
    $("#timeControl").show();    
    $("#divEmail").show();
    $("#divMobile").hide();
    $("#messgeData").hide();
    $("#chkEmail:checked").prop("checked",true);
    $("#chkSms:checked").prop("checked",false);   
    });
    
//show textbox 2 for "between   condition"
$("#conditionList").on("change",function(){
    if($("#conditionList").val()=="between")
        {
            $("#value2").show();
        }
        else
        {
          $("#value2").hide();  
        }
    }); 
    
//email checkbox    
$("#chkEmail").on("click",function(){
     $("#divEmail").toggle(this.checked);
    });  
    
//message checkbox    
$("#chkSms").on("click",function(){
     $("#divMobile").toggle(this.checked);
    }); 
    
//save button
$("#saveButton").on("click",function(){
    
//if condition is selected
    if($("#condition").is(":checked"))
        {          
            var data={};
            data["counter_id"]=$("#counterList :selected").val();  
            data["counter_name"]=$("#counterList option:selected").text();  
            var condition=$("#conditionList").val();
            
            if(condition=="between")
                {
                 data["condition"]="between "+$("#value1").val()+" and "+$("#value2").val();   
                }
            else
                {
                 data["condition"]=$("#conditionList").val()+" "+$("#value1").val();
                }
                
            if($("#everyHour").is(":checked"))
                data["hour"]=$("#hourSelect").val(); 
            
            if($("#everyMinute").is(":checked"))                                  
                    data["minute"]=$("#minuteSelect").val();
                
                   
            
            data["startdateTime"]=$("#datePicker").val() +" "+$("#timePicker").val();
                
            data["datamsg"]=$("#right").html();   
            data["datamsgId"]=idInMsg;
            
            if($("#chkEmail").is(":checked"))
                {
                    data["email"]=$("#email1").val()+","+$("#email2").val()+","+$("#email3").val()+","+$("#email4").val()+","+$("#email5").val();                                                                    
                }
                
            if($("#chkSms").is(":checked"))
                {
                    data["mobile"]=$("#mobile1").val()+","+$("#mobile2").val()+","+$("#mobile3").val()+","+$("#mobile4").val()+","+$("#mobile5").val();                                                                   
                }
                                                
        var temp=JSON.stringify(data);           
       // alert(temp);
        $.ajax({
            type: "post",
            url: "../../modules/ajax/alert.php?type=1",
            data: temp,
            cache:false,
            contentType:"application/json",	
            dataType:"json",                             				
            success:function(data){                                 
                                if(data['status']==200)
                                    alert(data['msg']);
                                },				
            error:function(jqXHR,textStatus,errorThrown){                                                                                
                                        alert(errorThrown+" "+textStatus);                                       
                                                        }
            });	//end of ajax      
            
        }
        
//if time interval is selcted   
    if($("#TimeInterval").is(":checked"))
        {
           var data={};
           
            if($("#everyHour").is(":checked"))
                data["hour"]=$("#hourSelect").val(); 
            
            if($("#everyMinute").is(":checked"))
                data["minute"]=$("#minuteSelect2").val(); 
            
            data["startdateTime"]=$("#datePicker").val() +" "+$("#timePicker").val(); 
                
            data["datamsg"]=$("#right").html();   
            data["datamsgId"]=idInMsg;
            
            if($("#chkEmail").is(":checked"))
                {
                    data["email"]=$("#email1").val()+","+$("#email2").val()+","+$("#email3").val()+","+$("#email4").val()+","+$("#email5").val();                                                                    
                }
                
            if($("#chkSms").is(":checked"))
                {
                    data["mobile"]=$("#mobile1").val()+","+$("#mobile2").val()+","+$("#mobile3").val()+","+$("#mobile4").val()+","+$("#mobile5").val();                                                                   
                }
                            
                          
        var temp=JSON.stringify(data);   
       //alert(temp);
        $.ajax({
            type: "post",
            url: "../../modules/ajax/alert.php?type=2",
            data: temp,
            cache:false,
            contentType:"application/json",	
            dataType:"json",                             				
            success:function(data){                                 
                                if(data['status']==200)
                                    alert(data['msg']);
                                },				
            error:function(jqXHR,textStatus,errorThrown){                                                                                
                                        alert(errorThrown+" "+textStatus);                                       
                                                        }
            });	//end of ajax      
        }
        
    //if NetworkStatus is selcted   
    if($("#NetworkStatus").is(":checked"))
        {
           var data={};
           
            if($("#everyHour").is(":checked"))
                data["hour"]=$("#hourSelect").val(); 
            
            if($("#everyMinute").is(":checked"))
                data["minute"]=$("#minuteSelect2").val(); 
            
            data["startdateTime"]=$("#datePicker").val() +" "+$("#timePicker").val();                             
            
            if($("#chkEmail").is(":checked"))
                {
                    data["email"]=$("#email1").val()+","+$("#email2").val()+","+$("#email3").val()+","+$("#email4").val()+","+$("#email5").val();                                                                    
                }
                
            if($("#chkSms").is(":checked"))
                {
                    data["mobile"]=$("#mobile1").val()+","+$("#mobile2").val()+","+$("#mobile3").val()+","+$("#mobile4").val()+","+$("#mobile5").val();                                                                   
                }
                            
                          
        var temp=JSON.stringify(data);   
      // alert(temp);
        $.ajax({
            type: "post",
            url: "../../modules/ajax/alert.php?type=3",
            data: temp,
            cache:false,
            contentType:"application/json",	
            dataType:"json",                             				
            success:function(data){                                 
                                if(data['status']==200)
                                    alert(data['msg']);
                                },				
            error:function(jqXHR,textStatus,errorThrown){                                                                                
                                        alert(errorThrown+" "+textStatus);                                       
                                                        }
            });	//end of ajax      
        }
    });
    

        
});  

