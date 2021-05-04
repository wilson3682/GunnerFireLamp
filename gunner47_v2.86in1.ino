//
// ======================= ATTENTION !!! =============================
// All settings are made on the Constants.h tab
// Read what is written in Russian there.
// Or don't touch anything if you collected it, as in the original video.
//
//  the solution to the problems can be found here under the spoilers:
//  https://community.alexgyver.ru/goto/post?id=33652
// ==================================================================

// Link for PCB manager:
// https://arduino.esp8266.com/stable/package_esp8266com_index.json

/*
  Sketch for the project "Multifunctional RGB lamp"
   Project page (diagrams, descriptions): https://alexgyver.ru/GyverLamp/
   Sources of the author's (old) firmware on GitHub: https://github.com/AlexGyver/GyverLamp/
   sources of the new version: https://community.alexgyver.ru/goto/post?id=33652
   Do you like the lamp? Support the author! https://alexgyver.ru/support_alex/
   Author of the idea and the first implementation: AlexGyver, AlexGyver Technologies, 2019
   https://AlexGyver.ru/
*/

/*
  Small changes between versions
  - If you hold down the button on the off lamp, the "White light" mode will turn on immediately.
  - If the "White light" mode is turned on (no matter what method) and this mode is not added to the list for automatic switching of effects, then it will not switch itself.
  - You can make the button switch only those modes that are added to the favorites for the Loop mode. Setting #define BUTTON_CHANGE_FAVORITES_MODES_ONLY
  - The lamp will try to connect to the router for 60 seconds, not 7, as it used to be. This is so that the lamp can wait for the router to load if the electricity in the house goes out. Setting #define ESP_CONN_TIMEOUT (60U)
  - The lamp will check for the appearance of the Internet on the router every 15 seconds, and not once every 5 minutes, as it used to be. This is to quickly synchronize the time in the lamp with the NTP server. Setting #define RESOLVE_INTERVAL (15UL * 1000UL)
  - The lamp will forget the password to the router if it was switched to "without a router" mode by pressing the button or after 60 seconds of waiting. This is to prevent the application from losing contact with the lamp in the "no router" mode. Setting #define RESET_WIFI_ON_ESP_MODE_CHANGE
  - The firmware cannot be loaded if the current libraries from the archive with the firmware are not placed in the library folder.
  - The lamp will not respond to "phantom presses" of the touch button if you forgot to plug it in before plugging the lamp into the socket. Setting #define BUTTON_LOCK_ON_START
  - Removed in a separate setting the lamp signal "about a problem with time" during the operation of effects. By default, the lamp will not blink two LEDs while effects are running.

  Version 1.5.85 effects in 1
  - Added effects Fairy, Source.
  - Removed the effect of Balls with a train (it was just a copy of the Balls for demonstration on other settings of the Speed ​​slider).
  - Added the ability to automatically synchronize the lamp time with a smartphone. The application must support this function.
  - Added indication that the lamp does not know the time (if 2 colored dots run in a circle along the bottom of the lamp, it means that it does not know).
  - Fixed a bug in the FastLed library in the implementation of the blur2d function. It is recommended to put the libraries from the archive with the firmware into the libraries folder of the Arduino IDE program.
  - Removed the prohibition to search for the lamp's ip-address when the lamp works as an access point without a router (in ESP_MODE = 0).
  - Added the ability to pass the name of the lamp to the application (in case you have several lamps in your home network). The application must support this function.

  Version 1.5.84 effects in 1
  - Added effects Boiling, Attraction, Drops on glass, Smoke bombs, Pacific Ocean, Nexus.
  - Removed the effects of White fire, Colored fire, White waterfall, Fast pulse, Pulsating coma (copies and similar effects are no longer interesting even in the Cycle mode, and there are a lot of effects).
  - Removed the prohibition to update firmware "over the air" when the lamp works as an access point without a router (in ESP_MODE = 0).
  - Fixes in DNA effects (added control of the Scale slider), Balls without borders, Whirlwinds, Multicolored whirlwinds, Flock, Flock and predator (anti-aliasing).

  Version 1.5.83 Effects in 1
  - Added effect Fire 2020.
  - The effect of the Pulsing Comet is returned.
  - The problems "the predator died", "the time is twitching with a creeping line" are eliminated.
  - Removed lamp attenuation when changing effects. Removed preliminary clearing of the previous effect image, where possible.
  - Added "secret commands" for setting the Dawn Alarm and Shutdown Timer from the Blynk application (suddenly someone really needs it).

  Version 1.5.81 effect in 1
  - Removed the White Comet effect.
  - Temporarily removed the Pulsing Comet effect.
  - Temporarily? added the Oscillator effect.
  - Updated support for the Blynk application for managing crawl text and transmitting "secret commands".
  - Smoke effects are better suited for lamps with poor diffuser.
  - Found and removed the reason that in Cycle mode the lamp was completely extinguished between effects.

  Version 1.5.82 effects in 1
  - Removed the Fireworks effect.
  - Added Liquid Lamp and Popcorn effects.
  - Implemented the ability to set the time on the lamp without an Internet connection (via the Android application from @Koteyka - it is in the archive with the firmware).
  - Added support for secret commands reset = wifi, reset = effects, esp_mode = .. (via the Android application from @Koteyka - it is in the archive with the firmware).

  Version 1.5.80 effects in 1
  - Added the ability to select saturation with the Speed ​​slider in the Color effect.
  - Added effects of Shadows, Moths, Lamp with moths, DNA, Snakes, Fireworks.
  - Added support for Blynk app (for iOS and Android) for lamps with constant Internet access. If you turn it on, everything will slow down a little.

  Version 1.5.74 effects in 1
  - Added Lava Lamp effect.

  Version 1.5.73 Effects in 1
  - Added Picasso effects.
  - Added the Jumpers effect.

  Version 1.5.69 effects in 1 proper
  - In the White light effect, a smoother change in the glow area was made with the Scale slider (maybe in vain).
  - In the Metabolz effect, the problem of restarting the lamp during prolonged operation has been finally fixed.

  Version 1.5.69 effects in 1
  - Added effects Smoke and Multicolored smoke.

  Version 1.5.67 effects in 1
  - Added a separate Clock mode. Just like a normal penultimate effect. "Speed" adjusts the location of the numbers. The better the diffusion on the lamp, the more useless this mode is.
    The minimum matrix height required for the Watch is 11 pixels. And 7 pixels wide should be visible on one side of the lamp, otherwise part of the dial will slide to the side.
  - Fixed bugs in the Comet effects, causing the lamp to reload when the height and width of the matrix differ from each other, and when the matrix is ​​less than 14 pixels high.

  Version 1.5.66 effects in 1
  - All complex settings in Constants.h have been moved down so as not to distract.
  - Added the effects of Balls Without Borders (Jumping), Combination Lock and Rubik's Cube. The effect of White balls is hidden (you can choose white as the usual color).
  - Added the ability to set the lamp to turn off automatically after several hours of continuous operation (in case someone forgot to turn it off).
    In the application, the shutdown time will not be displayed correctly until it is fixed in a future version.

  Version 1.5.64 effects in 1
  - Added the Flicker effect.
  - The Color Change effect is now slower. On scales> 10, the color switching will be after the selected number of seconds (1 second for Speed ​​= 255/2 seconds for Speed ​​= 254, etc.)
  - Added the ability to select a palette with the Scale slider in the Metabolz effect
  - Updated Blizzard and Starfall effects to @Palpalych's version.
  - Added the ability to enable the "queue of impressions" of the selected effects for the Loop mode (so that there are no repetitions too often). This needs to be included in Constants.h.
  - Added the ability to turn off the function "continue demonstrating the last effect after the lamp is de-energized". This must be done in Constants.h.
  - Fixed the dimension of the currentMode variable. Perhaps now the maximum number of effects can be more than 127. But it is not exactly.

  Version 1.5.63 Effects in 1
  - Added effects Cloud in the bank, Thunderstorm in the bank, Precipitation, Fire 2012.
  - Added a hue adjustment for the Fire 2018 effect.
  - Added parameter to Constants.h to support more recent version of the application from @Koteyka.
  - Optimized parsing.h functions
  - Fixed XY () function. It was an analogue of getPixelNumber (), but for some reason it was crookedly written.

  Version 1.5.60 effects in 1
  - Added effect Fire 2018.
  - Added a Scale adjustment for the effects of 2 comets, 3 comets, Soaring fire, Horse fire.
  - It is possible that the compatibility of the firmware with the matrices has been fixed, where the way of connecting the LEDs differs from the "snake".

  Version 1.5.59 effects in 1
  - Created an array of settings for all default effects. The settings were selected with a low brightness for working in the night light mode (I did it for myself). You can set the brightness higher.
  - Added resetting all effects settings to their default values ​​when prompted by the application ("load the list of effects from the lamp"). You can turn this off in Constants.h.
  - Added the effects of Flame Swirls and Multicolored Swirls.
  - In the effect of Fireflies with a train, you can now select colors for them using the Scale slider.

  Version 1.5.57 effects in 1
  - Combined the use of many variables in different effects to save a little memory.
  - The Color effect is also brought up in the effects with a forced delay of 50 ms, so as not to injure the board controller.
  - Added effects Flock, Flock and predator, Prismata, Waves.
  - Increased the size of the buffer for receiving favorites settings from the application to the lamp. Now the maximum number of effects is somewhere around 117.
    (but this is already the limit for the current implementation of the FavoritesManager.h functions. Then you will have to change the variable types there).

  Version 1.5.53 Effects in 1
  - Changed the way of sending the list of effects to the lamp. Now it is not limited by the size of the buffer, but is limited to xs than.
  - The list of effects has been expanded to 53 pieces.
  - Added new features supported by @Koteyka's app (drawing, creeping line).

  Version 1.5:
    the same list with hyperlinks: https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proshivki-ot-gunner47.2418/post-30883
  - Fixed a bug in the condition of the procedure for calling effects (for two effects the speed controller did not work)
  - Inverted Speed ​​control for all effects where it worked backwards
  - Removed references to their sequence numbers from all effects. Now any effect can be installed in any place (except numbers from 7 to 15), as well as make copies of effects
  - Reworked the allocation of flash memory EepromManager.h Now you can add new effects and make copies (copies - to demonstrate the same effect at different settings).
  - In the FastLED library, the coefficients for calculating the current consumption are corrected
  - The effect of Fire has been replaced by "Lamp fire", at the maximum Scale it will also be the effect of White fire
  - White fire effect replaced with Waterfall effect, at maximum Scale the water color will be white
  - Fixes for Paintball and Rainbow diagonal effects by @Palpalych (set Paintball to faster speed to make it look good)
  - Added the Pool effect to the Color effect (at maximum Speed, the glare of the water disappears, the Color effect works)
  - Added the Pulse effect to the Color Change effect (at the minimum Scale, the Color Change effect will work)
  - Fixes for Blizzard and Starfall effects by @Rampart
  - The Matrix effect has changed the color palette and the algorithm of work (Shameful Matrix)
  - The effect of the Firefly with a trail is replaced by the effect of the Comet (the old effect is left in the code, as well as the additional Comet mini, but they are not connected)
  - 2 applications are supplied with the firmware in the archive. Standard can only work with 26 effects. With an increase in the number of effects, only the application from @Koteyka will work
  - Added a file with instructions in the format of a book to the archive with the firmware. True, all the links there are already outdated.
  - Added a handler for the "GBR" command to change the brightness of all effects at once without saving to non-volatile memory. The application must support this function.
  - Added a handler for the "LIST" command to send the number and register of installed effects to the application. The application must support this function.
  Version 1.4:
  - Fixed bug when changing modes
  - Fixed brakes in hotspot mode
  --- 07/08/2019
  - Fixed parameters and process of connecting to a WiFi network (timeout 7 seconds) and deploying a WiFi access point (username / password parameters)
  - Added "#define USE_NTP" - allows you to prohibit access to the Internet
  - Added "#define ESP_USE_BUTTON - allows you to assemble a lamp without a physical button, otherwise the brightness of the effects spontaneously grows to maximum
  - Reworked IP address parameters, STA_STATIC_IP is now empty by default - eliminates confusion with IP addresses from wrong ranges
  - Added "#define GENERAL_DEBUG" - displays some debug messages in Serial / Telnet
  - Added "#define WIFIMAN_DEBUG (true)" - outputs debug messages of WiFiManager library to Serial / Telnet
  - Added table with test cases
  - Code formatting, comments
  --- 11.07.2019
  - Fixed a bug of not turning off the matrix after the alarm went off, if the matrix was turned off before the alarm
  - Added table with test cases
  --- 07/14/2019
  - Fixed a bug with turning off the alarm if the "matrix" effect was active before it went off (or another effect where few LEDs were used)
  - Added air control:
  - works only in WiFi client mode
  - works when the button is connected (because the firmware mode is activated by the button)
  --- 07/16/2019
  - Fixed the regular freezing of the matrix for 1-2 seconds in the absence of an Internet connection (but with a successful connection to WiFi)
  --- 28.07.2019
  - Improved interaction with android application (sending state after each operation)
  --- 01.08.2019
  - Return to the standard library GyverButton (change from it moved to button.ino
  - Added 2 effects: Fireflies with a trail and White light
  - When an update is requested over the air (2 quadruple touches to the button), the lamp switches to the "Matrix" mode for visual confirmation of readiness for firmware
  - Added the function of scanning the network and adding lamps using multicast packets to the android application, finalizing the firmware for this
  --- 03.08.2019
  - Fixed errors in the interaction of the android application with the lamp, the current time (or millis (), if the time is not synchronized) is added to the output of the CURR command
  --- 08/10/2019
  - Added fine tuning of brightness, speed and scale of effects
  - Added interaction with android application for managing alarms
  --- 14.08.2019
  - Added sleep timer function
  --- 26.08.2019
  - Added a mode for automatic switching of favorite effects
  - Reorganized code, fixed bugs
  --- 28.08.2019
  - Added a call to the esp module update mode from the android application
  --- 30.08.2019
  - Effect "Fireflies with trails" renamed to "Fading pixels"
  - Added 5 new effects: "Diagonal rainbow", "Blizzard", "Starfall", "Fireflies with trails" (new) and "Wandering cube"
  - Bugs fixed
  --- 04.09.2019
  - Most of the definitions (constants) have been moved to the Constants.h file
  - Big optimization of memory usage
  - Fixed a bug of not turning on the "White light" effect by the application and the button
  - Fixed bug with incorrect interval selection in Favorites mode in android application
  --- 16.09.2019
  - Added saving the state (on / off) of the lamp in the EEPROM memory
  - Added new white light effect (with horizontal stripe)
  - Reorganized code, fixed bugs
  --- 20.09.2019
  - Added the ability to save the state (on / off) of the "Favorites" mode; not cleared by turning off the matrix, not cleared by restarting the esp module
  - Removed cleaning of WiFi parameters when starting with a held down button; governed by the ESP_RESET_ON_START directive, which is set to false by default
  --- 24.09.2019
  - Added changes from the firmware from Alex Gyver v1.5: creeping line with the IP address of the lamp by a five-time click on the button
  --- 29.09.2019
  - Added option to display debug messages via telnet protocol instead of serial for remote debugging
  - Fixed a bug with adjusting the brightness with the button
  --- 05.10.2019
  - Added control via MQTT protocol
  - Fixed a bug with turning off the alarm clock with the button
  - Added a delay of 1 second immediately after the start, during which you need to press the button to clear the saved WiFi parameters (if ESP_RESET_ON_START == true)
  --- 12.10.2019
  - Added the ability to change the operating mode of the lamp (ESP_MODE) without the need for flashing; called by a sevenfold click on the button when the matrix is ​​on; saved to EEPROM
  - Changed the alarm clock algorithm:
  - * update of its hue / brightness occurs 1 time in 3 seconds instead of 1 time per minute
  - * diodes are divided into 6 groups, the first of which is assigned a new shade / brightness 1 time in 3 seconds, the second "lags behind" by 1 step, the third - by 2 steps, etc. (for smoother)
  - Added visual signaling about some important actions / states of the lamp:
  - * when starting in WiFi client mode and not yet configured WiFi network parameters (when they need to be entered) - 1 yellow flash
  - * if the lamp started in the WiFi client mode with unconfigured WiFi network parameters, and they were not entered within the allotted timeout (before rebooting) - 1 red flash
  - * when the lamp switches to the over-the-air (OTA) update mode by two four-time clicks on the button or on the OTA button from the android application - 2 flashes in yellow
  - * if the lamp was switched to OTA mode, but did not wait for the firmware in the allotted timeout (before rebooting) - 2 flashes in red
  - * when switching the working mode of the lamp WiFi access point / WiFi client by seven-time clicking on the button (before rebooting) - 3 flashes in red
  - * when requesting to display the time in creeping line, if time is not synchronized - 4 flashes in red
  - Reduced the timeout for connecting to a WiFi network to 6 seconds; caused by the increased duration of the setup () function, it must be less than 8 seconds in total
  - Optimized code
  --- 14.10.2019
  - If at the first start in the WiFi client mode, the requested WiFi network name and password are not entered within the allotted timeout (5 minutes), the lamp will restart in the access point mode
  - Added time display in creeping line:
  - * upon request - six times click - the current time in white;
  - * periodically - defined by the PRINT_TIME constant in Constants.h - from once per hour (in red) to once per minute (in blue) with the brightness of the current effect both with the matrix on and off
  --- 19.10.2019
  - Added "night hours" (from NIGHT_HOURS_START to NIGHT_HOURS_STOP inclusive) and "daytime hours" (all the rest of the time), for which brightness control is available for displaying the time as a running line - NIGHT_HOURS_BRIGHTNESS and DAY_HOURS_BRIGHTNESS
  --- 20.10.2019
  - Added blocking of the button on the lamp from the android application; saved in EEPROM memory
  --- 24.10.2019
  - Added signal output (HIGH / LOW - configurable by the constant MOSFET_LEVEL) synchronously with the switching on of the matrix to the pin of the MOSFET transistor (configurable by the constant MOSFET_PIN)
  - Added signal output (HIGH / LOW - configured by the ALARM_LEVEL constant) to the alarm pin (configured by the ALARM_PIN constant); the signal sounds for one minute, starting from the time at which the alarm was set
  --- 02.11.2019
  - Added summer / winter time transition (changed time zone settings, see Constants.h); added Timezone library
  - Added effect White fire
  - Fixed a bug with resetting the signal on the ALARM_PIN pin when you manually turn off the alarm
  - Added alarm (4 flashes in red) when requesting time output by six-click, if the time is not synchronized
  --- 04.11.2019
  - Fixed a bug of not turning on the MOSFET of the matrix when the "dawn" is triggered
  - Fixed a bug of not turning on the MOSFET matrix when displaying time and IP address
  --- 08.11.2019
  - Fixed static IP address assignment errors
  - Added WiFiManager Captive Portal sketch to enter custom options and settings
*/

