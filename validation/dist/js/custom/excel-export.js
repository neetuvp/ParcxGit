function export_to_excel(container, filename) {

  // get current date
  var d = new Date();
  var month = d.getMonth() + 1;
  var day = d.getDate();
  var todays_date = d.getFullYear() + '_' +
    (month < 10 ? '0' : '') + month + '_' +
    (day < 10 ? '0' : '') + day;

  // export to Excel
  let file = new Blob([$(container).html()], {
    type: "application/vnd.ms-excel"
  });

  let url = URL.createObjectURL(file);

  let a = $("<a />", {
    href: url,
    download: filename + " " + todays_date + ".xls"
  })
    .appendTo("body")
    .get(0)
    .click();

  // e.preventDefault();

}