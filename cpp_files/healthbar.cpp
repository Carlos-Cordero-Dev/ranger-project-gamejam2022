
void DrawPlayerHealthBar(Healthbar *healthbar,int screenXpos,int screenYpos,SDL_Renderer *renderer)
{
  //container
  DrawSingleSprite(&healthbar->container,screenXpos,screenYpos,renderer);
  //background black bar
  DrawSingleSprite(&healthbar->background,screenXpos + 14*4.0f,screenYpos + 2*4.0f,renderer);

  //bar displaying current hp
    //change both spritesheet and onscreen SDL_Rect width to match the current hp %
    SDL_Rect currentBarDimensionsRect = healthbar->bar.singleSpriteDimensionsRect;
    currentBarDimensionsRect.w = (healthbar->currentHp * currentBarDimensionsRect.w) / 100;

    SDL_Rect currentOnScreenSizeRect = healthbar->bar.onScreenSizeRect;
    currentOnScreenSizeRect.x = screenXpos + 14*4.0f/*offset to go inside of container*/;
    currentOnScreenSizeRect.y = screenYpos + 2*4.0f/*offset to go inside of container*/;
    currentOnScreenSizeRect.w = (healthbar->currentHp * currentOnScreenSizeRect.w) / 100;

  SDL_RenderCopy(renderer,healthbar->bar.texture,&currentBarDimensionsRect,&currentOnScreenSizeRect);
}

void DrawEnemyHealthbar(Healthbar *healthbar,int currentEnemyHp,int screenXpos,int screenYpos,SDL_Renderer *renderer)
{
  SDL_Rect currentBarDimensionsRect = healthbar->bar.singleSpriteDimensionsRect;
  currentBarDimensionsRect.w = (currentEnemyHp * currentBarDimensionsRect.w) / 100;

  SDL_Rect currentOnScreenSizeRect = healthbar->bar.onScreenSizeRect;
  currentOnScreenSizeRect.x = screenXpos;
  currentOnScreenSizeRect.y = screenYpos;
  currentOnScreenSizeRect.w*=0.5f; //too wide otherwise
  currentOnScreenSizeRect.w = (currentEnemyHp * currentOnScreenSizeRect.w) / 100;

  SDL_RenderCopy(renderer,healthbar->bar.texture,&currentBarDimensionsRect,&currentOnScreenSizeRect);
}
