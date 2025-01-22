// C:\Code\GameDev\muffinGL\src\main.cpp
#include "graphics.hpp"
#include "texture.hpp"
#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(600, 400, "MuffinGL Mask Test");
        
        // Load and resize textures to 200x200
        auto dirtTexture = Texture::create(graphics, "resources/dirt.png");
        auto grassTexture = Texture::create(graphics, "resources/grass.png");
        auto maskTexture = Texture::create(graphics, "resources/mask_white.png");
        
        dirtTexture.resize(200, 200);
        grassTexture.resize(200, 200);
        maskTexture.resize(200, 200);
        
        // Create a working texture for our composition
        auto drawLayer = Texture::create(graphics, 200, 200);
        
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
            float x = std::cos(time) * 50.0f;
            float y = std::sin(time) * 50.0f;
            
            graphics.clear();
            
            // Draw dirt background
            dirtTexture.render(50, 50, nullptr);
            
            // Clear draw layer
            drawLayer.clear(0, 0, 0, 0);
            
            // Compose brush in local space (at 0,0)
//            grassTexture.render(
//                drawLayer,
//                0,         // Local space x
//                0,         // Local space y
//                BlendMode::None  // No blending for initial composition
//            );
            
            // BitBlt region to screen
            // void Texture::render(Texture& target,
            //       int sourceX, int sourceY, int sourceWidth, int sourceHeight,
            //       int destX, int destY, BlendMode mode) {

            grassTexture.render(
                drawLayer,
                x,         // Texture source position x
                y,         // Texture source position y
                200,       // Texture source width
                200,       // Texture source height
                0,         // Local space x
                0,         // Local space y
                BlendMode::None  // No blending for initial composition
            );

            // Apply the mask in local space
            drawLayer.applyMask(maskTexture);
            
            // Render the composed brush at the dynamic position
            drawLayer.render(
                static_cast<int>(x + 50),  // Offset from dirt texture
                static_cast<int>(y + 50), 
                nullptr, 
                BlendMode::Alpha
            );
            
            graphics.render();
        }
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}