// Ссылка для менеджера плат:
// https://arduino.esp8266.com/stable/package_esp8266com_index.json

#define FASTLED_USE_PROGMEM 1 // просим библиотеку FASTLED экономить память контроллера на свои палитры

#include "pgmspace.h"
#include "Constants.h"
#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "CaptivePortalManager.h"
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "Types.h"
#include "timerMinim.h"
#ifdef ESP_USE_BUTTON
#include "GyverButton.h"
#endif
#include "fonts.h"
#ifdef USE_NTP
#include <NTPClient.h>
#include <Timezone.h>
#endif
#include <TimeLib.h>
#ifdef OTA
#include "OtaManager.h"
#endif
#if USE_MQTT
#include "MqttManager.h"
#endif
#include "TimerManager.h"
#include "FavoritesManager.h"
#include "EepromManager.h"
#ifdef USE_BLYNK
#include <BlynkSimpleEsp8266.h>
#endif


// --- ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ ----------
CRGB leds[NUM_LEDS];
WiFiManager wifiManager;
WiFiServer wifiServer(ESP_HTTP_PORT);
WiFiUDP Udp;

#ifdef USE_NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, 0, NTP_INTERVAL); // an object requesting time from the ntp server; it does not use the time zone offset (moved to the localTimeZone object); there should always be UTC time here
#ifdef SUMMER_WINTER_TIME
TimeChangeRule summerTime = { SUMMER_TIMEZONE_NAME, SUMMER_WEEK_NUM, SUMMER_WEEKDAY, SUMMER_MONTH, SUMMER_HOUR, SUMMER_OFFSET };
TimeChangeRule winterTime = { WINTER_TIMEZONE_NAME, WINTER_WEEK_NUM, WINTER_WEEKDAY, WINTER_MONTH, WINTER_HOUR, WINTER_OFFSET };
Timezone localTimeZone(summerTime, winterTime);
#else
TimeChangeRule localTime = { LOCAL_TIMEZONE_NAME, LOCAL_WEEK_NUM, LOCAL_WEEKDAY, LOCAL_MONTH, LOCAL_HOUR, LOCAL_OFFSET };
Timezone localTimeZone(localTime);
#endif
#ifdef PHONE_N_MANUAL_TIME_PRIORITY
bool stillUseNTP = true;
#endif
#endif

