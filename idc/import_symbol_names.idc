/*
 * 2019 JZW
 *
 * Import symbol names from a copy of .map file from gen III decomp
 * Syntax: [hex address] [spaces] [symbol name]
 */

#include <idc.idc>
#include "include/defines.h"

static main()
{
    auto err;

    try
    {
        auto file = AskFile(0, "*.txt", "Choose symbol file: ");
        auto fp, line = 0;

        fp = fopen(file, "r");
        while ((line = readstr(fp)) != -1)
        {
            auto index = 0;
            auto temp, ea, sym;

            while (line[index : (index + 1)] == " ")
                ++index;
            if (line[index : (index + 2)] == "0x" && (temp = strstr(line[(index + 2):], " ")) != -1)
            {
                auto i, val; 

                ea = 0;
                for (i = temp + index + 1; i >= (index + 2); --i)
                {
                    auto chara = line[i : (i + 1)];
                    auto j, b = 1;

                    if (chara == "a" || chara == "A")
                        val = 10;
                    else if (chara == "b" || chara == "B")
                        val = 11;
                    else if (chara == "c" || chara == "C")
                        val = 12;
                    else if (chara == "d" || chara == "D")
                        val = 13;
                    else if (chara == "e" || chara == "E")
                        val = 14;
                    else if (chara == "f" || chara == "F")
                        val = 15;
                    else
                        val = atol(chara);
                    for (j = 0; j < (temp + index + 1 - i); ++j)
                        b = b * 16;
                    ea = ea + b * val;
                }
                Message("0x%x ", line, ea);
                temp = temp + index + 2; // first intermediate space
                while (line[temp : (temp + 1)] == " ")
                    ++temp;
                index = temp;
                while (line[temp : (temp + 1)] != " " && line[temp : (temp + 1)] != "\n")
                    ++temp;
                if (strstr(line[index : temp], "gUnknown_") == -1
                 && strstr(line[index : temp], "sub_8") == -1)
                {
                    Message("%s\n", line[index : temp]);
                    MakeNameEx(ea, line[index : temp], SN_NOCHECK);
                }
                else
                {
                    Message("\n");
                }
            }
            else
            {
                throw exception();
            }
        }
    }
    catch (err)
    {
        Message("Error in %s\nat line %d within function %s\n", 
                err.file, err.line, err.func);
        Exit(1);
    }
}
