import json
from pprint import pprint

def toDegree(kelvin):
    return "{0:.2f}".format(float(kelvin) - 272.15)

data = json.load(open('forecast.json'))

print("City: {0}".format(data["city"]["name"]))

print("Forecast for upcoming 5 days\n------------------------")
for dt in data["list"]:
    print("Date: {0}".format(dt["dt_txt"])[:-8])
    print("Time: " + dt["dt_txt"][11:])
    print("Temperatue: {0} Degrees".format(toDegree(dt["main"]["temp"])))
    print("Weather is expected to have {0}\n".format(dt["weather"][0]["description"]))

