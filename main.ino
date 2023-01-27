//Libraries
#include <WiFiNINA.h>
//Pins definitions
#define force0SensorPin A0 // the FSR and 10K pulldown are connected to A0
#define force1SensorPin A1 // the FSR and 10K pulldown are connected to A1
#define force2SensorPin A2 // the FSR and 10K pulldown are connected to A2
#define force3SensorPin A3 // the FSR and 10K pulldown are connected to A3
#define force4SensorPin A4 // the FSR and 10K pulldown are connected to A2

//Declaring variables
char ssid[] = "";             //  your network SSID (name) between the " "
char pass[] = "";      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket

WiFiClient client = server.available();


void setup() {
  Serial.begin(9600);
  while (!Serial);

  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();
}

void loop() {
  int analogReading_FSR0 = analogRead(force0SensorPin);
  int analogReading_FSR1 = analogRead(force1SensorPin);
  int analogReading_FSR2 = analogRead(force2SensorPin);
  int analogReading_FSR3 = analogRead(force3SensorPin);
  int analogReading_FSR4 = analogRead(force4SensorPin);
  client = server.available();

  if (client) {
    printWEB();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}

void printWEB() {

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // this part is the content of web site
            client.println("<!DOCTYPE HTML><html>");
            client.println("<head>");
            client.println("  <meta name='viewport' content='width = device - width, initial - scale = 1'>");
            client.println("  <script src='https://code.highcharts.com/highcharts.js'></script>");
            client.println("  <style>");
            client.println("    body {");
            client.println("      min-width: 310px;");
            client.println("      max-width: 800px;");
            client.println("      height: 400px;");
            client.println("      margin: 0 auto;");
            client.println("    }");
            client.println("    h2 {");
            client.println("      font-family: Arial;");
            client.println("      font-size: 2.5rem;");
            client.println("      text-align: center;");
            client.println("    }");
            client.println("  </style>");
            client.println("</head>");
            client.println("<body>");
            client.println("  <h2>Finger Pressure Reading</h2>");
            client.println("  <div id='FSR0' class='container'></div>");
            client.println("  <div id='FSR1' class='container'></div>");
            client.println("  <div id='FSR2' class='container'></div>");
            client.println("  <div id='FSR3' class='container'></div>");
            client.println("  <div id='FSR4' class='container'></div>");
            client.println("</body>");
            client.println("<script>");
            client.println("var chartSensor0 = new Highcharts.Chart({");
            client.println("  chart:{ renderTo : 'FSR0' },");
            client.println("  title: { text: 'Force Sensitive Resistor 0' },");
            client.println("  series: [{");
            client.println("    showInLegend: false,");
            client.println("    data: []");
            client.println("  }],");
            client.println("  plotOptions: {");
            client.println("    line: { animation: false,");
            client.println("      dataLabels: { enabled: true }");
            client.println("    },");
            client.println("    series: { color: '#059e8a' }");
            client.println("  },");
            client.println("  xAxis: { type: 'datetime',");
            client.println("    dateTimeLabelFormats: { second: '%H:%M:%S' }");
            client.println("  },");
            client.println("  yAxis: {");
            client.println("    title: { text: 'Force (g)' }");
            client.println("    //title: { text: 'Temperature (Fahrenheit)' }");
            client.println("  },");
            client.println("  credits: { enabled: false }");
            client.println("});");
            client.println("setInterval(function ( ) {");
            client.println("  var xhttp = new XMLHttpRequest();");
            client.println("  xhttp.onreadystatechange = function() {");
            client.println("    if (this.readyState == 4 && this.status == 200) {");
            client.println("      var x = (new Date()).getTime(),");
            client.println("          y = parseFloat(this.responseText);");
            client.println("      //console.log(this.responseText);");
            client.println("      if(chartSensor0.series[0].data.length > 40) {");
            client.println("        chartSensor0.series[0].addPoint([x, y], true, true, true);");
            client.println("      } else {");
            client.println("        chartSensor0.series[0].addPoint([x, y], true, false, true);");
            client.println("      }");
            client.println("    }");
            client.println("  };");
            client.println("  xhttp.open('GET', '/sensor0', true);");
            client.println("  xhttp.send();");
            client.println("}, 30000 ) ;");
            client.println("");
            client.println("var chartSensor1 = new Highcharts.Chart({");
            client.println("  chart:{ renderTo : 'FSR1' },");
            client.println("  title: { text: 'Force Sensitive Resistor 1' },");
            client.println("  series: [{");
            client.println("    showInLegend: false,");
            client.println("    data: []");
            client.println("  }],");
            client.println("  plotOptions: {");
            client.println("    line: { animation: false,");
            client.println("      dataLabels: { enabled: true }");
            client.println("    },");
            client.println("    series: { color: '#059e8a' }");
            client.println("  },");
            client.println("  xAxis: { type: 'datetime',");
            client.println("    dateTimeLabelFormats: { second: '%H:%M:%S' }");
            client.println("  },");
            client.println("  yAxis: {");
            client.println("    title: { text: 'Force (g)' }");
            client.println("    //title: { text: 'Temperature (Fahrenheit)' }");
            client.println("  },");
            client.println("  credits: { enabled: false }");
            client.println("});");
            client.println("setInterval(function ( ) {");
            client.println("  var xhttp = new XMLHttpRequest();");
            client.println("  xhttp.onreadystatechange = function() {");
            client.println("    if (this.readyState == 4 && this.status == 200) {");
            client.println("      var x = (new Date()).getTime(),");
            client.println("          y = parseFloat(this.responseText);");
            client.println("      //console.log(this.responseText);");
            client.println("      if(chartSensor1.series[0].data.length > 40) {");
            client.println("        chartSensor1.series[0].addPoint([x, y], true, true, true);");
            client.println("      } else {");
            client.println("        chartSensor1.series[0].addPoint([x, y], true, false, true);");
            client.println("      }");
            client.println("    }");
            client.println("  };");
            client.println("  xhttp.open('GET', '/sensor1', true);");
            client.println("  xhttp.send();");
            client.println("}, 30000 ) ;");
            client.println("");
            client.println("var chartSensor2 = new Highcharts.Chart({");
            client.println("  chart:{ renderTo : 'FSR2' },");
            client.println("  title: { text: 'Force Sensitive Resistor 2' },");
            client.println("  series: [{");
            client.println("    showInLegend: false,");
            client.println("    data: []");
            client.println("  }],");
            client.println("  plotOptions: {");
            client.println("    line: { animation: false,");
            client.println("      dataLabels: { enabled: true }");
            client.println("    },");
            client.println("    series: { color: '#059e8a' }");
            client.println("  },");
            client.println("  xAxis: { type: 'datetime',");
            client.println("    dateTimeLabelFormats: { second: '%H:%M:%S' }");
            client.println("  },");
            client.println("  yAxis: {");
            client.println("    title: { text: 'Force (g)' }");
            client.println("    //title: { text: 'Temperature (Fahrenheit)' }");
            client.println("  },");
            client.println("  credits: { enabled: false }");
            client.println("});");
            client.println("setInterval(function ( ) {");
            client.println("  var xhttp = new XMLHttpRequest();");
            client.println("  xhttp.onreadystatechange = function() {");
            client.println("    if (this.readyState == 4 && this.status == 200) {");
            client.println("      var x = (new Date()).getTime(),");
            client.println("          y = parseFloat(this.responseText);");
            client.println("      //console.log(this.responseText);");
            client.println("      if(chartSensor2.series[0].data.length > 40) {");
            client.println("        chartSensor2.series[0].addPoint([x, y], true, true, true);");
            client.println("      } else {");
            client.println("        chartSensor2.series[0].addPoint([x, y], true, false, true);");
            client.println("      }");
            client.println("    }");
            client.println("  };");
            client.println("  xhttp.open('GET', '/sensor2', true);");
            client.println("  xhttp.send();");
            client.println("}, 30000 ) ;");
            client.println("");
            client.println("var chartSensor3 = new Highcharts.Chart({");
            client.println("  chart:{ renderTo : 'FSR3' },");
            client.println("  title: { text: 'Force Sensitive Resistor 3' },");
            client.println("  series: [{");
            client.println("    showInLegend: false,");
            client.println("    data: []");
            client.println("  }],");
            client.println("  plotOptions: {");
            client.println("    line: { animation: false,");
            client.println("      dataLabels: { enabled: true }");
            client.println("    },");
            client.println("    series: { color: '#059e8a' }");
            client.println("  },");
            client.println("  xAxis: { type: 'datetime',");
            client.println("    dateTimeLabelFormats: { second: '%H:%M:%S' }");
            client.println("  },");
            client.println("  yAxis: {");
            client.println("    title: { text: 'Force (g)' }");
            client.println("    //title: { text: 'Temperature (Fahrenheit)' }");
            client.println("  },");
            client.println("  credits: { enabled: false }");
            client.println("});");
            client.println("setInterval(function ( ) {");
            client.println("  var xhttp = new XMLHttpRequest();");
            client.println("  xhttp.onreadystatechange = function() {");
            client.println("    if (this.readyState == 4 && this.status == 200) {");
            client.println("      var x = (new Date()).getTime(),");
            client.println("          y = parseFloat(this.responseText);");
            client.println("      //console.log(this.responseText);");
            client.println("      if(chartSensor3.series[0].data.length > 40) {");
            client.println("        chartSensor3.series[0].addPoint([x, y], true, true, true);");
            client.println("      } else {");
            client.println("        chartSensor3.series[0].addPoint([x, y], true, false, true);");
            client.println("      }");
            client.println("    }");
            client.println("  };");
            client.println("  xhttp.open('GET', '/sensor3', true);");
            client.println("  xhttp.send();");
            client.println("}, 30000 ) ;");
            client.println("");
            client.println("var chartSensor4 = new Highcharts.Chart({");
            client.println("  chart:{ renderTo : 'FSR4' },");
            client.println("  title: { text: 'Force Sensitive Resistor 4' },");
            client.println("  series: [{");
            client.println("    showInLegend: false,");
            client.println("    data: []");
            client.println("  }],");
            client.println("  plotOptions: {");
            client.println("    line: { animation: false,");
            client.println("      dataLabels: { enabled: true }");
            client.println("    },");
            client.println("    series: { color: '#059e8a' }");
            client.println("  },");
            client.println("  xAxis: { type: 'datetime',");
            client.println("    dateTimeLabelFormats: { second: '%H:%M:%S' }");
            client.println("  },");
            client.println("  yAxis: {");
            client.println("    title: { text: 'Force (g)' }");
            client.println("    //title: { text: 'Temperature (Fahrenheit)' }");
            client.println("  },");
            client.println("  credits: { enabled: false }");
            client.println("});");
            client.println("setInterval(function ( ) {");
            client.println("  var xhttp = new XMLHttpRequest();");
            client.println("  xhttp.onreadystatechange = function() {");
            client.println("    if (this.readyState == 4 && this.status == 200) {");
            client.println("      var x = (new Date()).getTime(),");
            client.println("          y = parseFloat(this.responseText);");
            client.println("      //console.log(this.responseText);");
            client.println("      if(chartSensor4.series[0].data.length > 40) {");
            client.println("        chartSensor4.series[0].addPoint([x, y], true, true, true);");
            client.println("      } else {");
            client.println("        chartSensor4.series[0].addPoint([x, y], true, false, true);");
            client.println("      }");
            client.println("    }");
            client.println("  };");
            client.println("  xhttp.open('GET', '/sensor4', true);");
            client.println("  xhttp.send();");
            client.println("}, 30000 ) ;");
            client.println("</script>");
            client.println("</html>");




            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
