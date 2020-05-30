# AURALIGHT
Auralight is a ESP32-based wireless ambient lighting system. It sends screen pixel data to a conntected
WS2812B LED strip placed behind the monitor and cast light on the wall. I wrote a screen capturing program using Pytorch tensors to
accelerate computations, and both the server and the client web interface on the ESP32.

## Introduction
Auralight is a cost-effective solution to extend your entertainment to a next
level. It extends the edges of your screen to the surroundings by casting
ambient lights on the wall, thus bringing you into the moment. Built on ESP32,
it provides a web interface for an easier control. The asynchronous design in
coding provides low latency response. It can be used when you’re watching
movies, playing video games or even reading books.

Auralight provides two modes, the first one is ‘extension’ mode. It samples the
edges of your screen and create an interpolated ‘ambient’ version; the second
one is the ‘breathing’ light mode, accompanying your reading journey or lighting
up the room even the computer is not on. The main control web interface
allows user to adjust the brightness scaling when in ‘extension’ mode and to
adjust the color correction tone according to the FastLED API. Apart from
that, user can also adjust the breathing/blinking frequency, the start and end of
breathing/blinking brightness and the color.

This project covers various technologies, including hardware C++ coding using
Arduino IDE, jQuery and Python. I was also glad to study some mathematics
during my research of computer vision.

## Video demo
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/IZyTaM33Dow/0.jpg)](https://www.youtube.com/watch?v=IZyTaM33Dow)

## Web interface
![0](https://raw.githubusercontent.com/thetobysiu/auralight/master/img/0.png)

![1](https://raw.githubusercontent.com/thetobysiu/auralight/master/img/1.png)

![2](https://raw.githubusercontent.com/thetobysiu/auralight/master/img/2.png)

## Requirement
### Software
Windows 7+ (I use a screen capturing library called D3Dshot that uses that windows desktop api)

Python 3.6+
### Hardware
ESP32

WS2812B

External power adapter (depends on how many leds you have)

## Usage
Edit the project.ino inside arduino ide code folder, add your wireless ssid name and password in the file.

Upload the arduino code to ESP32

Create a virtual env in your PC environment, I suggest python 3.7.

Then pip install requirements.txt

Edit run.bat, add the virtualenv name after the first line:
``
call activate [your name here]
``

Edit the IP address in main.py, replace the ip with your ESP32 IP address in your local network.

Run run.bat every time you use it.

Feel free to drop a message if you have questions.

## Report
Some details might not be covered in this readme, so you could look at the full report.
It can be downloaded [here](https://github.com/thetobysiu/auralight/raw/master/report.pdf).