#include <Keypad.h>     // importa libreria Keypad
#include <IRremote.h>

int RECV_PIN = A5;                 // IR Receptor - Arduino Pin Numero 2
IRrecv irrecv(RECV_PIN);
decode_results results;


const byte FILAS = 4;     // define numero de filas
const byte COLUMNAS = 4;    // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'1', '5', '8', '0'},
  {'2', '4', '7', '*'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

byte pinesFilas[FILAS] = {9, 8, 7, 6}; // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2}; // pines correspondientes a las columnas

Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto

char TECLA;       // almacena la tecla presionada
char CLAVE[7];        // almacena en un array 6 digitos ingresados
char CLAVE_MAESTRA[7] = "123456";   // almacena en un array la contraseña maestra
byte INDICE = 0;      // indice del array
bool CLAVE_CORRECTA = false;
bool SENSOR_LOW = false;
bool ESTADO = true;


const int ledPin = 10;             // Built-in LED
const int ledPin2 = 13;             // Built-in LED
const int beepPin = 11;             // Built-in BEEP
const int sensorPin1 = 12;           // PIR Sensor is attached to digital pin 8
const int sensorPin2 = A0;           // PIR Sensor is attached to digital pin A0
const int sensorPin3 = A1;           // PIR Sensor is attached to digital pin A1
const int sensorPin4 = A2;          // PIR Sensor is attached to digital pin 9
const int relayPin = A3;  // Relay que dispara la sirena
//const int botonRF= 10;           //
//const int botonRF2= 11;           //
const int ledBlinkTime = 500;      // Blink one for half a second while calibrating
const unsigned int calibrationTime = 20000;
int var = 6;
bool DETECTION = false;
int i = 0 ;
bool ALARMA = false;


//**********************************************************************************************************************************************************


void setup()
{
  Serial.begin(9600);      // inicializa comunicacion serie
  irrecv.enableIRIn();           // Inicializa el Receptor
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(beepPin, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  //pinMode(botonRF, INPUT);
  //pinMode(botonRF2, INPUT);
}

void almacenar_codigo() {
  //while (INDICE<6){
  TECLA = teclado.getKey();
  if (TECLA) {       // comprueba que se haya presionado una tecla
    //for (INDICE=0;INDICE < 6;){
    CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
    INDICE++;       // incrementa indice en uno
    Serial.print(TECLA);    // envia a monitor serial la tecla presionada
  };
  //;}
}

void limpiar_contador() {
  INDICE = 0;
}

void validar_codigo() {
  if (!strcmp(CLAVE, CLAVE_MAESTRA)) { // compara clave ingresada con clave maestra
    Serial.println(" Correcta");
    CLAVE_CORRECTA = true;
  }   // imprime en monitor serial que es correcta la clave

  else {
    Serial.println(" Incorrecta");  // imprime en monitor serial que es incorrecta la clave
    CLAVE_CORRECTA = false;
  }
}


void etapa_armado() {
  while (i < calibrationTime)  {
    i += ledBlinkTime * 2;
    digitalWrite(ledPin, HIGH);
    digitalWrite(beepPin, HIGH);
    delay(ledBlinkTime);
    digitalWrite(ledPin, LOW);
    digitalWrite(beepPin, LOW);
    delay(ledBlinkTime);
  }
  i = 0;
  digitalWrite(ledPin2, HIGH);
  Serial.println("Sistema Armado"); // imprimir mensaje "Sistema Armado"
  ESTADO = false;
}

void tono_error() {
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

void generar_alarma() {
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin, HIGH);
  digitalWrite(beepPin, HIGH);
  digitalWrite(relayPin, HIGH);
  INDICE = 0;
}

void desarmar_alarma() {
  digitalWrite(beepPin, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(relayPin, LOW);
  INDICE = 0;
  Serial.println("DESARMANDO ALARMA");
  ESTADO = true;
}

//*********************************************************************************************************************************************************


void loop() {

  switch (var) {

    case 6:
      //Serial.println("estoy en caso 6");
      //if (digitalRead(10)==HIGH) {
      //Serial.println(results.value, HEX);
      //if (ESTADO == true) {
        if (irrecv.decode(&results)) {
          //Serial.println("reviso si se apreto el boton RF");
         // Serial.println(results.value, HEX);
           irrecv.resume(); // Recibe el proximo valor
          if (results.value == 0x4EA240AE) {
            //  Serial.println("ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ");
              
           // irrecv.resume(); // Recibe el proximo valor
            // break;
           // Serial.println("Llego el comando activar alarma");
             delay(100);
            etapa_armado();
            var = 1;
            break;
          }

        }
     // }

      else {
        var = 0;
        break;
      }


    case 0:
     // Serial.println("estoy en caso 0");
      //almacenar_codigo();
      if (INDICE < 6) {
        almacenar_codigo();
       // Serial.println("Almacenando codigo");
        var = 6;
        break;

      }
      else {
       // Serial.println("Codigo Almacenado");
        limpiar_contador();
        validar_codigo();
       // Serial.println("validando clave");
        if (CLAVE_CORRECTA == true) {
        //  Serial.println("la clave ha sido validada y es correcta");
          CLAVE_CORRECTA = false;
          etapa_armado(); // se ejecuta si la clave es correcta
         // Serial.println("A punto de saltar para caso uno");
          var = 1;
          break;
        }
        else {
          tono_error(); // se ejecuta si la clave es incorrecta
         // Serial.println("la clave ha sido validada y es incorrecta");
          var = 6;
          break;
        }
      }

    case 1:

   //  Serial.println("estoy en caso 1");
      // if (digitalRead(11)==HIGH) {
      //Serial.println(results.value, HEX);
      // if (results.value == 0xE0E036C9) {

      // desarmar_alarma();
      //  var = 6;
      // break;
      // }
      //Serial.println("Poniendo SENSOR_LOW en falso");
      SENSOR_LOW = false;
      // Serial.println("estoy en caso1");
      while (digitalRead(sensorPin1) == LOW) { //while 1
        SENSOR_LOW = true;
      //  Serial.println(results.value, HEX);
          // Recibe el proximo valor
        if (irrecv.decode(&results)) {
          irrecv.resume();
      //    Serial.println("reviso si se apreto el boton RF");
          if (results.value == 0xA32AB931) {
            //irrecv.resume(); // Recibe el proximo valor
              delay(100);
            desarmar_alarma();
            var = 6;
            break;
          }
        }
        else {
        //  Serial.println("estoy despues del else");
          if (INDICE < 6) { //while2
            //Serial.println("Almacenando codigo caso 1");
            almacenar_codigo();
            var = 3; //aqui estuvo el ultimo cambio
            //Serial.println("SENSOR_LOW esta en true");
          }
          else {
            limpiar_contador();
            validar_codigo();
            if (CLAVE_CORRECTA == true) {
              CLAVE_CORRECTA = false;
           //   Serial.println("Desarmando alarma");
              desarmar_alarma();
              var = 6;
              break;
            }
            else {
           //   Serial.println("Tono de error");
              tono_error();
              var = 3;
            }
          }
        }
        break;
      }

      //Serial.println("Comparando SENSOR_LOW");
      if (SENSOR_LOW == true) {
        //Serial.println("estoy aqui");
        //Serial.println("SENSOR_LOW esta en true");
        //var=3;
        break;
      }
      else {
      //  Serial.println("Senal en HIGH");
        generar_alarma();
        Serial.println("ZONA 1");
        var = 2;
        break;
      }
    case 2:
     if (irrecv.decode(&results)) {
     // Serial.println("estoy en caso 2");
      //if (digitalRead(11)==HIGH) {
      irrecv.resume();
      if (results.value == 0xA32AB931) {
      //  Serial.println(results.value, HEX);
       // delay(3000);
        // if (results.value == 0xE0E036C9) {

        desarmar_alarma();
        var = 6;
        break;
      }
     }
      else {
        if (INDICE < 6) {
          //Serial.println("Almacenando codigoCaso2");
          almacenar_codigo();
          var = 2;
          break;
        }
        else {
          limpiar_contador();
          validar_codigo();
          if (CLAVE_CORRECTA == true) {
            CLAVE_CORRECTA = false;
            //Serial.println("Desarmando alarma");
            desarmar_alarma();
            var = 6;
            break;
          }
          else {
            desarmar_alarma();
            delay(250);
            tono_error();
            generar_alarma();
            var = 2;
            break;
          }
        }
      }
      break;
    case 3:
      // Serial.println("estoy en caso 3");
      // if (digitalRead(11)==HIGH) {
      //Serial.println(results.value, HEX);
      // if (results.value == 0xE0E036C9) {

      // desarmar_alarma();
      //  var = 6;
      // break;
      // }
      //Serial.println("Poniendo SENSOR_LOW en falso");
      SENSOR_LOW = false;
      // Serial.println("estoy en caso1");
      while (digitalRead(sensorPin1) == LOW) { //while 1
        SENSOR_LOW = true;
      //  Serial.println(results.value, HEX);
         //irrecv.resume(); // Recibe el proximo valor
        if (irrecv.decode(&results)) {
          irrecv.resume();
       //   Serial.println("reviso si se apreto el boton RF");
          if (results.value == 0xA32AB931) {
            //irrecv.resume(); // Recibe el proximo valor
            desarmar_alarma();
            var = 6;
            break;
          }
        }
        else {
        //  Serial.println("estoy despues del else");
          if (INDICE < 6) { //while2
            //Serial.println("Almacenando codigo caso 1");
            almacenar_codigo();
            var = 4; //aqui estuvo el ultimo cambio
            //Serial.println("SENSOR_LOW esta en true");
          }
          else {
            limpiar_contador();
            validar_codigo();
            if (CLAVE_CORRECTA == true) {
              CLAVE_CORRECTA = false;
          //    Serial.println("Desarmando alarma");
              desarmar_alarma();
              var = 6;
              break;
            }
            else {
          //    Serial.println("Tono de error");
              tono_error();
              var = 4;
            }
          }
        }
        break;
      }

      //Serial.println("Comparando SENSOR_LOW");
      if (SENSOR_LOW == true) {
        //Serial.println("estoy aqui");
        //Serial.println("SENSOR_LOW esta en true");
        //var=3;
        break;
      }
      else {
      //  Serial.println("Senal en HIGH");
        generar_alarma();
        Serial.println("ZONA 2");
        var = 2;
        break;
      }
case 4:
  //Serial.println("estoy en caso 4");
      // if (digitalRead(11)==HIGH) {
      //Serial.println(results.value, HEX);
      // if (results.value == 0xE0E036C9) {

      // desarmar_alarma();
      //  var = 6;
      // break;
      // }
      //Serial.println("Poniendo SENSOR_LOW en falso");
      SENSOR_LOW = false;
      // Serial.println("estoy en caso1");
      while (digitalRead(sensorPin1) == LOW) { //while 1
        SENSOR_LOW = true;
      //  Serial.println(results.value, HEX);
       //  irrecv.resume(); // Recibe el proximo valor
        if (irrecv.decode(&results)) {
          irrecv.resume();
       //   Serial.println("reviso si se apreto el boton RF");
          if (results.value == 0xA32AB931) {
            //irrecv.resume(); // Recibe el proximo valor
            desarmar_alarma();
            var = 6;
            break;
          }
        }
        else {
       //   Serial.println("estoy despues del else");
          if (INDICE < 6) { //while2
            //Serial.println("Almacenando codigo caso 1");
            almacenar_codigo();
            var = 5; //aqui estuvo el ultimo cambio
            //Serial.println("SENSOR_LOW esta en true");
          }
          else {
            limpiar_contador();
            validar_codigo();
            if (CLAVE_CORRECTA == true) {
              CLAVE_CORRECTA = false;
         //     Serial.println("Desarmando alarma");
              desarmar_alarma();
              var = 6;
              break;
            }
            else {
          //    Serial.println("Tono de error");
              tono_error();
              var = 5;
            }
          }
        }
        break;
      }

      //Serial.println("Comparando SENSOR_LOW");
      if (SENSOR_LOW == true) {
        //Serial.println("estoy aqui");
        //Serial.println("SENSOR_LOW esta en true");
        //var=3;
        break;
      }
      else {
    //    Serial.println("Senal en HIGH");
        generar_alarma();
        Serial.println("ZONA 3");
        var = 2;
        break;
      }
case 5:
  //Serial.println("estoy en caso 5");
      // if (digitalRead(11)==HIGH) {
      //Serial.println(results.value, HEX);
      // if (results.value == 0xE0E036C9) {

      // desarmar_alarma();
      //  var = 6;
      // break;
      // }
      //Serial.println("Poniendo SENSOR_LOW en falso");
      SENSOR_LOW = false;
      // Serial.println("estoy en caso1");
      while (digitalRead(sensorPin1) == LOW) { //while 1
        SENSOR_LOW = true;
      //  Serial.println(results.value, HEX);
        // irrecv.resume(); // Recibe el proximo valor
        if (irrecv.decode(&results)) {
          irrecv.resume();
       //   Serial.println("reviso si se apreto el boton RF");
          if (results.value == 0xA32AB931) {
            //irrecv.resume(); // Recibe el proximo valor
            desarmar_alarma();
            var = 6;
            break;
          }
        }
        else {
        //  Serial.println("estoy despues del else");
          if (INDICE < 6) { //while2
            //Serial.println("Almacenando codigo caso 1");
            almacenar_codigo();
            var = 1; //aqui estuvo el ultimo cambio
            //Serial.println("SENSOR_LOW esta en true");
          }
          else {
            limpiar_contador();
            validar_codigo();
            if (CLAVE_CORRECTA == true) {
              CLAVE_CORRECTA = false;
           //   Serial.println("Desarmando alarma");
              desarmar_alarma();
              var = 6;
              break;
            }
            else {
            //  Serial.println("Tono de error");
              tono_error();
              var = 1;
            }
          }
        }
        break;
      }

      //Serial.println("Comparando SENSOR_LOW");
      if (SENSOR_LOW == true) {
        //Serial.println("estoy aqui");
        //Serial.println("SENSOR_LOW esta en true");
        //var=3;
        break;
      }
      else {
       // Serial.println("Senal en HIGH");
        generar_alarma();
         Serial.println("ZONA 4");
        var = 2;
        break;
      }
  
}
}
