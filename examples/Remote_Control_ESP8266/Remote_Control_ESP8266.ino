/**
 * This is an example of the Remote Control library used with an ESP8266
 * and websocket to control I/O pins, variables and function calls.
 * 
 * After loading the sketch, connect to the "myNetwork" wifi network and 
 * navigate your browser to http://192.168.4.1/. You should see a
 * control panel for connecting to a websocket server, as well as for sending 
 * and receiving websocket messages.
 * Try sending messages like: "led=0", "led=1", "floatvar" and 
 * "hello(first=SpongBob, last=Squarepants)"
 **/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <RemoteControl.h>        // https://github.com/idotuchman/RemoteControl

ESP8266WebServer server(80);      // Create a webserver object that listens for HTTP request on port 80
WebSocketsServer webSocket(81);   // Create a websocket server on port 81
RemoteControl control;            // Create a RemoteControl instance

const char *WIFI_NETWORK = "myNetwork";      // change to whatever wifi network name you want

// some variables controlled by RemoteControl
int intvar = 7;
float floatvar = 3.14;
char charvar = 'h';
String first = "";
String last = "";
bool boolvar = true;

// minified websocket control panel 
const char *HOMEPAGE_HTML PROGMEM = "<!DOCTYPE html><html lang=en><head><meta charset=UTF-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><meta http-equiv=X-UA-Compatible content=\"ie=edge\"><title>WebsSocket Console</title>\n<style>\n  /*! normalize.css v8.0.1 | MIT License | github.com/necolas/normalize.css */html{line-height:1.15;-webkit-text-size-adjust:100%}body{margin:0}main{display:block}h1{font-size:2em;margin:.67em 0}hr{box-sizing:content-box;height:0;overflow:visible}pre{font-family:monospace,monospace;font-size:1em}a{background-color:transparent}abbr[title]{border-bottom:none;text-decoration:underline;text-decoration:underline dotted}b,strong{font-weight:bolder}code,kbd,samp{font-family:monospace,monospace;font-size:1em}small{font-size:80%}sub,sup{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sub{bottom:-.25em}sup{top:-.5em}img{border-style:none}button,input,optgroup,select,textarea{font-family:inherit;font-size:100%;line-height:1.15;margin:0}button,input{overflow:visible}button,select{text-transform:none}[type=button],[type=reset],[type=submit],button{-webkit-appearance:button}[type=button]::-moz-focus-inner,[type=reset]::-moz-focus-inner,[type=submit]::-moz-focus-inner,button::-moz-focus-inner{border-style:none;padding:0}[type=button]:-moz-focusring,[type=reset]:-moz-focusring,[type=submit]:-moz-focusring,button:-moz-focusring{outline:1px dotted ButtonText}fieldset{padding:.35em .75em .625em}legend{box-sizing:border-box;color:inherit;display:table;max-width:100%;padding:0;white-space:normal}progress{vertical-align:baseline}textarea{overflow:auto}[type=checkbox],[type=radio]{box-sizing:border-box;padding:0}[type=number]::-webkit-inner-spin-button,[type=number]::-webkit-outer-spin-button{height:auto}[type=search]{-webkit-appearance:textfield;outline-offset:-2px}[type=search]::-webkit-search-decoration{-webkit-appearance:none}::-webkit-file-upload-button{-webkit-appearance:button;font:inherit}details{display:block}summary{display:list-item}[hidden],template{display:none}html{--theme-hue:0;--accent-hue:194;--text-color-richer:hsl(var(--theme-hue),0%,5%);--text-color-normal:hsl(var(--theme-hue),0%,13%);--text-color-softer:hsl(var(--theme-hue),0%,33%);--accent-color:hsl(var(--accent-hue),86%,57%);--accent-color-hover:hsl(var(--accent-hue),76%,49%);--border-color:hsl(var(--theme-hue),0%,73%);--border-color-softer:hsl(var(--theme-hue),0%,82%);--background-color:#fff;--background-color-softer:hsl(var(--theme-hue),0%,95%);--code-background:hsl(var(--theme-hue),0%,95%);--button-primary-color:#fff;--base-font-size:62.5%;--grid-max-width:960px}@media (prefers-color-scheme:dark){:html{--theme-hue:0;--accent-hue:194;--text-color-richer:hsl(var(--theme-hue),0%,95%);--text-color-normal:hsl(var(--theme-hue),0%,80%);--text-color-softer:hsl(var(--theme-hue),0%,67%);--accent-color:hsl(var(--accent-hue),76%,49%);--accent-color-hover:hsl(var(--accent-hue),86%,57%);--border-color:hsl(var(--theme-hue),0%,27%);--border-color-softer:hsl(var(--theme-hue),0%,20%);--background-color:hsl(var(--theme-hue),0%,12%);--background-color-softer:hsl(var(--theme-hue),0%,18%);--code-background:hsl(var(--theme-hue),0%,5%);--button-primary-color:#fff}img.value-img{filter:invert(.8)}}.grid-container{position:relative;max-width:var(--grid-max-width);margin:0 auto;padding:20px;text-align:center;display:grid;grid-gap:20px;gap:20px;grid-template-columns:minmax(200px,1fr)}@media (min-width:600px){.grid-container{grid-template-columns:repeat(3,1fr);padding:10px 0}.grid-container.fifths{grid-template-columns:repeat(5,1fr)}.grid-container.quarters{grid-template-columns:repeat(4,1fr)}.grid-container.thirds{grid-template-columns:repeat(3,1fr)}.grid-container.halves{grid-template-columns:repeat(2,1fr)}.grid-container.full{grid-template-columns:1fr}}html{font-size:var(--base-font-size);scroll-behavior:smooth}body{font-size:1.6rem;line-height:1.6;font-weight:400;font-family:Raleway,HelveticaNeue,Helvetica Neue,Helvetica,Arial,sans-serif;color:var(--text-color-normal);background-color:var(--background-color)}h1,h2,h3,h4,h5,h6{margin-top:0;margin-bottom:2rem;font-weight:300}h1{font-size:4rem;line-height:1.2}h1,h2{letter-spacing:-.1rem}h2{font-size:3.6rem;line-height:1.25}h3{font-size:3rem;line-height:1.3;letter-spacing:-.1rem}h4{font-size:2.4rem;line-height:1.35;letter-spacing:-.08rem}h5{font-size:1.8rem;line-height:1.5;letter-spacing:-.05rem}h6{font-size:1.5rem;line-height:1.6;letter-spacing:0}@media (min-width:600px){h1{font-size:5rem}h2{font-size:4.2rem}h3{font-size:3.6rem}h4{font-size:3rem}h5{font-size:2.4rem}h6{font-size:1.5rem}}p{margin-top:0}a{color:var(--accent-color)}a:hover{color:var(--accent-color-hover)}.button,button,input[type=button],input[type=reset],input[type=submit]{display:inline-block;height:38px;padding:0 30px;color:var(--text-color-softer);text-align:center;font-size:11px;font-weight:600;line-height:38px;letter-spacing:.1rem;text-transform:uppercase;text-decoration:none;white-space:nowrap;background-color:transparent;border-radius:4px;border:1px solid var(--border-color);cursor:pointer;box-sizing:border-box}.button:focus,.button:hover,button:focus,button:hover,input[type=button]:focus,input[type=button]:hover,input[type=reset]:focus,input[type=reset]:hover,input[type=submit]:focus,input[type=submit]:hover{color:var(--text-color-normal);border-color:var(--text-color-softer);outline:0}.button.button-primary,button.button-primary,input[type=button].button-primary,input[type=reset].button-primary,input[type=submit].button-primary{color:var(--button-primary-color);background-color:var(--accent-color);border-color:var(--accent-color)}.button.button-primary:focus,.button.button-primary:hover,button.button-primary:focus,button.button-primary:hover,input[type=button].button-primary:focus,input[type=button].button-primary:hover,input[type=reset].button-primary:focus,input[type=reset].button-primary:hover,input[type=submit].button-primary:focus,input[type=submit].button-primary:hover{color:var(--button-primary-color);background-color:var(--accent-color-hover);border-color:var(--accent-color-hover)}input[type=email],input[type=number],input[type=password],input[type=search],input[type=tel],input[type=text],input[type=url],select,textarea{height:38px;padding:6px 10px;background-color:var(--background-color);border:1px solid var(--border-color-softer);border-radius:4px;box-shadow:none;box-sizing:border-box}input[type=button],input[type=email],input[type=number],input[type=password],input[type=search],input[type=submit],input[type=tel],input[type=text],input[type=url],textarea{-webkit-appearance:none;-moz-appearance:none;appearance:none}textarea{min-height:65px;padding-top:6px;padding-bottom:6px}input[type=email]:focus,input[type=number]:focus,input[type=password]:focus,input[type=search]:focus,input[type=tel]:focus,input[type=text]:focus,input[type=url]:focus,select:focus,textarea:focus{border:1px solid var(--accent-color);outline:0}label,legend{display:block;margin-bottom:.5rem;font-weight:600}fieldset{padding:0;border-width:0}input[type=checkbox],input[type=radio]{display:inline}label>.label-body{display:inline-block;margin-left:.5rem;font-weight:400}ul{list-style:circle inside}ol{list-style:decimal inside}ol,ul{padding-left:0;margin-top:0}ol ol,ol ul,ul ol,ul ul{font-size:100%;margin:1rem 0 1rem 3rem;color:var(--text-color-softer)}li{margin-bottom:.5rem}code{padding:.2rem .5rem;margin:0 .2rem;font-size:90%;white-space:nowrap;background:var(--code-background);border:1px solid var(--border-color-softer);border-radius:4px}pre>code{display:block;padding:1rem 1.5rem;white-space:pre;overflow:auto}td,th{padding:12px 15px;text-align:left;border-bottom:1px solid var(--border-color-softer)}td:first-child,th:first-child{padding-left:0}td:last-child,th:last-child{padding-right:0}.button,button{margin-bottom:1rem}fieldset,input,select,textarea{margin-bottom:1.5rem}blockquote,dl,figure,form,ol,p,pre,table,ul{margin-bottom:2.5rem}.u-full-width{width:100%;box-sizing:border-box}.u-max-full-width{max-width:100%;box-sizing:border-box}.u-pull-right{float:right}.u-pull-left{float:left}.u-align-left{text-align:left}.u-align-right{text-align:right}hr{margin-top:3rem;margin-bottom:3.5rem;border-width:0;border-top:1px solid var(--border-color-softer)}.container:after,.row:after,.u-cf{content:\"\";display:table;clear:both}body,html{height:100%}.container{display:grid;max-width:var(--grid-max-width);grid-template-columns:1fr 1fr 1fr;grid-template-rows:auto 80vh;grid-template-areas:\"connection connection connection\" \"receive receive send\";grid-gap:20px 20px;padding:10px;margin:0 auto}.receive,.send{display:flex;flex-direction:column}.label{margin:0;font-size:.7em}.box{height:100%;border:1px solid var(--border-color-softer);border-radius:4px;box-shadow:none;box-sizing:border-box;overflow-y:auto}.connection{grid-area:connection}.connection-input{display:flex}#ipAddress{font-size:.9em;flex:auto;margin:0 5px 0 0}#connectButton,#disconnectButton{flex:initial;margin:0 5px 0 0}#clearButton{flex:initial;margin:0}.status{margin-bottom:20px}#connectionStatus{font-weight:700}.send{grid-area:send}.send-input{display:flex}#sendButton{flex:initial;margin:0}#newMessage{flex:auto;font-size:.9em;margin:0 5px 20px 0}#sentMessages{flex:auto;resize:none}#receivedMessages,#sentMessages{font-size:.8em}td,tr{padding:0;margin:0;border:0}tr:nth-child(2n){background-color:#f2f2f2}tr:hover{background-color:#ddd}table{background-color:var(--background-color);width:100%}.receive{grid-area:receive}input.button-disabled{border:1px solid #999;background-color:#ccc;color:#666;cursor:not-allowed}.hidden{visibility:hidden}@media (max-width:600px){.container{grid-template-rows:auto 50vh 30vh;grid-template-areas:\"connection connection connection\" \"receive receive receive\" \"send send send\"}.connection-input{flex-wrap:wrap}#ipAddress{margin:0 5px 5px 0}}\n</style>\n</head><body><div class=container><div class=connection><strong>WebSocket </strong><span id=connectionStatus></span><div class=connection-input><input type=text id=ipAddress> <input onclick=connect() class=button type=submit value=Connect id=connectButton> <input onclick=disconnect() class=button type=submit value=Disconnect id=disconnectButton> <input onclick=clearTables() class=button type=submit value=Clear id=clearButton></div></div><div class=receive><div class=send-input><input type=text placeholder=\"Enter outgoing message\" id=newMessage> <input onclick=sendMessage() class=button-primary type=submit value=Send id=sendButton></div><p class=label>RECEIVED MESSAGES</p><div class=box><table id=receivedMessages></table></div></div><div class=send><p class=label>SENT MESSAGES</p><div class=box><table id=sentMessages></table></div></div></div>\n<script>\n!function(e){var t={};function n(o){if(t[o])return t[o].exports;var s=t[o]={i:o,l:!1,exports:{}};return e[o].call(s.exports,s,s.exports,n),s.l=!0,s.exports}n.m=e,n.c=t,n.d=function(e,t,o){n.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:o})},n.r=function(e){\"undefined\"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:\"Module\"}),Object.defineProperty(e,\"__esModule\",{value:!0})},n.t=function(e,t){if(1&t&&(e=n(e)),8&t)return e;if(4&t&&\"object\"==typeof e&&e&&e.__esModule)return e;var o=Object.create(null);if(n.r(o),Object.defineProperty(o,\"default\",{enumerable:!0,value:e}),2&t&&\"string\"!=typeof e)for(var s in e)n.d(o,s,function(t){return e[t]}.bind(null,s));return o},n.n=function(e){var t=e&&e.__esModule?function(){return e.default}:function(){return e};return n.d(t,\"a\",t),t},n.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},n.p=\"\",n(n.s=3)}([function(e,t,n){},function(e,t,n){},function(e,t,n){},function(e,t,n){\"use strict\";n.r(t);n.d(t,\"receiveTable\",(function(){return a})),n(0),n(1),n(2);const o=document.getElementById(\"ipAddress\"),s=document.getElementById(\"receivedMessages\"),c=document.getElementById(\"sentMessages\"),r=document.getElementById(\"newMessage\"),i=document.getElementById(\"connectionStatus\");o.value=\"ws://192.168.4.1:81/\";const l=new class{constructor(){this.ws=new Object,this.ws.readyState=3}newConnection(e,t){this.ws=new WebSocket(e,t),this.ws.onmessage=function(e){a.addRow(e.data),console.log(\"Server: \",e.data)},this.ws.onclose=function(e){},this.ws.onerror=function(e){console.log(\"WebSocket error: \"+e.data)},this.ws.onopen=function(e){}}send(e){return 1===this.ws.readyState&&(this.ws.send(e),!0)}disconnect(){3!==this.ws.readyState&&this.ws.close()}getStatus(){return this.ws.readyState}},a=new class{constructor(e){this.table=e}addRow(e){let t=this.table.insertRow(0).insertCell(0);t.innerHTML=e,t.setAttribute(\"onclick\",\"copyToClipboard(\"+this.table.rows.length+\")\")}copyToClipboard(e){let t=document.getElementsByTagName(\"body\")[0],n=document.createElement(\"input\",{type:\"text\"});t.appendChild(n),n.value=this.table.rows[this.table.rows.length-e].cells[0].innerHTML,n.select(),n.setSelectionRange(0,99999),document.execCommand(\"copy\"),t.removeChild(n)}clear(){this.table.innerHTML=\"\"}}(s),d=new class{constructor(e){this.table=e,window.copyToSend=this.copyToSend}addRow(e){let t=this.table.insertRow(0).insertCell(0);t.innerHTML=e,t.setAttribute(\"onclick\",\"copyToSend(\"+this.table.rows.length+\")\")}getText(e){return this.table.rows[this.table.rows.length-e].cells[0].innerHTML}clear(){this.table.innerHTML=\"\"}}(c);r.addEventListener(\"keyup\",(function(e){\"Enter\"===e.key&&sendMessage()})),window.connect=function(){l.newConnection(o.value,[\"arduino\"])},window.sendMessage=function(){let e=r.value;l.send(e)&&d.addRow(e)},window.copyToClipboard=function(e){a.copyToClipboard(e)},window.copyToSend=function(e){r.value=d.getText(e)},window.disconnect=function(){l.disconnect()},window.clearTables=function(){a.clear(),d.clear()},setInterval(()=>{const e={0:\"Trying to Connect\",1:\"Connected\",2:\"Disconnecting\",3:\"Disconnected\"};let t=l.getStatus();i.innerHTML=e[t],i.setAttribute(\"style\",{0:\"color: #3477DB\",1:\"color: #00AA55\",2:\"color: #3477DB\",3:\"color: #E00000\"}[t]),\"Connected\"===e[t]?(connectButton.disabled=\"disabled\",connectButton.className=\"button-disabled\",sendButton.className=\"button-primary\"):sendButton.className=\"button-disabled\",\"Disconnected\"===e[t]&&(connectButton.className=\"button\",connectButton.disabled=\"\")},1e3)}]);\n</script>\n</body></html>";

