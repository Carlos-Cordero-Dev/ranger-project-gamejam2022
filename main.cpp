#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "FIFO.cpp"
#include "pointInsidePoly.cpp"
#include "colission.cpp"
#include "sprites.cpp"
#include "levelReader.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>
# define M_PI         3.141592653589793238462643383279502884L /* pi */
const int WIDTH = 1340, HEIGHT = 800;
const int MARGIN_TO_CLOSE_POLY = 80;

struct enemy{
  int enemyPosX,enemyPosY,enemyWidth,enemyHeight; //actual enemy hurtbox dimensions and position

  bool captured = false;
  int timeMilisecondsSinceCaptured = 0.0f;
  int hp= 50;

  //sprites
  SpriteSheet *animationSet; /*each enemy has its specified animation sets(ex: enemy 1 has atack and idle so far) */
  int currentNumberOfAnimations = 0;
  int currentAnimationSetId,currentSpriteId = 0;
  int animationSetXoffset, animationSetYoffset; //offset from (0,0) of spritesheet to make boundingbox match sprite (to ignore alpha channel)
  //timers for animations
  int animationCurrentTime, animationLastTime = 0;
};
enemy enemy1; //has atack_animation and idle_animation;

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
  }
}

void DrawEnemy(enemy enemy,SDL_Renderer *renderer)
{
  DrawSpriteFromAnimation(enemy.currentSpriteId,&enemy.animationSet[enemy.currentAnimationSetId],
    enemy.enemyPosX,enemy.enemyPosY,renderer);

  //debug
  if(enemy.captured == false)  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  else  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

  SDL_Rect debugEnemyCollider;
  debugEnemyCollider.x =enemy.enemyPosX+enemy.animationSetXoffset;
  debugEnemyCollider.y =enemy.enemyPosY+enemy.animationSetYoffset;
  debugEnemyCollider.w =enemy.enemyWidth;
  debugEnemyCollider.h =enemy.enemyHeight;

  SDL_RenderDrawRect(renderer,&debugEnemyCollider);
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


int main( int argc, char *argv[] )
{

    SDL_Surface *windowSurface = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Window *window = NULL;

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError( ) << std::endl;
    }

    // SDL_Window *window = SDL_CreateWindow( "SDL uwu Window 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI );
    if(SDL_CreateWindowAndRenderer(WIDTH, HEIGHT,  SDL_WINDOW_ALLOW_HIGHDPI ,&window, &renderer))
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
      std::cout << "Couldn't create window and renderer, SDL Error: " << SDL_GetError() << std::endl;
        return 3;
    }

    windowSurface = SDL_GetWindowSurface( window );

    if ( NULL == window )
    {
        std::cout << "Could not create window: " << SDL_GetError( ) << std::endl;

        return EXIT_FAILURE;
    }


    SDL_Event event;
    bool GameIsRunning = true;
    int lastMousePosX , lastMousePosY;
    unsigned int lastTime = 0, currentTime;
    bool mouseRightButtonPressed = false;
    Coord* stack = nullptr;

    //map initialization
    LoadLevelFromFile(&tileset_grass,"./lvl1.level");
    LoadLevelSpriteSheet(renderer,&tileset_grass,"./resources/Top_Down-Basic/Tileset_Grass.png",8,8 /*8 x 8 spritesheet*/,
      100,0/*x,y starting pos on screen*/,4.0f /*scaling*/);

    //enemy initilization
      //enemy1
      enemy1.enemyPosX = WIDTH / 2;
      enemy1.enemyPosY = HEIGHT / 2;
      enemy1.currentNumberOfAnimations = 2;
      enemy1.animationSet = (SpriteSheet*) malloc(enemy1.currentNumberOfAnimations*sizeof(SpriteSheet));
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[0],"./resources/Ball_and_Chain_Bot/attack.png",
          8,1,100,100,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[1],"./resources/Ball_and_Chain_Bot/idle.png",
          5,1,200,200,4.0f);
      ChangeEnemyAnimationSet(&enemy1,1);

    while ( GameIsRunning )
    {
      /*event handling*/
      if ( SDL_PollEvent( &event ) )
      {
        switch (event.type) {
          case SDL_QUIT: /* quit cuando ususario aprieta la x de la ventana o*/
          {
            GameIsRunning = false;
          }
          break;

          case SDL_KEYDOWN:
          {
            switch (event.key.keysym.sym) {
              case SDLK_ESCAPE:
              {
                /* quit cuando ususario usuario  aprienta tecla escape */
                GameIsRunning = false;
              }
              break;
            }

            const Uint8 *state = SDL_GetKeyboardState(NULL);
            float speed = 5.0f;
            if(state[SDL_SCANCODE_RIGHT])
            {
              // printf("tecla derecha apretada\n");
              // pintandoLinea ? pintandoLinea = false : pintandoLinea = true;
              // ShowStack(stack);
              enemy1.enemyPosX+=speed;
            }
            else if(state[SDL_SCANCODE_LEFT])
            {
              enemy1.enemyPosX-=speed;
            }

            if(state[SDL_SCANCODE_UP])
            {
              enemy1.enemyPosY-=speed;
            }
            else if(state[SDL_SCANCODE_DOWN])
            {
              enemy1.enemyPosY+=speed;
            }
          }
          break;

          case SDL_MOUSEBUTTONDOWN:
          {
            mouseRightButtonPressed = true;
          }
          break;
          case SDL_MOUSEBUTTONUP:
          {
            mouseRightButtonPressed = false;
            if(stack!=nullptr) DestroyStack(&stack);
          }
          break;
          // SDL_MOUSEMOTION    = 0x400, /**< Mouse moved */
          // SDL_MOUSEBUTTONDOWN,        /**< Mouse button pressed */
          // SDL_MOUSEBUTTONUP,          /**< Mouse button released */
        }

        if(mouseRightButtonPressed)
        {
            if(event.motion.x != lastMousePosX &&  event.motion.y != lastMousePosY)
             {
               // printf("Current Mouse positon %d %d\n",event.motion.x,event.motion.y);
               InsertCoord(&stack,event.motion.x,event.motion.y);
             }

             lastMousePosX = event.motion.x;
             lastMousePosY = event.motion.y;

             //check if the first few points of the polygon drawn are colliding with the enmey hurtbox
             // for(int i=0;i<4;i++)
             // {
             //   if(PolygonHeadCollidingWithLine(stack,20,enemy1.boundingBox[i],enemy1.boundingBox[i<3?(i+1):0]))
             //   {
             //     printf("POLYGON COLLIDED\n");
             //     DestroyStack(&stack); stack = nullptr;
             //     break;
             //   }
             // }
             if(PolygonHeadCollidingWithBox(stack,20,enemy1.enemyPosX+enemy1.animationSetXoffset,enemy1.enemyPosY+enemy1.animationSetYoffset,
               enemy1.enemyWidth,enemy1.enemyHeight))
             {
               printf("POLYGON COLLIDED\n");
               DestroyStack(&stack); stack = nullptr;
             }


             //see if enemy is inside of poly
             if(IsPolygonClosed(stack,event.motion.x,event.motion.y))
             {
               //for that we need to have coord[n] == coord[0]
               Coord *aux;
               for(aux=stack;aux->nextCoord!=nullptr;aux=aux->nextCoord)
               {
                 if((aux->nextCoord)->nextCoord==nullptr)
                          InsertCoord(&stack,aux->nextCoord->x,aux->nextCoord->y);
               }

               Point* polyPoints = (Point*) malloc(GetStackCount(stack) * sizeof(Point));
               int counter = 0;
               for(aux=stack;aux!=nullptr;aux=aux->nextCoord)
               {
                 polyPoints[counter] = {(float)aux->x,(float)aux->y};

                 counter++;
               }
               counter = 0;

               // for(int i=0;i<4;i++)
               // {
               //   if(pnpoly(enemy1.boundingBox[i], polyPoints, GetStackCount(stack))!= 0)
               //   {
               //     enemy1.hp-=1;
               //     printf("Current hp: %d\n",enemy1.hp);
               //     enemy1.captured = true;
               //     break;
               //   }
               // }
               if(IsBoxInsideOfPoly(enemy1.enemyPosX+enemy1.animationSetXoffset,enemy1.enemyPosY+enemy1.animationSetYoffset,
                 enemy1.enemyWidth,enemy1.enemyHeight, polyPoints, GetStackCount(stack)))
               {
                 enemy1.hp-=1;
                 printf("Current hp: %d\n",enemy1.hp);
                 enemy1.captured = true;
               }

               free(polyPoints); polyPoints = nullptr;
               DestroyStack(&stack); stack = nullptr;
             }

        }
      } //end of event handling

      //timer related stuff
      currentTime = SDL_GetTicks();
      if (currentTime > lastTime + 1000/100) {

        Coord *lastCoord = ExtractFIFO(&stack);
        free(lastCoord); lastCoord = nullptr;
        lastTime = currentTime;
      }

      if(enemy1.captured)
      {
        if(enemy1.timeMilisecondsSinceCaptured <= 1000)
        {
          enemy1.timeMilisecondsSinceCaptured++;
        }
        else
        {
          enemy1.captured = false;
          enemy1.timeMilisecondsSinceCaptured = 0;
        }
      }

      EnemyAnimationTimer(&enemy1);
      // enemy1.animationCurrentTime = SDL_GetTicks();
      // if (enemy1.animationCurrentTime > enemy1.animationLastTime + 1000/10) {
      //
      //   enemy1.currentSpriteId++;
      //   if(enemy1.currentSpriteId > (enemy1.animationSet[enemy1.animationSet[currentAnimationSetId].spritesPerRow
      //   * enemy1.animationSet[enemy1.animationSet[currentAnimationSetId].spritesPerColumn) ) enemy1.currentSpriteId = 0;
      //   enemy1.animationLastTime = enemy1.animationCurrentTime;
      // }

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
      SDL_RenderClear(renderer);

      DrawMap(tileset_grass,renderer);
      DrawCurrentPolygon(stack,renderer);
      DrawEnemy(enemy1,renderer);

      SDL_RenderPresent(renderer);

      //Update the surface (SOLO HAY QUE HACERLO CUANDO VAYAN A HABER CAMBIOS VISUALES)
      //SDL_UpdateWindowSurface( window );
    }

    DestroyStack(&stack);
    SDL_FreeSurface( windowSurface ); windowSurface = NULL;
    // SDL_FreeSurface( spriteSheet ); spriteSheet = NULL;
    SDL_DestroyRenderer(renderer); renderer = NULL;
    SDL_DestroyWindow( window ); window = NULL;
    SDL_Quit( );
    return EXIT_SUCCESS;
}
