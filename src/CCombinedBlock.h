/*
 * CCombinedBlock.h
 *
 *  Created on: 2018年6月3日
 *      Author: zys
 */

#ifndef CCOMBINEDBLOCK_H_
#define CCOMBINEDBLOCK_H_

#include <vector>
#include <bitset>
#include <map>
#include "common.h"
#include "CWindow.h"

typedef enum {
  SQUARE, LBLOCK, RLBLOCK, SWAGER, RSWAGER, TBLOCK, LINEPIECE, NONE
} SHAPE_TYPE;

typedef enum {
  CLOCK_WISE, ANTI_CLOCK_WISE
} DIRECTION;

typedef enum {
  HORIZONTAL, VERTICAL
} STATE;

class CCombinedBlock {
  friend int DrawBlockShape(CWindow* window, const CCombinedBlock& shape,
      bool draw_next_shape = false);
private:
  std::vector<CRect> m_vecBlocks;
  std::vector<CRect> m_vecNextBlocks;
  SHAPE_TYPE m_eType;
  SHAPE_TYPE m_eNextType;
  STATE m_eState;
  bool m_bOver;

public:
  CCombinedBlock(const CPoint& position, SHAPE_TYPE type = NONE);

  SHAPE_TYPE getShapeType()
  {
    return m_eType;
  }
  void setShapeType(SHAPE_TYPE type)
  {
    m_eType = type;
  }

  SHAPE_TYPE getNextShapeType()
  {
    return m_eNextType;
  }
  void setNextShapeType(SHAPE_TYPE type)
  {
    m_eNextType = type;
  }

  bool isOver()
  {
    return m_bOver;
  }

  void init(const CPoint &position, const SHAPE_TYPE type = NONE);
  void initBlocksPos(std::vector<CRect> &blocks, const CPoint &position,
      SHAPE_TYPE type);
  int moveLeft(std::vector<std::bitset<WIDTH_SIZE>> &bitmap);
  int moveRight(std::vector<std::bitset<WIDTH_SIZE>> &bitmap);
  int moveDown(std::vector<std::bitset<WIDTH_SIZE>> &bitmap);

  int rotate(std::vector<std::bitset<WIDTH_SIZE>> &bitmap);
  int rotate(DIRECTION direction, const CPoint &point);
};

#endif /* CCOMBINEDBLOCK_H_ */
