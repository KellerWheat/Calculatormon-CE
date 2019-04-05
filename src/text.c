#include <graphx.h>
#include <string.h>
#include <keypadc.h>
#include <stdint.h>
#include <stdarg.h> 
#include <stdio.h>
#include <stdbool.h>

#include "text.h"
#include "misc.h"

int8_t tv1 = 0;
int8_t tv2 = 0;

const uint8_t delaytime = 50;

/* Text Functions */

void text_Display(char text[], bool canSkip) {
	char newText1[50] = "";
	char newText2[50] = "";
	if (strlen(text) > 32) {
		strncpy(newText1, text, 32);
		strncpy(newText2, text + 32, (strlen(text) - 32));
	}
	else {
		strncpy(newText1, text, strlen(text));
	}

	gfx_SetDrawScreen();

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 165, 280, 55);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 165, 280, 55);
	gfx_SetColor(colors[0]);

	gfx_PrintStringXY(newText1, 30, 175);
	gfx_PrintStringXY(newText2, 30, 190);

	kb_Scan();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	if (canSkip && (kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) {
		Wait(delaytime);
	}
	kb_Scan();
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		Wait(5);
	}
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	gfx_SetDrawBuffer();
}
int text_AskQuestion2(char text1[], char text2[], bool canSkip) {
	tv1 = 1;
	gfx_SetDrawScreen();

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 165, 280, 55);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 165, 280, 55);
	gfx_SetColor(colors[0]);

	gfx_PrintStringXY(">", 25, 180);
	gfx_PrintStringXY(text1, 35, 180);
	gfx_PrintStringXY(text2, 35, 200);

	kb_Scan();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	if (canSkip && (kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) {
		Wait(delaytime);
	}
	kb_Scan();
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		if ((kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Left)) {
			if (tv1 == 1) {
				tv1 = 2;
			}
			else {
				tv1 = 1;
			}
			gfx_FillRectangle(24, 179, 10, 35);
			gfx_PrintStringXY(">", 25, 160 + 20 * tv1);
			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		tv1 = 0;
	}
	gfx_SetDrawBuffer();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	return tv1;
}
int text_AskQuestion4(char text1[], char text2[], char text3[], char text4[], bool canSkip) {
	tv2 = 0;
	tv1 = 0;
	gfx_SetDrawScreen();

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 165, 280, 55);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 165, 280, 55);
	gfx_SetColor(colors[0]);

	gfx_PrintStringXY(">", 25, 180);
	gfx_PrintStringXY(text1, 35, 180);
	gfx_PrintStringXY(text2, 35, 200);
	gfx_PrintStringXY(text3, 165, 180);
	gfx_PrintStringXY(text4, 165, 200);

	kb_Scan();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	if (canSkip && (kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) {
		Wait(delaytime);
	}
	kb_Scan();
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		if ((kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Up)) {
			if (tv1 == 1) {
				tv1 = 0;
			}
			else {
				tv1 = 1;
			}
			gfx_FillRectangle(24, 179, 10, 35);
			gfx_FillRectangle(154, 179, 10, 35);
			gfx_PrintStringXY(">", 25 + tv2 * 130, 180 + 20 * tv1);
			Wait(20);
		}
		if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			if (tv2 == 1) {
				tv2 = 0;
			}
			else {
				tv2 = 1;
			}
			gfx_FillRectangle(24, 179, 10, 35);
			gfx_FillRectangle(154, 179, 10, 35);
			gfx_PrintStringXY(">", 25 + tv2 * 130, 180 + 20 * tv1);
			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		if (!canSkip) {
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		}
		return(0);
	}
	gfx_SetDrawBuffer();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	return (tv1 + 1 + 2 * (tv2));
}
int text_AskQuestion6(char text1[], char text2[], char text3[], char text4[], char text5[], char text6[]) {
	tv2 = 0;
	tv1 = 0;
	gfx_SetDrawScreen();

	gfx_SetColor(colors[0]);
	gfx_FillRectangle(20, 140, 280, 80);
	gfx_SetColor(colors[1]);
	gfx_Rectangle(20, 140, 280, 80);
	gfx_SetColor(colors[0]);

	gfx_PrintStringXY(">", 25, 160);
	gfx_PrintStringXY(text1, 35, 160);
	gfx_PrintStringXY(text2, 35, 180);
	gfx_PrintStringXY(text3, 35, 200);
	gfx_PrintStringXY(text4, 125, 160);
	gfx_PrintStringXY(text5, 125, 180);
	gfx_PrintStringXY(text6, 125, 200);

	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	kb_Scan();
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
		if ((kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			if ((kb_Data[7] & kb_Up) && tv1 > 0) {
				tv1--;
			}
			if ((kb_Data[7] & kb_Down) && tv1 < 2) {
				tv1++;
			}
			if ((kb_Data[7] & kb_Left) && tv2 > 0) {
				tv2--;
			}
			if ((kb_Data[7] & kb_Right) && tv2 < 1) {
				tv2++;
			}
			gfx_FillRectangle(24, 159, 10, 55);
			gfx_FillRectangle(114, 159, 10, 55);
			gfx_PrintStringXY(">", 25 + tv2 * 90, 160 + 20 * tv1);
			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return(0);
	}
	gfx_SetDrawBuffer();
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	return (tv1 + 1 + 3 * tv2);
}

/* Text Varaibles*/

char text_status2[7][16] = { "attack","defence","special attack","special defence","speed","accuracy","evasiveness" };
char text_movecategories[18][10] = { "Physical","Special","Status","Status","Status","Special","Other","Other","Status","Special","Special","Special","Status","Status","Other","Physical","Other","Other" };