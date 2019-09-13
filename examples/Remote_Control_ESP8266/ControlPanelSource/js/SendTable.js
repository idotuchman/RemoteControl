export class SendTable {
  constructor(table) {
    this.table = table;
    window.copyToSend = this.copyToSend;
  }

  addRow(message) {
      let row = this.table.insertRow(0);
      let cell = row.insertCell(0);
      cell.innerHTML = message;
      cell.setAttribute("onclick", "copyToSend(" + (this.table.rows.length) + ")");
  }

  getText(rowNumber) {
    return this.table.rows[this.table.rows.length-rowNumber].cells[0].innerHTML;
  }

  clear() {
    this.table.innerHTML = '';
  }
}