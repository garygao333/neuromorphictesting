#include <Arduino.h>
#ifndef Comms_h
#define Comms_h

/*

param_names is a string array that holds parameters names. 
params_value is an array that is parallel/maps to param_names storing each parameter's value. 
num_params is the current number of parameters. 
codon: a simple scratch String that is used to read values from serial. 


*/


const int max_num_params = 50;

class Comms{

public:

    // Comms(char* divider, String buffer);
    Comms(const char* divider, const char* buffer);

    void sendMessageToPython(const char* message);
    void send1DArrayToPython(const char* varname, int* arr, int len);  
    void sendArrayToPython(const char* varname, int* arr, int a, int b, int c);   
    void sendMessageWithVarToPython(const char* message, int var);
    void readCharUntil(char* arr, char terminator);

    void receiveParameters();
    void printParameters(); 

    void addParameters();
    // void addParameters_manual(String* params, int len);
    int retrieve(const char* param_name);
    void resetParameters();

    char param_names[max_num_params][20];
    int params_value[max_num_params];
    int num_params = 0;

private:
    // char* _divider;
    // String _buffer;

    const char* _divider;
    const char* _buffer;

    // String codon;
    
    int findInArray(const char* elem);

};

#endif
