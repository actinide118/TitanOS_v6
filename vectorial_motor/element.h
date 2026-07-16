/*
 * =====================================================================================
 *
 *       Filename:  element.h
 *
 *    Description:  File containing the desription of the structures used by the renderer
 *
 *        Version:  1.0
 *        Created:  07/11/2026 09:21:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>

typedef struct b{
  uint16_t base_x;
  uint16_t base_y;
  uint16_t width;
  uint16_t height;
  uint8_t color;
}render_window_t;

typedef struct el{
  uint8_t id;
  uint8_t color;
  void* pointer;
}element_t;

#define POINT 0

typedef struct p{
  uint16_t x;
  uint16_t y;
}render_point_t;

#define LINE 1

typedef struct l{
  render_point_t* point1;
  render_point_t* point2;
}render_line_t;

#define POLYLINE 2

typedef struct pl{
  render_point_t* points;
}renderer_polyline_t;

#define RECTANGLE 3

typedef struct sq{ 
  render_point_t* point1;
  render_point_t* point2;
}render_rectangle_t;

#define TRAPEZE 4

typedef struct tr{
  render_point_t* point1;
  render_point_t* point2;
  render_point_t* point3;
  render_point_t* point4;
}render_trapeze_t;

#define TRIANGLE 5

typedef struct t{
  render_point_t* p1;
  render_point_t* p2;
  render_point_t* p3;
}render_triangle_t;









