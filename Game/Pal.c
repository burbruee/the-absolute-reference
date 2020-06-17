#include "Pal.h"

ROMDATA PalData Pals;

ROMDATA PalTableData PalsTable = {
	{
		'P', 'L', 'T', ' ', 'T', 'B', 'L', ' '
	},
	{
		// TODO: Change these to macros.
		PALPTR((0x63190 - 0x60090) / 0x40),
		PALPTR((0x631d0 - 0x60090) / 0x40),
		PALPTR((0x63210 - 0x60090) / 0x40),
		PALPTR((0x63250 - 0x60090) / 0x40),
		PALPTR((0x63290 - 0x60090) / 0x40),
		PALPTR((0x632d0 - 0x60090) / 0x40),
		PALPTR((0x63310 - 0x60090) / 0x40),
		PALPTR((0x63390 - 0x60090) / 0x40),
		PALPTR((0x63410 - 0x60090) / 0x40),
		PALPTR((0x63450 - 0x60090) / 0x40),
		PALPTR((0x63490 - 0x60090) / 0x40),
		PALPTR((0x634d0 - 0x60090) / 0x40),
		PALPTR((0x63510 - 0x60090) / 0x40),
		PALPTR((0x633d0 - 0x60090) / 0x40),
		PALPTR((0x63350 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x63850 - 0x60090) / 0x40),
		PALPTR((0x63890 - 0x60090) / 0x40),
		PALPTR((0x638d0 - 0x60090) / 0x40),
		PALPTR((0x63910 - 0x60090) / 0x40),
		PALPTR((0x63950 - 0x60090) / 0x40),
		PALPTR((0x63990 - 0x60090) / 0x40),
		PALPTR((0x639d0 - 0x60090) / 0x40),
		PALPTR((0x63a10 - 0x60090) / 0x40),
		PALPTR((0x63a50 - 0x60090) / 0x40),
		PALPTR((0x63a90 - 0x60090) / 0x40),
		PALPTR((0x63ad0 - 0x60090) / 0x40),
		PALPTR((0x63b10 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x63550 - 0x60090) / 0x40),
		PALPTR((0x63590 - 0x60090) / 0x40),
		PALPTR((0x635d0 - 0x60090) / 0x40),
		PALPTR((0x63610 - 0x60090) / 0x40),
		PALPTR((0x63650 - 0x60090) / 0x40),
		PALPTR((0x63690 - 0x60090) / 0x40),
		PALPTR((0x636d0 - 0x60090) / 0x40),
		PALPTR((0x63710 - 0x60090) / 0x40),
		PALPTR((0x63750 - 0x60090) / 0x40),
		PALPTR((0x63790 - 0x60090) / 0x40),
		PALPTR((0x637d0 - 0x60090) / 0x40),
		PALPTR((0x63810 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		PALPTR((0x60090 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x62e50 - 0x60090) / 0x40),
		PALPTR((0x62e90 - 0x60090) / 0x40),
		PALPTR((0x62ed0 - 0x60090) / 0x40),
		PALPTR((0x62f10 - 0x60090) / 0x40),
		PALPTR((0x62f50 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x63090 - 0x60090) / 0x40),
		PALPTR((0x630d0 - 0x60090) / 0x40),
		PALPTR((0x63110 - 0x60090) / 0x40),
		PALPTR((0x63150 - 0x60090) / 0x40),
		PALPTR((0x62e10 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x62f90 - 0x60090) / 0x40),
		PALPTR((0x62fd0 - 0x60090) / 0x40),
		PALPTR((0x63010 - 0x60090) / 0x40),
		PALPTR((0x63050 - 0x60090) / 0x40),
		PALPTR((0x62e10 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x62d10 - 0x60090) / 0x40),
		PALPTR((0x62d50 - 0x60090) / 0x40),
		PALPTR((0x62d90 - 0x60090) / 0x40),
		PALPTR((0x62dd0 - 0x60090) / 0x40),
		PALPTR((0x62e10 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x63f10 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x65cd0 - 0x60090) / 0x40),
		PALPTR((0x65c90 - 0x60090) / 0x40),
		PALPTR((0x65c50 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x65f50 - 0x60090) / 0x40),
		PALPTR((0x65f10 - 0x60090) / 0x40),
		PALPTR((0x65ed0 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x661d0 - 0x60090) / 0x40),
		PALPTR((0x66190 - 0x60090) / 0x40),
		PALPTR((0x66150 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x66450 - 0x60090) / 0x40),
		PALPTR((0x66410 - 0x60090) / 0x40),
		PALPTR((0x663d0 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x666d0 - 0x60090) / 0x40),
		PALPTR((0x66690 - 0x60090) / 0x40),
		PALPTR((0x66650 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x66950 - 0x60090) / 0x40),
		PALPTR((0x66910 - 0x60090) / 0x40),
		PALPTR((0x668d0 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x66bd0 - 0x60090) / 0x40),
		PALPTR((0x66b90 - 0x60090) / 0x40),
		PALPTR((0x66b50 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x65a50 - 0x60090) / 0x40),
		PALPTR((0x65a10 - 0x60090) / 0x40),
		PALPTR((0x659d0 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x66e50 - 0x60090) / 0x40),
		PALPTR((0x66e10 - 0x60090) / 0x40),
		PALPTR((0x66dd0 - 0x60090) / 0x40),
		NULL,

		PALPTR((0x670d0 - 0x60090) / 0x40),
		PALPTR((0x67090 - 0x60090) / 0x40),
		PALPTR((0x67050 - 0x60090) / 0x40),
		NULL
	}
};
