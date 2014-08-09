#include "project.h"
#include <string.h>

// driver
#include "counter.h"
#include "ports.h"
#include "display.h"
#include "user.h"

// logic
#include "menu.h"

// Variables
u16 counter_value = 0;

// Functions
void mx_counter(u8 line);
void sx_counter(u8 line);
void display_counter(u8 line, u8 update);

void mx_counter(u8 line)
{
    // Clear function (manual set, default to 0)
	s32 user_input = 0;

	// Clear display
	clear_line(LINE2);

	// User input
	set_value(	&user_input,
				3, 0,
				0, 999,
				SETVALUE_ROLLOVER_VALUE,
				LCD_SEG_L2_2_0,
				display_value);

	counter_value = user_input;

	// Clear button flags and request full update
	button.all_flags = 0;
	display.flag.full_update = 1;
}

void sx_counter(u8 line)
{
    // DOWN button: increase
    if (button.flag.down)
    {
    	counter_value ++;
    	display_counter(line, DISPLAY_LINE_UPDATE_FULL);
    }
}

void display_counter(u8 line, u8 update)
{
	u8 * str;
    if (update == DISPLAY_LINE_UPDATE_FULL)
	{
    	str = int_to_array(counter_value, 3, 0);
		display_chars(LCD_SEG_L2_2_0, str, SEG_ON);
	}
}

