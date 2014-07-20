/**************************************************************
  Name      GPS_Shield_SDCard                                
  Author    Bob Powell 
            
            texanfromiowa@gmail.com
            Copyright (C) 2012-2014, Parallelus Automation, Inc.
          
  Date      Febuary 24, 2014    
  Modified  July 9, 2014                               
  Version   1.0.1     
  Arduino   1.5.6-r2
  
  Notes     This demo shows use of logging GPS data to an SD Card for later review.
  
            GPS Unit used: ITEAD GPS shield Model IM120417017
            
            Datasheet & : http://imall.iteadstudio.com/im120417017.html
 
            The jumper pins set for Arduino pin 3 for RX and pin 4 for TX, and 
            toggle switch set for 5V.  A 2gig SD card was used 
            
            The beginning of the program was taken from the TinyGPS "simple_test"
            example.
            
   Version: 1.0.1 - Updated libraries for Arduino IDE 1.5.6-r2
                  -    
            
Legal Stuff:
============

            This program is free software: you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published by
            the Free Software Foundation, either version 3 of the License, or
            at your option, any later version.
     
            This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
            GNU General Public License for more details.
     
            You should have received a copy of the GNU General Public License
            along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
     
Personal note:
==============

            If you do something interesting with this code, expand it, or just
            have a question, please email me at the address above.  

            I hope you find this example helpful.  Enjoy.

            Bob

****************************************************************/
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS.h>


TinyGPS gps;
// The jumpers on the shield set to work with
// these pins, and can be changed if needed.
SoftwareSerial ss(2, 3);

// File for logging data
File rawdata;

/**************************************************************
Function: setup
Purpose:  set up Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino
***************************************************************/
void setup()
{
  
  // Serial monitor
  Serial.begin(9600);
  // baud rate for GPS - 38400 is prefered, but 4800 can also be used
  ss.begin(38400);
  
  // Comments from TinyGPS simple_test example
  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  
  // Initialize card
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    Serial.println("...initialization failed!");
  }
  else
    Serial.println("...initialization done.");

}

/**************************************************************
Function: loop
Purpose:  loop funtion for Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
          
***************************************************************/
void loop(){
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // Open data file
  File rawdata = SD.open("rawdata.txt", FILE_WRITE);
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
       // Send raw data to the terminal window
       Serial.write(c); 
       
       // Send raw data to the SD card
       if(rawdata)
         rawdata.write(c);
         
      if (gps.encode(c)) 
        newData = true;
    }
  }

  // Send manipulate data to the terminal window.
/*  
  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
*/  
  // Send a carriage return and close the file
  Serial.println("");
  rawdata.write("\r");
  rawdata.close();
  

}