timerMinim timeTimer(3000);
bool ntpServerAddressResolved = false;
bool timeSynched = false;
uint32_t lastTimePrinted = 0U;

#if defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
time_t manualTimeShift;
#endif

#ifdef GET_TIME_FROM_PHONE
time_t phoneTimeLastSync;
#endif

#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
uint8_t selectedSettings = 0U;
#endif //#if defined(USE_RANDOM_SETS_IN_APP) || defined(RANDOM_SETTINGS_IN_CYCLE_MODE)
#ifdef RANDOM_SETTINGS_IN_CYCLE_MODE
uint8_t random_on = RANDOM_SETTINGS_IN_CYCLE_MODE;
#endif //RANDOM_SETTINGS_IN_CYCLE_MODE

#ifdef ESP_USE_BUTTON
#if (BUTTON_IS_SENSORY == 1)
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  //for touch button LOW_PULL
#else
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN); //for a physical (non-touch) button HIGH_PULL. well, the button must be placed without a resistor in the gap between pins D2 and GND
#endif
#endif

#ifdef OTA
OtaManager otaManager(&showWarning);
OtaPhase OtaManager::OtaFlag = OtaPhase::None;
#endif

#if USE_MQTT
AsyncMqttClient* mqttClient = NULL;
AsyncMqttClient* MqttManager::mqttClient = NULL;
char* MqttManager::mqttServer = NULL;
char* MqttManager::mqttUser = NULL;
char* MqttManager::mqttPassword = NULL;
char* MqttManager::clientId = NULL;
char* MqttManager::lampInputBuffer = NULL;
char* MqttManager::topicInput = NULL;
char* MqttManager::topicOutput = NULL;
bool MqttManager::needToPublish = false;
char MqttManager::mqttBuffer[] = {};
uint32_t MqttManager::mqttLastConnectingAttempt = 0;
SendCurrentDelegate MqttManager::sendCurrentDelegate = NULL;
#endif

