
void LoadLevelSpriteSheet(SDL_Renderer *renderer,SpriteSheet *spriteSheet,const char *filepath,
  int spriteSheetNumberOfRows,int spriteSheetNumberOfCols,int screenXoffset,int screenYoffset,float screenScaling)
{
  //Source: https://caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html

  //loads from left to right and then from top to bottom
  // 0 1 2
  // 3 4 5

  //getting spritesheet image
  spriteSheet->spriteSheetTexture = IMG_LoadTexture(renderer,filepath);
  if (!spriteSheet->spriteSheetTexture) {
       SDL_Log("Couldn't load spritesheet: %s\n", SDL_GetError());
  }
  else
  {
    //individual sprites initialization
    int currentSprite = 0;
    for(int j = 0 ;j<spriteSheet->spritesPerColumn;j++)
    {
      for(int i = 0 ;i<spriteSheet->spritesPerRow;i++)
      {
        //if spriteSheet is 800 x 600, here goes the dimensions of every individual sprite
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.x = (spriteSheet->sprites[currentSprite].id % spriteSheetNumberOfRows)* spriteSheet->singleSpriteW; //changin this in increments of (in this case 32) changes the sprite
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.y = (int)(spriteSheet->sprites[currentSprite].id / spriteSheetNumberOfCols) * spriteSheet->singleSpriteH;
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.w = spriteSheet->singleSpriteW; //if you dont know use SDL_QueryTexture
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.h = spriteSheet->singleSpriteH;


        spriteSheet->sprites[currentSprite].onScreenSizeRect.x = screenXoffset+/*offset+*//*scaling*/screenScaling*i*spriteSheet->singleSpriteW; //pos x on screen
        spriteSheet->sprites[currentSprite].onScreenSizeRect.y = screenYoffset+/*offset+*//*scaling*/screenScaling*j*spriteSheet->singleSpriteH; //pos y on screen
         //scales the original resolution (if original is 32, 64 is double etc)
        spriteSheet->sprites[currentSprite].onScreenSizeRect.w = /*scaling*/screenScaling*spriteSheet->singleSpriteW;
        spriteSheet->sprites[currentSprite].onScreenSizeRect.h = /*scaling*/screenScaling*spriteSheet->singleSpriteH;

        // printf("id: %d current number of sprites: %d\n",spriteSheet->sprites[currentSprite].id,currentSprite);
        currentSprite++;
      }
    }
  }
}
void LoadSpriteSheetAnimation(SDL_Renderer *renderer,SpriteSheet *spriteSheet,const char *filepath,
  int spriteSheetNumberOfRows,int spriteSheetNumberOfCols,int screenXpos,int screenYpos,float screenScaling)
{
  //Source: https://caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html

  //loads from top to bottom and then from left to right
  // 0 3
  // 1 4
  // 2 5

  //getting spritesheet image
  spriteSheet->spriteSheetTexture = IMG_LoadTexture(renderer,filepath);
  if (!spriteSheet->spriteSheetTexture) {
       SDL_Log("Couldn't load spritesheet: %s\n", SDL_GetError());
  }
  else
  {
    //individual sprites initialization

    spriteSheet->spritesPerRow = spriteSheetNumberOfRows;
    spriteSheet->spritesPerColumn = spriteSheetNumberOfCols;
    // printf("sprites col: %d row: %d\n",spriteSheetNumberOfCols,spriteSheetNumberOfRows);

    spriteSheet->sprites = (Sprite*) malloc (spriteSheetNumberOfRows*spriteSheetNumberOfCols*sizeof(Sprite));
    int spriteSizeX,spriteSizeY;
    SDL_QueryTexture(spriteSheet->spriteSheetTexture,NULL,NULL,&spriteSizeX,&spriteSizeY);
    spriteSheet->singleSpriteW= spriteSizeX/spriteSheetNumberOfCols;
    spriteSheet->singleSpriteH= spriteSizeY/spriteSheetNumberOfRows;

    int currentSprite = 0;
    for(int j = 0 ;j<spriteSheetNumberOfCols;j++)
    {
      for(int i = 0 ;i<spriteSheetNumberOfRows;i++)
      {
        spriteSheet->sprites[currentSprite].id = currentSprite;
        //if spriteSheet is 800 x 600, here goes the dimensions of every individual sprite
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.x = j * spriteSheet->singleSpriteW; //changin this in increments of (in this case 32) changes the sprite
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.y = i * spriteSheet->singleSpriteH;
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.w = spriteSheet->singleSpriteW; //if you dont know use SDL_QueryTexture
        spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.h = spriteSheet->singleSpriteH;


        spriteSheet->sprites[currentSprite].onScreenSizeRect.x = screenXpos; //pos x on screen
        spriteSheet->sprites[currentSprite].onScreenSizeRect.y = screenYpos; //pos y on screen
         //scales the original resolution (if original is 32, 64 is double etc)
        spriteSheet->sprites[currentSprite].onScreenSizeRect.w = /*scaling*/screenScaling*spriteSheet->singleSpriteW;
        spriteSheet->sprites[currentSprite].onScreenSizeRect.h = /*scaling*/screenScaling*spriteSheet->singleSpriteH;

        currentSprite++;
      }
    }

  }
}

