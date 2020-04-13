// let's define some things

#define PPU_CTRL		*((unsigned char*)0x2000)
#define PPU_MASK		*((unsigned char*)0x2001)
#define PPU_STATUS		*((unsigned char*)0x2002)
#define OAM_ADDRESS		*((unsigned char*)0x2003)
#define SCROLL			*((unsigned char*)0x2005)
#define PPU_ADDRESS		*((unsigned char*)0x2006)
#define PPU_DATA		*((unsigned char*)0x2007)
#define OAM_DMA			*((unsigned char*)0x4014)


#define RIGHT		0x01
#define LEFT		0x02
#define DOWN		0x04
#define UP			0x08
#define START		0x10
#define SELECT		0x20
#define B_BUTTON	0x40
#define A_BUTTON	0x80


// Globals
// our startup code initialized all values to zero
#pragma bss-name(push, "ZEROPAGE")
unsigned char NMI_flag;
unsigned char Frame_Count;
unsigned char index;
unsigned char index4;
unsigned char state;
unsigned char state4;
unsigned char joypad1;
unsigned char joypad1old;
unsigned char joypad1test; 
unsigned char joypad2; 
unsigned char joypad2old;
unsigned char joypad2test;

#pragma bss-name(push, "OAM")
unsigned char SPRITES[256];
// OAM equals ram addresses 200-2ff




const unsigned char PALETTE[]={
0x1f, 0x00, 0x10, 0x20
};




// Prototypes
void All_Off (void);
void All_On (void);
void Reset_Scroll (void);
void Load_Palette (void);
void update_Sprites (void);
unsigned char move_logic (unsigned char left_volume, unsigned char right_volume);
unsigned char play_level(unsigned char level_number, unsigned char left_volume, unsigned char right_volume, unsigned char  goal);
void show_level_and_goal (unsigned char level_number, unsigned char goal);
void show_text(unsigned char level_number, unsigned char goal);
void show_win_level(void);
void clear_screen(void);