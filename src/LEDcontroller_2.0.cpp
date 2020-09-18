#include <Arduino.h>

#include <ss_oled.h>

#define USE_BACKBUFFER
static uint8_t ucBackBuffer[1024];

#ifdef USE_BACKBUFFER
#else
static uint8_t *ucBackBuffer = NULL;
#endif

#define SDA_PIN 21
#define SCL_PIN 22
#define RESET_PIN -1
#define OLED_ADDR -1
#define FLIP180 0
#define INVERT 0
#define USE_HW_I2C 0
#define MY_OLED OLED_72x40
#define OLED_WIDTH 72
#define OLED_HEIGHT 40

SSOLED ssoled;

#define BUTTON_PIN_BITMASK 0x2010

#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 50

#include "FastLED.h"

#define NUM_LEDS 35
#define DATA_PIN 26
#define DATA_PIN_HOLD GPIO_NUM_26


#include <esp_now.h>

#define CHANNEL 3
#define CHANNEL_2 1

esp_now_peer_info_t slave;

#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

#include <WiFi.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <stdlib.h>
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"

int max_bright = 11;
int val;
//int resetCount;
bool chargPin;
int colorVal;

CRGB leds[NUM_LEDS];

int wfBtMode;
int pageNum = 1;
int clientCounter;
int LEDcount = 35;
int pageCount;
int ledCount3;
int step1;
int ledArr[35][3];

String WiFiName = "LED_Jeans_by_519Obsessions";
String WiFiPass = "";

RTC_DATA_ATTR int modeNumber = 0;
RTC_DATA_ATTR int resetCount = 0;

unsigned long t_start_check_web = millis();

int deviceAction = 0;

int bat2;
boolean charging = false;

int count;

#define LEDstripPower GPIO_NUM_27
#define Button 4
#define BatteryVoltage 36
#define BatteryVoltagePower GPIO_NUM_25
#define ChargingSense 13
#define OLEDpower GPIO_NUM_10

String mode1 = "";
String mode11 = "";
String mode12 = "";
String mode13 = "";
String mode14 = "";
String mode15 = "";
String mode16 = "";
String mode17 = "";
String mode18 = "";
String mode19 = "";
String mode20 = "";

String mode00 = "";
String mode111 = "";
String mode112 = "";
String mode113 = "";
String mode114 = "";
String mode115 = "";
String mode116 = "";
String mode117 = "";
String mode118 = "";
String mode119 = "";
String mode120 = "";

Preferences preferences;

const byte DNS_PORT = 53;

IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiServer server(80);

String style = R"rawText(<style>.block1 { width: 60px;border: solid 0px; float: left;}html,body,div,span,applet,object,iframe,h1,h2,h3,h4,h5,h6,p,blockquote,pre,a,abbr,acronym,address,big,cite,code,del,dfn,em,img,ins,kbd,q,s,samp,small,strike,strong,sub,sup,tt,var,b,u,i,center,hr,dl,dt,dd,ol,ul,li,fieldset,form,label,legend,table,caption,tbody,tfoot,thead,tr,th,td,article,aside,canvas,details,embed,figure,figcaption,footer,header,hgroup,menu,nav,output,ruby,section,summary,time,mark,audio,video{margin:0;padding:0;outline:0;border:0;color:inherit;font-size:100%;font: "arial";text-decoration:none;text-align:left;vertical-align:baseline;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}article,aside,details,figcaption,figure,footer,header,hgroup,menu,nav,section{display:block}body{line-height:1.4;margin:1.5em auto;padding:0 .5em;overflow-y:scroll}body,input,button,textarea,select{font-size:20px;font-family: "arial"}h1,h2,h3,h4,h5,h6{line-height:1.2;margin:.4em 0;page-break-after:avoid}h1{font-size:3.1em}h2{font-size:2.5em}h3{font-size:2em}h4{font-size:1.6em}h5{font-size:1.3em}h6{font-size:1.1em}p,ul,ol,dl,blockquote,table,hr,form,address,pre,label{margin:.5em 0 1.5em}li{margin:.5em 0 .5em 2em}dd{margin:.5em 0 .5em 1.5em}ol ol{list-style-type:lower-latin}ul ul,ol ol{margin-bottom:.5em}acronym,abbr,dfn,dt,table caption{font-variant:small-caps}blockquote,cite,q,address,figcaption,em,i{font-style: "arial"}blockquote{quotes:none}q{quotes:'В«' 'В»' 'вЂћ' 'вЂњ'}blockquote:before,blockquote:after{content:'';content:none}strong,b{font-weight:bold}del,s{text-decoration:line-through}small,sub,sup{font-size:.8em}sub,sup{line-height:0;position:relative}sup{top:-0.5em}sub{bottom:-0.25em}acronym,abbr,dfn{cursor:help;border-bottom:1px dotted}code,samp,var,kbd,tt{font-family: "arial"}pre{white-space:pre;overflow-y:hidden;overflow-x:auto}pre>code{display:block;line-height:1.2;padding:.1em 0}hr{border-bottom:1px solid}a{text-decoration:underline}mark{background-color:highlight;color:highlighttext}table{width:100%;border-collapse:collapse;border-spacing:0}th,td{vertical-align:top;text-align:left;padding:.5em}table,th,td{border:1px solid}label{display:block}input,button,textarea,select{display:block;padding:.5em;margin:0;line-height:normal;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}input[type=checkbox],input[type=radio]{padding:0}button,input[type=checkbox],input[type=radio],input[type=submit],input[type=reset],input[type=button]{cursor:pointer;display:inline-block;width:auto}textarea{width:100%;height:7em}button,input[type=submit],input[type=reset],input[type=button]{overflow:visible;white-space:nowrap}::-moz-focus-inner{padding:0 !important;border:0 !important}body{background:#000;color:#aaa}h1,h2,h3,h4,h5,h6,acronym,abbr,dfn,dt,table caption,th,ins{color:#fff}a{color:#68a}a:hover{color:#9cf}acronym,abbr,dfn,h3,hr{border-bottom-color:#aaa}table,td{border-color:#393939}th{border-color:#555;background:#393939}kbd,tt{background:#1c1c1c}tbody tr:hover td{background:#11171c}input,button,textarea,select{outline:0;color:#aaa;background:#000;border:1px #393939 solid;box-shadow:0 0 1ex #393939}input:hover,button:hover,textarea:hover,select:hover,input:focus,button:focus,textarea:focus,select:focus{border-color:#345;box-shadow:0 0 1ex #345}input[type="checkbox"],input[type="checkbox"]:hover,input[type="checkbox"]:focus,input[type="radio"],input[type="radio"]:hover,input[type="radio"]:focus{box-shadow:none;border:none}button,input[type=submit],input[type=reset],input[type=button]{background-color:#1c1c1c;box-shadow:0 .3ex 1ex #393939;text-shadow:0 -1px 1px #000;background-image:-webkit-linear-gradient(top, #1c1c1c, #000);background-image:-moz-linear-gradient(top, #1c1c1c, #000);background-image:linear-gradient(top, #1c1c1c, #000)}button:hover,button:focus,input[type=submit]:hover,input[type=submit]:focus,input[type=reset]:hover,input[type=reset]:focus,input[type=button]:hover,input[type=button]:focus{box-shadow:0 .3ex 1ex #345;color:#9cf}button:active,input[type=submit]:active,input[type=reset]:active,input[type=button]:active{position:relative;top:1px;box-shadow:0 .2ex 1ex -0.2ex #345;color:#9cf}</style>)rawText";

String htmlEdit = "";

String htmlEditPart1 = R"rawText(<!DOCTYPE html><html><body>)rawText"
                        + style + R"rawText(<table width="40%" align="left"><tbody><tr><td><strong><em>LED Jeans by 519Obsessions</em></strong></td></tr><tr><td><strong><em>Mode editor)rawText";
