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

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

class FoliClient {
  private:
    char busData[30][11];
    int row = 0;
    String keyString;
    boolean IN_STRING = false;
    boolean KEY_SEPARATOR_PASSED = false;
    char buffer[512];
    int bufferPos = 0;

  public:
    boolean updateFoliData(String busStop);

    void parse(char c);

    void value(String key, char value[]);

    String getBusData(int row);
};
