/*
 * CCombinedBlock.cpp
 *
 *  Created on: 2018年6月3日
 *      Author: zys
 */
#include "CCombinedBlock.h"
#include <map>
#include <bitset>

// square number per block shape
std::map<SHAPE_TYPE, uint> mapTypeSize =
    {
    		{ SQUARE, 4 },
				{ LBLOCK, 4 },
				{ RLBLOCK, 4 },
				{ SWAGER, 4 },
        { RSWAGER, 4 },
				{ TBLOCK, 4 },
				{ LINEPIECE, 4 }
    };

int DrawBlockShape(CWindow* window, const CCombinedBlock& shape,
    bool draw_next_shape)
{
  for (uint i = 0; i < shape.m_vecBlocks.size(); i++)
    window->fillRect(shape.m_vecBlocks[i]);

  if (draw_next_shape)
  {
    for (uint i = 0; i < shape.m_vecNextBlocks.size(); i++)
      window->fillRect(shape.m_vecNextBlocks[i]);
  }
  return 0;
}

void CCombinedBlock::initBlocksPos(std::vector<CRect> &blocks,
    const CPoint &position, SHAPE_TYPE type)
{
  CPoint pos_array[mapTypeSize[type] - 1];
  for (uint i = 0; i < mapTypeSize[type]; i++)
  {
    blocks[i].x = position.x, blocks[i].y = position.y;
    blocks[i].w = blocks[i].h = SIDE_LEN;
  }

  switch (type) {
  case SQUARE:
    pos_array[0] = { 1, 0 };
    pos_array[1] = { 0, 1 };
    pos_array[2] = { 1, 1 };
    break;
    case LINEPIECE:
    pos_array[0] = { 1, 0 };
    pos_array[1] = { 2, 0 };
    pos_array[2] = { 3, 0 };
    break;
    case LBLOCK:
    pos_array[0] = { 0, 1 };
    pos_array[1] = { 1, 1 };
    pos_array[2] = { 2, 1 };
    break;
    case RLBLOCK:
    pos_array[0] = { 0, 1 };
    pos_array[1] = { -1, 1 };
    pos_array[2] = { -2, 1 };
    break;
    case SWAGER:
    pos_array[0] = { 1, 0 };
    pos_array[1] = { 1, 1 };
    pos_array[2] = { 2, 1 };
    break;
    case RSWAGER:
    pos_array[0] = { -1, 0 };
    pos_array[1] = { -1, 1 };
    pos_array[2] = { -2, 1 };
    break;
    case TBLOCK:
    pos_array[0] = { 0, 1 };
    pos_array[1] = { -1, 1 };
    pos_array[2] = { 1, 1 };
    break;
    default:
    break;
  }

  for (uint i = 0; i < mapTypeSize[type] - 1; i++)
  {
    blocks[i + 1].x += pos_array[i].x * SIDE_LEN;
    blocks[i + 1].y += pos_array[i].y * SIDE_LEN;
  }
}

void CCombinedBlock::init(const CPoint &position, const SHAPE_TYPE type)
{
  SHAPE_TYPE shape_type;
  shape_type = (type == NONE) ? (SHAPE_TYPE) (rand() % TYPE_NUM) : type;
  m_eType = shape_type;
  m_eNextType = (SHAPE_TYPE) (rand() % TYPE_NUM);
  m_eState = HORIZONTAL;
  m_bOver = false;

  initBlocksPos(m_vecBlocks, position, m_eType);

  CRect rect = { 0, 0, GAME_LEFT_BORDER - 4, SIDE_LEN * 4 };
  CPoint point = { int((rect.w * 1.0) / 2), int((rect.h * 1.0) / 2) };
  switch (m_eNextType) {
  case LBLOCK:
  case SWAGER:
    point.x -= (SIDE_LEN * 3.0 / 2);
    point.y -= SIDE_LEN;
    break;
  case TBLOCK:
    point.x -= (SIDE_LEN * 1.0 / 2);
    point.y -= SIDE_LEN;
    break;
  case RLBLOCK:
  case RSWAGER:
    point.x += (SIDE_LEN * 1.0 / 2);
    point.y -= SIDE_LEN;
    break;
  case SQUARE:
    point.x -= SIDE_LEN;
    point.y -= SIDE_LEN;
    break;
  case LINEPIECE:
    point.x -= SIDE_LEN * 2;
    point.y -= (SIDE_LEN * 1.0 / 2);
    break;
  default:
    break;
  }
  initBlocksPos(m_vecNextBlocks, point, m_eNextType);
}

CCombinedBlock::CCombinedBlock(const CPoint& position, SHAPE_TYPE type)
{
  SHAPE_TYPE shape_type = (type == NONE) ? (SHAPE_TYPE) (rand() % TYPE_NUM) : type;
  m_eType = shape_type;
  CRect rect;
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    rect = { position.x, position.y, SIDE_LEN, SIDE_LEN };
    m_vecBlocks.push_back(rect);
    m_vecNextBlocks.push_back(rect);
  }

  init(position, type);
}

// left shift
int CCombinedBlock::moveLeft(std::vector<std::bitset<WIDTH_SIZE>> &bitmap)
{
  int line_idx = 0, column_idx = 0;
  bool can_move_left = true;
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    line_idx = m_vecBlocks[i].y / SIDE_LEN;
    column_idx = (m_vecBlocks[i].x - GAME_LEFT_BORDER) / SIDE_LEN - 1;
    if (m_vecBlocks[i].x == GAME_LEFT_BORDER
        || bitmap[line_idx].test(column_idx))
    {
      can_move_left = false;
      break;
    }
  }
  if (can_move_left)   // do not reach to left border
  {
    for (uint i = 0; i < mapTypeSize[m_eType]; i++)
      m_vecBlocks[i].x -= SIDE_LEN;
  }
  return 0;
}

