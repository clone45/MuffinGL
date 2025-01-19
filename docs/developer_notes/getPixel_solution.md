
The issue with your original code was that you were trying to read pixel data from a GPU texture created using `IMG_LoadTexture`. GPU textures, especially when loaded with `IMG_LoadTexture`, are optimized for rendering, not direct pixel manipulation or reading. They often have constraints like being "static" (not writable) or using formats that aren’t directly compatible with `SDL_RenderReadPixels`. This was why you were seeing unexpected results: the color values got altered, and the alpha channel wasn’t behaving as expected.

To fix this, the solution involved introducing an `SDL_Surface`. Surfaces in SDL are software-based images stored in system memory, not on the GPU. This makes them perfect for tasks like directly accessing pixel data. Here’s what we did:

1. When loading your alpha map, instead of jumping straight to a GPU texture with `IMG_LoadTexture`, we first loaded it as a surface using `IMG_Load`. A surface stores the raw pixel data in a way that SDL understands and can easily manipulate.

2. We then created a GPU texture from this surface using `SDL_CreateTextureFromSurface`. This gives you the best of both worlds: a texture for fast rendering and a surface for direct access to pixel data.

3. For the `getPixel` function, we used the surface instead of the texture to read the pixel data. The surface provides direct access to its pixel array, so we could calculate the pixel's location and extract its RGBA values without any GPU-related issues.

By splitting these responsibilities—using the surface for pixel manipulation and the texture for rendering—we avoided the pitfalls of reading from GPU textures while still enabling fast rendering of the alpha map.

So, the crux of the solution was recognizing that surfaces and textures serve different purposes. Surfaces are ideal for operations like `getPixel`, while textures are designed for rendering. By combining them, we made your alpha map work exactly as intended!