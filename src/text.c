#include <graphx.h>
#include <string.h>
#include <keypadc.h>
#include <stdint.h>
#include <stdarg.h> 
#include <stdio.h>
#include <stdbool.h>

#include "text.h"
#include "misc.h"


/* If holding down second, the time to pause before moving to next text */
const uint8_t delaytime = 50;

gfx_sprite_t *textBoxSprite1;
gfx_sprite_t *textBoxSprite2;

/* Text Functions */


/* canSkip determines whether holding down 2nd before the text box pops up can do anything */
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
	gfx_Blit(gfx_screen);
	text_DrawTextBox();
	
	

	gfx_PrintStringXY(newText1, 30, 190);
	gfx_PrintStringXY(newText2, 30, 205);

	gfx_SwapDraw();

	Wait(10);

	kb_Scan();
	if (!canSkip) {
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	}
	if (canSkip && (kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) {
		Wait(delaytime);
	}
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
		kb_Scan();
	}
}
int text_AskQuestion2(char text1[], char text2[], bool canSkip) {
	int8_t tv1, tv2;
	tv1 = 1;
	tv2 = 1;


	gfx_Blit(gfx_screen);
	text_DrawTextBox();

	gfx_PrintStringXY(">", 25, 190);
	gfx_PrintStringXY(text1, 35, 190);
	gfx_PrintStringXY(text2, 35, 205);

	gfx_SwapDraw();
	gfx_SetDrawScreen();

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
			gfx_FillRectangle(24, 185, 10, 45);
			gfx_PrintStringXY(">", 25, 160 + 20 * tv1);
			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		tv1 = 0;
	}
	gfx_SetDrawBuffer();
	return tv1;
}
int text_AskQuestion4(char text1[], char text2[], char text3[], char text4[], bool canSkip) {
	int8_t tv1, tv2;
	tv1 = 0;
	tv2 = 0;

	gfx_Blit(gfx_screen);
	text_DrawTextBox();

	gfx_PrintStringXY(">", 25, 190);
	gfx_PrintStringXY(text1, 35, 190);
	gfx_PrintStringXY(text2, 35, 205);
	gfx_PrintStringXY(text3, 165, 190);
	gfx_PrintStringXY(text4, 165, 205);

	gfx_SwapDraw();
	gfx_SetDrawScreen();

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
			tv1 = !tv1;
			gfx_FillRectangle(24, 185, 10, 45);
			gfx_FillRectangle(154, 185, 10, 45);
			gfx_PrintStringXY(">", 25 + tv2 * 130, 190 + 15 * tv1);
			Wait(20);
		}
		if ((kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
			tv2 = !tv2;
			gfx_FillRectangle(24, 185, 10, 45);
			gfx_FillRectangle(154, 185, 10, 45);
			gfx_PrintStringXY(">", 25 + tv2 * 130, 190 + 15 * tv1);
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
	return (tv1 + 1 + 2 * (tv2));
}
int text_AskQuestion6(char text1[], char text2[], char text3[], char text4[], char text5[], char text6[]) {
	int8_t tv1, tv2;
	tv1 = 0;
	tv2 = 0;
	
	gfx_Blit(gfx_screen);
	text_DrawTextBox();

	gfx_PrintStringXY(">", 25, 160);
	gfx_PrintStringXY(text1, 35, 190);
	gfx_PrintStringXY(text2, 35, 205);
	gfx_PrintStringXY(text3, 35, 220);
	gfx_PrintStringXY(text4, 165, 190);
	gfx_PrintStringXY(text5, 165, 205);
	gfx_PrintStringXY(text6, 165, 220);

	gfx_SwapDraw();
	gfx_SetDrawScreen();

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
			gfx_FillRectangle(24, 185, 10, 45);
			gfx_FillRectangle(154, 185, 10, 45);
			gfx_PrintStringXY(">", 25 + tv2 * 130, 190 + 15 * tv1);
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
void text_DrawTextBox(void) {
	gfx_SetColor(colors[0]);	
	gfx_TransparentSprite(textBoxSprite1, 0, 176);
	gfx_TransparentSprite(textBoxSprite2, 160, 176);
}


/* Text Varaibles*/

char text_status2[7][16] = { "attack","defence","special attack","special defence","speed","accuracy","evasiveness" };
char text_movecategories[18][10] = { "Physical","Special","Status","Status","Status","Special","Other","Other","Status","Special","Special","Special","Status","Status","Other","Physical","Other","Other" };