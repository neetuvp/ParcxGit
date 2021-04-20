function export_to_excel(container, filename) {

  // get current date

  var d = new Date();
  var month = d.getMonth() + 1;
  var day = d.getDate();
  var todays_date = d.getFullYear() + '_' +
    (month < 10 ? '0' : '') + month + '_' +
    (day < 10 ? '0' : '') + day; 

  // export to Excel
  var RecordsTable_length;
  var excel_data=$(container).html();
  
  if($(container+' .dataTable').length==1)
    {
    RecordsTable_length=$('select[name="RecordsTable_length"]').val();       
    $('select[name="RecordsTable_length"]')
    .val('-1')
    .trigger('change'); 
    if($(container + " .jspdf-graph").length==1)
        excel_data=$(container + " .jspdf-graph").html()+$("#RecordsTable_wrapper").children().eq(1).html();  
    else
        excel_data=$("#RecordsTable_wrapper").children().eq(1).html();  
    }   
    
 
  
    
  let file = new Blob([excel_data], {
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
    
    if($(container+' .dataTable').length==1)
        {        
        $('select[name="RecordsTable_length"]')
        .val(RecordsTable_length)
        .trigger('change');   
        }
 
}