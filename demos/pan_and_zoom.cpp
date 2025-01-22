// main.cpp
#include "graphics.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(800, 600, "Camera Test");
        auto grassTexture = Texture::create(graphics, "resources/grass.png");
        
        Camera camera(0, 0, 1.0f);
        bool running = true;
        float time = 0;
        
        // Track mouse state for panning
        bool isPanning = false;
        int lastMouseX = 0;
        int lastMouseY = 0;
        
        while (running) {
            Graphics::Event event;

            while (graphics.pollEvent(event)) {
                if (event.type == Graphics::EventType::Quit) {
                    running = false;
                }
                else if (event.type == Graphics::EventType::MouseDown && 
                        event.mouseData.button == SDL_BUTTON_RIGHT) {
                    isPanning = true;
                    lastMouseX = event.mouseData.x;
                    lastMouseY = event.mouseData.y;
                }
                else if (event.type == Graphics::EventType::MouseUp && 
                        event.mouseData.button == SDL_BUTTON_RIGHT) {
                    isPanning = false;
                }
                else if (event.type == Graphics::EventType::MouseMove && isPanning) {
                    int dx = event.mouseData.x - lastMouseX;
                    int dy = event.mouseData.y - lastMouseY;
                    
                    // Convert screen delta to world delta based on zoom
                    float worldDX = -dx / camera.getZoom();
                    float worldDY = -dy / camera.getZoom();
                    
                    camera.pan(worldDX, worldDY);
                    
                    lastMouseX = event.mouseData.x;
                    lastMouseY = event.mouseData.y;
                }
                else if (event.type == Graphics::EventType::MouseWheel) {
                    // Calculate zoom factor based on wheel direction
                    float zoomFactor = (event.mouseData.wheelY > 0) ? 1.1f : 0.9f;
                    
                    // Zoom centered on mouse position
                    camera.zoomAtPoint(zoomFactor, event.mouseData.x, event.mouseData.y);
                }
            }
            
            // Update animation
            time += 0.016f;
            
            graphics.clear();
            
            // Render some test patterns using the camera
            
            // Static texture
            grassTexture.render(100, 100, &camera);
            
            // Animated texture with wrapping
            int sampleSize = 64;
            float x = std::cos(time) * 50.0f;
            float y = std::sin(time) * 50.0f;
            grassTexture.render(
                static_cast<int>(x), static_cast<int>(y),  // source coords
                sampleSize, sampleSize,                     // source size
                300, 200,                                   // dest position
                &camera                                     // camera transform
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