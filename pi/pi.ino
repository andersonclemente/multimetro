// MULTIMETRO
#define PINO_OHMIMETRO A0
#define PINO_VOLTIMETRO A1
#define PINO_AMPERIMETRO A2
#define PINO_BUZZER 9

const float maximoVoltimetro = 35.0;
const float r1_ohmimetro = 9940;




float voltage;

void setup(){
  pinMode(PINO_VOLTIMETRO, INPUT);
  pinMode(PINO_OHMIMETRO, INPUT);
  
  pinMode(PINO_BUZZER, OUTPUT);

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  
  Serial.begin(9600);
}

float corrente,resistencia, tensao; 


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

float lerAmperimetro(){
  const int offset = 484, qtdAmostragens = 50;
  
  unsigned int somador = 0, amostra = 0;
  float media = 0.0, leitura = 0.0, correnteResultante = 0.0;

  for (int x = 0; x < qtdAmostragens; x++) {
    amostra = analogRead(A2);
    somador = somador + amostra;

    delay(3);
  }

  media = somador / qtdAmostragens;
  leitura = media - offset;
  correnteResultante = leitura * 0.04;

  return correnteResultante;
}

float lerVoltimetro(){
	float leituraEscalaArduino = analogRead(PINO_VOLTIMETRO);
	float leituraEmVolts = leituraEscalaArduino *  maximoVoltimetro / 1023.0;
	return leituraEmVolts;
}

float lerOhmimetro(){
  int somador = 0, qtdAmostragens = 10;
  for (int x = 0; x < qtdAmostragens; x++) {
    somador = somador + analogRead(PINO_OHMIMETRO);;
    delay(3);
  }

	float leituraEscalaArduino = somador/qtdAmostragens;
  //impede divisão por zero
	if(leituraEscalaArduino >= 1022) return -1;

  double resistencaResultante, aux;
  aux = (float)analogRead(A0)/1023;
  if((1 - aux) == 0) return -1;

  resistencaResultante = aux * (float) r1_ohmimetro / (1 - aux);
  
  return resistencaResultante; 
}

void ligarBuzzer(){
	digitalWrite(PINO_BUZZER, true);
}

void desligarBuzzer(){
	digitalWrite(PINO_BUZZER, false);
}