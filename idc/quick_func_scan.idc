/*
 * 2019 JZW
 *
 * Quick Approach of function detection in a range. 
 */

#include <idc.idc>
#include "include/defines.h"

#define FUNCTIONS_START 0x8000000
#define FUNCTIONS_END -1

static main(void)
{
    auto ea, temp, lastFuncEa = NextFunction(ROM);

    while ((temp = NextFunction(lastFuncEa)) != BADADDR)
        lastFuncEa = temp;
    lastFuncEa = GetFunctionAttr(lastFuncEa, FUNCATTR_END);
    for (ea = FindCode(FUNCTIONS_START, SEARCH_DOWN); ea != BADADDR && ea < ((FUNCTIONS_END == -1) ? lastFuncEa : FUNCTIONS_END); ea = FindCode(ea, SEARCH_DOWN))
    {
        if (GetFunctionName(ea) == "" && MakeFunction(ea, BADADDR))
            Message("Address: 0x%x FuncName: %s\n", ea, GetFunctionName(ea));
        if ((temp = GetFunctionAttr(ea, FUNCATTR_END)) > ea) // thanks laqieer from github
            ea = temp;
    }
}
