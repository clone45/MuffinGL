#pragma once
#include <string>
#include <cstdint>
#include <SDL2/SDL.h>
#include "camera.hpp"
#include "graphics.hpp"

// Forward declarations
class Layer;
class Texture;

enum class BlendMode {
    None,           // No blending
    Alpha,          // Regular alpha blending
    Additive,       // Colors are added together
    Multiply,       // Colors are multiplied together
    AlphaPreserve,  // Custom blend mode for preserving alpha
    LayerComposite  // Custom blend mode for layer composition
};

enum class ScaleMode {
    Nearest,
    Linear
};

class Raster {
protected:
    Graphics& m_graphics;
    SDL_Texture* m_texture;
    int m_width;
    int m_height;
    SDL_BlendMode m_originalBlendMode;
    
    // Protected constructor - only derived classes can instantiate
    explicit Raster(Graphics& graphics);
    
    // Helper methods for blend modes
    void setBlendMode(BlendMode mode);
    void restoreBlendMode();

public:
    // Move semantics
    Raster(const Raster&) = delete;
    Raster& operator=(const Raster&) = delete;
    Raster(Raster&& other) noexcept;
    Raster& operator=(Raster&& other) noexcept;
    virtual ~Raster();

    // Basic operations all rasters support
    virtual void clear(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0);
    
    // Core rendering operations
    virtual void render(int x, int y, const Camera* camera = nullptr, BlendMode mode = BlendMode::Alpha);
    virtual void render(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                       int destX, int destY, const Camera* camera = nullptr, BlendMode mode = BlendMode::Alpha);
    
    // Render to another raster
    virtual void render(Raster& target, int destX, int destY, BlendMode mode = BlendMode::Alpha);
    virtual void render(Raster& target, int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                       int destX, int destY, BlendMode mode = BlendMode::Alpha);
    
    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    SDL_Texture* getTexture() const { return m_texture; }
    
    // Virtual interface for saving
    virtual bool save(const std::string& path) const;

    // Debug
    void debug();
    
    void debugTextureSamples(SDL_Renderer* renderer, SDL_Texture* texture);

    
};