String htmlEditPart2 = R"rawText(</em></strong></td></tr><form action="/">)rawText";
String htmlEditContent = "";
String htmlEditPart3 = R"rawText(<tr><td><button name="Save" value="true">Save</button></form></td><td><form action="/"><button name="Back" value="true">Back</button></form></td></tr><tr><td>&copy; LED Jeans by 519Obsessions</td></tr></tbody></table></body></html>)rawText";

String htmlSaveConfirm = R"rawText(<!DOCTYPE html><html> <head> </head> <body>)rawText"
                         + style + R"rawText(Settings saved <form action="/"><button name="Back" value="true">Back to settings</button></form> <form action="/"><button name="Exit" value="true">Exit</button></form> </body></html>)rawText";
String htmlExitConfirm = R"rawText(<!DOCTYPE html><html> <head> </head> <body>)rawText"
                         + style + R"rawText(WiFi config stopped. Device in standart operational mode  </body></html>)rawText";


void WiFiConfig();


int getBatteryState() {
  pinMode(BatteryVoltagePower, OUTPUT);
  gpio_hold_dis(BatteryVoltagePower);
  digitalWrite(BatteryVoltagePower, HIGH);
  pinMode(BatteryVoltage, INPUT);
  delay(100);
  
  int batteryVoltage;
  
  for (int i = 0; i < 10; i++ ) {
    Serial.println(analogRead(BatteryVoltage));
    batteryVoltage += analogRead(BatteryVoltage);
    delay(10);
  }
  
  batteryVoltage /= 10;
  batteryVoltage = map(batteryVoltage, 1740.0f, 2350.0f, 0, 100);

  if(batteryVoltage > 100){
    batteryVoltage = 100;
  }else if(batteryVoltage < 0){
    batteryVoltage = 0;
  }

  Serial.print("BatteryCharge(%): ");
  Serial.println(batteryVoltage);
  digitalWrite(BatteryVoltagePower, LOW);
  gpio_hold_en(BatteryVoltagePower);
  return (batteryVoltage);
}


void rebuildEditPage(String modeRAW) {
  htmlEditContent = "";
  
  for (int i = 0; i < LEDcount + 1; i++) {
    String colour = "";
    if (modeRAW.indexOf(",") != -1) {
      colour = modeRAW.substring(0, modeRAW.indexOf(","));
      modeRAW = modeRAW.substring(modeRAW.indexOf(",") + 1);
    }
    else {
      colour = modeRAW;
    }

    Serial.print("LED");
    Serial.print(i);
    Serial.print(":");
    Serial.println(colour);

    htmlEditContent += R"rawText(<tr><td><strong>)rawText";
    htmlEditContent += "LED" + String(i+1) + ":";
    htmlEditContent += R"rawText(</strong></td><td><input id="color" name=")rawText";
    htmlEditContent += "LED" + String(i+1) + ":";
    htmlEditContent += R"rawText(" type="color" value=")rawText";
    htmlEditContent += colour;
    htmlEditContent += R"rawText(" /> </td></tr>)rawText";

    htmlEdit = htmlEditPart1 + " " + String(modeNumber) + htmlEditPart2 + htmlEditContent + htmlEditPart3;
  }
}


void rebuildEditPage2(String modeRAW) {
  htmlEditContent = "";

  for (int j = 0; j < LEDcount; j++) {

    long rr, gg, bb;

    rr = ledArr[j][0];
    gg = ledArr[j][1];
    bb = ledArr[j][2];

    long RGB = 0;

    RGB = ((long)rr << 16L) | ((long)gg << 8L) | (long)bb;

    String colour = "";
    String colour2 = "8a7500";
    colour = String(int(RGB), HEX);

    htmlEditContent += R"rawText(<tr><td><strong>)rawText";
    htmlEditContent += "LED" + String(j+1) + ":";
    htmlEditContent += R"rawText(</strong></td><td>)rawText";
    htmlEditContent += R"rawText(<div style=" width: 30%; height: 40px; background:#)rawText";
    htmlEditContent += colour;
    //htmlEditContent += RGB;
    htmlEditContent += R"rawText(; float: left; border: solid 1px black;"> </div>    </td></tr>)rawText";

    htmlEdit = htmlEditPart1 + " " + String(modeNumber) + htmlEditPart2 + htmlEditContent + htmlEditPart3;
  }
  //Serial.println(htmlEdit);
}


void ledShow(int count, int red, int green, int blue){
  leds[count].setRGB(red, green, blue);
}


void one_color_all(int cred, int cgrn, int cblu) {
  for (int i = 0 ; i < LEDcount; i++ ) {
    ledShow(i, cred, cgrn, cblu);
  }
}


void turnOnLED() {
  pinMode(LEDstripPower, OUTPUT);
  gpio_hold_dis(LEDstripPower);
  digitalWrite(LEDstripPower, HIGH);
  gpio_hold_en(LEDstripPower);
}


void turnOffLED() {
  pinMode(LEDstripPower, OUTPUT);
  digitalWrite(LEDstripPower, HIGH);
  delay(50);
  for (int i = 0 ; i < LEDcount; i++ ) {
    ledShow(i, 0, 0, 0);
  }
  LEDS.show();
  delay(100);
  //pinMode(LEDstripPower, OUTPUT);
  //pinMode(DATA_PIN_HOLD, OUTPUT);
  gpio_hold_dis(LEDstripPower);
  digitalWrite(LEDstripPower, LOW);
  //digitalWrite(DATA_PIN_HOLD, HIGH);
  //ESP.restart();
  gpio_hold_en(LEDstripPower);
  //gpio_hold_en(DATA_PIN_HOLD);
  Serial.println("***led off***");
}


void oledOn(){
  pinMode(OLEDpower, OUTPUT);
  gpio_hold_dis(OLEDpower);
  digitalWrite(OLEDpower, HIGH);
  gpio_hold_en(OLEDpower);
}


void oledOff(){
  pinMode(OLEDpower, OUTPUT);
  gpio_hold_dis(OLEDpower);
  digitalWrite(OLEDpower, HIGH);
  delay(10);
  oledFill(&ssoled, 0, 1);
  oledWriteString(&ssoled, 0,10,1,(char*) "  ", FONT_LARGE, 0, 1);
  delay(10);
  digitalWrite(OLEDpower, LOW);
  gpio_hold_en(OLEDpower);
}


void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}


void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  // reset on each scan
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      //if (PRINTSCANRESULTS) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(SSID);
        Serial.print(" (");
        Serial.print(RSSI);
        Serial.print(")");
        Serial.println("");
      //}
      delay(10);
      // Check if the current device starts with `Slave`
      if (SSID.indexOf("Slave") == 0) {
        // SSID of interest
        Serial.println("Found a Slave.");
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        // Get BSSID => Mac Address of the Slave
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; // pick a channel
        slave.encrypt = 0; // no encryption

        slaveFound = 1;
        // we are planning to have only one slave in this example;
        // Hence, break after we find one, to be a bit efficient
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }

  // clean up ram
  WiFi.scanDelete();
}


void deletePeer(){
  esp_err_t delStatus = esp_now_del_peer(slave.peer_addr);
  Serial.print("Slave Delete Status: ");
  if (delStatus == ESP_OK) {
    // Delete success
    Serial.println("Success");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW Not Init");
  } else if (delStatus == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}


bool manageSlave() {
  if (slave.channel == CHANNEL) {
    if (DELETEBEFOREPAIR) {
      deletePeer();
    }

    Serial.print("Slave Status: ");
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(slave.peer_addr);
    if ( exists) {
      // Slave already paired.
      Serial.println("Already Paired");
      return true;
    } else {
      // Slave not paired, attempt pair
      esp_err_t addStatus = esp_now_add_peer(&slave);
      if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
      } else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
      } else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
      } else {
        Serial.println("Not sure what happened");
        return false;
      }
    }
  } else {
    // No slave found to process
    Serial.println("No Slave found to process");
    return false;
  }
}


