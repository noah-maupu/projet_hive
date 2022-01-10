#define MAXIMWIRE_EXTERNAL_PULLUP

#include "DHT.h"
#include "HX711.h"
#include <MaximWire.h>

float weight = 0;
float temperatureIn1 = 0;
float temperatureIn2 = 0;
float temperatureIn3 = 0;
float temperatureOut = 0;
float humidityIn = 0;
float humidityOut = 0;
unsigned int batteryState = 0;
unsigned int state = 0; ///////////////////
unsigned int orientation = 3; //////////////////////
unsigned int luminosity = 0;
unsigned int sunshineHours = 0;

int payload[3];

MaximWire::Bus busSondeTemperatureIn1(D2);
MaximWire::Bus busSondeTemperatureIn2(D3);
MaximWire::Bus busSondeTemperatureIn3(D4);
MaximWire::DS18B20 sondeTemperatureIn1("28316B75D0013C4C");
MaximWire::DS18B20 sondeTemperatureIn2("28628275D0013C12");
MaximWire::DS18B20 sondeTemperatureIn3("28707475D0013C1D");
DHT dhtTemperatureAndHumidityOut(D12, DHT22);
DHT dhtHumidityIn(D5, DHT22);
HX711 x711Weight;

const double scaleValue = 21655.5;
const int batteryStatePin = A6;
const int luminosityPin = A5;
const int tplPin = D11;

void getTemperaturesIn();
void getTemperatureAndHumidityOut();
void getWeight();
void getHumidityIn();
void getBatteryState();

void sendPayload();
void datasToPayload();

void setup()
{
  Serial.begin(9600); 
  Serial1.begin(9600);

  pinMode(batteryStatePin, INPUT);
  pinMode(luminosityPin, INPUT);
  pinMode(tplPin, OUTPUT);
  
  dhtTemperatureAndHumidityOut.begin();
  dhtHumidityIn.begin();

  x711Weight.begin(D6, D7);
  x711Weight.set_scale(scaleValue);

  delay(6000);
  digitalWrite(LED_PWR, LOW);
}

void loop()
{
  getTemperaturesIn();
  getTemperatureAndHumidityOut();
  getHumidityIn();
  getBatteryState();
  getLuminosity();
  getWeight();

  datasToPayload();

  sendPayload();

  Serial.println("-------------------------------------");

  delay(10000);
  
  digitalWrite(tplPin, HIGH);

  delay(0xFFFFFFFF);
}


void getTemperaturesIn()
{
  float localTemperatureIn1 = sondeTemperatureIn1.GetTemperature<float>(busSondeTemperatureIn1);

  if (!isnan(localTemperatureIn1))
  {
    Serial.print(localTemperatureIn1);
    Serial.println(" <== Température intérieure 1 : success !");
    temperatureIn1 = localTemperatureIn1;
  }
  else
  {
    Serial.println("FAILURE <== Température intérieure 1 : failed !");
    temperatureIn1 = 25;
  }

  sondeTemperatureIn1.Update(busSondeTemperatureIn1);

  float localTemperatureIn2 = sondeTemperatureIn2.GetTemperature<float>(busSondeTemperatureIn2);

  if (!isnan(localTemperatureIn2))
  {
    Serial.print(localTemperatureIn2);
    Serial.println(" <== Température intérieure 2 : success !");
    temperatureIn2 = localTemperatureIn2;
  }
  else
  {
    Serial.println("FAILURE <== Température intérieure 2 : failed !");
    temperatureIn2 = 25;
  }

  sondeTemperatureIn2.Update(busSondeTemperatureIn2);

  float localTemperatureIn3 = sondeTemperatureIn3.GetTemperature<float>(busSondeTemperatureIn3);

  if (!isnan(localTemperatureIn3))
  {
    Serial.print(localTemperatureIn3);
    Serial.println(" <== Température intérieure 3 : success !");
    temperatureIn3 = localTemperatureIn3;
  }
  else
  {
    Serial.println("FAILURE <== Température intérieure 3 : failed !");
    temperatureIn3 = 25;
  }

  sondeTemperatureIn3.Update(busSondeTemperatureIn3);
}

void getTemperatureAndHumidityOut()
{
  float localTemperatureOut = dhtTemperatureAndHumidityOut.readTemperature();
  float localHumidityOut = dhtTemperatureAndHumidityOut.readHumidity();

  if (!isnan(localTemperatureOut))
  {
    Serial.print(localTemperatureOut);
    Serial.println(" <== Température extérieure : success !");
    temperatureOut = localTemperatureOut;
  }
  else
    Serial.println("FAILURE <== Température extérieure : failed !");

  if (!isnan(localHumidityOut))
  {
    Serial.print(localHumidityOut);
    Serial.println(" <== Humidité extérieure : success !");
    humidityOut = localHumidityOut;
  }
  else
    Serial.println("FAILURE <== Humidité extérieure : failed !");
}

