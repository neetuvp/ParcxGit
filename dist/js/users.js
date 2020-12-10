$(function()
{ 
    
$("#AddUser").hide(); 
$("#ViewDisableUser").hide(); 
$("#ManageUser").show();


$("#ManageUserButton").click(function(event)
{ event.preventDefault();
  $("#ManageUser").show(); 
  $("#ViewDisableUser").hide(); 
  $("#AddUser").hide();
});
 

$("#AddUserButton").click(function(event)
{ event.preventDefault();
  $("#AddUser").show();
  $("#ViewDisableUser").hide(); 
  $("#ManageUser").show();
});
 


 $("#ViewDisableUserButton").click(function(event)
{ event.preventDefault();
  $("#ViewDisableUser").show(); 
  $("#AddUser").hide();
  $("#ManageUser").hide();
});
   
// Add User Section

    $("#add_User").click(function(event)
        {
        event.preventDefault();
        if(!UserValidation())
        {
            return false;
        }
        else
         {
        var user=getInputUserData();
        $.ajax({
                type: "post",
                url: "../../modules/ajax/admin.php?type=1",
                data: user,
                cache:false,
                contentType:"application/json",	                             				
                success:function(data){  
                                      $("#username").val('');
                                      $("#operatorname").val('');
                                      $("#email").val('');
                                      $("#usertype").val('');
                                      $("#password_1").val('');
                                      $("#password_2").val('');
                                      $("#companyname").val('');
                                      $("#phone").val('');
                                      $("#fromDate").val('');
                                      $("#toDate").val('');
                                      location.reload(true);

                                    },				
                error:function(jqXHR,textStatus,errorThrown){
                            alert("error");                                   
                                            } //end of error
                                        });	//end of aajax
         }   //end of else                    
     });
 //end of $("#AddUserSubmitbtn").click
  
  
 
//Function Cancel Button

$("#btnCancel").click(function(event){
    event.preventDefault();
    location.reload(true);
    });//end of $("btnCancel"),click()


function getInputUserData()
    {
        var fullname=$("#operatorname").val();
        var username=$("#username").val();
        var email=$("#email").val();
        var usertype=$("#usertype").val();
        var password_1=$("#password_1").val();
        var companyname=$("#companyname").val();
        var phone=$("#phone").val();
        var fromdate=$("#fromDate").val();
        var todate=$("#toDate").val();  
        var data={fullName:fullname,userName:username,userEmail:email,userType:usertype,password:password_1,companyname:companyname,phone:phone,userFromDate:fromdate, userToDate:todate};   
        var temp=JSON.stringify(data);     
        return temp;
    }//end of .getInputData
    

    function getInputAddCategoryData()
    {
        
        var catagoryname=$("#catagoryname").val();
 
        var data={catagoryname:catagoryname};   
        var temp=JSON.stringify(data);     
        return temp;
    } //end of .getInputData
    
    function UserValidation()
    {
        var username=$("#username").val();
        var email=$("#email").val();
        var usertype=$("#usertype").val();
        var password_1=$("#password_1").val();
        var password_2=$("#password_2").val();
        var fromDate=$("#fromDate").val();
        var toDate=$("#toDate").val(); 
        
        
        if(!validateEmail()){
          alert("Please Provide a valid email address");
          return false;  
        } 
       // if(!validatePassword()){
        //   alert("Password More Charecter is Requried");
        //  return false;  
       // } 
        if(password_1!= password_2)
        {
         alert("Password And Confirm password Not Match");   
           return false;
        }
        
        if(fromDate >= toDate){
        alert("Please Provide a Valid Date Range");
         return false;  
        }
        
        else
        return true
    
    } // End Validattion
    

function validateEmail(){
    
        var email=$("#email").val();
        var reg = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;

        if (reg.test(email) == false) 
        {
            alert('Invalid Email Address');
            return false;
        }
        return true;
}

function validatePassword(){

    var password=$("#password_1").val();
    var reg = /^(?=.*[A-Za-z])(?=.*\d)(?=.*[$@$!%*#])[A-Za-z\d$@$!%*#]{8,}$/;

    if (reg.test(password) == false) 
    {
        alert('Password More Charecter is Requried');
        return false;
    }
    return true;
}




// User Disable Button

    $(document).on('click', 'input.btn.btn-disable-user', function () {  
        if($(this).val()=="Cancel")
        {
            location.reload(true);
        }
        else
            {
            var id = $(this).attr('id');
            var data={id:id}
            $.ajax({
            type: "post",
            url: "../../modules/ajax/admin.php?type=7",
            data: JSON.stringify(data),
            cache:false,
            contentType:"application/json",	                             				
            success:function(data){ 
            location.reload(true);
            
                            },				
            error:function(jqXHR,textStatus,errorThrown){
                    
                    // alert(textStatus);alert(errorThrown);                                       
                                        }//end of error
                                    });	//end of aajax
            }

    });

//End User Disable Button

// User Enable Button

    $(document).on('click', 'input.btn.btn-enable-user', function () {
        var id = $(this).attr('id');
        var data={id:id}
        $.ajax({
            type: "post",
            url: "../../modules/ajax/admin.php?type=8",
            data: JSON.stringify(data),
            cache:false,
            contentType:"application/json",	                             				
            success:function(data){ 

            location.reload(true);
 
             },				
            error:function(jqXHR,textStatus,errorThrown){
                
                //alert(textStatus);alert(errorThrown);                                       
                        }//end of error
                        });//end of aajax

    });
  // End User Enable Button

  //change password

    $(document).on('click', '.change_password_button', function () {
      var id = $(this).attr('id');
      id=id.substring(8);
        $("#id").val(id);              

  });
  // End change password

    $(document).on('click', '#btnChangePassword', function () {
        var id = $("#id").val();       
        var currentPass=$("#current_password").val();
        var newPass = $("#new_password").val();
        var confirmPass = $("#confirm_password").val();
        $("#changePasswordModal").modal("toggle");
        $("#current_password").val("");
        $("#new_password").val("");
        $("#confirm_password").val("");
        if(newPass==confirmPass)
           {
           var data={id:id,currentPassword:currentPass,newPassword:newPass};            
            $.ajax({
                type: "post",
                url: "../../modules/ajax/admin.php?type=11",
                data: JSON.stringify(data),
                cache: false,
                contentType: "application/json",
                success: function (data) {
                    alert(data);
                },
                error: function (jqXHR, textStatus, errorThrown) {
                    alert("Error for conformation");
                    alert(textStatus);
                    alert(errorThrown);
                } //end of error
            }); //end of aajax
            }
        else
            alert("New Password and conform password not matching");

    });
  // End change password


//Edit user details
    $(document).on('click', '.edit-user-button', function() {
        var btnid = $(this).attr('id');
        id = btnid.substring(4);
        $("#"+id).val("Cancel");
        if($(this).val()=="Edit")
            {
            $("#" + btnid).val('Save');            
            var name = $("#name-" + id).text();
            var role = $("#role-" + id).text();
            var email = $("#email-" + id).text();
            var company = $("#company-" + id).text();
            var phone = $("#phone-" + id).text();
            var from = $("#from-" + id).text();
            var to = $("#to-" + id).text();
            $("#name-" + id).html("<input type='text'  class='col' id='txtname' value='" + name + "'>");
            $("#role-" + id).html("<select   class='col' id='txtrole'></select>");
            $("#email-" + id).html("<input type='text'   class='col' id='txtemail' value='" + email + "'>");
            $("#company-" + id).html("<input type='text'  class='col'  id='txtcompany' value='" + company + "'>");
            $("#phone-" + id).html("<input type='text'   class='col' id='txtphone' value='" + phone + "'>");
            $("#from-" + id).html("<input type='date'   class='col-md-10' id='txtfrom' value='" + from + "'>");
            $("#to-" + id).html("<input type='date'   class='col-md-10' id='txtto' value='" + to + "'>");       
                        
            
            $.ajax({
                type: "GET",                
                url: "../../modules/ajax/admin.php?type=9",                                                 				
                success:function(data){                   
                        $("#txtrole").html(data);
                        $("select option:contains("+role+")").prop("selected",true);
                        },				
                    error:function(jqXHR,textStatus,errorThrown){
                    alert("Error");                                                     
                            }//end of error
                            });//end of aajax*/
            }
        else
            {
                //save
            var name=$("#txtname").val();
            var role = $("#txtrole").val();
            var email = $("#txtemail").val();
            var company = $("#txtcompany").val();
            var phone = $("#txtphone").val();
            var from = $("#txtfrom").val();
            var to = $("#txtto").val();
            var data = {id: id,name:name,type:role,email:email,company:company,phone:phone,fromDate:from,toDate:to};    
            
             $.ajax({
                 type: "post",
                 url: "../../modules/ajax/admin.php?type=10",
                 data: JSON.stringify(data),
                 cache: false,
                 contentType: "application/json",
                 success: function (data) {  
                     if (data['status'] == 200)
                         alert(data['message']);

                     location.reload(true);

                 },
                 error: function (jqXHR, textStatus, errorThrown) {
                     alert("Error for conformation");
                     alert(textStatus);
                     alert(errorThrown);
                 } //end of error
             }); //end of aajax
                
            }    

    });
    
  
});  
