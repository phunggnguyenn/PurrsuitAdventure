#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"
#include "images/start.h"
#include "images/instructions.h"
#include "images/win.h"
#include "images/lose.h"
#include "images/cat.h"
#include "images/background.h"
/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  INSTRUCTIONS,
  PLAY,
  WIN,
  LOSE,
};

// struct variables
CAT pcat;
LOG wood1;
LOG wood2;
LOG wood3;

void initalize(CAT *pcat, LOG *wood1, LOG *wood2, LOG *wood3) {
  pcat -> row = 137;
  pcat -> col = 120;
  pcat -> width = 40;
  pcat -> height = 20;

  wood1 -> row = 110;
  wood1 -> col = 2;
  wood1 -> width = 60;
  wood1 -> length = 20;
  wood1 -> speed = 1;

  wood2 -> row = 85;
  wood2 -> col = 90;
  wood2 -> width = 60;
  wood2 -> length = 20;
  wood2 -> speed = -1;

  wood3 -> row = 60;
  wood3 -> col = 160;
  wood3 -> width = 60;
  wood3 -> length = 20;
  wood3 -> speed = 1;
}

static int jump = 4;
static int score = 0;
// static bool catWasOnLog = false;

// Beginning of game logic
int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  initalize(&pcat, &wood1, &wood2, &wood3);
  score = 0;

  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();

    if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
      state = START;
      drawFullScreenImageDMA(start);
      initalize(&pcat, &wood1, &wood2, &wood3);
      score = 0;
    }

    switch (state) {
      // start screen
      case START:
        drawFullScreenImageDMA(start);
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = INSTRUCTIONS;
          drawFullScreenImageDMA(instructions);
        }
        break;
      // instructions screen
      case INSTRUCTIONS:
        // write in increments of 10 to fit screen
        drawString(40, 20, "GOAL: Help Pusheen cross the water", GRAY);
        drawString(50, 40, "& reunite with Hello Kitty!", GRAY);

        drawString(80, 50, "Move with arrow keys.", GRAY);
        drawString(90, 10, "You lose if Pusheen hits the water.", GRAY);
        // drawString(100, 20, "-> +10 points for each successful", GRAY);
        // drawString(110, 90, "crossing.", GRAY);

        drawString(130, 60, "Press enter to play", WHITE);

        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY;
          drawFullScreenImageDMA(background);
        }
        break;
      // play screen
      case PLAY:

        // UNDRAWING
        undrawImageDMA(pcat.row,pcat.col, pcat.width, pcat.height, background);
        undrawLogs(background);

        // MOVING THE LOGS IN THE WATER
        wood1.col += wood1.speed;
	      wood2.col += wood2.speed;
	      wood3.col += wood3.speed;
        // check for boundaries
        // if out of bounds: move the logs in the reverse direction
        if (wood1.col < 0) {
          wood1.col = 0;
          wood1.speed = -wood1.speed;
        } else if (wood1.col + wood1.width > 240) {
          wood1.col = 240 - wood1.width;
          wood1.speed = -wood1.speed;
        }

        if (wood2.col < 0) {
          wood2.col = 0;
          wood2.speed = -wood2.speed;
        } else if (wood2.col + wood2.width > 240) {
          wood2.col = 240 - wood2.width;
          wood2.speed = -wood2.speed;
        }

        if (wood3.col < 0) {
          wood3.col = 0;
          wood3.speed = -wood3.speed;
        } else if (wood3.col + wood3.width > 240) {
          wood3.col = 240 - wood3.width;
          wood3.speed = -wood3.speed;
        }

        // If the cat is on the log, move the cat with the logs
        if (isCatOnLog()) {
          // updateScoreIfCatOnLog();
          if (pcat.col + pcat.width > wood1.col && pcat.col < wood1.col + wood1.width && pcat.row + pcat.height > wood1.row && pcat.row < wood1.row + wood1.length) {
            // score += 10;
            pcat.col += wood1.speed;
          } else if (pcat.col + pcat.width > wood2.col && pcat.col < wood2.col + wood2.width && pcat.row + pcat.height > wood2.row && pcat.row < wood2.row + wood2.length) {
            // score += 10;
            pcat.col += wood2.speed;
          } else if (pcat.col + pcat.width > wood3.col && pcat.col < wood3.col + wood3.width && pcat.row + pcat.height > wood3.row && pcat.row < wood3.row + wood3.length) {
            // score += 10;
            pcat.col += wood3.speed;
          }
        }

        // updateScoreIfCatOnLog();

        // KEYBOARD INPUT
        if (KEY_DOWN(BUTTON_UP, currentButtons)) { // Player pressed up arrow key
          if (pcat.row < 0) { // if out of screen boundaries
            pcat.row = 0;
          } else {
            pcat.row -= jump;
          }
			  }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {  // Player pressed down arrow key
          if (pcat.row + pcat.height + jump > 160) { // if out of screen boundaries
            pcat.row = 137;
          } else {
            pcat.row += jump;
          }
			  }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {  // Player pressed right arrow key
          if (pcat.col + pcat.width + jump > 240) { // if out of screen boundaries
            pcat.col = 240 - pcat.width;
          } else {
            pcat.col += 2;
          }
			  }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {  // Player pressed left arrow key
          if (pcat.col - 2 < 0) { // if out of screen boundaries
            pcat.col = 1;
          } else {
            pcat.col -= 2;
          }
			  }

        // Check if Pusheen is in water. Move to win screen if not
        if (isCatInWater()) {
          state = LOSE;
        } else if (pcat.row <= 30) {
          state = WIN;
        }

        // REDRAWING BELOW
        drawImageDMA(pcat.row, pcat.col, pcat.width, pcat.height, cat);
        drawLogs();
        // char scoreString[9];
        // sprintf(scoreString, "Score: %d", score);
        // drawString(10, 10, scoreString, WHITE);
        break;
      case WIN:
        drawFullScreenImageDMA(win);
        break;
      case LOSE:
        drawFullScreenImageDMA(lose);
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}

