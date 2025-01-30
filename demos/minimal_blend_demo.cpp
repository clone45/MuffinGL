#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Alpha Blend Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Create target texture (our "layer")
    SDL_Texture* targetTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        400, 400
    );

    // Create a simple colored square texture with alpha
    SDL_Surface* square = SDL_CreateRGBSurfaceWithFormat(
        0, 100, 100, 32, SDL_PIXELFORMAT_RGBA8888
    );
    SDL_FillRect(square, NULL, SDL_MapRGBA(square->format, 255, 0, 0, 128)); // Semi-transparent red

    SDL_Texture* squareTexture = SDL_CreateTextureFromSurface(renderer, square);
    SDL_FreeSurface(square);

    // Enable alpha blending on the texture
    SDL_SetTextureBlendMode(squareTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(targetTexture, SDL_BLENDMODE_BLEND);

    bool running = true;
    bool isMouseDown = false;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                isMouseDown = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                isMouseDown = false;
            }
        }

        if (isMouseDown) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Set target texture as render target
            SDL_SetRenderTarget(renderer, targetTexture);

            // Draw square at mouse position
            SDL_Rect destRect = { 
                mouseX - 50, 
                mouseY - 50, 
                100, 100 
            };
            SDL_RenderCopy(renderer, squareTexture, NULL, &destRect);

            // Reset render target to window
            SDL_SetRenderTarget(renderer, NULL);
        }

        // Clear window
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        // Draw the target texture to the window
        SDL_Rect displayRect = { 200, 100, 400, 400 };
        SDL_RenderCopy(renderer, targetTexture, NULL, &displayRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(squareTexture);
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}