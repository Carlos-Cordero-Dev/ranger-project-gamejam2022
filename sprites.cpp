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
SpriteSheet tileset_grass;

void LoadSpriteSheet(SDL_Renderer *renderer,SpriteSheet *spriteSheet)
{
  //Source: https://caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html

  //getting spritesheet image
  spriteSheet->spriteSheetTexture = IMG_LoadTexture(renderer,"./resources/Top_Down-Basic/Tileset_Grass.png");
  if (!spriteSheet->spriteSheetTexture) {
       SDL_Log("Couldn't load spritesheet: %s\n", SDL_GetError());
   }

  //individual sprites initialization
  int currentSprite = 0;
  for(int j = 0 ;j<spriteSheet->spritesPerColumn;j++)
  {
    for(int i = 0 ;i<spriteSheet->spritesPerRow;i++)
    {
      //if spriteSheet is 800 x 600, here goes the dimensions of every individual sprite
      spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.x = (spriteSheet->sprites[currentSprite].id % 8)* spriteSheet->singleSpriteW; //changin this in increments of (in this case 32) changes the sprite
      spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.y = (int)(spriteSheet->sprites[currentSprite].id / 8) * spriteSheet->singleSpriteH;
      spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.w = spriteSheet->singleSpriteW; //if you dont know use SDL_QueryTexture
      spriteSheet->sprites[currentSprite].singleSpriteDimensionsRect.h = spriteSheet->singleSpriteH;


      spriteSheet->sprites[currentSprite].onScreenSizeRect.x = 100+/*offset+*//*scaling*/4*i*spriteSheet->singleSpriteW; //pos x on screen
      spriteSheet->sprites[currentSprite].onScreenSizeRect.y = /*offset+*//*scaling*/4*j*spriteSheet->singleSpriteH; //pos y on screen
       //scales the original resolution (if original is 32, 64 is double etc)
      spriteSheet->sprites[currentSprite].onScreenSizeRect.w = /*scaling*/4*spriteSheet->singleSpriteW;
      spriteSheet->sprites[currentSprite].onScreenSizeRect.h = /*scaling*/4*spriteSheet->singleSpriteH;

      // printf("id: %d current number of sprites: %d\n",spriteSheet->sprites[currentSprite].id,currentSprite);
      currentSprite++;
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

void DrawSprite(SpriteSheet spriteSheet,SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor(renderer,0,0,0,255);
  // SDL_RenderClear(renderer);

  SDL_RenderCopy(renderer,spriteSheet.spriteSheetTexture,&spriteSheet.testSprite.singleSpriteDimensionsRect,&spriteSheet.testSprite.onScreenSizeRect);
}
