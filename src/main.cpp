/*
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/6ac2e19e-1c26-4275-a0bf-fad7b0f2b0d2

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float setTemp_HWS;
  float setTemp_Room;
  CloudSwitch manual_Control;
  CloudSwitch switch_5;
  CloudSwitch switch_GasBoiler;
  CloudSwitch switch_PumpHWS;
  CloudSwitch switch_PumpRadiators;
  CloudSwitch switch_PumpRecirculationHWS;
  CloudTemperatureSensor temp_GasBoiler;
  CloudTemperatureSensor temp_HWSBoiler;
  CloudTemperatureSensor temp_Radiators;
  CloudTemperatureSensor temp_Room;
  CloudTemperatureSensor temp_Street;
  int heatRequest_GasBoiler;
  int heatRequest_HWS;
  int heatRequest_Plus_HWS;
  int heatRequest_Radiators;
  int hysteresis_GasBoiler;
  int hysteresis_HWS;
  int maxTemp_GasBoiler;
  int minTemp_GasBoiler;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include <Arduino.h>
#include "arduino_secrets.h"
#include "thingProperties.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "Preferences.h"
#include "GyverPID.h"

#define ONE_WIRE_BUS 2
#define RelayPin1 32
#define RelayPin2 33
#define RelayPin3 25
#define RelayPin4 26 
#define RelayPin5 27

Preferences preferences;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
GyverPID PID_Radiators;

unsigned long previousMillis = 0;
const long interval = 1000; // Задержка в мс
float targetTemp_HWS; // Целевая температура ГВС
float targetTemp_Room;  // Целевая температура в комнате
int last_heatRequest_Plus_HWS;
int last_hysteresis_GasBoiler;
int last_hysteresis_HWS;
int last_maxTemp_GasBoiler;
int last_minTemp_GasBoiler;

DeviceAddress sensor1 = { 0x28, 0x6C, 0x5F, 0x57, 0x4, 0xD8, 0x3C, 0xA9 };
DeviceAddress sensor2 = { 0x28, 0x1C, 0x3C, 0x57, 0x4, 0xDA, 0x3C, 0xFF };
DeviceAddress sensor3 = { 0x28, 0x79, 0xEA, 0x81, 0xE3, 0xB6, 0x3C, 0xB7 };
DeviceAddress sensor4 = { 0x28, 0x5D, 0x5E, 0x57, 0x4, 0x27, 0x3C, 0x3F };
DeviceAddress sensor5 = { 0x28, 0x23, 0x72, 0x81, 0xE3, 0xD3, 0x3C, 0x7A };

void checkStateRelay()  {
  Serial.print("Relay 1 (PIN32) - "); 
    if(digitalRead(RelayPin1) == 1){
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }

  Serial.print("Relay 2 (PIN33) - "); 
    if(digitalRead(RelayPin2) == 1){
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }

  Serial.print("Relay 3 (PIN25) - "); 
    if(digitalRead(RelayPin3) == 1){
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }

  Serial.print("Relay 4 (PIN26) - "); 
    if(digitalRead(RelayPin4) == 1){
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }

  Serial.print("Relay 5 (PIN27) - "); 
    if(digitalRead(RelayPin5) == 1){
      Serial.println("ON");
    }
    else {
      Serial.println("OFF");
    }
}
void control_HWSBoiler();
void control_Radiators();
void control_GasBoiler();

void setup() {

  Serial.begin(115200);
  DS18B20.begin();
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);

  PID_Radiators.Kp = 10.0;
  PID_Radiators.Ki = 0.5;
  PID_Radiators.Kd = 0;
  PID_Radiators.setDt(1000);
  PID_Radiators.setLimits(minTemp_GasBoiler, maxTemp_GasBoiler);

  preferences.begin("setting", false);
  targetTemp_HWS = preferences.getFloat("tgTemp_HWS"); // Считываем из EEPROM
  targetTemp_Room = preferences.getFloat("tgTemp_Room");
  heatRequest_Plus_HWS = preferences.getInt("heatReq_PlusHWS");
  hysteresis_GasBoiler = preferences.getInt("hyst_GasBoiler");
  hysteresis_HWS = preferences.getInt("hyst_HWS");
  maxTemp_GasBoiler = preferences.getInt("maxTGasBoiler");
  minTemp_GasBoiler = preferences.getInt("minTGasBoiler");

  last_heatRequest_Plus_HWS = heatRequest_Plus_HWS;
  last_hysteresis_GasBoiler = hysteresis_GasBoiler;
  last_hysteresis_HWS = hysteresis_HWS;
  last_maxTemp_GasBoiler = maxTemp_GasBoiler;
  last_minTemp_GasBoiler = minTemp_GasBoiler;

  setTemp_HWS = -1;
  setTemp_Room = -1;
  
  delay(1000);
  Serial.println("load targetTemp_HWS = " + (String)preferences.getFloat("tgTemp_HWS"));
  Serial.println("load targetTemp_Room) = " + (String)preferences.getFloat("tgTemp_Room"));
  Serial.println("load heatRequest_Plus_HWS = " + (String)preferences.getInt("heatReq_PlusHWS"));
  Serial.println("load hysteresis_GasBoiler = " + (String)preferences.getInt("hyst_GasBoiler"));
  Serial.println("load hysteresis_HWS = " + (String)preferences.getInt("hyst_HWS"));
  Serial.println("load maxTemp_GasBoiler = " + (String)preferences.getInt("maxTGasBoiler"));
  Serial.println("load minTemp_GasBoiler = " + (String)preferences.getInt("minTGasBoiler"));

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
  initProperties(); // Defined in thingProperties.h
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
  */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > interval) { // Аналог Delay, но не блокирующий облачного обновления
    previousMillis = currentMillis;

    Serial.println("\n      Requesting temperatures...");
    DS18B20.requestTemperatures(); // Send the command to get temperatures
    Serial.println("\t\t\tDONE");
  
    Serial.print("Sensor 1(*C): ");
    Serial.println(DS18B20.getTempC(sensor1)); 
    Serial.print("Sensor 2(*C): ");
    Serial.println(DS18B20.getTempC(sensor2)); 
    Serial.print("Sensor 3(*C): ");
    Serial.println(DS18B20.getTempC(sensor3)); 
    Serial.print("Sensor 4(*C): ");
    Serial.println(DS18B20.getTempC(sensor4));
    Serial.print("Sensor 5(*C): ");
    Serial.println(DS18B20.getTempC(sensor5));

    temp_GasBoiler = round(DS18B20.getTempC(sensor1)*10)/10;
    temp_HWSBoiler = round(DS18B20.getTempC(sensor2)*10)/10;
    temp_Radiators = round(DS18B20.getTempC(sensor3)*10)/10;
    temp_Room = round(DS18B20.getTempC(sensor4)*10)/10;
    temp_Street = round(DS18B20.getTempC(sensor5)*10)/10;

    if(manual_Control == false) {  // Если ручное управление выключено
      control_HWSBoiler();  // ГВС
      control_Radiators();  // Радиаторы
      control_GasBoiler();
    }

    onSwitchGasBoilerChange();
    onSwitchPumpHWSChange();
    onSwitchPumpRecirculationHWSChange();
    onSwitchPumpRadiatorsChange();
    onSwitch5Change();
    onManualControlChange();

    onSetTempHWSChange();
    onSetTempRoomChange();
    onHeatRequestPlusHWSChange();

    Serial.println("\ntargetTemp_HWS = " + (String)targetTemp_HWS);
    Serial.println("targetTemp_Room = " + (String)targetTemp_Room);
    Serial.println("heatRequest_Plus_HWS = " + (String)heatRequest_Plus_HWS);
    Serial.println("hysteresis_GasBoiler = " + (String)hysteresis_GasBoiler);
    Serial.println("hysteresis_HWS = " + (String)hysteresis_HWS);
    Serial.println("maxTemp_GasBoiler = " + (String)maxTemp_GasBoiler);
    Serial.println("minTemp_GasBoiler = " + (String)minTemp_GasBoiler);


    Serial.println("\nheatRequest_Radiators = " + (String)PID_Radiators.output);
    Serial.println("heatRequest_HWS = " + (String)heatRequest_HWS);
    Serial.println("heatRequest_GasBoiler = " + (String)heatRequest_GasBoiler);

    checkStateRelay();  // Вывод в serial состояния реле

  }
}

