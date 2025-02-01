# SDL2 Texture Painter - Combined Class Design

## Core System

### Application
```cpp
class Application {
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    std::unique_ptr<Canvas> m_canvas;
    std::unique_ptr<BrushTool> m_brush;
    std::unique_ptr<InputHandler> m_input;
    std::unique_ptr<TextureManager> m_textureManager;
    // ...
};
```
- Main application control
- SDL2 initialization and cleanup
- Main loop management
- Resource coordination

### TextureManager
```cpp
class TextureManager {
    SDL_Renderer* m_renderer;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
    
    public:
        std::shared_ptr<Texture> loadTexture(const std::string& path);
        std::shared_ptr<Texture> createTexture(int width, int height);
        void releaseUnused();
        // ...
};
```
- RAII texture management
- Texture loading and caching
- Resource cleanup
- Reference counting for textures

### Texture
```cpp
class Texture {
    SDL_Texture* m_texture;
    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;
    BlendMode m_blendMode;
    
    public:
        void setBlendMode(const BlendMode& mode);
        void setAsRenderTarget();
        // ...
};
```
- RAII wrapper for SDL_Texture
- Blend mode management
- Render target functionality

## Canvas System

### Canvas
```cpp
class Canvas {
    std::shared_ptr<Texture> m_baseLayer;
    std::shared_ptr<Texture> m_drawLayer;
    std::unique_ptr<Camera> m_camera;
    float m_drawLayerOpacity;
    
    public:
        void bakeLayer();
        void clearDrawLayer();
        void render();
        // ...
};
```
- Dual-layer management
- Layer compositing
- View transformation via Camera
- Render management

### Camera
```cpp
class Camera {
    Vec2f m_position;
    float m_zoom;
    float m_rotation;
    Rectangle m_viewport;
    
    public:
        Vec2f screenToWorld(const Vec2f& screenPos) const;
        Vec2f worldToScreen(const Vec2f& worldPos) const;
        void pan(const Vec2f& delta);
        void zoom(float factor);
        // ...
};
```
- View transformation
- Coordinate conversion
- Pan/zoom operations

## Brush System

### BrushTool
```cpp
class BrushTool {
    std::unique_ptr<TextureSampler> m_sampler;
    std::unique_ptr<BrushMask> m_mask;
    std::shared_ptr<Texture> m_combinedBrush;
    float m_size;
    uint8_t m_opacity;
    
    public:
        void updateBrush(const Vec2f& position);
        void draw(Canvas& canvas);
        // ...
};
```
- High-level brush control
- Coordinates TextureSampler and BrushMask
- Manages final brush composition

### TextureSampler
```cpp
class TextureSampler {
    std::shared_ptr<Texture> m_sourceTexture;
    Rectangle m_sampleWindow;
    float m_scale;
    
    public:
        void updateSamplePosition(const Vec2f& position);
        std::shared_ptr<Texture> getSample();
        // ...
};
```
- Texture sampling window management
- Sample position calculation
- Texture coordinate management

### BrushMask
```cpp
class BrushMask {
    std::shared_ptr<Texture> m_maskTexture;
    float m_size;
    
    public:
        void resize(float newSize);
        void applyTo(Texture& target);
        // ...
};
```
- Mask texture management
- Mask scaling operations
- Mask application

## Utility Classes

### BlendMode
```cpp
class BlendMode {
    SDL_BlendMode m_mode;
    
    public:
        static BlendMode Alpha();
        static BlendMode Additive();
        static BlendMode Multiply();
        static BlendMode Custom(SDL_BlendFactor srcColor, SDL_BlendFactor dstColor,
                              SDL_BlendOperation colorOp,
                              SDL_BlendFactor srcAlpha, SDL_BlendFactor dstAlpha,
                              SDL_BlendOperation alphaOp);
};
```
- Encapsulates SDL blend modes
- Provides preset configurations
- Custom blend mode creation

### Vec2f
```cpp
class Vec2f {
    float x, y;
    
    public:
        Vec2f operator+(const Vec2f& other) const;
        Vec2f operator*(float scalar) const;
        float length() const;
        // ...
};
```
- 2D vector operations
- Mathematical utilities
- Position and coordinate management

### Rectangle
```cpp
class Rectangle {
    float x, y, width, height;
    
    public:
        bool contains(const Vec2f& point) const;
        bool intersects(const Rectangle& other) const;
        Rectangle& scale(float factor);
        // ...
};
```
- Rectangle utilities
- Intersection testing
- Dimension management

## Input Management

### InputHandler
```cpp
class InputHandler {
    Canvas* m_canvas;
    BrushTool* m_brush;
    Camera* m_camera;
    bool m_isPanning;
    Vec2f m_lastMousePos;
    
    public:
        void handleEvent(const SDL_Event& event);
        void update();
        // ...
};
```
- Event processing
- Input state management
- Coordinate conversion
- Tool state management

## Key Improvements Over Previous Designs

1. **Resource Management**
   - Proper RAII with smart pointers
   - Centralized texture management
   - Reference counting for shared resources

2. **Separation of Concerns**
   - Clear distinction between sampling and masking
   - Dedicated camera system
   - Encapsulated blend modes

3. **Mathematical Utilities**
   - Vector and rectangle classes
   - Consistent coordinate handling
   - Reusable geometric operations

4. **Memory Safety**
   - Smart pointer usage
   - Clear ownership hierarchies
   - Protected resource cleanup

5. **Extensibility**
   - Easy to add new brush types
   - Flexible rendering pipeline
   - Modular design for future features