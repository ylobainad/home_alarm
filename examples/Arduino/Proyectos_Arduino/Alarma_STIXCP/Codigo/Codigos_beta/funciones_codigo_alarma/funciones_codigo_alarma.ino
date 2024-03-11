#include <Keypad.h>     // importa libreria Keypad


const byte FILAS = 4;     // define numero de filas
const byte COLUMNAS = 4;    // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'%','9','8','7'},
  {'X','6','5','4'},
  {'-','3','2','1'},
  {'+','=','0','C'}
};

byte pinesFilas[FILAS] = {A5,A4,A3,A2};   // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {5,4,3,2}; // pines correspondientes a las columnas

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto

char TECLA;       // almacena la tecla presionada
char CLAVE[7];        // almacena en un array 6 digitos ingresados
char CLAVE_MAESTRA[7] = "123456";   // almacena en un array la contraseña maestra
byte INDICE = 0;      // indice del array


const int ledPin = 6;             // Built-in LED
const int beepPin = 7;             // Built-in BEEP
const int sensorPin = 8;           // PIR Sensor is attached to digital pin 12
const int ledBlinkTime = 500;      // Blink one for half a second while calibrating
const unsigned int calibrationTime = 20000;
int var=0;  
bool detection =false;


//**********************************************************************************************************************************************************


void setup()
{
 Serial.begin(9600);      // inicializa comunicacion serie
 pinMode(ledPin, OUTPUT);
 pinMode(beepPin, OUTPUT);
 pinMode(sensorPin, INPUT);


 
 void almacenar_codigo(){
   TECLA = teclado.getKey();
      if (TECLA){        // comprueba que se haya presionado una tecla
        for (INDICE < 7){
        CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
        INDICE++;       // incrementa indice en uno
        Serial.print(TECLA);    // envia a monitor serial la tecla presionada
      }
   }
  
 }

 void limpiar_contador(){
  INDICE=0;
 }

 void validar_codigo(){
    if(!strcmp(CLAVE, CLAVE_MAESTRA)){   // compara clave ingresada con clave maestra
          Serial.println(" Correcta");  // imprime en monitor serial que es correcta la clave
        else{
             Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
            }
        }
    }

 void etapa_armado(){
  for (unsigned int i=0; i<calibrationTime; i+=ledBlinkTime*2) {
            digitalWrite(ledPin, HIGH);
            digitalWrite(beepPin, HIGH);
            delay(ledBlinkTime);
            digitalWrite(ledPin, LOW);
            digitalWrite(beepPin, LOW);
            delay(ledBlinkTime);
 }
 }

 void tono_error(){
   digitalWrite(beepPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(beepPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(250);
    digitalWrite(beepPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(250);
    digitalWrite(beepPin, LOW);
    digitalWrite(ledPin, LOW);
 }

 void desarmar_alarma(){
   digitalWrite(beepPin, HIGH);
 }

 void desarmar_alarma(){
  digitalWrite(beepPin, LOW);
 }

}

//*********************************************************************************************************************************************************


void loop(){
   switch (var){
    case 0:
     while(true){
      TECLA = teclado.getKey();
      if (TECLA){        // comprueba que se haya presionado una tecla
        CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
        INDICE++;       // incrementa indice en uno
        Serial.print(TECLA);    // envia a monitor serial la tecla presionada
      }
      if(INDICE == 6){      // si ya se almacenaron los 6 digitos
        if(!strcmp(CLAVE, CLAVE_MAESTRA)){   // compara clave ingresada con clave maestra
          Serial.println(" Correcta");  // imprime en monitor serial que es correcta la clave
          for (unsigned int i=0; i<calibrationTime; i+=ledBlinkTime*2) {
            digitalWrite(ledPin, HIGH);
            digitalWrite(beepPin, HIGH);
            delay(ledBlinkTime);
            digitalWrite(ledPin, LOW);
            digitalWrite(beepPin, LOW);
            delay(ledBlinkTime);
            
            var=1;
          }
          Serial.println(" Armado");
        }
        else{
             Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
             digitalWrite(beepPin, HIGH);
             digitalWrite(ledPin, HIGH);
             delay(250);
             digitalWrite(beepPin, LOW);
             digitalWrite(ledPin, LOW);
             delay(250);
             digitalWrite(beepPin, HIGH);
             digitalWrite(ledPin, HIGH);
             delay(250);
             digitalWrite(beepPin, LOW);
             digitalWrite(ledPin, LOW);
            }
            INDICE=0;
      }
        
        break;  //break para salir del while 
     }
     break;
     case 1:
       
       while ( digitalRead(sensorPin) == HIGH) {
               Serial.println("Senal en HIGH");
               detection =true;
              
               break;
               }
       if (detection){
       var =2;}
       else{var=1;}
       
            
       break; 
      case 2:
          digitalWrite(ledPin, HIGH);
          digitalWrite(beepPin, HIGH);
          Serial.println("Deteccion de movimiento");
          delay(5000);
          digitalWrite(ledPin, LOW);
          digitalWrite(beepPin, LOW);
          var=0;
          detection =false;

          break;  
     }
         
      

        
  }

}
