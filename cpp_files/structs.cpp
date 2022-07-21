
struct Coord
{ //only used for fifo stack implementation
  int x,y;
  Coord *nextCoord;
};

struct Sprite{
  int id; //id on the numbered spriteSheet
  SDL_Texture* texture = nullptr;
  SDL_Rect singleSpriteDimensionsRect; //if spriteSheet is 800 x 600, here goes the dimensions of every individual sprite
  SDL_Rect onScreenSizeRect; //dimensions that the sprite will have on screen

};
struct SpriteSheet{

  Sprite testSprite; //later it'll be a vector / pointer of sprites
  Sprite *sprites = nullptr;
  SDL_Texture* spriteSheetTexture = nullptr;
  int singleSpriteW,singleSpriteH,spritesPerRow,spritesPerColumn; //sprites per row/column dont match the ones in the spritesheet,
                                                                  //these are meant to match colunns and rows from the .level files
};
struct Healthbar
{
  Sprite container,background,bar;
  int currentHp = 100;
};

struct Hitbox
{
  bool isDamaging = false;
  float startingPosX,startingPosY,width,height;
};

struct enemy{
  int enemyPosX,enemyPosY,enemyWidth,enemyHeight; //actual enemy hurtbox dimensions and position

  bool captured = false;
  int timeMilisecondsSinceCaptured = 0.0f;
  int hp= 100;

  //sprites
  SpriteSheet *animationSet; /*each enemy has its specified animation sets(ex: enemy 1 has atack and idle so far) */
  int currentNumberOfAnimations = 0;
  int currentAnimationSetId,currentSpriteId = 0;
  int animationSetXoffset, animationSetYoffset; //offset from (0,0) of spritesheet to make boundingbox match sprite (to ignore alpha channel)
  int healthbarXoffset,healthbarYoffset;
  //damaging only hitboxes
  Hitbox **damageHitboxes;
  //timers for animations
  int animationCurrentTime, animationLastTime = 0;
};
