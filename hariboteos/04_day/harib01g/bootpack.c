void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

void HariMain(void)
{
	char *p; /* p偲偄偆曄悢偼丄BYTE [...]梡偺斣抧 */

	init_palette(); /* 僷儗僢僩傪愝掕 */

	p = (char *) 0xa0000; /* 斣抧傪戙擖 */

	boxfill8(p, 320, COL8_FF0000,  20,  20, 120, 120);
	boxfill8(p, 320, COL8_00FF00,  70,  50, 170, 150);
	boxfill8(p, 320, COL8_0000FF, 120,  80, 220, 180);

	for (;;) {
		io_hlt();
	}
}

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:崟 */
		0xff, 0x00, 0x00,	/*  1:柧傞偄愒 */
		0x00, 0xff, 0x00,	/*  2:柧傞偄椢 */
		0xff, 0xff, 0x00,	/*  3:柧傞偄墿怓 */
		0x00, 0x00, 0xff,	/*  4:柧傞偄惵 */
		0xff, 0x00, 0xff,	/*  5:柧傞偄巼 */
		0x00, 0xff, 0xff,	/*  6:柧傞偄悈怓 */
		0xff, 0xff, 0xff,	/*  7:敀 */
		0xc6, 0xc6, 0xc6,	/*  8:柧傞偄奃怓 */
		0x84, 0x00, 0x00,	/*  9:埫偄愒 */
		0x00, 0x84, 0x00,	/* 10:埫偄椢 */
		0x84, 0x84, 0x00,	/* 11:埫偄墿怓 */
		0x00, 0x00, 0x84,	/* 12:埫偄惵 */
		0x84, 0x00, 0x84,	/* 13:埫偄巼 */
		0x00, 0x84, 0x84,	/* 14:埫偄悈怓 */
		0x84, 0x84, 0x84	/* 15:埫偄奃怓 */
	};
	set_palette(0, 15, table_rgb);
	return;

	/* static char 柦椷偼丄僨乕僞偵偟偐巊偊側偄偗偳DB柦椷憡摉 */
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags();	/* 妱傝崬傒嫋壜僼儔僌偺抣傪婰榐偡傞 */
	io_cli(); 					/* 嫋壜僼儔僌傪0偵偟偰妱傝崬傒嬛巭偵偡傞 */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags);	/* 妱傝崬傒嫋壜僼儔僌傪尦偵栠偡 */
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}
