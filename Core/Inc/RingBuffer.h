/*
 * RingBuffer.h
 *
 *  Created on: Feb 4, 2025
 *      Author: hayden
 */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_
#include "main.h"


#ifdef __cplusplus

template <size_t SIZE, typename T>
class RingBuffer{
public:
	void push(T obj){
		data[head] = obj;
		head = (head + 1) % SIZE;
	}
	T& operator[](uint16_t idx){
		return data[(head + idx) % SIZE];
	}
private:
	T data[SIZE];
	uint16_t head = 0;
};

#endif

#ifdef __cplusplus
#define EXTERNC extern "c"
#else
#define EXTERNC
#endif

/* ----C-FRIENDLY BINDINGS GO HERE------*/

#undef EXTERNC

#endif /* INC_RINGBUFFER_H_ */
