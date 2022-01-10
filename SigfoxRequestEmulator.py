import requests
import json

BASE_URL = "http://localhost:7071/api/HttpTrigger1"
DEVICE = "C556E6"

def datasToPayload(weight, temperatureIn1, temperatureIn2, temperatureIn3, temperatureOut, humidityIn, humidityOut, batteryState, state, orientation, luminosity, sunshineHours) :
	return hex((weight*10 << 85) | (temperatureIn1*10 << 76) | (temperatureIn2*10 << 67) | (temperatureIn3*10 << 58) | ((temperatureOut + 30)*10 << 48) | (int(humidityIn/2) << 42) | (int(humidityOut/2) << 36) | (batteryState << 29) | (state << 25) | (orientation << 22) | (luminosity << 6) | (sunshineHours << 1) | 1)[2:]

def main() :

	weight = 12
	temperatureIn1 = 11
	temperatureIn2 = 10
	temperatureIn3 = 9
	temperatureOut = 8
	humidityIn = 7
	humidityOut = 6
	batteryState = 5
	state = 4 
	orientation = 3
	luminosity = 2
	sunshineHours = 1

	payload = datasToPayload(weight, temperatureIn1, temperatureIn2, temperatureIn3, temperatureOut, humidityIn, humidityOut, batteryState, state, orientation, luminosity, sunshineHours)
	url = BASE_URL + "?payload=" + payload + "&device=" + DEVICE

	headers = {
                "content-length"  : '0',
                "accept-language" : "fr",
                "host"            : "quentinredirecttest.azurewebsites.net",
                "content-type"    : "application/json",
                "accept-encoding" : "gzip,deflate",
                "accept-charset"  : "UTF-8;q=0.9,*;q=0.7",
                "user-agent"      : "SIGFOX"
              }

	emptyData = {}

	try :
		requests.post(url, data=json.dumps(emptyData), headers=headers)
		print("Message sent")
	except requests.exceptions.ConnectionError as e:
		print(e)



if __name__ == '__main__' :
	main()