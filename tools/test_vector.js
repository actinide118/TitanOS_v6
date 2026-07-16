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

class Triangle{
    /**
     * 
     * @param {Point} p1 
     * @param {Point} p2 
     * @param {Point} p3 
     */
    constructor(p1,p2,p3){
        this.point1=p1
        this.point2=p2
        this.point3=p3
    }
}


let matrix=[]
for(let i=0;i<20;i++){
  let temp=[]
  for(let j=0;j<20;j++){
    temp.push("0")
  }
  matrix.push(temp)
}

/**
 * 
 * @param {Board} board 
 * @param {Point} point 
 * @param {Number} color 
 * @returns 
 */
function put_point( board, point, color){
  if(board.width<point.x || board.height<point.y || point.y<0||point.x<0){
    console.log(`rejected: ${point.x};${point.y}\n`);
    return;
  }
  //console.log(`put: ${point.x};${point.y}`)
  matrix[point.y][point.x]="x"
  //console.log(`put: ${point.x};${point.y}`)
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
  console.log(`p1: (${line.point1.x};${line.point1.y}) p2: (${line.point2.x};${line.point2.y})`)
  put_point(board,line.point1,color)
  put_point(board,line.point2,color)
  let vector_x=line.point2.x-line.point1.x;
  let vector_y=line.point2.y-line.point1.y;
  let cartesian_a=vector_y;
  let cartesian_b=-vector_x;
  //console.log(`(${vector_x},${vector_y}) a:${cartesian_a}, b:${cartesian_b}`)
  let last_y_shown=0;
  let temp=new Point(0,0)
  if(vector_x > 0){
    for(let i=0;i<vector_x;i++){
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
    for(let i=0;i>vector_x;i--){
      let exact_y=cartesian_a/-cartesian_b*(i);
      let rendered_y=Math.floor(exact_y);
      if(rendered_y > last_y_shown){
        for(let j=last_y_shown;j<=rendered_y;j++){
          temp.x=line.point1.x+i;
          temp.y=line.point1.y+j;
          put_point(board,temp,color);
        }
        last_y_shown=rendered_y
      }else{
        for(let j=last_y_shown;j>=rendered_y;j--){
          temp.x=line.point1.x+i;
          temp.y=line.point1.y+j;
          put_point(board,temp,color);
        }
        last_y_shown=rendered_y
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

/**
 * 
 * @param {Board} board 
 * @param {Triangle} triangle 
 * @param {Number} color 
 */
function put_triangle(board, triangle, color){
  /*let vectorabx=triangle.point2.x-triangle.point1.x
  let vectoraby=triangle.point2.y-triangle.point1.y
  let vectoracx=triangle.point3.x-triangle.point1.x
  let vectoracy=triangle.point3.y-triangle.point1.y*/
  let vectorbcx=triangle.point3.x-triangle.point2.x
  let vectorbcy=triangle.point3.y-triangle.point2.y
  //console.log(`ab: (${vectorabx},${vectoraby}) \n ac: (${vectoracx},${vectoracy})\n bc:(${vectorbcx},${vectorbcy})`)
  
  /*let cartesian_a_ab=vectoraby
  let cartesian_b_ab=-vectorabx
  let cartesian_a_ac=vectoracy
  let cartesian_b_ac=-vectoracx*/
  let cartesian_a_bc=vectorbcy
  let cartesian_b_bc=-vectorbcx
  /*let minx=triangle.point1.x
  if(minx>triangle.point2.x) minx=triangle.point2.x
  if(minx>triangle.point3.x) minx=triangle.point3.x
  let maxx=0
  if(maxx<triangle.point2.x) maxx=triangle.point2.x
  if(maxx<triangle.point3.x) maxx=triangle.point3.x
  let miny=0
  if(miny>triangle.point2.y) miny=triangle.point2.y
  if(miny>triangle.point3.y) miny=triangle.point3.y
  let maxy=0
  if(maxy<triangle.point2.y) maxy=triangle.point2.y
  if(maxy<triangle.point3.y) maxy=triangle.point3.y
  */
  let p1=new Point(0,0)
  let p2=new Point(0,0)
  let temp =new Line(p1,p2)
  temp.point1=triangle.point2
  temp.point2=triangle.point3
  put_line(board,temp,color)
  temp.point1=triangle.point1
  put_line(board,temp,color)
  temp.point2=triangle.point2
  put_line(board,temp,color)
  last_y_shown=0
  let bc_x_direction=(vectorbcx>0) ? 1:-1
  if(vectorbcx==0){}else{
    for(let i=0;(i<=vectorbcx && bc_x_direction==1)||(i>=vectorbcx && bc_x_direction==-1);i+=bc_x_direction){
      console.log("iteration")
      let exact_y=(cartesian_a_bc/-cartesian_b_bc)*(i);
      let rendered_y=Math.floor(exact_y);
      console.log(`r:${rendered_y}, l:${last_y_shown}`)
      if(rendered_y > last_y_shown){
        for(let j=last_y_shown;j<=rendered_y;j++){
          p1.x=triangle.point2.x+i;
          p1.y=triangle.point2.y+j;
          temp.p1=p1
          temp.p2=triangle.point1
          put_line(board,temp,color);
        }
        last_y_shown=rendered_y;
      }else{
        for(let j=last_y_shown;j>=rendered_y;j--){
          p1.x=triangle.point2.x+i;
          p1.y=triangle.point2.y+j;
          temp.point1=p1
          temp.point2=triangle.point1
          put_line(board,temp,color);
        }
        last_y_shown=rendered_y;
      }
      
    }
  }
}

/**
 * 
 * @param {Board} board 
 * @param {Triangle} triangle 
 * @param {Number} color 
 */
function put_triangle_v2(board, triangle, color){
  if((((triangle.point3.y-triangle.point1.y)*(triangle.point2.x-triangle.point1.x))/(triangle.point2.y-triangle.point1.y))==(triangle.point3.x-triangle.point1.x)){
    return;
  }
  let minx=triangle.point1.x
  if(minx>triangle.point2.x) minx=triangle.point2.x
  if(minx>triangle.point3.x) minx=triangle.point3.x
  let maxx=triangle.point1.x
  if(maxx<triangle.point2.x) maxx=triangle.point2.x
  if(maxx<triangle.point3.x) maxx=triangle.point3.x
  let miny=triangle.point1.y
  if(miny>triangle.point2.y) miny=triangle.point2.y
  if(miny>triangle.point3.y) miny=triangle.point3.y
  let maxy=triangle.point1.y
  if(maxy<triangle.point2.y) maxy=triangle.point2.y
  if(maxy<triangle.point3.y) maxy=triangle.point3.y

  let pool=[triangle.point1,triangle.point2,triangle.point3].sort((a,b)=>{a.y-b.y})
  let highest=pool.pop()
  pool=pool.sort((a,b)=>{a.x-b.x})
  let leftist=pool.pop()
  let righter=pool.pop()

}
/**
 * @param {Board} board 
 * @param {Triangle} triangle
 * @param {Number} color  
 */
function put_triangle_rectangle(board,triangle,color){
  //points non alignes
  if((((triangle.point3.y-triangle.point1.y)*(triangle.point2.x-triangle.point1.x))/(triangle.point2.y-triangle.point1.y))==(triangle.point3.x-triangle.point1.x)){
    return;
  }
  //orthogonalite des axes
  if((((triangle.point3.x-triangle.point1.x)*(triangle.point2.x-triangle.point1.x))+((triangle.point3.y-triangle.point1.y)*(triangle.point2.y-triangle.point1.y)))!=0){
    return
  }


  let vectorbcx=triangle.point3.x-triangle.point2.x
  let vectorbcy=triangle.point3.y-triangle.point2.y
  if(triangle.point3.x!=triangle.point1.x){
    vectorbcx=-vectorbcx
    vectorbcy=-vectorbcy
  }
  let cartesian_a=vectorbcy
  let cartesian_b=-vectorbcx

  let directionx= (vectorbcx > 0)? 1:-1
  let directiony= (vectorbcy > 0)? 1:-1

  console.log(`directionx: ${directionx}; vecx: ${vectorbcx}; vecy:${vectorbcy}; a:${cartesian_a}; b:${cartesian_b}`)

  let target=[vectorbcx,vectorbcy].sort((a,b)=>{return Math.abs(a)-Math.abs(b)})[1]
  let targetdir=(target>0)? 1:-1
  /*for (let i=0; i!=target;i+=targetdir){
    let p1=new Point(triangle.point1.x+vectorbcx,triangle.point1.y)
    let p2=new Point(triangle.point1.x,triangle.point1.y+vectorbcy)
    let temp=new Line(p1,p2)
    put_line(board,temp,color)
    vectorbcx-=directionx
    vectorbcy-=directiony

  }*/
  /*for(let i=0;((i<=vectorbcx&&directionx==1)||(i>=vectorbcx&&directionx==-1));i+=directionx){
    let exact=(cartesian_a*i)/-cartesian_b
    let rendered_y=Math.floor(exact)
    console.log("ry: "+rendered_y+" re: "+exact+" i: "+i)
    let p1=new Point(triangle.point1.x+(i),triangle.point1.y)
    let p2=new Point(triangle.point1.x+(i),triangle.point1.y+(rendered_y*directiony))
    let temp=new Line(p1,p2)
    put_line(board,temp,color)
  }*/
}

/**
 * 
 * @param {Board} board 
 * @param {Triangle} triangle 
 * @param {Number} color 
 */
function put_triangle_rectangle_v2(board,triangle,color){
  //points non alignes
  if((((triangle.point3.y-triangle.point1.y)*(triangle.point2.x-triangle.point1.x))/(triangle.point2.y-triangle.point1.y))==(triangle.point3.x-triangle.point1.x)){
    return;
  }
  //orthogonalite des axes
  if((((triangle.point3.x-triangle.point1.x)*(triangle.point2.x-triangle.point1.x))+((triangle.point3.y-triangle.point1.y)*(triangle.point2.y-triangle.point1.y)))!=0){
    return;
  }
  /*put_point(board,triangle.point1,color)
  put_point(board,triangle.point2,color)
  put_point(board,triangle.point3,color)*/
  let horizontal;
  let vertical;
  if(triangle.point2.x==triangle.point1.x){
    horizontal=triangle.point2
    vertical=triangle.point3
  }else{
    horizontal=triangle.point3
    vertical=triangle.point2
  }
  let vector_x = vertical.x-horizontal.x
  let vector_y = vertical.y-horizontal.y

  let cartesian_a=vector_y
  let cartesian_b=-vector_x

  let x_direction = (vector_x>0)? 1:-1
  let y_direction = (vector_y>0)? 1:-1

  for(let i=0; i!=vector_x+x_direction; i+=x_direction){
    let rendered_y=Math.floor((cartesian_a*(i))/-cartesian_b)
    console.log(rendered_y)
    let p1=new Point(triangle.point1.x+i,triangle.point1.y)
    let p2=new Point(triangle.point1.x+i,horizontal.y+rendered_y)
    let temp =new Line(p1,p2)
    put_line(board,temp,color)
  }
}

function test(){
board=new Board(
    base_x=0,
    base_y=0,
    width=100,
    height=50,
    color=0,
);
  p1=new Point(
    x=2,
    y=3,
   )
  p2=new Point(
    x=4,
    y=11,
   );
  p3=new Point(7,7)
  line=new Line(
    point1=p1,
    point2=p2,
  );
  p4=new Point(5,4)
  p5=new Point(5,9)
  p6=new Point(0,4)

  triangle=new Triangle(p1,p2,p3)
  triangle2=new Triangle(p4,p6,p5)
  //put_point(board,p1,14);
  //put_triangle(board,triangle,14);
  put_triangle_rectangle_v2(board,triangle2,1)
}

test()

console.log(matrix.reverse().map(e=>e.join("")).join("\n"))
