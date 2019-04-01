/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

//#include <Adafruit_NeoPixel.h> 

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

//*********************************************************************
//             FAST_LED INICIALIZATION
//*********************************************************************
#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    16
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

uint8_t brightness = 20;

#define UPDATES_PER_SECOND 50


//*********************************************************************
//             VIBRATION MOTORS INICIALIZATION
//*********************************************************************

#define VIB_PIN 11

uint8_t vibrationValue = 0;


//*********************************************************************
//             BLE INICIALIZATION
//*********************************************************************
                            
#define FACTORYRESET_ENABLE        1
// Minimum firmware version to have some new features
#define MINIMUM_FIRMWARE_VERSION   "0.7.0"

// Create the bluefruit object, either software serial
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// Writable GATT Service
int32_t charid_posture;
int32_t charid_button;


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// What to do when a Bluetooth client connect to the Feather
void connected(void) {
  Serial.println( F("Connected") );
}

// What to do when a Bluetooth client disconnect from the Feather
void disconnected(void) {
  Serial.println( F("Disconnected") );
}


//***********************************************************************************
//             DECODES GATT CHARACTERISTICS INTO currentPosture and buttonState
//***********************************************************************************

int currentPosture = 0;
int prevPosture = 0;
int buttonState = 0;

void BleGattRX(int32_t chars_id, uint8_t data, uint16_t len) {
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");


  if (chars_id == charid_posture) {
    //Todos estos commandos Serial solo sirven para escribir en el terminal lo que le llega por bluetooth a la feather
    Serial.print("Data received on posture characteristic: ");
    //el ",DEC" sirve para definir como quiero que me escriba en el terminal el valor de data[x], en este caso como un numero decimal (DEC)
    Serial.println(data, DEC);
    currentPosture = (int)data;
  };
  
  if (chars_id == charid_button){
    Serial.print("Data received on button characteristic: ");
    Serial.println(data, DEC);
    buttonState = int(data);
  };
}


//*********************************************************************
//             SET UP
//*********************************************************************

void setup(void) {
  
  pinMode(VIB_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
  
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentBlending = LINEARBLEND;

  off();

  delay(500);

  Serial.begin(115200);

  if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  if ( FACTORYRESET_ENABLE ) {
    // Perform a factory reset to make sure everything is in a known state
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }
  
  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    error( F("Callback requires at least 0.7.0") );
  }

  ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=uuid=0x1234") );
  
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=00-00-23-45-00-00-10-00-80-00-00-80-5f-9b-34-fb,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_button);
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID=02-11-88-33-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_posture);
  //PROPERTIES=0x08 is the Bit-field of the property and indicates how to interact with this property, 
  //the 0x08 value defines the property as write, permiting writes of the characteristic value with response

  ble.reset();

  // Disable command echo from Bluefruit
  ble.echo(false);

  // Print Bluefruit information
  ble.info();
  
  /* Set callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  
  // Only one BLE GATT function should be set, it is possible to set it
  // multiple times for multiple Chars ID
  ble.setBleGattRxCallback(charid_posture, BleGattRX);

  ble.setBleGattRxCallback(charid_button, BleGattRX);

  
}

//*********************************************************************
//            LOOP
//*********************************************************************

void loop(void) {
   
  // Check every 200ms for new command receive from Bluetooth
  ble.update(200);

  if (buttonState){
    respiration();
  }else{
    off();
  };

  
  switch (currentPosture){
      case 0://start audio
        showImage();
        break;
      case 1://start audio
        showImage();
        break;
      case 2://start audio
        showImage();
        break;
      case 3://start audio
        showImage();
        break;
      case 4://start audio
        showImage();
        break;
      case 5://start audio
        showImage();
        break;
      default:
        Serial.println("default");
        off();
        break;
    }
}

//*********************************************************************
//             LED CONTROL FUNCTIONS
//*********************************************************************

void off(){
   
   analogWrite(VIB_PIN, 0);

   fadeToBlackBy( leds, NUM_LEDS, 255);
   Serial.println("off() function triggered");
   FastLED.show();
}


void showImage(string image1, string image2, string image3){
  
}

void respiration(){
    
    SetupBreadingPallette();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

    EVERY_N_MILLISECONDS(1000 / UPDATES_PER_SECOND){
       analogWrite(VIB_PIN, vibrationValue);       
    };
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    

    if(colorIndex>126){
        if (colorIndex<189){
          vibrationValue=colorIndex*2+80;
        }else{
          vibrationValue=332-colorIndex*2;
        };      
    }else{
        if (colorIndex<63){
          vibrationValue=colorIndex*2+80;
        }else{
          vibrationValue=332-colorIndex*2;
        };
    }
      brightness = constrain(colorIndex,0,250);
      brightness = map(brightness,0,250,0,50);

    if (colorIndex<126){
      for( int i = 0; i < NUM_LEDS/2; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        int j = NUM_LEDS - (1+i);
        leds[j] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 1;
      }
      
    }else if(colorIndex >= 126 && colorIndex <256){
      for( int i = NUM_LEDS/2; i >=0; i--) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        int j = NUM_LEDS - (1+i);
        leds[j] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 1;
      };
      
    };
}


void SetupBreadingPallette()
{
    CRGB blue = CHSV( 130, 255, 255);
    CRGB green  = CHSV( 110, 255, 255);
    CRGB softBlue = CHSV(130,100,10);
    CRGB softGreen= CHSV(110,100,10);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   black,  black, black,  black,
                                   black,  black, green,  blue,
                                   green,  blue, green,  blue,
                                   green,  blue, green,  blue);
}