// right shift
int CCombinedBlock::moveRight(std::vector<std::bitset<WIDTH_SIZE>> &bitmap)
{
  int line_idx = 0, column_idx = 0;
  bool can_move_rigth = true;
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    line_idx = m_vecBlocks[i].y / SIDE_LEN;
    column_idx = (m_vecBlocks[i].x - GAME_LEFT_BORDER) / SIDE_LEN + 1;
    if (m_vecBlocks[i].x + SIDE_LEN == GAME_RIGHT_BORDER
        || bitmap[line_idx].test(column_idx))
    {
      can_move_rigth = false;
      break;
    }
  }
  if (can_move_rigth)        // do not reach right border
  {
    for (uint i = 0; i < mapTypeSize[m_eType]; i++)
      m_vecBlocks[i].x += SIDE_LEN;
  }
  return 0;
}

// move down
int CCombinedBlock::moveDown(std::vector<std::bitset<WIDTH_SIZE>> &bitmap)
{
  int line_idx = 0, column_idx = 0;
  bool can_move_down = true;
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    line_idx = (m_vecBlocks[i].y / SIDE_LEN) + 1;
    column_idx = (m_vecBlocks[i].x - GAME_LEFT_BORDER) / SIDE_LEN;
    if (m_vecBlocks[i].y + SIDE_LEN >= WINDOW_HEIGHT
        || bitmap[line_idx].test(column_idx))
    {
      can_move_down = false;
      m_bOver = true;
      break;
    }
  }
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    switch (can_move_down) {
    case false:
      line_idx = (m_vecBlocks[i].y / SIDE_LEN);
      column_idx = (m_vecBlocks[i].x - GAME_LEFT_BORDER) / SIDE_LEN;
      bitmap[line_idx].set(column_idx);
      break;
    case true:
      m_vecBlocks[i].y += SIDE_LEN;
      break;
    }
  }
  return 0;
}

// rotate
int CCombinedBlock::rotate(std::vector<std::bitset<WIDTH_SIZE>> &bitmap)
{
  DIRECTION rot_direction, sv_direction;
  bool can_rotate = true;
  STATE save_state = m_eState;
  CPoint point;

  bool clock_wise = (m_eState == HORIZONTAL && m_eType != LINEPIECE);
  clock_wise = clock_wise || (m_eState == VERTICAL && m_eType == LINEPIECE);
  clock_wise = clock_wise || m_eType == RLBLOCK;
  clock_wise = clock_wise || m_eType == LBLOCK || m_eType == TBLOCK;
  rot_direction = clock_wise ? CLOCK_WISE : ANTI_CLOCK_WISE;

  m_eState = (m_eState == HORIZONTAL) ? VERTICAL : HORIZONTAL;
  sv_direction = (rot_direction == CLOCK_WISE ? ANTI_CLOCK_WISE : CLOCK_WISE);

  switch (m_eType) {
  case SQUARE:
    break;
  case TBLOCK:
  case SWAGER:
  case RSWAGER:
  case LINEPIECE:
    point.x = m_vecBlocks[1].x + SIDE_LEN / 2;
    point.y = m_vecBlocks[1].y + SIDE_LEN / 2;
    rotate(rot_direction, point);
    break;
  case LBLOCK:
  case RLBLOCK:
    point.x = m_vecBlocks[2].x + SIDE_LEN / 2;
    point.y = m_vecBlocks[2].y + SIDE_LEN / 2;
    rotate(rot_direction, point);
    break;
  case NONE:
    break;
  }

  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    int line_idx = m_vecBlocks[i].y / SIDE_LEN;
    int column_idx = (m_vecBlocks[i].x - GAME_LEFT_BORDER) / SIDE_LEN;
    if ((column_idx < 0) || (m_vecBlocks[i].x < int(GAME_LEFT_BORDER))
        || (m_vecBlocks[i].x > int(GAME_RIGHT_BORDER - SIDE_LEN))
        || (m_vecBlocks[i].y > int(WINDOW_HEIGHT - SIDE_LEN))
        || (m_vecBlocks[i].y < 0) || bitmap[line_idx].test(column_idx))
    {
      can_rotate = false;
      m_eState = save_state;
      break;
    }
  }
  if (!can_rotate)  // can not rotate, restore
    rotate(sv_direction, point);
  return 0;
}

// execute rotate
int CCombinedBlock::rotate(DIRECTION direction, const CPoint &center)
{
  CPoint point;
  for (uint i = 0; i < mapTypeSize[m_eType]; i++)
  {
    point = { m_vecBlocks[i].x, m_vecBlocks[i].y };
    if(direction == CLOCK_WISE)                // clockwise rotation
    {
      m_vecBlocks[i].x = center.x + (center.y - point.y) - SIDE_LEN;
      m_vecBlocks[i].y = center.y - (center.x - point.x);
    }
    else if(direction == ANTI_CLOCK_WISE)     // anti-clockwise rotation
    {
      m_vecBlocks[i].x = center.x + (point.y - center.y);
      m_vecBlocks[i].y = center.y - (point.x - center.x) - SIDE_LEN;
    }
  }
  return 0;
}

