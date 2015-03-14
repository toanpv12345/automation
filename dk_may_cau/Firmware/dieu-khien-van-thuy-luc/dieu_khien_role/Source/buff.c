#include "buff.h"

#define MAX_BUFF_LEN   40

static uint8_t buff[(MAX_BUFF_LEN + 1)];
static uint8_t buff_idx = 0;
static uint8_t buff_size = 0;

void buff_init()
{
    buff_idx = buff_size = 0;
}

bool buff_push(uint8_t ch)
{
    if (buff_size < sizeof(buff))
    {
        buff[(buff_idx + buff_size++) % sizeof(buff)] = ch;
        return true;
    }
    return false;
}

bool buff_pop(uint8_t* ch)
{
    if (buff_size)
    {
        *ch = buff[buff_idx++];
        buff_idx %= sizeof(buff);
        buff_size--;
        return true;
    }
    return false;
}
