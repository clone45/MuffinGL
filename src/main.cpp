#include "graphics.hpp"
#include "texture.hpp"
#include "layer.hpp"
#include <iostream>
#include <filesystem>

void ensureDirectoryExists(const std::string& path) {
    std::filesystem::create_directories(path);
}

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(800, 600, "Layer System Tests");
        ensureDirectoryExists("logs");
        
        // Constants for testing
        const int LAYER_SIZE = 1024;
        const int BRUSH_SIZE = 200;
        
        // Load and resize all textures to same dimensions
        auto grass = Texture::create(graphics, "resources/grass_1024x1024.png");
        auto dirt = Texture::create(graphics, "resources/dirt_1024x1024.png");
        auto mask = Texture::create(graphics, "resources/mask_white_255x255.png");
        
        grass.resize(BRUSH_SIZE, BRUSH_SIZE);
        mask.resize(BRUSH_SIZE, BRUSH_SIZE);
        
        auto brushTexture = Texture::create(graphics, BRUSH_SIZE, BRUSH_SIZE);

        // grass.render(brushTexture, 0, 0, BlendMode::None);
        grass.render(brushTexture, 0, 0, BlendMode::AlphaPreserve);

        brushTexture.applyMask(mask);
        
        // Test 4: Layer composition
        auto baseLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);
        auto topLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);
        auto finalLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);

        // Draw dirt background
        dirt.render(baseLayer, 0, 0);

        // Draw masked grass in center
        brushTexture.render(topLayer, LAYER_SIZE/2, LAYER_SIZE/2, BlendMode::AlphaPreserve);

        // Compose layers
        // baseLayer.flattenTo(finalLayer);
        baseLayer.flattenTo(finalLayer, BlendMode::LayerComposite);
        topLayer.flattenTo(finalLayer, BlendMode::LayerComposite);
        
        // Save final result
        finalLayer.save("logs/08_final_composition.png");
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}