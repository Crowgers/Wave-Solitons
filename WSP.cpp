// Water Wave Solitons :
// This program reads in voltages from a 12 bit dual AtoD convertor and writes them to a file.
// Standard Libraries.
#include <iostream> 
#include <fstream>
#include <string> 
#include <cstdlib>
#include <cmath>
// Custom Libraries.
#include <UCDLabs.h>
#include <Windows.h>
#include <NanoTimer.h> 
// Housekeeping.
using namespace std ;
// Main Program.
int main()
{
// Numerical and word variables.
    string filename ;                       // filename string.
    unsigned long BASE = 0xD800 ;           // Base Port.
    unsigned long PORTA = BASE ;            // 1st input port.
    unsigned long PORTB = BASE + 1 ;        // 2nd input port.
    unsigned long PORTC = BASE + 2 ;        // 1st output port.
    unsigned long PORTCTRL = BASE + 3 ;     // Controls the port configuration.
    int LSBPr1, LSBPr2 ;                    // Least significant bits. Pr1 = Probe 1
    int MSBPr1, MSBPr2 ;                    // Most Significant bits.  Pr2 = Probe 2
    int TBVPr1, TBVPr2 ;                    // Twelve Bit Value.
    int i, n ; 
    double V1, V2 ;                         // Filestream Values
// Configuring input and output ports as A,B : input & C : output.
    outport(PORTCTRL, 146) ;
// User Provides filename. 
    cout << "Please enter a filename and include extension .txt Eg. test1.txt." << endl ;
    cout << "Filename : " ;
    cin >> filename ;
// Number of points to take.
    cout << "Enter number of points : " ;
    cin >> n ;
// Creating a filestream to write data to.
    ofstream filestr ; 
    filestr.open(filename.c_str()) ;       // Opens file and converts to a c string. 
    if(!filestr.is_open())                 // Checks if file was opened correctly.
        {
            cout << "Error opening file : " << filename << "." << endl ;
            cout << "Exiting before computer explodes . . . " << endl ;
            return 1 ;
        }
    else 
        {
            cout << "File has been opened succesfully." << endl ; 
            cout << "Ready to start data acquisition." << endl ;
            system("PAUSE") ;
        }
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS) ;
// Timer for duration of program. 
            NanoTimer t1 ;
            t1.start() ;                   // Starts the timer.
// Main loop for data acquisition and converting values from 8 bit to twelve bit.
    for(i=0 ; i<n ; i++) 
        { 
// Reading Voltages from Probe 1. 
            outport(PORTC, 1+64) ;         // Configuring PortC(0) as LO and PortC(6) as HI.
            MilliSleep(1) ;                // Pauses to allow Configuration. 
            outport(PORTC, 1+64+2) ;       // Configures PortC(1) as HI. 
            LSBPr1 = inport(PORTA) ;       // Reads in the least significant bits.
            MilliSleep(1) ;                // Pause to allow conversion.  
            MSBPr1 = inport(PORTB) ;       // Reads in the most significant bits.
// Constructing the 12 bit value using entire LSBPr1 and uses the 4 MSB from MSBPr1. 
            TBVPr1 = ( (MSBPr1 & 15) <<8) + LSBPr1 ; 
// Assigns a voltage value to the 12 bit digital value.  4096 values(0 - 4095) and 10.0 volt range. 
            V1 = (10.0/4095.0)*TBVPr1 - 5.0 ;   
            outport(PORTC, 0) ;            // Reset PORTC to all 0's for next part.
            MilliSleep(1) ;
// Reading Voltages from Probe 2.
            outport(PORTC, 1+64) ;         // Configuring PortC(0) as LO and PortC(6) as HI.
            MilliSleep(1) ;                // Pauses to allow Configuration. 
            outport(PORTC, 1+64+128) ;     // Configures PortC(1) as HI. 
            LSBPr2 = inport(PORTA) ;       // Reads in the least significant bits.
            MilliSleep(1) ;                // Pause to allow conversion. 
            MSBPr2 = inport(PORTB) ;       // Reads in the most significant bits.
// Constructing the 12 bit value using entire LSBPr2 and uses the 4 MSB from MSBPr2. 
            TBVPr2 = ( (MSBPr2 & 15) <<8) + LSBPr2 ; 
// Assigns a voltage value to the 12 bit digital value.  4096 values(0 - 4095)and 10.0 (-5.0 - +5.0) volt range. 
            V2 = (10.0/4095.0)*TBVPr2 - 5.0 ;
            outport(PORTC, 0) ;            // Reset PORTC to all 0's for next loop.
            MilliSleep(1) ;
// Prints both probes voltage values to the screen.
            cout << "Voltage1 : " << V1 << " Volts." << " Voltage2 : " << V2 << " Volts." << endl ;
            filestr << i << " " << V1 << " " << V2 << endl ;     // Writes value to file.
       }
// Reset the programs priority to normal. 
            t1.stop() ;                    // Stops timer.
            cout << "Time interval : " << (t1.value())/n <<  "ms." << endl ;
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS) ;
    filestr.close() ;                      // Closes File Stream.
    system ("PAUSE") ;
return 0 ;
}
