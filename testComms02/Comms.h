#include <Arduino.h>
#ifndef Comms_h
#define Comms_h


const int max_num_params = 50;

class Comms{

public:

    Comms(char* divider, String buffer);

    void sendMessageToPython(const char* message);
    void send1DArrayToPython(const char* varname, int* arr, int len);  
    void sendArrayToPython(const char* varname, int* arr, int a, int b, int c);   
    void sendMessageWithVarToPython(const char* message, int var);

    void receiveParameters();
    void printParameters(); 

    void addParameters();
    // void addParameters_manual(String* params, int len);
    int retrieve(String param_name);
    void resetParameters();

    String param_names[max_num_params];
    int params_value[max_num_params];
    int num_params;

private:
    char* _divider;
    String _buffer;

    String codon;
    
    int findInArray(String elem);

};

#endif
