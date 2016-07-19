/*
	_ButtonBuffer.h
	
	History:
	
	Version 1.0: (2016-07-XY) - Initial Release	(Sascha Heegt)
*/

#ifndef _ButtonBuffer_H
#define _ButtonBuffer_H

const byte BUFFER_SIZE = 10;

class _ButtonBuffer
{
	public:
		_ButtonBuffer();
	
		// Variables
		
		
		// Methods
		void begin();
		bool available();
		byte count();
		void add(byte buttonBit);
		byte next();
		void flush();

	private:
		byte _buffer[BUFFER_SIZE];
		byte _readIndex;
		byte _writeIndex;
		byte _length;
		byte _count;
};

#endif