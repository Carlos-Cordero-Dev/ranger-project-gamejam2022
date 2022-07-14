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
  Point boundingBox[4]; // 0 topleft, 1 topright, 2 botright, 3botleft
  bool captured = false;
  int timeMilisecondsSinceCaptured = 0.0f;
  int hp= 50;

  //sprite
  SDL_Texture* texture = nullptr;
  int img_width,img_height;

};
enemy enemy1;


void InitializeEnemy1(SDL_Renderer *renderer)
{
  //Sprites handling (source: https://github.com/libsdl-org/SDL_image/blob/main/showimage.c)
  enemy1.texture = IMG_LoadTexture(renderer,"./resources/Ball_and_Chain_Bot/hit.png");
  if (!enemy1.texture) {
       SDL_Log("Couldn't load image: %s\n", SDL_GetError());
   }
  SDL_QueryTexture(enemy1.texture,NULL,NULL,&enemy1.img_width,&enemy1.img_height);

  //enemy bounding box
  enemy1.boundingBox[0].x = WIDTH/2;
  enemy1.boundingBox[0].y = HEIGHT/2;
  enemy1.boundingBox[1].x = enemy1.boundingBox[0].x + enemy1.img_width;
  enemy1.boundingBox[1].y = enemy1.boundingBox[0].y;
  enemy1.boundingBox[2].x = enemy1.boundingBox[0].x +enemy1.img_width;
  enemy1.boundingBox[2].y = enemy1.boundingBox[0].y +enemy1.img_height;
  enemy1.boundingBox[3].x = enemy1.boundingBox[0].x;
  enemy1.boundingBox[3].y = enemy1.boundingBox[0].y +enemy1.img_height;

}


void DrawEnemy(enemy enemy,SDL_Renderer *renderer)
{
  if(enemy.captured == false)  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  else  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_Rect enemyRectangle;

  enemyRectangle.x =  enemy1.boundingBox[0].x;
  enemyRectangle.y =  enemy1.boundingBox[0].y;
  enemyRectangle.h = enemy1.img_height;
  enemyRectangle.w = enemy1.img_width;

  SDL_RenderFillRect(renderer,&enemyRectangle);
  // SDL_RenderCopy(renderer,enemy1.texture,NULL,&enemyRectangle);
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

void ShowStackVSpolyPoints(Coord *stack,Point *polyPoints)
{
  int i = 0;
  Coord *aux;
  printf("=====================\n");
  for(aux=stack;aux!=nullptr;aux=aux->nextCoord)
  {
    printf("%d %d %f %f\n",aux->x,aux->y,polyPoints[i].x,polyPoints[i].y);
    i++;
  }
  printf("=====================\n");
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

    InitializeEnemy1(renderer);
    LoadLevelFromFile(&tileset_grass,"./lvl1.level");
    LoadSpriteSheet(renderer,&tileset_grass);
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
            if(state[SDL_SCANCODE_RIGHT])
            {
              // printf("tecla derecha apretada\n");
              // pintandoLinea ? pintandoLinea = false : pintandoLinea = true;
              ShowStack(stack);
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
             for(int i=0;i<4;i++)
             {
               if(PolygonHeadCollidingWithLine(stack,20,enemy1.boundingBox[i],enemy1.boundingBox[i<3?(i+1):0]))
               {
                 printf("POLYGON COLLIDED\n");
                 DestroyStack(&stack); stack = nullptr;
                 break;
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

               for(int i=0;i<4;i++)
               {
                 if(pnpoly(enemy1.boundingBox[i], polyPoints, GetStackCount(stack))!= 0)
                 {
                   enemy1.hp-=1;
                   printf("Current hp: %d\n",enemy1.hp);
                   enemy1.captured = true;
                   break;
                 }
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
