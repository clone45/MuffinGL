# MuffinGL API Reference

## Graphics Class
The primary interface for window management and rendering.

### Constructor
```cpp
Graphics(int width, int height, const std::string& title)
```
Creates a new window with hardware-accelerated rendering.

- **Parameters:**
  - `width`: Window width in pixels
  - `height`: Window height in pixels
  - `title`: Window title

### Event Handling

#### Types
```cpp
enum class EventType {
    None,
    Quit,
    MouseMove,
    MouseDown,
    MouseUp,
    MouseWheel
}
```

```cpp
struct MouseData {
    int x, y;          // Mouse coordinates
    uint8_t button;    // Mouse button
    int wheelX;        // Horizontal wheel movement
    int wheelY;        // Vertical wheel movement
}
```

```cpp
struct Event {
    EventType type = EventType::None;
    MouseData mouseData = {0, 0, 0};
}
```

#### Methods
```cpp
bool pollEvent(Event& event)
```
Polls for pending events. Returns true if an event was retrieved.

### Rendering Methods
```cpp
void clear()
```
Clears the screen to black.

```cpp
void render()
```
Presents the current frame to the screen.

```cpp
void drawRectangle(int x, int y, int width, int height)
```
Draws a rectangle at the specified position.

## Texture Class
Handles image loading and manipulation. Inherits from Raster.

### Factory Methods
```cpp
static Texture create(Graphics& graphics, int width, int height)
```
Creates an empty texture with the specified dimensions.

```cpp
static Texture create(Graphics& graphics, const std::string& path, bool makeTarget = false)
```
Loads a texture from an image file.
- `makeTarget`: If true, creates the texture as a render target

### Methods
```cpp
void resize(int width, int height, ScaleMode mode = ScaleMode::Default)
```
Resizes the texture to new dimensions.

```cpp
void applyMask(Texture& mask)
```
Applies an alpha mask to the texture.

```cpp
Color getPixel(int x, int y) const
```
Gets the color of a specific pixel.

## Layer Class
Manages layers for composition. Inherits from Raster.

### Factory Methods
```cpp
static Layer create(Graphics& graphics, int width, int height)
```
Creates a new layer with specified dimensions.

### Methods
```cpp
void flattenTo(Layer& target, BlendMode mode = BlendMode::LayerComposite)
```
Flattens this layer onto the target layer using the specified blend mode.

## Camera Class
Handles view transformations for pan and zoom operations.

### Constructors
```cpp
Camera()
Camera(float x, float y, float zoom)
```

### Methods
```cpp
void pan(float dx, float dy)
```
Moves the camera by the specified delta.

```cpp
void zoomAtPoint(float factor, int screenX, int screenY)
```
Zooms the camera, keeping the specified screen point stationary.

```cpp
void worldToScreen(float worldX, float worldY, int& screenX, int& screenY) const
void screenToWorld(int screenX, int screenY, float& worldX, float& worldY) const
```
Convert between world and screen coordinates.

## Common Types

### BlendMode
```cpp
enum class BlendMode {
    None,
    Alpha,
    Additive,
    Multiply,
    AlphaPreserve,
    LayerComposite
}
```

### Color
```cpp
struct Color {
    uint8_t r, g, b, a;
}
```

## Error Handling
Most methods that can fail will throw a `std::runtime_error` with a descriptive message.

### Common Error Cases
- Failed resource loading
- Invalid dimensions
- Out of memory
- Invalid operation parameters