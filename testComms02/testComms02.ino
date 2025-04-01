//NEW Arduino communications test file
// /Users/garygao/Desktop/test_Comms/test_Comms.ino
#include "Comms.h"
Comms Comms(":", "    ");

const int nvalues = 1000;

// make an array with n values and populate it
int testArray[nvalues] = {0};


void setup() {
  // put your setup code here, to run once:

  for (int i=0; i < nvalues; i++){
    testArray[i] = i;
  }

  Serial.begin(250000);
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil(';');
    if (message=="print") {
      Comms.printParameters();  
    }
    else if (message=="receive") {
      Comms.receiveParameters();
    } 
    else if (message=="addparam") {
      Comms.addParameters();
    }
    else if (message=="resetparam") {
      Comms.resetParameters();
    }
    else if (message=="sendtest") {
      Comms.send1DArrayToPython("testArray", testArray, nvalues);
    }
    else if (message=="display") {
      for (int i = 0; i < Comms.num_params; i++){
        Serial.print(Comms.param_names[i]);
        Serial.print(": ");
        Serial.println(Comms.params_value[i]);
      }
    }
  }

};
