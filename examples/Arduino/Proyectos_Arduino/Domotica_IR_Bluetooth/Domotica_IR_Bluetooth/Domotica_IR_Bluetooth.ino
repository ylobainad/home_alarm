/*  Domotica con IR $ Bluetoo
 *  Instagram: el_profe_garcia
 *   
 *  Modificado IRrecvDemo ejemplo de Ken Shirriff IRremote Library
 *  Ken Shirriff

 */
#include <IRremote.h>
#include <Servo.h>                // Incluye la libreria Servo
int RECV_PIN = 2;                 // IR Receptor - Arduino Pin Numero 2
IRrecv irrecv(RECV_PIN);
decode_results results;
int estado1=0;
Servo servo8;                    // Crea el objeto servo8 con las caracteristicas de Servo
 
void setup()
{
  Serial.begin(9600);            // Inicializa el puerto serial a 9600 Baudios
  irrecv.enableIRIn();           // Inicializa el Receptor
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT); 
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);  
  digitalWrite(13, 0); 
  digitalWrite(12, 1);
  digitalWrite(11, 0); 
  servo8.attach(8,660,1400);       // Asocia el servo8 al pin 8, define el min y max del ancho del pulso 
  servo8.write(90);                // Gira el servo a 90 grados  
  delay(1000);     
  
}
 
void loop() {
  
  if (irrecv.decode(&results)) {   
        Serial.println(results.value, HEX);
     if (estado1 == 0) {
        digitalWrite(13, 1);  
        estado1=1;
      }
      else {
         digitalWrite(13, 0);  
        estado1=0;

     }
    if (results.value == 0xE0E036C9) {      //cambiar el codigo manteniendo "0x"
      digitalWrite(12, 1);  
      delay(100);
      }
      if (results.value == 0xE0E028D7) {    //cambiar el codigo manteniendo "0x"
      digitalWrite(12, 0);  
      delay(100);
      }

    if (results.value == 0xE0E0A857) {        //cambiar el codigo manteniendo "0x" 
       servo8.write(130);                     // Gira el servo a 130 grados  
       digitalWrite(11, 1);  
       delay(1000);                          // Espera 1000 mili segundos a que el servo llegue a la posicion
      }
      if (results.value == 0xE0E06897) {    //cambiar el codigo manteniendo "0x"
       servo8.write(53);                    // Gira el servo a 53 grados  offffffff
        digitalWrite(11, 0);  
       delay(1000);                         // Espera 1000 mili segundos a que el servo llegue a la posicion
      }
    
 
    irrecv.resume(); // Recibe el proximo valor
  }
  delay(300);
}
