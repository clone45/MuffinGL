#pragma once
#include "raster.hpp"

class Texture; 

class Layer : public Raster {
public:
    // Factory method for creation
    static Layer create(Graphics& graphics, int width, int height);
    
    // Layer-specific operations
    // void flattenTo(Layer& target, BlendMode mode = BlendMode::Alpha);
    void flattenTo(Layer& target);
    // Move semantics
    Layer(Layer&& other) noexcept = default;
    Layer& operator=(Layer&& other) noexcept = default;

    // Expose render-to methods from base class as public
    using Raster::render;  // Inherit all render overloads
    using Raster::clear;

protected:
    // Protected constructor - use factory method instead
    explicit Layer(Graphics& graphics);
};