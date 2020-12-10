<?php
//logout
session_start();
if(!isset($_SESSION["operator_id"]) )  
    session_destroy();
?>
<html>

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>Parcx Validation| Login</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="dist/img/favicon-32x32.png" type="image/x-icon">

    <link href="dist/css/parcx.min.css" rel="stylesheet">
    <link href="dist/css/login.css" rel="stylesheet">
    <link href="dist/fonts/fontawesome/css/all.min.css" rel="stylesheet">
    <link href="dist/fonts/googlefonts/noto-sans.css" rel="stylesheet">
</head>

<body class="admin-login user-login">
    <div class="login-container">
        <div class="row no-gutters">
            <div class="col-md-7 logo-box">
                <div class="img-box">
                    <img src="dist/img/parcx-cloud-logo.jpg" class="img-fluid">
                </div>
            </div>
            <div class="col-md-5 login-form">
                <div class="login-logo">
                    <img src="dist/img/parcx-cloud-transparent.png" class="img-fluid">
                    <span>Welcome</span>
                </div>
                <form class="login-card-body" id="login-form">
                    <p class="login-box-msg">Sign in to start your session</p>
                    <div class="input-group mb-3">
                        <div class="input-group-prepend">
                            <div class="input-group-text"><span class="fa fa-user"></span></div>
                        </div>
                        <input type="text" id="username" class="form-control" placeholder="Email id" name="Username">
                    </div>
                    <div class="input-group pw-view-btn mb-3" id="pvb-1">
                        <div class="input-group-prepend">
                            <div class="input-group-text"><span class="fa fa-eye-slash"></span></div>
                        </div>
                        <input type="password" class="form-control" id="password" aria-describedby="" placeholder="Password" name="Password">
                    </div>
                    <div class="row">
                        <div class="col-8">
                            <div id="messagebox"></div>
                        </div>
                        <div class="col-4">
                            <button type="submit" class="btn btn-dark btn-block">Login</button>
                        </div>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <!-- jquery -->
    <script src="dist/js/jquery.min.js"></script>

    <!-- jquery validate -->
    <script src="dist/js/validate/jquery.validate.js"></script>

    <script>

    $(document).ready(function() {

        /* === form validation === */

        var formElement = $("#login-form") // id of the form, must be a <form> element
        // inputs should have name attr as below (username, etc)
        // submit button must have type="submit"

        formElement.validate({
            rules: {
                Username: {
                    required: true,
                    minlength: 3
                },
                Password: {
                    required: true,
                    minlength: 8
                }              
            },
            messages: {
                Username: {
                    required: "Please enter your username",
                    minlength: "Username must have at least 5 characters"
                },
                Password: {
                    required: "Please enter your password",
                    minlength: "Password must have at least 8 characters"
                }
            },
            errorElement: "div",
            errorPlacement: function (error, element) {
                error.addClass("text-danger");
                error.insertAfter(element);
            },
            submitHandler: function(){                             
                var data={};
                data["username"]=$("#username").val();
                data["password"]=$("#password").val();				
                data["option-type"]=1;
                var jsondata = JSON.stringify(data);                 
                $.post("ajax/validation.php",jsondata,function(data){
					console.log(data);
                    var json=JSON.parse(data);
                    if(json.status=="400")                        
                        {                             
                        $("#messagebox").html(json.message);                           
                        }
                    else
                        {                       
                        window.location='validate-ticket.php';
                        }            
                    })
                .fail(function(jqxhr,status,error){
                    alert("Error: "+error);
                }); 
            }
        });

        /* === show / hide password === */

        $(".pw-view-btn .input-group-prepend").on('click', function(event) {  
            event.preventDefault();  
            var currentElement = $(this).parents(".pw-view-btn");
            var id = $("#" + currentElement.attr('id'))
            var inputElement = id.find('input');
            var inputIcon = id.find('span');

            if (inputElement.attr("type") == "text") {
                inputElement.attr('type', 'password');
                inputIcon.addClass( "fa-eye-slash" );
                inputIcon.removeClass( "fa-eye" );
            } else if(inputElement.attr("type") == "password") {
                inputElement.attr('type', 'text');
                inputIcon.removeClass( "fa-eye-slash" );
                inputIcon.addClass( "fa-eye" );
            }
        });

    });

    </script>

</body>

</html>