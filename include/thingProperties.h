// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "2be684ca-5b26-4305-bb91-911bddbf2c45";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onSetTempHWSChange();
void onSetTempRoomChange();
void onManualControlChange();
void onSwitch5Change();
void onSwitchGasBoilerChange();
void onSwitchPumpHWSChange();
void onSwitchPumpRadiatorsChange();
void onSwitchPumpRecirculationHWSChange();
void onHeatRequestPlusHWSChange();
void onHysteresisGasBoilerChange();
void onHysteresisHWSChange();
void onMaxTempGasBoilerChange();
void onMinTempGasBoilerChange();

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

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(setTemp_HWS, READWRITE, ON_CHANGE, onSetTempHWSChange);
  ArduinoCloud.addProperty(setTemp_Room, READWRITE, ON_CHANGE, onSetTempRoomChange);
  ArduinoCloud.addProperty(manual_Control, READWRITE, ON_CHANGE, onManualControlChange);
  ArduinoCloud.addProperty(switch_5, READWRITE, ON_CHANGE, onSwitch5Change);
  ArduinoCloud.addProperty(switch_GasBoiler, READWRITE, ON_CHANGE, onSwitchGasBoilerChange, 1);
  ArduinoCloud.addProperty(switch_PumpHWS, READWRITE, ON_CHANGE, onSwitchPumpHWSChange);
  ArduinoCloud.addProperty(switch_PumpRadiators, READWRITE, ON_CHANGE, onSwitchPumpRadiatorsChange, 1);
  ArduinoCloud.addProperty(switch_PumpRecirculationHWS, READWRITE, ON_CHANGE, onSwitchPumpRecirculationHWSChange);
  ArduinoCloud.addProperty(temp_GasBoiler, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(temp_HWSBoiler, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(temp_Radiators, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(temp_Room, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(temp_Street, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(heatRequest_GasBoiler, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(heatRequest_HWS, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(heatRequest_Plus_HWS, READWRITE, ON_CHANGE, onHeatRequestPlusHWSChange);
  ArduinoCloud.addProperty(heatRequest_Radiators, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(hysteresis_GasBoiler, READWRITE, ON_CHANGE, onHysteresisGasBoilerChange);
  ArduinoCloud.addProperty(hysteresis_HWS, READWRITE, ON_CHANGE, onHysteresisHWSChange);
  ArduinoCloud.addProperty(maxTemp_GasBoiler, READWRITE, ON_CHANGE, onMaxTempGasBoilerChange);
  ArduinoCloud.addProperty(minTemp_GasBoiler, READWRITE, ON_CHANGE, onMinTempGasBoilerChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
