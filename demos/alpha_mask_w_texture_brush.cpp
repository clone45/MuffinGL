// C:\Code\GameDev\muffinGL\src\main.cpp
#include "muffinGL/graphics.hpp"
#include "muffinGL/texture.hpp"
#include "muffinGL/layer.hpp"
#include <iostream>
#include <cmath>

#define BRUSH_SIZE 100

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(600, 400, "MuffinGL Mask Test");
        
        // Load and resize textures to 200x200
        auto grassTexture = Texture::create(graphics, "resources/grass_1024x1024.png");
        auto maskTexture = Texture::create(graphics, "resources/mask_white_255x255.png");
        auto dirtTexture = Texture::create(graphics, "resources/dirt_1024x1024.png", true);
        auto compositeBrushLayer = Texture::create(graphics, BRUSH_SIZE, BRUSH_SIZE);

        // maskTexture.save("resources/loaded_mask.png");

        dirtTexture.resize(1024, 1024);
        grassTexture.resize(BRUSH_SIZE, BRUSH_SIZE);
        maskTexture.resize(BRUSH_SIZE, BRUSH_SIZE);
        
        // maskTexture.save("resources/resized_mask.png");        
        
        float time = 0.0f;
        bool running = true;
        
        while (running) {

            Graphics::Event event;
            while (graphics.pollEvent(event)) {
                if (event.type == Graphics::EventType::Quit) {
                    running = false;
                }
            }
            
            // Calculate position for final placement
            time += 0.016f;
            float x = (std::cos(time) * 50.0f) + 50.0f;
            float y = (std::sin(time) * 50.0f) + 50.0f;
            
            graphics.clear();
            
            compositeBrushLayer.clear(0, 0, 0, 0);

            // Draw the brush texture to the composite layer
            grassTexture.render(
                compositeBrushLayer,
                x,          // Texture source position x
                y,          // Texture source position y
                BRUSH_SIZE, // Texture source width
                BRUSH_SIZE, // Texture source height
                0,          // Local space x
                0,          // Local space y
                BlendMode::None  // No blending for initial composition
            );

            // Apply the mask to the composite layer
            compositeBrushLayer.applyMask(maskTexture);
            
            // save the composed brush to a file
            // compositeBrushLayer.save("resources/composite.png");

            // Render the composed brush to the dirt texture
            compositeBrushLayer.render(
                dirtTexture,
                static_cast<int>(x),
                static_cast<int>(y), 
                BlendMode::AlphaPreserve
            );
            
            // Draw dirt background with composed brush baked into it to the screen
            float screen_x = 50.0;
            float screen_y = 50.0;

            dirtTexture.render(screen_x, screen_y, nullptr);

            graphics.render();
        }
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}