int resetCounter;


void sendData() {
  char sendMode[952];
  esp_err_t result;

  for(int ii = 0; ii < 10; ii++){
    if(ii == 0){
      oledOn();
      int rc;
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
      }
      resetCounter += 1;
      if(resetCounter == 2){
        ESP.restart();
      }
      mode11.toCharArray(sendMode, 952);
    }else if(ii == 1){
      oledOff();
      mode12.toCharArray(sendMode, 952);
    }else if(ii == 2){
      oledOn();
      int rc;
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
      }
      mode13.toCharArray(sendMode, 952);
    }else if(ii == 3){
      oledOff();
      mode14.toCharArray(sendMode, 952);
    }else if(ii == 4){
      oledOn();
      int rc;
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
      }
      mode15.toCharArray(sendMode, 952);
    }else if(ii == 5){
      oledOff();
      mode16.toCharArray(sendMode, 952);
    }else if(ii == 6){
      oledOn();
      int rc;
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
      }
      mode17.toCharArray(sendMode, 952);
    }else if(ii == 7){
      oledOff();
      mode18.toCharArray(sendMode, 952);
    }else if(ii == 8){
      oledOn();
      int rc;
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
      }
      mode19.toCharArray(sendMode, 952);
    }else if(ii == 9){
      oledOff();
      mode20.toCharArray(sendMode, 952);
    }
    
    const uint8_t *peer_addr = slave.peer_addr;
    Serial.print("Sending: ");
    
      for(int i = 0; i < (953/200)+1; i++){
      delay(1);
      char k[200];
      for(int j = 0; j < 200; j++){
        delay(1);
        if((j+(i*200)) < 952){
          k[j] = sendMode[j+(i*200)];
        } 
      }
      Serial.println(k);

      result = esp_now_send(peer_addr, (uint8_t *) &k, sizeof(k));
      delay(1);
    }
  }


delay(3);
//result = esp_now_send(peer_addr, (uint8_t *) &mode11, sizeof(mode11));
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

int iii;

void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL_2, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}


void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  // int ii;
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  Serial.print("Last Packet Recv Data: "); Serial.println((char*)data);
  Serial.println("");

    if(iii < 5){
      if(iii == 0){
        oledOn();
        int rc;
        rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
        }
      }
      mode111 = mode111 + (char*)data;
      preferences.putString("mode11", mode00);
      delay(3);
      preferences.putString("mode11", mode111);
      Serial.println(mode111);
    }else if(iii < 10){
      if(iii == 5){
        oledOff();
      }
      mode112 = mode112 + (char*)data;
      preferences.putString("mode12", mode00);
      delay(3);
      preferences.putString("mode12", mode112);
      Serial.println(mode112);
    }else if(iii < 15){
      if(iii == 10){
        oledOn();
        int rc;
        rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
        }
      }
      mode113 = mode113 + (char*)data;
      preferences.putString("mode13", mode00);
      delay(3);
      preferences.putString("mode13", mode113);
      Serial.println(mode113);
    }else if(iii < 20){
      if(iii == 15){
        oledOff();
      }
      mode114 = mode114 + (char*)data;
      preferences.putString("mode14", mode00);
      delay(3);
      preferences.putString("mode14", mode114);
      Serial.println(mode114);
    }else if(iii < 25){
      if(iii == 20){
        oledOn();
        int rc;
        rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
        }
      }
      mode115 = mode115 + (char*)data;
      preferences.putString("mode15", mode00);
      delay(3);
      preferences.putString("mode15", mode115);
      Serial.println(mode115);
    }else if(iii < 30){
      if(iii == 25){
        oledOff();
      }
      mode116 = mode116 + (char*)data;
      preferences.putString("mode16", mode00);
      delay(3);
      preferences.putString("mode16", mode116);
      Serial.println(mode116);
    }else if(iii < 35){
      if(iii == 30){
        oledOn();
        int rc;
        rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
        }
      }
      mode117 = mode117 + (char*)data;
      preferences.putString("mode17", mode00);
      delay(3);
      preferences.putString("mode17", mode117);
      Serial.println(mode117);
    }else if(iii < 40){
      if(iii == 35){
        oledOff();
      }
      mode118 = mode118 + (char*)data;
      preferences.putString("mode18", mode00);
      delay(3);
      preferences.putString("mode18", mode118);
      Serial.println(mode118);
    }else if(iii < 45){
      if(iii == 40){
        oledOn();
        int rc;
        rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
        if (rc != OLED_NOT_FOUND){
          oledFill(&ssoled, 0, 1);
          oledWriteString(&ssoled, 0,0,1,(char*) "SYNC...", FONT_LARGE, 0, 1);
          oledSetBackBuffer(&ssoled, ucBackBuffer);
        }
      }
      mode119 = mode119 + (char*)data;
      preferences.putString("mode19", mode00);
      delay(3);
      preferences.putString("mode19", mode119);
      Serial.println(mode119);
    }else if(iii < 50){
      if(iii == 45){
        oledOff();
      }
      mode120 = mode120 + (char*)data;
      preferences.putString("mode20", mode00);
      delay(3);
      preferences.putString("mode20", mode120);
      Serial.println(mode120);
    }

    iii+=1;

    if(iii == 50){
      ESP.restart();
    }

  // mode111 = mode111 + (char*)data;
  // preferences.putString("mode11", mode111);
   
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
 
  //status == ESP_NOW_SEND_SUCCESS ?  : );
}


void btSGet(){

  //#%3A=%23ffff80&LED2%3A=%23ffff00&LED3%3A=%23ff80c0&LED4%3A=%23ff80ff&LED5%3A=%23ff00ff&LED6%3A=%23ff0080&LED7%3A=%238000ff&LED8%3A=%23000000&3000000&LED9%3A=%2LED10%3A=%23000000&LED11%3A=%23000000&LED12%3A=%23000000&LED13%3A=%23000000&LED14%3A=%23000000&LED15%3A=%23000000&LED16%3A=%23000000&LED17%3A=%23000000&LED18%3A=%23000000&LED19%3A=%23000000&LED20%3A=%23000000&LED21%3A=%23000000&LED22%3A=%23000000&LED23%3A=%23000000&LED24%3A=%23000000&LED25%3A=%23000000&LED26%3A=%23000000&LED27%3A=%23000000&LED28%3A=%23000000&LED29%3A=%23000000&LED30%3A=%23800040&LED31%3A=%23000000&LED32%3A=%23000000&LED33%3A=%23000000&LED34%3A=%23000000&LED35%3A=%23000000&Save=true HTTP/1.1,#ffff80,#ffff00,#ff80c0,#ff80ff,#ff00ff,#ff0080,#8000ff,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#000000,#800040,#000000,#000000,#000000,#000000,#000000

  preferences.putInt("wfBtMode", 0);

  //preferences.putInt("wfBtMode", 3);

  Serial.print("button value: ");
  Serial.println(digitalRead(Button));

  oledOn();

  int rc;

  rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND){
    oledFill(&ssoled, 0, 1);
    oledWriteString(&ssoled, 0,11,1,(char*) "GET", FONT_LARGE, 0, 1);
    oledSetBackBuffer(&ssoled, ucBackBuffer);
  }

  // unsigned long t_end_check_4;

  delay(100);

  unsigned long t_end_check_4;

  unsigned long timing;
  timing = millis(); 

  while(millis() - timing < 3000){
    while (digitalRead(Button) == HIGH) { 
      t_end_check_4 = millis();
    }

    while (millis() - t_end_check_4 < 100) {
      if (digitalRead(Button) == LOW) {
        Serial.println("short button press in btGet() setup");
        //btStop();
        preferences.putInt("wfBtMode", 1);
        delay(10);
        //ESP.restart();
        WiFiConfig();
      }
    }
  }

  Serial.println("ESPNow/Basic/Slave Example");
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode
  configDeviceAP();
  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);

  unsigned long millisTimeout;
  millisTimeout = millis();

  while(1){
    while (digitalRead(Button) == HIGH) { 
      t_end_check_4 = millis();
    }

    while (millis() - t_end_check_4 < 100) {
      if (digitalRead(Button) == LOW) {
        Serial.println("short button press in btGet() setup");
        //btStop();
        preferences.putInt("wfBtMode", 1);
        delay(10);
        //ESP.restart();
        WiFiConfig();
      }
    }
    if(millis() - millisTimeout >= 30000){
      Serial.println("timeout in btSent(), board will reset now");
      delay(10);
      ESP.restart();
    }
  }
}


