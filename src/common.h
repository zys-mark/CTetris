/*
 * common.h
 *
 *  Created on: 2018年6月3日
 *      Author: zys
 */

#ifndef COMMON_H_
#define COMMON_H_

#define SHOW_LIB 1

struct CRect {
  int x;
  int y;
  uint w;
  uint h;
};

struct CPoint {
  int x;
  int y;
};

typedef enum {
  SDL2_LIB, NCURSES_LIB
} GRAPHICS_LIB;

const int MIN_FONT_SIZE = 16;
const int MAX_FONT_SIZE = 54;

const int INTERVAL = 500;
const int DELAY_TIME = 20;
const uint TYPE_NUM = 7;
const uint BLOCK_SIZE = 4;
const uint SIDE_LEN = 40;
const uint WIDTH_SIZE = 10;
const uint HEIGHT_SIZE = 18;

const uint WINDOW_HEIGHT = HEIGHT_SIZE * SIDE_LEN;
const uint WINDOW_WIDTH = SIDE_LEN * 20;

const uint GAME_LEFT_BORDER = SIDE_LEN * 5;
;
const uint GAME_RIGHT_BORDER = 10 * SIDE_LEN + GAME_LEFT_BORDER;

const int DEFAULT_POSX = (WINDOW_WIDTH / (2 * SIDE_LEN) - 1) * SIDE_LEN;
const int DEFAULT_POSY = SIDE_LEN;

#endif /* COMMON_H_ */
