require('../css/normalize.css');
require('../css/barebones.css');
require('../css/bert-calibrate.css');

import {WsConnection} from "./WsConnection";
import {ReceiveTable} from "./ReceiveTable";
import {SendTable} from "./SendTable";

const ipAddress = document.getElementById('ipAddress');
const receivedMessages = document.getElementById('receivedMessages');
const sentMessages = document.getElementById('sentMessages');
const newMessage = document.getElementById('newMessage');
const statusMessage = document.getElementById('connectionStatus');

ipAddress.value = "ws://192.168.4.1:81/"; // set default IP address 192.168.4.1
const connection = new WsConnection();
export const receiveTable = new ReceiveTable(receivedMessages);
const sendTable = new SendTable(sentMessages);

newMessage.addEventListener('keyup', function(event) {
  if (event.key === 'Enter'){
    sendMessage();
  }
});

window.connect = function() {
  // websocket api: https://developer.mozilla.org/en-US/docs/Web/API/WebSocket  
  connection.newConnection(ipAddress.value, ['arduino']);
}

window.sendMessage = function() {
  let message = newMessage.value;
  if(connection.send(message)) {
    sendTable.addRow(message);
  }
}

window.copyToClipboard = function(rowNumber) {
  receiveTable.copyToClipboard(rowNumber);
}

window.copyToSend = function(rowNumber){
  // copy clicked text to textarea
  newMessage.value = sendTable.getText(rowNumber);
}

window.disconnect = function() {
  connection.disconnect();
}

window.clearTables = function() {
  receiveTable.clear();
  sendTable.clear();
}

setInterval(()=>{
  const statusLookup = {0:"Trying to Connect", 1:"Connected", 2:"Disconnecting", 3:"Disconnected"};
  const colorLookup = {0:"color: #3477DB", 1:"color: #00AA55", 2:"color: #3477DB", 3:"color: #E00000"};
  
  let status = connection.getStatus();
  statusMessage.innerHTML = statusLookup[status];
  statusMessage.setAttribute("style", colorLookup[status]);
  
  if (statusLookup[status] === "Connected") {
    connectButton.disabled ='disabled';
    connectButton.className = 'button-disabled';
    sendButton.className = 'button-primary';
  } else {
    sendButton.className = 'button-disabled';
  }

  if (statusLookup[status] === "Disconnected") {
    connectButton.className = 'button';
    connectButton.disabled ='';
  }
}, 1000);