void btSent(){

  Serial.print("button value: ");
  Serial.println(digitalRead(Button));

  // wfBtMode = 2;

  preferences.putInt("wfBtMode", 0);

  //oledOn();
  
  Serial.println("Starting OLED...");
  
  int rc;

  rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz

  if (rc != OLED_NOT_FOUND){
    oledFill(&ssoled, 0, 1);
    oledWriteString(&ssoled, 0,6,1,(char*) "SENT", FONT_LARGE, 0, 1);
    oledSetBackBuffer(&ssoled, ucBackBuffer);
  }

  delay(100);
  
  unsigned long t_end_check_3;

  unsigned long timing7;
  timing7 = millis(); 

  while(millis() - timing7 < 3000){
    while (digitalRead(Button) == HIGH) { 
      t_end_check_3 = millis();
    }

    while (millis() - t_end_check_3 < 100) {
      if (digitalRead(Button) == LOW) {
        Serial.println("short button press in btGet() setup");
        //btStop();
        preferences.putInt("wfBtMode", 1);
        delay(10);
        //ESP.restart();
        btSGet();
      }
    }
  }

  //unsigned long t_end_check_3;

  

  WiFi.mode(WIFI_STA);
  Serial.println("ESPNow/Basic/Master Example");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  unsigned long timing;
  unsigned long timing2;
  unsigned long millisTimeout;
  timing = millis();
  timing2 = millis(); 
  millisTimeout = millis();
 

  while(1){

    //while(millis() - timing2 < 300){
      while (digitalRead(Button) == HIGH) { 
        t_end_check_3 = millis();
      }

      while (millis() - t_end_check_3 < 100) {
        if (digitalRead(Button) == LOW) {
          Serial.println("short button press in btSent() setup");
          preferences.putInt("wfBtMode", 3);
          delay(100);
          //ESP.restart();
          btSGet();
        }
      }
    //}

      if(millis() - millisTimeout >= 30000){
        Serial.println("timeout in btSent(), board will reset now");
        delay(10);
        ESP.restart();
      }

    //if (millis() - timing > 3000){

      ScanForSlave();
      // If Slave is found, it would be populate in `slave` variable
      // We will check if `slave` is defined and then we proceed further
      if (slave.channel == CHANNEL) { // check if slave channel is defined
        // `slave` is defined
        // Add slave as peer if it has not been added already
        bool isPaired = manageSlave();
        if (isPaired) {
          // pair success or already paired
          // Send data to device
          sendData();
          millisTimeout = millis();
        } else {
          // slave pair failed
          Serial.println("Slave pair failed!");
        }
      }
      else {
        // No slave found to process
      }

      // wait for 3seconds to run the logic again
      //delay(3000);
    //}

    //timing2 = millis(); 
  
  }
}


