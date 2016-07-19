/*
	ButtonBuffer.cpp
*/

#include <Arduino.h>
#include <_ButtonBuffer.h>

_ButtonBuffer::_ButtonBuffer()
{

}

void _ButtonBuffer::begin()
{
	_readIndex	= 0;
	_writeIndex = 0;
	_count 		= 0;
	_length		= BUFFER_SIZE;
}

void _ButtonBuffer::add(byte buttonBit)
{
	
	if(_writeIndex == _length) { _ButtonBuffer::flush(); }

	_buffer[_writeIndex] = buttonBit;
	_writeIndex++;
	_count++;
	
	return;
}

byte _ButtonBuffer::next()
{
	byte rBit = 255;
	
	if(_readIndex == _length || _count == 0) { _ButtonBuffer::flush(); }	
	
		rBit = _buffer[_readIndex];
		_readIndex++;
		_count--;
		
	return rBit;
}

void _ButtonBuffer::flush()
{
	_readIndex	= 0;
	_writeIndex = 0;
	_count		= 0;
}

bool _ButtonBuffer::available()
{
	byte available;
	
	available = (_count == 0) ? false : true;
	
	return available;
}

byte _ButtonBuffer::count()
{
	return _count;
}