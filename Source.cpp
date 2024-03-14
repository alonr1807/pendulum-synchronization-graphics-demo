#include <SDL.h>
#include <cmath>
#include <iostream>
//#include <string>
//#include <sstream>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

const double PIVOT_X = SCREEN_WIDTH / 2;
const double PIVOT_Y = 100;

const SDL_Color colors[12] = { {255, 0, 0, 255},
                                {255, 128, 0, 255},
    {255, 255, 0, 255},
    {128, 255, 0, 255},
    {0, 255, 0, 255},
    {0, 255, 128, 255},
    {0, 255, 255, 255},
    {0, 128, 255, 255},
    {128, 0, 255, 255},
    {255, 0, 255, 255},
    {255, 0, 128, 255},
    {255, 255, 255, 255},
};
//const double plength[12] = { 30.646530854,29.93412616525647,29.217167126722254,28.52566104529929,27.858417208916836,27.21431372853677,26.592292817390245,25.991356448368874,25.41056234523619,24.84902028828334,24.305888701206674,23.780371494913533 };
//const int PENDULUM_LENGTH = 200;
// double as[12] = { 1.11111111111,1.09756097561,1.0843373494,1.07142857143,1.05882352941,1.04651162791,1.03448275862,1.02272727273,1.01123595506,1,0.98901098901,0.97826086956 };
/*
0.30646530854
0.2993412616525647
0.29217167126722254
0.2852566104529929
0.27858417208916836
0.2721431372853677
0.26592292817390245
0.25991356448368874
0.2541056234523619
0.2484902028828334
0.24305888701206674
0.23780371494913533

*/

const int FPS = 165;
const int FRAME_TIME = 1000 / FPS;

int main(int argc, char* argv[])
{
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Pendulum", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set up a timer for the frame rate
    Uint32 frameStart;
    int frameTime;

    // PERIOD & LENGTH CREATION ******
    double periods[12];
    double pendulum_lengths[12];
    for (int i = 0; i < 12; i++) {
        periods[i] = 90.0f /(81.0f + i);
        pendulum_lengths[i] = pow(periods[i] * sqrt(9.81) / (2*M_PI), 2);
        std::cout << "Period of " << i << ": " << periods[i]
            << "    Length of " << i << ": " << pendulum_lengths[i] << std::endl;
    }
    
    // Main loop
    bool running = true;
    while (running)
    {
        // Start the frame timer
        frameStart = SDL_GetTicks();

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // This for loop starts backwards to render the closes object before the one behind it
        for (int i = 11; i >= 0; i--) {
            // Calculate the position of the pendulum
            double angle = std::cos((SDL_GetTicks() / (1000.0f )) * pow(periods[i],-1) *(2*M_PI)) * M_PI / 5;
            double x = PIVOT_X + pendulum_lengths[i]*1000 * std::sin(angle);
            double y = PIVOT_Y + pendulum_lengths[i]*1000 * std::cos(angle);

            // Draw the pendulum
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            SDL_RenderDrawLine(renderer, PIVOT_X, PIVOT_Y, x, y);
            SDL_Rect rect = { x - 10, y - 10, 20, 20 };
            SDL_RenderFillRect(renderer, &rect);
        }

        // Present the renderer
        SDL_RenderPresent(renderer);

        // End the frame timer
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_TIME)
        {
            SDL_Delay(FRAME_TIME - frameTime);
        }
    }

    // Exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
