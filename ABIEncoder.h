#ifndef __ABIENCODER_H__
#define __ABIENCODER_H__

/*----------------------------------------------------------------------------
 * Module dependencies
 *----------------------------------------------------------------------------*/
 
 
/*----------------------------------------------------------------------------
 * Module types and definitions
 *----------------------------------------------------------------------------*/
enum CountMode
{
  CountMode_ClockDir = 0,
  CountMode_x1,
  CountMode_x2,
  CountMode_x4,
  CountMode_count
};
typedef enum CountMode CountMode_t;


class ABIEncoder
{
	public:
		ABIEncoder(int pin);							// Constructeur permettant de choisir un SS différent du défaut sur la broche 10
		ABIEncoder(void) : ABIEncoder(10) {};			// Constructeur utilisant un SS sur la broche 10 par défaut
		bool SetCountMode(CountMode_t mode);			// Définition du mode de comptage: clock/dir, x1, x2 ou x4
		void Enable(void);								// Active le comptage
		void Disable(void);								// Arrête le comptage
		long operator =(const long newEncoderPosition);	// Définit la nouvelle position du codeur
		operator long();								// Retourne la position actuelle du codeur
		
	protected:
		long	encoderPosition;
		int		ssPin;

  private:
		unsigned char mdr0, mdr1;
};


/*---------------------------------------------------------------------------
 * Module globals
 *----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
 * Module prototype functions
 *---------------------------------------------------------------------------*/


#endif	// __ABIENCODER_H__

