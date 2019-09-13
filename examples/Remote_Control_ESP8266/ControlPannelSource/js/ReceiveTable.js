export class ReceiveTable {
  constructor(table) {
    this.table = table;
  }

  addRow(message) {    
    let row = this.table.insertRow(0);
    let cell = row.insertCell(0);
    cell.innerHTML = message;
    cell.setAttribute("onclick", "copyToClipboard(" + (this.table.rows.length) + ")");
  }

  copyToClipboard(rowNumber) {
    // create temporary textarea in body
    let body = document.getElementsByTagName('body')[0];
    let clipBoard = document.createElement('input', {'type': 'text'});
    body.appendChild(clipBoard);
  
    // copy clicked text to textarea
    clipBoard.value = this.table.rows[this.table.rows.length-rowNumber].cells[0].innerHTML;
  
    // select text in textarea
    clipBoard.select(); 
    clipBoard.setSelectionRange(0, 99999); /*For mobile devices*/
  
    // copy text inside textarea to clipboard
    document.execCommand("copy");
  
    // remove temporary text area
    body.removeChild(clipBoard);
  }

  clear() {
    this.table.innerHTML = '';
  }
}