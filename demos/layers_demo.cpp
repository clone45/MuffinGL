#include "graphics.hpp"
#include "texture.hpp"
#include "layer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(800, 600, "Layer System Demo");
        
        // Create our working layers
        auto baseLayer = Layer::create(graphics, 800, 600);       // Background
        auto paintLayer = Layer::create(graphics, 800, 600);      // For painting
        auto compositeLayer = Layer::create(graphics, 800, 600);  // Final composition
        
        // Load textures
        auto background = Texture::create(graphics, "resources/dirt.png");
        auto brushTexture = Texture::create(graphics, "resources/grass.png");
        auto brushMask = Texture::create(graphics, "resources/mask_white.png");
        
        // Create composition buffer for brush
        auto compositeBrush = Texture::create(graphics, 100, 100);
        
        // Resize textures as needed
        background.resize(800, 600);
        brushTexture.resize(100, 100);
        brushMask.resize(100, 100);
        
        bool running = true;
        bool painting = false;
        int mouseX = 0, mouseY = 0;
        
        while (running) {
            // Handle events
            Graphics::Event event;
            while (graphics.pollEvent(event)) {
                switch (event.type) {
                    case Graphics::EventType::Quit:
                        running = false;
                        break;
                        
                    case Graphics::EventType::MouseDown:
                        painting = true;
                        mouseX = event.mouseData.x;
                        mouseY = event.mouseData.y;
                        break;
                        
                    case Graphics::EventType::MouseUp:
                        painting = false;
                        break;
                        
                    case Graphics::EventType::MouseMove:
                        mouseX = event.mouseData.x;
                        mouseY = event.mouseData.y;
                        break;
                }
            }
            
            // Clear the frame
            graphics.clear();
            
            // Draw background to base layer
            background.render(baseLayer, 0, 0);
            
            // Handle painting
            if (painting) {
                // Clear the composition buffer
                compositeBrush.clear(0, 0, 0, 0);
                
                // Draw brush texture to composition buffer
                brushTexture.render(compositeBrush, 0, 0, BlendMode::None);
                
                // Apply the mask
                compositeBrush.applyMask(brushMask);
                
                // Add brush to paint layer
                compositeBrush.render(
                    paintLayer,
                    mouseX - 50,
                    mouseY - 50,
                    BlendMode::AlphaPreserve
                );
            }
            
            // Compose final frame
            baseLayer.flattenTo(compositeLayer, BlendMode::Alpha);      // Copy background
            paintLayer.flattenTo(compositeLayer, BlendMode::AlphaPreserve);  // Add paint layer
            
            // Render to screen
            compositeLayer.render(0, 0);
            
            graphics.render();
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}