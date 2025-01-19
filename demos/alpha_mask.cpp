#include "graphics.hpp"
#include "texture.hpp"
#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(600, 400, "KansoGL Mask Test");
        
        // Load and resize textures to 200x200
        auto dirtTexture = Texture::create(graphics, "resources/dirt.png");
        auto grassTexture = Texture::create(graphics, "resources/grass.png");
        auto maskTexture = Texture::create(graphics, "resources/mask_white.png");
        
        dirtTexture.resize(200, 200);
        grassTexture.resize(200, 200);
        maskTexture.resize(200, 200);

        // Create a working texture
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

            // Calculate mask position
            time += 0.016f;
            float x = std::cos(time) * 50.0f;
            float y = std::sin(time) * 50.0f;

            graphics.clear();
            
            // Draw dirt background
            dirtTexture.render(50, 50);

            // Clear working texture
            drawLayer.clear(0, 0, 0, 0);

            // Render mask into working texture with "None" blend mode (overwrite alpha)
            maskTexture.render(
                drawLayer,
                static_cast<int>(x),
                static_cast<int>(y),
                BlendMode::None
            );

            // Render grass, but only where mask alpha allows it
            grassTexture.render(
                drawLayer,
                static_cast<int>(x),  // Match mask position
                static_cast<int>(y),
                BlendMode::Multiply
            );

            // Blend result over dirt
            drawLayer.render(50, 50, BlendMode::Alpha);
            
            graphics.render();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}