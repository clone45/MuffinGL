// layer.cpp
#include "layer.hpp"
#include "util.hpp"
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
    std::cout << "Layer created with width: " << layer.m_width << " and height: " << layer.m_height << std::endl;
    
    return layer;
}

void Layer::flattenTo(Layer& target, BlendMode mode) {
    std::cout << "== Layer::flattenTo Debug ==" << std::endl;
    
    // Debug blend modes before changes
    SDL_BlendMode srcBlend, targetBlend;
    SDL_GetTextureBlendMode(m_texture, &srcBlend);
    SDL_GetTextureBlendMode(target.getTexture(), &targetBlend);
    std::cout << "Initial blend modes - Source: " << blendModeToString(srcBlend)
              << ", Target: " << blendModeToString(targetBlend) << std::endl;
    
    // Store current render target
    SDL_Texture* previousTarget = SDL_GetRenderTarget(m_graphics.getRenderer());
    
    // Set the target layer as render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), target.getTexture());
    
    // Render this layer onto the target with specified blend mode
    setBlendMode(mode);
    SDL_GetTextureBlendMode(m_texture, &srcBlend);
    std::cout << "After setBlendMode - Source: " << blendModeToString(srcBlend) << std::endl;
    
    SDL_Rect destRect = { 0, 0, m_width, m_height };
    SDL_RenderCopy(m_graphics.getRenderer(), m_texture, nullptr, &destRect);
    
    // Debug blend modes after render
    SDL_GetTextureBlendMode(m_texture, &srcBlend);
    SDL_GetTextureBlendMode(target.getTexture(), &targetBlend);
    std::cout << "Post-render blend modes - Source: " << blendModeToString(srcBlend)
              << ", Target: " << blendModeToString(targetBlend) << std::endl;
    
    restoreBlendMode();
    
    // Restore previous render target
    SDL_SetRenderTarget(m_graphics.getRenderer(), previousTarget);
    
    std::cout << "== End Layer::flattenTo Debug ==" << std::endl;
}