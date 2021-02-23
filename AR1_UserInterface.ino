#include <SoftwareSerial.h>       // Cette bibliothèque émule une voie série.
SoftwareSerial mySerial(11, 10);  // affectation des broches (RX, TX) et création de l'objet mySerial

// Communication série avec le 74HC595N
int latchPin = 4;   // STCP of 74HC595N
int clockPin = 7;   // SHCP of 74HC595N
int dataPin = 6;    // DataS of 74HC595N


int readData = 0; // Number read on BT

void setup() {
  // set pins to output to control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // BT SETUP
  Serial.begin(9600);
  mySerial.begin(9600); //Default Baud for comm, it may be different for your Module
 
}
// Les nombres, UTF-8 code.
const int int_equivalant_of[11] =
{
  63,   // 0
  6,	// 1
  91,   // 2
  79,   // 3
  102,  // 4
  109,  // 5
  125,  // 6
  7,	// 7
  127,  // 8
  111,  // 9
  113   // 10 - F
};


// foction pour afficher les nombres avec le 7 segment
void update_to_nombre(int nombre)
{	// --- Bloque le latch ---
  	// Le latch devient non passant. Il memorise la valeur actuelle 
	//    des entrée du latch (du registre à décalage) et maintient
  	//   les valeurs des sorties Q0 à Q7.
  	// Peut importe les modifications du registre a décalage, le 
  	//   latch n'en tient pas compte. 
	digitalWrite(latchPin, LOW);
	
	
	// Cette instruction écrit chaque bit d'un byte/octet sur la 
	//   dataPin du registre à décalage. Après chaque bit écrit,
	//   la fonction fait osciller la clockPin pour que le registre
	//   a décalage enregistre la valeur et effectue un décalage
	//   du registre (vers la droite) pour recevoir la valeur suivante.
	// La constante MSBFIRST (Most Significant Bit First) indique que
	//   l'on commence l'écriture par le bit le plus important (de gauche
	//   a droite) 
	shiftOut(dataPin, clockPin, MSBFIRST, int_equivalant_of[nombre]);
	
	// --- Active le latch ---
  	// Devient passant et applique les valeurs du registre à décalage
  	// sur les sorties
	digitalWrite(latchPin, HIGH);
	delay(500);
}

void loop()
{
      	// Repeter jusque'
	while ( !mySerial.available() ); 

      	readData = mySerial.read(); // UTF-08 of +3symboles (input number(s)+ ctl U+000D 13 + ctl U+000A 10)
	
      	// Use the code below to check the data that is being read
      	// Serial.print(readData);
      	if ( readData <= 57 && readData >= 48) {
        	update_to_nombre(readData - 48); // UTF-08 : 0 = 48
      	} else if ( readData == 70) { // UTF-08 70 = F
        	update_to_nombre(10);
      	}
}
	
