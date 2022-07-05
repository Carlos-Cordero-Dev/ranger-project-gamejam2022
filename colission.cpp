
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
