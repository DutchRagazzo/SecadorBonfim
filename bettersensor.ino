const int pinSensor = 2; // MARROM 5V, AZUL GND E PRETO É A PORTA
const int pinLedRed = 5; // PINO RELÉ
const int pinLedGreen = 13; // PINO LED INDICADOR

bool motorActive = false;
unsigned long startTime = 0;
unsigned long previousBlinkTime = 0;
unsigned long absenceStartTime = 0;
bool monitoringAbsence = false;

int blinkInterval = 1000;
bool ledState = LOW;

void setup() {
  pinMode(pinSensor, INPUT);
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedGreen, OUTPUT);

  digitalWrite(pinLedRed, LOW);
  digitalWrite(pinLedGreen, LOW);

  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(pinSensor);
  unsigned long currentTime = millis();

  if (sensorValue == LOW && !motorActive) {
    // Ativa o motor
    digitalWrite(pinLedRed, HIGH);
    motorActive = true;
    startTime = currentTime;
    Serial.println("Movimento Detectado - motor ligado");
  }

  if (motorActive) {
    unsigned long elapsedTime = currentTime - startTime;

    if (elapsedTime >= 7000) {
      // Desliga o motor após 7 segundos
      digitalWrite(pinLedRed, LOW);
      digitalWrite(pinLedGreen, LOW);
      motorActive = false;
      monitoringAbsence = false;
      Serial.println("Tempo de motor finalizado - aguardando 2 segundos");

      delay(2000); // Aguarda 2 segundos antes de checar novamente

    } else {
      // Piscar LED verde com frequência crescente nos últimos 3 segundos
      unsigned long remainingTime = 7000 - elapsedTime;

      if (remainingTime <= 3000) {
        if (remainingTime > 2000) {
          blinkInterval = 500;
        } else if (remainingTime > 1000) {
          blinkInterval = 250;
        } else {
          blinkInterval = 100;
        }
      } else {
        blinkInterval = 1000;
      }

      // Gerencia o piscar do LED verde
      if (currentTime - previousBlinkTime >= blinkInterval) {
        previousBlinkTime = currentTime;
        ledState = !ledState;
        digitalWrite(pinLedGreen, ledState);
      }

      // Verificação de ausência temporária
      if (sensorValue == HIGH && !monitoringAbsence) {
        absenceStartTime = currentTime;
        monitoringAbsence = true;
      }

      // Se ausência for confirmada por mais de 2 segundos
      if (monitoringAbsence) {
        if (sensorValue == LOW) {
          // Mão voltou antes dos 2 segundos
          monitoringAbsence = false;
        } else if (currentTime - absenceStartTime >= 2000) {
          // Ausência confirmada, desliga motor
          digitalWrite(pinLedRed, LOW);
          digitalWrite(pinLedGreen, LOW);
          motorActive = false;
          monitoringAbsence = false;
          Serial.println("Mão retirada confirmada - motor desligado.");
        }
      }
    }
  } else {
    // Garante que tudo está desligado quando o motor está inativo
    digitalWrite(pinLedRed, LOW);
    digitalWrite(pinLedGreen, LOW);
    monitoringAbsence = false;
  }

  delay(50); // Leitura suave do sensor
}
