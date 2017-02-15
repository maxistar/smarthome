#include "dimmer_types.h"
#include "simple_switcher.h"


//корридор
SimpleSwitcher sd1(A0, A4);
SimpleSwitcher sd2(A2, A5);

//A6, A7 do not work as outut

//A5 works as input
//A6 does not work as input too!!!
//A7 does not work as input too!!!

//ванная
//SimpleSwitcher sd2(A7, A6);

 
void setup() 
{
  sd1.setup();
  sd2.setup();
}
 
void loop() 
{
    sd1.loop();
    sd2.loop();    
}
