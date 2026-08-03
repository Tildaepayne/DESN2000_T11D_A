#include "display.h"
#include "../lcd/lcd_hw.h"
#include "../lcd/lcd_grph.h"
#include "../lcd/lcd_cfg.h"
#include "../lcd/sdram.h"

static void display_number(unsigned short x,
                           unsigned short y,
                           unsigned int value)
{
    unsigned char text[11];
    unsigned char reversed[11];
    unsigned int length;
    unsigned int index;

    length = 0U;

    if (value == 0U) {
        text[0] = '0';
        text[1] = 0;
        lcd_putString(x, y, text);
        return;
    }

    while ((value > 0U) && (length < 10U)) {
        reversed[length] = (unsigned char)('0' + (value % 10U));
        value = value / 10U;
        length++;
    }

    for (index = 0U; index < length; index++) {
        text[index] = reversed[length - index - 1U];
    }

    text[length] = 0;
    lcd_putString(x, y, text);
}

static void display_on_off(unsigned short x,
                           unsigned short y,
                           unsigned int state)
{
    if (state != 0U) {
        lcd_putString(x, y, (unsigned char *)"ON ");
    } else {
        lcd_putString(x, y, (unsigned char *)"OFF");
    }
}

static void display_light_level(unsigned short x,
                                unsigned short y,
                                light_level_t level)
{
    if (level == LIGHT_LEVEL_DARK) {
        lcd_putString(x, y, (unsigned char *)"DARK  ");
    } else if (level == LIGHT_LEVEL_BRIGHT) {
        lcd_putString(x, y, (unsigned char *)"BRIGHT");
    } else {
        lcd_putString(x, y, (unsigned char *)"MEDIUM");
    }
}

static void display_blind_position(unsigned short x,
                                   unsigned short y,
                                   blind_position_t position)
{
    if (position == BLIND_UP) {
        lcd_fontColor(RED, WHITE);
        lcd_putString(x, y, (unsigned char *)"UP ");
    } else if (position == BLIND_DOWN) {
        lcd_fontColor(BLUE, WHITE);
        lcd_putString(x, y, (unsigned char *)"DOWN");
    } else {
        lcd_fontColor(DARK_GREEN, WHITE);
        lcd_putString(x, y, (unsigned char *)"MID ");
    }

    lcd_fontColor(BLACK, WHITE);
}

static void display_routine(unsigned short x,
                            unsigned short y,
                            routine_mode_t routine)
{
    if (routine == ROUTINE_MORNING) {
        lcd_putString(x, y, (unsigned char *)"MORNING");
    } else if (routine == ROUTINE_DAY) {
        lcd_putString(x, y, (unsigned char *)"DAY    ");
    } else if (routine == ROUTINE_EVENING) {
        lcd_putString(x, y, (unsigned char *)"EVENING");
    } else {
        lcd_putString(x, y, (unsigned char *)"NIGHT  ");
    }
}

static void display_hvac(unsigned short x,
                         unsigned short y,
                         heating_mode_t mode)
{
    if (mode == HVAC_HEATING) {
        lcd_fontColor(RED, WHITE);
        lcd_putString(x, y, (unsigned char *)"HEATING    ");
    } else if (mode == HVAC_COOLING) {
        lcd_fontColor(BLUE, WHITE);
        lcd_putString(x, y, (unsigned char *)"COOLING    ");
    } else {
        lcd_fontColor(DARK_GREEN, WHITE);
        lcd_putString(x, y, (unsigned char *)"COMFORTABLE");
    }

    lcd_fontColor(BLACK, WHITE);
}

static void display_button(unsigned short x0,
                           unsigned short y0,
                           unsigned short x1,
                           unsigned short y1,
                           unsigned char *label,
                           lcd_color_t colour)
{
    lcd_fillRect(x0, y0, x1, y1, colour);
    lcd_drawRect(x0, y0, x1, y1, BLACK);
    lcd_fontColor(BLACK, colour);
    lcd_putString((unsigned short)(x0 + 8U),
                  (unsigned short)(y0 + 12U),
                  label);
    lcd_fontColor(BLACK, WHITE);
}

