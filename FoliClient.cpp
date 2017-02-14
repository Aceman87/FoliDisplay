/**The MIT License (MIT)

  Copyright (c) 2017 Juho Suominen

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  See more at https://github.com/Aceman87/FoliDisplay
*/

#include "FoliClient.h"

boolean catchValue = false;

boolean FoliClient::updateFoliData(String stopNumber) {
  WiFiClient client;
  const int httpPort = 80;
  // Connect to http://data.foli.fi/
  if (!client.connect("192.103.106.131", httpPort)) {
    Serial.println("Foli connection failed");
    return false;
  }
  delay(1);

  Serial.print("Requesting bus stop: ");
  Serial.println(stopNumber);

  // This will send the request to the server
  // http://data.foli.fi/siri/sm/
  client.print(String("GET ") +
               "/siri/sm/" +
               stopNumber +
               "/format" +
               " HTTP/1.1\r\n" +
               "Host: data.foli.fi\r\n" +
               "Connection: close\r\n" +
               "Accept: */*\r\n" +
               "User-Agent: FoliDisplay/1.0; (Esp8266; C++; https://github.com/Aceman87/FoliDisplay; no-public)\r\n\r\n");

  while (!client.available()) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();

  //reset row pointer
  row = 0;
  //reset buffer pointer
  bufferPos = 0;
  //reset parse flags
  IN_STRING = false;
  KEY_SEPARATOR_PASSED = false;
  //reset key
  keyString = "";
  //reset data array
  //memset(busData, 0, sizeof(busData[0][0]) * 30 * 2);
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available() > 0) {
    parse(client.read());
  }

//  Serial.println();
//  for (int r = 0; r < 30; r++) {
//    Serial.println("Update: " + String(r) + "_" + getBusData(r));
//  }

  Serial.println();
  Serial.println("closing Foli connection");
  return true;
}

void FoliClient::parse(char c) {
  if (c == '"') {
    if (IN_STRING) {
      //end buffer in null
      buffer[bufferPos] = '\0';

      if (!KEY_SEPARATOR_PASSED) {
        //pass buffer to value()
        keyString = String(buffer);
      } else {
        //pass buffer to value()
        value(keyString, buffer);
        Serial.println("Value buffer: " + String(buffer));
      }

      //set in string pointer false
      IN_STRING = false;
      bufferPos = 0;
    } else {
      IN_STRING = true;
    }
  } else if (c == ':' && !IN_STRING) {
    KEY_SEPARATOR_PASSED = true;
  } else if (c == '\n') {
    KEY_SEPARATOR_PASSED = false;
    IN_STRING = false;
    bufferPos = 0;
  } else if (IN_STRING) {
    buffer[bufferPos] = c;
    bufferPos++;
  }
}

void FoliClient::value(String key, char value[]) {
  if (key == "lineref") {
    busData[row][5] = '|';
    busData[row][6] = value[0];
    busData[row][7] = value[1];
    busData[row][8] = value[2];
    busData[row][9] = value[3];
    busData[row][10] = '\0';    
    
//    String rowString = "";
//    rowString += busData[row][5];
//    rowString += busData[row][6];
//    rowString += busData[row][7];
//    rowString += busData[row][8];
//    rowString += busData[row][9];
//    rowString += busData[row][10];
//    Serial.println("Value: " + String(row) + "_" + rowString);
    
  } else if (key == "expecteddeparturetime") {
    busData[row][0] = value[11];
    busData[row][1] = value[12];
    busData[row][2] = value[13];
    busData[row][3] = value[14];
    busData[row][4] = value[15];
    
//    String rowString = "";
//    rowString += busData[row][0];
//    rowString += busData[row][1];
//    rowString += busData[row][2];
//    rowString += busData[row][3];
//    rowString += busData[row][4];
//    Serial.println("Value: " + String(row) + "_" + rowString);

    row++;
  }
  delay(1);
}

String FoliClient::getBusData(int row) {
  return String(busData[row]);
}

