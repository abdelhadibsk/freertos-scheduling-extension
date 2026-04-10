#ifndef DBG_H
#define DBG_H
// the p
//#include "cmsis_os.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>


#define DBG_PRINT(fmt, ...)                                    \
    do {                                                       \
        char _buf[128];                                        \
        snprintf(_buf, sizeof(_buf), fmt, ##__VA_ARGS__);      \
        HAL_UART_Transmit(&huart2,                             \
                          (uint8_t *)_buf,                     \
                          (uint16_t)strlen(_buf),              \
                          HAL_MAX_DELAY);                      \
    } while (0)

#endif
