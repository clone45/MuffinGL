// layer.cpp
#include "muffinGL/layer.hpp"
#include "muffinGL/util.hpp"
#include <stdexcept>
#include <iostream>


Layer::Layer(Graphics& graphics)
    : Raster(graphics)
{
}

Layer Layer::create(Graphics& graphics, int width, int height) {
    Layer layer(graphics);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    layer.m_texture = SDL_CreateTexture(
        graphics.getRenderer(),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width, height
    );

    if (!layer.m_texture) {
        throw std::runtime_error("Failed to create layer: " + std::string(SDL_GetError()));
    }

    // Enable alpha blending by default for layers
    SDL_SetTextureBlendMode(layer.m_texture, SDL_BLENDMODE_BLEND);
    
    layer.m_width = width;
    layer.m_height = height;

    // print out the width and height of the layer
    // std::cout << "Layer created with width: " << layer.m_width << " and height: " << layer.m_height << std::endl;
    
    return layer;
}

// void Layer::flattenTo(Layer& target, BlendMode mode) {
//     
//    // Render this layer onto the target with specified blend mode
//     setBlendMode(mode);
// 
//     // Store current render target
//     SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
//     
//     // Set the target layer as render target
//     if (SDL_SetRenderTarget(m_graphics.getRenderer(), target.getTexture()) != 0) {
//         std::cerr << "Failed to set render target: " << SDL_GetError() << std::endl;
//         std::exit(EXIT_FAILURE);
//     }
//      
//     SDL_Rect destRect = { 0, 0, m_width, m_height };
//     if (SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect) != 0) {
//         std::cerr << "Failed to render texture: " << SDL_GetError() << std::endl;
//         std::exit(EXIT_FAILURE);
//     }   
//     
//     // Restore previous render target
//     SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
//     restoreBlendMode();
// }

void Layer::flattenTo(Layer& target) {
    // First, create a temporary texture for alpha composition
    auto tempLayer = Layer::create(m_graphics, m_width, m_height);
    tempLayer.clear(0, 0, 0, 0);

    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Step 1: Copy alpha values to temp layer using None blend mode
    setBlendMode(BlendMode::None);
    SDL_SetRenderTarget(m_graphics.getRenderer(), tempLayer.getTexture());
    SDL_Rect destRect = { 0, 0, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    // Step 2: Now blend the colors using the pre-computed alpha
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.getTexture());
    setBlendMode(BlendMode::Alpha);  // Or a custom blend mode for just color blending
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    // Restore previous render target and blend mode
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    restoreBlendMode();
}