// --- INITIALIZING VARIABLES -------
uint16_t localPort = ESP_UDP_PORT;
char packetBuffer[MAX_UDP_BUFFER_SIZE];                     //buffer to hold incoming packet
char inputBuffer[MAX_UDP_BUFFER_SIZE];
static const uint8_t maxDim = max(WIDTH, HEIGHT);

ModeType modes[MODE_AMOUNT];
AlarmType alarms[7];

static const uint8_t dawnOffsets[] PROGMEM = {5, 10, 15, 20, 25, 30, 40, 50, 60};   //options for the drop-down list of the "time before dawn" parameter (alarm clock); synchronized with android app
uint8_t dawnMode;
bool dawnFlag = false;
uint32_t thisTime;
bool manualOff = false;

uint8_t currentMode = 0;
bool loadingFlag = true;
bool ONflag = false;
uint32_t eepromTimeout;
bool settChanged = false;
bool buttonEnabled = true; //this is an important initial value. you cannot do false by default

unsigned char matrixValue[8][16]; //this is an array for the Fire effect

bool TimerManager::TimerRunning = false;
bool TimerManager::TimerHasFired = false;
uint8_t TimerManager::TimerOption = 1U;
uint64_t TimerManager::TimeToFire = 0ULL;

uint8_t FavoritesManager::FavoritesRunning = 0;
uint16_t FavoritesManager::Interval = DEFAULT_FAVORITES_INTERVAL;
uint16_t FavoritesManager::Dispersion = DEFAULT_FAVORITES_DISPERSION;
uint8_t FavoritesManager::UseSavedFavoritesRunning = 0;
uint8_t FavoritesManager::FavoriteModes[MODE_AMOUNT] = {0};
uint32_t FavoritesManager::nextModeAt = 0UL;

