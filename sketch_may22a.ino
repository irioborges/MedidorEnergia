#include <EmonLib.h>
#include <LiquidCrystal.h>              
#include <SoftwareSerial.h>


SoftwareSerial sim800l(8, 9);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
EnergyMonitor emon1;

String Comando; 
int rede = 220.0, pino_sct = A1, tempo_segundos = 0, LED = 13, tp_tela = 0;
double consumo = 0, irms = 0, preco_kws = 0.000213;
char inchar = 0;
 
void setup() 
{

  int i;

  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Calibrando");
     
  emon1.current(pino_sct, 29);
  
  for(i = 0; i < 10; i++){
    irms = emon1.calcIrms(1480);
  }
    
  //lcd.setCursor(0,0);
  //lcd.print("Corr.(A):");
  //lcd.setCursor(0,1);
  //lcd.print("Pot. (W):");

  sim800l.begin(9600); 
  //sim800l.println("AT+CMGF=1"); 
  sim800l.println("AT+CMGF=1\r\n");
  delay(1000); 
  //sim800l.println("AT+CNMI=1,2,0,0,0"); 
  pinMode(LED, OUTPUT); 
  //EnviaSMS();
  sim800l.println("AT+CMGD=4");
  sim800l.println("AT+CMGD=1");
      
} 
 
void loop() 
{ 

  irms = emon1.calcIrms(1480); 
  
  if(irms <= 0.26){
    irms = 0;      
  }                

  consumo = consumo + ((irms * rede) / 3600);
    
  if(tempo_segundos % 3 == 0){
    if(tp_tela == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Potn: ");
      lcd.print((irms * rede), 0);
      lcd.print(" W");
      lcd.setCursor(0,1);
      lcd.print("Corr: ");
      lcd.print(irms);
      lcd.print(" A");
      tp_tela = 1;
    }else if(tp_tela == 1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cons: ");
      lcd.print(consumo / 1000,2);
      lcd.print(" kWh");
      lcd.setCursor(0,1);
      lcd.print("Valr: ");
      lcd.print((consumo) * preco_kws,2);
      lcd.print(" R$");
      tp_tela = 0;  
    }
  }

  sim800l.println("AT+CMGR=1");
   
  Comando = sim800l.readString();

  Serial.println(Comando);
  
  if(Comando.indexOf("Reset") >= 0){ //Se a variável Comando contem a palavra ON
    Serial.println("Acender o LED"); //Avisamos por comunicação serie
    digitalWrite(LED, HIGH); //Activamos o LED
    delay(100);
    sim800l.println("AT+CMGD=4");
    delay(100);
    sim800l.println("AT+CMGD=1");
  } 
 
  if(Comando.indexOf("Fatura") >= 0){ //Se a variavel Comando contem a palavra OFF
    Serial.println("Apagar o LED"); //Avisamos por comunicação serie
    digitalWrite(LED, LOW); //Desactivamos o LED
    EnviaSMS();
    delay(100);
    sim800l.println("AT+CMGD=4");
    delay(100);
    sim800l.println("AT+CMGD=1");
  }
  
  tempo_segundos++;
  delay(1000);
  
}

void mostraPrimeiraTela(double irms, int rede){
  
}

void mostraSegundaTela(double consumo, double preco_kws){
  
}

void EnviaSMS(){             
 sim800l.println("AT+CMGF=1");                 // Activamos a função de envio de SMS
 delay(100);                                    // Pequena pausa
 sim800l.println("AT+CMGS=\"+5553999018454\"");  // Definimos o número do destinatário em formato internacional
 delay(100);                                    // Pequena pausa
 sim800l.print("Olá Mundo!");                 // Definimos o corpo da mensagem
 delay(500);                                    // Pequena pausa
 sim800l.print(char(26));                      // Enviamos o equivalente a Control+Z
 delay(100);                                    // Pequena pausa
 sim800l.println("");                          // Enviamos um fim de linha
 delay(100);                                    // Pequena pausa
}