void control_GasBoiler()  {
  heatRequest_GasBoiler = max(heatRequest_HWS, heatRequest_Radiators);
  if(temp_GasBoiler > (heatRequest_GasBoiler + hysteresis_GasBoiler)) {
    switch_GasBoiler = false;
  }
  else if (temp_GasBoiler <= (heatRequest_GasBoiler - hysteresis_GasBoiler)) {
    switch_GasBoiler = true;
  }
}

void control_HWSBoiler()  { // Контур ГВС температура
  if(temp_HWSBoiler > (targetTemp_HWS + hysteresis_HWS))  { 
    switch_PumpHWS = false;   
    heatRequest_HWS = 0;
  }
  else if (temp_HWSBoiler <= (targetTemp_HWS - hysteresis_HWS)) {
    switch_PumpHWS = true;    
    if((targetTemp_HWS + heatRequest_Plus_HWS) > maxTemp_GasBoiler) {
      heatRequest_HWS = maxTemp_GasBoiler;
    }
    else {
      heatRequest_HWS = targetTemp_HWS + heatRequest_Plus_HWS;
    }
  }
}


void control_Radiators() {
  PID_Radiators.setLimits(minTemp_GasBoiler, maxTemp_GasBoiler);
  PID_Radiators.input = temp_Room;  // Указываем текущую температуру для ПИД регулятора
  PID_Radiators.setpoint = targetTemp_Room; // Указываем целевую температуру для комнаты
  PID_Radiators.getResult();
  heatRequest_Radiators = PID_Radiators.output;
  if (heatRequest_Radiators < 20 || (heatRequest_Radiators - temp_Radiators) < -6 ) {
    switch_PumpRadiators = false;
  }
  else if (heatRequest_Radiators >= 20){
    switch_PumpRadiators = true;
  }
}



