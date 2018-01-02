#!/bin/bash
echo "Input location to get weather information (Format: CityName,CountryCode) ex:Istanbul,TR"
read location
wget -q -O - "api.openweathermap.org/data/2.5/forecast?q=$location&APPID=53470a19de3d7a3a8e8c5b1421d17d81" > forecast.json
python parseWeather.py
rm forecast.json

