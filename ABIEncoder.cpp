/*----------------------------------------------------------------------------
 * Dependencies
 *----------------------------------------------------------------------------*/
#include "ABIEncoder.h"
#include <SPI.h>
#include <Arduino.h>


/*----------------------------------------------------------------------------
 * Module local definitions
 *----------------------------------------------------------------------------*/
// Opérations sur le LS7366R
#define OP_CLEAR                	0b00000000
#define OP_READ                 	0b01000000
#define OP_WRITE                	0b10000000
#define OP_LOAD                 	0b11000000

// Registres du LS7366R
#define REG_MDR0                	0b00001000
#define REG_MDR1                	0b00010000
#define REG_DTR                 	0b00011000
#define REG_CNTR                	0b00100000
#define REG_OTR                 	0b00101000
#define REG_STR                 	0b00110000

// Paramétrages de MDR0
// Résolution du comptage
#define	MDR0_B1B0_CLOCK_DIR			0b00000000		// A = clock, B = direction
#define	MDR0_B1B0_X1				0b00000001		// Une seule voie de comptage
#define	MDR0_B1B0_X2				0b00000010		// Deux voies de comptage
#define	MDR0_B1B0_X4				0b00000011		// Tous les fronts des deux voies
// Mode de comptage
#define	MDR0_B3B2_FREE_RUNNING		0b00000000		// Comptage libre
#define	MDR0_B3B2_SINGLE_CYCLE		0b00000100		// Comptage en cycle unique
#define	MDR0_B3B2_RANGE_LIMIT		0b00001000		// Comptage avec limite
#define	MDR0_B3B2_MODULU_COUNT		0b00001100		// Comptage modulo DTR
// Gestion du signal d'index
#define	MDR0_B5B4_NO_INDEX			0b00000000		// Index ignoré
#define	MDR0_B5B4_LOAD_CNTR			0b00010000		// L'arrivée de l'index charge DTR dans CNTR
#define	MDR0_B5B4_RESET_CNTR		0b00100000		// L'arrivée de l'index remet à zéro CNTR
#define	MDR0_B5B4_LOAD_OTR			0b00110000		// L'arrivée de l'index charge CNTR dans OTR
// Type d'index
#define	MDR0_B6_ASYNC_INDEX			0b00000000		// Index asynchrone
#define	MDR0_B6_SYNC_INDEX			0b01000000		// Index synchrone
// Filtrage
#define	MDR0_B7_FILTER_CLOCK_F1		0b00000000		// Clock division factor = 1
#define	MDR0_B7_FILTER_CLOCK_F2		0b10000000		// Clock division factor = 2

// Paramétrages de MDR1
// Longueur du compteur
#define	MDR1_B1B0_COUNT_32_BITS		0b00000000		// Comptage sur 32 bits
#define	MDR1_B1B0_COUNT_24_BITS		0b00000001		// Comptage sur 24 bits
#define	MDR1_B1B0_COUNT_16_BITS		0b00000010		// Comptage sur 16 bits
#define	MDR1_B1B0_COUNT_8_BITS		0b00000011		// Comptage sur 8 bits
// Activation ou désactivation du comptage
#define	MDR1_B2_ENABLE_COUNTING		0b00000000		// Active le comptage
#define	MDR1_B2_DISABLE_COUNTING	0b00000100		// Désactive le comptage
// Gestion des événements en sortie
#define	MDR1_B4_NO_FLAG_INDEX		0b00000000		// Flag sur IDX inactif
#define	MDR1_B4_FLAG_INDEX			0b00010000		// Flag sur IDX (INDEX détecté)
#define	MDR1_B5_NO_FLAG_COMPARE		0b00000000		// Flag sur CMP inactif
#define	MDR1_B5_FLAG_COMPARE		0b00100000		// Flag sur CMP (CNTR égal à DTR)
#define	MDR1_B6_NO_FLAG_UNDERFLOW	0b00000000		// Flag sur BW inactif
#define	MDR1_B6_FLAG_UNDERFLOW		0b01000000		// Flag sur BW (dépassement de capacité négatif sur CNTR)
#define	MDR1_B7_NO_FLAG_OVERFLOW	0b00000000		// Flag sur CY inactif
#define	MDR1_B7_FLAG_OVERFLOW		0b10000000		// Flag sur CY (débordement de capacité sur CNTR)