void onSwitchGasBoilerChange()  {
  if(switch_GasBoiler){
    digitalWrite(RelayPin1, HIGH);
  }
  else{
    digitalWrite(RelayPin1, LOW);
  }
}

void onSwitchPumpHWSChange()  {
  if(switch_PumpHWS){
    digitalWrite(RelayPin2, HIGH);
  }
  else{
    digitalWrite(RelayPin2, LOW);
  }
}

void onSwitchPumpRecirculationHWSChange()  {
  if(switch_PumpRecirculationHWS){
    digitalWrite(RelayPin3, HIGH);
  }
  else{
    digitalWrite(RelayPin3, LOW);
  }
}

void onSwitchPumpRadiatorsChange()  {
  if(manual_Control == false) {
    if(switch_PumpRadiators == true && switch_PumpHWS == false){
      digitalWrite(RelayPin4, HIGH);
    }
    else if (switch_PumpRadiators == false || switch_PumpHWS == true){
      switch_PumpRadiators = false;
      digitalWrite(RelayPin4, LOW);
    }
  }
  else {
    if(switch_PumpRadiators){
      digitalWrite(RelayPin4, HIGH);
    }
    else{
      digitalWrite(RelayPin4, LOW);
    }
  }
}

void onSwitch5Change()  {
  if(switch_5){
    digitalWrite(RelayPin5, HIGH);
  }
  else{
    digitalWrite(RelayPin5, LOW);
  }
}

