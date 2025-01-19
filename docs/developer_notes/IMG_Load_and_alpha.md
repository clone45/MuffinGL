SDL_image, through the IMG_Load function, handles PNG alpha gradients effectively, preserving transparency information when loading PNG images with alpha channels[1][2]. Here's how PNG alpha gradients are handled:

1. Alpha Channel Preservation: When loading a PNG file with IMG_Load, the alpha channel information is retained, allowing for smooth transparency gradients[1].

2. 32-bit Surface Creation: IMG_Load typically creates a 32-bit surface for PNGs with alpha channels, ensuring that the full range of transparency values is maintained[2].

3. Blending Mode Setup: After loading the image, you need to enable alpha blending for the texture to properly display the transparency gradient:

```c
SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
```

4. Conversion Considerations: If you need to convert the loaded surface, be careful not to lose alpha information. Use SDL_ConvertSurface() with a pixel format that includes an alpha channel[1].

5. OpenGL Integration: When using SDL with OpenGL, you can specify the internal texture format to include alpha, ensuring the gradient is preserved during texture upload[2].

6. Performance Optimization: For better performance, consider using hardware-accelerated rendering with SDL's renderer API, which can handle alpha gradients more efficiently than software rendering.

By following these practices, SDL_image can effectively load and display PNG images with alpha gradients, maintaining smooth transitions in transparency across the image.

Citations:
[1] https://discourse.libsdl.org/t/sdl-2-0-and-png-alpha/21029
[2] http://forums.libsdl.org/viewtopic.php?p=26180
[3] https://github.com/libsdl-org/SDL/issues/6990
[4] https://stackoverflow.com/questions/8027026/sdl-image-png-transparency-error
[5] https://pyra-handheld.com/boards/threads/what-on-earth-is-wrong-transparent-pngs-arent-transparent-sdl.68437/
[6] https://gamedev.stackexchange.com/questions/80118/sdl2-ttf-gradient
[7] https://github.com/pygame/pygame/issues/1289
[8] https://gamedev.stackexchange.com/questions/179360/sdl-image-quality-on-ios