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
        const int TEST_SIZE = 200;
        const int SMALL_SIZE = 100;  // For testing resize
        
        // Load and resize all textures to same dimensions
        auto grass = Texture::create(graphics, "resources/grass_1024x1024.png");
        auto dirt = Texture::create(graphics, "resources/dirt_1024x1024.png");
        auto mask = Texture::create(graphics, "resources/mask_white_255x255.png");
        
        grass.resize(TEST_SIZE, TEST_SIZE);
        dirt.resize(TEST_SIZE, TEST_SIZE);
        mask.resize(TEST_SIZE, TEST_SIZE);
        
        // Test 1: Save base textures
        grass.save("logs/01_grass_base.png");
        dirt.save("logs/02_dirt_base.png");
        mask.save("logs/03_mask_base.png");

        // Test 2: Mask resize test
        auto resizedMask = Texture::create(graphics, "resources/mask_white_255x255.png");
        resizedMask.resize(SMALL_SIZE, SMALL_SIZE);
        resizedMask.save("logs/04_mask_resized.png");
        
        // Test 3: Basic masking
        auto compositeTexture = Texture::create(graphics, TEST_SIZE, TEST_SIZE);
        compositeTexture.clear(0, 0, 0, 0);
        compositeTexture.save("logs/05_composit_texture_cleared.png");

        // grass.render(compositeTexture, 0, 0, BlendMode::None);

        grass.render(
            compositeTexture,
            0,          // Texture source position x
            0,          // Texture source position y
            TEST_SIZE, // Texture source width
            TEST_SIZE, // Texture source height
            0,          // Local space x
            0,          // Local space y
            BlendMode::None  // No blending for initial composition
        );        

        compositeTexture.applyMask(mask);
        compositeTexture.save("logs/06_grass_masked.png");
        
        // Test 4: Layer composition
        auto baseLayer = Layer::create(graphics, TEST_SIZE * 2, TEST_SIZE * 2);
        auto topLayer = Layer::create(graphics, TEST_SIZE * 2, TEST_SIZE * 2);
        auto finalLayer = Layer::create(graphics, TEST_SIZE * 2, TEST_SIZE * 2);
        
baseLayer.clear(0, 0, 0, 0);
topLayer.clear(0, 0, 0, 0);
finalLayer.clear(0, 0, 0, 0);

        // Draw dirt background
        dirt.render(baseLayer, 0, 0);
        
        // Draw masked grass in center
        compositeTexture.render(topLayer, TEST_SIZE/2, TEST_SIZE/2, BlendMode::AlphaPreserve);
        
        // Compose layers
        baseLayer.flattenTo(finalLayer);
        topLayer.flattenTo(finalLayer, BlendMode::AlphaPreserve);
        
        // Save final result
        finalLayer.save("logs/07_final_composition.png");
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}