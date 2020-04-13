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
unsigned char level_number = 0;
unsigned char max_left = 0;
unsigned char max_right = 0;
unsigned char to_win = 0;

const unsigned char WIN_LEVEL_TEXT[]={"LEVEL PASSED!"};
const unsigned char LEVEL_NUMBER_TEXT[]={"LEVEL:"};
const unsigned char GOAL_TEXT[]={"GOAL:"};
const unsigned char LEVELS[] = {1, 5, 3, 4, 
								2, 7, 5, 6,
								3, 7, 4, 2};
const unsigned char LEVEL_COUNT = 3;

unsigned char cycle = 0;
unsigned char status = 0;
	
char int_to_char(char i) {
	return i + '0';
}	

void show_text(unsigned char level_number, unsigned char goal) {
	//	turn off the screen
	All_Off();
	
	//	load the palette
	Load_Palette();

	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x6b;  	//	about the middle of the screen
	PPU_DATA = int_to_char(left);
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x73;  	//	about the middle of the screen
	PPU_DATA = int_to_char(right);	

	if (level_number > 0) {
		show_level_and_goal(level_number, goal);
	}

	//	reset the scroll position	
	Reset_Scroll();
	
	//	turn on screen
	All_On();
}

void show_level_and_goal (unsigned char level_number, unsigned char goal){
	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x01;  	//	left up corner
	for( index = 0; index < sizeof(LEVEL_NUMBER_TEXT); ++index ){
		PPU_DATA = LEVEL_NUMBER_TEXT[index];
	}
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x07;  	//	left up corner
	PPU_DATA = int_to_char(level_number);	

	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x19;  	//	right up corner
	for( index = 0; index < sizeof(GOAL_TEXT); ++index ){
		PPU_DATA = GOAL_TEXT[index];
	}
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x1e;  	//	right up corner
	PPU_DATA = int_to_char(goal);	
}

void show_win_level(void) {
	//	turn off the screen
	All_Off();
	
	//	load the palette
	Load_Palette();
		
	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0xc9;  	//	about the middle of the screen
	for( index = 0; index < sizeof(WIN_LEVEL_TEXT); ++index ){
		PPU_DATA = WIN_LEVEL_TEXT[index];
	}
				
	//	reset the scroll position	
	Reset_Scroll();
	
	//	turn on screen
	All_On();
}

void clear_screen(void) {
	//	turn off the screen
	All_Off();
	
	//load the text
	PPU_ADDRESS = 0x21;  	//	set an address in the PPU of 0x21ca
	PPU_ADDRESS = 0x00;  	//	about the middle of the screen
	for( index = 0; index < 255; ++index ){
		PPU_DATA = 0;
	}
				
	//	reset the scroll position	
	Reset_Scroll();
	
	//	turn on screen
	All_On();
}

unsigned char wait_start_button(){
	Get_Input();
	if ((joypad1 & START)!= 0){
		return 1;
	}
	return 0;
}

unsigned char play_level(unsigned char level_number, unsigned char left_volume, unsigned char right_volume, unsigned char  goal){
	//every_frame();	// moved this to the nmi code in reset.s for greater stability
	unsigned char s = 0;
	while (NMI_flag == 0); // wait till NMI
	Get_Input();
	s = move_logic(left_volume, right_volume);
	if (s > 0){
		show_text(level_number, goal);
		s = 0;
	}
	NMI_flag = 0;

	if ((left == goal)||(right == goal)){
		show_win_level();
		while (s == 0){
			s = wait_start_button();
		}
		return 1;
	}
	return 0;
}


void main (void) {
	show_text(0, 0);

	while (1){ 		// infinite loop
		for (cycle = 0; cycle < LEVEL_COUNT; cycle++) {
			left = 0;
			right = 0;
			level_number = LEVELS[cycle*4+0];
			max_left = LEVELS[cycle*4+1];
			max_right = LEVELS[cycle*4+2];
			to_win = LEVELS[cycle*4+3];
			status = 0;
			show_text(level_number, to_win);
			while (status == 0){
				status = play_level(level_number, max_left, max_right, to_win);
			}
			clear_screen();
		}
		break;
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


unsigned char move_logic (unsigned char left_volume, unsigned char right_volume) {
	if ((joypad1 & RIGHT) != 0){
		to_move = right_volume - right;
		if (left > to_move){
			left -= to_move;
			right += to_move;
		} else {
			right += left;
			left = 0;
		}
		return 1;
	}
	if ((joypad1 & LEFT) != 0){
		to_move = left_volume - left;
		if (right > to_move){
			right -= to_move;
			left += to_move;
		} else {
			left += right;
			right = 0;
		}
		return 1;
	}
	if ((joypad1 & B_BUTTON) != 0){
		if ((joypad1 & DOWN) != 0){
			left = left_volume;
			return 1;
		}
		if ((joypad1 & UP) != 0){
			left = 0;
			return 1;
		}
	}
	if ((joypad1 & A_BUTTON) != 0){
		if ((joypad1 & DOWN) != 0){
			right = right_volume;
			return 1;
		}
		if ((joypad1 & UP) != 0){
			right = 0;
			return 1;
		}
	}
	return 0;
}





