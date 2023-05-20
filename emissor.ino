#define TX_PIN 13
#define RTS_PIN 12
#define PINO_CTS 11
#define BAUD_RATE 1
#define HALF_BAUD 1 / (2 * BAUD_RATE)

void configuraTemporizador(int baud_rate)
{
    int frequencia;
    frequencia = constrain(baud_rate, 1, 1500);
    // set timer1 interrupt
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1 = 0;  // initialize counter value to 0
    // OCR1A = contador;// = (16*10^6) / (10*1024) - 1 (must be <65536)
    OCR1A = ((16 * pow(10, 6)) / (1024 * frequencia)) - 1;
    // turn on CTC mode (clear time on compare)
    TCCR1B |= (1 << WGM12);
    // Turn T1 off
    TCCR1B &= 0xF8;
    // Disable interrupts
    TIMSK1 = 0;
    TIFR1 = 0;
}

void startTimer()
{
    Serial.println("T1 iniciado");
    TCNT1 = 0; // initialize counter value to 0
    TIFR1 = 0;
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void stopTimer()
{
    Serial.println("T1 parado");
    // Turn T1 off
    TCCR1B &= 0xF8;
    TIMSK1 = 0;
}

// Get parity bit
bool getParityBit(char c){
  int parity = 0;
  int numOnes = 0;

  // Count the number of ones in the character's bits
  for (int i = 0; i < 8; i++) {
    if ((c >> i) & 1) {
      numOnes++;
    }
  }

  // Calculate the parity bit based on the number of ones
  if (numOnes % 2 == 0) {
    return 0;  // Even parity
  } else {
    return 1;  // Odd parity
  }

  return parity;
}


///////// COMEÇA
enum STATES {
    Start = 0,
    Transmitting = 1,
    Finish = 3
};

String message = "";
volatile boolean transmitting = false;
volatile int currentState = 0;
volatile char transmittingChar = 0;
volatile int transmittingCharCurrentBit = 0;

// Rotina de interrupcao do timer1
ISR(TIMER1_COMPA_vect) {
    transmitChar();
}

// Executada uma vez quando o Arduino reseta
void setup()
{
    // desabilita interrupcoes
    noInterrupts();
    // Configura porta serial (Serial Monitor - Ctrl + Shift + M)
    Serial.begin(9600);
    // Inicializa TX ou RX
    pinMode(TX_PIN, OUTPUT);
    pinMode(RTS_PIN, OUTPUT);
    pinMode(PINO_CTS, INPUT);
    // Configura timer
    configuraTemporizador(BAUD_RATE);
    // habilita interrupcoes
    interrupts();
}


void transmitChar() {
    switch (currentState) {
        case Start:
            startTimer();

            Serial.print("Transmiting char: ");
            Serial.println(transmittingChar);

            digitalWrite(TX_PIN, LOW);
            
            Serial.println("0");

            currentState++;  // Move to next state (Data transmission)

            return;

        case Transmitting:
            // Get the next bit to be transmitted
            boolean bit = bitRead(transmittingChar, transmittingCharCurrentBit);

            // Transmit bit
            digitalWrite(TX_PIN, bit);
            Serial.println(bit);

            transmittingCharCurrentBit++;

            // When we finish transmitting our current char, transmit a parity bit
            if (transmittingCharCurrentBit == 7)
                digitalWrite(TX_PIN, getParityBit(transmittingChar));
                currentState++;

            return;

        case Finish:
        {
            transmittingCharCurrentBit = 0;
            currentState = 0;
            transmitting = false;
            return;
        }
    }
}

void loop() {
    // O Emissor espera um novo dado no monitor serial
    Serial.println("Type a message to be transmitted:");
    while (!Serial.available()) {
      // Wait for a message to be typed
    }

    message = Serial.readString();
    Serial.println(message);

    for (int i = 0; i < message.length() - 1; i++) {
        transmittingChar = message.charAt(i);

        // Indicate the intetion to transmit through RTS
        digitalWrite(RTS_PIN, HIGH);

        while (digitalRead(PINO_CTS) == LOW) {
          // Wait for Cleat-to-Send signal from receiver
        }

        transmitChar();

        while (transmitting) {
          // Wait while transmitting
        }
        
        stopTimer();
        digitalWrite(RTS_PIN, LOW);
    }

    Serial.println("Successfully sent message!");
}