void LoadSingleSprite(SDL_Renderer *renderer,Sprite *sprite,int startingPosX,int startingPosY,
  int spriteWidth,int spriteHeight,float screenScaling,const char *filepath)
{
  sprite->texture = IMG_LoadTexture(renderer,filepath);
  if (!sprite->texture) {
       SDL_Log("Couldn't load sprite: %s\n", SDL_GetError());
  }
  else
  {
    //dimensions in spritesheet
    sprite->singleSpriteDimensionsRect.x = startingPosX;
    sprite->singleSpriteDimensionsRect.y = startingPosY;
    sprite->singleSpriteDimensionsRect.w = spriteWidth;
    sprite->singleSpriteDimensionsRect.h = spriteHeight;

    //dimension on screen (can be changed later)
    sprite->onScreenSizeRect.x = 0;
    sprite->onScreenSizeRect.y = 0;
    sprite->onScreenSizeRect.w = spriteWidth * screenScaling;
    sprite->onScreenSizeRect.h = spriteHeight * screenScaling;
  }
}


void DrawMap(SpriteSheet spriteSheet,SDL_Renderer *renderer)
{
  for(int i = 0 ;i<spriteSheet.spritesPerRow * spriteSheet.spritesPerColumn;i++)
  {
    SDL_RenderCopy(renderer,spriteSheet.spriteSheetTexture,&spriteSheet.sprites[i].singleSpriteDimensionsRect,
      &spriteSheet.sprites[i].onScreenSizeRect);
  }
}

void DrawSpriteFromAnimation(int id,SpriteSheet *spriteSheet,int screenXpos,int screenYpos,SDL_Renderer *renderer)
{
  if(id < 0 || id >= spriteSheet->spritesPerRow * spriteSheet->spritesPerColumn)
  {
    printf("Tried to draw sprite with incorrect Id\n");
  }
  else
  {
    spriteSheet->sprites[id].onScreenSizeRect.x = screenXpos;
    spriteSheet->sprites[id].onScreenSizeRect.y = screenYpos;

    SDL_RenderCopy(renderer,spriteSheet->spriteSheetTexture,&spriteSheet->sprites[id].singleSpriteDimensionsRect,
      &spriteSheet->sprites[id].onScreenSizeRect);
  }
}

void DrawSingleSprite(Sprite *sprite,int screenXpos,int screenYpos,SDL_Renderer *renderer)
{
  sprite->onScreenSizeRect.x = screenXpos;
  sprite->onScreenSizeRect.y = screenYpos;

  SDL_RenderCopy(renderer,sprite->texture,&sprite->singleSpriteDimensionsRect,&sprite->onScreenSizeRect);
}
