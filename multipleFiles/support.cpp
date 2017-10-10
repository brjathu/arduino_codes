#include "Arduino.h"
extern int x;
void printFun( void ){
  Serial.println( "Called helper_function" );
  Serial.println(x);
}
