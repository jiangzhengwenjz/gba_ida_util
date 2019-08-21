#ifndef GUARD_DEFINES_H
#define GUARD_DEFINES_H

#define TRUE  1
#define FALSE 0

#define EWRAM       0x2000000
#define EWRAM_SIZE  0x40000
#define IWRAM       0x3000000
#define IWRAM_SIZE  0x8000
#define IORAM       0x4000000
#define IORAM_SIZE  0x400
#define PLTT        0x5000000
#define PLTT_SIZE   0x400
#define VRAM        0x6000000
#define VRAM_SIZE   0x18000
#define OAM         0x7000000
#define OAM_SIZE    0x400
#define ROM         0x8000000

#define BG_VRAM           VRAM
#define BG_VRAM_SIZE      0x10000
#define BG_CHAR_SIZE      0x4000
#define BG_SCREEN_SIZE    0x800
#define BG_CHAR_ADDR(n)   (BG_VRAM + (BG_CHAR_SIZE * (n)))
#define BG_SCREEN_ADDR(n) (BG_VRAM + (BG_SCREEN_SIZE * (n)))
#define BG_TILE_ADDR(n)   (BG_VRAM + (0x80 * (n)))

#define BG_TILE_H_FLIP(n) (0x400 + (n))
#define BG_TILE_V_FLIP(n) (0x800 + (n))

// text-mode BG
#define OBJ_VRAM0      (VRAM + 0x10000)
#define OBJ_VRAM0_SIZE 0x8000

// bitmap-mode BG
#define OBJ_VRAM1      (VRAM + 0x14000)
#define OBJ_VRAM1_SIZE 0x4000

#define ROM_HEADER_SIZE   0xC0

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 160

#define TILE_SIZE_4BPP 32
#define TILE_SIZE_8BPP 64

#define TOTAL_OBJ_TILE_COUNT 1024

#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))

#define RGB_BLACK RGB(0, 0, 0)
#define RGB_WHITE RGB(31, 31, 31)

#define WIN_RANGE(a, b) (((a) << 8) | (b))

#endif // GUARD_DEFINES_H
