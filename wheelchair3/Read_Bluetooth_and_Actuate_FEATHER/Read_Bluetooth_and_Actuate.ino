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
#define BRIGHTNESS  200
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#define UPDATES_PER_SECOND 50


//*********************************************************************
//             VIBRATION MOTORS INICIALIZATION
//*********************************************************************

#define VIB_PIN_1 9
#define VIB_PIN_2 10
#define VIB_PIN_3 11

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

void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len) {
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");


  if (chars_id == charid_posture) {
    //Todos estos commandos Serial solo sirven para escribir en el terminal lo que le llega por bluetooth a la feather
    Serial.println("Data received on posture characteristic: ");
    //el ",DEC" sirve para definir como quiero que me escriba en el terminal el valor de data[x], en este caso como un numero decimal (DEC)
    Serial.write(data, len);
//    Serial.println(data, DEC);
    currentPosture = (int)data[0];
    Serial.print("CurrentPosture = ");
    print_Posture_Name(currentPosture);
  };
}

void print_Posture_Name(int postureNumber){
  switch (postureNumber){
    case 0: 
       Serial.println("Not Sitting");
       buttonState = 0;
       break;
    case 1:
       Serial.println("Proper Sitting");
       buttonState = 1;
       break;
    case 2:
       Serial.println("Leg Stretch Right");
       break;
    case 3:
       Serial.println("Leg Stretch Left");
       break;
    case 4:
       Serial.println("Twist Right");
       break;
    case 5:
       Serial.println("Twist Left");
       break;
    case 6:
       Serial.println("Forward Bend");
       break;
    default:
       Serial.println("unknown posture number");
       break;
  };
}
//*********************************************************************
//             SET UP
//*********************************************************************

void setup(void) {
  
  pinMode(VIB_PIN_1, OUTPUT);
  pinMode(VIB_PIN_2, OUTPUT);
  pinMode(VIB_PIN_3, OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
  
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentBlending = LINEARBLEND;

  //Turn of all the leds at the start just in case
  fadeToBlackBy( leds, NUM_LEDS, 255);
  FastLED.show();

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
  
  ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=02-11-88-33-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_posture);
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
}

//*********************************************************************
//            LOOP
//*********************************************************************
long timeCounter = 0;
int interval = 1000;

void loop(void) {
   
  // Check every 200ms for new command receive from Bluetooth
  ble.update(200);

  blinkLeds(currentPosture);
}

//*********************************************************************
//             LED CONTROL FUNCTIONS
//*********************************************************************


void SetupBlinkingGreenAndBluePalette()
{
    CRGB green = CHSV( 135, 255, 255);
    CRGB blue  = CHSV( 180, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  blue,  black,  black,
                                   blue, green, black,  black,
                                   green,  blue,  black,  black,
                                   blue, green, black,  black );
                                   
}
                       
void blinkLeds(int posture){
  
  SetupBlinkingGreenAndBluePalette();

  //Color Index increases till 256 and the restarts
  static uint8_t colorIndex = 0;
  colorIndex = colorIndex + 1; /* motion speed */

  //*****************************************************************************************
  //VIBRATION ONLY ACTIVATED IF I PRESS THE BUTTON IN THE MEGA
  //Serial.print("Button State =  ");
  //Serial.println(buttonState);
  
  if (buttonState == 1){   /*Set up vibration value*/
    setVibration(colorIndex);
  }else{
    vibrationValue = 0;
    analogWrite(VIB_PIN_1, vibrationValue); 
    analogWrite(VIB_PIN_2, vibrationValue); 
    analogWrite(VIB_PIN_3, vibrationValue); 
  };

  fadeToBlackBy( leds, NUM_LEDS, 255);
  Serial.println(posture);
  switch (posture){
    case 1: 
       leds[13] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[12] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    case 2: 
       leds[11] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[10] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    case 3: 
       leds[9] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[8] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[7] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    case 4: 
       leds[6] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[5] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[4] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    case 5: 
       leds[3] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[2] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[1] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    case 6: 
       leds[0] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       leds[15] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
       break;
    default: 
       fadeToBlackBy( leds, NUM_LEDS, 255);
       break;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}


void setVibration( uint8_t colorIndex)
{  
    if(colorIndex>128){
        if (colorIndex<192){
          vibrationValue=colorIndex*2-128; //from 128 to 254
        }else{
          vibrationValue=638-colorIndex*2; //from 254 to 128
        };      
    }else{
        if (colorIndex<64){
          vibrationValue=colorIndex*2+128; //from 128 to 254
        }else{
          vibrationValue=382-colorIndex*2; //from 254 to 128
        };
    }

    analogWrite(VIB_PIN_1, vibrationValue); 
    analogWrite(VIB_PIN_2, vibrationValue); 
    analogWrite(VIB_PIN_3, vibrationValue); 
 
}



