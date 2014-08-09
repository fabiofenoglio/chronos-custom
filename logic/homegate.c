// system
#include <project.h>

// driver
#include "rf1a.h"
#include "simpliciti.h"
#include "display.h"
#include "ports.h"

// logic
#include "homegate.h"
#include "timer.h"
#include "radio.h"
#include "user.h"

// Define section

#define HOMEGATE_ALLOW_WITH_LOW_BATT	(1)

#define st(x)                      	do { x } while (__LINE__ == -1)
#define ENTER_CRITICAL_SECTION(x)   st(x = __get_interrupt_state(); __disable_interrupt(); )
#define EXIT_CRITICAL_SECTION(x)    __set_interrupt_state(x)

u8 homegate_burst_num = 2;
u8 homegate_safemode = 1;

// *************************************************************************************************
// Prototypes section
void homegate_setup_radio();
void mx_hgate(u8 line);
void sx_hgate(u8 line);
void display_hgate(u8 line, u8 update);

void homegate_transmit();

// Dummy data
static u8 gateseq[] = { 0x00, 0x00, 0x00, 0x00, 0x0B, 0x25, 0xB6, 0x5B, 0x65, 0x80 };

void display_hgate(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
		display_chars(LCD_SEG_L2_4_0, (u8 *) " GATE ", SEG_ON);
    }
}

void mx_hgate(u8 line)
{
	// do nothing
}

void sx_hgate(u8 line)
{
    // Exit if battery voltage is too low for radio operation
#if !ALLOW_WITH_LOW_BATT
    if (sys.flag.low_battery) return;
#endif

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;

    display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_OFF);
	display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_OFF);

	homegate_transmit();

    // Clear icons
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);

    // Debounce button event
    Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));

   	BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;
}

void homegate_setup_radio()
{
	WriteSingleReg(IOCFG0,0x06);  //GDO0 Output Configuration
	WriteSingleReg(FIFOTHR,0x47); //RX FIFO and TX FIFO Thresholds
	WriteSingleReg(PKTLEN,4*sizeof(gateseq)+1); //Packet Length
	WriteSingleReg(PKTCTRL0,0x00);//Packet Automation Control
	WriteSingleReg(FSCTRL1,0x06); //Frequency Synthesizer Control
	WriteSingleReg(FREQ2,0x10);   //Frequency Control Word, High Byte
	WriteSingleReg(FREQ1,0xB0);   //Frequency Control Word, Middle Byte
	WriteSingleReg(FREQ0,0x71);   //Frequency Control Word, Low Byte
	WriteSingleReg(MDMCFG4,0x26); //Modem Configuration
	WriteSingleReg(MDMCFG3,0xE4); //Modem Configuration
	WriteSingleReg(MDMCFG2,0x30); //Modem Configuration
	WriteSingleReg(MDMCFG1,0x02); //Modem Configuration
	WriteSingleReg(DEVIATN,0x62); //Modem Deviation Setting
	WriteSingleReg(MCSM0,0x10);   //Main Radio Control State Machine Configuration
	WriteSingleReg(FOCCFG,0x1D);  //Frequency Offset Compensation Configuration
	WriteSingleReg(AGCCTRL1,0x00);//AGC Control
	WriteSingleReg(WORCTRL,0xFB); //Wake On Radio Control
	WriteSingleReg(FREND0,0x11);  //Front End TX Configuration
	WriteSingleReg(FSCAL3,0xE9);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL2,0x2A);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL1,0x00);  //Frequency Synthesizer Calibration
	WriteSingleReg(FSCAL0,0x1F);  //Frequency Synthesizer Calibration
	WriteSingleReg(TEST2,0x81);   //Various Test Settings
	WriteSingleReg(TEST1,0x35);   //Various Test Settings
	WriteSingleReg(TEST0,0x09);   //Various Test Settings
}

void homegate_transmit()
{
	u8* data = gateseq;
	u8 len = sizeof(gateseq);

	u8 c = len;
	u8 wordCount = 0;
	u8 burst_count;
	u16 int_state;

    // Prepare radio for RF communication
    open_radio();
    Timer0_A4_Delay(CONV_MS_TO_TICKS(1));
    
    homegate_setup_radio();

    if (homegate_safemode)
	{
		ENTER_CRITICAL_SECTION(int_state);
	}

	RF1AIES |= BIT9;
	RF1AIFG &= ~BIT9;
	
	for (burst_count = 0; burst_count < homegate_burst_num; burst_count ++)
	{
		Strobe(RF_SFTX);
		while (!(RF1AIFCTL1 & RFINSTRIFG)) ;
		RF1AINSTRB = 0x7F;

		for (wordCount = 0; wordCount< 4; wordCount ++)
		{
			c = len;
			data = gateseq;
			do
			{
				while (!(RF1AIFCTL1 & RFDINIFG)) ;
				RF1ADINB = *data;
				data++;
			}
			while ( --c );
		}

		while (!(RF1AIFCTL1 & RFDINIFG)) ;
		RF1ADINB = 0x00;

		Strobe(RF_STX);
		while (!(RF1AIFG & BIT9));
		RF1AIFG &= ~BIT9;

		Timer0_A4_Delay(CONV_MS_TO_TICKS(10));
	}

	if (homegate_safemode)
	{
		EXIT_CRITICAL_SECTION(int_state);
	}

	// Powerdown radio
    close_radio();
}
