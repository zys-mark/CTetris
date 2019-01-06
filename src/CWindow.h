/*
 * CWindow.h
 *
 *  Created on: 2018年6月3日
 *      Author: zys
 */

#ifndef CWINDOW_H_
#define CWINDOW_H_

#define SDL_GRAPHICS
#ifdef SDL_GRAPHICS
#include "SDL2/SDL.h"
#include "common.h"
#include "SDL_FontCache.h"
#include <map>
#endif

class CWindowImp {
public:
  CWindowImp()
  {
  }
  virtual ~CWindowImp()
  {
  }
  virtual void drawRect(const CRect& rect) = 0;
  virtual void drawText(int x, int y, const std::string& str,
  		size_t fsize, int align = 1) = 0;
  virtual void clearBackground() = 0;
  virtual void setColor(uint r, uint g, uint b, uint a) = 0;
  virtual void renderPresent() = 0;
  virtual void fillRect(const CRect& rect) = 0;
};

class CSDLWindowImp: public CWindowImp {
private:
  SDL_Window* m_window;
  SDL_Renderer* m_render;
  std::map<size_t, FC_Font*> m_fonts; // 用于进行自体设置
public:
  CSDLWindowImp()
  {
    m_window = SDL_CreateWindow("Tetris With SDL", SDL_WINDOWPOS_UNDEFINED,
    		SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    m_render = SDL_CreateRenderer(m_window, -1, 0);
  }
  virtual ~CSDLWindowImp()
  {
    std::map<size_t, FC_Font*>::iterator iter;
    FC_Font *font = NULL;
    for (iter = m_fonts.begin(); iter != m_fonts.end(); iter++)
    {
      font = iter->second;
      FC_FreeFont(font);
      iter->second = NULL;
    }
    SDL_DestroyRenderer(m_render), m_render = NULL;
    SDL_DestroyWindow(m_window), m_window = NULL;
    SDL_Quit();
  }

  void drawRect(const CRect& rect)
  {
    SDL_Rect sdl_rect;
    sdl_rect.x = (int) rect.x + 1;
    sdl_rect.y = (int) rect.y + 1;
    sdl_rect.w = (int) rect.w - 1;
    sdl_rect.h = (int) rect.h - 1;
    SDL_RenderDrawRect(m_render, &sdl_rect);
  }

  void fillRect(const CRect& rect)
  {
    SDL_Rect sdl_rect;
    sdl_rect.x = (int) rect.x + 1;
    sdl_rect.y = (int) rect.y + 1;
    sdl_rect.w = (int) rect.w - 1;
    sdl_rect.h = (int) rect.h - 1;
    SDL_RenderFillRect(m_render, &sdl_rect);
  }

  void clearBackground()
  {
    SDL_SetRenderDrawColor(m_render, 173, 216, 230, 255);
    SDL_RenderClear(m_render);
  }

  void setColor(uint r, uint g, uint b, uint a)
  {
    SDL_SetRenderDrawColor(m_render, r, g, b, a);
  }

  void renderPresent()
  {
    SDL_RenderPresent(m_render);
  }

  void drawText(int x, int y, const std::string& str, size_t fsize,
      int align = 1)
  {
  	FC_AlignEnum sdl_align = FC_AlignEnum(align);
    size_t font_size = fsize < MIN_FONT_SIZE ?
            MIN_FONT_SIZE : (fsize > MAX_FONT_SIZE ? MAX_FONT_SIZE : fsize);
    FC_Font *font = NULL;
    switch (m_fonts.find(font_size) == m_fonts.end()) {
    case false:
      font = m_fonts[font_size];
      break;
    case true:
      font = FC_CreateFont();
      FC_LoadFont(font, m_render, "fonts/FreeSans.ttf", font_size,
          FC_MakeColor(0, 0, 0, 255), TTF_STYLE_NORMAL);
      m_fonts[font_size] = font;
      break;
    }
    FC_DrawAlign(font, m_render, x, y, sdl_align, str.c_str());
  }
};

class CWindowSystemFactory {
public:
  CWindowSystemFactory()
  {
  }
  virtual ~CWindowSystemFactory()
  {
  }

  virtual CWindowImp* createWindowImp() = 0;
};

class CSDLWindowSystemFactory: public CWindowSystemFactory {
public:
  virtual CWindowImp* createWindowImp()
  {
    return new CSDLWindowImp;
  }
};

class CWindow {
private:
  CWindowImp *_imp;
public:
  CWindow(CWindowSystemFactory *factory)
  {
    _imp = factory->createWindowImp();
  }
  ~CWindow()
  {
    delete _imp;
  }

  void drawRect(const CRect& rect)
  {
    _imp->drawRect(rect);
  }

  void fillRect(const CRect& rect)
  {
    _imp->fillRect(rect);
  }

  void drawText(int x, int y, const std::string& str, size_t fsize,
      int align = 1)
  {
    _imp->drawText(x, y, str, fsize, align);
  }

  void clearBackground()
  {
    _imp->clearBackground();
  }

  void setColor(uint r, uint g, uint b, uint a)
  {
    _imp->setColor(r, g, b, a);
  }

  void renderPresent()
  {
    _imp->renderPresent();
  }
};

#endif /* CWINDOW_H_BAK_ */
