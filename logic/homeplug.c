// system
#include <project.h>

// driver
#include "rf1a.h"
#include "simpliciti.h"
#include "display.h"
#include "ports.h"

// logic
#include "homeplug.h"
#include "timer.h"
#include "radio.h"
#include "user.h"

// Define section
#define HOMEPLUG_ALLOW_WITH_LOW_BATT (1)

#define HOMEPLUG_CHANNEL_A 0x00
#define HOMEPLUG_CHANNEL_B 0x01
#define HOMEPLUG_CHANNEL_C 0x02
#define HOMEPLUG_CHANNEL_D 0x03

#define HOMEPLUG_CHANNEL_ON 1
#define HOMEPLUG_CHANNEL_OFF 0

#define st(x)                      	do { x } while (__LINE__ == -1)
#define ENTER_CRITICAL_SECTION(x)   st(x = __get_interrupt_state(); __disable_interrupt(); )
#define EXIT_CRITICAL_SECTION(x)    __set_interrupt_state(x)

#define HOMEPLUG_BIT_0 0x88
#define HOMEPLUG_BIT_F 0x8E

u8 homeplug_address = 0x18;
u8 homeplug_burst_num = 3;
u8 homeplug_safemode = 1;

// *************************************************************************************************
// Prototypes section
void mx_homeplug(u8 line);
void sx_homeplug(u8 line);
void display_homeplug(u8 line, u8 update);

void homeplug_setup_radio();
void tx_homeplug_sequence(u8 address, u8 chnum, u8 on_off);

void display_homeplug(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
		display_chars(LCD_SEG_L2_4_0, (u8 *) "PLUG ", SEG_ON);
    }
}

void mx_homeplug(u8 line)
{
	s32 user_input;

	// Ask channel num
    clear_line(LINE2);
    display_chars(LCD_SEG_L2_4_2, (u8 *) "CH", SEG_ON);

    user_input = homeplug_address;
	set_value(	&user_input,
				2, 0,
				0, 63,
				SETVALUE_ROLLOVER_VALUE,
				LCD_SEG_L2_1_0,
				display_value);

	homeplug_address = user_input;

	// Ask burst number
    clear_line(LINE2);
    display_chars(LCD_SEG_L2_4_2, (u8 *) "BN", SEG_ON);

    user_input = homeplug_burst_num;
	set_value(	&user_input,
				2, 0,
				1, 10,
				SETVALUE_ROLLOVER_VALUE,
				LCD_SEG_L2_1_0,
				display_value);

	homeplug_burst_num = user_input;

	// Ask safe mode
	clear_line(LINE2);
	display_chars(LCD_SEG_L2_4_0, (u8 *) "SAFE", SEG_ON);

	user_input = homeplug_safemode;
	set_value(	&user_input,
				2, 1,
				0, 1,
				SETVALUE_ROLLOVER_VALUE,
				LCD_SEG_L2_1_0,
				display_value);

	homeplug_safemode = user_input;

	button.all_flags = 0;
	display.flag.full_update = 1;
}

void sx_homeplug(u8 line)
{
	u8 chnum;
	u8 on_off;

    s32 user_input = 0;

	// Exit if battery voltage is too low for radio operation
#if !HOMEPLUG_ALLOW_WITH_LOW_BATT
    if (sys.flag.low_battery) return;
#endif

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;

	// Clear display
    clear_line(LINE2);

	// User input
    display_chars(LCD_SEG_L2_4_0, (u8 *) "CODE", SEG_ON);
	set_value(	&user_input,
				2, 1,
				0, 9,
				SETVALUE_ROLLOVER_VALUE,
				LCD_SEG_L2_1_0,
				display_value);

	if ((user_input % 2) == 0)
		on_off = HOMEPLUG_CHANNEL_ON;
	else
		on_off = HOMEPLUG_CHANNEL_OFF;

	chnum = user_input >> 1;

	// Clear button flags and request full update
	button.all_flags = 0;
	display.flag.full_update = 1;

    display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_OFF);

	tx_homeplug_sequence(homeplug_address, chnum, on_off);

    // Clear icons
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);

    // Debounce button event
    Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));

   	BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;
}

