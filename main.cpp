#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <math.h> //pow
#include <stdlib.h> //atoi

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>

#include "cpp_files/constants.cpp"
#include "cpp_files/structs.cpp"
#include "cpp_files/FIFO.cpp"
#include "cpp_files/colission.cpp"
#include "cpp_files/sprites.cpp"
#include "cpp_files/levelReader.cpp"
#include "cpp_files/playerPolygon.cpp"
#include "cpp_files/enemy.cpp"
#include "cpp_files/healthbar.cpp"
#include "cpp_files/variables.cpp"


int main( int argc, char *argv[] )
{
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

    //map initialization
    LoadLevelFromFile(&tileset_grass,"./level_data/lvl1.level");
    LoadLevelSpriteSheet(renderer,&tileset_grass,"./resources/Top_Down-Basic/Tileset_Grass.png",8,8 /*8 x 8 spritesheet*/,
      100,0/*x,y starting pos on screen*/,4.0f /*scaling*/);

    //healthbar initialization
      //player healthbar
      LoadSingleSprite(renderer,&playerHealthbar.container,1,41,(91-1),(57-41),4.0f,"./resources/healthbar/rpg_style.png");
      LoadSingleSprite(renderer,&playerHealthbar.background,15,28,(89-15),(36-28),4.0f,"./resources/healthbar/rpg_style.png");
      LoadSingleSprite(renderer,&playerHealthbar.bar,15,10,(89-15),(18-10),4.0f,"./resources/healthbar/rpg_style.png");
      //enemy healthbar
      LoadSingleSprite(renderer,&enemyHealthbar.bar,23,30,(81-23),(34-30),4.0f,"./resources/healthbar/enemy_healthbar.png");

    //enemy initilization
      //enemy1
      enemy1.enemyPosX = WIDTH / 2;
      enemy1.enemyPosY = HEIGHT / 2;
      enemy1.currentNumberOfAnimations = 7;
      enemy1.animationSet = (SpriteSheet*) malloc(enemy1.currentNumberOfAnimations*sizeof(SpriteSheet));
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[0],"./resources/Ball_and_Chain_Bot/attack.png",
          8,1,100,100,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[1],"./resources/Ball_and_Chain_Bot/idle.png",
          5,1,200,200,4.0f);
          LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[2],"./resources/Ball_and_Chain_Bot/transition_to_charge.png",
              2,1,200,200,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[3],"./resources/Ball_and_Chain_Bot/charge.png",
          4,1,200,200,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[4],"./resources/Ball_and_Chain_Bot/run.png",
          8,1,200,200,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[5],"./resources/Ball_and_Chain_Bot/hit.png",
          2,1,200,200,4.0f);
      LoadSpriteSheetAnimation(renderer,&enemy1.animationSet[6],"./resources/Ball_and_Chain_Bot/death.png",
          5,1,200,200,4.0f);

      LoadAnimationSetDamageHitboxes(&enemy1,0/*ball and chain enemy*/);

      ChangeEnemyAnimationSet(&enemy1,0);

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
            if(state[SDL_SCANCODE_SPACE])
            {
              if(enemy1.currentAnimationSetId+1 >= enemy1.currentNumberOfAnimations)
                ChangeEnemyAnimationSet(&enemy1,0);
              else ChangeEnemyAnimationSet(&enemy1,enemy1.currentAnimationSetId+1);

              playerHealthbar.currentHp++;
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

             if(PolygonHeadCollidingWithBox(stack,20,enemy1.enemyPosX+enemy1.animationSetXoffset,enemy1.enemyPosY+enemy1.animationSetYoffset,
               enemy1.enemyWidth,enemy1.enemyHeight))
             {
               printf("HIT ENEMY HURTBOX\n");
               DestroyStack(&stack); stack = nullptr;
             }
             else if(enemy1.damageHitboxes[enemy1.currentAnimationSetId]!=nullptr &&
               enemy1.damageHitboxes[enemy1.currentAnimationSetId][enemy1.currentSpriteId].isDamaging)
             { //hitting enemy damagin hitboxes
               if(PolygonHeadCollidingWithBox(stack,30,enemy1.enemyPosX+enemy1.damageHitboxes[enemy1.currentAnimationSetId][enemy1.currentSpriteId].startingPosX,
                 enemy1.enemyPosY+enemy1.damageHitboxes[enemy1.currentAnimationSetId][enemy1.currentSpriteId].startingPosY,
                 enemy1.damageHitboxes[enemy1.currentAnimationSetId][enemy1.currentSpriteId].width,
                 enemy1.damageHitboxes[enemy1.currentAnimationSetId][enemy1.currentSpriteId].height))
                 {
                   printf("HIT BY ENEMY ATACK\n");
                   playerHealthbar.currentHp-=5;
                   DestroyStack(&stack); stack = nullptr;
                 }
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

               if(IsBoxInsideOfPoly(enemy1.enemyPosX+enemy1.animationSetXoffset,enemy1.enemyPosY+enemy1.animationSetYoffset,
                 enemy1.enemyWidth,enemy1.enemyHeight, polyPoints, GetStackCount(stack)))
               {
                 enemy1.hp-=1;
                 printf("Current hp: %d\n",enemy1.hp);
                 playerHealthbar.currentHp++;
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

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
      SDL_RenderClear(renderer);

      DrawMap(tileset_grass,renderer);
      DrawPlayerHealthBar(&playerHealthbar,HEALTHBAR_X_POS,HEALTHBAR_Y_POS,renderer);
      DrawCurrentPolygon(stack,renderer);
      DrawEnemy(enemy1,renderer);
      DrawEnemyHealthbar(&enemyHealthbar,enemy1.hp,enemy1.enemyPosX + enemy1.healthbarXoffset,
        enemy1.enemyPosY + enemy1.healthbarYoffset,renderer);

      SDL_RenderPresent(renderer);

      //Update the surface (still dont know what this does)
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
