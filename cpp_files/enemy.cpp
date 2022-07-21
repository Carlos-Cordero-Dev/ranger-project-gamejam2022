
void ChangeEnemyAnimationSet(enemy *enemy,int animationSetID)
{
  if(animationSetID < 0 || enemy->currentNumberOfAnimations < animationSetID)
  {
    printf("AnimationSet ID non existant\n");
  }
  else
  {
    //spriteSheet->sprites[currentSprite].onScreenSizeRect.w = /*scaling*/screenScaling*spriteSheet->singleSpriteW;
    float scaling = enemy->animationSet[animationSetID].sprites[0].onScreenSizeRect.w/enemy->animationSet[animationSetID].singleSpriteW;

    switch (animationSetID) { //each sprite has its own actual size ignoring the alpha channel of the spritesheet
      case 0: //atack
      {
        //(55 , 9) - (76 , 32)

        enemy->enemyWidth = (76 - 55)*scaling;
        enemy->enemyHeight = (32 - 9)*scaling;
        //position offset
        enemy->animationSetXoffset = 55*scaling;
        enemy->animationSetYoffset = 9*scaling;
      }
      break;
      case 1: //idle
      {
        //(57 , 11) - (73 , 32)

        enemy->enemyWidth = (73 - 57)*scaling;
        enemy->enemyHeight = (32 - 11)*scaling;
        //position offset
        enemy->animationSetXoffset = 57*scaling;
        enemy->animationSetYoffset = 11*scaling;
      }
      break;
      case 2: //transition to charge
      {
        //(54 , 6) - (78 , 32)

        enemy->enemyWidth = (78 - 54)*scaling;
        enemy->enemyHeight = (32 - 6)*scaling;
        //position offset
        enemy->animationSetXoffset = 54*scaling;
        enemy->animationSetYoffset = 6*scaling;
      }
      break;
      case 3: //charge
      {
        //(52 , 8) - (78 , 32)

        enemy->enemyWidth = (78 - 52)*scaling;
        enemy->enemyHeight = (32 - 8)*scaling;
        //position offset
        enemy->animationSetXoffset = 52*scaling;
        enemy->animationSetYoffset = 8*scaling;
      }
      break;
      case 4: //run
      {
        //(55 , 9) - (78 , 32)

        enemy->enemyWidth = (78 - 55)*scaling;
        enemy->enemyHeight = (32 - 9)*scaling;
        //position offset
        enemy->animationSetXoffset = 55*scaling;
        enemy->animationSetYoffset = 9*scaling;
      }
      break;
      case 5: //hit
      {
        //(54 , 6) - (78 , 32)

        enemy->enemyWidth = (78 - 54)*scaling;
        enemy->enemyHeight = (32 - 6)*scaling;
        //position offset
        enemy->animationSetXoffset = 54*scaling;
        enemy->animationSetYoffset = 6*scaling;
      }
      break;
      case 6: //death
      {
        //(56 , 10) - (78 , 32)

        enemy->enemyWidth = (78 - 56)*scaling;
        enemy->enemyHeight = (32 - 10)*scaling;
        //position offset
        enemy->animationSetXoffset = 56*scaling;
        enemy->animationSetYoffset = 10*scaling;
      }
      break;
      default: //width and height of the alpha channel
      {
        enemy->enemyWidth = enemy->animationSet[animationSetID].sprites[0].onScreenSizeRect.w;
        enemy->enemyHeight = enemy->animationSet[animationSetID].sprites[0].onScreenSizeRect.h;

        enemy->animationSetXoffset = 0;
        enemy->animationSetYoffset = 0;
      }
      break;
    }

    enemy->currentSpriteId = 0;
    enemy->currentAnimationSetId = animationSetID;

    enemy->animationCurrentTime = 0;
    enemy->animationLastTime = 0;

    //healthbar x,y offset
    enemy->healthbarXoffset = 51  * scaling;
    enemy->healthbarYoffset = 36 * scaling;
  }
}

