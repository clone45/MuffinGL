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
        "Premultiplied Alpha Test",
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

    // If your alpha.png is unpremultiplied, do this once:
    std::cout << "Attempting premultiply..." << std::endl;
    if (SDL_PremultiplyAlpha(
            alphaARGB->w, alphaARGB->h,
            SDL_PIXELFORMAT_ARGB8888, alphaARGB->pixels, alphaARGB->pitch,
            SDL_PIXELFORMAT_ARGB8888, alphaARGB->pixels, alphaARGB->pitch
        ) < 0)
    {
        std::cerr << "Premultiply failed: " << SDL_GetError() << std::endl;
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
    std::cout << "Premultiply completed" << std::endl;

    // Create the final display texture
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
    SDL_SetTextureBlendMode(displayTexture, SDL_BLENDMODE_BLEND);

    // Make textures from surfaces
    SDL_Texture* baseTexture = SDL_CreateTextureFromSurface(renderer, baseARGB);
    SDL_Texture* alphaTexture = SDL_CreateTextureFromSurface(renderer, alphaARGB);

    if (!baseTexture || !alphaTexture) {
        std::cerr << "Failed to create base or alpha texture: " << SDL_GetError() << std::endl;
        if (alphaTexture) SDL_DestroyTexture(alphaTexture);
        if (baseTexture) SDL_DestroyTexture(baseTexture);
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

    // IMPORTANT: set up a premultiplied “source over” blend:
    SDL_BlendMode pmBlend = SDL_ComposeCustomBlendMode(
        // RGB:
        SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD,
        // Alpha:
        SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD
    );
    SDL_SetTextureBlendMode(alphaTexture, pmBlend);

    // Flatten them into displayTexture
    SDL_SetRenderTarget(renderer, displayTexture);
    SDL_RenderCopy(renderer, baseTexture, NULL, NULL);

    // Demonstrate multiple passes (like stamping the brush repeatedly)
    for(int i = 0; i < 2; i++) {
        SDL_RenderCopy(renderer, alphaTexture, NULL, NULL);
    }

    // Return to screen
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
