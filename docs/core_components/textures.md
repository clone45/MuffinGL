# Texture Class

## Overview
The Texture class manages 2D images in MuffinGL, providing functionality for loading, manipulating, and rendering images. It inherits from the Raster class and adds specific image-handling capabilities. Textures can be created from files or generated dynamically, and support various blend modes for complex rendering effects.

## Key Features
- Load images from files (PNG support)
- Dynamic texture creation
- Alpha channel support
- Custom blend modes
- Hardware-accelerated rendering
- Texture scaling and resizing
- Masking operations

## Basic Usage

### Creating Textures

```cpp
// Create from file
auto texture = Texture::create(graphics, "path/to/image.png");

// Create empty texture
auto emptyTexture = Texture::create(graphics, 512, 512);

// Create render target texture
auto targetTexture = Texture::create(graphics, "image.png", true);
```

### Basic Operations

```cpp
// Resize a texture
texture.resize(256, 256);

// Render texture to screen
texture.render(100, 100);  // At position (100, 100)

// Render with camera transformation
camera.zoomAtPoint(1.5f, mouseX, mouseY);
texture.render(x, y, &camera);
```

### Advanced Operations

```cpp
// Apply a mask to a texture
auto mask = Texture::create(graphics, "mask.png");
texture.applyMask(mask);

// Render with blend mode
texture.render(x, y, BlendMode::AlphaPreserve);

// Get pixel color
Color color = texture.getPixel(x, y);
```

## Blend Modes
Textures support various blend modes for different rendering effects:

- `BlendMode::None`: No blending, direct copy
- `BlendMode::Alpha`: Standard alpha blending
- `BlendMode::Additive`: Add source to destination
- `BlendMode::Multiply`: Multiply source with destination
- `BlendMode::AlphaPreserve`: Preserves alpha channel during blending
- `BlendMode::LayerComposite`: Special mode for layer composition

### Example: Blend Mode Usage
```cpp
// Standard alpha blending
texture.render(x, y, BlendMode::Alpha);

// Additive blending for glow effects
texture.render(x, y, BlendMode::Additive);

// Alpha preservation for masks
texture.render(x, y, BlendMode::AlphaPreserve);
```

## Texture Wrapping
When rendering portions of textures, MuffinGL supports automatic wrapping:

```cpp
// Render a wrapped portion of the texture
texture.render(sourceX, sourceY,     // Source position
              sourceWidth, sourceHeight,  // Source dimensions
              destX, destY);         // Destination position
```

## Internal Details

### Memory Management
- Textures use RAII for automatic resource cleanup
- Move semantics are supported for efficient texture handling
- Underlying SDL textures are managed automatically

### Implementation Notes
- Textures are stored in GPU memory for efficient rendering
- When created as render targets, textures can be both read from and written to
- Alpha channel is preserved throughout texture operations

## Best Practices

### Resource Management
```cpp
// Prefer automatic cleanup
{
    auto texture = Texture::create(graphics, "image.png");
    // Use texture...
} // Texture is automatically cleaned up here
```

### Performance Tips
- Avoid excessive texture creation and destruction
- Use appropriate texture sizes (preferably powers of 2)
- Consider using render targets for complex operations
- Batch similar rendering operations together

### Error Handling
```cpp
try {
    auto texture = Texture::create(graphics, "image.png");
} catch (const std::runtime_error& e) {
    // Handle loading failure
}
```

## Common Issues and Solutions

### Memory Usage
- Large textures should be scaled down when possible
- Consider using compressed texture formats for large images
- Release unused textures promptly

### Alpha Channel
- Use `BlendMode::AlphaPreserve` when maintaining alpha is critical
- Check alpha settings when textures appear incorrect
- Use masking operations for complex alpha effects

### Rendering Quality
- Use appropriate scale modes for resizing
- Consider using render targets for multi-step operations
- Test different blend modes for desired effects