// Configuration par défaut du LS7366R (MDR0 + MDR1)
#define MDR0_INITIAL_VALUE      	(MDR0_B7_FILTER_CLOCK_F1 | MDR0_B6_ASYNC_INDEX | MDR0_B5B4_NO_INDEX | MDR0_B3B2_FREE_RUNNING | MDR0_B1B0_X4)
#define MDR1_INITIAL_VALUE      	(MDR1_B7_NO_FLAG_OVERFLOW | MDR1_B6_NO_FLAG_UNDERFLOW | MDR1_B5_NO_FLAG_COMPARE | MDR1_B4_NO_FLAG_INDEX | MDR1_B2_ENABLE_COUNTING | MDR1_B1B0_COUNT_32_BITS)


/*----------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
 
 
/*----------------------------------------------------------------------------
 * Module local function prototypes
 *----------------------------------------------------------------------------*/
 
 
/*----------------------------------------------------------------------------
 * Module local functions
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 * Module public functions
 *----------------------------------------------------------------------------*/
ABIEncoder::ABIEncoder(int pin)
{
	ssPin = pin;
	pinMode(ssPin, OUTPUT);

	mdr0 = MDR0_INITIAL_VALUE, 
	mdr1 = MDR1_INITIAL_VALUE;
	
	SPI.begin();

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_WRITE | REG_MDR0);
	SPI.transfer(mdr0);
	digitalWrite(ssPin, HIGH);
	
	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_WRITE | REG_MDR1);
	SPI.transfer(mdr1);
	digitalWrite(ssPin, HIGH);

	encoderPosition = 0;
}


bool ABIEncoder::SetCountMode(CountMode_t mode)
{
	if ((mode < 0 ) || (mode >= CountMode_count)) return false;

	mdr0 = (mdr0 & 0b11111100) | mode;

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_WRITE | REG_MDR0);
	SPI.transfer(mdr0);
	digitalWrite(ssPin, HIGH);

	return true;
}

void ABIEncoder::Enable(void)
{
	mdr1 = (mdr1 & 0b11111011);	// MDR1_B2_DISABLE_COUNTING

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_WRITE | REG_MDR1);
	SPI.transfer(mdr1);
	digitalWrite(ssPin, HIGH);
}


void ABIEncoder::Disable(void)
{
	mdr1 = (mdr1 | 0b00000100);	// MDR1_B2_DISABLE_COUNTING

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_WRITE | REG_MDR1);
	SPI.transfer(mdr1);
	digitalWrite(ssPin, HIGH);	
}


long ABIEncoder::operator =(const long newEncoderPosition)
{
	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_CLEAR | REG_CNTR);
	digitalWrite(ssPin, HIGH);

	encoderPosition = newEncoderPosition;

	return encoderPosition;
}

ABIEncoder::operator long()
{
	long regOTR;

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_LOAD | REG_OTR);
	digitalWrite(ssPin, HIGH);

	digitalWrite(ssPin, LOW);
	SPI.transfer(OP_READ | REG_OTR);
	regOTR = SPI.transfer(0x00);
	regOTR <<= 8;
	regOTR |= SPI.transfer(0x00);
	regOTR <<= 8;
	regOTR |= SPI.transfer(0x00);
	regOTR <<= 8;
	regOTR |= SPI.transfer(0x00);
	digitalWrite(ssPin, HIGH);
  
	return (long)(encoderPosition + regOTR);
}

