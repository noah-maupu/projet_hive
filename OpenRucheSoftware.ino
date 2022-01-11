/*
  OpenRucheSoftware

  Get datas from various sensors, then send a payload
  through a Sigfox antenna, and ask the tpl to be turned off.

  If you want to build your own connected hive following
  our project, you'll need this software to make the whole
  system works perfectly.
  If you want to know more about the project, check the
  project hackster at :
  https://www.hackster.io/noahmaupu/system-of-monitoring-for-hive-05767d

  modified 11 january 2022
  by Raimbaud Quentin

  This code is in the public domain.

  Check the project github at :
  https://github.com/noah-maupu/projet_hive
*/

/* Necessary include for MaximWire
   to work, because the Arduino
   nano 33 BLE has pull-up 
   resistor on its different ports. */
#define MAXIMWIRE_EXTERNAL_PULLUP

/* Including libraries.
   All credits in the
   README.md from Github. */
#include <DHT.h>
#include <HX711.h>
#include <MaximWire.h>

/*~~ Sensors measurement variables declaration ~~*/
/*~~~*/ float weight = 0;                   /*~~~*/
/*~~~*/ float temperatureIn1 = 0;           /*~~~*/
/*~~~*/ float temperatureIn2 = 0;           /*~~~*/
/*~~~*/ float temperatureIn3 = 0;           /*~~~*/
/*~~~*/ float temperatureOut = 0;           /*~~~*/
/*~~~*/ float humidityIn = 0;               /*~~~*/
/*~~~*/ float humidityOut = 0;              /*~~~*/
/*~~~*/ unsigned int batteryState = 0;      /*~~~*/
/*~~~*/ unsigned int state = 0;             /*~~~*/
/*~~~*/ unsigned int orientation = 0;       /*~~~*/
/*~~~*/ unsigned int luminosity = 0;        /*~~~*/
/*~~~*/ unsigned int sunshineHours = 0;     /*~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~ Sigfox antenna's payload structure ~~*/
/*~~~*/ int payload[3];              /*~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~ DS12B20 Grove temperature sensors objects ~~~~~~~~~~~~~~*/
/*~~~*/ MaximWire::Bus busSondeTemperatureIn1(D2);                  /*~~~*/
/*~~~*/ MaximWire::Bus busSondeTemperatureIn2(D3);                  /*~~~*/
/*~~~*/ MaximWire::Bus busSondeTemperatureIn3(D4);                  /*~~~*/
/*~~~*/ MaximWire::DS18B20 sondeTemperatureIn1("28316B75D0013C4C"); /*~~~*/ 
/*~~~*/ MaximWire::DS18B20 sondeTemperatureIn2("28628275D0013C12"); /*~~~*/ 
/*~~~*/ MaximWire::DS18B20 sondeTemperatureIn3("28707475D0013C1D"); /*~~~*/ 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~ DHT22 temperature&humidity sensors objects ~~~~~~*/
/*~~~*/ DHT dhtTemperatureAndHumidityOut(D12, DHT22); /*~~~*/ 
/*~~~*/ DHT dhtHumidityIn(D5, DHT22);                 /*~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~ DHT22 weight sensors objects ~*/
/*~~~*/ HX711 x711Weight;    /*~~~*/ 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~ pins used for the battery and    ~~~~~*/
/*~~~~~ luminosity  measurement, and to  ~~~~~*/
/*~~~~~ turn off the tpl output voltage  ~~~~~*/
/*~~~*/ const int batteryStatePin = A6;  /*~~~*/
/*~~~*/ const int luminosityPin = A5;    /*~~~*/
/*~~~*/ const int tplPin = D11;          /*~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**
 * Get the temperature from the "sondeTemperatureIn1", 
 * "sondeTemperatureIn2" and "sondeTemperatureIn3" DS18B20 object.
 * 
 * This measurement use the "MaximWire" library's functions "GetTemperature".
 */
void getTemperaturesIn();

/**
 * Get the temperature and the dumidity from 
 * the "dhtTemperatureAndHumidityOut" DHT object.
 * 
 * This measurement use the "DHT" library's 
 * functions "readTemperature" and readHumidity.
 */
void getTemperatureAndHumidityOut();

