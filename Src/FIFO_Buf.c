// Generic FIFO Buffer
// By Cory Schwarzmiller 2016


//------------------------------------------------------------------------------
// Includes
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f3xx_hal.h"
#include "FIFO_Buf.h"
//#include "usart.h"

//------------------------------------------------------------------------------
// Private Types


//------------------------------------------------------------------------------
// Global Vars Private

//------------------------------------------------------------------------------
// Private Function Prototypes


//------------------------------------------------------------------------------
// Public Function Definitions

//------------------------------------------------------------------------------
// Initialize a general FIFO Buffer
void FIFO_Buf_Init(FIFO_Buf_t *Buf_p, uint8_t *buf, uint32_t capacity)
{
	if (buf != NULL){
		Buf_p->buf = buf;
	}
	
	Buf_p->capacity = capacity;
	Buf_p->head = 0;
	Buf_p->tail = 0;
}

//------------------------------------------------------------------------------
// Initialize a DMA UART RX type specific FIFO Buffer
void FIFO_Buf_Init_DMA_UART_RX(FIFO_Buf_t *Buf_p, uint8_t *buf, uint32_t capacity, volatile uint32_t *ndtr_reg_p)
{
	if (buf != NULL){
		Buf_p->buf = buf;
	}
	
	if (ndtr_reg_p != NULL){
		Buf_p->ndtr_reg_p = ndtr_reg_p;
	}
	
	Buf_p->capacity = capacity;
	Buf_p->head = 0;
	Buf_p->tail = 0;
}


//------------------------------------------------------------------------------
// Interrupt safe remove (only modifies tail) - returns 1 if char was removed
int32_t FIFO_Buf_Remove(FIFO_Buf_t *Buf_p, volatile uint8_t * c)
{
    if(Buf_p->tail != Buf_p->head)
    {
        *c = Buf_p->buf[Buf_p->tail];
        Buf_p->tail = (Buf_p->tail + 1) % Buf_p->capacity;
        return 1;
    } else {
        return 0;
    }
}


//------------------------------------------------------------------------------
// Interrupt safe add (only modifies head)
int32_t FIFO_Buf_Add(FIFO_Buf_t *Buf_p, uint8_t c)
{
    uint32_t next_head = (Buf_p->head + 1) % Buf_p->capacity;
    if(next_head != Buf_p->tail)
    {
        Buf_p->buf[Buf_p->head] = c;
        Buf_p->head = next_head;
        if(((Buf_p->head + 1) % Buf_p->capacity) == Buf_p->tail)
            return 0;    // Now full
        else
            return 1;    // Still room
    }
    else
    {
        return 0;        // Full
    }
}


//------------------------------------------------------------------------------
// Interrupt safe remove for circular DMA RX FIFO (only modifies tail)
// returns "1" if a char has been removed, "0" if not
int32_t FIFO_Buf_Remove_DMA(FIFO_Buf_t *Buf_p, volatile uint8_t * c)
{
	Buf_p->head = Buf_p->capacity - *Buf_p->ndtr_reg_p;	// Get head from DMA "number to go" register
	
	if (Buf_p->head != Buf_p->tail){
		//invalidate_dcache_range((uint32_t)&Buf_p->buf[Buf_p->tail], 1);	// Needed since DMA is accessing this asynchronously
		*c = Buf_p->buf[Buf_p->tail++];
		if (Buf_p->tail >= Buf_p->capacity)
			Buf_p->tail = 0;
		return 1;
	} else {
		return 0;
	}
}


//------------------------------------------------------------------------------
// Clear the buffer by moving the tail to the head without processing any data.
void FIFO_Buf_Clear(FIFO_Buf_t *Buf_p)
{
	Buf_p->tail = Buf_p->head;
}



