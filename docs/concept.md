# Texture Painting System Documentation

## Overview
The system provides a specialized texture painting tool designed primarily for terrain editing in game development. It enables artists and level designers to paint seamless textures (like grass, sand, or snow) onto terrain surfaces while maintaining natural variation and avoiding obvious repetition patterns.

### Game Development Context
In game development, terrain texturing is a crucial part of level design. Rather than using a single tiled texture across an entire terrain, designers need tools to:
- Blend different terrain types naturally (e.g., grass fading into dirt)
- Create organic-looking transitions between textures
- Add variety to prevent obvious texture repetition
- Quickly iterate on terrain appearance during level design

### Comparison to Industry Tools
This system implements functionality similar to several well-known tools:

- **Unity's Terrain Tool**: Uses a similar layer-based approach where textures are painted onto terrain with variable opacity. Our system's two-layer approach (base and drawing layer) mirrors Unity's preview system for terrain modifications.

- **Photoshop's Pattern Stamp Tool**: Our texture sampling mechanism is very similar to Photoshop's Pattern Stamp, particularly in how it:
  - Samples from a repeating pattern (our grass texture)
  - Uses position-based sampling to create variation
  - Applies the sample through a masked brush

- **Substance Painter's Smart Materials**: While simpler, our system's approach to texture application is conceptually similar to how Substance Painter applies materials, particularly in:
  - Using masks to control texture application
  - Supporting opacity-based blending
  - Maintaining texture coherence across strokes

The key feature that distinguishes this system is its "texture brush" behavior, which combines:
- Seamless texture sampling (like Photoshop's Pattern Stamp)
- Mask-based application (like digital painting brushes)
- Real-time preview (like Unity's terrain tools)
- Automatic variation based on position (similar to Substance Painter's smart masks)

## Core Components

### Canvas System
The canvas uses a dual-layer approach:
1. **Base Layer**: The permanent layer that holds the final result
2. **Drawing Layer**: A temporary layer used during active brush strokes
   - Can have variable opacity
   - Gets "baked" into the base layer when a brush stroke is completed

### Brush Tool
The brush combines multiple elements to create the final brush effect:

1. **Source Texture** (e.g., grass texture)
   - Seamless 512x512 texture
   - Gets sampled in a "window" pattern
   - Texture coordinates update based on brush position

2. **Mask Texture**
   - Defines the brush's alpha channel
   - Gets resized to match the brush size
   - Applied to the sampled texture portion

3. **Brush Assembly Process**
   - Sample a portion of the source texture based on brush position
   - Scale the texture sample to the brush size
   - Scale the mask to match the brush size
   - Combine the scaled texture and mask

## Key Technical Processes

### Texture Sampling
- A "window" into the source texture is used rather than the entire texture
- The sampling position changes based on brush movement
- Formula: `sample_position = brush_position % texture_size`
- This creates a "texture brush" effect without manual selection

### Brush Assembly
The brush is reassembled whenever its position changes:
1. Calculate the texture sampling rectangle based on current position
2. Extract the portion of the source texture
3. Scale both the texture portion and mask to brush size
4. Combine using blend modes:
   - Source texture: Preserve color and alpha
   - Mask: Multiply with existing alpha

### Layer Management
1. **Drawing Process**
   - Active strokes are rendered to the drawing layer
   - Drawing layer can have variable opacity
   - Uses custom shader for brush application

2. **Baking Process**
   - Triggered when mouse button is released
   - Drawing layer is composited onto base layer
   - Drawing layer is cleared for next stroke

## Implementation Details

### Blend Modes
Different blend modes are used for different stages:

1. **Texture Assembly**
```cpp
BlendMode(
    Factor::One,              // Source color
    Factor::Zero,             // Dest color
    Equation::Add,            // Color equation
    Factor::One,              // Source alpha
    Factor::Zero,             // Dest alpha
    Equation::Add             // Alpha equation
)
```

2. **Mask Application**
```cpp
BlendMode(
    Factor::DstColor,         // Keep dest color
    Factor::Zero,             // Ignore source color
    Equation::Add,            // Add them
    Factor::DstAlpha,         // Keep dest alpha
    Factor::Zero,             // Ignore source alpha
    Equation::Add             // Add them
)
```

### View Management
- Supports pan and zoom operations
- Maintains proper brush positioning in world coordinates
- Snaps to grid when drawing

## Asset Requirements

1. **Source Texture (Grass)**
   - 512x512 seamless texture
   - Used for brush content
   - Should tile naturally

2. **Base Texture (Terrain)**
   - Any size
   - Acts as the canvas
   - Loaded at startup

3. **Mask Texture**
   - Alpha map for brush shape
   - Gets dynamically resized
   - Determines brush softness/hardness

## Technical Considerations

### Performance Optimizations
- Texture scaling is cached when possible
- Brush assembly only occurs on position changes
- Uses hardware-accelerated rendering
- Efficient blend modes minimize render passes

### Memory Management
- Textures are properly managed with RAII
- Render textures are reused rather than recreated
- Clear cleanup of resources in destructors

## Future Enhancements
Potential areas for improvement:

1. Multiple brush patterns
2. Adjustable brush rotation
3. Pressure sensitivity support
4. Multiple layer support
5. Brush spacing control
6. Dynamic mask generation
7. Texture blending modes