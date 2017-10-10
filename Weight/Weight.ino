/* sample for digital weight scale of hx711
 * library design: Weihong Guan (@aguegu)
 * library host on
 *https://github.com/aguegu/ardulibs/tree/3cdb78f3727d9682f7fd22156604fc1e4edd75d1/hx711
 */
 // See original article and code / library link at http://arduinotronics.blogspot.com/2015/06/arduino-hx711-digital-scale.html

// Hx711.DOUT - pin #A2
// Hx711.SCK - pin #A3

#include "HX711.h"
HX711 scale(A2, A3);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(scale.read_average(20));
  Serial.println(" g");
  delay(200);
}
