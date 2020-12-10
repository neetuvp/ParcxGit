$(function()
{     
    //Edit Button
    $(".edit-correction-factor").on("click", function () {
       var btnid=$(this).attr('id');
       var id=btnid.replace('btn','');
          
       
       if($(this).val()=="Save")       
        {   
            var correctionFactor=$("#txtCorrection"+id).val();   
            var data={};
            data["counter_id"]=id;
            data["value"]=correctionFactor;
            var temp=JSON.stringify(data);  
            
            //alert(temp);
            //update
            $.ajax({
            type: "post",
            url: "../../modules/ajax/counter.php?type=1",
            data: temp,
            cache:false,
            contentType:"application/json",	
            //dataType:"json",                             				
            success:function(data){                                
                                alert(data);                                                               
                                window.location.reload();
                                },				
            error:function(jqXHR,textStatus,errorThrown){                                                                                
                                        alert(errorThrown+" "+textStatus);                                       
                                                        }
            });	//end of ajax   
            
            //$("#val"+id).html(correctionFactor);
            //$("#"+btnid).val('Edit');
        }
       else if($(this).val()=="Edit")
        {
            var correctionFactor=$("#val"+id).text(); 
            $("#val"+id).html("<input type='text'  class='txtCorrection' id='txtCorrection"+id+"' value='"+correctionFactor+"'>");       
            $("#"+btnid).val('Save');
        }       
       
    });
    
   
    
});
