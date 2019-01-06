//============================================================================
// Name        : CTetris.cpp
// Author      : mark
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style, Tetris game for good code
//============================================================================

#include <iostream>

#include "common.h"
#include "CCombinedBlock.h"
#include <string>
#include "stdlib.h"

#include "CWindow.h"
using namespace std;
std::vector<std::bitset<WIDTH_SIZE>> gShapeBitmap;
std::vector<std::bitset<WIDTH_SIZE>> gSwapShapeBitmap;
std::map<std::string, GRAPHICS_LIB>  gGraphicsLibs =
{
		{ "USE_LIB", SDL2_LIB },
		{ "SDL2_LIB", SDL2_LIB },
		{ "NCURSES_LIB", NCURSES_LIB }
};
CWindow *gWindow = NULL;
CWindowSystemFactory *gWindowSystemFactory = NULL;
bool gGameStop = false;
bool gGameOver = false;
int gLines = 0, gScores = 0;

void UpdateGameData(const CPoint &position, CCombinedBlock &shape)
{
  if (shape.isOver() && (!gGameOver)) {
    int count = 0;
    int swap_line_idx = HEIGHT_SIZE - 1;
    int line_idx = HEIGHT_SIZE - 1;

    bool finish = false;
    while (line_idx >= 0 && !finish) {
      size_t bits = gShapeBitmap[line_idx].count();
      switch (bits) {
      case WIDTH_SIZE:
        count++;
        gShapeBitmap[line_idx--] = 0x000;
        break;
      case 0:
        gGameOver = (swap_line_idx <= 1);
        gShapeBitmap.swap(gSwapShapeBitmap);
        finish = true;
        break;
      default:
        gSwapShapeBitmap[swap_line_idx--] = gShapeBitmap[line_idx];
        gShapeBitmap[line_idx--] = 0x000;
        break;
      }
    }

    if (count > 0) {
      gLines += count;
      gScores += count * 100 + (count - 1) * 50;
    }
    if (!gGameOver) {
      SHAPE_TYPE type = shape.getNextShapeType();
      shape.init(position, type);
    }
  }
}

void RenderGameData(const CCombinedBlock &block)
{
  CRect rect;
  gWindow->clearBackground();  // clear background color

  // render border of game area
  rect.x = GAME_LEFT_BORDER - 1, rect.y = 0;
  rect.w = GAME_RIGHT_BORDER - GAME_LEFT_BORDER + 2, rect.h = WINDOW_HEIGHT;
  gWindow->setColor(245, 255, 250, 255);
  gWindow->drawRect(rect);

  // render fixed area
  gWindow->setColor(0, 139, 139, 255);
  for (uint i = 0; i < gShapeBitmap.size(); i++) {
    if (!gShapeBitmap[i].count())
      continue;
    for (uint j = 0; j < WIDTH_SIZE; j++) {
      if (gShapeBitmap[i].test(j)) {
        rect.x = GAME_LEFT_BORDER + j * SIDE_LEN;
        rect.y = i * SIDE_LEN;
        rect.w = rect.h = SIDE_LEN;
        gWindow->fillRect(rect);
      }
    }
  }

  // render left area
  rect = { 0, 0, GAME_LEFT_BORDER - 2, SIDE_LEN * 4 };
  gWindow->setColor(175, 238, 238, 80);
  gWindow->fillRect(rect);   // draw next shape
  rect.h = SIDE_LEN * 2, rect.w = SIDE_LEN * 2, rect.y += SIDE_LEN * 5;
  gWindow->setColor(175, 238, 238, 80);
  rect.y += (SIDE_LEN * 2 + 1);
  gWindow->drawText(SIDE_LEN, rect.y - 10, "→", 60);
  gWindow->drawText(SIDE_LEN * 2 + 5, rect.y + 25, "MOVE RIGHT", 16,
      FC_ALIGN_LEFT);
  gWindow->drawRect(rect);	 // draw right shape
  rect.y += (SIDE_LEN * 2 + 1);
  gWindow->drawText(SIDE_LEN, rect.y - 10, "←", 60);
  gWindow->drawText(SIDE_LEN * 2 + 5, rect.y + 25, "MOVE LEFT", 16,
      FC_ALIGN_LEFT);
  gWindow->drawRect(rect);	 // draw left shape
  rect.y += (SIDE_LEN * 2 + 1);
  gWindow->drawText(SIDE_LEN, rect.y + 4, "↑", 60);
  gWindow->drawText(SIDE_LEN * 2 + 5, rect.y + 25, "ROTATE", 16, FC_ALIGN_LEFT);
  gWindow->drawRect(rect);	 // draw up shape
  rect.y += (SIDE_LEN * 2 + 1);
  gWindow->drawText(SIDE_LEN, rect.y + 4, "↓", 60);
  gWindow->drawText(SIDE_LEN * 2 + 5, rect.y + 25, "ACCELERATE", 16,
      FC_ALIGN_LEFT);
  gWindow->drawRect(rect);	 // draw down shape

  // render dropping block
  gWindow->setColor(0, 139, 139, 255);
  DrawBlockShape(gWindow, block, true);

  // render text
  int middle_pos = GAME_RIGHT_BORDER + (WINDOW_WIDTH - GAME_RIGHT_BORDER) / 2;
  // draw score
  rect = { GAME_RIGHT_BORDER + 1, 0, WINDOW_WIDTH - GAME_RIGHT_BORDER - 2, 100 };
  gWindow->drawRect(rect);
  string str_score = to_string(gScores);
  gWindow->drawText(middle_pos, 30, str_score, 28);
  gWindow->drawText(middle_pos, 110, "SCORE", 20);

  // draw time
  rect.y = 180;
  gWindow->drawRect(rect);
  gWindow->drawText(middle_pos, 290, "TIME", 20);

  gWindow->renderPresent();
}

