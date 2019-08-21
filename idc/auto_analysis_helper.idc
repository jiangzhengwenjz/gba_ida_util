/*
 * 2019 JZW
 *
 * IDA auto-analysis helper script. 
 */

#include <idc.idc>
#include "include/defines.h"
#include "include/idc_util.h"

static ParseCartHeader(void)
{
    MakeFunction(ROM, ROM + 4);
    MAKE_BYTE_ARRAY(ROM + 4, 156, "gNintendoLogo");
    MAKE_STR(ROM + 0xA0, 12, "gGameTitle");
    MAKE_STR(ROM + 0xAC, 4,"gGameCode");
    MAKE_STR(ROM + 0xB0, 2,"gMakerCode");
    MAKE_BYTE_ARRAY(ROM + 0xB2, 1, "gFixedValue");
    MAKE_BYTE_ARRAY(ROM + 0xB3, 1, "gMainUnitCode");
    MAKE_BYTE_ARRAY(ROM + 0xB4, 1, "gDeviceType");
    MAKE_BYTE_ARRAY(ROM + 0xB5, 7, "gReservedArea_xB5");
    MAKE_BYTE_ARRAY(ROM + 0xBC, 1, "gSoftwareVersion");
    MAKE_BYTE_ARRAY(ROM + 0xBD, 1, "gComplimentCheck");
    MAKE_BYTE_ARRAY(ROM + 0xBE, 2, "gReservedArea_xBE");
}

static CreateGbaRamSegmentation(void)
{
    ADD_SEGMENT(EWRAM, EWRAM_SIZE, "EWRAM");
    ADD_SEGMENT(IWRAM, IWRAM_SIZE, "IWRAM");
    ADD_SEGMENT(IORAM, IORAM_SIZE, "IORAM");
    ADD_SEGMENT(PLTT, PLTT_SIZE, "PLTT");
    ADD_SEGMENT(VRAM, VRAM_SIZE, "VRAM");
    ADD_SEGMENT(OAM, OAM_SIZE, "OAM");
    DelSeg(EWRAM + EWRAM_SIZE, SEGMOD_KILL);
    DelSeg(IWRAM + IWRAM_SIZE, SEGMOD_KILL);
    DelSeg(IORAM + IORAM_SIZE, SEGMOD_KILL);
    DelSeg(PLTT + PLTT_SIZE, SEGMOD_KILL);
    DelSeg(VRAM + VRAM_SIZE, SEGMOD_KILL);
    DelSeg(OAM + OAM_SIZE, SEGMOD_KILL);
    Message("RAM segmentation created.\n");
}

// The main loop function range seems always incorrectly analyzed
static FixInit(void)
{
    auto funcInit = ((((Dword(ROM) << 8) >> 8) & 0xFFFFFF) << 2) + ROM + 8;
    auto ea = RfirstB(funcInit);

    while ((ea = RnextB(funcInit, ea)) != BADADDR)
    {
        if (!GetReg(ea, "T") && (((Dword(ea) << 4) >> 28) & 0xF) == 0xA)
        {
            if (GetFunctionName(funcInit) == "")
                MakeFunction(funcInit, ea + 4);
            else
                SetFunctionEnd(funcInit, ea + 4);
            SetFunctionFlags(funcInit, GetFunctionFlags(funcInit) | FUNC_NORET);
            break;
        }
    }
}

static main(void)
{
    auto err;

    try
    {
        SetTargetAssembler(1); // GNU AS
        CreateGbaRamSegmentation();
        ParseCartHeader();
        FixInit();
    }
    catch (err)
    {
        Message("Error in %s\nat line %d within function %s\n", 
                err.file, err.line, err.func);
        Exit(1);
    }
}
