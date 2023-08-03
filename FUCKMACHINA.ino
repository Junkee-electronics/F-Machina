#define plspin 2
#define dirpin 3
#define endpin 4

int unsigned spd;
int stroke = 32000;
int pos;
bool dir = true;
bool calib;
bool strknow;

const float spdtable[40] = {7.814,4.498,3.265,2.608,2.197,1.916,1.711,1.556,1.436,1.340,1.263,1.201,1.150,1.109,1.076,1.050,1.030,1.015,1.005,1.001,1.001,1.005,1.015,1.030,1.050,1.076,1.109,1.150,1.201,1.263,1.340,1.436,1.556,1.711,1.916,2.197,2.608,3.265,4.498,7.814};

//____________________________________________________________________________________

void setup() {
  pinMode(endpin, INPUT_PULLUP);
  pinMode(plspin, OUTPUT);
  pinMode(dirpin, OUTPUT);
  /*
  //set timer0 interrupt at 500Hz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 500hz increments
  OCR0A = 256;
  TCCR0A |= (1 << WGM01);
  TCCR0B =0b00000011;
  TIMSK0 |= (1 << OCIE0A);
  */
  sei();
  ADCSRA = 0b11001101;
  Serial.begin(9600);
  /*
  while(!calib){
    OCR0A=128;
    dir = false;
    if (digitalRead(endpin) == false)calib = true;
    }*/
}

//______________________________________________________________________
/*
ISR(TIMER0_COMPA_vect) {
  if (pos < 0) dir = true;
  if (pos > stroke) dir = false;
  if (dir) {
    PORTD |= 0b00001000;
    PORTB |= 0b00100000;
    pos++;
  } else{
    PORTD &= 0b11110111;
    PORTB &= 0b11011111;
    pos--;
  }
  PORTD ^= 0b00000100;
}
*/
  //______________________________________________________________________

  ISR(ADC_vect) {
    if (strknow) {
      stroke = ADC*10+1000;
      ADMUX = 0b01000000;
    } else {
      spd = ADC;
      ADMUX = 0b01000001;
    }
    ADCSRA |= (1 << ADSC);
    strknow = !strknow;
  }

  void loop() {
    delayMicroseconds(((spd/4)+2)*spdtable[constrain(pos/(stroke/40),0,39)]);
  
    if (pos < 0) dir = true;
    if (pos > stroke) dir = false;
    if (dir) {
      PORTD |= 0b00001000;
      pos++;
    } else{
      PORTD &= 0b11110111;
      pos--;
    }
    PORTD ^= 0b00000100;
  
    while(spd >= 1014)delay(100);
  }
