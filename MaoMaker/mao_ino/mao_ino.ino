
#include <Servo.h> 
 
Servo dedo1; 
Servo dedo2;
Servo dedo3;
Servo dedo4;
const int port1 = 3;
const int port2 = 5;
const int port3 = 6;
const int port4 = 9;
const int maxVar = 90;      
           
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  dedo1.attach(port1);  
  dedo2.attach(port2);  
  dedo3.attach(port3);  
  dedo4.attach(port4);  
} 

void abreDedo(int dedo){
  
  for(pos = 0; pos < maxVar; pos += 1)  
  {
    if (dedo == 1)    
      dedo1.write(pos);
    else if (dedo == 2)
      dedo2.write(pos); 
    else if (dedo == 3)
      dedo3.write(pos);
    else 
      dedo4.write(pos);
    delay(15);                       
  } 
}
 
void fechaDedo(int dedo){
  for(pos = maxVar; pos>=1; pos-=1)  
  {                                
     if (dedo == 1)    
      dedo1.write(pos);
    else if (dedo == 2)
      dedo2.write(pos); 
    else if (dedo == 3)
      dedo3.write(pos);
    else 
      dedo4.write(pos);
    delay(15);                      
  } 
}

void abreMao(){
  abreDedo(1);
  abreDedo(2);
  abreDedo(3);
  abreDedo(4);
}

void fechaMao(){
  fechaDedo(1);
  fechaDedo(2);
  fechaDedo(3);
  fechaDedo(4);
}
 
void loop() 
{ 
 abreMao();
 fechaMao();
 
} 
