/*
 * 2020 JZW
 *
 * Report misaligned functions -> likely to be padding from unknown compiler
 */

#include <idc.idc>
#include "include/defines.h"

static main(void)
{
    auto ea = ROM;

    while ((ea = NextFunction(ea)) != BADADDR)
        if (ea & 3)
            Message("0x%x %s\n", ea, GetFunctionName(ea));
}