bool CaptivePortalManager::captivePortalCalled = false;

char* TextTicker;
int Painting = 0; CRGB DriwingColor = CRGB(255, 255, 255);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  ESP.wdtEnable(WDTO_8S);


  // ПИНЫ
#ifdef MOSFET_PIN                                         // initialize the pin that drives the MOSFET transistor to the "off" state
  pinMode(MOSFET_PIN, OUTPUT);
#ifdef MOSFET_LEVEL
  digitalWrite(MOSFET_PIN, !MOSFET_LEVEL);
#endif
#endif

#ifdef ALARM_PIN                                          // initialization of the pin that controls the alarm clock to the "off" state
  pinMode(ALARM_PIN, OUTPUT);
#ifdef ALARM_LEVEL
  digitalWrite(ALARM_PIN, !ALARM_LEVEL);
#endif
#endif


  // TELNET
#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
  telnetServer.begin();
  for (uint8_t i = 0; i < 100; i++)                         // pause for 10 seconds in debug mode to have time to connect via telnet before the first messages are displayed
    handleTelnetClient();
    delay(100);
    ESP.wdtFeed();
  }
#endif


  // КНОПКА
#if defined(ESP_USE_BUTTON)
  touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
  touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
#if ESP_RESET_ON_START
  delay(1000);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
  if (digitalRead(BTN_PIN))
  {
    wifiManager.resetSettings();                          // сброс сохранённых SSID и пароля при старте с зажатой кнопкой, если разрешено
    LOG.println(F("Настройки WiFiManager сброшены"));
    //buttonEnabled = true;                                   // при сбросе параметров WiFi сразу после старта с зажатой кнопкой, также разблокируется кнопка, если была заблокирована раньше
    EepromManager::SaveButtonEnabled(&buttonEnabled);
  }
  ESP.wdtFeed();
