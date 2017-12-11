#include "ABIEncoder.h"

ABIEncoder RoulementCapteur(10);

void setup() {
  Serial.begin(2000000);	// Affichage sur la ligne série, à 2Mbps
  RoulementCapteur = 0;		// Remise à zéro de l'index.
}

void loop() 
{
	long	valeur;

	while(true)
	{
		valeur = RoulementCapteur;	// Lecture de la position du roulement capteur
		Serial.print("Position: ");	// Affichage de la position
		Serial.print(valeur);
		Serial.println();
		delay(100);
	}
}

