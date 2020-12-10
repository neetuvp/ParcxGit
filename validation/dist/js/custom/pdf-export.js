/* === PDF Export === */

// use class="jspdf-table" for tables
// use class="jspdf-graph" for graphs/visuals

/* === get logo img for header === */

function imgToBase64(url, callback) {
    if (!window.FileReader) {
        callback(null);
        return;
    }
    var xhr = new XMLHttpRequest();
    xhr.responseType = 'blob';
    xhr.onload = function () {
        var reader = new FileReader();
        reader.onloadend = function () {
            callback(reader.result.replace('text/xml', 'image/jpeg'));
        };
        reader.readAsDataURL(xhr.response);
    };
    xhr.open('GET', url);
    xhr.send();
}

function getImage() {
    imgToBase64("dist/img/parcx-pdf-logo.png", function (base64) {
        base64Img = base64;
    });
}

$(document).ready(function () {
    var base64Img;
    getImage();
});

/* === table export function === */

function jspdfTable(elementHeight, doc, pdf_margin, page_title) {

    var elem = $(".jspdf-table");
    var tableY = (elementHeight - 6)

    elem.each(function () {

        // manage margins for multiple tables
        if ($(".jspdf-table").length > 1) {

            if (this === elem[0]) {
                tableY = (elementHeight - 6)
            } else {
                tableY = doc.autoTableEndPosY() + 10;
            }

            // table header
            table_heading = $(this).prev('.header').text();
            if (table_heading !== undefined) {
                doc.text(table_heading, pdf_margin, (tableY));
            }

        }

        // get table data
        var res = doc.autoTableHtmlToJson(this);
        doc.internal.scaleFactor = 2.834645669291339; // default

        // create table
        doc.autoTable(res.columns, res.data, {
            styles: {
                cellPadding: [3, 2, 3, 1.4], // top, right, bottom, left
                overflow: 'linebreak',
                columnWidth: 'auto', // 'wrap' could lead to some columns getting cut off
                fontSize: 10,
                valign: 'middle',
            },
            startY: tableY,
            margin: { bottom: 15, top: 30 },
        });

    })

    // save file
    jspdfSave(doc, pdf_margin, page_title);

}

/* === graph export function === */

function jspdfGraph() {

    var graphs = $(".jspdf-graph");
    var remaining = graphs.length;
    var current = 0;
    var graphHeight = [];
    var offsetValue = 0;
    var pageCount = 1;
    var previousGraphIndex;

    graphs.each(function () {
        html2canvas($(this), {

            profile: true,
            allowTaint: true,
            letterRendering: true,
            useCORS: true,

            onrendered: function (canvas) {

                // get graph and convert it to an img
                var imageData = canvas.toDataURL("image/png", wid = canvas.width, hgt = canvas.height);
                var hratio = hgt / wid
                var width = doc.internal.pageSize.width;
                var height = width * hratio;
                graphHeight[current] = height;
                pageHeight = doc.internal.pageSize.height;
                pageHeight -= (pdf_margin * 2)
                pageHeight = Math.round(pageHeight)

                getGraphPosition();
                addGraphToPage();

            }

        }); // close html2canvas
    }); // close graphs.each

}

function getGraphPosition() {

    // for the 1st graph, add top margin for report header
    if (remaining === graphs.length) {
        var img_top_margin = pageHeaderHeight + reportHeaderHeight - 6 + offsetValue;
    }

    // for all graphs apart from the 1st, if graph exceeds remaining space on pg, create new pg
    else {

        previousGraphIndex = current - 1
        offsetValue = graphHeight[previousGraphIndex] + pageHeaderHeight + 6 + reportHeaderHeight + graphHeight[current];
        offsetValue = Math.round(offsetValue);

        if (offsetValue > pageHeight) {

            doc.addPage();
            doc.line(14, pageHeaderHeight, (doc.internal.pageSize.width - 14), pageHeaderHeight);
            offsetValue = 0; // restart height position
            offsetValue += pageHeaderHeight
            offsetValue += 8 // buffer
            pageCount += 1;
            console.log("graph exceeds pageheight, move to new page")

        } else {

            // reset offsetValue for positioning
            // different offsetValue for first page versus other pages due to report header

            if (pageCount === 1) {
                offsetValue = pageHeaderHeight + reportHeaderHeight + graphHeight[previousGraphIndex] - 2;
            } else {
                offsetValue = pageHeaderHeight + graphHeight[previousGraphIndex] + 12;
            }
            console.log("graph fits on current pg")

        }

        // add top margin for page header
        var img_top_margin = offsetValue;

    }

}

