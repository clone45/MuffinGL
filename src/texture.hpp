// texture.hpp
#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "camera.hpp"

class Graphics;

enum class BlendMode {
    None,       // No blending
    Alpha,      // Regular alpha blending
    Additive,   // Colors are added together
    Multiply    // Colors are multiplied together
};

// In texture.hpp:
struct Color {
    uint8_t r, g, b, a;
};

class Texture {
public:
    ~Texture();

    enum class ScaleMode {
        Nearest,   // Fast but pixelated
        Linear,    // Smooth blending
        Best      // Best quality but slower
    };

    // Prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow moving
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Creation
    static Texture create(Graphics& graphics, int width, int height);
    static Texture create(Graphics& graphics, const std::string& path);

    // Saving
    bool save(const std::string& path) const;

    // Clearing
    void clear(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0);

    // Drawing
    void draw(int x, int y);
    void draw(Texture& target, int x, int y);
    
    // Manipulation
    void resize(int width, int height, ScaleMode mode = ScaleMode::Linear);
    void applyMask(Texture& mask);
    
    // Pixel operations
    Color getPixel(int x, int y) const;

    // Properties
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    void render(int x, int y, const Camera* camera = nullptr, BlendMode mode = BlendMode::Alpha);
    
    void render(Texture& target, int destX, int destY, 
                BlendMode mode = BlendMode::Alpha);
    
    void render(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                int destX, int destY, const Camera* camera = nullptr, BlendMode mode = BlendMode::Alpha);
    
    void render(Texture& target,
                int sourceX, int sourceY, int sourceWidth, int sourceHeight,
                int destX, int destY, BlendMode mode = BlendMode::Alpha);


private:
    // Private constructor - use create() instead
    Texture(Graphics& graphics);
    
    Graphics& m_graphics;
    SDL_Texture* m_texture;
    SDL_Surface* m_surface = nullptr;
    SDL_BlendMode m_previousBlendMode;
    int m_width = 0;
    int m_height = 0;

    void setBlendMode(BlendMode mode) {
        // Store current blend mode before changing it
        SDL_GetTextureBlendMode(m_texture, &m_previousBlendMode);
        SDL_SetTextureBlendMode(m_texture, toSDLBlendMode(mode));
    }

    void restoreBlendMode() {
        SDL_SetTextureBlendMode(m_texture, m_previousBlendMode);
    }

    static SDL_BlendMode toSDLBlendMode(BlendMode mode) {
        switch (mode) {
            case BlendMode::None:
                return SDL_BLENDMODE_NONE;
            case BlendMode::Alpha:
                return SDL_BLENDMODE_BLEND;
            case BlendMode::Additive:
                return SDL_BLENDMODE_ADD;
            case BlendMode::Multiply:
                return SDL_BLENDMODE_MOD;
            default:
                return SDL_BLENDMODE_BLEND;
        }
    }

};