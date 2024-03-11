/*
  Capitulo 9 de Arduino desde cero en Español
  Programa que permite conectar un teclado matricial de membrana de 4x4 y realizar
  un simple control de acceso con contraseña. Debe instalarse la libreria Keypad.

  Autor: bitwiseAr  

*/

#include <Keypad.h>     // importa libreria Keypad


const byte FILAS = 4;     // define numero de filas
const byte COLUMNAS = 4;    // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'2','5','8','0'},
  {'1','4','7','*'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};

byte pinesFilas[FILAS] = {9,8,7,6};   // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {5,4,3,2}; // pines correspondientes a las columnas

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto

char TECLA;       // almacena la tecla presionada
char CLAVE[7];        // almacena en un array 6 digitos ingresados
char CLAVE_MAESTRA[7] = "123456";   // almacena en un array la contraseña maestra
byte INDICE = 0;      // indice del array


const int ledPin = 10;             // Built-in LED
const int beepPin = 11;             // Built-in BEEP
const int sensorPin = 12;           // PIR Sensor is attached to digital pin 2
const int ledBlinkTime = 500;      // Blink one for half a second while calibrating
const unsigned int calibrationTime = 10000;
int MARCADOR = 0;
  
void setup()
{
 Serial.begin(9600);      // inicializa comunicacion serie
 pinMode(ledPin, OUTPUT);
 pinMode(beepPin, OUTPUT);
 pinMode(sensorPin, INPUT);
}


void loop(){
  TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable
  if (TECLA)        // comprueba que se haya presionado una tecla
  {
    CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
    INDICE++;       // incrementa indice en uno
    Serial.print(TECLA);    // envia a monitor serial la tecla presionada
  }

  if(INDICE == 6)       // si ya se almacenaron los 6 digitos
  {
    if(!strcmp(CLAVE, CLAVE_MAESTRA))   // compara clave ingresada con clave maestra
    {
      Serial.println(" Correcta");  // imprime en monitor serial que es correcta la clave
      // We need to wait one minute for the sensor to calibrate
  // Get out of view of the sensor for this duration!
      for (unsigned int i=0; i<calibrationTime; i+=ledBlinkTime*2) {
        digitalWrite(ledPin, HIGH);
        digitalWrite(beepPin, HIGH);
        delay(ledBlinkTime);
        digitalWrite(ledPin, LOW);
        digitalWrite(beepPin, LOW);
        delay(ledBlinkTime);
        MARCADOR = !MARCADOR;
      }
      // Constantly check the state of pin 12
      // If it is HIGH the sensor is detecting motion
    if ( MARCADOR == 1) {
        if (digitalRead(sensorPin) == HIGH) {
      // Turn the LED on
        digitalWrite(ledPin, HIGH);
        delay(1000);
       
        }
        else {
        // Turn the LED off
        digitalWrite(ledPin, LOW);  
      }
        
      
      
     
    }
    else
    {
      Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
      digitalWrite(beepPin, HIGH);
      delay(150);
      digitalWrite(beepPin, LOW);
      delay(150);
      digitalWrite(beepPin, HIGH);
      delay(150);
      digitalWrite(beepPin, LOW);
      
    }

    INDICE = 0;
  }
}
