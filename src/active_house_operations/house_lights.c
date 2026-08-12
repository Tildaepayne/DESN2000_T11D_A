#include "house_lights.h"
#include "../lpc24xx.h"

/* I2C0 control bits used by the Lab 1 PCA9532 example. */
#define I2C_AA       0x00000004
#define I2C_SI       0x00000008
#define I2C_STO      0x00000010
#define I2C_STA      0x00000020
#define I2C_I2EN     0x00000040

#define I2C_WAIT_LIMIT          10000U
#define PCA9532_WRITE_ADDRESS   0x000000C0

/* LS2 controls the first four red LEDs on the QVGA Base Board. */
#define PCA9532_LS2             0x00000018
#define FOUR_LEDS_ON            0x00000055
#define FOUR_LEDS_OFF           0x00000000

static unsigned int house_lights_wait_for_i2c(void)
{
    unsigned int poll_count;

    poll_count = 0U;
    while (((I20CONSET & I2C_SI) == 0U) &&
           (poll_count < I2C_WAIT_LIMIT)) {
        poll_count++;
    }

    if (poll_count >= I2C_WAIT_LIMIT) {
        return 0U;
    }

    return 1U;
}

static void house_lights_stop_i2c(void)
{
    unsigned int poll_count;

    I20CONSET = I2C_STO;
    I20CONCLR = I2C_SI;

    poll_count = 0U;
    while (((I20CONSET & I2C_STO) != 0U) &&
           (poll_count < I2C_WAIT_LIMIT)) {
        poll_count++;
    }
}

static void house_lights_write(unsigned int led_value)
{
    I20CONCLR = I2C_AA | I2C_SI | I2C_STA | I2C_STO;
    I20CONSET = I2C_STA;
    if (house_lights_wait_for_i2c() == 0U) {
        house_lights_stop_i2c();
        return;
    }

    I20DAT = PCA9532_WRITE_ADDRESS;
    I20CONCLR = I2C_SI | I2C_STA;
    if (house_lights_wait_for_i2c() == 0U) {
        house_lights_stop_i2c();
        return;
    }

    I20DAT = PCA9532_LS2;
    I20CONCLR = I2C_SI;
    if (house_lights_wait_for_i2c() == 0U) {
        house_lights_stop_i2c();
        return;
    }

    I20DAT = led_value;
    I20CONCLR = I2C_SI;
    if (house_lights_wait_for_i2c() == 0U) {
        house_lights_stop_i2c();
        return;
    }

    house_lights_stop_i2c();
}

void house_lights_init(void)
{
    /* Power I2C0 and select SDA0/SCL0 on P0.27/P0.28. */
    PCONP = PCONP | 0x00000080;
    PINSEL1 = PINSEL1 & 0xFC3FFFFF;
    PINSEL1 = PINSEL1 | 0x01400000;

    I20CONCLR = I2C_AA | I2C_SI | I2C_STA | I2C_I2EN;
    I20SCLL = 0x00000080;
    I20SCLH = 0x00000080;
    I20CONSET = I2C_I2EN;

    house_lights_show(0U);
}

void house_lights_show(unsigned int lights_on)
{
    if (lights_on != 0U) {
        /* Four logical house lights: board indicator LEDs 1 to 4. */
        house_lights_write(FOUR_LEDS_ON);
    } else {
        house_lights_write(FOUR_LEDS_OFF);
    }
}
