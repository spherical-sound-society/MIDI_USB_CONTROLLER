void buttonsManager() {
  LeftButton.read();
  RightButton.read();
  AButton.read();
  BButton.read();
  UserButton.read();

  static bool BButtonLongPressActionTaken = false;
  static bool AButtonLongPressActionTaken = false;

  if (UserButton.isReleased()) {
    //FUNCTION BUTTON PRESSED
    //Serial.print("pressed");
    //bank2();
    if (BButton.wasReleased()) {

      // rp2040.fifo.push(REFRESH_LENGTH);
      // rp2040.fifo.push(subSequenceLong[subsequenceIndex]);
    }
    if (AButton.wasReleased()) {
      Serial.println("PERCUSEQ");
    }
    if (RightButton.wasReleased()) {
      //Serial.println("RightButton was released");
    }
    if (LeftButton.wasReleased()) {
    }
  }
  //REGULAR BUTTON PRESSES

  if (RightButton.wasReleased()) {
    potBank = !potBank;
    digitalWrite(LED_A_Pin, potBank);
  }
  if (LeftButton.wasReleased()) {
   // isEditMode = !isEditMode;
  }

  if (AButton.pressedFor(400)) {
    if (!AButtonLongPressActionTaken) {
     // snapshotLiveBank();
      //ledBlinker(LED_B_Pin, 4);
     // ledBlinker4.trigger();
     // AButtonLongPressActionTaken = true;
    }
  }
  if (AButton.wasReleased()) {
    // Serial.println("B Button was released");
    if (AButtonLongPressActionTaken) {
      // Serial.println("long press.");
      AButtonLongPressActionTaken = false;
    } else {
      // Serial.println("short press.");
      // loadBank(byte bankno);
      //isAutoModeFlag=1;
      //ledBlinker(LED_B_Pin, 2);
     // ledBlinker1.trigger();
     // retrieveSnapshot();
    }
  }


  if (BButton.pressedFor(300)) {
    if (!BButtonLongPressActionTaken) {
      //Serial.println("Long press activated");
     // isAutoMode = 1;
     // BButtonLongPressActionTaken = true;
    }
  }
  if (BButton.wasReleased()) {
    // Serial.println("B Button was released");
    if (BButtonLongPressActionTaken) {
      // Serial.println("long press.");
      BButtonLongPressActionTaken = false;
    } else {
      // Serial.println("short press.");
     // isAutoMode = 0;
      //isAutoModeFlag=1;
    }
  }
}

#define NUM_SAMPLES 16  // Número de lecturas para promediar

void readPots(byte bank) {
  static byte bankState = 0;
  static uint16_t lastMappedPot[2][8] = { { 0 } };  // Buffer valores mapeados
  static uint16_t lastRawPot[2][8] = { { 0 } };     // Buffer valores raw (nuevo)

  if (bank != bankState) {
    // 1. Guardar estado actual antes de cambiar
    for (int k = 0; k < 8; k++) {
      lastMappedPot[bankState][k] = mappedPot[bankState * 8 + k];
      lastRawPot[bankState][k] = pot[bankState * 8 + k];  // Guardar raw
    }

    // 2. Cargar nuevo banco (ambos valores)
    for (int k = 0; k < 8; k++) {
      mappedPot[bank * 8 + k] = lastMappedPot[bank][k];
      pot[bank * 8 + k] = lastRawPot[bank][k];    // Cargar raw guardado
      lastPot[bank * 8 + k] = pot[bank * 8 + k];  // Sincronizar lastPot
    }

    bankState = bank;

    // 3. Resetear detección de movimiento
    for (int j = 0; j < 8; j++) {
      int potIndex = bank * 8 + j;
      pot[potIndex] = analogReadMuxAvg(j);  // Lectura física actual
      lastPot[potIndex] = pot[potIndex];    // Forzar igualdad
    }
    return;  // Saltar primera lectura tras cambio
  }

  // Lectura normal (solo si no hubo cambio de banco)
  for (int j = 0; j < 8; j++) {
    int potIndex = bank * 8 + j;

    lastPot[potIndex] = pot[potIndex];
    pot[potIndex] = softDebounce(analogReadMuxAvg(j), pot[potIndex]);

    if (pot[potIndex] != lastPot[potIndex]) {
      mappedPot[potIndex] = map(pot[potIndex], 0, 4095,
                                boundaries[isEditMode][potIndex][0],
                                boundaries[isEditMode][potIndex][1]);
      // Actualizar buffers
      lastMappedPot[bank][j] = mappedPot[potIndex];
      lastRawPot[bank][j] = pot[potIndex];

      Serial.print("B");
      Serial.print(bank);
      Serial.print(" Pot");
      Serial.print(j);
      Serial.print(": ");
      Serial.println(mappedPot[potIndex]);
      changeParam(j + bank * 8, mappedPot[potIndex]);
    }
  }
}

// works perfect, dont modify
int analogReadMuxAvg(byte chan) {
  digitalWrite(MUX_A_Pin, bitRead(chan, 0));
  digitalWrite(MUX_B_Pin, bitRead(chan, 1));
  digitalWrite(MUX_C_Pin, bitRead(chan, 2));

  int sum = 0;
  for (byte j = 0; j < NUM_SAMPLES; j++) {
    sum += analogRead(POT_Pin);
    //delayMicroseconds(1);  // Pequeño delay entre muestras
  }
  return sum / NUM_SAMPLES;  // Devolver promedio
}

// works perfect, dont modify
int softDebounce(int readCV, int oldRead) {
  return (abs(readCV - oldRead) > debounceRange) ? readCV : oldRead;
}