We're building something really interesting - a friendly wrapper around SDL2 that makes it much easier and safer to work with graphics. Instead of dealing with SDL's complex C-style functions and having to manage different types of textures and render targets, we're creating a unified, intuitive interface.

Think of it like this: SDL is powerful but complicated, with lots of ways to make mistakes. Our wrapper is like adding guardrails - it helps prevent those mistakes while making everything simpler to understand. We've created two main classes: Graphics handles the window and basic rendering, while Texture gives us a single, clear way to work with images, whether we're loading them from files, drawing them, or copying between them.

The cool thing about our approach is that users of our wrapper don't need to know anything about SDL's complexities - they just work with simple, intuitive functions.

The whole thing is part of a larger project to build a texture painting system, but we're taking our time to build this foundation right. We're using modern C++ features to make everything safe and efficient, but we're keeping the actual interface as simple and clear as possible.

Design principles:
- Hide SDL complexity from users
- Provide intuitive, unified interfaces
- Use modern C++ features (RAII, move semantics)
- Keep the API simple but powerful