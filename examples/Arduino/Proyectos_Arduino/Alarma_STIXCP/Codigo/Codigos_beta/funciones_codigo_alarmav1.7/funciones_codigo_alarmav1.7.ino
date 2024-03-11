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
bool CLAVE_CORRECTA = false;
bool SENSOR_LOW = false;


const int ledPin = 6;             // Built-in LED
const int beepPin = 7;             // Built-in BEEP
const int sensorPin = 8;           // PIR Sensor is attached to digital pin 12
const int ledBlinkTime = 500;      // Blink one for half a second while calibrating
const unsigned int calibrationTime = 20000;
int var=0;  
bool DETECTION =false;
int i =0 ;
bool ALARMA =false;


//**********************************************************************************************************************************************************


void setup()
{
 Serial.begin(9600);      // inicializa comunicacion serie
 pinMode(ledPin, OUTPUT);
 pinMode(beepPin, OUTPUT);
 pinMode(sensorPin, INPUT);
}

void almacenar_codigo(){
  //while (INDICE<6){
   TECLA = teclado.getKey();
   if (TECLA){        // comprueba que se haya presionado una tecla
      //for (INDICE=0;INDICE < 6;){
      CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
      INDICE++;       // incrementa indice en uno
      Serial.print(TECLA);    // envia a monitor serial la tecla presionada
      };
   //;}
  }

 void limpiar_contador(){
  INDICE=0;
 }

 void validar_codigo(){
    if(!strcmp(CLAVE, CLAVE_MAESTRA)){   // compara clave ingresada con clave maestra
       Serial.println(" Correcta");
       CLAVE_CORRECTA=true;}   // imprime en monitor serial que es correcta la clave
       
    else{
             Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
             CLAVE_CORRECTA=false;
            }
        }
    

 void etapa_armado(){
  while (i<calibrationTime)  {
    i+=ledBlinkTime*2;
    digitalWrite(ledPin, HIGH);
    digitalWrite(beepPin, HIGH);
    delay(ledBlinkTime);
    digitalWrite(ledPin, LOW);
    digitalWrite(beepPin, LOW);
    delay(ledBlinkTime);
 }
  i=0;
  Serial.println("Sistema Armado"); // imprimir mensaje "Sistema Armado"
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

 void generar_alarma(){
   digitalWrite(beepPin, HIGH);
 }

 void desarmar_alarma(){
  digitalWrite(beepPin, LOW);
 }

//*********************************************************************************************************************************************************


void loop(){
 
 switch (var){
    case 0:
      //almacenar_codigo();
      while (INDICE <6){
         almacenar_codigo();
         Serial.println("Almacenando codigo");
         
      }
      Serial.println("Codigo Almacenado");
      limpiar_contador();
      validar_codigo();
      Serial.println("validando clave"); 
        if (CLAVE_CORRECTA== true){
           Serial.println("la clave ha sido validada y es correcta");
           CLAVE_CORRECTA = false;
           etapa_armado(); // se ejecuta si la clave es correcta
           Serial.println("A punto de saltar para caso uno");
           var = 1;
           break;
           }
         else{
           tono_error(); // se ejecuta si la clave es incorrecta
           Serial.println("la clave ha sido validada y es incorrecta");
           break;
             }


     case 1:
     Serial.println("Poniendo SENSOR_LOW en falso");
      SENSOR_LOW = false;
      Serial.println("estoy en caso1");
       while (digitalRead(sensorPin)==LOW){ //while 1
        SENSOR_LOW =true;
        if (INDICE <6){   //while2
         Serial.println("Almacenando codigo caso 1");
         almacenar_codigo();
        }
         else{
           limpiar_contador();
           validar_codigo();
           if (CLAVE_CORRECTA== true){
              CLAVE_CORRECTA = false;
              Serial.println("Desarmando alarma");
              var = 0;
           }
           else{
     Serial.println("Tono de error");
     tono_error();
     }
  }
   break;
}

Serial.println("Comparando SENSOR_LOW");
if (SENSOR_LOW == true){
  Serial.println("SENSOR_LOW esta en true");
  break;
}
else{
  Serial.println("Senal en HIGH");
  generar_alarma();
  var =2;
  break;
}
case 2:
  while (INDICE <6){
     Serial.println("Almacenando codigoCaso2");
     almacenar_codigo();
  }
  limpiar_contador();
    validar_codigo();
    if (CLAVE_CORRECTA == true){
      CLAVE_CORRECTA = false;
      Serial.println("Desarmando alarma");
      desarmar_alarma();
      var = 0;
      break;
    }
    else{
     desarmar_alarma();
     delay(250);
     tono_error();
     generar_alarma();
     var=2;
     break;
     }
     
         
  
     
  }
            
}
