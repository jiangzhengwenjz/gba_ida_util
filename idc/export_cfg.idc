/*
 * 2019 JZW
 *
 * Create .cfg file in the same directory as the database file
 * to feed gbadisasm by camthesaxman. 
 */

#include <idc.idc>
#include "include/defines.h"

static main(void)
{
    auto err;

    // IDC built-in function AskFile can also be used here
    try
    {
        auto cfgName, fHandle, funcName, ea;
        auto romName = GetInputFile();
        auto prevIdx = -1, index = -1;

        while ((index = strstr(romName[index + 1 : ], ".")) != -1)
            prevIdx = index;
        cfgName = (prevIdx == -1) ? (GetString(ROM + 0xAC) + ".cfg") : (romName[:prevIdx] + ".cfg");
        if (!isLoaded(ROM + 0xAC) || !(fHandle = fopen(cfgName, "w")))
            throw exception();
        for (ea = ROM; ea != BADADDR; ea = NextFunction(ea))
        {
            funcName = GetFunctionName(ea);
            writestr(fHandle, GetReg(ea, "T") ? "thumb_func" : "arm_func");
            fprintf(fHandle, " 0x%x %s\n", ea, funcName);
        }
        fclose(fHandle);
    }
    catch (err)
    {
        Message("Error in %s\nat line %d within function %s\n", 
                err.file, err.line, err.func);
        Exit(1);
    }
}
