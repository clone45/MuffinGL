#include "graphics.hpp"
#include "texture.hpp"
#include <iostream>
#include <cmath>

int main(int argc, char* argv[]) {
    try {
        Graphics graphics(800, 600, "Advanced Wrap Test");
        
        // Load source texture
        auto sourceTexture = Texture::create(graphics, "resources/grass.png");
        auto destTexture = Texture::create(graphics, 800, 600);
        
        bool running = true;
        float time = 0;
        float scale = 1.0f;
        bool scaleIncreasing = true;
        
        while (running) {
            Graphics::Event event;
            while (graphics.pollEvent(event)) {
                if (event.type == Graphics::EventType::Quit) {
                    running = false;
                }
            }

            graphics.clear();
            
            // Update animations
            time += 0.016f;  // Approximate 60 FPS
            if (scaleIncreasing) {
                scale += 0.001f;
                if (scale > 2.0f) scaleIncreasing = false;
            } else {
                scale -= 0.001f;
                if (scale < 0.5f) scaleIncreasing = true;
            }

            // Test 1: Rotating sample positions (top left)
            {
                float radius = 50.0f;
                float x = std::cos(time * 2.0f) * radius;
                float y = std::sin(time * 2.0f) * radius;
                sourceTexture.render(
                    static_cast<int>(x), static_cast<int>(y),
                    32, 32,
                    100, 100
                );
            }

            // Test 2: Growing/shrinking sample size (top right)
            {
                int sampleSize = static_cast<int>(64.0f * scale);
                sourceTexture.render(
                    0, 0,
                    sampleSize, sampleSize,
                    500, 100
                );
            }

            // Test 3: Diagonal movement with large sample (middle)
            {
                float diagonal = std::fmod(time * 100.0f, 200.0f) - 100.0f;
                sourceTexture.render(
                    static_cast<int>(diagonal), 
                    static_cast<int>(diagonal),
                    128, 128,
                    300, 200
                );
            }

            // Test 4: Multiple small samples in a grid (bottom)
            {
                float offset = std::sin(time * 3.0f) * 32.0f;
                for (int i = 0; i < 8; i++) {
                    sourceTexture.render(
                        static_cast<int>(offset) + (i * 16), 
                        static_cast<int>(offset),
                        16, 16,
                        100 + (i * 80), 400
                    );
                }
            }

            // Test 5: Super-wide sample (spanning multiple wraps)
            {
                float yPos = std::sin(time) * 32.0f;
                sourceTexture.render(
                    0, static_cast<int>(yPos),
                    400, 32,  // width larger than texture
                    200, 500
                );
            }
            
            graphics.render();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}