static void display_draw_static(void)
{
    lcd_fillScreen(WHITE);
    lcd_fillRect(0, 0, 239, 24, NAVY);
    lcd_fontColor(WHITE, NAVY);
    lcd_putString(14, 9, (unsigned char *)"T11D COTTAGE HUB");
    lcd_fontColor(BLACK, WHITE);

    lcd_putString(10, 36, (unsigned char *)"DEMO TIME:");
    lcd_putString(10, 54, (unsigned char *)"ROUTINE:");
    lcd_putString(10, 78, (unsigned char *)"LIGHT RAW:");
    lcd_putString(10, 96, (unsigned char *)"ROOM TEMP:");
    lcd_putString(10, 114, (unsigned char *)"HVAC:");
    lcd_putString(10, 142, (unsigned char *)"BLIND 1:");
    lcd_putString(10, 160, (unsigned char *)"BLIND 2:");
    lcd_putString(10, 188, (unsigned char *)"COFFEE PLUG:");
    lcd_putString(10, 206, (unsigned char *)"HOUSE LIGHTS:");
    lcd_putString(10, 224, (unsigned char *)"DND:");
    lcd_putString(160, 224, (unsigned char *)"MISSED:");

    display_button(0, 250, 78, 282,
                   (unsigned char *)"PLUG", CYAN);
    display_button(80, 250, 158, 282,
                   (unsigned char *)"BLIND", YELLOW);
    display_button(160, 250, 239, 282,
                   (unsigned char *)"AUTO RESET", LIGHT_GRAY);
    display_button(0, 285, 118, 319,
                   (unsigned char *)"DND", MAGENTA);
    display_button(120, 285, 239, 319,
                   (unsigned char *)"LIGHT TEST", GREEN);
}

static void display_clear_dynamic_fields(void)
{
    lcd_fillRect(88, 30, 239, 68, WHITE);
    lcd_fillRect(88, 72, 239, 104, WHITE);
    lcd_fillRect(88, 108, 239, 126, WHITE);
    lcd_fillRect(88, 136, 239, 172, WHITE);
    lcd_fillRect(108, 182, 159, 214, WHITE);
    lcd_fillRect(108, 218, 159, 238, WHITE);
    lcd_fillRect(212, 218, 239, 238, WHITE);
}

void display_init(void)
{
    sdramInit();
    lcdInit(&lcd_config);
    lcdTurnOn();
    display_draw_static();
}

void display_render(const app_state_t *state)
{
    unsigned int hour;
    unsigned int minute;

    hour = (unsigned int)(state->demo_minutes / 60U);
    minute = (unsigned int)(state->demo_minutes % 60U);

    display_clear_dynamic_fields();

    display_number(90, 36, hour);
    lcd_putChar(108, 36, ':');
    if (minute < 10U) {
        lcd_putChar(114, 36, '0');
        display_number(120, 36, minute);
    } else {
        display_number(114, 36, minute);
    }

    display_routine(90, 54, state->routine);
    display_number(90, 78, state->light_raw);
    display_light_level(145, 78, state->light_level);
    display_number(90, 96, state->room_temperature_c);
    lcd_putString(108, 96, (unsigned char *)"C");
    display_hvac(90, 114, state->heating_mode);
    display_blind_position(90, 142, state->blind_1);
    display_blind_position(90, 160, state->blind_2);
    display_on_off(110, 188, state->smart_plug_on);
    display_on_off(110, 206, state->house_lights_on);
    display_on_off(110, 224, state->dnd_on);
    display_number(214, 224, state->missed_guests);
}

void display_recover_after_disco(const app_state_t *state)
{
    /*
     * Disco mode temporarily uses the LCD data pins for the LED ladder.
     * Restoring PINSEL and LCD_CTRL alone does not reset the LCD panel's
     * scan direction.  Run the Lab 6 LCD setup again, then redraw the
     * dashboard.  SDRAM is already initialized, so do not reset it here.
     */
    lcdInit(&lcd_config);
    lcdTurnOn();
    display_draw_static();
    display_render(state);
}