void tx_homeplug_sequence(u8 address, u8 chnum, u8 on_off)
{
	u8 plugseq[] = {
				 0, 0, 0, 0, 0,
				 HOMEPLUG_BIT_F, HOMEPLUG_BIT_F, HOMEPLUG_BIT_F, HOMEPLUG_BIT_F,
	             HOMEPLUG_BIT_F,
	             HOMEPLUG_BIT_F, HOMEPLUG_BIT_F,
	             0x80, 0x00, 0x00, 0x00};

	u8 * data = plugseq;
	u8 len = sizeof(plugseq);

	u8 c = len;
	u8 wordCount = 0;
	u8 burst_count;
	u16 int_state;

	// Prepare packet
	plugseq[0] = (address & 0x10 ? HOMEPLUG_BIT_0 : HOMEPLUG_BIT_F);
	plugseq[1] = (address & 0x08 ? HOMEPLUG_BIT_0 : HOMEPLUG_BIT_F);
	plugseq[2] = (address & 0x04 ? HOMEPLUG_BIT_0 : HOMEPLUG_BIT_F);
	plugseq[3] = (address & 0x02 ? HOMEPLUG_BIT_0 : HOMEPLUG_BIT_F);
	plugseq[4] = (address & 0x01 ? HOMEPLUG_BIT_0 : HOMEPLUG_BIT_F);

	plugseq[5 + chnum] = HOMEPLUG_BIT_0;

	if (on_off == HOMEPLUG_CHANNEL_ON)
		plugseq[10] = HOMEPLUG_BIT_0;
	else
		plugseq[11] = HOMEPLUG_BIT_0;

	// Prepare radio for RF communication
	open_radio();
	Timer0_A4_Delay(CONV_MS_TO_TICKS(1));

	homeplug_setup_radio();

	if (homeplug_safemode)
	{
		ENTER_CRITICAL_SECTION(int_state);
	}

	RF1AIES |= BIT9;
	RF1AIFG &= ~BIT9;

	for (burst_count = 0; burst_count < homeplug_burst_num; burst_count ++)
	{
		Strobe(RF_SFTX);
		while (!(RF1AIFCTL1 & RFINSTRIFG)) ;
		RF1AINSTRB = 0x7F;

		for (wordCount = 0; wordCount< 4; wordCount ++)
		{
			c = len;
			data = plugseq;
			do
			{
				while (!(RF1AIFCTL1 & RFDINIFG)) ;
				RF1ADINB = ~ ( *data );
				data++;
			}
			while ( --c );
		}

		Strobe(RF_STX);
		while (!(RF1AIFG & BIT9));
		RF1AIFG &= ~BIT9;

		Timer0_A4_Delay(CONV_MS_TO_TICKS(10));
	}

	if (homeplug_safemode)
	{
		EXIT_CRITICAL_SECTION(int_state);
	}

	// Powerdown radio
	close_radio();
}

// Rf settings for CC430

void homeplug_setup_radio()
{
	WriteSingleReg(IOCFG0,   0x06);  //GDO0 Output Configuration
	WriteSingleReg(FIFOTHR,  0x47); //RX FIFO and TX FIFO Thresholds
	WriteSingleReg(PKTLEN,   64); //Packet Length
	WriteSingleReg(PKTCTRL0, 0x00); //Packet Automation Control
	WriteSingleReg(FSCTRL1,  0x06); //Frequency Synthesizer Control
	WriteSingleReg(FREQ2,    0x10);   //Frequency Control Word, High Byte
	WriteSingleReg(FREQ1,    0xB0);   //Frequency Control Word, Middle Byte
	WriteSingleReg(FREQ0,    0x71);   //Frequency Control Word, Low Byte
	WriteSingleReg(MDMCFG4,  0xF6); //Modem Configuration
	WriteSingleReg(MDMCFG3,  0xF8); //Modem Configuration
	WriteSingleReg(MDMCFG2,  0x33); //Modem Configuration
	WriteSingleReg(MDMCFG1,  0x22); //Modem Configuration
	WriteSingleReg(DEVIATN,  0x62); //Modem Deviation Setting
	WriteSingleReg(MCSM0,    0x10);   //Main Radio Control State Machine Configuration
	WriteSingleReg(FOCCFG,   0x1D);  //Frequency Offset Compensation Configuration
	WriteSingleReg(AGCCTRL1, 0x00); //AGC Control
	WriteSingleReg(WORCTRL,  0xFB); //Wake On Radio Control
	WriteSingleReg(FREND0,   0x11);  //Front End TX Configuration
	WriteSingleReg(FSCAL3,   0xE9);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL2,   0x2A);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL1,   0x00);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL0,   0x1F);  //Frequency Synthesizer Calibration
	WriteSingleReg(TEST2,    0x81);   //Various Test Settings
	WriteSingleReg(TEST1,    0x35);   //Various Test Settings
	WriteSingleReg(TEST0,    0x09);   //Various Test Settings
}
