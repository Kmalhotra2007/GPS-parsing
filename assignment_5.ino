/*
   FILE          : Assignment_5
   PROJECT       : PROG8125 - ASSIGNMENT #5
   PROGRAMMER    : Kunal Malhotra
   FIRST VERSION : 2016-07-20
   DESCRIPTION   : This code waits for a $ signifying the start of transmission places the bytes that follow into an array called recBuffer[] until a CR is received.After that it takes
                   the GPS String as an input and parses it separately in different rows for ALtitude, Longitude, Latitude etc.After that it separates different values in time as 
                   hh mm and ss. Similar type of work is done on longitude and latitude and they are separated as mmm & mmmmm. The altitude value is converted to a float type value 
                   using atof() function and is printed below the rest of the things on the serial port. In the end the checksum byte is printed after being converted from a string to
                   an integer using stroul function.
                   The function CheckForRecvdCharAndParseGPS(void) takes the input from the Serial port and parses the input GPS string.
                   The function PrintingTime()converts the parsed time string to integer ad prints hours, minutes and seconds separately
                   The function PrintingLatitude converts the parsed Latitude String to an integer value and is further separated into mmm & mmmm and prints the same value.
                   The function PrintingLongitude()converts the parsed Longitude String to an integer value and is further separated into mmm & mmmm and prints the same value.
                   The function PrintingFloatTypeAltitude() converts the altitude value to a float type and prints it on the serial port
                   The function PrintingCheckSum() converts the hexadecimal string to an integer value and then prints it on the serial port.
*/
/*
  $GPGGA,014729.10,4303.5753,N,08019.0810,W,1,6,1.761,214.682,M,0,M,0,*5D

*/
#define ARRAYSIZE 255                                                                           //Defining the array size as 255 as i am not going to change it
const int16_t CarriageReturn = 13 ;                                                             // Value 13 is the value for carriage return,if the value of our buffer is equal to 13
                                                                                                // that means out GPS string has ended
const int16_t InitialValue = 0 ;                                                                // Initial value for different variables set as 0
const int32_t SerialPortDelay = 2000;                                                           //Assuming Time taken to for the serial port to initialise as 2 secs
const int32_t SerialBaudRate = 9600;                                                            //Setting the Serial Baud rate freequency as 9600
char *FirstItem = NULL;                                                                         //
char *SecondItem = NULL;                                                                        //
char *ThirdItem = NULL;                                                                         //
char *FourthItem = NULL;                                                                        //
char *FifthItem = NULL;                                                                         //These pointer items are used for parsing of the GPS string, Its been declared as global
char *SixthItem = NULL;                                                                         //because they are being used in different functions more than one time.The initial
char *SeventhItem = NULL;                                                                       //value of these pointer variables is being set as NULL and the logic related to that
char *EighthItem = NULL;                                                                        //is eing used in the code given below.
char *NinthItem = NULL;                                                                         //
char *TenthItem = NULL;                                                                         //
char *EleventhItem = NULL;                                                                      //
char *TwelthItem = NULL;                                                                        //
char *ThirteenthItem = NULL ;                                                                   //
char *FourteenthItem = NULL;                                                                    //Declaring the function prototypes so that the compiler knows what to expect in 
char *FifteenthItem = NULL;                                                                     //the program
void CheckForRecvdCharAndParseGPS();                                                            //                                   
void PrintingTime();                                                                            //
void PrintingLatitude();                                                                        //
void PrintingLongitude();                                                                       //
void PrintingFloatTypeAltitude();                                                               //
void PrintingCheckSum();                                                                        //

// FUNCTION      : CheckForRecvdCharAndParseGPS(void)
// DESCRIPTION   : This function takes the inpur of the string from the serial port using Serial.read() and it knows when the string has ended using Carriage return.
//                 After that it uses strtok() to parse the GPS string using "," as the deliminator  and prints the string separately on the serial port.
//                 Further different functions are called within this function to print altitude, longitude and latitude separately.
// PARAMETERS    : None
// RETURNS       : Nothing

