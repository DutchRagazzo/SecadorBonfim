// Definição dos pinos
const int pinSensor = 2;   // Pino do sensor TCRT 5000
const int pinLedRed = 3;   // Pino do LED vermelho (motor)
const int pinLedGreen = 13; // Pino do LED verde (indicador de tempo)

// Variáveis de controle
unsigned long startTime = 0;  // Armazena o tempo de início do motor
bool motorActive = false;     // Indica se o motor está acionado

void setup() {
  // Inicializa os pinos
  pinMode(pinSensor, INPUT);   // Pino do sensor como entrada
  pinMode(pinLedRed, OUTPUT);  // Pino do LED vermelho como saída
  pinMode(pinLedGreen, OUTPUT);// Pino do LED verde como saída

  // Inicializa os LEDs como desligados
  digitalWrite(pinLedRed, LOW);
  digitalWrite(pinLedGreen, LOW);

  Serial.begin(9600);  // Inicia a comunicação serial (opcional, para depuração)
}

void loop() {
  int sensorValue = digitalRead(pinSensor);  // Lê o valor do sensor

  // Se o sensor detectar movimento (valor baixo)
  if (sensorValue == LOW && !motorActive) {
    Serial.println("Movimento Detectado");

    // Ativa o motor (LED vermelho)
    digitalWrite(pinLedRed, HIGH);
    motorActive = true;
    startTime = millis();  // Registra o tempo de início do motor

    // Loop enquanto o motor estiver ligado
    while (motorActive) {
      unsigned long elapsedTime = millis() - startTime; // Tempo desde que o motor foi ligado

      // Calcula o tempo restante
      unsigned long remainingTime = 10000 - elapsedTime;

      // Se o tempo restante for maior que 5 segundos, o LED verde pisca lentamente
      if (remainingTime > 5000) {
        // Pisca o LED verde a cada 1 segundo
        digitalWrite(pinLedGreen, HIGH);
        delay(1000);
        digitalWrite(pinLedGreen, LOW);
        delay(1000);
      } 
      // Se o tempo restante for menor que ou igual a 5 segundos, o LED verde pisca mais rápido
      else {
        // Pisca o LED verde a cada 500 ms
        digitalWrite(pinLedGreen, HIGH);
        delay(500);
        digitalWrite(pinLedGreen, LOW);
        delay(500);
      }

      // Se o tempo de 10 segundos foi atingido
      if (remainingTime <= 0) {
        // Desliga o motor (LED vermelho)
        digitalWrite(pinLedRed, LOW);
        motorActive = false;  // Desativa o motor
        digitalWrite(pinLedGreen, LOW); // Desliga o LED verde
        Serial.println("Tempo finalizado!");
      }

      // Verifica se a mão foi retirada (sensor para de detectar movimento)
      if (sensorValue == HIGH) {
        // Desliga o motor imediatamente (ambos os LEDs desligados)
        digitalWrite(pinLedRed, LOW);
        digitalWrite(pinLedGreen, LOW);
        motorActive = false;  // Desativa o motor
        Serial.println("Mão retirada, secador desligado.");
        break;  // Sai do loop
      }

      // Atualiza a leitura do sensor para verificar se a mão foi retirada
      sensorValue = digitalRead(pinSensor);
    }
  } else {
    // Caso não haja movimento, o motor está desligado e o LED verde está apagado
    digitalWrite(pinLedGreen, LOW);
    digitalWrite(pinLedRed, LOW);
  }

  delay(3000);  // Pequeno atraso para evitar leituras excessivas
}
