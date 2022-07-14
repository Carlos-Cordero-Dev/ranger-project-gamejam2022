#include <SDL2/SDL.h>
#include <stdlib.h> //atoi
#include <math.h> //pow

void EmptyString(char string[3])
{
  for(int i=2;i>=0;i--) string[i]='\0';
}

void LoadLevelFromFile(SpriteSheet *spriteSheet,const char *filepath)
{
  //source: https://lazyfoo.net/tutorials/SDL/33_file_reading_and_writing/index.php

  SDL_RWops *file = SDL_RWFromFile(filepath,"r");
  if( file != NULL)
  {
    SDL_ClearError();

    char digitsString[3];
    char tmpchar;
    int numberOfDigits = 1,numberOfSprites = 0;
    bool DimensionsStored = false;
    if(!SDL_RWread(file,&tmpchar,sizeof(char),1)) printf("end of file prematurely (empty file probably)\n");
    else
    {
      digitsString[numberOfDigits-1] = tmpchar;

      while(SDL_RWread(file,&tmpchar,sizeof(char),1))
      {
        if(tmpchar != ' ' && tmpchar != '\n')
        {
          numberOfDigits++;
          digitsString[numberOfDigits-1] = tmpchar;
        }
        else
        {
          numberOfSprites++;
          if(!DimensionsStored)
          {
            switch(numberOfSprites)
            {
              case 1: spriteSheet->singleSpriteW = atoi(digitsString);
                break;
              case 2: spriteSheet->singleSpriteH = atoi(digitsString);
                break;
              case 3: spriteSheet->spritesPerRow= atoi(digitsString);
                break;
              case 4:
              {
                spriteSheet->spritesPerColumn = atoi(digitsString);
                numberOfSprites = 0;
                DimensionsStored = true;
              }
                break;
            }
          }
          else
          {
            spriteSheet->sprites = (Sprite*) realloc (spriteSheet->sprites,sizeof(Sprite)*numberOfSprites);
            spriteSheet->sprites[numberOfSprites-1].id = atoi(digitsString);
            printf("%d ",spriteSheet->sprites[numberOfSprites-1].id);
          }
          EmptyString(digitsString);
          numberOfDigits = 0;
        }
      }
      numberOfSprites++;
      spriteSheet->sprites = (Sprite*) realloc (spriteSheet->sprites,sizeof(Sprite)*numberOfSprites);
      spriteSheet->sprites[numberOfSprites-1].id = atoi(digitsString);
      printf("%d ",spriteSheet->sprites[numberOfSprites-1].id);
      
      printf("end of file normally\n");
    }
  }
  else
  {
    printf( "Error: Unable to save file! %s\n", SDL_GetError() );
  }
}