void CheckForRecvdCharAndParseGPS(void)
{
  int RecByteCtr = InitialValue;                                                                 //Declaring and initializing the rec'd byte counter variable to 0
  char RecBuffer [ARRAYSIZE] = {InitialValue};                                                   //Declaring the array in which we are going to store our GPS string 
  char ByteRead;                                                                                 //Initializing the Byte string which will be used to store the GPS string in the array
  byte Items = InitialValue;                                                                     //Items is used as a counter for my on convinience and it is initialised to 0
  const char *delim  = ",";                                                                      //Using , as a deliminator to parse the GPS string

  while (Serial.available())                                                                     //Checks if any byte is available on the serial port
  {
    if (RecByteCtr == InitialValue)                                                              //Checks if the Rect Byte is equal to 0 which tells it is the start of the message
    {
      ByteRead = Serial.read();                                                                  //Reading the bytes in the variable called ByteRead
      if (ByteRead == '$')                                                                       //Its checking if the first byte is $ as the strng starts with a $ sign
      {
        RecBuffer[RecByteCtr] = ByteRead;                                                        //Copying the byte inside the rect buffer
        RecByteCtr++;                                                                            //Incrementing the byte to store rest of the gps string in the buffer
      }
    }
    if (RecByteCtr != InitialValue)                                                              //Then we're reading bytes after receiving an STX in the message stream
    {
      ByteRead = Serial.read();                                                                  //Reads the byte again
      if (ByteRead != CarriageReturn)                                                            //Checks if the calue is equal to carriage return(13 = CR) which means end of the string
      {
        RecBuffer[RecByteCtr] = ByteRead;                                                        //If its not equal to the carriage return it further reads the string and stores it in 
                                                                                                 // the buffer  
        RecByteCtr++;                                                                            //Incrementing the byte to store rest of the gps string in the buffer
      }
      else                                                                                       //This mean that the recbyte == 13 , which is end of the string
      {
        RecBuffer[RecByteCtr] = '\0';                                                            //Null terminates the rec'd string in rec buffer

        RecByteCtr = InitialValue;                                                               //The RecByteCtr is again initialised to 0 for reading the next incoming GPS string
        Serial.printf("%s", RecBuffer);                                                          //Prints the string on the serial port
      }
    }
  }

  FirstItem = strtok(RecBuffer, delim);                                                          //Using strtok function to parse the first item of the string using , as the deliminator
  
  if (FirstItem != NULL)                                                                         //This means there is something stored in that address
  {
    Items = 1;                                                                                   //The item counter counts it as the first item
    Serial.print("Sentence ID: ");                                                               //Prints the Sentence Id from th GPS string
    Serial.println(FirstItem);                                                                   //on the serial port as it is the first item
    SecondItem = strtok(NULL, delim);                                                            //Using strtok function to parse the second item of the string using , as the deliminator
    
    if (SecondItem != NULL)                                                                      //This means there is something stored in that address
    {
      Items = 2;                                                                                 //The item counter counts it as the second item
      Serial.print("UTC Time: ");                                                                //Prints UTC time on the serial port as it 
      Serial.println(SecondItem);                                                                //is the second item on the GPS string                                                           
      ThirdItem = strtok(NULL, delim );                                                          //Using strtok function to parse the second item of the string using , as the deliminator
      if (ThirdItem != NULL)                                                                     //This means there is something stored in that address
      {
        Items = 3;                                                                               //The item counter counts it as the third item
        Serial.print("Latitude: ");                                                              //Prints Latitude on the serial port as it 
        Serial.println(ThirdItem);                                                               //is the third item on the GPS string 
        FourthItem = strtok(NULL, delim);                                                        //Starting the Fourth item with a null because there is NULL(/o)at theend of the last
                                                                                                 //parsed string
        if (FourthItem != NULL)                                                                  //This means there is something stored in that address
        {
          Items = 4;                                                                             //The item counter counts it as the fourth item
          Serial.print("Direction of Measured Latitude: ");                                      //Prints fourth item on the screen 
          Serial.println(FourthItem);                                                            //which is latirude
          FifthItem = strtok(NULL, delim);                                                       //Starting the Fourth item with a null because there is NULL(/o)at theend of the last
                                                                                                 //parsed string
          if (FifthItem != NULL)                                                                 //This means there is something stored in that address
          {
            Items = 5;                                                                           //The item counter counts it as the fifth item
            Serial.print("Longitude: ");                                                         //Prints fifth item on the screen 
            Serial.println(FifthItem);                                                           //which is longitude
            SixthItem = strtok(NULL, delim);                                                     //Starting the Sixth item with a null because there is NULL(/o)at theend of the last
                                                                                                 //parsed string
            if (SixthItem != NULL)                                                               //This means there is something stored in that address
            {
              Items = 6;                                                                         //The item counter counts it as the sixth item
              Serial.print("Measure Direction of Longitude: ");                                  //Prints sixth item on the screen
              Serial.println(SixthItem);                                                         //which is direction of the longitude
              SeventhItem = strtok(NULL, delim);                                                 //Starting the Seventh item with a null because there is NULL(/o)at theend of the last
                                                                                                 //parsed string
              if (SeventhItem != NULL)                                                           //This means there is something stored in that address
            {    
                Items = 7;                                                                       //The item counter counts it as the seventh item
                Serial.print("Position Fix: ");                                                  //Prints seventh item on the screen
                Serial.println(SeventhItem);                                                     //which is Position FIx in the GPS Srting
                EighthItem = strtok(NULL, delim);                                                //Starting the Seventh item with a null because there is NULL(/o)at theend of the last
                                                                                                 //parsed string
                if (EighthItem != NULL)                                                          //This means there is something stored in that address
                {
                  Items = 8;                                                                     //The item counter counts it as the eight item
                  Serial.print("Sattelites Used: ");                                             //Prints the eight item on the screen 
                  Serial.println(EighthItem);                                                    //that is the nmber of sattelites used.
                  NinthItem = strtok(NULL, delim);                                               //Starting the ninth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  if (NinthItem != NULL)                                                         //This means there is something stored in that address
                  {
                    Items = 9;                                                                   //The item counter counts it as the ninth item
                    Serial.print("HDOP: ");                                                      //Prints HDOP on the serial port after the lst printed thing
                    Serial.println(NinthItem);                                                   //as it is the ninth item in the GPS string
                    TenthItem = strtok(NULL, delim);                                             //Starting the tenth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }
                  if (TenthItem != NULL)                                                         //This means there is something stored in that address
                  {
                    Items = 10;                                                                  //The item counter counts it as the tenth item
                    Serial.print("Altitude: ");                                                  //Prints Altitude on the serial port after the last printed value
                    Serial.println(TenthItem);                                                   //as it is the tenth item in the GPS string
                    EleventhItem = strtok(NULL, delim);                                          //Starting the eleventh item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }
                  if (EleventhItem != NULL)                                                      //This means there is something stored in that address
                  {
                    Items = 11 ;                                                                 //The item counter counts it as the eleventh item
                    Serial.print("Altitude Units: ");                                            //Prints Altitude units on the serial port after the lst printed value
                    Serial.println(EleventhItem);                                                //as it is the eleventh item in the GPS string
                    TwelthItem = strtok(NULL, delim);                                            //Starting the twelth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }
                  if (TwelthItem != NULL)                                                        //This means there is something stored in that address
                  {
                    Items = 12 ;                                                                 //The item counter counts it as the twelth item
                    Serial.print("Geoid Separation: ");                                          //Prints Altitude units on the serial port after the last printed value
                    Serial.println(TwelthItem);                                                  //as it is the twelth item in the GPS string
                    ThirteenthItem = strtok(NULL, delim);                                        //Starting the thirteenth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }
                  if (ThirteenthItem != NULL)                                                    //This means there is something stored in that address
                  {
                    Items = 13;                                                                  //The item counter counts it as the thirteenth item
                    Serial.print("Units(in meters): ");                                          //Print units on the serial port after the last printed value
                    Serial.println(ThirteenthItem);                                              //as it is the thirteenth item in the GPS string
                    FourteenthItem = strtok(NULL, delim);                                        //Starting the thirteenth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }
                  if (FourteenthItem != NULL)                                                    //This means there is something stored in that address
                  {
                    Items = 14;                                                                  //The item counter counts it as the fourteenh item
                    Serial.print("DGPS Age: ");                                                  //Print DGPS Age on the serial port after the last printed value
                    Serial.println(FourteenthItem);                                              //as it is the fourteenth item on the GPS string
                    FifteenthItem = strtok(NULL, delim);                                         //Starting the thirteenth item with a null because there is NULL(/o)at the end of the last
                                                                                                 //parsed string
                  }

                  if (FifteenthItem != NULL)                                                     //This means there is something stored in that address
                  {
                    Items = 15;                                                                  //The item counter counts it as the fifteenth item
                    Serial.print("Checksum: ");                                                  //Print DGPS Age on the serial port after the last printed value
                    Serial.println(FifteenthItem);                                               //as it is the fifteenth item on the GPS string
                  }
                  PrintingTime();                                                                //Calling this funtion inside the current function and it prints the time below
                                                                                                 //the parsed strings
                  PrintingLatitude();                                                            //This function prints the latitude just below the time
                  PrintingLongitude();                                                           //This function prints the longitude just below the time
                  PrintingFloatTypeAltitude();                                                   //This function prints altitude as a floating type value on the serial port
                  PrintingCheckSum();                                                            //This funcion prints the checksum as an integer after converting it from a hexadecimal
                                                                                                 //string
                }
              }
            }
          }
        }
      }
    }
  }
}

// FUNCTION      : PrintingTime()
// DESCRIPTION   : This function converts the parsed string to an integer value and uses math functions to print different parts of time separately.It even uses strtok to print the 
//                 values of time(decimal degrees) after the decimal point.                
// PARAMETERS    : None
// RETURNS       : Nothing

void PrintingTime()
{
  uint32_t GpsTime = InitialValue;                                                                //Declaring Variables local to the function 
  uint32_t GpsTime2 = InitialValue;                                                               // and intialising them                                                                                
  uint32_t Hours = InitialValue ;                                                                 //to 0
  GpsTime = atoi(SecondItem);                                                                     //Converts the Second Item that is time to an integer type & storing it in a variable
  GpsTime2 = GpsTime;                                                                             //Copying that value in another variable ,this is done to perform math functions
  Serial.println(GpsTime);
  Serial.printf("Time:%d hours ", Hours = (GpsTime2 / 10000));                                    //This prints the hours part of time on the screen
  Serial.printf("%d mins ", (GpsTime - (10000 * Hours)) / 100);                                   //This Prints the minutes part of the string on the serial port
  Serial.printf("%d secs ", GpsTime % 100);                                                       //This prints the seconds part of time on the screen
  char *Secs = NULL ;                                                                             //Declaring two pointers that are going to be
  char *Hours1 = NULL;                                                                            //used for parsing the decimal degrees of the time string
  const char *Delim1 = "." ;                                                                      //Using Period as a deliminator for this string
  Hours1 = strtok(SecondItem, Delim1);                                                            //Using strtok and separating the part before the period
  if (Hours1 != NULL)                                                                             //This means there is something present on that address
  {
    Secs = strtok(NULL, Delim1);                                                                  //Starts reading the byte after the last NULL operator
    Serial.printf("%s Decimal Degrees \n", Secs);                                                 //Prints decimal degrees on the serial port                                    
  }
}

// FUNCTION      : PrintingLatitude()
// DESCRIPTION   : This function converts the parsed string to an integer value and uses math functions to print different parts of Latitude separately.It even uses strtok to print the 
//                 values of Latitude after the decimal point.                
// PARAMETERS    : None
// RETURNS       : Nothing

void PrintingLatitude()
{
  uint32_t GPSLatitude = InitialValue;                                                           //Declaring a local variable and initialising it to 0
  GPSLatitude = atoi(ThirdItem);                                                                 //Converting the Latitude String to an integer and storing it in a variable
  Serial.println(GPSLatitude );                                                                  //Printing the converted latitude on the screen
  Serial.printf("Latitude:%d Degrees ", GPSLatitude / 100);                                      //Using math function and printing the degree part of latitude on the screen
  Serial.printf("%d minutes ", GPSLatitude % 100);                                               //Using math functions and printing the minutes part og the latitude on serial port
  char *LatitudeParse = NULL;                                                                    //Declaring two pointers that are going to be
  char *LDecimalDegrees = NULL;                                                                  //used for parsing the decimal degrees of the Latitude string
  const char *Delim2 = ".";                                                                      //Using Period as the demiliminator
  LatitudeParse = strtok(ThirdItem, Delim2);
  if (LatitudeParse != NULL)                                                                     //This means there is something present in this particular address
  {
    LDecimalDegrees = strtok(NULL, Delim2);                                                      //Starts reading the byte after the last NULL operator
    Serial.printf("%s DecimalDegrees\n", LDecimalDegrees);                                       //Prints Decimal Degrees on the Serial Port
  }
}
// FUNCTION      : PrintingLongitude()
// DESCRIPTION   : This function converts the parsed string to an integer value and uses math functions to print different parts of Longitude separately.It even uses strtok to print the 
//                 values of Logitude after the decimal point.                
// PARAMETERS    : None
// RETURNS       : Nothing


void PrintingLongitude()
{
  uint32_t GPSLongitude = InitialValue ;                                                       //Declaring a local variable and initialising it to 0
  GPSLongitude = atoi(FifthItem);                                                              //Converting the Longitude String to an integer and storing it in a variable
  Serial.println(GPSLongitude );                                                               //Printing the converted longitude on the screen
  Serial.printf("Longitude:%d Degrees ", GPSLongitude / 100);                                  //Using math function and printing the degree part of longitude on the screen
  Serial.printf("%d minutes ", GPSLongitude % 100);                                            //Using math functions and printing the minutes part of the longitude on serial port
  char *LongitudeParse = NULL;                                                                 //Declaring two pointers that are going to be
  char *LongitudeDecimalDegrees = NULL;                                                        //used for parsing the decimal degrees of the Longitude string
  const char *Delim3 = ".";                                                                    //Using Period as the demiliminator
  LongitudeParse = strtok(FifthItem, Delim3);                                                  
  if (LongitudeParse != NULL)                                                                  //This means there is something present in this particular address
  {
    LongitudeDecimalDegrees = strtok(NULL, Delim3);                                            //Starts reading the byte after the last NULL opera
    Serial.printf("%s DecimalDegrees\n", LongitudeDecimalDegrees);                             //Prints Decimal Degrees on the Serial Port
  }
}

// FUNCTION      : PrintingFloatTypeAltitude()
// DESCRIPTION   : This function converts the parsed string to a floating type value and prints converted altitude string on the serial port as a floating value                 
// PARAMETERS    : None
// RETURNS       : Nothing

void PrintingFloatTypeAltitude()
{
  float Altitude = InitialValue;                                                              //Declaring a variable Altitude and initialising it to 0
  Altitude = atof(TenthItem);                                                                 //Converting the Tenth Term i.e altitude to a floating type value
  Serial.printf("Altitude : %f meters \n", Altitude);                                         //Printing the floating type value on the serial port
}

// FUNCTION      : PrintingCheckSum()
// DESCRIPTION   : This function converts the hexadecimal parsed string to an integer data type. There is a * sign present in the begining of the string, there for we pointed the
//                 pointer that has to be converted to the second byte of the string and then use strtoul to convert it to integer and eventually printing it on the serial port.                
// PARAMETERS    : None
// RETURNS       : Nothing

void PrintingCheckSum()
{
  uint32_t CheckSum = InitialValue ;                                                         //Declaring the variable and initialising it to 0
  CheckSum = strtoul(FifteenthItem + 1, NULL, 16);                                           //Reading the string after the * sign and converting it to an integer using strtoul()
  Serial.printf("CheckSum : %d \n", CheckSum);                                               //Printing the Converted value on the serial port
}

void setup()
{
  Serial.begin( SerialBaudRate);                                                             // Opens serial port, sets data rate to 9600 bps
  delay(SerialPortDelay);                                                                    //Assuming the Time taken to initialise the serial port is 2 seconds
}

void loop()
{
  CheckForRecvdCharAndParseGPS();                                                           //Calling this function prints the required fields on the serial port whenever we get a GPS
                                                                                            //as an input. 
}
