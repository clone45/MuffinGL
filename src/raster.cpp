#include "muffinGL/raster.hpp"
#include "muffinGL/util.hpp"
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <cmath>

Raster::Raster(Graphics& graphics)
    : m_graphics(graphics)
    , m_texture(nullptr)
    , m_width(0)
    , m_height(0)
{
}

Raster::Raster(Raster&& other) noexcept
    : m_graphics(other.m_graphics)
    , m_texture(other.m_texture)
    , m_width(other.m_width)
    , m_height(other.m_height)
{
    other.m_texture = nullptr;
    other.m_width = 0;
    other.m_height = 0;
}

Raster& Raster::operator=(Raster&& other) noexcept {
    if (this != &other) {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }
        
        m_texture = other.m_texture;
        m_width = other.m_width;
        m_height = other.m_height;
        
        other.m_texture = nullptr;
        other.m_width = 0;
        other.m_height = 0;
    }
    return *this;
}

Raster::~Raster() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

void Raster::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set this texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), m_texture);

    // Enable blend mode
    SDL_SetRenderDrawBlendMode(m_graphics.getRenderer(), SDL_BLENDMODE_BLEND);
    
    // Clear with specified color
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), r, g, b, a);
    SDL_RenderClear(m_graphics.getRenderer());
    
    // Restore previous target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
}