void onManualControlChange()  {
}

void onSetTempHWSChange()  {
  if(ArduinoCloud.connected() == 1 && setTemp_HWS >= 0 && targetTemp_HWS != setTemp_HWS) {
    targetTemp_HWS = setTemp_HWS;
    preferences.putFloat("tgTemp_HWS", targetTemp_HWS); // Сохраняем в EEPROOM
    Serial.println("Сохранено targetTemp_HWS = " + (String)preferences.getFloat("tgTemp_HWS"));
  }
}

void onSetTempRoomChange()  {
  if(ArduinoCloud.connected() == 1 && setTemp_Room >= 0 && targetTemp_Room != setTemp_Room) {
    targetTemp_Room = setTemp_Room;
    preferences.putFloat("tgTemp_Room", targetTemp_Room); // Сохраняем в EEPROOM
    Serial.println("Сохранено targetTemp_Room = " + (String)preferences.getFloat("tgTemp_Room"));
  }
}

void onHysteresisGasBoilerChange()  {
  if(ArduinoCloud.connected() == 1 && hysteresis_GasBoiler >= 0 && hysteresis_GasBoiler != last_hysteresis_GasBoiler) {
    last_hysteresis_GasBoiler = hysteresis_GasBoiler;
    preferences.putInt("hyst_GasBoiler", hysteresis_GasBoiler); // Сохраняем в EEPROOM
    Serial.println("Сохранено hysteresis_GasBoiler = " + (String)preferences.getInt("hyst_GasBoiler"));
  }
}

void onHysteresisHWSChange()  {
 if(ArduinoCloud.connected() == 1 && hysteresis_HWS >= 0 && hysteresis_HWS != last_hysteresis_HWS) {
    last_hysteresis_HWS = hysteresis_HWS;
    preferences.putInt("hyst_HWS", hysteresis_HWS); // Сохраняем в EEPROOM
    Serial.println("Сохранено hysteresis_HWS = " + (String)preferences.getInt("hyst_HWS"));
  }
}

void onMaxTempGasBoilerChange()  {
 if(ArduinoCloud.connected() == 1 && maxTemp_GasBoiler >= 0 && maxTemp_GasBoiler != last_maxTemp_GasBoiler) {
    last_maxTemp_GasBoiler = maxTemp_GasBoiler;
    preferences.putInt("maxTGasBoiler", maxTemp_GasBoiler); // Сохраняем в EEPROOM
    Serial.println("Сохранено maxTemp_GasBoiler = " + (String)preferences.getInt("maxTGasBoiler"));
  }
}

void onMinTempGasBoilerChange()  {
 if(ArduinoCloud.connected() == 1 && minTemp_GasBoiler >= 0 && minTemp_GasBoiler != last_minTemp_GasBoiler) {
    last_minTemp_GasBoiler = minTemp_GasBoiler;
    preferences.putInt("minTGasBoiler", minTemp_GasBoiler); // Сохраняем в EEPROOM
    Serial.println("Сохранено minTemp_GasBoiler = " + (String)preferences.getInt("minTGasBoiler"));
  }
}

void onHeatRequestGasBoilerChange()  {
  // Add your code here to act upon HeatRequestGasBoiler change
}

void onHeatRequestPlusHWSChange()  {
  if(ArduinoCloud.connected() == 1 && heatRequest_Plus_HWS >= 0 && heatRequest_Plus_HWS != last_heatRequest_Plus_HWS) {
    last_heatRequest_Plus_HWS = heatRequest_Plus_HWS;
    preferences.putInt("heatReq_PlusHWS", heatRequest_Plus_HWS); // Сохраняем в EEPROOM
    Serial.println("Сохранено heatRequest_Plus_HWS = " + (String)preferences.getInt("heatReq_PlusHWS"));
  }
}