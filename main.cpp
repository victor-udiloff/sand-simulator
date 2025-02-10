#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

#define nRows 300
#define nCollumns 300

void handleInput(SDL_Event event, bool &isRunning, unsigned int worldMatrix[nRows][nCollumns], bool &isPressed, bool &bigBrush)
{

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_d)
        {
            for (int i = 0; i < nRows; i++)
            {
                for (int j = 0; j < nCollumns; j++)
                {
                    worldMatrix[i][j] = 0;
                }
            }
        }
        if (event.key.keysym.sym == SDLK_b && bigBrush == false)
        {
            bigBrush = true;
            break;
        }
        if (event.key.keysym.sym == SDLK_b && bigBrush == true)
        {
            bigBrush = false;
            break;
        }

        break;
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            isPressed = true;
            worldMatrix[event.button.y][event.button.x] = 1;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            isPressed = false;
        }
        break;
    case SDL_MOUSEMOTION:
        if (isPressed == true && bigBrush == false)
        {
            worldMatrix[event.motion.y][event.motion.x] = 1;
        }
        if (isPressed == true && bigBrush == true)
        {
            worldMatrix[event.motion.y - 1][event.motion.x - 1] = 1;
            worldMatrix[event.motion.y - 1][event.motion.x] = 1;
            worldMatrix[event.motion.y - 1][event.motion.x + 1] = 1;
            worldMatrix[event.motion.y][event.motion.x - 1] = 1;
            worldMatrix[event.motion.y][event.motion.x] = 1;
            worldMatrix[event.motion.y][event.motion.x + 1] = 1;
            worldMatrix[event.motion.y + 1][event.motion.x - 1] = 1;
            worldMatrix[event.motion.y + 1][event.motion.x] = 1;
            worldMatrix[event.motion.y + 1][event.motion.x + 1] = 1;
        }
        break;
    }
}

void draw(unsigned int worldMatrix[nRows][nCollumns], SDL_Rect *rect1, SDL_Surface *winSurface)
{
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCollumns; j++)
        {
            if (worldMatrix[i][j] == 1)
            {
                rect1->y = i;
                rect1->x = j;
                SDL_FillRect(winSurface, rect1, SDL_MapRGB(winSurface->format, 230, 187, 131));
            }
        }
    }
}

void sandPhysics(unsigned int worldMatrix[nRows][nCollumns], unsigned int worldMatrix2[nRows][nCollumns])
{

    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCollumns; j++)
        {
            worldMatrix2[i][j] = worldMatrix[i][j];
        }
    }

    for (int i = 0; i < nRows - 1; i++)
    {
        for (int j = 1; j < nCollumns - 1; j++)
        {
            if (worldMatrix[i][j] == 1 && worldMatrix[i + 1][j] == 0)
            {
                worldMatrix2[i][j] = 0;
                worldMatrix2[i + 1][j] = 1;
            }
            if (worldMatrix[i][j] == 1 && worldMatrix[i + 1][j] == 1 && worldMatrix[i + 1][j - 1] == 0)
            {
                worldMatrix2[i][j] = 0;
                worldMatrix2[i + 1][j - 1] = 1;
            }
            if (worldMatrix[i][j] == 1 && worldMatrix[i + 1][j] == 1 && worldMatrix[i + 1][j + 1] == 0)
            {
                worldMatrix2[i][j] = 0;
                worldMatrix2[i + 1][j + 1] = 1;
            }
        }
    }

    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCollumns; j++)
        {
            worldMatrix[i][j] = worldMatrix2[i][j];
        }
    }
}

int main(int argv, char **args)
{

    // initialize with 0
    unsigned int worldMatrix[nRows][nCollumns];
    unsigned int worldMatrix2[nRows][nCollumns];
    bool isPressed = false;
    bool bigBrush = false;
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCollumns; j++)
        {
            worldMatrix[i][j] = 0;
            worldMatrix2[i][j] = 0;
        }
    }

    // test
    worldMatrix[30][70] = 1;

    // SDL initialization
    SDL_Init(SDL_INIT_EVERYTHING);

    Uint32 currentTime;
    Uint32 lastTime;
    Uint32 deltaTime;

    SDL_Window *window = SDL_CreateWindow("Sand Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nRows, nCollumns, 0);

    bool isRunning = true;
    SDL_Event event;

    SDL_Surface *winSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 8, 23, 43));
    SDL_UpdateWindowSurface(window);
    SDL_Rect *rect1 = new SDL_Rect();
    rect1->x = 0;
    rect1->y = 0;
    rect1->w = 1;
    rect1->h = 1;

    while (isRunning)
    {

        lastTime = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            handleInput(event, isRunning, worldMatrix, isPressed, bigBrush);
        }

        SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 8, 23, 43));
        sandPhysics(worldMatrix, worldMatrix2);
        draw(worldMatrix, rect1, winSurface);
        SDL_UpdateWindowSurface(window);

        // Avoid negative delays
        currentTime = SDL_GetTicks();
        if (currentTime - lastTime > 17)
        {
            deltaTime = 0;
        }
        else
        {
            deltaTime = currentTime - lastTime;
        }

        // Delay function to have max 60 fps
        SDL_Delay(17 - deltaTime);
    }

    SDL_DestroyWindow(window);
    window = NULL;
    winSurface = NULL;
    SDL_Quit();

    return 0;
}