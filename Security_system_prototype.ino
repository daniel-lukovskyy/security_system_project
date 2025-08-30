#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // &Wire is link for I^2 object ,-1 is res port

struct SensorData
{
  int motion_brightness;
  int motion_IR;
  int motion_PIR;
};

// center text
void center_text(String lines[], int count, int text_size)
{
  int th = 8 * text_size;
  int block_h = th * count;
  int y_start = (display.height() - block_h) / 2; // display height - block_h = empty space therefore / by 2 this is the center
  //centering the x coordinate
  display.clearDisplay();
  for (int i = 0; i < count; i++)
  {
    int tw = 6 * lines[i].length() * text_size;
    int x = (display.width() - tw) / 2;
    int y = y_start + i * th;

    display.setTextSize(text_size);
    display.setCursor(x, y);
    display.setTextColor(SSD1306_WHITE);
    display.println(lines[i]);
  }
  display.display();
}

SensorData get_sensors_data(int brightness_pin, int IR_pin, int PIR_pin)
{
  SensorData data;

  data.motion_brightness = 4095 - analogRead(brightness_pin);
  data.motion_IR = 4095 - analogRead(IR_pin);
  data.motion_PIR = digitalRead(PIR_pin);
  Serial.print("PIR Value : ");
  Serial.println(data.motion_PIR);

  return data;
}

void show_sensors_data(struct SensorData data)
{
  String brightness = "Brightness: " + String(data.motion_brightness);
  String IR_data = "IR: " + String(data.motion_IR);
  String PIR_data = "PIR: " + String(data.motion_PIR);

  String sensors_data[] = {brightness, IR_data, PIR_data};
  int len_sensor_data = sizeof(sensors_data) / sizeof(sensors_data[0]);

  center_text(sensors_data , len_sensor_data, 1);
}

void show_inter_data(int lights_count, int inter_count)
{
  String lights_on_counter = "Lights: " + String(lights_count);
  String inter_counter = "Interruptions: " + String(inter_count);

  String inter_data[] = {lights_on_counter, inter_counter};
  int len_inter_data = sizeof(inter_data) / sizeof(inter_data[0]);

  center_text(inter_data, len_inter_data, 1);
}

int inter_res(struct SensorData data)
{
  //bounds
  const int brightness_bound = 500;
  const int IR_bound = 100;
  const int PIR_bound = 1;

  bool case_1 = data.motion_brightness >= brightness_bound;
  bool case_2 = case_1 && data.motion_IR >= IR_bound;
  bool case_3 = case_1 && data.motion_PIR >= PIR_bound;
  bool case_4 = (data.motion_IR >= IR_bound) && (data.motion_PIR >= PIR_bound);

  if (case_1) {return 1 ;}
  else if (case_2 || case_3 || case_4) {return 2 ;}
  else {return 0 ;}

}

//pins
const int brightness_pin = 32;
const int IR_pin = 33;
const int PIR_pin = 25;
const int led_pin = 4;





int lights_on_counter = 0;
int inter_counter = 0;



 //time
const char* ssid = "iPhone (Leila)";
const char* password = "12345678";
WiFiUDP ntpUDP; // object for UDP work
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3 * 3600, 60000); // GT+3


void setup() 
{
  // display
  Wire.begin(21,22); // starts I^2 (pins 21,22); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while(true); //if adress not found, 0X3C is adress
  }
  Serial.println("Display OK");
  //PIR
  pinMode(brightness_pin, INPUT);
  pinMode(IR_pin, INPUT);
  pinMode(PIR_pin, INPUT);
  Serial.begin(115200);

  //time
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }
  if (WiFi.status() == WL_CONNECTED) 
  {
  Serial.println("\nWiFi connected!");
  } 
  else 
  {
  Serial.println("\nWiFi not connected, continue offline");
  }
  timeClient.begin();
  

}


void loop() 
{ 
//time
timeClient.update();
    int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();

    if ( (hours >= 7) && (hours < 22) )
    {
      String turn_off_message[] = {"Not working hours", String(hours) + ":" + String(minutes), "L_on: " + String(lights_on_counter), "Itrruptions: " + String(inter_counter)};
      int len_turn_off_message = sizeof(turn_off_message) / sizeof(turn_off_message[0]);
      center_text(turn_off_message, len_turn_off_message, 1);

      if (hours == 15 && minutes == 59 && seconds == 55) 
      {
        lights_on_counter = 0;
        inter_counter = 0;
      }

    }
  
    else 
    {
      SensorData sensors_data = get_sensors_data(brightness_pin, IR_pin, PIR_pin); 
      int res = inter_res(sensors_data);
      if (res == 1) {lights_on_counter++;}
      if (res == 2) {inter_counter++;}
      show_inter_data(lights_on_counter, inter_counter);
      //show_sensors_data(sensors_data);
      delay(500);

      //time update
      timeClient.update();
      hours = timeClient.getHours();
      minutes = timeClient.getMinutes();
    }

    //time update
    timeClient.update();
    hours = timeClient.getHours();
    minutes = timeClient.getMinutes();
    Serial.printf("Time: %02d:%02d\n", hours, minutes);
    delay (1000);

}
