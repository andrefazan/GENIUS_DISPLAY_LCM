// JOGUIN
#include <UnicViewAD.h>

LCM Lcm(Serial);
// Variáveis
const long lcmBaudrate = 115200;


int sequencia [32] = {};
int leds [4] = {8, 9, 10, 11};
int botoes [4] = {2, 3, 4, 5};
int tons [4] = {420, 550, 750, 950};
int rodada = 0;
int passo = 0;
int repeticoes = -1;
int botaoPressionado = 0;

bool fimdejogo = false;
bool iniciajogo = false;





//leds lcm
LcmVar ledButton0 (8);
LcmVar ledButton1 (9);
LcmVar ledButton2 (10);
LcmVar ledButton3 (11);

//botoes lcm
LcmVar button0 (2);
LcmVar button1 (3);
LcmVar button2 (4);
LcmVar button3 (5);

LcmVar startbutton (1);

LcmVar reps (20);




int returnValue = 32767;


void setup() {



  Lcm.begin(lcmBaudrate);

  //LEDS
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  //botões
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  // tone
  pinMode(7, OUTPUT);

  randomSeed(analogRead(0));



}

void loop() {

  if (digitalRead(13) == true) {
    iniciajogo = true;
    Lcm.changePicId(1);
    delay(500);

  }
  if (startbutton.available()) {
    if (startbutton.getData() == returnValue)  {
      iniciajogo = true;
      Lcm.changePicId(1);
      delay(500);
    }
  }

  if (iniciajogo == false) {
    Lcm.changePicId(0);

  }
  else {


    proximaRodada();
    reproduzirSequencia();
    aguardaJogador();

    // reset
    if (fimdejogo == true) {
      sequencia[32] = {};

      passo = 0;
      rodada = 0;
      fimdejogo = false;
      iniciajogo = false;
      delay(500);
      Lcm.changePicId(2);
      delay(2000);
      Lcm.changePicId(0);

      repeticoes = -1 ;
      reps.write(repeticoes);


    }
    delay(1000);
  }
}

void proximaRodada() {
  int sorteio = random(4);
  sequencia[rodada] = sorteio;
  rodada = rodada + 1;
  repeticoes = repeticoes + 1;
  reps.write(repeticoes);


}


void reproduzirSequencia() {
  for (int i = 0 ; i < rodada ; i++) {
    digitalWrite(leds[sequencia[i]] , HIGH);
    tone(7, tons[sequencia[i]]);
    if (sequencia[i] == 0) {
      ledButton0.write(1);
    }
    if (sequencia[i] == 1) {
      ledButton1.write(1);
    }
    if (sequencia[i] == 2) {
      ledButton2.write(1);
    }
    if (sequencia[i] == 3) {
      ledButton3.write(1);
    }
    delay(200);
    noTone(7);
    delay(300);
    ledButton0.write(0);
    ledButton1.write(0);
    ledButton2.write(0);
    ledButton3.write(0);

    digitalWrite(leds[sequencia[i]] , LOW);
    delay(300);


  }


}







void aguardaJogador() {

  for (int i = 0 ; i < rodada ; i++) {
    bool jogadaefetuada = false;
    while (!jogadaefetuada) {
      for (int i = 0 ; i <= 3 ; i++ ) {
        if (button0.available()) {
          if (button0.getData() == returnValue) {
            botaoPressionado = 0;
            ledButton0.write(1);
            digitalWrite(leds[0], HIGH);
            tone(7, tons[0]);
            delay(200);

            ledButton0.write(0);
            digitalWrite(leds[0], LOW);
            noTone(7);
            jogadaefetuada = true;
          }
        }
        if (button1.available()) {
          if (button1.getData() == returnValue) {
            botaoPressionado = 1;
            ledButton1.write(1);
            digitalWrite(leds[1], HIGH);
            tone(7, tons[1]);
            delay(200);

            ledButton1.write(0);
            digitalWrite(leds[1], LOW);
            noTone(7);
            jogadaefetuada = true;
          }
        }

        if (button2.available()) {
          if (button2.getData() == returnValue) {
            botaoPressionado = 2;
            ledButton2.write(1);
            digitalWrite(leds[2], HIGH);
            tone(7, tons[2]);
            delay(200);

            ledButton2.write(0);
            digitalWrite(leds[2], LOW);
            noTone(7);
            jogadaefetuada = true;
          }
        }
        if (button3.available()) {
          if (button3.getData() == returnValue) {
            botaoPressionado = 3;
            ledButton3.write(1);
            digitalWrite(leds[3], HIGH);
            tone(7, tons[3]);
            delay(200);

            ledButton3.write(0);
            digitalWrite(leds[3], LOW);
            noTone(7);
            jogadaefetuada = true;
          }
        }





        if (digitalRead(botoes[i]) == HIGH ) {
          botaoPressionado = i;
          digitalWrite(leds[i], HIGH);

          if (i == 0) {
            ledButton0.write(1);
          }
          if (i == 1) {
            ledButton1.write(1);
          }
          if (i == 2) {
            ledButton2.write(1);
          }
          if (i == 3) {
            ledButton3.write(1);
          }


          tone(7, tons[i]);
          delay(200);
          digitalWrite(leds[i], LOW);
          ledButton0.write(0);
          ledButton1.write(0);
          ledButton2.write(0);
          ledButton3.write(0);
          noTone(7);
          jogadaefetuada = true;

        }
      }
    }


    //Verifica se o jogador acertou a ordem dos botões e
    // emite um sinal luminoso e sonoro no caso de erro


    if (sequencia[passo] != botaoPressionado) {
      for (int i = 0 ; i <= 3; i++) {
        tone(7, 100);
        digitalWrite(leds[i], HIGH);
        ledButton0.write(1);
        ledButton1.write(1);
        ledButton2.write(1);
        ledButton3.write(1);
        delay(100);
        digitalWrite(leds[i], LOW);
        ledButton0.write(0);
        ledButton1.write(0);
        ledButton2.write(0);
        ledButton3.write(0);
        noTone(7);
      }
      fimdejogo = true;
      break;
    }



    passo = passo + 1;

  }

  passo = 0;
}
