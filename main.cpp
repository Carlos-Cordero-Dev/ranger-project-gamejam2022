#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "FIFO.cpp"
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_mixer.h>
// #include <SDL2/SDL_ttf.h>

const int WIDTH = 1040, HEIGHT = 585;

// void draw_rects(SDL_Renderer *renderer, int x, int y)
// {
//     // R
//     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//     SDL_Rect r = {x, y, 64, 64};
//     SDL_RenderFillRect(renderer, &r);
//
//     // G
//     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
//     SDL_Rect g = {x + 64, y, 64, 64};
//     SDL_RenderFillRect(renderer, &g);
//
//     // B
//     SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
//     SDL_Rect b = {x + 128, y, 64, 64};
//     SDL_RenderFillRect(renderer, &b);
// }

void DrawCurrentPolygon(Coord *stack,SDL_Renderer *renderer)
{
  if(stack!=nullptr && stack->nextCoord!=nullptr)
  {
    Coord *aux;
    for(aux=stack;aux->nextCoord!=nullptr;aux=aux->nextCoord)
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderDrawLineF(renderer, aux->x,aux->y,aux->nextCoord->x,aux->nextCoord->y);
    }
  }
}

bool IsPolygonClosed(Coord *stack, int x,int y)
{
  if(stack!=nullptr && stack->nextCoord!=nullptr)
  {
    if(GetStackCount(stack) > 10)
    {
      int margin = 20;
      Coord *aux = BotStack(stack,0);
      if(abs(aux->x - x) <= margin && abs (aux->y - y) <= margin ) return true;
      else return false;
    }
    else return false;

  }
  else return false;
}

void DrawCircle(int radius,int x, int y)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

  

  SDL_RenderDrawLineF(renderer, aux->x,aux->y,aux->nextCoord->x,aux->nextCoord->y);
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

             if(IsPolygonClosed(stack,event.motion.x,event.motion.y))
             {
               //+1 a las vueltas
               printf("ONE SCORE UP\n");
               DestroyStack(&stack);
             }

        }
      } //end of event handling

      //every frame
      currentTime = SDL_GetTicks();
      if (currentTime > lastTime + 1000/100) {

        Coord *lastCoord = ExtractFIFO(&stack);
        // if(lastCoord != NULL) printf("Se ha extraido %d %d\n",lastCoord->x,lastCoord->y);
        free(lastCoord); lastCoord = nullptr;
        lastTime = currentTime;
      }

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
      SDL_RenderClear(renderer);

      DrawCurrentPolygon(stack,renderer);
      SDL_RenderPresent(renderer);

      //Update the surface (SOLO HAY QUE HACERLO CUANDO VAYAN A HABER CAMBIOS VISUALES)
      //SDL_UpdateWindowSurface( window );
    }

    DestroyStack(&stack);
    SDL_FreeSurface( windowSurface ); windowSurface = NULL;
    SDL_DestroyRenderer(renderer); renderer = NULL;
    SDL_DestroyWindow( window ); window = NULL;
    SDL_Quit( );

    return EXIT_SUCCESS;
}