#elif defined(BUTTON_LOCK_ON_START)
  delay(1000);                                            // ожидание инициализации модуля кнопки ttp223 (по спецификации 250мс)
  if (digitalRead(BTN_PIN))
    buttonEnabled = false;
  ESP.wdtFeed();
#endif
#endif


  // ЛЕНТА/МАТРИЦА
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection(TypicalLEDStrip)*/;
  //FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(0xFFB0F0); // по предложению @kostyamat добавлена такая цветокоррекция "теперь можно получить практически чистый желтый цвет" и получилось плохо
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0)
  {
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  }
  FastLED.clear();
  FastLED.show();

#ifdef USE_SHUFFLE_FAVORITES // первоначальная очередь избранного до перемешивания
  for (uint8_t i = 0; i < MODE_AMOUNT; i++)
    shuffleFavoriteModes[i] = i;
#endif

  // EEPROM
  EepromManager::InitEepromSettings(                        // инициализация EEPROM; запись начального состояния настроек, если их там ещё нет; инициализация настроек лампы значениями из EEPROM
    modes, alarms, &espMode, &ONflag, &dawnMode, &currentMode, &buttonEnabled,
#ifdef RANDOM_SETTINGS_IN_CYCLE_MODE
    &random_on,
#endif //ifdef RANDOM_SETTINGS_IN_CYCLE_MODE
    & (FavoritesManager::ReadFavoritesFromEeprom),
    &(FavoritesManager::SaveFavoritesToEeprom),
    &(restoreSettings)); // не придумал ничего лучше, чем делать восстановление настроек по умолчанию в обработчике инициализации EepromManager
  LOG.printf_P(PSTR("Рабочий режим лампы: ESP_MODE = %d\n"), espMode);


  // WI-FI
  wifiManager.setDebugOutput(WIFIMAN_DEBUG);                // вывод отладочных сообщений
  // wifiManager.setMinimumSignalQuality();                 // установка минимально приемлемого уровня сигнала WiFi сетей (8% по умолчанию)
  CaptivePortalManager *captivePortalManager = new CaptivePortalManager(&wifiManager);
  if (espMode == 0U)                                        // режим WiFi точки доступа
  {
    // wifiManager.setConfigPortalBlocking(false);

    if (sizeof(AP_STATIC_IP))
    {
      LOG.println(F("Используется статический IP адрес WiFi точки доступа"));
      wifiManager.setAPStaticIPConfig(                      // wifiManager.startConfigPortal использовать нельзя, т.к. он блокирует вычислительный процесс внутри себя, а затем перезагружает ESP, т.е. предназначен только для ввода SSID и пароля
        IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], AP_STATIC_IP[3]),      // IP адрес WiFi точки доступа
        IPAddress(AP_STATIC_IP[0], AP_STATIC_IP[1], AP_STATIC_IP[2], 1),                    // первый доступный IP адрес сети
        IPAddress(255, 255, 255, 0));                                                       // маска подсети
    }

    WiFi.softAP(AP_NAME, AP_PASS);
    //WiFi.softAP(AP_NAME+String("_MQTTid_")+String(ESP.getChipId(), HEX), AP_PASS);// для тех, кому нужно узнать чип-id для mqtt. HEX - в шестнадцатиричном формате, DEC - в десятичном.

    LOG.println(F("Старт в режиме WiFi точки доступа"));
    LOG.print(F("IP адрес: "));
    LOG.println(WiFi.softAPIP());

    wifiServer.begin();
  }
  else                                                      // WiFi client mode (we connect to the router if there is a saved SSID and password, otherwise we create a WiFi access point and request them)
  {
    LOG.println(F("Старт в режиме WiFi клиента (подключение к роутеру)"));

    if (WiFi.SSID().length())
    {
      LOG.printf_P(PSTR("Подключение к WiFi сети: %s\n"), WiFi.SSID().c_str());
#ifdef RUNUP_PAUSE // a pause before connecting to the router for those to whom electricity often goes out, and the router does not have time to boot
      for (uint8_t i = 0; i < RUNUP_PAUSE; i++) {
        delay(1000);
        ESP.wdtFeed();
      }
#endif

      if (sizeof(STA_STATIC_IP))                            // ВНИМАНИЕ: настраивать статический ip WiFi клиента можно только при уже сохранённых имени и пароле WiFi сети (иначе проявляется несовместимость библиотек WiFiManager и WiFi)
      {
        LOG.print(F("Configured a static IP address: "));
        LOG.printf_P(PSTR("%u.%u.%u.%u\n"), STA_STATIC_IP[0], STA_STATIC_IP[1], STA_STATIC_IP[2], STA_STATIC_IP[3]);
        wifiManager.setSTAStaticIPConfig(
          IPAddress(STA_STATIC_IP[0], STA_STATIC_IP[1], STA_STATIC_IP[2], STA_STATIC_IP[3]),// статический IP адрес ESP в режиме WiFi клиента
          IPAddress(STA_STATIC_IP[0], STA_STATIC_IP[1], STA_STATIC_IP[2], 1),               // первый доступный IP адрес сети (справедливо для 99,99% случаев; для сетей меньше чем на 255 адресов нужно вынести в константы)
          IPAddress(255, 255, 255, 0));                                                     // маска подсети (справедливо для 99,99% случаев; для сетей меньше чем на 255 адресов нужно вынести в константы)
      }
    }
    else
    {
      LOG.println(F("WiFi network not detected, launching WiFi access point to configure parameters for connecting to WiFi network..."));
      CaptivePortalManager::captivePortalCalled = true;
      wifiManager.setBreakAfterConfig(true);                // перезагрузка после ввода и сохранения имени и пароля WiFi сети
      showWarning(CRGB::Yellow, 1000U, 500U);               // мигание жёлтым цветом 0,5 секунды (1 раз) - нужно ввести параметры WiFi сети для подключения
    }
#ifdef WARNING_IF_NO_TIME
    noTimeWarningShow();
#endif

    wifiManager.setConnectTimeout(ESP_CONN_TIMEOUT);        // установка времени ожидания подключения к WiFi сети, затем старт WiFi точки доступа
    wifiManager.setConfigPortalTimeout(ESP_CONF_TIMEOUT);   // установка времени работы WiFi точки доступа, затем перезагрузка; отключить watchdog?
    wifiManager.autoConnect(AP_NAME, AP_PASS);              // пытаемся подключиться к сохранённой ранее WiFi сети; в случае ошибки, будет развёрнута WiFi точка доступа с указанными AP_NAME и паролем на время ESP_CONN_TIMEOUT секунд; http://AP_STATIC_IP:ESP_HTTP_PORT (обычно http://192.168.0.1:80) - страница для ввода SSID и пароля от WiFi сети роутера

    delete captivePortalManager;
    captivePortalManager = NULL;

    if (WiFi.status() != WL_CONNECTED)                      // подключение к WiFi не установлено
    {
      if (CaptivePortalManager::captivePortalCalled)        // была показана страница настройки WiFi ...
      {
        if (millis() < (ESP_CONN_TIMEOUT + ESP_CONF_TIMEOUT) * 1000U) // пользователь ввёл некорректное имя WiFi сети и/или пароль или запрошенная WiFi сеть недоступна
        {
          LOG.println(F("Не удалось подключиться к WiFi сети\nУбедитесь в корректности имени WiFi сети и пароля\nРестарт для запроса нового имени WiFi сети и пароля...\n"));
          wifiManager.resetSettings();
        }
        else                                                // пользователь не вводил имя WiFi сети и пароль
        {
          LOG.println(F("Время ожидания ввода SSID и пароля от WiFi сети или подключения к WiFi сети превышено\nЛампа будет перезагружена в режиме WiFi точки доступа!\n"));

#ifdef RESET_WIFI_ON_ESP_MODE_CHANGE
          if (espMode) wifiManager.resetSettings();                             // сброс сохранённых SSID и пароля (сброс настроек подключения к роутеру)
#endif
          espMode = (espMode == 0U) ? 1U : 0U;

          EepromManager::SaveEspMode(&espMode);

          LOG.printf_P(PSTR("Рабочий режим лампы изменён и сохранён в энергонезависимую память\nНовый рабочий режим: ESP_MODE = %d, %s\nРестарт...\n"),
                       espMode, espMode == 0U ? F("WiFi точка доступа") : F("WiFi клиент (подключение к роутеру)"));
        }
      }
      else                                                  // страница настройки WiFi не была показана, не удалось подключиться к ранее сохранённой WiFi сети (перенос в новую WiFi сеть)
      {
        //LOG.println(F("Не удалось подключиться к WiFi сети\nВозможно, заданная WiFi сеть больше не доступна\nРестарт для запроса нового имени WiFi сети и пароля...\n"));
        LOG.println(F("Failed to connect to WiFi network, Maybe the specified WiFi network is no longer available. Restart to request a new WiFi network name and password...\n"));
        wifiManager.resetSettings();
      }

      showWarning(CRGB::Red, 1000U, 500U);                  // blinking red for 0.5 seconds (1 time) - waiting for the SSID and password of the WiFi network is stopped, reboot
      ESP.restart();
    }

    if (CaptivePortalManager::captivePortalCalled &&        // first connection to a WiFi network after configuring WiFi parameters on the setup page - a reboot is required to apply a static IP
        sizeof(STA_STATIC_IP) &&
        WiFi.localIP() != IPAddress(STA_STATIC_IP[0], STA_STATIC_IP[1], STA_STATIC_IP[2], STA_STATIC_IP[3]))
    {
      //LOG.println(F("Рестарт для применения заданного статического IP адреса..."));
      LOG.println(F("Restart to apply the specified static IP address..."));
      delay(100);
      ESP.restart();
    }

    LOG.print(F("IP address: "));
    LOG.println(WiFi.localIP());

#ifdef USE_BLYNK
    Blynk.config(USE_BLYNK);
#endif
  }
  ESP.wdtFeed();

  LOG.printf_P(PSTR("UDP server port: %u\n"), localPort);
  Udp.begin(localPort);


  // NTP
