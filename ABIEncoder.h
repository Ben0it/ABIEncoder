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


class ABIEncoder {
	public:
		ABIEncoder(int pin);
		ABIEncoder(void) : ABIEncoder(10) {};
		bool SetCountMode(CountMode_t mode);
		void Enable(void);
		void Disable(void);
		long operator =(const long newEncoderPosition);
		operator long();
		
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

