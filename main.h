#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#include "stdbool.h"

// TODO: Create any necessary structs

/*
* For example, fosr a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

typedef struct {
    int row; // location of the cat
	int col; // location of the cat
	int width; // width of the cat
	int height; // height of the cat
} CAT;

typedef struct {
    int row;
	int col;
    int width;  // width of the log
	int length; // length of the log
	int speed;  // speed that the log is moving at
} LOG;


// Function Prototypes
void drawLogs(void);
void undrawLogs(const u16 *);
bool isCatOnLog(void);
bool isCatInWater(void);

#endif
