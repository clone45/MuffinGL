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
    
    // Create window and renderer

    SDL_Window* window = SDL_CreateWindow(
        "Layer Blend Issue Demo",
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

    // Load images
    SDL_Surface* baseSurface = IMG_Load("base.png");
    SDL_Surface* alphaSurface = IMG_Load("alpha.png");
    if (!baseSurface || !alphaSurface) {
        std::cerr << "Failed to load images: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Convert to ARGB8888
    SDL_Surface* baseARGB = SDL_ConvertSurfaceFormat(baseSurface, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_Surface* alphaARGB = SDL_ConvertSurfaceFormat(alphaSurface, SDL_PIXELFORMAT_ARGB8888, 0);

    // Create our three layers
    SDL_Texture* baseLayer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        baseARGB->w, baseARGB->h
    );

    SDL_Texture* paintLayer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        baseARGB->w, baseARGB->h
    );

    SDL_Texture* displayLayer = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        baseARGB->w, baseARGB->h
    );

    // Create our source textures
    SDL_Texture* baseTexture = SDL_CreateTextureFromSurface(renderer, baseARGB);
    SDL_Texture* alphaTexture = SDL_CreateTextureFromSurface(renderer, alphaARGB);

    // Set blend modes
    SDL_SetTextureBlendMode(baseLayer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(paintLayer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(displayLayer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(alphaTexture, SDL_BLENDMODE_BLEND);

    // Setup base layer
    SDL_SetRenderTarget(renderer, baseLayer);
    SDL_RenderCopy(renderer, baseTexture, NULL, NULL);

    // Paint alpha texture to paint layer
    SDL_SetRenderTarget(renderer, paintLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, alphaTexture, NULL, NULL);

    // Composite to display layer
    SDL_SetRenderTarget(renderer, displayLayer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    // Copy base layer
    SDL_RenderCopy(renderer, baseLayer, NULL, NULL);
    
    // Composite paint layer multiple times
    for(int i = 0; i < 100; i++) {
        SDL_RenderCopy(renderer, paintLayer, NULL, NULL);
    }

    // Reset render target
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
        SDL_RenderCopy(renderer, displayLayer, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(alphaTexture);
    SDL_DestroyTexture(baseTexture);
    SDL_DestroyTexture(baseLayer);
    SDL_DestroyTexture(paintLayer);
    SDL_DestroyTexture(displayLayer);
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