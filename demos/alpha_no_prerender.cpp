#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "SDL initialized" << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image Init failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    std::cout << "SDL_image initialized" << std::endl;

    SDL_Window* window = SDL_CreateWindow(
        "Normal Alpha Blending Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Window created" << std::endl;

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Renderer created" << std::endl;

    // Load images
    SDL_Surface* baseSurface = IMG_Load("base.png");
    if (!baseSurface) {
        std::cerr << "Failed to load base.png: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Base image loaded" << std::endl;

    SDL_Surface* alphaSurface = IMG_Load("alpha.png");
    if (!alphaSurface) {
        std::cerr << "Failed to load alpha.png: " << IMG_GetError() << std::endl;
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Alpha image loaded" << std::endl;

    // Convert to ARGB8888
    SDL_Surface* baseARGB = SDL_ConvertSurfaceFormat(baseSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!baseARGB) {
        std::cerr << "Failed to convert base surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(alphaSurface);
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Base surface converted to ARGB8888" << std::endl;

    SDL_Surface* alphaARGB = SDL_ConvertSurfaceFormat(alphaSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    if (!alphaARGB) {
        std::cerr << "Failed to convert alpha surface: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(baseARGB);
        SDL_FreeSurface(alphaSurface);
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Alpha surface converted to ARGB8888" << std::endl;

    // Create an intermediary (layer) texture
    SDL_Texture* displayTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        baseARGB->w,
        baseARGB->h
    );
    if (!displayTexture) {
        std::cerr << "Failed to create display texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(alphaARGB);
        SDL_FreeSurface(baseARGB);
        SDL_FreeSurface(alphaSurface);
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Display texture created" << std::endl;

    // Make sure the texture can blend
    SDL_SetTextureBlendMode(displayTexture, SDL_BLENDMODE_BLEND);

    // Create standard textures from the surfaces
    SDL_Texture* baseTexture = SDL_CreateTextureFromSurface(renderer, baseARGB);
    if (!baseTexture) {
        std::cerr << "Failed to create base texture: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(displayTexture);
        SDL_FreeSurface(alphaARGB);
        SDL_FreeSurface(baseARGB);
        SDL_FreeSurface(alphaSurface);
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Base texture created" << std::endl;

    SDL_Texture* alphaTexture = SDL_CreateTextureFromSurface(renderer, alphaARGB);
    if (!alphaTexture) {
        std::cerr << "Failed to create alpha texture: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(baseTexture);
        SDL_DestroyTexture(displayTexture);
        SDL_FreeSurface(alphaARGB);
        SDL_FreeSurface(baseARGB);
        SDL_FreeSurface(alphaSurface);
        SDL_FreeSurface(baseSurface);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    std::cout << "Alpha texture created" << std::endl;

    // Use normal blend mode
    SDL_SetTextureBlendMode(alphaTexture, SDL_BLENDMODE_BLEND);

    // Render to our intermediary texture
    SDL_SetRenderTarget(renderer, displayTexture);

    // Start by copying the base
    SDL_RenderCopy(renderer, baseTexture, NULL, NULL);

    // Demonstrate stamping the brush multiple times
    for(int i = 0; i < 4; i++) {
        SDL_RenderCopy(renderer, alphaTexture, NULL, NULL);
    }

    // Return to rendering on screen
    SDL_SetRenderTarget(renderer, NULL);

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, displayTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(alphaTexture);
    SDL_DestroyTexture(baseTexture);
    SDL_DestroyTexture(displayTexture);
    SDL_FreeSurface(alphaARGB);
    SDL_FreeSurface(baseARGB);
    SDL_FreeSurface(alphaSurface);
    SDL_FreeSurface(baseSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
