# espotify

### Description
View your Spotify's song info on the SSD1306 using an ESP32 board

Special thanks to [fbiego](https://github.com/fbiego) for his [original repo](https://github.com/fbiego/esp32-spotify-oled). This is just a rewrite using a different library to suit my needs.

All Contributions are welcome!

### Required Libraries
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [HTTPClient](https://github.com/amcewen/HttpClient)
- [Thingpulse's OLED library](https://github.com/ThingPulse/esp8266-oled-ssd1306)

## How to setup
### Spotify
- Visit Spotify Dashboard [`https://developer.spotify.com/dashboard/login`](https://developer.spotify.com/dashboard/login)
- [`Create an app`](https://developer.spotify.com/documentation/general/guides/authorization/app-settings/)
- Add Redirect URI http://httpbin.org/anything & Save in `Edit Settings`
	![[Pasted image 20220619001608.png]]


### How to authenticate application
- Open Chrome Developer Tools `Ctrl+Shift+I`, head to the networks tab and hit `Ctrl+r`
- On the link below, add your clientID (replace and remove the curly brackets)
	```https://accounts.spotify.com/authorize?response_type=token&redirect_uri=https%3A%2F%2Fhttpbin.org%2Fanything&client_id={CLIENTID}&scope=user-read-playback-state+user-read-playback-position+user-modify-playback-state&state=cryq3```
- Hit enter and it should redirect to the specified URI with and Access Token on the URL
- In Chrome's Developer tools window, you should see something like this
	![[Pasted image 20220618235517.png]]
- Click on the one with authorize, click on headers, scroll down until you see `cookie:`, right click and copy value
	![[Pasted image 20220618235859.png]]
	
	
### How to use
- clone this repository
	```https://github.com/itami12354/espotify.git```
- Use [platform.io](https://platformio.org/) to open the project
- In `platformio.ini`, configure the upload port (default is COM4)
- Configure the settings in `src/main.cpp`
- Replace all the required fields (dont delete the "")
	![[Pasted image 20220619000924.png]]
	
	- Get your clientID on your Spotify app's dashboard
	- Note: SSID is the name of your Wifi Network
	
- Upload to your ESP32 and Done!

Feel free to contribute if you can improve on it

#### Known issues
- Title/Artist/Artist which is in another language (e.g  Japanese) doesn't display anything
- Long Title/Artist/Artist goes out of the displat (idk how to make it scroll)