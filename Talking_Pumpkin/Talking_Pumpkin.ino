/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

// create shield-example object!
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

const int trigPin = 10; 
const int echoPin = 9;

// Distance at which sounds trigger
const float trigDistance = 60.0f;

float duration, distance;

//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
const int calibrationTime = 5; 

// A list of the sounds available on the SD card
char const* sounds[]={"track001.mp3","track002.mp3","track003.mp3","track004.mp3","track005.mp3","track006.mp3","track007.mp3","track008.mp3","track009.mp3","track010.mp3","track011.mp3","track012.mp3","track013.mp3","track014.mp3","track015.mp3",
"track016.mp3","track017.mp3","track018.mp3","track019.mp3","track020.mp3","track021.mp3","track022.mp3","track023.mp3","track024.mp3","track025.mp3","track026.mp3","track027.mp3","track028.mp3","track029.mp3","track030.mp3","track031.mp3"};

const int numSounds = 31;

void UpdateDistance()
{
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 

  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; // 29.1 is the speed of sound.
}
  
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
   pinMode(trigPin, OUTPUT); 
   pinMode(echoPin, INPUT);

   //give the sensor some time to calibrate*****************************************************
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++)
  {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");

  UpdateDistance();

  if (distance != 0)
  {
    delay(300);
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

  //*******************************************************************************************

  Serial.println("Adafruit VS1053 Simple Test");

  if (!musicPlayer.begin()) // initialise the music player
  {
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS))
  {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10,10);

  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void loop()
{
  UpdateDistance();

 Serial.print("Distance: "); 
 Serial.println(distance); 

  if ( distance < trigDistance )
  {
      long randomFileIndex = random(numSounds);  //set random file value anything between 1 and numSounds (defined above)

      // Play one file, don't return until complete
      Serial.println(F("Playing track"));
      musicPlayer.playFullFile(sounds[randomFileIndex]);
  }

  delay(100);
}
