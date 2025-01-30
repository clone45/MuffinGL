#pragma once
#include "raster.hpp"
#include <string>
#include <cstdint>

struct Color {
    uint8_t r, g, b, a;
};

class Texture : public Raster {
public:
    // Factory methods for creation
    static Texture create(Graphics& graphics, const std::string& path, bool makeTarget = false);
    static Texture create(Graphics& graphics, int width, int height);

    // Destructor to handle surface cleanup
    ~Texture() override;

    // Move semantics (using default since base class handles texture)
    Texture(Texture&& other) noexcept : Raster(std::move(other)), m_surface(nullptr) {}
    Texture& operator=(Texture&& other) noexcept;

    // Texture-specific operations
    void resize(int width, int height, ScaleMode mode = ScaleMode::Linear);
    Color getPixel(int x, int y) const;
    void applyMask(Texture& mask);

protected:
    explicit Texture(Graphics& graphics);
    SDL_Surface* m_surface;  // Kept for pixel operations
};