void Raster::render(int x, int y, const Camera* camera, BlendMode mode) {
    setBlendMode(mode);
    
    int screenX = x;
    int screenY = y;
    int width = m_width;
    int height = m_height;
    
    if (camera) {
        camera->worldToScreen(x, y, screenX, screenY);
        width = static_cast<int>(width * camera->getZoom());
        height = static_cast<int>(height * camera->getZoom());
    }
    
    SDL_Rect destRect = { screenX, screenY, width, height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    restoreBlendMode();
}

void Raster::render(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                   int destX, int destY, const Camera* camera, BlendMode mode) {
    setBlendMode(mode);

    int screenX = destX;
    int screenY = destY;
    int width = sourceWidth;
    int height = sourceHeight;

    // output m_width and m_height
    // std::cout << "m_width: " << m_width << std::endl;
    // std::cout << "m_height: " << m_height << std::endl;
    
    if (camera) {
        camera->worldToScreen(destX, destY, screenX, screenY);
        width = static_cast<int>(width * camera->getZoom());
        height = static_cast<int>(height * camera->getZoom());
    }

    // Wrap source coordinates to valid texture positions
    sourceX = sourceX % m_width;
    sourceY = sourceY % m_height;
    
    // Handle negative coordinates by wrapping to positive
    if (sourceX < 0) sourceX += m_width;
    if (sourceY < 0) sourceY += m_height;
    
    // Calculate how much we need to wrap horizontally
    int firstWidth = std::min(sourceWidth, m_width - sourceX);
    int remainingWidth = sourceWidth - firstWidth;
    
    // Calculate how much we need to wrap vertically
    int firstHeight = std::min(sourceHeight, m_height - sourceY);
    int remainingHeight = sourceHeight - firstHeight;
    
    // First segment (top-left)
    SDL_Rect sourceRect = { sourceX, sourceY, firstWidth, firstHeight };
    SDL_Rect destRect = { screenX, screenY, 
                         static_cast<int>(firstWidth * (camera ? camera->getZoom() : 1.0f)), 
                         static_cast<int>(firstHeight * (camera ? camera->getZoom() : 1.0f)) };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    
    // If we need to wrap horizontally, render the top-right segment
    if (remainingWidth > 0) {
        sourceRect = { 0, sourceY, remainingWidth, firstHeight };
        destRect = { screenX + static_cast<int>(firstWidth * (camera ? camera->getZoom() : 1.0f)), 
                    screenY, 
                    static_cast<int>(remainingWidth * (camera ? camera->getZoom() : 1.0f)),
                    static_cast<int>(firstHeight * (camera ? camera->getZoom() : 1.0f)) };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap vertically, render the bottom-left segment
    if (remainingHeight > 0) {
        sourceRect = { sourceX, 0, firstWidth, remainingHeight };
        destRect = { screenX, 
                    screenY + static_cast<int>(firstHeight * (camera ? camera->getZoom() : 1.0f)),
                    static_cast<int>(firstWidth * (camera ? camera->getZoom() : 1.0f)),
                    static_cast<int>(remainingHeight * (camera ? camera->getZoom() : 1.0f)) };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap both horizontally and vertically, render the bottom-right segment
    if (remainingWidth > 0 && remainingHeight > 0) {
        sourceRect = { 0, 0, remainingWidth, remainingHeight };
        destRect = { screenX + static_cast<int>(firstWidth * (camera ? camera->getZoom() : 1.0f)),
                    screenY + static_cast<int>(firstHeight * (camera ? camera->getZoom() : 1.0f)),
                    static_cast<int>(remainingWidth * (camera ? camera->getZoom() : 1.0f)),
                    static_cast<int>(remainingHeight * (camera ? camera->getZoom() : 1.0f)) };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }

    restoreBlendMode();
}

void Raster::render(Raster& target, int destX, int destY, BlendMode mode) {

    setBlendMode(mode);
    
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
       
    // Set the target texture as render target
    if (SDL_SetRenderTarget(m_graphics.getRenderer(), target.getTexture()) != 0) {
        std::cerr << "Failed to set render target: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
  
    
    // if (mode == BlendMode::Alpha) {
    //     SDL_SetRenderDrawBlendMode(m_graphics.getRenderer(), SDL_BLENDMODE_BLEND);
    // }

    // Render the texture
    SDL_Rect destRect = { destX, destY, m_width, m_height };
    if (SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect) != 0) {
        std::cerr << "Failed to render texture: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    restoreBlendMode();
}



void Raster::render(Raster& target, int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                   int destX, int destY, BlendMode mode) {
    setBlendMode(mode);
    
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set the target texture as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.getTexture());
    
    // Wrap source coordinates to valid texture positions
    sourceX = sourceX % m_width;
    sourceY = sourceY % m_height;
    
    // Handle negative coordinates by wrapping to positive
    if (sourceX < 0) sourceX += m_width;
    if (sourceY < 0) sourceY += m_height;
    
    // Calculate how much we need to wrap horizontally
    int firstWidth = std::min(sourceWidth, m_width - sourceX);
    int remainingWidth = sourceWidth - firstWidth;
    
    // Calculate how much we need to wrap vertically
    int firstHeight = std::min(sourceHeight, m_height - sourceY);
    int remainingHeight = sourceHeight - firstHeight;
    
    // First segment (top-left)
    SDL_Rect sourceRect = { sourceX, sourceY, firstWidth, firstHeight };
    SDL_Rect destRect = { destX, destY, firstWidth, firstHeight };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    
    // If we need to wrap horizontally, render the top-right segment
    if (remainingWidth > 0) {
        sourceRect = { 0, sourceY, remainingWidth, firstHeight };
        destRect = { destX + firstWidth, destY, remainingWidth, firstHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap vertically, render the bottom-left segment
    if (remainingHeight > 0) {
        sourceRect = { sourceX, 0, firstWidth, remainingHeight };
        destRect = { destX, destY + firstHeight, firstWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // If we need to wrap both horizontally and vertically, render the bottom-right segment
    if (remainingWidth > 0 && remainingHeight > 0) {
        sourceRect = { 0, 0, remainingWidth, remainingHeight };
        destRect = { destX + firstWidth, destY + firstHeight, remainingWidth, remainingHeight };
        SDL_RenderCopy(m_graphics.getRenderer(), m_texture, &sourceRect, &destRect);
    }
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    
    restoreBlendMode();
}

void Raster::setBlendMode(BlendMode mode) {
    // Debug renderer blend mode support
    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(m_graphics.getRenderer(), &rendererInfo);


    // Store current blend mode
    SDL_GetTextureBlendMode(m_texture, &m_originalBlendMode);
   
    // Convert to SDL blend mode
    SDL_BlendMode sdlMode;
    switch (mode) {
        case BlendMode::None:
            sdlMode = SDL_BLENDMODE_NONE;
            break;
        case BlendMode::Alpha:
            sdlMode = SDL_BLENDMODE_BLEND;
            break;
        case BlendMode::Additive:
            sdlMode = SDL_BLENDMODE_ADD;
            break;
        case BlendMode::Multiply:
            sdlMode = SDL_BLENDMODE_MOD;
            break;
        case BlendMode::AlphaPreserve:
            {
                // Do not change this blend mode.  It is working as expected.
                SDL_BlendMode customMode = SDL_ComposeCustomBlendMode(
                    SDL_BLENDFACTOR_ONE,                    // source color factor
                    SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination color factor
                    SDL_BLENDOPERATION_ADD,                 // color operation
                    SDL_BLENDFACTOR_ONE,                    // source alpha factor
                    SDL_BLENDFACTOR_ZERO,                   // destination alpha factor - THIS WAS THE WORKING VERSION
                    SDL_BLENDOPERATION_ADD                  // alpha operation
                );
                // std::cout << "Created AlphaPreserve blend mode with value: " << static_cast<int>(customMode) << std::endl;
                sdlMode = customMode;
            }
            break;
        case BlendMode::LayerComposite:
            {
                SDL_BlendMode customMode = SDL_ComposeCustomBlendMode(
    SDL_BLENDFACTOR_SRC_ALPHA,              // source color factor
    SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination color factor
    SDL_BLENDOPERATION_ADD,                 // color operation
    SDL_BLENDFACTOR_ONE,                    // source alpha factor
    SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination alpha factor - CHANGED
    SDL_BLENDOPERATION_ADD                  // alpha operation
);
                // std::cout << "Created LayerComposite blend mode with value: " << static_cast<int>(customMode) << std::endl;
                sdlMode = customMode;
            }
            break;
    }
   
    // std::cout << "Attempting to set blend mode " << static_cast<int>(mode) 
    //          << " which translated to SDL blend mode: " << static_cast<int>(sdlMode) << std::endl;
    
    if (SDL_SetTextureBlendMode(m_texture, sdlMode) != 0) {
        std::cerr << "Failed to set blend mode: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Verify the blend mode was set correctly
    SDL_BlendMode currentMode;
    SDL_GetTextureBlendMode(m_texture, &currentMode);
    // std::cout << "setBlendMode requested: " << static_cast<int>(mode) 
    //          << ", actual mode set: " << blendModeToString(currentMode)
    //          << " (value: " << static_cast<int>(currentMode) << ")" << std::endl;
}

void Raster::restoreBlendMode() {
    SDL_SetTextureBlendMode(m_texture, m_originalBlendMode);
}

bool Raster::save(const std::string& path) const {
    // Create a surface to hold our texture data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, m_width, m_height, 32, SDL_PIXELFORMAT_RGBA32
    );
    
    if (!surface) {
        return false;
    }
    
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set up a temporary texture for reading
    SDL_Texture* readTexture = SDL_CreateTexture(
        m_graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        m_width, m_height
    );
    
    if (!readTexture) {
        SDL_FreeSurface(surface);
        return false;
    }
    
    // Clear the intermediate texture to fully transparent
    SDL_SetRenderTarget(m_graphics.getRenderer(), readTexture);
    SDL_SetRenderDrawColor(m_graphics.getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(m_graphics.getRenderer());
    
    // Disable blending on the source texture
    SDL_BlendMode previousBlendMode;
    SDL_GetTextureBlendMode(m_texture, &previousBlendMode);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);
    
    // Copy the source texture to the intermediate texture
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, nullptr);
    
    // Restore the source texture's blend mode
    SDL_SetTextureBlendMode(m_texture, previousBlendMode);
    
    // Read the pixels into the surface
    SDL_RenderReadPixels(
        m_graphics.getRenderer(),
        nullptr,
        SDL_PIXELFORMAT_RGBA32,
        surface->pixels,
        surface->pitch
    );
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    SDL_DestroyTexture(readTexture);
    
    // Save the surface as a PNG file
    bool success = (IMG_SavePNG(surface, path.c_str()) == 0);
    SDL_FreeSurface(surface);
    
    return success;
}

void Raster::debug() {
    Uint32 format;
    int access;
    int width, height;
    SDL_QueryTexture(m_texture, &format, &access, &width, &height);
    // std::cout << "Texture dimensions: " << width << "x" << height << std::endl;
    // std::cout << "Texture format: " << SDL_GetPixelFormatName(format) << std::endl;
    // std::cout << "Texture access: " << (access == SDL_TEXTUREACCESS_STATIC ? "STATIC" :
    //                                    access == SDL_TEXTUREACCESS_STREAMING ? "STREAMING" :
    //                                    access == SDL_TEXTUREACCESS_TARGET ? "TARGET" :
    //                                    "UNKNOWN") << std::endl;
                                        
    // debugTextureSamples(m_graphics.getRenderer(), m_texture);
}


void Raster::debugTextureSamples(SDL_Renderer* renderer, SDL_Texture* texture) {
    // Get texture dimensions
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    // Create a surface for reading pixels
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        std::cerr << "Failed to create debug surface: " << SDL_GetError() << std::endl;
        return;
    }

    // Set the texture as the render target temporarily
    SDL_Texture* previousTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);

    // Read the texture pixels into the surface
    SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);

    // Restore the original render target
    SDL_SetRenderTarget(renderer, previousTarget);

    // Middle column and row
    int middleX = width / 2;
    int middleY = height / 2;

    // Output middle column
    // std::cout << "Middle Column (x = " << middleX << "):" << std::endl;
    for (int y = 0; y < height; ++y) {
        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        Uint32 pixel = pixels[y * (surface->pitch / 4) + middleX];

        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
        // std::cout << "[" << y << "] RGBA(" << static_cast<int>(r) << ", " 
        //          << static_cast<int>(g) << ", " << static_cast<int>(b) << ", " 
        //          << static_cast<int>(a) << ") ";
    }
    // std::cout << std::endl;

    // Output middle row
    // std::cout << "Middle Row (y = " << middleY << "):" << std::endl;
    Uint32* rowPixels = static_cast<Uint32*>(surface->pixels) + (middleY * (surface->pitch / 4));
    for (int x = 0; x < width; ++x) {
        Uint32 pixel = rowPixels[x];

        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
        // std::cout << "[" << x << "] RGBA(" << static_cast<int>(r) << ", " 
        //          << static_cast<int>(g) << ", " << static_cast<int>(b) << ", " 
        //          << static_cast<int>(a) << ") ";
    }
    // std::cout << std::endl;

    // Free the surface
    SDL_FreeSurface(surface);
}

