#ifndef GUARD_IDC_UTIL_H
#define GUARD_IDC_UTIL_H

#define ADD_SEGMENT(addr, size, name)                                  \
do                                                                     \
{                                                                      \
    AddSegEx(addr, addr + size, 0, 1, saAbs, scPub, ADDSEG_SPARSE);    \
    RenameSeg(addr, name);                                             \
} while (FALSE)

#define MAKE_ARRAY(addr, nelems, elemType, name)    \
do                                                  \
{                                                   \
    MakeData(addr,                                  \
             elemType,                              \
             elemType == FF_BYTE ? 1 :              \
             (elemType == FF_WORD ? 2 :             \
             (elemType == FF_DWRD ? 4 :             \
             -1)), 0);                              \
    if (nelems > 1)                                 \
        MakeArray(addr, nelems);                    \
    MakeNameEx(addr, name, SN_PUBLIC | SN_CHECK);   \
} while (FALSE)

#define MAKE_BYTE_ARRAY(addr, nelems, name) MAKE_ARRAY(addr, nelems, FF_BYTE, name)
#define MAKE_SHORT_ARRAY(addr, nelems, name) MAKE_ARRAY(addr, nelems, FF_WORD, name)
#define MAKE_LONG_ARRAY(addr, nelems, name) MAKE_ARRAY(addr, nelems, FF_DWRD, name)

#define MAKE_STR(ea, len, name)                     \
do                                                  \
{                                                   \
    MakeStr(ea, ea + len);                          \
    MakeNameEx(ea, name, SN_PUBLIC | SN_CHECK);     \
} while (FALSE)

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define ABS(a) (((a) >= 0) ? (a) : (-(a)))

#endif // GUARD_IDC_UTIL_H