/**
 * Get the humidity from the "dhtHumidityIn" DHT object.
 * 
 * This measurement use the "DHT" library's functions "readHumidity".
 */
void getHumidityIn();

/**
 * Get the battery state.
 * 
 * This measurement use the "Arduino" library's 
 * functions "analogRead" with the parameter "batteryStatePin".
 */
void getBatteryState();

/**
 * Get the luminosity.
 * 
 * This measurement use the "Arduino" library's 
 * functions "analogRead" with the parameter "luminosityPin".
 */
void getLuminosity();

/**
 * Get the weight from the "x711Weight" HX711 object.
 * 
 * This measurement use the "HX711" library's 
 * functions "get_units" with the parameter 10.
 */
void getWeight();

/**
 * Use the sensors measurement variables to create the payload.
 * 
 * this function use a combination of bits shift and logical "and"
 * to fill the "payload" array.
 */
void datasToPayload();

/**
 * Use the payload array to send datas through the Sigfox antenna.
 * 
 * This measurement use the "stdio" library's 
 * functions "sprintf" with a buffer and the "payload" array.
 */
void sendPayload();


/**
 * Arduino setup function, only executed once.
 */
void setup()
{
  /* Setup the Serial and Serial1 baud rate */
  Serial.begin(9600); 
  Serial1.begin(9600);

  /* Set the battery, luminosity and tpl pinmodes */
  pinMode(batteryStatePin, INPUT);
  pinMode(luminosityPin, INPUT);
  pinMode(tplPin, OUTPUT);
  
  /* Start the DHT objects */
  dhtTemperatureAndHumidityOut.begin();
  dhtHumidityIn.begin();

  /* Start, scale and shut temporary the HX711 object */
  x711Weight.begin(D6, D7);
  x711Weight.set_scale(21655.5);
  x711Weight.power_down();

  /* Turn off the I2C pull-up and the power LDE */
  digitalWrite(PIN_ENABLE_I2C_PULLUP, HIGH);
  digitalWrite(LED_PWR, LOW);

  /* wait 3 seconds for the setup to be completed */
  delay(3000);
}

/**
 * Arduino loop function, executed in a while(1)
 */
void loop()
{
  /* Get all datas via all sensors measurement */
  getTemperaturesIn();
  getTemperatureAndHumidityOut();
  getHumidityIn();
  getBatteryState();
  getLuminosity();
  getWeight();

  /* Fill the payload with all the datas */
  datasToPayload();

  /* Send the payload through the Sigfox antenna */
  sendPayload();

  /* Wait for the payload to be sent */
  delay(10000);

  /* Turn off the tpl output voltage,
     shutting up the Arduino         */

  digitalWrite(tplPin, HIGH);

  /* Wait for a very long time,
     should never be reached   */
  delay(0xFFFFFFFF);
}

/**
 * getTemperaturesIn
 */
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

/**
 * getTemperatureAndHumidityOut
 */
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

/**
 * getWeight
 */
void getWeight()
{
  x711Weight.power_up();
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

/**
 * getHumidityIn
 */
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
      temperatureIn2 = localTemperatureIn + (float)random(-10, 10)/10;
      temperatureIn3 = localTemperatureIn + (float)random(-10, 10)/10;
    }
    else
      Serial.println("FAILURE <== Température intérieure : failed !");
  }  
}

/**
 * getBatteryState
 */
void getBatteryState()
{
  float rawBatteryState = analogRead(batteryStatePin);
  float Vin = 3.3*(rawBatteryState/1024.0);
  float Vbat = Vin*1.303241442;
  int localBatteryState = (((Vbat-3.3)*139.86013986 + 0.5)*100/127);
  Serial.print("(Valeur non bordée : ");
  Serial.print(localBatteryState);
  Serial.print(") Valeur bordée : ");
  localBatteryState = max(min(localBatteryState, 100), 0);
  Serial.print(localBatteryState);
  Serial.println(" <== Etat de la batterie : success !");
  batteryState = localBatteryState;
}

/**
 * getLuminosity
 */
void getLuminosity()
{
  float localLuminosity = analogRead(luminosityPin);
  Serial.print(localLuminosity);
  Serial.println(" <== Luminosite : success !");
  luminosity = (494 - localLuminosity)*100/494;
}

/**
 * datasToPayload
 */
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

/**
 * sendPayload
 */
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
