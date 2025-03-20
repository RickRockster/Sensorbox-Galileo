#include "Adafruit_VL53L0X.h"
#include <phyphoxBle.h>
#include "max6675.h"

//Pins von: CLK, CS, SO
MAX6675 ktc(8, 9, 7);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//Um die Debug Ausgaben zu aktivieren
//muss dieser Wert auf "true" gesetzt werden.
#define debugSensor false

//button
  const int buttonPin = A7;
  int menuState = 1;
  float menuVal;
  bool buttonPressed = false;
  const int debounceTime = 200; 
  unsigned long lastPressTime = 0;

//laser
  //Es werden 10 Messungen durchgeführt.
  const int MAX_DATA = 10;
  //der Index der aktuellen Messung
  int readDataIndex = -1;
  //das Array für die Daten
  int data[MAX_DATA] = {};
  float averagelaser;

  //zählen der fehlerhaften Messungen
  int failureMeasures = 0;
  bool sensorstate = true;
  const int red = 6;
  const int green = 3;
  const int button = 2;
  int buttonread;
  int lastButtonState = false;

//Magnetsensor
  const int magnetpin = A0;
  int state = 0;
  float newstate = 0;
//Photoresistor

  float photostate = 0;
  const int photoresistorPin = A3;

//Temperatursensor

  float temperature;
void setup() {
  //begin der seriellen Kommunikation mit 115200 baud
  Serial.begin(115200);
  Serial.println("TEST");
  pinMode(magnetpin, INPUT);
  pinMode(buttonPin, INPUT);

  Phyphox();
  //Warten auf den Seriellen Port


  //Wenn der Serielle Port bereit ist dann eine Ausgabe auf diesen tätigen
  //und prüfen ob der Sensor korrekt angeschlossen ist (ob dieser Ansprechbar ist)
  Serial.println("GY-VL53L0X test");
  if (!lox.begin()) {
    Serial.println("Fehler beim lesen des Sensors!");
    while (1)
      ;  //eine Endlos Schleife
  }
}

void loop() {

  SensorstateSwitch();

  PhyphoxBLE::write(averagelaser, newstate, photostate, temperature, menuVal);
}

void laser() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, debugSensor);

  //lesen des Sensor Status
  //der Sensor kann verschiedene Status annehmen,
  //jedoch interessiert für uns nur der Wert "4"
  int sensorStatus = measure.RangeStatus;
  
  //Wenn Daten empfangen wurden dann...
  if (sensorStatus != VL53L0X_DEVICEERROR_MSRCNOTARGET) {
    //den Zähler für das Array um eins erhöhen
    readDataIndex++;
    //zuweisen des Wertes in das Array
    data[readDataIndex] = measure.RangeMilliMeter;

    //Wenn das Array "fertig" befüllt ist, dann...
    if (readDataIndex == MAX_DATA) {
      //eine Variable für die Berechnung des Durchschnittswertes
      float averageData = 0;
      //über das Array itereieren und die Daten zusammenzählen
      for (int i = 0; i < MAX_DATA; i++) {
        averageData = averageData + data[i];
      }
      //um den Durchschnittswert zu erhalten muss man die Zahl
      //durch die Anzahl der Werte teilen
      averageData = averageData / MAX_DATA;
      //Ausgeben des berechnenen Durchschnitts für die Distanz

      averagelaser = averageData / 10 - 2,5;

      //Serial.println("cm");
      readDataIndex = -1;
      failureMeasures = 0;
    } else if (debugSensor && readDataIndex % 2 == 0) {
      Serial.print("X");
    }
  } else if (debugSensor && sensorStatus != VL53L0X_DEVICEERROR_NONE) {
    Serial.println(" Signal konnte nicht gelesen / verarbeitet werden! ");
  } else {
    if (debugSensor && ++failureMeasures % 2 == 0) {
      Serial.print("-");
    }
  }

  if (debugSensor && (failureMeasures + readDataIndex) > MAX_DATA) {
    Serial.println("");
  }
}

