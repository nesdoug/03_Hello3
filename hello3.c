/*	simple Hello World, for cc65, for NES
 *  writing to the screen with rendering ON, via a vram buffer
 *	using neslib
 *	Doug Fraker 2018
 */	
 

// using a buffer, we have more flexibility, since we can adjust the 
// screen position of a PPU write, and can piggy back multiple data 
// sets into 1 push, doing more than 1 update per frame, and 
// the data sets can have zeroes, since they are not zero terminated

#include "LIB/neslib.h"
#include "LIB/nesdoug.h"

#define BLACK 0x0f
#define DK_GY 0x00
#define LT_GY 0x10
#define WHITE 0x30
// there's some oddities in the palette code, black must be 0x0f, white must be 0x30


const unsigned char palette[]={
BLACK, DK_GY, LT_GY, WHITE,
0,0,0,0,
0,0,0,0,
0,0,0,0
}; 


// example of sequential vram data
const unsigned char text[]={
'H',
'E',
'L',
'L',
'O',
' ',
'W',
'O',
'R',
'L',
'D',
'!',
}; // not a c string, so no zero terminating


// example of single byte write
const unsigned char LETTERA='A';

//global variable
int address;






void main (void) {

	ppu_on_all(); // turn on screen
	
	pal_bg(palette); //	load the palette, this can be done any time, even with rendering on
					 // but, it's competing with the vram_buffer for v-blank time
					 // so, let's wait a v-blank first, so all the palette changes are done
					 
	ppu_wait_nmi(); // wait
	
	// now fill the vram_buffer
	
	set_vram_buffer(); // points ppu update to vram_buffer, do this at least once
						 
	one_vram_buffer(LETTERA, NTADR_A(2,3)); // pushes 1 byte worth of data to the vram_buffer
	one_vram_buffer(0x42, NTADR_A(5,6)); // another 1 byte write, letter B
	
	// optionally, you could use this function to get the ppu address at run time
	address = get_ppu_addr(0,0x38,0xc0); // (char nt, char x, char y);
	one_vram_buffer('C', address);	// another 1 byte write
	
	multi_vram_buffer_horz(text, sizeof(text), NTADR_A(10,7)); // pushes 12 bytes, horz
	multi_vram_buffer_horz(text, sizeof(text), NTADR_A(12,12)); // lower
	multi_vram_buffer_horz(text, sizeof(text), NTADR_A(14,17)); // lower still
	
	multi_vram_buffer_vert(text, sizeof(text), NTADR_A(10,7)); // vertical
	
	// we've done 51 bytes of transfer to the ppu in 1 v-blank
	
	// do not try to push much more than 30 non-sequential or 70 sequential bytes at once
	
	ppu_wait_nmi(); // waits till nmi, and push new updates to the ppu
	
	
	while (1){
		// infinite loop
		// game code can go here later.
		
	}
}
	
	