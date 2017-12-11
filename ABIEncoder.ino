#include "ABIEncoder.h"

ABIEncoder RoulementCapteur(10);

void setup() {
  Serial.begin(2000000);
  RoulementCapteur = 0;
}

void loop() 
{
	long	valeur;

  while(true)
  {
	  valeur = RoulementCapteur;
	  Serial.print("Position: ");
	  Serial.print(valeur);
	  Serial.println();
	  delay(100);
  }
}

