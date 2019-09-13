import {receiveTable} from "./App.js";

export class WsConnection {
  constructor() {
    this.ws = new Object();
    this.ws.readyState = 3;         // default status is Not Connected
  }

  newConnection(ipAddress, type) {
    this.ws = new WebSocket(ipAddress, type);
    
    this.ws.onmessage = function (e) {
      receiveTable.addRow(e.data);
      console.log('Server: ', e.data);
    }
    this.ws.onclose = function (e) {
      // console.log("WebSocket is closed.");  
    }
    this.ws.onerror = function (e) {
      console.log("WebSocket error: " + e.data);
    }
    this.ws.onopen = function (e) {
      // console.log("WebSocket connected.");
    }
  }

  send(message) {
    if (this.ws.readyState === 1) {
      this.ws.send(message);
      return true;
    }
    return false;
  }

  disconnect() {
    // only disconnect if WS is not disconnected
    if (this.ws.readyState !== 3) {
      this.ws.close();
    }    
  }

  getStatus() {
    return this.ws.readyState;
  }
}