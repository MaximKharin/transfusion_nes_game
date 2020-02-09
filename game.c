#include "DEFINE.c"
//defines variables, constants, and function prototypes

void Get_Input (void); 
/*	this calls an asm function, written in asm4c.s
 *	it will read both joypads and store their reads in joypad1 and joypad2
 *	The buttons come in the order of A, B, Select, Start, Up, Down, Left, Right
 */

unsigned char left = 0;
unsigned char right = 0;
unsigned char to_move = 0;

const unsigned char MAX_LEFT = 5;
const unsigned char MAX_RIGHT = 3;
const unsigned char TO_WIN = 4;
const unsigned char WIN_TEXT[]={"YOU WON!"};
	
char int_to_char(char i) {
	return i + '0';
}	

void show_text(void) {
	//	turn off the screen
	All_Off();
	
	//	load the palette
	Load_Palette();

	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x4b;  	//	about the middle of the screen
	PPU_DATA = int_to_char(left);
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x54;  	//	about the middle of the screen
	PPU_DATA = int_to_char(right);	

	//	reset the scroll position	
	Reset_Scroll();
	
	//	turn on screen
	All_On();
}

void show_win(void) {
	//	turn off the screen
	All_Off();
	
	//	load the palette
	Load_Palette();
		
	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0xcc;  	//	about the middle of the screen
	for( index = 0; index < sizeof(WIN_TEXT); ++index ){
		PPU_DATA = WIN_TEXT[index];
	}
				
	//	reset the scroll position	
	Reset_Scroll();
	
	//	turn on screen
	All_On();
}

void main (void) {
	show_text();
	while (1){ 		// infinite loop
		while (NMI_flag == 0); // wait till NMI
		
		//every_frame();	// moved this to the nmi code in reset.s for greater stability
		Get_Input();
		move_logic();
		show_text();

		if ((left == TO_WIN)||(right == TO_WIN)){
			show_win();
		}
		
		NMI_flag = 0;
	}
}
	
// inside the startup code, the NMI routine will ++NMI_flag and ++Frame_Count at each V-blank
	

	
void All_Off (void) {
	PPU_CTRL = 0;
	PPU_MASK = 0; 
}
	
	
void All_On (void) {
	PPU_CTRL = 0x90; // screen is on, NMI on
	PPU_MASK = 0x1e; 
}
	
	
void Reset_Scroll (void) {
	PPU_ADDRESS = 0;
	PPU_ADDRESS = 0;
	SCROLL = 0;
	SCROLL = 0;
}
	
	
void Load_Palette (void) {
	PPU_ADDRESS = 0x3f;
	PPU_ADDRESS = 0x00;
	for( index = 0; index < sizeof(PALETTE); ++index ){
		PPU_DATA = PALETTE[index];
	}
}


void move_logic (void) {
	if ((joypad1 & RIGHT) != 0){
		to_move = MAX_RIGHT - right;
		if (left > to_move){
			left -= to_move;
			right += to_move;
		} else {
			right += left;
			left = 0;
		}
	}
	if ((joypad1 & LEFT) != 0){
		to_move = MAX_LEFT - left;
		if (right > to_move){
			right -= to_move;
			left += to_move;
		} else {
			left += right;
			right = 0;
		}
	}
	if ((joypad1 & B_BUTTON) != 0){
		if ((joypad1 & DOWN) != 0){
			left = MAX_LEFT;
		}
		if ((joypad1 & UP) != 0){
			left = 0;
		}
	}
	if ((joypad1 & A_BUTTON) != 0){
		if ((joypad1 & DOWN) != 0){
			right = MAX_RIGHT;
		}
		if ((joypad1 & UP) != 0){
			right = 0;
		}
	}
}





