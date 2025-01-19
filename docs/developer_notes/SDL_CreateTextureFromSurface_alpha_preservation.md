SDL_CreateTextureFromSurface generally preserves alpha gradients correctly, but there are some important considerations and potential issues to be aware of:

1. Alpha Channel Preservation: SDL_CreateTextureFromSurface typically maintains the alpha channel information when creating a texture from a surface with an alpha gradient[1].

2. Blending Mode: To properly display the alpha gradient, you need to set the correct blending mode for the texture after creation:

```c
SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
```

3. Potential Issues: There have been reports of problems with alpha handling in certain situations. For instance, a bug was identified where SDL_CreateTextureFromSurface was not using alpha because the surface's A mask was 0[1]. This issue was related to incorrect pixel format detection in the SDL_MasksToPixelFormatEnum function.

4. Surface Format: Ensure that the source surface has the correct pixel format with an alpha channel. Using a 32-bit surface format (RGBA) is recommended for surfaces with alpha gradients[1].

5. Alpha Modulation: SDL provides functions to modify the overall alpha of a texture, which can be useful for fading effects:

```c
SDL_SetTextureAlphaMod(texture, alphaValue);
```

This allows for dynamic adjustment of the entire texture's transparency[2].

6. Rendering Considerations: When rendering textures with alpha gradients, make sure your rendering pipeline supports alpha blending. This may involve additional setup in your rendering code.

7. Performance: For better performance with alpha gradients, consider using hardware-accelerated rendering with SDL's renderer API, which can handle alpha blending more efficiently[1].

In conclusion, while SDL_CreateTextureFromSurface generally preserves alpha gradients, it's crucial to ensure proper surface format, blending mode setup, and be aware of potential issues in specific SDL versions. If you encounter problems, checking your surface creation process and SDL version might help identify the cause.

Citations:
[1] https://forums.libsdl.org/viewtopic.php?p=38479
[2] https://thenumb.at/cpp-course/sdl2/05/05.html
[3] https://github.com/libsdl-org/SDL/issues/6990
[4] https://www.studyplan.dev/sdl-dev/sdl2-ttf/q/text-color-gradients
[5] https://stackoverflow.com/questions/20348616/how-to-create-a-color-gradient-in-sdl
[6] https://discourse.libsdl.org/t/bad-text-quality-on-transparent-texture/23098
[7] https://forum.processing.org/topic/linear-alpha-gradient
[8] https://www.reddit.com/r/sdl/comments/9jpdmj/adding_alpha_opacity_to_image_in_sdl_2/