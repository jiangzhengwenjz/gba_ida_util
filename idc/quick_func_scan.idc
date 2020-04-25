/*
 * 2019 JZW
 *
 * Quick Approach of function detection in a range. 
 */

#include <idc.idc>
#include "include/defines.h"

#define FUNCTIONS_START 0x80001CC
#define FUNCTIONS_END 0x815BD84
//#define ROM 0x2000000

static main(void)
{
    auto ea, temp, lastFuncEa = NextFunction(ROM);

    while ((temp = NextFunction(lastFuncEa)) != BADADDR)
        lastFuncEa = temp;
    lastFuncEa = GetFunctionAttr(lastFuncEa, FUNCATTR_END);
    for (ea = FindCode(FUNCTIONS_START, SEARCH_DOWN);
         ea != BADADDR
             && ea < ((FUNCTIONS_END == -1) ? lastFuncEa : FUNCTIONS_END);
         ea = temp > ea ? temp : FindCode(ea, SEARCH_DOWN)) // thanks laqieer from github
    {
        if (ea & 2) // non_word_aligned? 
        {
            // assert(!(ea & 1));
            if (Word(ea) == 0x46C0 || Word(ea) == 0x0000) // nop || .align 2, 0
                ea = ea + 2;
        }
        if (GetFunctionName(ea) == "" && MakeFunction(ea, BADADDR))
            Message("Address: 0x%x FuncName: %s\n", ea, GetFunctionName(ea));
        temp = GetFunctionAttr(ea, FUNCATTR_END);
    }
}
