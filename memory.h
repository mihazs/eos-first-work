#ifndef MEMORY_H
#define	MEMORY_H

#include "types.h"

#define	MAX_HEAP_SIZE		0x200
#define NEAR


#define	_MAX_SEGMENT_SIZE	0x7F
#define _MAX_HEAP_SIZE 	MAX_HEAP_SIZE-1




/*********************************************************************
 * Segment header data type
 ********************************************************************/
typedef union _SALLOC
{
	byte byte;
	struct _BITS
	{
		unsigned count:7;
		unsigned alloc:1;	
	}bits;
}SALLOC;





/*********************************************************************
 * Reserve the memory heap
 ********************************************************************/
#pragma 	udata	_SRAM_ALLOC_HEAP
byte _uDynamicHeap[MAX_HEAP_SIZE];


/*********************************************************************
 * Set the memory type
 ********************************************************************/
#if defined(NEAR_MODEL)
#pragma		udata access	_SRAM_ALLOC
#else
#pragma		udata _SRAM_ALLOC
#endif


byte * SRAMalloc(byte nBytes);
void SRAMfree(byte * pSRAM);
void SRAMInitHeap(void);


#endif	/* MEMORY_H */

