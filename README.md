# Autonomous outside weather station

Build a complete autonomous outside weather station that can stay for years without battery change in your garden.
The station communicate the weather data to your home server OR a web server.
You can at any time access the graphs and last sensors values, from a web interface (smartphone compatible).

TODO : put a picture here

# Elements

Here is the detailed list of logical elements that compose the station.

## Station 

## Main card with Wifi communication

## Power module (Solar powered battery with level  via I2C)

This module provide 4 outputs, 2 for power (+3.7V/GND) and 2 for battery level (I2C).
The battery is continuously charged by solar panel, with surcharge protection.
This module is autonomous and can be reuse in some other ESP8266 project.

[Detailed infos (Build instructions, components, sample code)](./elements/power_module/README.md)

## Rain counter module (Autonomous + low power consumption)  

This module provide a 5 pin interface: 2 for power (+3.7V/GND), 1 for waking up the module, 2 for communication (I2C)
This module should count the number of millimeter of rain. 
With a low power and complete autonomy even if main card is in sleep mode.

[Detailed infos (Build instructions, components, sample code)](./elements/rain_counter_module/README.md)

## Temperature sensor

## Atmospheric Pressure

## Sun sensor (Light level sensor)

## Web Interface (Running on a home server)

# Build instructions

## Complete list of required materials

## Complete wire schemes (with all elements together)

## Final programs