void WiFiConfig() {

  Serial.print("button value: ");
  Serial.println(digitalRead(Button));

  preferences.putInt("wfBtMode", 0);

  //preferences.putInt("wfBtMode", 1);

  // btSent();

  // delay(100);

  Serial.println("Starting OLED...");

  oledOn();
  
  int rc;

  rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND){
    oledFill(&ssoled, 0, 1);
    oledWriteString(&ssoled, 0,6,1,(char*) "WIFI", FONT_LARGE, 0, 1);
    oledSetBackBuffer(&ssoled, ucBackBuffer);
    delay(100);
  }

  unsigned long t_end_check_2;
  
  Serial.println("WiFi config starting...");
  WiFi.mode(WIFI_AP);

  if (WiFiPass == "") {
    WiFi.softAP(WiFiName.c_str());
  }
  else {
    WiFi.softAP(WiFiName.c_str(), WiFiPass.c_str());
  }
  while(!(WiFi.softAPIP()== apIP)){
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));    
  }

  //Serial.println(WiFi.softAP(WiFiName.c_str()) ? "soft-AP setup": "Failed to connect");

  //Serial.println(WiFi.softAPIP());
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  dnsServer.start(DNS_PORT, "*", apIP);
  server.begin();
  
  while (true) {

    while (digitalRead(Button) == HIGH) { 
      t_end_check_2 = millis();
    }

    while (millis() - t_end_check_2 < 100) {
      if (digitalRead(Button) == LOW) {
        //WiFi.mode(WIFI_OFF);
        Serial.println("short button press in WiFi setup");
        btSent();
      }
    }

    //LEDcount = preferences.getInt("LEDcount", LEDcount);
    dnsServer.processNextRequest();
    WiFiClient client = server.available(); 
    if (client) {
      String currentLine = "";
      while (client.connected()) {
        if (client.available()) {
          // if(clientCounter == 0){
          //   clientCounter = 1;
          //   t_start_check_web = millis();
          // }
          char c = client.read();
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();
              if (pageNum == 1) {
                String htmlMain = R"rawText(<!DOCTYPE html><html><body>)rawText"
                  + style + R"rawText(<table width="40%" align="left"> <tbody> <tr> <td colspan="3"><strong><em>LED Jeans by 519Obsessions</em></strong></td></tr><form action="/"> <tr> <td><strong>Mode:</strong></td><td><strong>Mode name:</strong></td></tr><tr> <td>Mode 1:</td><td><input type="submit" value="Rainbow" name="mode1"/></td></tr><tr> <td>Mode 2:</td><td>Blue</td></tr><tr> <td>Mode 3:</td><td>Purple</td></tr><tr> <td>Mode 4:</td><td>Pink</td></tr><tr> <td>Mode 5:</td><td>Red</td></tr><tr> <td>Mode 6:</td><td>Orange</td></tr><tr> <td>Mode 7:</td><td>Yellow</td></tr><tr> <td>Mode 8:</td><td>Green</td></tr><tr> <td>Mode 9:</td><td>Light blue</td></tr><tr> <td>Mode 10:</td><td>White</td></tr><tr> <td>Mode 11:</td><td><input type="submit" value="Edit" name="mode11"/></td></tr><tr> <td>Mode 12:</td><td><input type="submit" value="Edit" name="mode12"/></td></tr><tr> <td>Mode 13:</td><td><input type="submit" value="Edit" name="mode13"/></td></tr><tr> <td>Mode 14:</td><td><input type="submit" value="Edit" name="mode14"/></td></tr><tr> <td>Mode 15:</td><td><input type="submit" value="Edit" name="mode15"/></td></tr><tr> <td>Mode 16:</td><td><input type="submit" value="Edit" name="mode16"/></td></tr><tr> <td>Mode 17:</td><td><input type="submit" value="Edit" name="mode17"/></td></tr><tr> <td>Mode 18:</td><td><input type="submit" value="Edit" name="mode18"/></td></tr><tr> <td>Mode 19:</td><td><input type="submit" value="Edit" name="mode19"/></td></tr><tr> <td>Mode 20:</td><td><input type="submit" value="Edit" name="mode20"/></td></tr><tr> <td><strong><em>Device settings:</em></strong></td></tr><tr> <td>Dev name (WiFi name):</td><td><input type="text" id="devname" name="devname" value=")rawText"
                  + WiFiName + R"rawText("></td></tr><tr> <td>Dev pass (WiFi pass):</td><td><input type="text" id="devpass" name="devpass" value=")rawText"
                  + WiFiPass + R"rawText("></td></td></tr><tr> <td>LEDs count in strip:</td><td><input type="number" id="ledcount" name="ledcount" value=")rawText"
                  + String(LEDcount) + R"rawText("></td></td></tr><tr> <td><button name="Save" value="true">Save</button></form></td><td><form action="/"><button name="Exit" value="true">Exit</button> </form></td></tr><tr> <td>&copy; LED Jeans by 519Obsessions</td></tr></tbody> </table> </body></html>)rawText";

                client.print(htmlMain);
              }
              if (pageNum == 0) {
                client.print(htmlEdit);
              }
              if (pageNum == 2) { 
                client.print(htmlSaveConfirm);
              }
              if (pageNum == 3) {
                client.print(htmlExitConfirm);
                Serial.println("Exit from WiFi config mode");
                Serial.println("ESP restart in 1 second");
                preferences.putInt("wfBtMode", 0);
                oledOff();
                delay(1000);
                client.stop();
                ESP.restart();
              }
              break;
            } else {
              Serial.println(currentLine);
              if (pageNum == 1) {
                if (currentLine.indexOf("mode") != -1) {
                  modeNumber = (currentLine.substring(currentLine.indexOf("mode") + 4, currentLine.indexOf("=Edit"))).toInt();
                  Serial.print("mode number that editing:");
                  Serial.println(modeNumber);
                  if (modeNumber == 1) {
                    rebuildEditPage2(mode1);
                  }
                  if (modeNumber == 11) {
                    rebuildEditPage(mode11);
                  }
                  if (modeNumber == 12) {
                    rebuildEditPage(mode12);
                  }
                  if (modeNumber == 13) {
                    rebuildEditPage(mode13);
                  }
                  if (modeNumber == 14) {
                    rebuildEditPage(mode14);
                  }
                  if (modeNumber == 15) {
                    rebuildEditPage(mode15);
                  }
                  if (modeNumber == 16) {
                    rebuildEditPage(mode16);
                  }
                  if (modeNumber == 17) {
                    rebuildEditPage(mode17);
                  }
                  if (modeNumber == 18) {
                    rebuildEditPage(mode18);
                  }
                  if (modeNumber == 19) {
                    rebuildEditPage(mode19);
                  }
                  if (modeNumber == 20) {
                    rebuildEditPage(mode20);
                  }

                  client.println();
                  client.print(htmlEdit);
                  pageNum = 0;
                  break;
                }

                if (currentLine.indexOf("Exit") != -1) {
                  client.println();
                  client.print(htmlExitConfirm);
                  pageNum = 3;
                  break;
                }
                if (currentLine.indexOf("Save") != -1) {
                  client.println();
                  client.print(htmlSaveConfirm);
                  pageNum = 2;

                  WiFiName = currentLine.substring(currentLine.indexOf("?devname=") + 9, currentLine.indexOf("&devpass=")); //get WiFiName from substring
                  Serial.print("WiFiName:");
                  Serial.println(WiFiName);

                  WiFiPass = currentLine.substring(currentLine.indexOf("&devpass=") + 9, currentLine.indexOf("&ledcount=")); //get WiFiPass from substring
                  Serial.print("WiFiPass:");
                  Serial.println(WiFiPass);

                  LEDcount = (currentLine.substring(currentLine.indexOf("&ledcount=") + 10, currentLine.indexOf("&Save="))).toInt(); //get LEDcount from substring
                  Serial.print("LEDcount:");
                  Serial.println(LEDcount);

                  preferences.putString("wifi_name", WiFiName);
                  preferences.putString("wifi_pass", WiFiPass);
                  preferences.putInt("LEDcount", LEDcount);

                  break;
                }
              }

              if (pageNum == 0) { 
                if (currentLine.indexOf("Back") != -1) {
                  client.println();
                  String htmlMain = R"rawText(<!DOCTYPE html><html><body>)rawText"
                  + style + R"rawText(<table width="40%" align="left"> <tbody> <tr> <td colspan="3"><strong><em>LED Jeans by 519Obsessions</em></strong></td></tr><form action="/"> <tr> <td><strong>Mode:</strong></td><td><strong>Mode name:</strong></td></tr><tr> <td>Mode 1:</td><td><input type="submit" value="Rainbow" name="mode1"/></td></tr><tr> <td>Mode 2:</td><td>Blue</td></tr><tr> <td>Mode 3:</td><td>Purple</td></tr><tr> <td>Mode 4:</td><td>Pink</td></tr><tr> <td>Mode 5:</td><td>Red</td></tr><tr> <td>Mode 6:</td><td>Orange</td></tr><tr> <td>Mode 7:</td><td>Yellow</td></tr><tr> <td>Mode 8:</td><td>Green</td></tr><tr> <td>Mode 9:</td><td>Light blue</td></tr><tr> <td>Mode 10:</td><td>White</td></tr><tr> <td>Mode 11:</td><td><input type="submit" value="Edit" name="mode11"/></td></tr><tr> <td>Mode 12:</td><td><input type="submit" value="Edit" name="mode12"/></td></tr><tr> <td>Mode 13:</td><td><input type="submit" value="Edit" name="mode13"/></td></tr><tr> <td>Mode 14:</td><td><input type="submit" value="Edit" name="mode14"/></td></tr><tr> <td>Mode 15:</td><td><input type="submit" value="Edit" name="mode15"/></td></tr><tr> <td>Mode 16:</td><td><input type="submit" value="Edit" name="mode16"/></td></tr><tr> <td>Mode 17:</td><td><input type="submit" value="Edit" name="mode17"/></td></tr><tr> <td>Mode 18:</td><td><input type="submit" value="Edit" name="mode18"/></td></tr><tr> <td>Mode 19:</td><td><input type="submit" value="Edit" name="mode19"/></td></tr><tr> <td>Mode 20:</td><td><input type="submit" value="Edit" name="mode20"/></td></tr><tr> <td><strong><em>Device settings:</em></strong></td></tr><tr> <td>Dev name (WiFi name):</td><td><input type="text" id="devname" name="devname" value=")rawText"
                  + WiFiName + R"rawText("></td></tr><tr> <td>Dev pass (WiFi pass):</td><td><input type="text" id="devpass" name="devpass" value=")rawText"
                  + WiFiPass + R"rawText("></td></td></tr><tr> <td>LEDs count in strip:</td><td><input type="number" id="ledcount" name="ledcount" value=")rawText"
                  + String(LEDcount) + R"rawText("></td></td></tr><tr> <td><button name="Save" value="true">Save</button></form></td><td><form action="/"><button name="Exit" value="true">Exit</button> </form></td></tr><tr> <td>&copy; LED Jeans by 519Obsessions</td></tr></tbody> </table> </body></html>)rawText";

                  client.print(htmlMain);
                  pageNum = 1;
                  break;
                }

                if (currentLine.indexOf("Save") != -1) {
                  String newColours = "";
                  for (int i = 1; i < LEDcount + 1; i++) {
                    int counterIncresedByOne = i + 1;
                    String buff1 = "LED" + String(i) + "%3A=%23";
                    String buff2 = "&LED" + String(counterIncresedByOne) + "%3A=%23";
                    if (i == LEDcount) {
                      newColours = newColours + "#" + currentLine.substring(currentLine.lastIndexOf(buff1) + buff1.length(), currentLine.indexOf("&Save"));  //get last LED colour from substring
                    }
                    else {
                      newColours = newColours + "#" + currentLine.substring(currentLine.indexOf(buff1) + buff1.length(), currentLine.indexOf(buff2)) + "," ; //get LED colour from substring
                    }
                  }
                  Serial.println("");
                  Serial.println(newColours);

                  if (modeNumber == 11) {
                    preferences.putString("mode11", mode00);
                    delay(3);
                    preferences.putString("mode11", newColours);
                    mode11 = newColours;
                  }
                  if (modeNumber == 12) {
                    preferences.putString("mode12", mode00);
                    delay(3);
                    preferences.putString("mode12", newColours);
                    mode12 = newColours;
                  }
                  if (modeNumber == 13) {
                    preferences.putString("mode13", mode00);
                    delay(3);
                    preferences.putString("mode13", newColours);
                    mode13 = newColours;
                  }
                  if (modeNumber == 14) {
                    preferences.putString("mode14", mode00);
                    delay(3);
                    preferences.putString("mode14", newColours);
                    mode14 = newColours;
                  }
                  if (modeNumber == 15) {
                    preferences.putString("mode15", mode00);
                    delay(3);
                    preferences.putString("mode15", newColours);
                    mode15 = newColours;
                  }
                  if (modeNumber == 16) {
                    preferences.putString("mode16", mode00);
                    delay(3);
                    preferences.putString("mode16", newColours);
                    mode16 = newColours;
                  }
                  if (modeNumber == 17) {
                    preferences.putString("mode17", mode00);
                    delay(3);
                    preferences.putString("mode17", newColours);
                    mode17 = newColours;
                  }
                  if (modeNumber == 18) {
                    preferences.putString("mode18", mode00);
                    delay(3);
                    preferences.putString("mode18", newColours);
                    mode18 = newColours;
                  }
                  if (modeNumber == 19) {
                    preferences.putString("mode19", mode00);
                    delay(3);
                    preferences.putString("mode19", newColours);
                    mode19 = newColours;
                  }
                  if (modeNumber == 20) {
                    preferences.putString("mode20", mode00);
                    delay(3);
                    preferences.putString("mode20", newColours);
                    mode20 = newColours;
                  }
                  client.println();
                  client.print(htmlSaveConfirm);
                  pageNum = 2;

                  break;
                }
              }

              if (pageNum == 2) {
                if (currentLine.indexOf("Exit") != -1) {
                  client.println();
                  client.print(htmlExitConfirm);
                  pageNum = 3;
                  break;
                }
                if (currentLine.indexOf("Back") != -1) {
                  client.println();
                  String htmlMain = R"rawText(<!DOCTYPE html><html><body>)rawText"
                  + style + R"rawText(<table width="40%" align="left"> <tbody> <tr> <td colspan="3"><strong><em>LED Jeans by 519Obsessions</em></strong></td></tr><form action="/"> <tr> <td><strong>Mode:</strong></td><td><strong>Mode name:</strong></td></tr><tr> <td>Mode 1:</td><td><input type="submit" value="Rainbow" name="mode1"/></td></tr><tr> <td>Mode 2:</td><td>Blue</td></tr><tr> <td>Mode 3:</td><td>Purple</td></tr><tr> <td>Mode 4:</td><td>Pink</td></tr><tr> <td>Mode 5:</td><td>Red</td></tr><tr> <td>Mode 6:</td><td>Orange</td></tr><tr> <td>Mode 7:</td><td>Yellow</td></tr><tr> <td>Mode 8:</td><td>Green</td></tr><tr> <td>Mode 9:</td><td>Light blue</td></tr><tr> <td>Mode 10:</td><td>White</td></tr><tr> <td>Mode 11:</td><td><input type="submit" value="Edit" name="mode11"/></td></tr><tr> <td>Mode 12:</td><td><input type="submit" value="Edit" name="mode12"/></td></tr><tr> <td>Mode 13:</td><td><input type="submit" value="Edit" name="mode13"/></td></tr><tr> <td>Mode 14:</td><td><input type="submit" value="Edit" name="mode14"/></td></tr><tr> <td>Mode 15:</td><td><input type="submit" value="Edit" name="mode15"/></td></tr><tr> <td>Mode 16:</td><td><input type="submit" value="Edit" name="mode16"/></td></tr><tr> <td>Mode 17:</td><td><input type="submit" value="Edit" name="mode17"/></td></tr><tr> <td>Mode 18:</td><td><input type="submit" value="Edit" name="mode18"/></td></tr><tr> <td>Mode 19:</td><td><input type="submit" value="Edit" name="mode19"/></td></tr><tr> <td>Mode 20:</td><td><input type="submit" value="Edit" name="mode20"/></td></tr><tr> <td><strong><em>Device settings:</em></strong></td></tr><tr> <td>Dev name (WiFi name):</td><td><input type="text" id="devname" name="devname" value=")rawText"
                  + WiFiName + R"rawText("></td></tr><tr> <td>Dev pass (WiFi pass):</td><td><input type="text" id="devpass" name="devpass" value=")rawText"
                  + WiFiPass + R"rawText("></td></td></tr><tr> <td>LEDs count in strip:</td><td><input type="number" id="ledcount" name="ledcount" value=")rawText"
                  + String(LEDcount) + R"rawText("></td></td></tr><tr> <td><button name="Save" value="true">Save</button></form></td><td><form action="/"><button name="Exit" value="true">Exit</button> </form></td></tr><tr> <td>&copy; LED Jeans by 519Obsessions</td></tr></tbody> </table> </body></html>)rawText";

                  client.print(htmlMain);
                  Serial.println("Back to WiFi config mode");
                  pageNum = 1;
                  break;
                }
              }
              //Serial.println(pageNum);
              currentLine = "";
            }
            //Serial.println(pageNum);

            if(pageCount != pageNum){
              t_start_check_web = millis();
              pageCount = pageNum;
              Serial.println("millis = count");
              // delay(10);
              // Serial.println(millis() - t_start_check_web);
            }

            if (millis() - t_start_check_web > 300000) {
              //if (millis() - t_start_check_web > 60000) {
                client.print(htmlExitConfirm);
                Serial.println("Exit from WiFi config mode");
                Serial.println("ESP restart in 1 second");
                oledOff();
                delay(1000);
                client.stop();
                ESP.restart();
            }

          } else if (c != '\r') {
            currentLine += c;
          }
        }
      }
      client.stop();
    }
  }
}