#ifdef USE_NTP
  timeClient.begin();
  ESP.wdtFeed();
#endif


  // MQTT
#if (USE_MQTT)
  if (espMode == 1U)
  {
    mqttClient = new AsyncMqttClient();
    MqttManager::setupMqtt(mqttClient, inputBuffer, &sendCurrent);    // создание экземпляров объектов для работы с MQTT, их инициализация и подключение к MQTT брокеру
  }
  ESP.wdtFeed();
#endif


  // ОСТАЛЬНОЕ
  memset(matrixValue, 0, sizeof(matrixValue)); //это массив для эффекта Огонь. странно, что его нужно залить нулями
  randomSeed(micros());
  changePower();
  loadingFlag = true;

  TextTicker = RUNNING_TEXT_DEFAULT;
}


void loop()
{
  parseUDP();
  if (Painting == 0) {

    effectsTick();

    EepromManager::HandleEepromTick(&settChanged, &eepromTimeout, &ONflag,
                                    &currentMode, modes, &(FavoritesManager::SaveFavoritesToEeprom));

    //#ifdef USE_NTP
#if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
    //if (millis() > 30 * 1000U) можно попытаться оттянуть срок первой попытки синхронизации времени на 30 секунд, чтобы роутер успел не только загрузиться, но и соединиться с интернетом
    timeTick();
#endif

#ifdef ESP_USE_BUTTON
    //if (buttonEnabled) в процедуре ведь есть эта проверка
    buttonTick();
#endif

#ifdef OTA
    otaManager.HandleOtaUpdate();                             // ожидание и обработка команды на обновление прошивки по воздуху
#endif

    TimerManager::HandleTimer(&ONflag, &settChanged,          // обработка событий таймера отключения лампы
                              &eepromTimeout, &changePower);

    if (FavoritesManager::HandleFavorites(                    // обработка режима избранных эффектов
          &ONflag,
          &currentMode,
          &loadingFlag
          //#ifdef USE_NTP
#if defined(USE_NTP) || defined(USE_MANUAL_TIME_SETTING) || defined(GET_TIME_FROM_PHONE)
          , &dawnFlag
#endif
#ifdef RANDOM_SETTINGS_IN_CYCLE_MODE
          , &random_on
          , &selectedSettings
#endif
        ))
    {
#ifdef USE_BLYNK
      updateRemoteBlynkParams();
#endif
      FastLED.setBrightness(modes[currentMode].Brightness);
      //FastLED.clear(); //because of this strange line between the effects, the lamp was completely extinguished. why do that ?!
      //delay(1); //and because of this still
    }

#if USE_MQTT
    if (espMode == 1U && mqttClient && WiFi.isConnected() && !mqttClient->connected())
    {
      MqttManager::mqttConnect();                             // the library does not know how to restore the connection if the connection to the MQTT broker is lost, you need to manage this explicitly
      MqttManager::needToPublish = true;
    }

    if (MqttManager::needToPublish)
    {
      if (strlen(inputBuffer) > 0)                            // check the incoming MQTT message; if it is not empty, the command is executed from it and the MQTT response is generated
      {
        processInputBuffer(inputBuffer, MqttManager::mqttBuffer, true);
      }

      MqttManager::publishState();
    }
#endif

#ifdef USE_BLYNK
    if (espMode == 1U && WiFi.isConnected())
      Blynk.run();
#endif

#if defined(GENERAL_DEBUG) && GENERAL_DEBUG_TELNET
    handleTelnetClient();
#endif
  }//if (Painting == 0)
  ESP.wdtFeed();
}

#ifndef WIFI_MANAGER_LIBRARY_PROPER_TEST
//!!!!!!!!!!!!!!!!!!!!!!!!!!!   the libraries from the firmware archive were not placed in the Arduino IDE libraries folder. so you will not download the firmware.
#endif
