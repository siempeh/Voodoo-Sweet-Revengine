#pragma once
#include "Vec3.h"
#include "Rect.h"
#include "Colors.h"
#include "Utility.h"

// ****************************************************************************
struct ScreenGridCell
{
  size_t num_indices;
  size_t indices[8];
  Color* buff;
};

template<int width, int height, int cell_width, int cell_height>
class ScreenGridImpl
{
  static constexpr size_t RESOLUTION_X = width * cell_width;
  static constexpr size_t RESOLUTION_Y = height * cell_height;

public:
  static constexpr size_t NUM_CELLS = width*height;
  static constexpr size_t NUM_CELL_PIXELS = cell_width * cell_height;
  static constexpr size_t WIDTH = width;
  static constexpr size_t HEIGHT = height;
  static constexpr size_t CELL_WIDTH = cell_width;
  static constexpr size_t CELL_HEIGHT = cell_height;

  ScreenGridImpl()
  {
    static_assert(IsPowerOf2(cell_width));
    static_assert(IsPowerOf2(cell_height));

    buff = new Color[RESOLUTION_X * RESOLUTION_Y];
    int cell_buff_sz = cell_width * cell_height;
    int offset = 0;
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j, offset += cell_buff_sz)
      {
        ScreenGridCell& cell = cells[i * width + j];
        cell.num_indices = 0;
        cell.buff = buff + offset;
      }
    }
  }
  ~ScreenGridImpl()
  {
    delete[] buff;
  }

  void PlaceTriangleInCell(Vec2 points[3], int primitive_index)
  {
    int v_start[height];
    for(int i = 0; i < height; ++i)
    {
      v_start[i] = 100000;
    }
    int v_end[height];
    memset(v_end, 0, sizeof(int)*height);
    for(int i = 0; i < 3; ++i)
    {
      points[i].x /= cell_width;
      points[i].y /= cell_height;
    }

    int y_min = height - 1;
    int y_max = 0;
    for(int iEdge = 0; iEdge < 3; ++iEdge)
    {
      Vec2 start = points[iEdge];
      Vec2 end = points[(iEdge + 1) == 3 ? 0 : (iEdge + 1)];

      if(start.y > end.y)
      {
        b2Swap(start, end);
      }
      float y0 = start.y;
      float y1 = end.y;
      float x0 = start.x;
      if((end.y - start.y) == 0.0f)
      {
        continue;
      }
      float dx = (end.x - start.x) / (end.y - start.y);
      // Clip with optimisations
#if 0
      int y0i = y0;
      for(; y0i < 0; ++y0i)
      {
        x0 += dx;
    }
#else
      int y0i = (int)y0;
      if(y0i < 0)
      {
        x0 -= dx*y0i;// Maybe keep it as a float and do floor?
        y0i = 0;
      }
#endif
      y_min = Min(y_min, y0i);
      int y1i = (int)(y1 + 1);
      if(y1i > y_max)
      {
        y_max = y1i;
      }
      if(y1i >= height)
      {
        y_max = height - 1;
        y1i = height - 1;
      }
      x0 += ((float)(y0i)-y0) * dx;
      for(int i = y0i; i <= y1i; ++i)
      {
        v_start[i] = Min((int)(x0 - 0.5f), v_start[i]);
        v_end[i] = Max((int)(x0), v_end[i]);
        x0 += dx;
      }
  }

    for(; y_min < y_max; ++y_min)
    {
      for(int x = Max(v_start[y_min], 0); x <= Min(v_end[y_min], width - 1); ++x)
      {
        ScreenGridCell& cell = cells[y_min * width + x];
        assert(cell.num_indices < _countof(cell.indices) && "Cell indices out of bounds");
        cell.indices[cell.num_indices] = primitive_index;
        ++cell.num_indices;
      }
    }
  }

  void PlaceAABBInCell(RectI rect, int primitive_index, int primitve_index_2)
  {
    // Calculate grid start and end points
    size_t start_x = Max(rect.left / cell_width, 0);
    size_t start_y = Max(rect.bottom / cell_height, 0);
    size_t end_x = Min((rect.right + cell_width - 1) / cell_width, width);
    size_t end_y = Min((rect.top + cell_height - 1) / cell_height, height);

    for(int y = start_y; y < end_y; ++y)
    {
      for(int x = start_x; x < end_x; ++x)
      {
        ScreenGridCell& cell = cells[y * width + x];
        assert(cell.num_indices < _countof(cell.indices) && "Cell indices out of bounds");
        cell.indices[cell.num_indices] = primitive_index;
        cell.num_indices++;
        cell.indices[cell.num_indices] = primitve_index_2;
        cell.num_indices++;
      }
    }
  }

  void UnPackBuffer(Color* destination)
  {
    for(int i = 0; i < height; ++i)
    {
      for(int j = 0; j < width; ++j)
      {
        ScreenGridCell& cell = cells[i * width + j];
        Color* cur = &destination[(i * CELL_HEIGHT)*Graphics::ScreenWidth + j*CELL_WIDTH];
        for(int y = 0; y < cell_height; ++y)
        {
          memcpy(cur, &cell.buff[y*cell_width], sizeof(Color)*ScreenGrid::CELL_WIDTH);
          cur += Graphics::ScreenWidth;
        }
      }
    }
  }

  void Clear()
  {
    for(int i = 0; i < NUM_CELLS; ++i)
    {
      cells[i].num_indices = 0;
    }
    memset(buff, 0, sizeof(Color)*ScreenGrid::RESOLUTION_X * ScreenGrid::RESOLUTION_Y);
  }

  ScreenGridCell cells[width * height];
  Color* buff;
};

// ****************************************************************************
using ScreenGrid = ScreenGridImpl<60, 135, 32, 8>;
//using ScreenGrid = ScreenGridImpl<25, 75, 32, 8>;
//using ScreenGrid = ScreenGridImpl<30, 17, 64, 64>
//using ScreenGrid = ScreenGridImpl<15, 17, 128, 64>;