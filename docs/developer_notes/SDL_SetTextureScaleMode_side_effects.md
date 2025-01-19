SDL_SetTextureScaleMode() does affect how gradients are handled in SDL2, particularly when scaling textures. Here's how it impacts gradient handling:

1. The function allows you to set the scale mode used for texture scale operations[1]. This is particularly relevant for textures containing gradients, as the scaling method can significantly impact the smoothness and appearance of the gradient.

2. There are typically three scale modes available:
   - SDL_ScaleModeNearest: This uses nearest neighbor scaling, which can result in pixelation of gradients when scaled up.
   - SDL_ScaleModeLinear: This uses linear filtering, which can provide smoother gradients when scaling.
   - SDL_ScaleModeAnisotropic: This provides the highest quality scaling, potentially preserving gradient quality better than the other modes.

3. When dealing with alpha gradients, the choice of scale mode becomes crucial. Linear or anisotropic scaling can help maintain the smoothness of alpha transitions during scaling operations[1].

4. However, it's important to note that not all scale modes may be supported on all systems. If an unsupported mode is specified, SDL will choose the closest supported mode[1].

5. The scale mode set by SDL_SetTextureScaleMode() interacts with the renderer's scaling quality hint. For best results with gradients, especially alpha gradients, using high-quality scaling hints in combination with appropriate scale modes can yield better results[1].

6. It's worth noting that while SDL_SetTextureScaleMode() can help with gradient quality during scaling, it doesn't directly address some known issues with SDL's texture sampling affecting alpha gradients, such as problems with linear filtering causing unwanted artifacts around transparent areas.

In practice, when working with gradients, especially alpha gradients, you may need to experiment with different scale modes to find the best balance between performance and visual quality for your specific use case.

Citations:
[1] https://metacpan.org/pod/SDL2::render
[2] https://community.khronos.org/t/render-to-texture-issue-sdl-opengl/26284
[3] https://stackoverflow.com/questions/20348616/how-to-create-a-color-gradient-in-sdl
[4] https://bumbershootsoft.wordpress.com/2018/10/26/aspect-corrected-image-scaling-with-sdl2/
[5] https://gamedev.stackexchange.com/questions/71970/background-color-gradient-with-sdl
[6] https://www.reddit.com/r/roguelikedev/comments/dnhuq5/sdl_and_lighting_textures/
[7] http://forums.libsdl.org/viewtopic.php?p=41355
[8] https://grimfang4.github.io/sdl-gpu/group__ImageControls.html