void getWeight()
{
  if (x711Weight.is_ready())
  {
    double localWeight = x711Weight.get_units(10);
    if (!isnan(localWeight))
    {
      Serial.print(localWeight - 2.14);
      Serial.println(" <== HX711 : success !");
      weight = localWeight - 2.14;
    }
    else
      Serial.println("FAILURE <== HX711 : failed !");
  }
  else
    Serial.println("FAILURE <== HX711 : failed !");

  x711Weight.power_down();
  delay(1000);
}

void getHumidityIn()
{
  float localHumidityIn = dhtHumidityIn.readHumidity();

  if (!isnan(localHumidityIn))
  {
    Serial.print(localHumidityIn);
    Serial.println(" <== Humidité intérieure : success !");
    humidityIn = localHumidityIn;
  }
  else
    Serial.println("FAILURE <== Humidité intérieure : failed !");

  if ((int) temperatureIn1 == 25 || (int) temperatureIn2 == 25 || (int) temperatureIn3 == 25 || isnan(temperatureIn1) || isnan(temperatureIn2) || isnan(temperatureIn3))
  {
    float localTemperatureIn = dhtHumidityIn.readTemperature();
  
    if (!isnan(localTemperatureIn))
    {
      Serial.print(localTemperatureIn);
      Serial.println(" <== Température intérieure via DHT22 : success !");
      temperatureIn1 = localTemperatureIn;
      temperatureIn2 = localTemperatureIn + random(-10, 10)/10;
      temperatureIn3 = localTemperatureIn + random(-10, 10)/10;
    }
    else
      Serial.println("FAILURE <== Température intérieure : failed !");
  }  
}

void getBatteryState()
{
  float rawBatteryState = analogRead(batteryStatePin);
  float Vin = 3.3*(rawBatteryState/1024.0);
  float Vbat = Vin*1.303241442;
  int localBatteryState = (((Vbat-3.3)*139.86013986 + 0.5)*100/127);
  int LOCAL_TEST = localBatteryState;
  localBatteryState = max(min(localBatteryState, 100), 0);
  Serial.print(localBatteryState);
  Serial.print("(Valeur non bordée : ");
  Serial.print(LOCAL_TEST);
  Serial.print(")");
  Serial.println(" <== Etat de la batterie : success !");
  batteryState = localBatteryState;
}

void getLuminosity()
{
  float localLuminosity = analogRead(luminosityPin);
  Serial.print(localLuminosity);
  Serial.println(" <== Luminosite : success !");
  luminosity = (494 - localLuminosity)*100/494;
}

void sendPayload()
{
  char buffer[100];
  sprintf(buffer,"AT$SF=%08x%08x%08x\r\n", *payload, *(payload+1), *(payload+2));
  Serial.print(*payload);
  Serial.print(*(payload+1));
  Serial.println(*(payload+2));
  Serial.write(buffer);
  Serial1.write(buffer);
}

void datasToPayload()
{
  payload[0] = 0;
  payload[1] = 0;
  payload[2] = 0;

  payload[0]  = (int) ((unsigned int) ((min(max(weight, 0), 204.7) + 0.05)*10) << 21);
  payload[0] |= (int) ((unsigned int) ((min(max(temperatureIn1, 0), 51.1) + 0.05)*10) << 12);
  payload[0] |= (int) ((unsigned int) ((min(max(temperatureIn2, 0), 51.1) + 0.05)*10) << 3);
  payload[0] |= (int) ((unsigned int) ((min(max(temperatureIn3, 0), 51.1) + 0.05)*10) >> 6);

  payload[1]  = (int) ((unsigned int) ((min(max(temperatureIn3, 0), 51.1) + 0.05)*10) << 26);
  payload[1] |= (int) ((unsigned int) ((min(max(temperatureOut, -30.0), 72.3) + 30 + 0.05)*10) << 16);
  payload[1] |= (int) ((unsigned int) (humidityIn/2) << 10);
  payload[1] |= (int) ((unsigned int) (humidityOut/2) << 4);
  payload[1] |= (int) (batteryState >> 3);

  payload[2]  = (int) (batteryState << 29);
  payload[2] |= (int) (state << 25);
  payload[2] |= (int) (orientation << 22);
  payload[2] |= (int) ((min(max(luminosity, (unsigned int) 0), (unsigned int) 100000)) << 6);
  payload[2] |= (int) ((min(max(sunshineHours, (unsigned int) 0), (unsigned int) 31)) << 1);
  payload[2] |= (int) 1;
}
