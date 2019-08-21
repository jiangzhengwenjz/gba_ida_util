/*
 * 2019 JZW
 *
 * Kill data wrongly marked as functions. (Slow)
 */

#include <idc.idc>
#include "include/defines.h"
#include "include/idc_util.h"

#define FUNC_DISTANCE(ea1, ea2) (MIN(ABS(GetFunctionAttr(ea1, FUNCATTR_START) - GetFunctionAttr(ea2, FUNCATTR_END)), ABS(GetFunctionAttr(ea2, FUNCATTR_START) - GetFunctionAttr(ea1, FUNCATTR_END))))
#define CLUSTER_NUM 32
#define ITER_NUM 4
#define THRESHOLD 10

static main(void)
{
    auto centrHandle, clusHandle, countHandle;
    auto lastFuncEa = NextFunction(ROM);
    auto temp, chunkSize, i, j, it, dist, min, funcCount = 1;
    auto whichClass;

    DeleteArray(GetArrayId("Centroids"));
    DeleteArray(GetArrayId("Classes"));
    DeleteArray(GetArrayId("Count"));
    centrHandle = CreateArray("Centroids");
    clusHandle = CreateArray("Classes");
    countHandle = CreateArray("Count");
    while ((temp = NextFunction(lastFuncEa)) != BADADDR)
    {
        lastFuncEa = temp;
        ++funcCount;
    }
    chunkSize = (lastFuncEa - ROM) / CLUSTER_NUM;
    for (it = 0; it < ITER_NUM; ++it)
    {
        for (i = 0; i < CLUSTER_NUM; ++i)
        {
            // Update new centroids
            if (!it)
            {
                SetArrayLong(centrHandle, i, NextFunction(i * chunkSize + ROM) - ROM);
            }
            else
            {
                auto sum = 0, hit = 0;

                j = 0;
                for (temp = ROM; temp != BADADDR; temp = NextFunction(temp))
                {
                    if (i == GetArrayElement(AR_LONG, clusHandle, j))
                    {
                        sum = sum + (temp - ROM);
                        ++hit;
                    }
                    ++j;
                }
                SetArrayLong(centrHandle, i, NextFunction((hit ? (sum / hit) : 0) + ROM) - ROM);
            }
        }
        // Logically we can check if we can get out of the loop here. 

        // Compute clusters
        j = 0;
        for (temp = ROM; temp != BADADDR; temp = NextFunction(temp))
        {
            min = FUNC_DISTANCE(temp, ROM + GetArrayElement(AR_LONG, centrHandle, 0));
            whichClass = 0;
            for (i = 1; i < CLUSTER_NUM; ++i)
            {
                dist = FUNC_DISTANCE(temp, ROM + GetArrayElement(AR_LONG, centrHandle, i));
                if (dist < min)
                {
                    min = dist;
                    whichClass = i;
                }
            }
            SetArrayLong(clusHandle, j, whichClass);
            ++j;
        }
    }
    for (i = 0; i < CLUSTER_NUM; ++i)
        SetArrayLong(countHandle, i, 0);
    for (i = 0; i < funcCount; ++i)
    {
        whichClass = GetArrayElement(AR_LONG, clusHandle, i);
        SetArrayLong(countHandle, whichClass, GetArrayElement(AR_LONG, countHandle, whichClass) + 1);
    }
    for (i = 0; i < CLUSTER_NUM; ++i)
    {
        temp = GetArrayElement(AR_LONG, countHandle, i);
        if (temp > 0 && temp <= THRESHOLD)
        {
            j = 0;
            for (temp = ROM; temp != BADADDR; temp = NextFunction(temp))
            {
                if (i == GetArrayElement(AR_LONG, clusHandle, j))
                {
                    Message("Not a function: 0x%x\n", temp);
                    MakeUnkn(temp, DOUNK_SIMPLE | DOUNK_DELNAMES);
                }
                ++j;
            }
        }
    }
    DeleteArray(clusHandle);
    DeleteArray(centrHandle);
    DeleteArray(countHandle);
}
