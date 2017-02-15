#include "base_dimmer.h"
#include "simple_dimmer.h"
#include "simple_kitchen_dimmer.h"
#include "living_room_dimmer.h"


SimpleDimmer sd1(4, 5);
SimpleKitchenDimmer sd2(6, 3, 12); 
LivingRoomDimmer sd3(A2,8,7);
 
void setup() 
{
  sd1.setup();
  sd2.setup();
  sd3.setup();
}
 
void loop() 
{
    sd1.loop();
    sd2.loop();    
    sd3.loop();
}
