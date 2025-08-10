/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FIFO_BUF_H
#define __FIFO_BUF_H

#include <stdlib.h>
#include <stdint.h>		// for Standard Data Types

//---------------------------------------------------------------------
// Public Types


typedef struct {
	uint8_t handle;
	uint8_t *buf;
	uint32_t capacity;
	uint32_t head;
	uint32_t tail;
	volatile uint32_t *ndtr_reg_p;	// Pointer to the NDR register for DMA enabled FIFOs
} FIFO_Buf_t;


//------------------------------------------------------------------------------
// Public Functions

// Initialize a general FIFO Buffer
void FIFO_Buf_Init(FIFO_Buf_t *Buf_p, uint8_t *buf, uint32_t capacity);

// Initialize a DMA UART RX type specific FIFO Buffer
void FIFO_Buf_Init_DMA_UART_RX(FIFO_Buf_t *Buf_p, uint8_t *buf, uint32_t capacity, volatile uint32_t *ndtr_reg_p);
	
// Interrupt safe remove (only modifies tail) - returns 1 if char was removed
int32_t FIFO_Buf_Remove(FIFO_Buf_t *Buf_p, volatile uint8_t * c);

// Interrupt safe add (only modifies head)
int32_t FIFO_Buf_Add(FIFO_Buf_t *Buf_p, uint8_t c);

// Interrupt safe remove for circular DMA RX FIFO (only modifies tail)
// returns "1" if a char has been removed, "0" if not
int32_t FIFO_Buf_Remove_DMA(FIFO_Buf_t *Buf_p, volatile uint8_t * c);

// Clear the buffer by moving the tail to the head without processing any data.
void FIFO_Buf_Clear(FIFO_Buf_t *Buf_p);


#endif
