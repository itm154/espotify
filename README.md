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
  ![1](https://user-images.githubusercontent.com/67090166/174447856-40982c11-f922-4e62-bbbd-740d192de174.png)

### How to authenticate application

- Open Chrome Developer Tools `Ctrl+Shift+I`, head to the networks tab and hit `Ctrl+R`
- On the link below, add your clientID (replace and remove the curly brackets)
  `https://accounts.spotify.com/authorize?response_type=token&redirect_uri=https%3A%2F%2Fhttpbin.org%2Fanything&client_id={CLIENTID}&scope=user-read-playback-state+user-read-playback-position+user-modify-playback-state&state=cryq3`
- Hit enter and it should redirect to the specified URI with and Access Token on the URL
- In Chrome's Developer tools window, you should see something like this
  ![2](https://user-images.githubusercontent.com/67090166/174447880-2312405c-2363-4c65-b9a3-9152224f7cb0.png)

- Click on the one with authorize, click on headers, scroll down until you see `cookie:`, right click and copy value
  <img width="594" alt="3" src="https://user-images.githubusercontent.com/67090166/174447921-b421e2f6-462c-404e-a4aa-1d472e3e190c.png">


### How to use

- Clone this repository
  `https://github.com/itami12354/espotify.git`
- Use [platform.io](https://platformio.org/) to open the project
- In `platformio.ini`, configure the upload port (default is `COM4`)
- Configure the settings in `src/main.cpp`
- Replace all the required fields (dont delete the "")
  ![4](https://user-images.githubusercontent.com/67090166/174447925-84baac5b-6a37-4758-8d4c-dd93a411cb7f.png)


  - Get your clientID on your Spotify app's dashboard
  - Note: SSID is the name of your Wifi Network

- Upload to your ESP32 and Done!

Feel free to contribute if you can improve on it

#### Known issues

- Title/Artist/Artist which is in another language (e.g Japanese) doesn't display anything
- Long Title/Artist/Artist goes out of the displat (idk how to make it scroll)
