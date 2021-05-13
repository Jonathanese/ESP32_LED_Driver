/*
	Jonathan P Weber
	Library for creating formatted and filtered debug outputs

	Modified for ESP8266

*/

// DebugMessage.h

#ifndef _DEBUGMESSAGE_h
#define _DEBUGMESSAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DM_SEND 1
#define DM_RECEIVE 2
#define DM_SSEND 4
#define DM_SRECEIVE 8
#define DM_ERROR 16
#define DM_INFO 32
#define DM_TIMING 64


class Debug_t
{
public:
	Debug_t(uint32_t baud, uint16_t mask);
	~Debug_t();
	void begin();
	void Message(uint16_t DM_TYPE, String message);
	void Message(uint16_t DM_TYPE, String message, bool newline);
	void setBaud(uint32_t baud);
	void setFilter(uint16_t mask);
	void setANSI(bool ansi);

private:

	struct {
		bool SEND : 1;
		bool RECEIVE : 1;
		bool SSEND : 1;
		bool SRECEIVE : 1;
		bool ERROR : 1;
		bool INFO : 1;
		bool TIMING : 1;
		bool ANSI : 1;
	}ConfigMask;

	uint32_t BaudRate;
};

extern Debug_t Debug;

#endif
