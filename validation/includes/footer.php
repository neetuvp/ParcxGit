</div>

    <!-- mobile alert -->
    <div class="content-wrapper mobile-container">
        <div class="card card-outline card-danger">
            <h5>Alert</h5>
            <p>To view this page, please use a tablet or desktop device.</p>
        </div>
    </div>

    <!-- scripts -->
    <script src="dist/js/parcx.js"></script>

    <!-- datepicker -->
    <link href="dist/js/daterangepicker/daterangepicker.css" rel="stylesheet">
    <script src="dist/js/daterangepicker/moment.js"></script>
    <script src="dist/js/daterangepicker/daterangepicker.js"></script>

    <!-- bootstrap multiselect -->
    <script src="dist/js/bootstrap-bundle.js"></script>
    <script src="dist/js/bootstrap-multiselect.js"></script>

    <!-- export --> 
    <script src="dist/js/custom/excel-export.js"></script>
    <script src="dist/js/custom/pdf-export.js"></script>
    <script src="dist/js/jspdf/html2canvas.js"></script>
    <script src="dist/js/jspdf/jspdf.1.5.3.js"></script>
    <script src="dist/js/jspdf/jspdf.autotable.js"></script>

    <script>

        /* === check navbar padding === */

        function checkPD() {
            var nav = $(".navbar");

            if (window.innerWidth > 600) {
                if (($(".tab-link").length) > 0) {
                    nav.addClass("has-tablink");
                }
            } else {
                nav.removeClass("has-tablink");
            }
        }

        checkPD();

        $(document).ready(function() {
            checkPD();
        });

        $(window).resize(function(){
            checkPD();
        });

        /* === show / hide password === */

        $(document).ready(function() {

            $(".pw-view-btn button").on('click', function(event) {  
                event.preventDefault();  
                var currentElement = $(this).parents(".pw-view-btn");
                var id = $("#" + currentElement.attr('id'))
                var inputElement = id.find('input');
                var inputIcon = id.find('i');

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

        /* === add export btn for reporting pages === */
        
        var exportBtn = 
        '<li class="nav-item mr-3 hidden" id="export-container">' +
            '<div id="action-buttons">' +
                '<div class="btn-group">' +
                    '<button type="button" class="btn btn-warning">Export</button>' +
                    '<button type="button" class="btn btn-warning dropdown-toggle" data-toggle="dropdown">' +
                        '<span class="caret"></span>' +
                        '<span class="sr-only">Toggle Dropdown</span>' +
                    '</button>' +
                    '<div class="dropdown-menu" role="menu">' +
                        '<a class="dropdown-item" href="#" id="export_excel_report">Export to Excel</a>' +
                        '<a class="dropdown-item" href="#" id="export_pdf_report">Export to PDF</a>' +
                    '</div>' +
                '</div>' +
            '</div>' +
        '</li>';

        $(document).ready(function() {
            var current_path = window.location.pathname.split('/').pop();
            var reportingPages = ["report-summary.php", "report-transactions.php"];
            for (var i in reportingPages) {
                if (reportingPages[i] === current_path) {
                    $('ul.navbar-nav.ml-auto').prepend(exportBtn);
                }
            }
        });

        $(document).on("click", "#view-report-button", function() {
            $("#export-container").removeClass("hidden");
        });

        /* === excel export === */

        $(document).on("click", "#export_excel_report", function() {
            var page = $("#pdf-report-header").html();
            page = page.replace(/\s+/g,"_");
            export_to_excel("#report-table", page);
        });

    </script>

</body>
</html>