/**
 * Function to redraw the logs followed by redrawing pusheen
 */
void drawLogs(void) {
  drawRectDMA(wood1.row, wood1.col, wood1.width, wood1.length, BROWN); // wood1
  drawRectDMA(wood2.row, wood2.col, wood2.width, wood2.length, BROWN); // wood2
  drawRectDMA(wood3.row, wood3.col, wood3.width, wood3.length, BROWN); // wood3
  // rewdraw pusheen so it is on top of the logs when jumping
  drawImageDMA(pcat.row, pcat.col, pcat.width, pcat.height, cat);
}

/**
 * Function to undraw the logs followed by undrawing pusheen
 */
void undrawLogs(const u16 *image) {
  undrawImageDMA(wood1.row,wood1.col, wood1.width, wood1.length, image);
  undrawImageDMA(wood2.row,wood2.col, wood2.width, wood2.length, image);
  undrawImageDMA(wood3.row,wood3.col, wood3.width, wood3.length, image);
  // undraw pusheen
  undrawImageDMA(pcat.row, pcat.col, pcat.width, pcat.height, image);
}

/**
 * Boolean function to check if pusheen is on the log when it has moved
 */
bool isCatOnLog(void) {
  return (pcat.col + pcat.width > wood1.col && pcat.col < wood1.col + wood1.width && pcat.row + pcat.height > wood1.row && pcat.row < wood1.row + wood1.length)
    || (pcat.col + pcat.width > wood2.col && pcat.col < wood2.col + wood2.width && pcat.row + pcat.height > wood2.row && pcat.row < wood2.row + wood2.length)
    || (pcat.col + pcat.width > wood3.col && pcat.col < wood3.col + wood3.width && pcat.row + pcat.height > wood3.row && pcat.row < wood3.row + wood3.length);
}

/**
 * Boolean function to check if pusheen missed the logs or is in the water to move to lose state
 */
bool isCatInWater(void) {
  if (pcat.row > 50 && pcat.row <= 120) {
    if (!isCatOnLog()) {
      return true;
    }
  }
  return false;
}

/**
void updateScoreIfCatOnLog(void) {

  if (isCatOnLog()) {
    catWasOnLog = true;
  }

  if (isCatOnLog() || catWasOnLog) {
    score += 100;
  }

} */