void sendModeFromString(String buff) {  

  Serial.println("String to parse");
  Serial.println(buff);
  buff.replace(",", ""); 
  Serial.println("String after removing comma symbol");
  Serial.println(buff);

  for (int i = 0; i < LEDcount; i++) {
    
    if (buff.indexOf("#") == -1) {
      ledShow(i, 0, 0, 0);
      continue;
    }

    String hexString = buff.substring(1, buff.indexOf("#", 1)).c_str(); //char* hex="#6f56a3";
    Serial.print("Colour hexString:");
    Serial.println(hexString);

    buff = buff.substring(buff.indexOf("#", 1));
    Serial.print("Rest of the String:");
    Serial.println(buff);

    long number = strtol( &hexString[0], NULL, 16);
    long r = number >> 16;
    long g = number >> 8 & 0xFF;
    long b = number & 0xFF;

    Serial.println("Parsed colours: ");
    Serial.print("Red:");
    Serial.println(r);
    Serial.print("Green:");
    Serial.println(g);
    Serial.print("Blue:");
    Serial.println(b);

    //ledShow(i, r, g, b);

    if(r == 0 || g == 0 || b == 0){
      colorVal = 1;
    }
  }
  if(colorVal == 0){
    //turnOffLED();
  }
}


void showModeFromString(String buff) {  
  
  turnOnLED();
  delay(100);
  
  Serial.println("String to parse");
  Serial.println(buff);
  buff.replace(",", ""); 
  Serial.println("String after removing comma symbol");
  Serial.println(buff);

  for (int i = 0; i < LEDcount; i++) {
    
    if (buff.indexOf("#") == -1) {
      ledShow(i, 0, 0, 0);
      continue;
    }

    String hexString = buff.substring(1, buff.indexOf("#", 1)).c_str(); //char* hex="#6f56a3";
    Serial.print("Colour hexString:");
    Serial.println(hexString);

    buff = buff.substring(buff.indexOf("#", 1));
    Serial.print("Rest of the String:");
    Serial.println(buff);

    long number = strtol( &hexString[0], NULL, 16);
    long r = number >> 16;
    long g = number >> 8 & 0xFF;
    long b = number & 0xFF;

    Serial.println("Parsed colours: ");
    Serial.print("Red:");
    Serial.println(r);
    Serial.print("Green:");
    Serial.println(g);
    Serial.print("Blue:");
    Serial.println(b);

    ledShow(i, g, r, b);

    if(r == 0 || g == 0 || b == 0){
      colorVal = 1;
    }
  }
  if(colorVal == 0){
    turnOffLED();
  }
}