// game initialize
void GameInit(int argc, char **argv)
{
#if SHOW_LIB == 1
  gWindowSystemFactory = new CSDLWindowSystemFactory();
#elif SHOW_LIB == 2
  gWindowSystemFactory = new CNCURSESWindowSystemFactory();
#endif

  gWindow = gWindowSystemFactory ? new CWindow(gWindowSystemFactory) : NULL;
  gGameStop = (gWindow == NULL || gWindowSystemFactory == NULL);

  // initialize bitmap
  for (uint i = 0; i < HEIGHT_SIZE; i++) {
    bitset<WIDTH_SIZE> line_bit(0x000);
    gShapeBitmap.push_back(line_bit);
    gSwapShapeBitmap.push_back(line_bit);
  }

  srand((int) time(NULL));
  if (argc == 1) {
    fprintf(stdout, "INFO: use default graphics lib: SDL2");
    return;
  }
  if (argc != 2) {
    gGameStop = true;
    fprintf(stderr, "ERROR: usage %s graphics lib.", argv[0]);
    return;
  }
  string use_lib = argv[1];
  if (gGraphicsLibs.find(use_lib) != gGraphicsLibs.end()) {
    gGraphicsLibs["USE_LIB"] = gGraphicsLibs[use_lib];
    fprintf(stdout, "INFO: use graphics lib: %s.", use_lib.c_str());
  } else {
    fprintf(stderr, "ERROR: graphics does not exists, use default SDL2 lib");
  }
}

// SDL2 game message loop
Uint32 SDL2CallbackFunc(Uint32 interval, void *param)
{
  SDL_Event event;
  SDL_UserEvent userevent;
  /* In this example, our callback pushes a function
   into the queue, and causes our callback to be called again at the
   same interval: */
  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = param;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return interval;
}
void SDL2MessageLoop()
{
  const CPoint position = { DEFAULT_POSX, DEFAULT_POSY };
  CCombinedBlock block(position);

  SDL_AddTimer(INTERVAL, SDL2CallbackFunc, NULL);
  while (!gGameStop) {
    SDL_Event event;
    while (!gGameStop && SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: // TODO keyboard input handling goes here
        gGameStop = true;
        break;
      case SDL_USEREVENT:   // for down event
        if ((!gGameStop) && (!gGameOver))
          block.moveDown(gShapeBitmap);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_UP:
          block.rotate(gShapeBitmap);
          break;
        case SDLK_DOWN:
          block.moveDown(gShapeBitmap);
          break;
        case SDLK_LEFT:
          block.moveLeft(gShapeBitmap);
          break;
        case SDLK_RIGHT:
          block.moveRight(gShapeBitmap);
          break;
        default:
          break;
        }
        break;
      case SDL_MOUSEMOTION:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        break;
      default:
        break;
      }
    }
    UpdateGameData(position, block);
    RenderGameData(block);
    SDL_Delay(DELAY_TIME);
  }
}
void NcursesMessageLoop()
{

}
void GameLoop()
{
  switch (gGraphicsLibs["USE_LIB"]) {
  case SDL2_LIB:
    SDL2MessageLoop();
    break;
  case NCURSES_LIB:
    NcursesMessageLoop();
    break;
  default:
    break;
  }
}

// release resource
void GameEnd()
{
  delete gWindow;
  delete gWindowSystemFactory;
}

// main fun of Tetris
int main(int argc, char *argv[])
{
  GameInit(argc, argv);
  GameLoop();
  GameEnd();
  return 0;
}