// serves websocket control panel
void handleRoot() {
  server.send(200, "text/html", HOMEPAGE_HTML);
}

// starts a web server
void startWebServer() {
  // send index.html  
  server.on("/", handleRoot);
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

// handles websocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) { // When a WebSocket message is received
  String response;

  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);      
      response = control.handle((char *) payload);        // send websocket messages to contoller
      if (response != "") {
        Serial.printf("Response: %s\n", response.c_str());
        webSocket.sendTXT(num, response.c_str());
      }
      break;
  }
}

// start a websocket server
void startWebSocket() {
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incoming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

// starts an wifi access point
void setupWifi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_NETWORK);
  Serial.printf("\n\nStarting Access Point\n");
  WiFi.softAP(WIFI_NETWORK);
  Serial.printf("SSID: %s\n", WIFI_NETWORK);
  Serial.printf("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
}

/**
 * Functions controlled by RemoteControl must return 
 * a String and receive only a character string argument.
 * 
 * Function arguments can be extracted by passing the 
 * string argument to the handleArgs() method. Multiple 
 * arguments are separated by commas (,). Make sure the
 * function arguments are registered.
 * 
 * In this example, the hello() function take two 
 * arguments: first and last. Thus, sending 
 * "hello(first=SpongeBob, last=SquarePants)" 
 * calls the hello function and sets the first and 
 * last variables.
 **/
String hello(char *args) {
  // set default values
  first = "Foo";
  last = "Bar";

  // read function argument(s)
  control.handleArgs(args);
  return "Hello " + first + " " + last + "!\n";
}

void setup() {
  Serial.begin(115200);         // Start serial communication to send/receive messages
  setupWifi();
  startWebServer();
  startWebSocket();

  // register I/O pins, variables and functions for RemoteControl to control
  control.pin("led", LED_BUILTIN, OUTPUT);
  control.variable("intvar", &intvar);
  control.variable("floatvar", &floatvar);
  control.variable("charvar", &charvar);
  control.variable("first", &first);
  control.variable("last", &last);
  control.variable("boolvar", &boolvar);
  control.function("hello", &hello);
  Serial.println("Remote Control started. Ready to receive commands.");
}

void loop() {
  server.handleClient();
  webSocket.loop();
  delay(0);                                   // yield for built-in wifi functions
}