void Phyphox() {
  PhyphoxBLE::start("Galileo");

  PhyphoxBleExperiment AbstandExp;

  AbstandExp.setTitle("Galileo");
  AbstandExp.setCategory("Arduino Experiments");
  AbstandExp.setDescription("Hier drin sind ein paar Experimente der Galileo-Box!");

  PhyphoxBleExperiment::View firstView;
  firstView.setLabel("Abstand");  //Create a "view"
  PhyphoxBleExperiment::View secondView;
  secondView.setLabel("Magnetfeld");
  PhyphoxBleExperiment::View thirdView;
  thirdView.setLabel("Lichtstärke");
  PhyphoxBleExperiment::View fourthView;
  fourthView.setLabel("Temperatur");

  PhyphoxBleExperiment::Graph firstGraph;
  firstGraph.setMinY(0, LAYOUT_FIXED);
  firstGraph.setMaxY(120, LAYOUT_FIXED);
  firstGraph.setLabel("Abstand");
  firstGraph.setUnitX("s");
  firstGraph.setUnitY("cm");
  firstGraph.setLabelX("Zeit");
  firstGraph.setLabelY("Abstand");
  firstGraph.setXPrecision(1);
  firstGraph.setYPrecision(1);

  firstGraph.setChannel(0, 1);

  PhyphoxBleExperiment::Separator LaserSeparator;  //Creates a line to separate elements.
  LaserSeparator.setHeight(2);                     //Sets height of the separator.
  LaserSeparator.setColor("404040");

  PhyphoxBleExperiment::Separator MagnetSeparator;  //Creates a line to separate elements.
  MagnetSeparator.setHeight(2);                     //Sets height of the separator.
  MagnetSeparator.setColor("404040");

  PhyphoxBleExperiment::Separator LichtSeparator;  //Creates a line to separate elements.
  MagnetSeparator.setHeight(2);                    //Sets height of the separator.
  MagnetSeparator.setColor("404040");

  PhyphoxBleExperiment::Separator TempSeparator;
  TempSeparator.setHeight(2);
  TempSeparator.setColor("404040");

  PhyphoxBleExperiment::Separator TextSeparator1;
  TextSeparator1.setHeight(0.5);
  TextSeparator1.setColor("404040");

  PhyphoxBleExperiment::Separator MenuSeparator;
  MenuSeparator.setHeight(2);
  MenuSeparator.setColor("404040");


  PhyphoxBleExperiment::Graph secondGraph;  //Create graph which will plot random numbers over time
  secondGraph.setLabel("Strength");
  secondGraph.setUnitX("s");
  secondGraph.setUnitY("");
  secondGraph.setLabelX("Zeit");
  secondGraph.setLabelY("Stärke");
  secondGraph.setXPrecision(1);  //The amount of digits shown after the decimal point
  secondGraph.setYPrecision(1);

  secondGraph.setChannel(0, 2);

  PhyphoxBleExperiment::Graph thirdGraph;
  thirdGraph.setMinY(0, LAYOUT_FIXED);
  thirdGraph.setLabelX("Zeit");
  thirdGraph.setLabelY("Lichtstärke");
  thirdGraph.setLabel("Lichtstärke");
  thirdGraph.setUnitX("s");
  thirdGraph.setUnitY("");
  thirdGraph.setXPrecision(1);
  thirdGraph.setYPrecision(1);

  thirdGraph.setChannel(0, 3);

  PhyphoxBleExperiment::Graph fourthGraph;
  fourthGraph.setLabelX("Zeit");
  fourthGraph.setLabelY("Temperatur");
  fourthGraph.setLabel("Temperatur");
  fourthGraph.setUnitX("s");
  fourthGraph.setUnitY("°C");
  fourthGraph.setXPrecision(2);
  fourthGraph.setYPrecision(2);

  fourthGraph.setChannel(0, 4);

  PhyphoxBleExperiment::Value LaserValue;  //Creates a value-box.
  LaserValue.setLabel("Abstand:");         //Sets the label
  LaserValue.setPrecision(1);              //The amount of digits shown after the decimal point.
  LaserValue.setUnit("cm");                //The physical unit associated with the displayed value.
  LaserValue.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  LaserValue.setChannel(1);
  LaserValue.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value MagnetValue;  //Creates a value-box.
  MagnetValue.setLabel("Stärke:");          //Sets the label
  MagnetValue.setPrecision(1);              //The amount of digits shown after the decimal point.
  MagnetValue.setUnit("");                  //The physical unit associated with the displayed value.
  MagnetValue.setColor("FFFFFF");           //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  MagnetValue.setChannel(2);
  MagnetValue.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value LichtValue;
  LichtValue.setLabel("Lichtstärke: ");
  LichtValue.setPrecision(1);
  LichtValue.setUnit("");
  LichtValue.setColor("FFFFFF");  //Sets font color. Uses a 6 digit hexadecimal value in "quotation marks".
  LichtValue.setChannel(3);
  LichtValue.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value TempValue;
  TempValue.setLabel("Temperatur: ");
  TempValue.setPrecision(2);
  TempValue.setUnit("°C");
  TempValue.setColor("FFFFFF");
  TempValue.setChannel(4);
  TempValue.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::Value MenuValue;
  MenuValue.setLabel("aktueller Sensor: ");
  MenuValue.setPrecision(0);
  MenuValue.setColor("FFFFFF");
  MenuValue.setChannel(5);
  MenuValue.setXMLAttribute("size=\"2\"");

  PhyphoxBleExperiment::InfoField Sensor1;
  Sensor1.setInfo("1: Abstandsmessung");
  Sensor1.setColor("ffffff");
  Sensor1.setXMLAttribute("size=\"1.2\"");

  PhyphoxBleExperiment::InfoField Sensor2;
  Sensor2.setInfo("2: Magnetfeldmessung");
  Sensor2.setColor("ffffff");
  Sensor2.setXMLAttribute("size=\"1.2\"");

  PhyphoxBleExperiment::InfoField Sensor3;
  Sensor3.setInfo("3: Helligkeitsmessung");
  Sensor3.setColor("ffffff");
  Sensor3.setXMLAttribute("size=\"1.2\"");

  PhyphoxBleExperiment::InfoField Sensor4;
  Sensor4.setInfo("4: Temperaturmessung");
  Sensor4.setColor("ffffff");
  Sensor4.setXMLAttribute("size=\"1.2\"");


  PhyphoxBleExperiment::ExportSet mySet;  //Provides exporting the data to excel etc.
  mySet.setLabel("mySet");
  PhyphoxBleExperiment::ExportData myData1;
  myData1.setLabel("myData1");
  myData1.setDatachannel(1);

  firstView.addElement(firstGraph);
  firstView.addElement(LaserSeparator);  //attach graph to view
  firstView.addElement(LaserValue);
  firstView.addElement(MenuSeparator);
  firstView.addElement(MenuValue);
  firstView.addElement(TextSeparator1);
  firstView.addElement(Sensor1);
  firstView.addElement(TextSeparator1);
  firstView.addElement(Sensor2);
  firstView.addElement(TextSeparator1);
  firstView.addElement(Sensor3);
  firstView.addElement(TextSeparator1);
  firstView.addElement(Sensor4);
  secondView.addElement(secondGraph);
  secondView.addElement(MagnetSeparator);
  secondView.addElement(MagnetValue);
  secondView.addElement(MenuSeparator);
  secondView.addElement(MenuValue);
  secondView.addElement(TextSeparator1);
  secondView.addElement(Sensor1);
  secondView.addElement(TextSeparator1);
  secondView.addElement(Sensor2);
  secondView.addElement(TextSeparator1);
  secondView.addElement(Sensor3);
  secondView.addElement(TextSeparator1);
  secondView.addElement(Sensor4);
  thirdView.addElement(thirdGraph);
  thirdView.addElement(LichtSeparator);
  thirdView.addElement(LichtValue);
  thirdView.addElement(MenuSeparator);
  thirdView.addElement(MenuValue);
  thirdView.addElement(TextSeparator1);
  thirdView.addElement(Sensor1);
  thirdView.addElement(TextSeparator1);
  thirdView.addElement(Sensor2);
  thirdView.addElement(TextSeparator1);
  thirdView.addElement(Sensor3);
  thirdView.addElement(TextSeparator1);
  thirdView.addElement(Sensor4);
  fourthView.addElement(fourthGraph);
  fourthView.addElement(TempSeparator);
  fourthView.addElement(TempValue);
  fourthView.addElement(MenuSeparator);
  fourthView.addElement(MenuValue);
  fourthView.addElement(TextSeparator1);
  fourthView.addElement(Sensor1);
  fourthView.addElement(TextSeparator1);
  fourthView.addElement(Sensor2);
  fourthView.addElement(TextSeparator1);
  fourthView.addElement(Sensor3);
  fourthView.addElement(TextSeparator1);
  fourthView.addElement(Sensor4);

  AbstandExp.addView(firstView);
  AbstandExp.addView(secondView);
  AbstandExp.addView(thirdView);  //attach data to exportSet
  AbstandExp.addView(fourthView);
  AbstandExp.addExportSet(mySet);  //attach exportSet to experiment
  PhyphoxBLE::addExperiment(AbstandExp);

  //Channel 0 = Zeit die Daten werden von Phyphox empfagen
  //Channel 1-N Die Daten die mit PhyphoxBLE::write("float") geschrieben werden
}

void Magnet() {
  state = analogRead(magnetpin);

  newstate = (map(state, 0, 4500, -2000, 2000) + 240);//Wert auf Nullbringen


  delay(100);
}

void Photoresistor() {
  photostate = analogRead(photoresistorPin);
}

void Temperatursensor() {
  temperature = ktc.readCelsius();

  delay(250);
}

void SensorstateSwitch() {


if (analogRead(buttonPin) >= 4090 && !buttonPressed && millis() - lastPressTime > debounceTime) {
  buttonPressed = true;  
  lastPressTime = millis();  
  menuState++;           
  if (menuState > 4) {   
    menuState = 1;
  }
}

if (analogRead(buttonPin) < 4090 && buttonPressed) {
  buttonPressed = false;
}
  Serial.println(menuState);


  switch (menuState) {
    case 1:
      laser();
      //Ergebis der Messung = averagelaser
      // SensorText = "Abstandmessung";
      break;

    case 2:
      Magnet();
      //Ergebnis der Messung = newstate
      break;

    case 3:
      Photoresistor();
      //Ergebnis der Messung = photostate
      break;

    case 4:
      Temperatursensor();
      //Ergebnis der Messung = temperature
      break;
  }



  menuVal = (float)menuState;
}