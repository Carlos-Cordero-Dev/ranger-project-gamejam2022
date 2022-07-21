
//SDL variables
SDL_Surface *windowSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Window *window = NULL;
SDL_Event event;

//event variables
bool GameIsRunning = true;
int lastMousePosX , lastMousePosY;
unsigned int lastTime = 0, currentTime;
bool mouseRightButtonPressed = false;

//polygon variables
Coord* stack = nullptr;

//enemy variables
enemy enemy1; //has atack_animation and idle_animation;

//sprite variables
SpriteSheet tileset_grass;

//healthbar variables
Healthbar playerHealthbar,enemyHealthbar;
