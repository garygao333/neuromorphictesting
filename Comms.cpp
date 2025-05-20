#include "Arduino.h"
#include "Comms.h"

// Comms::Comms(char* divider, String buffer)
Comms::Comms(const char* divider, const char* buffer)
{
    _divider = divider; //divider is ":" --> a C-string
    _buffer = buffer; //buffer is " " --> an Arduino String
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

//REFACTORED: Because the char message will be sent with a null-terminator \0, we don't actually need a for loop or strlen. 
void Comms::sendMessageWithVarToPython(const char* message, int var)
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

// void Comms::sendMessageWithVarToPython(const char* message, int a, int var) {
//   Serial.println(); // buffer line print
//   Serial.print(_buffer); // buffer characters
//   Serial.print(_divider);
//   Serial.print("msgvar");
//   Serial.print(_divider);
//   for (int i = 0; i < a; i++) {
//     Serial.print(message[i]);
//   }
//   Serial.print(_divider);
//   Serial.print(var);
//   Serial.print(_divider);
//   Serial.println(_buffer); // buffer characters
// }

void Comms::printParameters() {
    Serial.println("here are parameters");
    for (int i =0; i < num_params; i++) {
      sendMessageWithVarToPython(param_names[i], params_value[i]); //Currently not the best solution but works for now. 
        // Serial.print(param_names[i]);
        // Serial.print(":");
        // Serial.println(params_value[i]);
    }
}

//REFACTORING THE RECEIVE PARAMETERS CODE.
// void Comms::receiveParameters() {
//     int param_idx;
    
//     int num_received = 0;
    
//     while (num_received < num_params) {
//         if (Serial.available()) {
//             codon =  Serial.readStringUntil(';');
//             param_idx = findInArray(codon);
            
//             if (param_idx >= 0) {
//                 params_value[param_idx] = Serial.readStringUntil(';').toInt();
//                 // Serial.print(param_names[param_idx]);
//                 // Serial.print(": ");
//                 // Serial.println(params_value[param_idx]);
//                 // Serial.println(';');
//                 num_received++;
//               } else {
//                 Serial.print("Invalid Codon, need more param values;");
//               }
//             }
//         }
// }

// void Comms::receiveParameters() {
//   int param_idx; 
//   int num_recieved = 0;
//   char*[] arrayList; //Explore the possibility of using 1. a LinkedList 2. an array initialized with some values and then just replace it with characters as the read string goes down the list.

//   while (num_received < num_params) {
//     if (Serial.available()) {
//       char* placeholder = c;
//       if (param_idx >= 0) {
//         while (placeholder != ';') {
//           placeholder = Serial.read();
//           arrayList.append(placeholder);
//       }
//       Serial.print(param_names[param_idx]);
//       Serial.print(": ");
//       Serial.println(params_value[param_idx]);
//       Serial.println(';');
//       num_received++;
//       } else {
//         Serial.print("Invalid Codon, need more param values.")
//       }
//     }
//   }
// }

//Char reader helper function
void Comms::readCharUntil(char* arr, char terminator) {
  int index = 0; 
  while (Serial.available() && index < 19) {
    char c = Serial.read();
    if (c == terminator) {
      break;
    }
    arr[index] = c;
    index++;
  }
  arr[index] = '\0';
}

void Comms::receiveParameters() {
  int num_received = 0; 

  while (num_received < num_params) {
    if (Serial.available()) {
      char codon[20];
      readCharUntil(codon, ';');
      int param_idx = findInArray(codon);
      if (param_idx >= 0) {
        char value[20];
        readCharUntil(value, ';');
        // strcpy(params_value[param_idx], value);
        params_value[param_idx] = atoi(value);
        Serial.print(param_names[param_idx]);
        Serial.print(": ");
        Serial.println(params_value[param_idx]);
        Serial.println(';');
        num_received++;
      } else {
        Serial.print("Invalid Codon, need more param values;");
      }
    }
  }
}

// RETRIEVE SHOULD MORE OR LESS BE THE SAME, JUST MAKE SURE TO CHANGE THE INPUT FROM THE STRING PARAM NAME TO A CHAR[] FOR THE MAPPING. 
int Comms::retrieve(const char* param_name){
    int param_idx = findInArray(param_name);
    if (param_idx >= 0){
        return params_value[param_idx];
    }
    else{
        Serial.println("Invalid param name");
        return -1;
    }
}

//ADD PARAMETERS OPERATES IN A SIMILAR WAY - CAN EITHER ADD BY ITERATING THROUGH THE INPUTTED CHAR ARRAY (EG. REAPLACING READSTRINGUNTIL WITH SOMETHING ELSE) OR JUST INITIALIZING IT WITH SOME DUMMY AND GO THROUGH AND REPLACE. 
// void Comms::addParameters() {

//   bool keepReading = true;
//   while (keepReading) {  
//         String param = Serial.readStringUntil(';');
//         if (param == "\n"){ //if you hit and end of line, terminate
//         keepReading = false;
//         }
//         else if (param.length() > 0) { 
//             param_names[num_params] = param; 
//             num_params++;
//         }
//         else { //if you have a string of length 0, terminate
//             keepReading = false;
//         } 
//   }
    // Serial.print("num_params = ");
    // Serial.println(num_params); 
    // Comms.sendMessageWithVarToPython("num_params", num_params);
// }

void Comms::addParameters() {

  while (!Serial.available()) ;
  delay(3);  

  char dataReceived[100];
  int index = 0;
  while (Serial.available() && index < 99) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        break;
      }
      dataReceived[index++] = c;
  }
  dataReceived[index] = '\0';

  char* token = strtok(dataReceived, ";");
  while (token != NULL) {
    strncpy(param_names[num_params], token, 19);
    param_names[num_params][19] = '\0';

    num_params++;
    token = strtok(NULL, ";");
  }
  Serial.print("num_params = ");
  Serial.println(num_params);
  sendMessageWithVarToPython("num_params", num_params);
}

//CHECK THIS OVER --> WHAT THIS IS SUPPSOED TO DO IS TO BE ABLE TO TAKE IN AN ARRAY AND MANUALLY ADD PARAMETERS IN ARDUINO WITHOUT PYTHON. 
// void Comms::addParameters_manual(String* params, int len) {
//     for (int idx = 0; idx < len; idx++){
//         param_names[idx] = params[idx];
//     }
//     num_params = len;
// }

// void Comms::addParameters_manual(const char* params[], int len) { 
//   for (int idx = 0; idx < len; idx++) {
//     const char* hold = params + idx*a;
//     strncpy(params, hold, a);

//   }
// }

void Comms::resetParameters() {
  num_params = 0;
}


// int Comms::findInArray(String elem) {
//     for (int idx = 0; idx < num_params; idx++) {
//         if (elem.equals(param_names[idx])) {
//             return idx;
//         }
//     }
//     return -1;
// }

int Comms::findInArray(const char* elem) {
  for (int idx = 0; idx < num_params; idx++) {
    if (strcmp(elem, param_names[idx]) == 0) {
      return idx;
    }
  }
  return -1;
}
