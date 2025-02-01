# Graphics Class

## Overview
The Graphics class is the foundation of MuffinGL, providing window management, event handling, and basic rendering capabilities. It encapsulates SDL's window and renderer functionality into a more intuitive interface while adding safety through RAII principles.

## Key Features
- Automatic resource management (RAII)
- Hardware-accelerated rendering
- Event handling system
- Basic shape drawing
- Window management

## Basic Usage

### Creating a Window
```cpp
try {
    Graphics graphics(800, 600, "My Application");
} catch (const std::runtime_error& e) {
    // Handle initialization failure
}
```

### Main Loop Structure
```cpp
bool running = true;
while (running) {
    // Handle events
    Graphics::Event event;
    while (graphics.pollEvent(event)) {
        if (event.type == Graphics::EventType::Quit) {
            running = false;
        }
    }

    // Clear frame
    graphics.clear();

    // Your rendering code here

    // Present frame
    graphics.render();
}
```

## Event Handling

### Available Events
The Graphics class provides a type-safe event system that wraps SDL's event handling. Events include:
- Window events (quit)
- Mouse movement
- Mouse button clicks
- Mouse wheel scrolling

### Event Data Structures
The event system uses three main structures:

1. EventType
```cpp
enum class EventType {
    None,        // No event
    Quit,        // Window close requested
    MouseMove,   // Mouse movement
    MouseDown,   // Mouse button pressed
    MouseUp,     // Mouse button released
    MouseWheel   // Mouse wheel scrolled
};
```

2. MouseData
```cpp
struct MouseData {
    int x, y;          // Current mouse position
    uint8_t button;    // Button involved in the event
    int wheelX;        // Horizontal wheel movement
    int wheelY;        // Vertical wheel movement
};
```

3. Event
```cpp
struct Event {
    EventType type;
    MouseData mouseData;
};
```

### Example: Mouse Input Handling
```cpp
Graphics::Event event;
while (graphics.pollEvent(event)) {
    switch (event.type) {
        case Graphics::EventType::MouseDown:
            // Handle click at (event.mouseData.x, event.mouseData.y)
            break;
            
        case Graphics::EventType::MouseMove:
            // Handle mouse movement
            break;
            
        case Graphics::EventType::MouseWheel:
            // Handle scrolling
            break;
    }
}
```

## Drawing Operations

### Clear Screen
```cpp
graphics.clear();  // Clears screen to black
```

### Basic Shape Drawing
```cpp
graphics.drawRectangle(x, y, width, height);  // Draws a rectangle
```

## Internal Details

### Resource Management
The Graphics class manages two key SDL resources:
- `SDL_Window*`: The application window
- `SDL_Renderer*`: The hardware-accelerated renderer

These resources are automatically cleaned up when the Graphics object is destroyed, preventing memory leaks.

### Renderer Access
The Graphics class provides controlled access to the underlying SDL renderer through the `getRenderer()` method, which is used by other MuffinGL components (like Texture and Layer) to perform their rendering operations.

## Best Practices

### Initialization
- Always create the Graphics instance first before any other MuffinGL components
- Use try-catch when creating a Graphics instance as initialization can fail
- Only create one Graphics instance per application

### Event Handling
- Process all events each frame to prevent event queue backup
- Handle the Quit event to allow proper application shutdown
- Clear and render every frame, even if no drawing occurs

### Resource Management
- Let the Graphics instance clean up automatically rather than manual cleanup
- Don't try to use the Graphics instance after it's been destroyed