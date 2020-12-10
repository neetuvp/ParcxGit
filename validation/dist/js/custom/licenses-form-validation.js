/* === form validation === */

$(document).ready(function () {

  
    /* === add validator form === */
var formElement = $("#addvalidator")

    var rules_set = {
        ConfirmPassword: {
            equalTo: "#password",
            required: true
        },
        Username: {
            required: true,
            email: true      
        }
    }

    formElement.find('input[type=text]').each(function(){
        var name = $(this).attr('name');
        rules_set[name] = 'required';
    });

    formElement.validate({
        rules: rules_set,
        messages: {
            ConfirmPassword: {
                required: "Retype your new password",
                equalTo: "Passwords don't match, please check"
            }
        },
        errorElement: "div",
        errorPlacement: function (error, element) {
            error.addClass("text-danger");
            error.insertAfter(element);
        },
        submitHandler: function () 
          {			
          event.preventDefault();
			    //console.log("add operator");
          var data = {};
           
          data['password'] = $("#password").val();              
          data['operator_name'] = $("#account-name").val();           
			    data['email'] = $("#username").val();            
          data['mobile'] = $("#mobile").val();
          data['phone'] = $("#phone").val();
            							
		    data['carparks'] =$("#multiselect").val().toString();			        
          if ($("#validation-report-view").is(":checked"))
              data['validation_view_report'] = "1";
          else
              data['validation_view_report'] = "0";			
			
          if ($("#date-validity").is(":checked")) 
            {
            data['date_validity'] = "1";            
            data['account_activation_date'] = $("#account-active-date").val();				
            data['account_expiry_date'] = $("#account-expiry-date").val();
            }
          else 
				    {
            data['date_validity'] = "0";                
				    }						                                               
          data['validation_product'] = $("#validation-product").val().toString();
          data['validation_ticket_age'] = $("#validation-ticket-age").val();
          data['validation_max_hours'] = $("#validation-max-hours").val();
          data['validation_daily_hours'] = $("#validation-daily-hours").val();
          data['validation_weekly_hours'] = $("#validation-weekly-hours").val();
          data['validation_monthly_hours'] = $("#validation-monthly-hours").val();							
            
          if($('#add_validator').val()=="Submit")
            data["option-type"] = 3;
          else
            {
            data["option-type"] = 7;
            data["user_id"]=id;
            //console.log("id:"+id);
            }
              
            var upload=true;
            data["logo"] = "user.jpg";
          //Logo upload
          var file_data = $("#logo").prop("files")[0];

          
          if (typeof file_data !== "undefined") 
            {
            var file_name = file_data.name;
            var extension = file_name.split('.').pop();

            var name = Date.parse(new Date()).toString();
            name = name + "." + extension;
            data["logo"] = name;
            var jsondata = JSON.stringify(data);				
            var form_data = new FormData();
            form_data.append("file", file_data);
            form_data.append("name", name);

            $.ajax({
                url: "../../validation/ajax/fileUpload.php",
                method: "POST",
                data: form_data,
                contentType: false,
                cache: false,
                processData: false,
                success: function (result) 
                  {                        
                  var json = JSON.parse(result);
                  if (json.status != 200)                    
                    {
                    upload=false;  
                    alert(json.Message);
                    }
                }
              });
            }
          if(upload==true)
			{
            var jsondata = JSON.stringify(data);	
           // console.log(jsondata);			                   
            $.post("../../validation/ajax/validation.php", jsondata, function (data) 
              {              
                if(data.indexOf("Successfull") >= 0)					    
                    location.reload();                    
                else
                    alert(data);
              })
            .fail(function (jqxhr, status, error) 
              {
              alert("Error: " + error);
              });
                  
            }
        }
    });
    /* create product */

    formElement = $("#createProduct")

    var rules_set = {}

    formElement.find('input[type=text]').each(function(){
        var name = $(this).attr('name');
        rules_set[name] = 'required';
    });

    formElement.validate({
        rules: rules_set,
        errorElement: "div",
        errorPlacement: function (error, element) {
            error.addClass("text-danger");
            error.insertAfter(element);
        },
        submitHandler: function () {	

            // submit data here
            event.preventDefault();

            $(".add-product-btn").click(function(event) {
                var carparks = [];
                $('.carparks:checkbox:checked').each(function(index, obj) {
                    carparks.push(this.value);
                });
            
            
                var datastring = {};
                datastring['product_id'] = "0";
                datastring['product-name'] = $('#name').val();
                datastring["description"] = $('#description').val();
                datastring["type"] = $('#product_type').val();
                datastring["value"] = 0;
                if ($('#product_type').val() == "Validation") {
                    if ($("#value").val() == "") {
                        alert("Please fill 'Value' field");
                        return;
                    }
            
                    if ($("#date-validity").is(":checked")) {
                        datastring['date_validity'] = "1";
                        datastring["start-date"] = $("#product-active-date_validity").val();
                        datastring["expiry-date"] = $("#product-expiry-date_validity").val();
                    } else {
                        datastring['date_validity'] = "0";
                        datastring["start-date"] = "";
                        datastring["expiry-date"] = "";
                    }
            
                    datastring["days"] = $("#days").val().toString();
                    datastring["time-slots"] = $("#time-slots").val().toString();
                    datastring["validation_type"] = $("#type").val();
                    datastring["value"] = $("#value").val();
                    datastring['carparks'] = carparks.toString();
                } else {
                    datastring['date_validity'] = "0";
                    datastring["start-date"] = $('#product-active-date').val();
                    datastring["expiry-date"] = $('#product-expiry-date').val();
                    datastring["days"] = "";
                    datastring["validation_type"] = "";
                    datastring["time-slots"] = "";
                    datastring["value"] = 0;
                    datastring['carparks'] = carparks.toString();
                }
                datastring["option-type"] = 9;
                datastring["operator_id"] = $("#operator_id").val();
                $.ajax({
                    type: "post",
                    url: "ajax/system_user.php",
                    data: JSON.stringify(datastring),
                    cache: false,
                    contentType: "application/json",
                    success: function(data) {
                        location.reload(true);
            
                    },
                    error: function(jqXHR, textStatus, errorThrown) {
                        alert(textStatus);
                        alert(errorThrown);
                    } //end of error
                }); //end of ajax
                //}
                //event.preventDefault();
            }); //end of Add Product

        }
    });
   

});