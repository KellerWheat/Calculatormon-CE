#ifndef TEXT_H
#define TEXT_H

#include <graphx.h>

void text_Display(char text[], bool canSkip);
int text_AskQuestion2(char text1[], char text2[], bool canSkip);
int text_AskQuestion4(char text1[], char text2[], char text3[], char text4[], bool canSkip);
int text_AskQuestion6(char text1[], char text2[], char text3[], char text4[], char text5[], char text6[]);

void text_DrawTextBox(void);

extern int textBoxType;
extern char text_status2[7][16];
extern char text_movecategories[18][10];
extern uint8_t lastTextIndex;

#endif