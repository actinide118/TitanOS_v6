class Board{
    constructor(base_x,base_y,width,height,color){
        this.base_x=base_x
        this.base_y=base_y
        this.width=width
        this.height=height
        this.color=color
    }
}

class Point{
    constructor(x,y){
        this.x=x
        this.y=y
    }
}

class Line{
    /**
     * 
     * @param {Point} p1 
     * @param {Point} p2 
     */
    constructor(p1,p2){
        this.point1=p1
        this.point2=p2
    }
}
/**
 * 
 * @param {Board} board 
 * @param {Point} point 
 * @param {Number} color 
 * @returns 
 */
function put_point( board, point, color){
  if(board.width<point.x || board.height<point.y){
    console.log(`rejected: ${point.x};${point.y}\n`);
    return;
  }
  console.log(`put: ${point.x};${point.y}`)
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_line
 *  Description: 
 * =====================================================================================
 */
/**
 * 
 * @param {Board} board 
 * @param {Line} line 
 * @param {Number} color 
 */
function put_line(board, line,color){
  let vector_x=line.point2.x-line.point1.x;
  let vector_y=line.point2.y-line.point1.y;
  let cartesian_a=vector_y;
  let cartesian_b=-vector_x;
  console.log(`(${vector_x},${vector_y}) a:${cartesian_a}, b:${cartesian_b}`)
  let last_y_shown=0;
  let temp=new Point(0,0)
  if(vector_x > 0){
    for(let i=1;i<=vector_x;i++){
      //console.log("vec")
      let exact_y=(cartesian_a/-cartesian_b)*(i);
      let rendered_y=Math.floor(exact_y);
      //console.log(`vec2-3 prep: l:${last_y_shown} r:${rendered_y}`)
      if(rendered_y > last_y_shown){
        
        for(let j=last_y_shown;j<=rendered_y;j++){
          //console.log("vec2")
          temp.x=line.point1.x+i;
          temp.y=line.point1.y+j;
          put_point(board,temp,color);
        }
        last_y_shown=rendered_y;
      }else{
        for(let j=last_y_shown;j>=rendered_y;j--){
          //console.log("vec3")
          temp.x=line.point1.x+i;
          temp.y=line.point1.y+j;
          put_point(board,temp,color);
        }
        last_y_shown=rendered_y;
      }
    }
  }else if(vector_x < 0){
    for(let i=1;i<=vector_x;i++){
      let exact_y=cartesian_a/-cartesian_b*(line.point1.x-1);
      let rendered_y=Math.floor(exact_y);
      if(rendered_y > last_y_shown){
        for(let j=last_y_shown;j<rendered_y;j++){
          temp.x=line.point1.x-i;
          temp.y=line.point1.y+j;
          put_point(board,temp,color);
        }
      }else{
        for(let j=last_y_shown;j>rendered_y;j++){
          temp.x=line.point1.x-i;
          temp.y=line.point1.y-j;
          put_point(board,temp,color);
        }
      }
    }
  }else{
    if(vector_y > 0){
      for(let i=0; i<vector_y;i++){
        temp.x=line.point1.x;
        temp.y=line.point1.y+i;
        put_point(board,temp,color);
      }
    }else if(vector_y < 0){
      for(let i=0; i<vector_y;i++){
        temp.x=line.point1.x;
        temp.y=line.point1.y+i;
        put_point(board,temp,color);

      }
    }else{
      temp.x=line.point1.x;
      temp.y=line.point2.y;
      put_point(board,temp,color);

    }
    
  }
};

function test(){
board=new Board(
    base_x=0,
    base_y=0,
    width=100,
    height=50,
    color=0,
);
   p1=new Point(
    x=1,
    y=3,
   )
   p2=new Point(
    x=100,
    y=50,
   );
line=new Line(
    point1=p1,
    point2=p2,
);
  put_point(board,p1,14);
  put_line(board,line,14);
}

test()