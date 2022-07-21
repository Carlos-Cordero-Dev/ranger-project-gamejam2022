
void DrawCircle(int radius,int x, int y,SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

  int KCirclePoints = 400;
  float angle = M_PI * 2 / KCirclePoints;
  for(int i=0;i<KCirclePoints;i+=2)
  {
    float x1 = cos(i * angle) * radius;
    float y1 = sin(i * angle) * radius;

    float x2 = cos((i+1) * angle) * radius;
    float y2 = sin((i+1) * angle) * radius;

    SDL_RenderDrawLineF(renderer, x + x1, y + y1,x + x2,y + y2);
  }
}

void DrawCurrentPolygon(Coord *stack,SDL_Renderer *renderer)
{
  if(stack!=nullptr && stack->nextCoord!=nullptr)
  {
    Coord *aux;
    for(aux=stack;aux->nextCoord!=nullptr;aux=aux->nextCoord)
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawLineF(renderer, aux->x,aux->y,aux->nextCoord->x,aux->nextCoord->y);

      if((aux->nextCoord)->nextCoord==nullptr)
      {
        DrawCircle(MARGIN_TO_CLOSE_POLY,aux->nextCoord->x,aux->nextCoord->y,renderer);
      }
    }
  }
}

bool IsPolygonClosed(Coord *stack, int x,int y)
{
  if(stack!=nullptr && stack->nextCoord!=nullptr)
  {
    if(GetStackCount(stack) > MARGIN_TO_CLOSE_POLY / 2)
    {
      Coord *aux = BotStack(stack,0);
      if(abs(aux->x - x) <= MARGIN_TO_CLOSE_POLY && abs (aux->y - y) <= MARGIN_TO_CLOSE_POLY ) return true;
      else return false;
    }
    else return false;

  }
  else return false;
}
