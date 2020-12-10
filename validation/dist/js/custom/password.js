/* === password strength === */

$(document).ready(function () {
    "use strict";

    // add validator form
    var options = {};
    options.ui = {
        container: "#pwd-container",
        viewports: {
            progress: ".pwstrength_viewport_progress"
        },
        showPopover: true,
        showErrors: true
    };
    options.common = {
        debug: false, // check password strength in console
        onKeyUp: function (evt, data) {
            // hide errors label if there are no errors
            if ($(".error-list li").length > 0) {
                $(".popover-body div").show();
            } else {
                $(".popover-body div").hide();
            }
            // enable/disable submit button based on pw strength
            if ($(".pwstrength_viewport_progress .progress-bar").hasClass("bg-success")) {
                $("#addValidator").prop('disabled', false);
            } else {
                $("#addValidator").prop('disabled', true);
            }
        },
    };
    $('#password').pwstrength(options);

    // edit validator form
    var options_edit = {};
    options_edit.ui = {
        container: "#pwd-container-edit",
        viewports: {
            progress: ".pwstrength_viewport_progress_edit"
        },
        showPopover: true,
        showErrors: true
    };
    options_edit.common = {
        debug: false, // check password strength in console
        onKeyUp: function (evt, data) {
            // hide errors label if there are no errors
            if ($(".error-list li").length > 0) {
                $(".popover-body div").show();
            } else {
                $(".popover-body div").hide();
            }
        },
    };
    $('#password-edit').pwstrength(options_edit);

});