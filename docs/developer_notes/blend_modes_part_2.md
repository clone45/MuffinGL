Here's a detailed breakdown of what we learned about SDL2 blend modes and texture composition:

# The Investigation
We were debugging an issue where a grass texture with a mask wasn't compositing correctly with a dirt texture background. The initial symptoms were:
1. A dark ring appearing around the grass texture
2. The dirt texture completely disappearing in some attempts to fix it
3. Alpha values not being preserved correctly during different stages of the rendering

# Key Discoveries

## Custom Blend Modes in SDL2
1. SDL2 allows creation of custom blend modes using `SDL_ComposeCustomBlendMode` with these parameters:
   - Source color factor
   - Destination color factor
   - Color operation
   - Source alpha factor
   - Destination alpha factor
   - Alpha operation

2. The Direct3D renderer supports custom blend modes (verified through testing)
   ```cpp
   SDL_BlendMode testCustom = SDL_ComposeCustomBlendMode(...);
   // Test returned true, indicating support
   ```

## Blend Mode States
1. We found that blend modes need to be tracked and managed carefully:
   - Initial states need to be stored (`m_originalBlendMode`)
   - States need to be restored after operations
   - Different textures (source and target) can have different blend modes

2. Debug output revealed blend modes were sometimes reverting to `SDL_BLENDMODE_BLEND` (value 1) when we expected custom modes

## Different Operations Need Different Blend Modes
We discovered that different graphical operations require different blend mode configurations:

1. **Mask Operation Blend Mode** (AlphaPreserve):
```cpp
SDL_ComposeCustomBlendMode(
    SDL_BLENDFACTOR_ONE,                    // source color factor
    SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination color factor
    SDL_BLENDOPERATION_ADD,                 // color operation
    SDL_BLENDFACTOR_ONE,                    // source alpha factor
    SDL_BLENDFACTOR_ZERO,                   // destination alpha factor
    SDL_BLENDOPERATION_ADD                  // alpha operation
);
```
This configuration preserves alpha during masking operations while preventing alpha multiplication artifacts.

2. **Layer Composition Blend Mode** (LayerComposite):
```cpp
SDL_ComposeCustomBlendMode(
    SDL_BLENDFACTOR_SRC_ALPHA,              // source color factor
    SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,    // destination color factor
    SDL_BLENDOPERATION_ADD,                 // color operation
    SDL_BLENDFACTOR_ONE,                    // source alpha factor
    SDL_BLENDFACTOR_ONE,                    // destination alpha factor
    SDL_BLENDOPERATION_ADD                  // alpha operation
);
```
This configuration allows proper blending between layers while maintaining alpha information.

# The Solution
1. Created two distinct blend modes in our enum:
```cpp
enum class BlendMode {
    None,
    Alpha,
    Additive,
    Multiply,
    AlphaPreserve,      // For grass masking
    LayerComposite      // For layer flattening
};
```

2. Used appropriate blend modes for different operations:
```cpp
// For masking operations
compositeTexture.render(topLayer, TEST_SIZE/2, TEST_SIZE/2, BlendMode::AlphaPreserve);

// For layer composition
baseLayer.flattenTo(finalLayer);
topLayer.flattenTo(finalLayer, BlendMode::LayerComposite);
```

# Debugging Techniques That Helped
1. Saving intermediate textures to PNG files:
```cpp
target.save("logs/debug_target_pre_render.png");
target.save("logs/debug_target_post_render.png");
```

2. Logging blend modes at each stage:
```cpp
SDL_BlendMode srcBlend, targetBlend;
SDL_GetTextureBlendMode(m_texture, &srcBlend);
std::cout << "Blend mode: " << blendModeToString(srcBlend) << std::endl;
```

3. Verifying texture properties:
```cpp
Uint32 format;
int access;
SDL_QueryTexture(m_texture, &format, &access, nullptr, nullptr);
```

4. Creating a utility function to translate blend modes to readable strings:
```cpp
std::string blendModeToString(SDL_BlendMode mode) {
    if(mode == SDL_BLENDMODE_NONE) return "NONE";
    if(mode == SDL_BLENDMODE_BLEND) return "BLEND";
    // ... etc
}
```

# Key Lessons
1. Different graphical operations (masking vs. compositing) may require different blend mode configurations
2. Custom blend modes need to be carefully tracked and restored
3. Debugging graphical operations benefits from both visual (saved images) and technical (logged states) information
4. The relationship between source and destination blend factors is crucial for achieving desired visual results
5. When dealing with alpha channels, both color and alpha blend factors need to be considered independently

This investigation highlighted the complexity of graphics blend modes and the importance of having different blend mode configurations for different types of operations.