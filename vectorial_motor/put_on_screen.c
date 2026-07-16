/*
 * =====================================================================================
 *
 *       Filename:  put_on_screen.c
 *
 *    Description:  Functions to use the renderer
 *
 *        Version:  1.0
 *        Created:  07/11/2026 09:36:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "put_on_screen.h"
#include "../driver/vga.h"





/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_point
 *  Description:  Show a point on screen
 * =====================================================================================
 */
void put_point(render_window_t* board,render_point_t* point,uint8_t color){
  if(board->width<point->x || board->height<point->y){
    //print_string("rejected");
    return;
  }
  M13h_set_pixel((int)board->base_x+point->x,(int)board->base_y+point->y,(int)color);
};

int16_t abs(int16_t nb){
  if(nb <0){
    return -nb;
  }else{
    return nb;
  }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_line
 *  Description:  Print a line
 * =====================================================================================
 */

void put_line(render_window_t* board,render_line_t* line,uint8_t color){
  if(line->point2->x==line->point1->x&&line->point1->y==line->point2->y){
    //print_string("p");
  }
  int16_t vector_x=line->point2->x-line->point1->x;
  int16_t vector_y=line->point2->y-line->point1->y;
  int16_t cartesian_a=vector_y;
  int16_t cartesian_b=-vector_x;
  int16_t last_y_shown=0;
  render_point_t temp;
  if(vector_x > 0){
    //print_string("q");
    for(uint16_t i=1;i<=vector_x;i++){
      float exact_y=cartesian_a/-cartesian_b*(i);
      int16_t rendered_y=(cartesian_a*i)/-cartesian_b;
      if(rendered_y >last_y_shown){
        for(int16_t j=last_y_shown;j<=rendered_y;j++){
          temp.x=line->point1->x+i;
          temp.y=line->point1->y+j;
          put_point(board,&temp,color);
          //print_string("q");
        }
        last_y_shown=rendered_y;
      }else{
        for(int16_t j=last_y_shown;j>=rendered_y;j--){
          temp.x=line->point1->x+i;
          temp.y=line->point1->y+j;
          put_point(board,&temp,color);
          //print_string("w");
        }
        last_y_shown=rendered_y;
      }
    }
  }else if(vector_x < 0){
    for(int16_t i=-1;i>=vector_x;i--){
      float exact_y=cartesian_a/-cartesian_b*(line->point1->x-1);
      int16_t rendered_y=(cartesian_a*i)/-cartesian_b;
      if(rendered_y > last_y_shown){
        for(int16_t j=last_y_shown;j<=rendered_y;j++){
          temp.x=line->point1->x+i;
          temp.y=line->point1->y+j;
          put_point(board,&temp,color);
        }
        last_y_shown=rendered_y;
      }else{
        for(int16_t j=last_y_shown;j>=rendered_y;j--){
          temp.x=line->point1->x+i;
          temp.y=line->point1->y+j;
          put_point(board,&temp,color);
        }
        last_y_shown=rendered_y;
      }
    }
  }else{
    
    if(vector_y > 0){
      for(int16_t i=0; i<vector_y;i++){
        temp.x=line->point1->x;
        temp.y=line->point1->y+i;
        put_point(board,&temp,color);
      }
    }else if(vector_y < 0){
      for(int16_t i=0; i>vector_y;i--){
        temp.x=line->point1->x;
        temp.y=line->point1->y+i;
        put_point(board,&temp,color);

      }
    }else{
      temp.x=line->point1->x;
      temp.y=line->point2->y;
      put_point(board,&temp,color);

    }
    
  }
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_rectangle
 *  Description:  Show a rectangle in the defined enclosed space
 * =====================================================================================
 */
void put_rectangle(render_window_t* board,render_rectangle_t* rec,uint8_t color){
  
  int16_t width=rec->point2->x-rec->point1->x;
  int16_t height=rec->point2->y-rec->point1->y;
  int16_t counter1=0;
  int16_t counter2=0;  
  render_point_t temp;
  if(width>0){
    for(counter1=0;counter1<=width;counter1++){
     
      if(height>0){
        for(counter2=0;counter2<=height;counter2++){
          temp.x=rec->point1->x+counter1;
          temp.y=rec->point1->y+counter2;
          put_point(board,&temp,color);
        }
      }else{
         for(counter2=0;counter2>=height;counter2--){
          temp.x=rec->point1->x+counter1;
          temp.y=rec->point1->y+counter2;
          put_point(board,&temp,color);
        }       
      }
    }
  }else{
     for(counter1=0;counter1>=width;counter1--){
     
      if(height>0){
        for(counter2=0;counter2<=height;counter2++){
          temp.x=rec->point1->x+counter1;
          temp.y=rec->point1->y+counter2;
          put_point(board,&temp,color);
        }
      }else{
         for(counter2=0;counter2>=height;counter2--){
          temp.x=rec->point1->x+counter1;
          temp.y=rec->point1->y+counter2;
          put_point(board,&temp,color);
        }       
      }
    }
  }
}
void put_triangle_rectangle(render_window_t* board,render_triangle_t* triangle,uint8_t color){
  if((((triangle->p3->y-triangle->p1->y)*(triangle->p2->x-triangle->p1->x))/(triangle->p2->y-triangle->p1->y))==(triangle->p3->x-triangle->p1->x)){
    return;
  }
  //orthogonalite des axes
  if((((triangle->p3->x-triangle->p1->x)*(triangle->p2->x-triangle->p1->x))+((triangle->p3->y-triangle->p1->y)*(triangle->p2->y-triangle->p1->y)))!=0){
    return;
  }

  put_point(board,triangle->p1,color);
  put_point(board,triangle->p2,color);
  put_point(board,triangle->p3,color);

  render_point_t* horizontal;
  render_point_t* vertical;
  if(triangle->p2->x==triangle->p1->x){
    horizontal=triangle->p2;
    vertical=triangle->p3;
  }else{
    horizontal=triangle->p3;
    vertical=triangle->p2;
  }
  int16_t vector_x = vertical->x-horizontal->x;
  int16_t vector_y = vertical->y-horizontal->y;

  int16_t cartesian_a=vector_y;
  int16_t cartesian_b=-vector_x;

  int16_t x_direction = (vector_x>0)? 1:-1;
  
  render_point_t p1;
  render_point_t p2;
  render_line_t temp;
  temp.point1=&p1;
  temp.point2=&p2;

  for(int16_t i=0;i!=vector_x+x_direction;i+=x_direction){
    //print_string("q");
    int16_t rendered_y=(cartesian_a*i)/-cartesian_b;
    p1.x=triangle->p1->x+i;
    p1.y=triangle->p1->y;
    p2.x=triangle->p1->x+i;
    p2.y=horizontal->y+rendered_y;
    temp.point1=&p1;
    temp.point2=&p2;
    put_line(board,&temp,color);
  }
}


void test(){
  render_window_t board={
    .base_x=0,
    .base_y=0,
    .width=100,
    .height=50,
    .color=0,
  };
  render_point_t p1={
    .x=50,
    .y=25,
  };
  render_point_t p2={
    .x=100,
    .y=50,
  };
  render_point_t p3={
    .x=0,
    .y=0,
  };
  render_point_t p4={
    .x=100,
    .y=0,
  };
  render_point_t p5={
    .x=0,
    .y=50,
  };
  render_point_t p6={
    .x=0,
    .y=20,
  };
  render_line_t line={
    .point1=&p1,
    .point2=&p2,
  };
  render_line_t line2={
    .point1=&p1,
    .point2=&p3,
  };
  render_line_t line3={
    .point1=&p1,
    .point2=&p4
  };
  render_line_t line4={
    .point1=&p1,
    .point2=&p5,
  };
  render_rectangle_t rec={
    .point1=&p1,
    .point2=&p6,
  };
  render_point_t p7={
    .x=60,
    .y=45,
  };
  render_point_t p8={
    .x=60,
    .y=25,
  };
  render_point_t p9={
    .x=80,
    .y=45,
  };
  render_triangle_t tr={
    .p1=&p7,
    .p2=&p8,
    .p3=&p9,
  };
  
  put_point(&board,&p1,14);
  put_line(&board,&line,14);
  put_line(&board,&line2,14);
  put_line(&board,&line3,14);
  put_line(&board,&line4,14);
  put_rectangle(&board,&rec,4);
  put_triangle_rectangle(&board,&tr,3);
}










