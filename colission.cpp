
struct Point{
  float x,y;
};
//source: https://gamedev.stackexchange.com/questions/26004/how-to-detect-2d-line-on-line-collision

bool IsIntersecting(Point a, Point b, Point c, Point d)
{
    float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
    float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
    float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

    // Detect coincident lines (has a problem, read below)
    if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

    float r = numerator1 / denominator;
    float s = numerator2 / denominator;

    return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

bool PolygonHeadCollidingWithLine(Coord *stack,int lineCount /*starting from head*/,Point lineOrgin,Point lineEnd)
{
  if(stack == nullptr) return false;
  else
  { //stack not empty

    Coord *aux = stack;
    for(int i=0;i<lineCount;i++)
    {
      //at least 2 points
      if(aux->nextCoord==nullptr) return false;
      else
      {
      Point polyLineOrigin = {(float)aux->x,(float)aux->y}, polyLineEnd = {(float)aux->nextCoord->x,(float)aux->nextCoord->y};

      if(IsIntersecting(polyLineOrigin,polyLineEnd,lineOrgin,lineEnd)) return true;

      //next coord exists
      if(aux->nextCoord!=nullptr)aux = aux->nextCoord;
      else return false;
      }
    }
    return false;
  }
}

//this collision includes checks for middle of the box with 2 lines from opposite vertexes
bool PolygonHeadCollidingWithBox(Coord *stack,int lineCount,float boxOriginX,float boxOriginY,float boxWidth,float boxHeight)
{
  //cross1 (topleft to botright)
  if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX,boxOriginY},{boxOriginX+boxWidth,boxOriginY+boxHeight})) return true;
  //cross2 (topright to botleft)
  else if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX+boxWidth,boxOriginY},{boxOriginX,boxOriginY+boxHeight})) return true;
  //top
  else if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX,boxOriginY},{boxOriginX+boxWidth,boxOriginY})) return true;
  //right
  else if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX+boxWidth,boxOriginY},{boxOriginX+boxWidth,boxOriginY+boxHeight})) return true;
  //bot
  else if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX,boxOriginY+boxHeight},{boxOriginX+boxWidth,boxOriginY+boxHeight})) return true;
  //left
  else if(PolygonHeadCollidingWithLine(stack,lineCount,{boxOriginX,boxOriginY},{boxOriginX,boxOriginY+boxHeight})) return true;

  else return false;
}

int pnpoly(Point P,Point* V,int n)
{
  //modified version of: https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm#:~:text=By%20repeatedly%20inverting%20the%20value,number%2C%20the%20point%20is%20outside.
  int i, j, c = 0;
  for (i = 0, j = n-1; i < n; j = i++) {
    if ( ((V[i].y>P.y) != (V[j].y>P.y)) &&
     (P.x < (V[j].x-V[i].x) * (P.y-V[i].y) / (V[j].y-V[i].y) + V[i].x) )
       c = !c;
  }
  return c;
}

bool IsBoxInsideOfPoly(float boxOriginX,float boxOriginY,float boxWidth,float boxHeight,Point* polygonPoints,int numberOfPolygonPoints)
{
  //topleft
  if(pnpoly({boxOriginX,boxOriginY},polygonPoints,numberOfPolygonPoints) != 0) return true;
  //topright
  else if(pnpoly({boxOriginX+boxWidth,boxOriginY},polygonPoints,numberOfPolygonPoints) != 0) return true;
  //botright
  else if(pnpoly({boxOriginX+boxWidth,boxOriginY+boxHeight},polygonPoints,numberOfPolygonPoints) != 0) return true;
  //botleft
  else if(pnpoly({boxOriginX,boxOriginY+boxHeight},polygonPoints,numberOfPolygonPoints) != 0) return true;

  else return false;
}