void ShowColours(int mode_light) {
  if (mode_light == 0) {
    turnOffLED();
    return;
  }
  turnOnLED();

  if (mode_light == 1) {

    int ledCount3;
    int step1;
    ledCount3 = LEDcount / 3;
    step1 = 255 / ledCount3;
    
    int r = 0;
    int g = 0;
    int b = 255;
    
    for(int i = 0; i < ledCount3; i++){
      ledShow(i, r, g+=step1, b-=step1);
    }

    r = 0;
    g = 255;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      ledShow(i+ledCount3, r+=step1, g-=step1, b);
    }

    r = 255;
    g = 0;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      ledShow(i+ledCount3+ledCount3, r-=step1, g, b+=step1);
    }

    if(ledCount3 * 3 != LEDcount){
      for(int i = 0; i < (LEDcount - (ledCount3 * 3)); i++){
        ledShow(i+ledCount3+ledCount3+ledCount3, 0, 0, 255);
      }
    }
  }
  else if (mode_light == 2) {
    one_color_all(0, 0, 255); 
  }
  else if (mode_light == 3) {
    one_color_all(0, 200, 255); 
  }
  else if (mode_light == 4) {
    one_color_all(0, 255, 200); 
  }
  else if (mode_light == 5) {
    one_color_all(0, 255, 0); 
  }
  else if (mode_light == 6) {
    one_color_all(140, 255, 0); 
  }
  else if (mode_light == 7) {
    one_color_all(255, 255, 0); 
  }
  else if (mode_light == 8) {
    one_color_all(255, 0, 0); 
  }
  else if (mode_light == 9) {
    one_color_all(224, 130, 250); 
  }
  else if (mode_light == 10) {
    one_color_all(255, 255, 255); 
  }

  else if (mode_light == 11) {
    showModeFromString(mode11);
  }
  else if (mode_light == 12) {
    showModeFromString(mode12);
  }
  else if (mode_light == 13) {
    showModeFromString(mode13);
  }
  else if (mode_light == 14) {
    showModeFromString(mode14);
  }
  else if (mode_light == 15) {
    showModeFromString(mode15);
  }
  else if (mode_light == 16) {
    showModeFromString(mode16);
  }
  else if (mode_light == 17) {
    showModeFromString(mode17);
  }
  else if (mode_light == 18) {
    showModeFromString(mode18);
  }
  else if (mode_light == 19) {
    showModeFromString(mode19);
  }
  else if (mode_light == 20) {
    showModeFromString(mode20);
  }

  //LEDS.setBrightness(max_bright);
  delay(10);
  LEDS.show();
  Serial.println("LED strip updated");
}


