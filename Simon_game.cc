#include <Arduino.h>

// Define os pinos dos botões e dos LEDs.
const int buttonPins[] = {15, 17, 5, 19};
const int ledPins[] = {33, 32, 27, 14};
// Pino do botão de reiniciar.
const int resetButtonPin = 18;
// Número total de botões/LEDs.
const int numButtons = 4;
// Array para armazenar a sequência de sinais que o jogador deve reproduzir.
int sequence[200];
// Comprimento inicial da sequência de sinais.
int sequenceLength = 1;
// Passo atual do jogador na sequência.
int currentStep = 0;

void setup() {
  Serial.begin(9600);
  // Configura os pinos dos botões como entrada com resistores de pull-up e os pinos dos LEDs como saída.
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  // Configura o pino do botão de reiniciar com resistência de pull-up.
  pinMode(resetButtonPin, INPUT_PULLUP);
  // Inicializa a geração de números aleatórios.
  randomSeed(analogRead(0));
  // Reinicia o jogo para o estado inicial.
  resetGame();
}

void loop() {
  // Verifica se o botão de reinício foi pressionado.
  if (digitalRead(resetButtonPin) == LOW) {
    delay(50); // Debounce para evitar leituras falsas.
    while (digitalRead(resetButtonPin) == LOW); // Espera até que o botão seja solto.
    resetGame(); // Reinicia o jogo.
    return; // Retorna para começar o jogo do início.
  }

  // Itera pelos botões para verificar se algum foi pressionado.
  for (int i = 0; i < numButtons; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      delay(50); // Debounce para evitar leituras falsas.
      while (digitalRead(buttonPins[i]) == LOW); // Espera até que o botão seja solto.
      
      Serial.print("sequence ");
      Serial.println(sequence[currentStep]);
      
      Serial.print("i; ");
      Serial.println(i);



      // Se o botão correto foi pressionado, avança na sequência.
      if (i == sequence[currentStep]) {
        currentStep++;
        // Se a sequência atual foi completada, prepara a próxima.
        if (currentStep == sequenceLength) {
          sequenceLength++;
          currentStep = 0;
          delay(1000); // Espera antes de começar a próxima sequência.
          playSequence(); // Inicia a próxima sequência.
        }
      } else {
        gameOver(); // Encerra o jogo se o botão errado foi pressionado.
        return; // Retorna para reiniciar o jogo.
      }
    }
  }
}

// Função para exibir a sequência de sinais usando os LEDs.
void playSequence() {
  for (int i = 0; i < sequenceLength; i++) {
    digitalWrite(ledPins[sequence[i]], HIGH); // Acende o LED correspondente.
    delay(500); // Mantém o LED aceso por um tempo.
    digitalWrite(ledPins[sequence[i]], LOW); // Desliga o LED.
    delay(250); // Intervalo entre os sinais.
  }
}

// Função chamada quando o jogador perde o jogo.
void gameOver() {
  // Piscar todos os LEDs simultaneamente para indicar o fim do jogo.
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < numButtons; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(200);
    for (int j = 0; j < numButtons; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(200);
  }
  // Reinicia o jogo após o fim do jogo.
  resetGame();
}




// Reinicia o jogo para o estado inicial.
void resetGame() {
  // Desliga todos os LEDs antes de começar uma nova sequência.
  for (int i = 0; i < numButtons; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  // Reseta a sequência para o início.
  sequenceLength = 1;
  currentStep = 0;
  // Preenche o array da sequência com valores aleatórios.
  for (int i = 0; i < 100; i++) {
    sequence[i] = random(0, numButtons);
  }
  delay(1000); // Espera antes de iniciar a nova sequência.
  playSequence(); // Começa a exibir a primeira sequência de sinais.
}
