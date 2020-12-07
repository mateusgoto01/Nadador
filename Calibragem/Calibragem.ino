#include <HX711_ADC.h>
#include <EEPROM.h>

//pins:
const int HX711_dout = 5; //mcu > HX711 dout pin
const int HX711_sck = 4; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
long t;

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Iniciando...");

  LoadCell.begin();
  long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = false; //set this to false if you don't want tare to be performed in the next step
  LoadCell.startMultiple(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, cheque se os pinos e conexões estão certas");
    while (1);
  }
  else {
    LoadCell.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Inicialização terminada!");
  }
  while (!LoadCell.update());
  calibrate(); //start calibration procedure
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      float i = LoadCell.getData();
      Serial.print("Load_cell valor de saída: ");
      Serial.println(i);
      newDataReady = 0;
      t = millis();
    }
  }

  // receive command from serial terminal
  if (Serial.available() > 0) {
    float i;
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay(); //tare
    else if (inByte == 'r') calibrate(); //calibrate
    else if (inByte == 'c') changeSavedCalFactor(); //edit calibration value manually
  }

  // check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tara completa");
  }

}

void calibrate() {
  Serial.println("***");
  Serial.println("Iniciando calibração:");
  Serial.println("Coloque as celulas de carga em uma superficie plana e estável.");
  Serial.println("RETIRE qualquer peso aplicado sobre a celula de carga");
  Serial.println("Aperte 't' se deseja realizar uma tara(Recomendado para realizar a calibração)");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      if (Serial.available() > 0) {
        float i;
        char inByte = Serial.read();
        if (inByte == 't') LoadCell.tareNoDelay();
      }
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tara completa");
      _resume = true;
    }
  }

  Serial.println("Agora, coloque uma massa conhecida encima da estrutura");
  Serial.println("Agora envie no Serial Monitor o valor da força peso dessa massa conhecida em N. Exemplo: 5, para uma carga de prova de 0.5Kg");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("O modulo da força peso é: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

  Serial.print("O VALOR DE CALIGRAÇÃO É: ");
  Serial.print(newCalibrationValue);
  Serial.println(", use esse valor de calibração (calFactor) para o codigo LCD.ino");
  Serial.print("Deseja salvar esse valor no EEPROM? (Não é necessário)");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");

  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Valor ");
        Serial.print(newCalibrationValue);
        Serial.print(" Salvado no EEPROM : ");
        Serial.println(calVal_eepromAdress);
        _resume = true;

      }
      else if (inByte == 'n') {
        Serial.println("Valor não salvo em EEPROM");
        _resume = true;
      }
    }
  }

  Serial.println("Fim da calibração");
  Serial.println("***");
  Serial.println("Se deseja recalibrar, digite 'r' no Serial Monitor");
  Serial.println("Para definição manual do valor de calibração, digite 'c' no serial monitor");
  Serial.println("***");
}

void changeSavedCalFactor() {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println("***");
  Serial.print("O Valor atual é: ");
  Serial.println(oldCalibrationValue);
  Serial.println("Agora, envite o novo valor de calibração no serial monitor, i.e. 696.0");
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print("O Novo valor de calibração é: ");
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  _resume = false;
  Serial.print("Deseja salvar esse valor no EEPROOM (Não é necessário)");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Valor ");
        Serial.print(newCalibrationValue);
        Serial.print(" Salvado no EEPROOM: ");
        Serial.println(calVal_eepromAdress);
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println("Valor não salvo no EEPROOM: ");
        _resume = true;
      }
    }
  }
  Serial.println("Fim da mudança de valor de calibração");
  Serial.println("***");
}
