
//int ledVermelho = 2;
#define LED_AMARELO 2
#define LED_VERMELHO 3
#define LED_VERDE 4
#define LED_AZUL 5
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define TAMANHO_SEQUENCIA 4
#define BOTAO_LED_AMARELO 8
#define BOTAO_LED_VERMELHO 9
#define BOTAO_LED_VERDE 10
#define BOTAO_LED_AZUL 11
#define INDEFINIDO -1
#define BUZZER 13

enum Estados {
  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDEDO,
  JOGO_FINALIZADO_COM_SUCESSO,
  JOGO_FINALIZADO_COM_FALHA,
};
 

int sequenciaLuzes [TAMANHO_SEQUENCIA];

int rodada = 0;

int leds_respondidos = 0;


void setup() {
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}


void iniciaPortas() {
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  
  pinMode(BUZZER, OUTPUT);
  
  pinMode(BOTAO_LED_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_LED_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_LED_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_LED_AZUL, INPUT_PULLUP);

}



void iniciaJogo() {

  int jogo = analogRead(0);
  randomSeed(jogo);

  for (int indice = 0; indice < TAMANHO_SEQUENCIA; indice ++){
    sequenciaLuzes[indice] = sorteiaCor(); 
  } 
}

void loop() {
  switch(estadoAtual()){
    case PRONTO_PARA_PROX_RODADA:
    preparaNovaRodada();
    break;
    case USUARIO_RESPONDEDO:
    processaRespostaUser();
    break;
    case JOGO_FINALIZADO_COM_SUCESSO:
    ///jogoFinalizadoSucesso();
    break;
    case JOGO_FINALIZADO_COM_FALHA:
    jogoFinalizadoSemSucesso();
    break;
    
  }
 
}



void  processaRespostaUser(){
 int resposta = checaRespostaJogador();
 if (resposta == INDEFINIDO){
  return;
 }
 if(resposta == sequenciaLuzes[leds_respondidos]){
   leds_respondidos++;
  
 }else{
  rodada == TAMANHO_SEQUENCIA + 2;
  jogoFinalizadoSemSucesso();
  //erro
  
 }
  
}

void preparaNovaRodada(){
  rodada++;
  leds_respondidos = 0;
  if(rodada <= TAMANHO_SEQUENCIA){
    tocaLedRodada();
  }
  
}

int estadoAtual() {
  if(rodada <= TAMANHO_SEQUENCIA){
    if(leds_respondidos == rodada){
      return PRONTO_PARA_PROX_RODADA; 
    }else{
      return USUARIO_RESPONDEDO; 
    }
     return PRONTO_PARA_PROX_RODADA; 
  }else if(rodada == TAMANHO_SEQUENCIA + 1 ){
    return JOGO_FINALIZADO_COM_SUCESSO;    
  }
  else {
    return JOGO_FINALIZADO_COM_FALHA;  
  }
  
}


void tocaLedRodada(){
   for(int i = 0; i < rodada; i++){
    piscaLed(sequenciaLuzes[i]); 
  } 
}

int checaRespostaJogador(){
  if(digitalRead(BOTAO_LED_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);
  }
   if(digitalRead(BOTAO_LED_VERMELHO) == LOW){
     return piscaLed(LED_VERMELHO);
  }
  if(digitalRead(BOTAO_LED_VERDE) == LOW){
    return piscaLed(LED_VERDE);
  }
  if(digitalRead(BOTAO_LED_AZUL) == LOW){
     return piscaLed(LED_AZUL);
  }
  return INDEFINIDO;
}



int sorteiaCor(){
  return random(LED_AMARELO, LED_AZUL + 1);
}





void piscaSequencia1() {
  piscaLed(LED_AMARELO);
  piscaLed(LED_VERMELHO);
  piscaLed(LED_VERDE);
  piscaLed(LED_AZUL);

}
void piscaSequencia2() {
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_VERDE, LOW);
  delay(MEIO_SEGUNDO);


}


int piscaLed(int portaLed) {
  verificaSomDoLed(portaLed);
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;


}


void playFreq(double freqHz, int durationMs){
  //Calculate the period in microseconds
  int periodMicro = int((1/freqHz)*1000000);
  int halfPeriod = periodMicro/2;
   
  //store start time
  int startTime = millis();
   
  //(millis() - startTime) is elapsed play time
  while((millis() - startTime) < durationMs){
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(halfPeriod);
  }
}



void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100);
}

void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2500);
      break;
  }
}

void jogoFinalizadoSemSucesso(){
 
  for(double wah=0; wah<4; wah+=6.541){
    playFreq(440+wah, 50);        //'A4' gliss to A#4
  }
  playFreq(466.164, 100);         //A#4
  delay(80);
  for(double wah=0; wah<5; wah+=4.939){
    playFreq(415.305+wah, 50);    //Ab4 gliss to A4
  }
  playFreq(440.000, 100);          //A4
  delay(80);
  for(double wah=0; wah<5; wah+=4.662){
    playFreq(391.995+wah, 50);    //G4 gliss to Ab4
  }
  playFreq(415.305, 100);          //Ab4
  delay(80);
  for(int j=0; j<7; j++){          //oscillate between G4 and Ab4
    playFreq(391.995, 70);         //G4
    playFreq(415.305, 70);         //Ab4
  }
}
  
