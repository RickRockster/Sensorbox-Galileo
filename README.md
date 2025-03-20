# Sensorbox-Galileo
Nach dem erreichen vom 2. Platz bei Jugend-forscht nun der Code, die 3mf. Dateien und eine Anleitung (folgt noch!).

# Was ist die Sensorbox?
Im Physikunterricht ist Teil des Lehrplans die digitale Messwerterfassung. Die vorhanden Sensoren sind allerdings sehr teuer. Der Galileo ersetzt diese Lösungen für 45€. beigefügt ist eine Liste aller benötigten Sensoren. 

# Materialliste
| Komponent | Funktion/Beschreibung |
| ------------- | ------------- |
| Arduino Nano ESP32 | Mikrocontroller |
| Steckbrett | Zum befestigen der Sensoren *  |
| VL53LXX-V2  | Abstandssensor  |
| PDV-P8104  | Fotowiderstand  |
| KY-035  | Mangetfeldsensor  |
| MAX6675   | Temperatursensor  |
| Jumper-Wires  | Kabel  |
| Filament (PLA)  |  für 3D-Druck   |
| Knopf/Schalter  |  für den Sensorwechsel   |

*Steckbrett ist ggf. austauschbar, falls man es auf einem PCB oder ähnlichem Bauen möchte

# Pinbelegung

| MAX6675-Pin | Arduino Nano-Pin |
|     :---:      |     :---:      |
| SO  | D7  |
| CS  | D9  |
| SCK  | D8  |
| VCC  | 3.3V  |
| GND  | GND  |

<br /> 
<br /> 

| KY-035-Pin  | Arduino Nano-Pin |
|     :---:      |     :---:      |
| GND  | GND  |
| VIN  | 3.3V  |
| S  | A0  |

<br /> 
<br /> 

| Fotowiderstand-Pin  | Arduino Nano-Pin |
|     :---:      |     :---:      |
| PIN 1  | 3.3V |
| PIN 2  | A3  |

zwischen den zweiten Pin des Fotowiderstandes noch einen 1KOhm-Widerstand in GND des Arduinos

Bei dem Fotowiderstand sind beide Pins identisch. Welcher für was verwendet wird ist irrelevant. 

<br /> 
<br /> 

| VL53LXX-V2-Pin  | Arduino Nano-Pin |
|     :---:      |     :---:      |
| VIN  | 3.3V  |
| GND  | GND  |
| SCL  | A5  |
| SDA  | A4  |

<br /> 
<br /> 

| Knopf/Schalter  | Arduino Nano-Pin |
|     :---:      |     :---:      |
| PIN 1 (meist rot)  | 3.3V  |
| PIN 2 (meist schwarz)  | A7  |

Knopf kann nach belieben ausgewählt werden. Bei meinem Gehäuse habe ich folgenden Knopf verwendet:
https://www.amazon.de/dp/B08L49F7DV?th=1
