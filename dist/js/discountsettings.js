$(function()
{
//Disable discounts    
    $(".btn-disable-discount").click(function (event) {
        if($(".btn-disable-discount").val()=="Cancel")
        {
            document.location = "discounts.php";
        }
        else{
            var id = $(this).attr('id');
            var data = {id: id}
            $.ajax({
                type: "post",
                url: "../../modules/ajax/discountsettings.php?type=1",
                data: JSON.stringify(data),
                cache: false,
                contentType: "application/json",
                success: function (data) {
                        //alert(data);
                        location.reload(true);
    
                },
                error: function (jqXHR, textStatus, errorThrown) {
    
                    alert(textStatus);alert(errorThrown);                                       
                } //end of error
            }); //end of ajax
        }

       
    }); //end of Disable discounts


//Enable discounts
    $(".btn-enable-discount").click(function (event) {
        var id = $(this).attr('id');
        var data = {id: id}
        $.ajax({
            type: "post",
            url: "../../modules/ajax/discountsettings.php?type=2",
            data: JSON.stringify(data),
            cache: false,
            contentType: "application/json",
            success: function (data) {
                //alert(data);
                location.reload(true);

            },
            error: function (jqXHR, textStatus, errorThrown) {

                alert(textStatus);alert(errorThrown);                                       
            } //end of error
        }); //end of aajax
        
    }); //end of Enable discounts

//Add discounts
    $("#add_discount").click(function (event) {  
        var datastring = {};
         datastring['discount_name'] = $('#discount_name').val();
         datastring['discount_type'] = $('#discount_type').val();
         datastring['discount_option'] = $('#discount_option').val();
         datastring['discount_category'] = $('#discount_category').val();
         datastring['discount_value'] = $('#discount_value').val();
         datastring['cp_name'] = ($('#cp_name').val()).split(":")[0];
         if ($("#status").is(":checked"))
        {
                datastring['status'] = 1;
        }
        else 
        {
            datastring['status'] = 0;
        }
        $.ajax({
             type: "post",
             url: "../../modules/ajax/discountsettings.php?type=3",
             data: JSON.stringify(datastring),
             cache: false,
             contentType: "application/json",
             success: function (data) {
                 location.reload(true);

             },
             error: function (jqXHR, textStatus, errorThrown) {

                 alert(textStatus);alert(errorThrown);                                       
             } //end of error
         }); //end of ajax
        
    }); //end of Add discounts


//Edit Devices

        
});  
function EditDiscount(id,cp)
{
    //var id = $(this).attr('id');
    //alert("here"+id);
    var discount_name = $('#discount_name'+id).html();
    var discount_type = $('#discount_type'+id).html();
    var discount_option = $('#discount_option'+id).html();
    var discount_category = $('#discount_category'+id).html();
    var discount_value = $('#discount_value'+id).html();
    var carpark_name = $('#carpark_number'+id).html();
    var carpark_id = "";
    $('#discount_name'+id).html("<input type = 'text' id = 'discount_name_text"+id+"' name = 'discount_name_text"+id+"'  value = '"+discount_name+"' >");
    $('#discount_type'+id).html("<select name = 'discount_type_text"+id+"' id = 'discount_type_text"+id+"'><option value = 'amount'>amount</option><option value = 'percentage'>percentage</option></select>");
    $('#discount_option'+id).html("<select name = 'discount_option_text"+id+"' id = 'discount_option_text"+id+"'><option value = 'coupon' >coupon</option><option value = 'pos'>pos</option></select>");
    $('#discount_category'+id).html("<input type = 'text' id = 'discount_category_text"+id+"' name = 'discount_category_text"+id+"'  value = '"+discount_category+"' >");
    $('#discount_value'+id).html("<input type = 'text' id = 'discount_value_text"+id+"' name = 'discount_value_text"+id+"' value = '"+discount_value+"' >");
    var carpark = "<select name = 'carpark_number_text"+id+"' id = 'carpark_number_text"+id+"'>";
    
    for(var key in cp)
    {
        if(cp.hasOwnProperty(key))
        {
             carpark += "<option value = '"+cp[key].carpark_id+":"+cp[key].carpark_name+"' >"+cp[key].carpark_name+"</option>";
        }
        if(cp[key].carpark_name==carpark_name)
        {
            carpark_id = cp[key].carpark_id;
        }
    }
    $('#carpark_number'+id).html(carpark);        
    $('#editbtn'+id).html("<input class='btn btn-info btn-block btn-edit-discount' id ='editbtn"+id+"' type='submit'  value='Save' onclick='EditDiscountRecord("+id+")' />");
    $('.btn-disable-discount').attr('value','Cancel');
   
    $('#discount_option_text'+id).val(discount_option); 
    $('#discount_type_text'+id).val(discount_type); 
    $('#carpark_number_text'+id).val(carpark_id+":"+carpark_name);

    
    event.preventDefault();
}

function EditDiscountRecord(id)
{
    var datastring = {};
    datastring['discount_id'] = $('#discount_id'+id).html();
    datastring['discount_name'] = $('#discount_name_text'+id).val();
    datastring['discount_type'] = $('#discount_type_text'+id).val();
    datastring['discount_option'] = $('#discount_option_text'+id).val();
    datastring['discount_category'] = $('#discount_category_text'+id).val();
    datastring['discount_value'] = $('#discount_value_text'+id).val();
    if($('#carpark_number_text'+id).val()==null)
    {
        datastring['cp_name'] = 0;
    }
    else{
        datastring['cp_name'] = ($('#carpark_number_text'+id).val()).split(":")[0];
    }
    var jsondata = JSON.stringify(datastring);
        $.ajax({
        type:'post',
        url:"../../modules/ajax/discountsettings.php?id="+id+"&type=4",
        data:jsondata,
        cache:false,
        contentType:'application/json',
        dataType:'json',
        success:function(data){
           // alert(data);
            location.reload();
        },
        error:function(jqXHR,textStatus,errorThrown)
        {            
            alert(errorThrown+" "+textStatus);
        }
        });
}





//end of Edit Devices