Yes, there are known issues with SDL's texture sampling and scaling that can affect alpha gradients:

1. Texture filtering can cause problems with alpha gradients, especially when magnifying textures. When linear filtering is applied to textures with sharp alpha transitions, it can result in unwanted artifacts such as black borders around transparent areas[1].

2. The issue arises when mixing opaque pixels with transparent pixels during texture magnification. This can lead to the creation of semi-transparent gray pixels instead of the desired semi-transparent white pixels[1].

3. In some cases, alpha values may not blend as expected when rendering textures over opaque pixels. This can cause the transparency to "eat away" at the underlying image rather than stacking like stained glass[1].

4. There have been reports of SDL_CreateTextureFromSurface not properly handling alpha channels due to incorrect pixel format detection. This was related to a bug in the SDL_MasksToPixelFormatEnum function, where it would return an incorrect pixel format for 32-bit surfaces with an alpha channel[2].

To mitigate these issues, several solutions have been suggested:

1. Increase the size of the opaque area in the texture by at least one pixel without changing the alpha values, creating a transparent border around the opaque content[1].

2. Use separate alpha blend functions, such as those provided by the EXT_blend_func_separate extension in OpenGL[1].

3. For brush textures that only control alpha, consider using grayscale images uploaded as GL_ALPHA textures[1].

4. Ensure proper blending mode setup for textures with alpha gradients using SDL_SetTextureBlendMode[3].

5. When creating surfaces and textures, pay attention to the pixel format and alpha mask to ensure they are set up correctly for alpha channel support[2].

These issues highlight the importance of careful texture preparation and rendering setup when working with alpha gradients in SDL.

Citations:
[1] https://community.khronos.org/t/render-to-texture-issue-sdl-opengl/26284
[2] https://forums.libsdl.org/viewtopic.php?p=38479
[3] https://discourse.libsdl.org/t/texture-with-an-alpha-channel/19991
[4] https://discourse.libsdl.org/t/having-issues-with-texture-pixel-manipulation-and-alpha/19880
[5] https://stackoverflow.com/questions/3562098/sdl-opengl-texture-transparency
[6] https://gamedev.stackexchange.com/questions/80118/sdl2-ttf-gradient
[7] https://github.com/libsdl-org/SDL/issues/8915
[8] https://github.com/pygame/pygame/issues/1289