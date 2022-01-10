#include <stdlib.h>
#include <stdio.h>

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

void printBinary(int x)
{
    int count = 31;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 4; j++)
  		    printf("%i", x>>(count--) & 1);
}

float weight = 12;
float temperatureIn1 = 11;
float temperatureIn2 = 10;
float temperatureIn3 = 9;
float temperatureOut = 8;
float humidityIn = 7;
float humidityOut = 6;
unsigned int batteryState = 5;
unsigned int state = 4;
unsigned int orientation = 3;
unsigned int luminosity = 2;
unsigned int sunshineHours = 1;

unsigned int payload[3];

void datasToPayload(float weight, float temperatureIn1, float temperatureIn2, float temperatureIn3,
                    float temperatureOut, float humidityIn,  float humidityOut, unsigned int batteryState, 
                    unsigned int state, unsigned int luminosity, unsigned int orientation, unsigned int sunshineHours)
{
  payload[0] = 0;
  payload[1] = 0;
  payload[2] = 0;

  payload[0]  = ((unsigned int) ((min(max(weight, 0), 204.7) + 0.05)*10) << 21);
  payload[0] |= ((unsigned int) ((min(max(temperatureIn1, 0), 51.1) + 0.05)*10) << 12);
  payload[0] |= ((unsigned int) ((min(max(temperatureIn2, 0), 51.1) + 0.05)*10) << 3);
  payload[0] |= ((unsigned int) ((min(max(temperatureIn3, 0), 51.1) + 0.05)*10) >> 6);

  payload[1]  = ((unsigned int) ((min(max(temperatureIn3, 0), 51.1) + 0.05)*10) << 26);
  payload[1] |= ((unsigned int) ((min(max(temperatureOut, -30.0), 72.3) + 30 + 0.05)*10) << 16);
  payload[1] |= ((unsigned int) (humidityIn/2) << 10);
  payload[1] |= ((unsigned int) (humidityOut/2) << 4);
  payload[1] |= (batteryState >> 3);

  payload[2]  = (batteryState << 29);
  payload[2] |= (state << 25);
  payload[2] |= (orientation << 22);
  payload[2] |= ((min(max(luminosity, (unsigned int) 0), (unsigned int) 100000)) << 6);
  payload[2] |= ((min(max(sunshineHours, (unsigned int) 0), (unsigned int) 31)) << 1);
  payload[2] |= 1;
}

int main()
{
	datasToPayload(weight, temperatureIn1, temperatureIn2, temperatureIn3,
	         temperatureOut, humidityIn, humidityOut, batteryState,
	         state, luminosity, orientation, sunshineHours);

	printBinary(*payload);printBinary(*(payload+1));printBinary(*(payload+2));

	return EXIT_SUCCESS;
}