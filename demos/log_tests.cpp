#include "muffinGL/graphics.hpp"
#include "muffinGL/texture.hpp"
#include "muffinGL/layer.hpp"
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
        const int SMALL_SIZE = 100;  // For testing resize
        
        // Load and resize all textures to same dimensions
        auto grass = Texture::create(graphics, "resources/grass_1024x1024.png");
        auto dirt = Texture::create(graphics, "resources/dirt_1024x1024.png");
        auto mask = Texture::create(graphics, "resources/mask_white_255x255.png");
        
        grass.resize(BRUSH_SIZE, BRUSH_SIZE);
        // dirt.resize(BRUSH_SIZE, BRUSH_SIZE);
        mask.resize(BRUSH_SIZE, BRUSH_SIZE);
        
        // Test 1: Save base textures
        grass.save("logs/01_grass_base.png");
        dirt.save("logs/02_dirt_base.png");
        mask.save("logs/03_mask_base.png");
        
        // Test 3: Basic masking
        auto brushTexture = Texture::create(graphics, BRUSH_SIZE, BRUSH_SIZE);

        // grass.render(brushTexture, 0, 0, BlendMode::None);

        grass.render(
            brushTexture,
            0,          // Texture source position x
            0,          // Texture source position y
            BRUSH_SIZE, // Texture source width
            BRUSH_SIZE, // Texture source height
            0,          // Local space x
            0,          // Local space y
            BlendMode::None  // No blending for initial composition
        );        

        brushTexture.applyMask(mask);
        brushTexture.save("logs/06_grass_masked.png");
        
        // Test 4: Layer composition
        auto baseLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);
        auto topLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);
        auto finalLayer = Layer::create(graphics, LAYER_SIZE, LAYER_SIZE);
        
        baseLayer.clear(0, 0, 0, 0);
        topLayer.clear(0, 0, 0, 0);
        finalLayer.clear(0, 0, 0, 0);

        // Draw dirt background
        dirt.render(baseLayer, 0, 0);
        
        brushTexture.save("logs/06b_composite_pre_render.png");

        // Draw masked grass in center
        brushTexture.render(topLayer, LAYER_SIZE/2, LAYER_SIZE/2, BlendMode::AlphaPreserve);

        // Save toplayer for debugging
        std::cout << "Saving topLayer to logs/07_top_layer.png" << std::endl;
        topLayer.save("logs/07_top_layer.png");
        topLayer.debug();
        
        // Compose layers
        baseLayer.flattenTo(finalLayer);
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