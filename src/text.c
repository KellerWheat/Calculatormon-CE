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


int textBoxType = 0;

/* Text Functions */

uint16_t lastTextIndex = 0;
uint16_t lastTextID = 0;
/* canSkip determines whether holding down 2nd before the text box pops up can do anything */
void text_Display(char text[]) {
	char newText1[50] = "";
	char newText2[50] = "";
	int textIndex = 0;
	bool endText = false;

	gfx_SetDrawScreen();
	text_DrawTextBox();

	gfx_SetTextXY(30, 190);
	for (textIndex = 0; !endText; textIndex++) {
		if (text[textIndex] == '\0') {
			endText = true;
		}
		else if (text[textIndex] == '\n' && text[textIndex + 1] == '\n' && text[textIndex + 2] == '\n') {
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
				kb_Scan();
			}
			endText = true;
			lastTextIndex = textIndex + 3;
			gfx_SetDrawBuffer();
			return;
		}
		else if (text[textIndex] == '\n' && text[textIndex + 1] == '\n') {
			while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
			while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
				kb_Scan();
			}
			gfx_SetTextXY(30, 190);
			Wait(5);
			text_DrawTextBox();
			textIndex++;
		}
		else if (text[textIndex] == '\n') {
			gfx_SetTextXY(30, gfx_GetTextY() + 15);
			Wait(5);
		}
		else {
			gfx_PrintChar(text[textIndex]);
			Wait(1);
		}
	}
	
	Wait(10);
	kb_Scan();
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) { kb_Scan(); }
	
	gfx_SetDrawBuffer();
}
int text_AskQuestion2(char text1[], char text2[]) {
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
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }

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
			gfx_PrintStringXY(">", 25, 175 + 15 * tv1);
			Wait(20);
		}
	}
	if (kb_Data[6] & kb_Clear) {
		tv1 = 0;
	}
	gfx_SetDrawBuffer();
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	return tv1;
}
int text_AskQuestion4(char text1[], char text2[], char text3[], char text4[]) {
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
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
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
		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
		return(0);
	}
	gfx_SetDrawBuffer();
	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
	return (tv1 + 1 + 2 * (tv2));
}
//int text_AskQuestion6(char text1[], char text2[], char text3[], char text4[], char text5[], char text6[]) {
//	int8_t tv1, tv2;
//	tv1 = 0;
//	tv2 = 0;
//	
//	gfx_Blit(gfx_screen);
//	text_DrawTextBox();
//
//	gfx_PrintStringXY(">", 25, 160);
//	gfx_PrintStringXY(text1, 35, 190);
//	gfx_PrintStringXY(text2, 35, 205);
//	gfx_PrintStringXY(text3, 35, 220);
//	gfx_PrintStringXY(text4, 165, 190);
//	gfx_PrintStringXY(text5, 165, 205);
//	gfx_PrintStringXY(text6, 165, 220);
//
//	gfx_SwapDraw();
//	gfx_SetDrawScreen();
//
//	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
//	kb_Scan();
//	while (!((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear))) {
//		kb_Scan();
//		if ((kb_Data[7] & kb_Down) || (kb_Data[7] & kb_Up) || (kb_Data[7] & kb_Right) || (kb_Data[7] & kb_Left)) {
//			if ((kb_Data[7] & kb_Up) && tv1 > 0) {
//				tv1--;
//			}
//			if ((kb_Data[7] & kb_Down) && tv1 < 2) {
//				tv1++;
//			}
//			if ((kb_Data[7] & kb_Left) && tv2 > 0) {
//				tv2--;
//			}
//			if ((kb_Data[7] & kb_Right) && tv2 < 1) {
//				tv2++;
//			}
//			gfx_FillRectangle(24, 185, 10, 45);
//			gfx_FillRectangle(154, 185, 10, 45);
//			gfx_PrintStringXY(">", 25 + tv2 * 130, 190 + 15 * tv1);
//			Wait(20);
//		}
//	}
//	if (kb_Data[6] & kb_Clear) {
//		while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
//		return(0);
//	}
//	gfx_SetDrawBuffer();
//	while ((kb_Data[1] & kb_2nd) || (kb_Data[6] & kb_Clear)) { kb_Scan(); }
//	return (tv1 + 1 + 3 * tv2);
//}
void text_DrawTextBox(void) {
	if (textBoxType == 0) {
		int points[16] = { 4,188,  12,180,  307,180,  315,188,  315,227,  307,235,  12,235,  4,227,};
		gfx_SetColor(colors[13]);
		gfx_Polygon_NoClip(points, 8);
		gfx_SetColor(colors[0]);
		gfx_FillTriangle_NoClip(5, 188, 12, 181, 12, 188);
		gfx_FillTriangle_NoClip(314, 188, 307, 181, 307, 188);
		gfx_FillTriangle_NoClip(5, 227, 12, 234, 12, 227);
		gfx_FillTriangle_NoClip(314, 227, 307, 234, 307, 227);
		gfx_FillRectangle_NoClip(12, 181, 296, 54);
		gfx_FillRectangle_NoClip(5, 188, 8, 40);
		gfx_FillRectangle_NoClip(307, 188, 8, 40);

	}
	else if (textBoxType == 1) {
		gfx_SetColor(colors[14]);
		gfx_Rectangle_NoClip(0, 176, 320, 64);
		gfx_Rectangle_NoClip(1, 177, 318, 62);
		gfx_Rectangle_NoClip(2, 178, 316, 60);
		gfx_SetColor(colors[15]);
		gfx_Rectangle_NoClip(3, 179, 314, 58);
		gfx_Rectangle_NoClip(4, 180, 312, 56);
		gfx_Rectangle_NoClip(5, 181, 310, 54);
		gfx_SetColor(colors[16]);
		gfx_Rectangle_NoClip(6, 182, 308, 52);
		gfx_Rectangle_NoClip(7, 183, 306, 50);
		gfx_SetColor(colors[0]);
		gfx_FillRectangle_NoClip(8, 184, 304, 48);
	}
	else if (textBoxType == 2) {
		gfx_SetColor(colors[14]);
		gfx_Rectangle_NoClip(0, 176, 255, 64);
		gfx_Rectangle_NoClip(1, 177, 253, 62);
		gfx_Rectangle_NoClip(2, 178, 251, 60);
		gfx_SetColor(colors[15]);
		gfx_Rectangle_NoClip(3, 179, 249, 58);
		gfx_Rectangle_NoClip(4, 180, 247, 56);
		gfx_Rectangle_NoClip(5, 181, 245, 54);
		gfx_SetColor(colors[16]);
		gfx_Rectangle_NoClip(6, 182, 243, 52);
		gfx_Rectangle_NoClip(7, 183, 241, 50);
		gfx_SetColor(colors[0]);
		gfx_FillRectangle_NoClip(8, 184, 239, 48);

		gfx_SetColor(colors[14]);
		gfx_Rectangle_NoClip(255, 176, 66, 64);
		gfx_Rectangle_NoClip(256, 177, 64, 62);
		gfx_Rectangle_NoClip(257, 178, 62, 60);
		gfx_SetColor(colors[15]);
		gfx_Rectangle_NoClip(258, 179, 60, 58);
		gfx_Rectangle_NoClip(259, 180, 58, 56);
		gfx_Rectangle_NoClip(260, 181, 56, 54);
		gfx_SetColor(colors[16]);
		gfx_Rectangle_NoClip(261, 182, 54, 52);
		gfx_Rectangle_NoClip(262, 183, 52, 50);
		gfx_SetColor(colors[0]);
		gfx_FillRectangle_NoClip(263, 184, 50, 48);
	}
	else {
		gfx_SetColor(colors[14]);
		gfx_Rectangle_NoClip(0, 176, 320, 64);
		gfx_Rectangle_NoClip(1, 177, 318, 62);
		gfx_Rectangle_NoClip(2, 178, 316, 60);
		gfx_SetColor(colors[15]);
		gfx_Rectangle_NoClip(3, 179, 314, 58);
		gfx_Rectangle_NoClip(4, 180, 312, 56);
		gfx_Rectangle_NoClip(5, 181, 310, 54);
		gfx_SetColor(colors[16]);
		gfx_Rectangle_NoClip(6, 182, 308, 52);
		gfx_Rectangle_NoClip(7, 183, 306, 50);
		gfx_SetColor(colors[0]);
		gfx_FillRectangle_NoClip(8, 184, 304, 48);
	}
	gfx_SetColor(colors[0]);
}


/* Text Varaibles*/

char text_status2[7][16] = { "attack","defence","special attack","special defence","speed","accuracy","evasiveness" };
char text_movecategories[18][10] = { "Physical","Special","Status","Status","Status","Special","Other","Other","Status","Special","Special","Special","Status","Status","Other","Physical","Other","Other" };