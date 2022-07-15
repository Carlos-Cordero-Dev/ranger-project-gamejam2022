#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Sprite{
  int id; //id on the numbered spriteSheet
  SDL_Texture* texture = nullptr;
  SDL_Rect onScreenSizeRect; //dimensions that the sprite will have on screen
  SDL_Rect singleSpriteDimensionsRect; //if spriteSheet is 800 x 600, here goes the dimensions of every individual sprite

};
struct SpriteSheet{

  Sprite testSprite; //later it'll be a vector / pointer of sprites
  Sprite *sprites = nullptr;
  SDL_Texture* spriteSheetTexture = nullptr;
  int singleSpriteW,singleSpriteH,spritesPerRow,spritesPerColumn; //sprites per row/column dont match the ones in the spritesheet,
                                                                  //these are meant to match colunns and rows from the .level files
};
SpriteSheet tileset_grass,atack_animation,idle_animation;

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
    printf("sprites col: %d row: %d\n",spriteSheetNumberOfCols,spriteSheetNumberOfRows);

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

        // printf("id: %d current number of sprites: %d\n",spriteSheet->sprites[currentSprite].id,currentSprite);
        printf("sprite number: %d\n",currentSprite);
        printf("size: %d %d\n",spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.w,spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.h);
        printf("pos: %d %d\n",spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.x,spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.y);
        currentSprite++;
      }
    }

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

void DrawSpriteFromAnimation(int id,SpriteSheet spriteSheet,SDL_Renderer *renderer)
{
  if(id < 0 || id >= spriteSheet.spritesPerRow * spriteSheet.spritesPerColumn)
  {
    printf("Tried to draw sprite with incorrect Id\n");
  }
  else
  {
    // printf("paiting sprite %d\n",id);
    // printf("size: %d %d\n",spriteSheet.sprites[id].singleSpriteDimensionsRect.w,spriteSheet.sprites[id].singleSpriteDimensionsRect.h);
    // printf("pos: %d %d\n",spriteSheet.sprites[id].singleSpriteDimensionsRect.x,spriteSheet.sprites[id].singleSpriteDimensionsRect.y);
    SDL_RenderCopy(renderer,spriteSheet.spriteSheetTexture,&spriteSheet.sprites[id].singleSpriteDimensionsRect,
      &spriteSheet.sprites[id].onScreenSizeRect);
  }
}