void setup() {

  preferences.begin("settings", false);

  
  mode11 = preferences.getString("mode11", "");
  mode12 = preferences.getString("mode12", "");
  mode13 = preferences.getString("mode13", "");
  mode14 = preferences.getString("mode14", "");
  mode15 = preferences.getString("mode15", "");
  mode16 = preferences.getString("mode16", "");
  mode17 = preferences.getString("mode17", "");
  mode18 = preferences.getString("mode18", "");
  mode19 = preferences.getString("mode19", "");
  mode20 = preferences.getString("mode20", "");

  pinMode(Button, INPUT);
  
  Serial.begin(115200);

  Serial.println(preferences.getString("mode11", ""));

  Serial.println(preferences.getInt("wfBtMode", 0));

  if(preferences.getInt("wfBtMode", 0) != 0){
    //delay(500);
    if(preferences.getInt("wfBtMode", 0) == 1){
      ledCount3 = LEDcount / 3;
    step1 = 255 / ledCount3;
    
    
    int r = 17;
    int g = 0;
    int b = 255;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i, r, g+=step1, b-=step1);
      ledArr[i][0] = r;
      ledArr[i][1] = g+=step1;
      ledArr[i][2] = b-=step1;
    }

    r = 0;
    g = 255;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i+ledCount3, r+=step1, g-=step1, b);
      ledArr[i+ledCount3][0] = r+=step1;
      ledArr[i+ledCount3][1] = g-=step1;
      ledArr[i+ledCount3][2] = b;
    }

    r = 255;
    g = 0;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i+ledCount3+ledCount3, r-=step1, g, b+=step1);
      if((r - step1) >= 17){
        ledArr[i+ledCount3+ledCount3][0] = r-=step1;
      }else{
        ledArr[i+ledCount3+ledCount3][0] = 17;
      }
      delay(5);
      ledArr[i+ledCount3+ledCount3][1] = g;
      ledArr[i+ledCount3+ledCount3][2] = b+=step1;
    }

    if(ledCount3 * 3 != LEDcount){
      for(int i = 0; i < (LEDcount - (ledCount3 * 3)); i++){
        //ledShow(i+ledCount3+ledCount3+ledCount3, 0, 0, 255);
        ledArr[i+ledCount3+ledCount3+ledCount3][0] = 17;
        ledArr[i+ledCount3+ledCount3+ledCount3][1] = 17;
        ledArr[i+ledCount3+ledCount3+ledCount3][2] = 255;
      }
    }

    delay(20);
      WiFiConfig();
    }else if(preferences.getInt("wfBtMode", 0) == 2){
      btSent();
    }else if(preferences.getInt("wfBtMode", 0) == 3){
      btSGet();
    }
  }

  

  // Serial.print("bat level: ");
  // Serial.println(getBatteryState());

  // if(getBatteryState() <= 5){
  //   //getBatteryState()
  //   String chargingState = String(getBatteryState()) + "%";
  //   oledOn();
  //   Serial.println("Starting OLED...");
    
  //   int rc;

  //   rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  //   if (rc != OLED_NOT_FOUND){
  //     oledFill(&ssoled, 0, 1);
  //     oledWriteString(&ssoled, 0,20,1,(char*) chargingState.c_str(), FONT_LARGE, 0, 1);
  //     oledSetBackBuffer(&ssoled, ucBackBuffer);
  //   }
  //   delay(600);
  //   oledOff();
  //   delay(600);
  //   oledOn();
  //   Serial.println("Starting OLED...");

  //   rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  //   if (rc != OLED_NOT_FOUND){
  //     oledFill(&ssoled, 0, 1);
  //     oledWriteString(&ssoled, 0,20,1,(char*) chargingState.c_str(), FONT_LARGE, 0, 1);
  //     oledSetBackBuffer(&ssoled, ucBackBuffer);
  //   }
  //   delay(600);
  //   oledOff();
  //   delay(600);
  //   oledOn();
  //   Serial.println("Starting OLED...");

  //   rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  //   if (rc != OLED_NOT_FOUND){
  //     oledFill(&ssoled, 0, 1);
  //     oledWriteString(&ssoled, 0,20,1,(char*) chargingState.c_str(), FONT_LARGE, 0, 1);
  //     oledSetBackBuffer(&ssoled, ucBackBuffer);
  //   }
  //   delay(600);
  //   oledOff();
  //   delay(600);

  //   WiFi.mode(WIFI_OFF);
  //   btStop(); 
  //   delay(100);
  //   esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  //   Serial.println("Going to sleep now");
  //   esp_deep_sleep_start();
  // }

  while((chargPin = digitalRead(13)) == HIGH){

    if(getBatteryState() != bat2){
      val = 0;
    }

    Serial.print(getBatteryState());
    Serial.print(", ");
    Serial.print(bat2);
    Serial.print(", ");

    bat2 = getBatteryState();

    Serial.println(bat2);
    
    if(val == 0){
      turnOffLED();
      delay(10);
      oledOn();
      Serial.println("Starting OLED...");
      getBatteryState();
      String chargingState = String(getBatteryState()) + "%";
  
      int rc;
      //int a = 1;
//      String c = "xcvbnb";
      
      rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
      if (rc != OLED_NOT_FOUND){
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0,10,1,(char*) chargingState.c_str(), FONT_LARGE, 0, 1);
        oledSetBackBuffer(&ssoled, ucBackBuffer);
        delay(500);   
        val += 1;   
      }
    }
    resetCount = 0;
  }


  if(chargPin == LOW){
    oledOff();
    val = 0;
    //resetCount = 0;
  }

  unsigned long t_start_check = millis();
  unsigned long t_end_check = millis();

  while (digitalRead(Button) == HIGH) { 
    t_end_check = millis();
  }

  if (t_end_check - t_start_check > 20000) {
    Serial.println(">20seconds button press");
    deviceAction = 3;
  }
  else if (t_end_check - t_start_check > 10000) {
    Serial.println(">10seconds button press");
    deviceAction = 2;
  }
  else if (t_end_check - t_start_check > 2000) {
    Serial.println(">2seconds button press");
    deviceAction = 1;
  }
  else if (resetCount != 0) {
    Serial.println("quick button press");
    deviceAction = 0;
    //resetCount = 1;
  } else {
    Serial.print("Reset count value is: ");
    Serial.println(resetCount);
    resetCount = 1;
    Serial.println("trash hold press, not counted");  
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH); 
    Serial.println("Going to sleep now");
    WiFi.mode(WIFI_OFF);
    btStop();
    delay(10);
    //ESP.restart();
    esp_deep_sleep_start();
  }


  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, LEDcount);
  LEDS.setBrightness(max_bright);
  count = LEDcount;
  
  //esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

  
  if (preferences.getString("wifi_name", "") == "") {
    Serial.println("custom WiFi name not stored, so write default value");
    preferences.putString("wifi_name", WiFiName);
  }
  else {
    WiFiName = preferences.getString("wifi_name", "");
    Serial.print("WiFi name:");
    Serial.println(WiFiName);
  }

  if (preferences.getString("wifi_pass", "") == "") {
    Serial.println("custom WiFi pass not stored, so write default value");
    preferences.putString("wifi_pass", WiFiPass);
  }
  else {
    WiFiPass = preferences.getString("wifi_pass", "");
    Serial.print("WiFi pass:");
    Serial.println(preferences.getString("wifi_pass", ""));
  }

  if (preferences.getInt("LEDcount", 0) == 0) {
    Serial.println("custom LEDcount not stored, so write default value");
    preferences.putInt("LEDcount", LEDcount);
  }
  else {
    LEDcount = preferences.getInt("LEDcount", 0);
    Serial.print("LEDcount:");
    Serial.println(LEDcount);
  }

  if (deviceAction == 0) {

    modeNumber++;
    if (modeNumber == 21) {
      modeNumber = 0;
    }

    char szTemp[32];

    sprintf(szTemp, "%d", (int)modeNumber);

    turnOnLED();

    Serial.print("Mode: ");
    Serial.println(modeNumber);
    ShowColours(modeNumber);
    
    oledOn();
    Serial.println("Starting OLED...");
    
    int rc;
  
    rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
    if (rc != OLED_NOT_FOUND && modeNumber <= 9){
      oledFill(&ssoled, 0, 1);
      oledWriteString(&ssoled, 0,30,1,(char*) szTemp, FONT_LARGE, 0, 1);
      oledSetBackBuffer(&ssoled, ucBackBuffer);
      delay(500);
    }else if (rc != OLED_NOT_FOUND){
      oledFill(&ssoled, 0, 1);
      oledWriteString(&ssoled, 0,18,1,(char*) szTemp, FONT_LARGE, 0, 1);
      oledSetBackBuffer(&ssoled, ucBackBuffer);
      delay(500);
    }
    oledOff();
  }
   if (deviceAction == 1) {
    oledOn();
    Serial.println("Starting OLED...");
    getBatteryState();
    String chargingState = String(getBatteryState()) + "%";

    int rc;
    //int a = 1;
    
    rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
    if (rc != OLED_NOT_FOUND){
      oledFill(&ssoled, 0, 1);
      oledWriteString(&ssoled, 0,10,1,(char*) chargingState.c_str(), FONT_LARGE, 0, 1);
      oledSetBackBuffer(&ssoled, ucBackBuffer);
      delay(500);     
    }

    delay(3000);
    oledOff();
    delay(100);
   }


  else if (deviceAction == 2) {


    // int ledCount3;
    // int step1;
    // int ledArr[ledCount3][3];
    ledCount3 = LEDcount / 3;
    step1 = 255 / ledCount3;
    
    
    int r = 17;
    int g = 0;
    int b = 255;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i, r, g+=step1, b-=step1);
      ledArr[i][0] = r;
      ledArr[i][1] = g+=step1;
      ledArr[i][2] = b-=step1;
    }

    r = 0;
    g = 255;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i+ledCount3, r+=step1, g-=step1, b);
      ledArr[i+ledCount3][0] = r+=step1;
      ledArr[i+ledCount3][1] = g-=step1;
      ledArr[i+ledCount3][2] = b;
    }

    r = 255;
    g = 0;
    b = 0;
    
    for(int i = 0; i < ledCount3; i++){
      //ledShow(i+ledCount3+ledCount3, r-=step1, g, b+=step1);
      if((r - step1) >= 17){
        ledArr[i+ledCount3+ledCount3][0] = r-=step1;
      }else{
        ledArr[i+ledCount3+ledCount3][0] = 17;
      }
      delay(5);
      ledArr[i+ledCount3+ledCount3][1] = g;
      ledArr[i+ledCount3+ledCount3][2] = b+=step1;
    }

    if(ledCount3 * 3 != LEDcount){
      for(int i = 0; i < (LEDcount - (ledCount3 * 3)); i++){
        //ledShow(i+ledCount3+ledCount3+ledCount3, 0, 0, 255);
        ledArr[i+ledCount3+ledCount3+ledCount3][0] = 17;
        ledArr[i+ledCount3+ledCount3+ledCount3][1] = 17;
        ledArr[i+ledCount3+ledCount3+ledCount3][2] = 255;
      }
    }
    delay(200);


    oledOn();
    // Serial.println("Starting OLED...");
    
    // int rc;

    // rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
    // if (rc != OLED_NOT_FOUND){
    //   oledFill(&ssoled, 0, 1);
    //   oledWriteString(&ssoled, 0,6,1,(char*) "WIFI", FONT_LARGE, 0, 1);
    //   oledSetBackBuffer(&ssoled, ucBackBuffer);
    //   delay(1000);
    //   //oledOff();
    //   //delay(500);
    // }
    turnOnLED();
    delay(100);
    turnOffLED();
    delay(100);
    WiFiConfig();
  }
  else if (deviceAction == 3) {

    oledOn();

    Serial.println("Starting OLED...");
    
    int rc;

    rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
    if (rc != OLED_NOT_FOUND){
      oledFill(&ssoled, 0, 1);
      oledWriteString(&ssoled, 0,0,1,(char*) "reset", FONT_LARGE, 0, 1);
      oledSetBackBuffer(&ssoled, ucBackBuffer);
      turnOnLED();
      delay(1000);
      oledOff();
      turnOffLED();
      //delay(500);
    } 
    
    preferences.putString("wifi_name", "LED_Jeans_by_519Obsessions");
    preferences.putString("wifi_pass", ""); 
    delay(10);
    ESP.restart();
  }

  WiFi.mode(WIFI_OFF);
  btStop(); 


  delay(100);

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  Serial.println("Going to sleep now");
  //ESP.restart();
  delay(100);
  esp_deep_sleep_start();
  
}


void loop() {
}