void LoadAnimationSetDamageHitboxes(enemy *enemy,int type/* 0 = ball and chain bot*/)
{
  float scaling = enemy->animationSet[0].sprites[0].onScreenSizeRect.w/enemy->animationSet[0].singleSpriteW;
  switch(type)
  {
    case 0: // ball and chain bot
    {
      enemy->damageHitboxes = (Hitbox**) malloc (enemy->currentNumberOfAnimations * sizeof(Hitbox*));
      for(int i=0;i<enemy->currentNumberOfAnimations;i++)
      {
        switch (i) {
          case 0: //atack animation
          {
            enemy->damageHitboxes[i] = (Hitbox*) malloc(8/*frames of atack animation*/ * sizeof(Hitbox));

            for(int j=0;j<(enemy->animationSet[i].spritesPerRow
              * enemy->animationSet[i].spritesPerColumn);j++)
              {
                switch (j) {
                  case 0: //frame 0
                  {

                    enemy->damageHitboxes[i][j].isDamaging = true;
                    // (57,18) - (116,31)
                    enemy->damageHitboxes[i][j].startingPosX = 57 * scaling;
                    enemy->damageHitboxes[i][j].startingPosY = 18 * scaling;
                    enemy->damageHitboxes[i][j].width = (116 - 57) * scaling;
                    enemy->damageHitboxes[i][j].height = (31 - 18) * scaling;
                  }
                  break;
                  case 4: //frame 4
                  {
                    enemy->damageHitboxes[i][j].isDamaging = true;
                    // each sprite is 126x39
                    // (52,175) - (112,188)
                    enemy->damageHitboxes[i][j].startingPosX = 52 * scaling;
                    enemy->damageHitboxes[i][j].startingPosY = (175 - (39 * j)) * scaling;
                    enemy->damageHitboxes[i][j].width = (112 - 52) * scaling;
                    enemy->damageHitboxes[i][j].height = (188 - 175) * scaling;
                  }
                  break;
                }
              }
          }
          break;
          default: //if that animation set doesnt have damaging hitboxes
          {
            enemy->damageHitboxes[i] = nullptr;
          }
          break;
        }
      }
    }
    break;
  }
}

void DrawEnemy(enemy enemy,SDL_Renderer *renderer)
{
  DrawSpriteFromAnimation(enemy.currentSpriteId,&enemy.animationSet[enemy.currentAnimationSetId],
    enemy.enemyPosX,enemy.enemyPosY,renderer);

  //debug
    //hurtbox
    if(enemy.captured == false)  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    else  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

    SDL_Rect debugEnemyCollider;
    debugEnemyCollider.x =enemy.enemyPosX+enemy.animationSetXoffset;
    debugEnemyCollider.y =enemy.enemyPosY+enemy.animationSetYoffset;
    debugEnemyCollider.w =enemy.enemyWidth;
    debugEnemyCollider.h =enemy.enemyHeight;

    SDL_RenderDrawRect(renderer,&debugEnemyCollider);

    //damagin hitboxes
    if(enemy.damageHitboxes[enemy.currentAnimationSetId]!=nullptr &&
      enemy.damageHitboxes[enemy.currentAnimationSetId][enemy.currentSpriteId].isDamaging)
    {
      SDL_Rect debugEnemyAtackHitbox;
      debugEnemyAtackHitbox.x = enemy.damageHitboxes[enemy.currentAnimationSetId][enemy.currentSpriteId].startingPosX
        +enemy.enemyPosX;
      debugEnemyAtackHitbox.y = enemy.damageHitboxes[enemy.currentAnimationSetId][enemy.currentSpriteId].startingPosY
        +enemy.enemyPosY;
      debugEnemyAtackHitbox.w = enemy.damageHitboxes[enemy.currentAnimationSetId][enemy.currentSpriteId].width;
      debugEnemyAtackHitbox.h = enemy.damageHitboxes[enemy.currentAnimationSetId][enemy.currentSpriteId].height;

      SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawRect(renderer,&debugEnemyAtackHitbox);
    }

}

void EnemyAnimationTimer(enemy *enemy)
{
  enemy->animationCurrentTime = SDL_GetTicks();
  if (enemy->animationCurrentTime > enemy->animationLastTime + 1000/10) {

    enemy->currentSpriteId++;
    if(enemy->currentSpriteId >= (enemy->animationSet[enemy->currentAnimationSetId].spritesPerRow
    * enemy->animationSet[enemy->currentAnimationSetId].spritesPerColumn) ) enemy->currentSpriteId = 0;
    enemy->animationLastTime = enemy->animationCurrentTime;
  }
}
