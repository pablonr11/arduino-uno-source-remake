#ifndef _I2C_H
#define _I2C_H

// I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR * PrescalerValue)
// This value is used to calculate TWBR
// We need to obtain a formula to calculate TWBR.
// The whole process to separate TWBR in the previous formula is the following:
// 1. I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR * PrescalerValue)
// 2. We use PrescalerValue 1 so we can remove it
//      I2C_FREQUENCY = F_CPU / (16 + 2 * TWBR)
// 3. I2C_FREQUENCY * (16 + 2 * TWBR) = F_CPU
// 4. 16 + 2 * TWBR = F_CPU / I2C_FREQUENCY
// 5. 2 * TWBR = (F_CPU / I2C_FREQUENCY) - 16
// 6. TWBR = ((F_CPU / I2C_FREQUENCY) - 16) / 2
#define I2C_FREQUENCY 100000

void initI2C(void);

#endif