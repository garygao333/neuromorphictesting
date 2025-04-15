#include "Arduino.h"
#include "Comms.h"

Comms::Comms(char* divider, String buffer)
{
    _divider = divider;
    _buffer = buffer;
    num_params=0;

    // addParameters("ETA;SPEED;ALF;EPO;MES;NMS;NSR;NTR;NEG;NDR;NDS;");

}

void Comms::sendMessageToPython(const char* message) 
{
  // Serial.println(); // buffer line print
  Serial.print(_buffer); // buffer characters
  Serial.print(_divider);
  Serial.print("msg");
  Serial.print(_divider);
  Serial.print(message);
  Serial.print(_divider);
  Serial.println(_buffer); // buffer characters
}

void Comms::send1DArrayToPython(const char* varname, int* arr, int len)
{
  Serial.print(_buffer); // buffer characters
  Serial.print(_divider);
  Serial.print("1Darr");
  Serial.print(_divider);
  Serial.print(varname);
  Serial.print(_divider);
  Serial.print("[");
  for (int i = 0; i < len; i++) {
    Serial.print(arr[i]);
    if (i < (len)-1){
      Serial.print(", ");
    }
    else{
      Serial.print("]");
    }
  }
  Serial.print(_divider);
  Serial.println(_buffer);
}

void Comms::sendArrayToPython(const char* varname, int* arr, int a, int b, int c)
{
  Serial.print(_buffer); // buffer characters
  Serial.print(_divider);
  Serial.print("arr");
  Serial.print(_divider);
  Serial.print(varname);
  Serial.print(_divider);
  Serial.print("[");
  for (int i = 0; i < a*b*c; i++) {
    Serial.print(arr[i]);
    if (i < (a*b*c)-1){
      Serial.print(", ");
    }
    else{
      Serial.print("]");
    }
  }
  Serial.print(_divider);
  Serial.print("[");
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.print(c);
  Serial.print("]");
  Serial.println(_buffer);
}

void Comms::sendMessageWithVarToPython(const String message, int var)
{
  // Serial.println(); // buffer line print
  Serial.print(_buffer); // buffer characters
  Serial.print(_divider);
  Serial.print("msgvar");
  Serial.print(_divider);
  Serial.print(message);
  Serial.print(_divider);
  Serial.print(var);
  Serial.print(_divider);
  Serial.println(_buffer); // buffer characters
}

void Comms::printParameters() {
    // Serial.println("here are parameters");
    for (int i =0; i < num_params; i++) {
      sendMessageWithVarToPython(param_names[i], params_value[i]);
        // Serial.print(param_names[i]);
        // Serial.print(":");
        // Serial.println(params_value[i]);
    }
}

void Comms::receiveParameters() {
    int param_idx;
    
    int num_received = 0;
    
    while (num_received < num_params) {
        if (Serial.available()) {
            codon =  Serial.readStringUntil(';');
            param_idx = findInArray(codon);
            
            if (param_idx >= 0) {
                params_value[param_idx] = Serial.readStringUntil(';').toInt();
                // Serial.print(param_names[param_idx]);
                // Serial.print(": ");
                // Serial.println(params_value[param_idx]);
                // Serial.println(';');
                num_received++;
              } else {
                Serial.print("Invalid Codon, need more param values;");
              }
            }
        }
}

int Comms::retrieve(String param_name){
    int param_idx = findInArray(param_name);
    if (param_idx >= 0){
        return params_value[param_idx];
    }
    else{
        Serial.println("Invalid param name");
        return -1;
    }
}

void Comms::addParameters() {

  bool keepReading = true;
  while (keepReading) {  
        String param = Serial.readStringUntil(';');
        if (param == "\n"){ //if you hit and end of line, terminate
        keepReading = false;
        }
        else if (param.length() > 0) { 
            param_names[num_params] = param; 
            num_params++;
        }
        else { //if you have a string of length 0, terminate
            keepReading = false;
        }
  }
    // Serial.print("num_params = ");
    // Serial.println(num_params); 
    // Comms.sendMessageWithVarToPython("num_params", num_params);
}

//CHECK THIS OVER --> WHAT THIS IS SUPPSOED TO DO IS TO BE ABLE TO TAKE IN AN ARRAY AND MANUALLY ADD PARAMETERS IN ARDUINO WITHOUT PYTHON. 
void Comms::addParameters_manual(String* params, int len) {
    for (int idx = 0; idx < len; idx++){
        param_names[idx] = params[idx];
    }
    num_params = len;
}

void Comms::resetParameters() {
  num_params = 0;
}


int Comms::findInArray(String elem) {
    for (int idx = 0; idx < num_params; idx++) {
        if (elem.equals(param_names[idx])) {
            return idx;
        }
    }
    return -1;
}
