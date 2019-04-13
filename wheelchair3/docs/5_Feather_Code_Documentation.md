## Adafruit Feather 32u4

**Code Name:** Read_Bluetooth_and_Actuate.ino

The code runs on the Feather 32u4, an Arduino-like, Bluetooth enabled board by AdaFruit. Have a look at the documentation we compiled to get an overview of its capabilities and set up your Arduino IDE: [Feather 32u4](https://datacentricdesign.github.io/wheelchair-design-platform/resources/feather.html)

Feather 32u4 has a Bluetooth Low Energy (BLE) communication capability. To communicate over BLE, we choose the GATT specification (standing for Generic ATTribute Profile). It is implemented by most Bluetooth consumer product and will enable our prototype to easily fit in its technology landscape.

> With this code the Feather will get the order from the Pi to indicate the next Yoga posture and use a Neopixel 16-Leds ring to output the corresponding posture. It also has 3 vibrator motors that start breathing when the first signal from the bluetooth is detected.

### Setting up the GATT Bluetooth Conexion
```
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
```
### Add GATT Service and GATT characteristics

We setup a GATT Service with the following command:
```
    ble.sendCommandCheckOK( F("AT+GATTADDSERVICE=UUID=0x1234") );
```
UUID means that it is a 16-bit Service, reserved for officially supported services. For custom Services we should use a 32-bit characteristic, defined by UUID128, however, in this case, it also worked with a 16-bit service.

A service can contain many characteristics, each of them encapsulating a single data point. In our project we will use only one characteristic, defined as follows:
```
    ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=02-11-88-33-44-55-66-77-88-99-AA-BB-CC-DD-EE-FF,PROPERTIES=0x08,MIN_LEN=1,MAX_LEN=6,DATATYPE=string,DESCRIPTION=string,VALUE=abc"), &charid_posture);
```
This is a custom characteristic, so that is why it is defined as UUID128 and has 32 characters.

PROPERTIES=0x08 means is a write characteristic, permitting writes of the characteristic value with a response.

MIN_LEN and MAX_LEN are the min and max number of bytes to be sent. A character in encoded on 1 byte. We want to send data as a string of character such as -0.00,-0.00,-.00 which represents 17 characters.

VALUE is the default value sent when subscribing, in our case an empty string.

    ble.reset();

    // Disable command echo from Bluefruit
    ble.echo(false);

    // Print Bluefruit information
    ble.info();

### Callback API

This code uses the Bluefruit callback API.

In the setup() these functions have to be declared to install callback function for a specific event:
```
    void setup(void) {

      ble.setConnectCallback(connected);
      ble.setDisconnectCallback(disconnected);

      // Only one BLE GATT function should be set, it is possible to set it
      // multiple times for multiple Chars ID
      ble.setBleGattRxCallback(charid_posture, BleGattRX);
    }
```
Furthermore, update() must be called inside loop() for callback to be executed:
```
    void loop(void) {

      // Check every 200ms for new command receive from Bluetooth
      ble.update(200);
    }
```
Then the function BleGattRX has to be defined:
```
    void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len) {

      Serial.print( F("[BLE GATT RX] (" ) );
      Serial.print(chars_id);
      Serial.print(") ");

      if (chars_id == charid_posture) {

        Serial.println("Data received on posture characteristic: ");
        Serial.write(data, len);

        currentPosture = (int)data[0];  
      };
    }
```
### FastLed Functions

We used FastLed library to control the 16-led Neopixel ring. To initialize the leds we use the following commands:
```
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

    void setup(void) {
      FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
      // set master brightness control
      FastLED.setBrightness(BRIGHTNESS);
      currentBlending = LINEARBLEND;

      //Turn of all the leds at the start just in case
      fadeToBlackBy( leds, NUM_LEDS, 255);
      FastLED.show();
    }
```
### Color Pallettes

A color palette is effectively an array of sixteen CRGB colors which fade through each other making in total 256 possible unique color points.

You can manually define your own color palettes as we did with these piece of code:
```
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
```
The function ColorFromPalette() defines the color of the led[i] to the corresponding colorIndex of the currentPalette.
```
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
```
The colorIndex of a palette is a single byte integer that ranges as mention before between 0 and 255. When its value is a multiple of 16 it is representing one of the colors defined in the 4x4 CRGBPalette16 matrix. In the palette that we have defined previously the colorIndex 0 represents pure-green, 16 pure-blue, and 8 an even mixture between green and blue. ColorIndex 32 is pure-black, as well as colorIndex 48.

To make sure that the variable colorIndex loops between the values 0 to 255, in this code it has been defined as a uint8_t, a variable that can only store 1 byte.

    //Color Index increases till 256 and the restarts
      static uint8_t colorIndex = 0;
      colorIndex = colorIndex + 1;

### FastLed Show

To be able to visualize the changes of color on the led string, it is necessary to use the function FastLED.show(). In our sketch, we use it on the loop() function followed by FastLED.delay(), which sets the speed of the color visualization. FastLED.delay() is not a regular delay, and won't stop the code from running, it will only delay the FastLED.show() function.
```
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
```
### Adjust the LED ring depending on the posture received through the GATT characteristic

We will use a switch loop with the variable 'posture', turning on only the leds required for each of the postures.

Before the switch loop we turn of all the other leds, that dont correspond to the current posture with:
```
    fadeToBlackBy( leds, NUM_LEDS, 255);
```
### Setting the vibration rhythm

We wanted to control the vibration motors in a way that they resemble the breathing of a person. For that, we tweaked the vibrationValue through trial and error until we get to a desirable state.

The function setVibration() is the responsible for this task. It uses the colorIndex  (an 8-bit integer that loops from 0 to 255) as the base value that will be constrained to achieve the desired vibrationValue.

It is a quite complex way to make it because it the idea has been taken from an earlier version of the code, were the leds and vibration motors were synchronized in a breathing pattern. The idea here is to make two vibration cycles inside each palette cycle: So when the colorIndex is between 0 and 127 is one vibration cycle and when the colorIndex is between 128 and 255 is the second vibration cycle. Besides, when the value of the vibration is lower than 128 the vibration is almost unnoticeable so the vibrationValue has been constrained between 128 and 254.
```
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
```