function addGraphToPage() {

    doc.addImage(
        imageData, // img
        'JPEG', // img format
        pdf_margin, // left margin
        img_top_margin, // top margin
        width - (pdf_margin * 2), // img width
        height // img height
    );

    // increase graph counter
    current += 1
    remaining--;

    // if no graphs remain, add table data & save
    if (remaining === 0) {

        // if page only has graph content
        if ($(".jspdf-table").length == 0) {
            jspdfSave(doc, pdf_margin, page_title);
        }

        // if page has table and graph content
        else {

            elementHeight = height; // height of image
            elementHeight += 8 // buffer for graphic bottom margin
            elementHeight += reportHeaderHeight // add height of header
            elementHeight += pageHeaderHeight

            if (previousGraphIndex !== undefined) {
                elementHeight += graphHeight[previousGraphIndex]
            }

            jspdfTable(elementHeight, doc, pdf_margin, page_title);

        }

    }

}

/* === pdf header === */

function addHeader(doc, pdf_margin) {

    var headerBuffer = 0.5
    pdf_margin -= headerBuffer;

    // add logo image
    if (base64Img) {
        doc.addImage(base64Img, 'JPEG', (pdf_margin + 1), pdf_margin, 0, 0);
    }

    // reset margin
    pdf_margin += headerBuffer;

}

/* === pdf footer === */

function addFooter(doc, pageCount, currentPage, pdf_margin) {
    var footerString = "Page " + currentPage + " of " + pageCount;
    doc.text(footerString, pdf_margin, doc.internal.pageSize.height - 10);
}

/* === pdf save === */

function jspdfSave(doc, pdf_margin, page_title) {

    var pageCount = doc.internal.getNumberOfPages();
    var currentPage;

    for (currentPage = 1; currentPage <= pageCount; currentPage++) {
        doc.setPage(currentPage);
        addHeader(doc, pdf_margin);
        addFooter(doc, pageCount, currentPage, pdf_margin);
    }

    // get current date for filename
    var d = new Date();
    var month = d.getMonth() + 1;
    var day = d.getDate();
    var current_date = d.getFullYear() + '_' + (month < 10 ? '0' : '') + month + '_' + (day < 10 ? '0' : '') +
        day;

    // create filename
    var filename = page_title + " " + current_date;

    // save page
    doc.save(filename + ".pdf");

}

/* === generate pdf based on page content (table / graph) === */

function generatePDF(reportHeaderHeight, pageHeaderHeight, doc, pdf_margin, page_title) {

    var table_heading
    var elementHeight

    // case 1 - if page only has table content
    if ($(".jspdf-graph").length == 0) {
        elementHeight = reportHeaderHeight + pageHeaderHeight
        jspdfTable(elementHeight, doc, pdf_margin, page_title);
    }
    // case 2 - page has table & graph // or page only has graphs
    else {
        jspdfGraph();
    }

}

/* === pdf click function === */

$(document).on("click", "#export_pdf_report", function () {

    // initial document set-up
    var doc = new jsPDF('l');
    var pdf_margin = 14;
    var pageHeaderHeight = 32;
    var reportHeaderHeight = 38; // pt

    // page title
    if ($("#pdf-report-header").length != 0) {
        var page_title = $('#pdf-report-header').html();
    } else {
        var page_title = "null";
    }

    // add 'report' to page title
    if (page_title.indexOf("Report") <= 0) {
        page_title += " Report";
    }

    // export date and time
    var export_date = new Date($.now());
    export_date = "Export Date: " + export_date.toString()

    // report duration
    if ($("#reservationtime").length == 0) {
        var report_duration = "Live Report"
    } else {
        // from to vars used from init-daterangpicker.js
        var report_duration = "Report Duration: " + from + " to " + to
    }

    // add page title, export date and report duration to pdf
    doc.setFontSize(13);
    doc.text(pdf_margin, (10 + pageHeaderHeight), page_title);
    doc.setFontSize(11);
    doc.text(pdf_margin, (18 + pageHeaderHeight), report_duration);
    doc.text(pdf_margin, (24 + pageHeaderHeight), export_date);

    // draw line
    doc.line(14, pageHeaderHeight, (doc.internal.pageSize.width - 14), pageHeaderHeight);

    // generate pdf
    generatePDF(reportHeaderHeight, pageHeaderHeight, doc, pdf_margin, page_title);

});