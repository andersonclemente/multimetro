// DEFINE PINAGEM
#define PINO_OHMIMETRO A0
#define PINO_VOLTIMETRO A1
#define PINO_AMPERIMETRO A2
#define PINO_BUZZER 9
#define ALIMENTACAO_AMPERIMETRO 5

//VARIAVEIS DE CALIBRAÇÃO
const float maximoVoltimetro = 35.2;
const float r1_ohmimetro = 9940;

//VARIAVEIS USADA EM LOOP
float corrente,resistencia, tensao; 

void setup(){
  pinMode(PINO_VOLTIMETRO, INPUT);
  pinMode(PINO_OHMIMETRO, INPUT);
  
  pinMode(PINO_BUZZER, OUTPUT);

  pinMode(ALIMENTACAO_AMPERIMETRO, OUTPUT);
  digitalWrite(ALIMENTACAO_AMPERIMETRO, HIGH);
  
  Serial.begin(9600);
}



void loop(){
  corrente = lerAmperimetro();
  tensao = lerVoltimetro();
  resistencia = lerOhmimetro();

  if(resistencia >= 0 && resistencia < 1) ligarBuzzer();
  else desligarBuzzer();
  
  Serial.print(corrente);
  Serial.print("|");
  Serial.print(tensao);
  Serial.print("|");
  Serial.println(resistencia) ;
}


//ler sensor de corrente
float lerAmperimetro(){
  const int offset = 484, qtdAmostragens = 50;
  
  unsigned int somador = 0, amostra = 0;
  float media = 0.0, leitura = 0.0, correnteResultante = 0.0;
  
  //soma as amostragens do sensor
  for (int x = 0; x < qtdAmostragens; x++) {
    amostra = analogRead(PINO_AMPERIMETRO);
    somador = somador + amostra;

    delay(3);
  }

  //calcula a media e converte em Ampere
  media = somador / qtdAmostragens;
  leitura = media - offset;
  correnteResultante = leitura * 0.04;

  return correnteResultante;
}


float lerVoltimetro(){
  float leituraEscalaArduino = analogRead(PINO_VOLTIMETRO);
	
  //converte valor lido na porta lógica em tensão desejada
  float leituraEmVolts = leituraEscalaArduino *  maximoVoltimetro / 1023.0;
  return leituraEmVolts;
}

float lerOhmimetro(){
  int somador = 0, qtdAmostragens = 10;
  for (int x = 0; x < qtdAmostragens; x++) {
    somador = somador + analogRead(PINO_OHMIMETRO);
    delay(3);
  }

  //calcula a média dos valores lidos no pino do ohmimetro
  float leituraEscalaArduino = somador/qtdAmostragens;
  
  //impede divisão por zero e indica que não há resistência com valor negativo
  if(leituraEscalaArduino >= 1022) return -1;

  //converte o valor lido em resistência
  double resistencaResultante, aux;
  aux = (float)analogRead(A0)/1023;
  resistencaResultante = aux * (float) r1_ohmimetro / (1 - aux);
  
  return resistencaResultante; 
}

void ligarBuzzer(){
  digitalWrite(PINO_BUZZER, true);
}

void desligarBuzzer(){
  digitalWrite(PINO_BUZZER, false);
}