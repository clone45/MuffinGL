Here’s the full content of the Markdown file:

```markdown
# Alpha Compositing, OpenGL Blending, and Premultiplied Alpha

1 The precise interpretation is more nuanced, as described in Interpreting Alpha by Andrew
Glassner.

2 Compositing Digital Images by Thomas Porter and Tom Duff.

apoorvaj.io

Alpha compositing, OpenGL blending
and premultiplied alpha
15 June 2019

Image compositing is the process of combining one or more images into a single one.
Compositing can be done in any number of ways, but a very common way of doing it is to
use the alpha channel for blending. Here, alpha is loosely treated as opacity.1

This is the equivalent of the image produced when a layer is composited on another layer
with the “Normal” blend mode in Photoshop. This technique was widely popularized by a
seminal paper published in 1984.2 This compositing technique is so common these days

that most people conceptualize alpha as being transparency, instead of just a numeric
channel.

## Table of contents

- Conventions and nomenclature
- Interlude: The OpenGL blending API
- The mathematics of alpha compositing
- Optimization: Opaque destination
- Pragmatism interlude: What if my destination isn’t opaque
- Premultiplied alpha

## Conventions and nomenclature

To simplify the discussion here, we’ll only talk about compositing two images at a time.
More images can be composited in exactly the same way, one after the other. We’ll call the
background image the destination, since it is already present in the buffer we want to composite on. We’ll call the foreground image the source.

Each image has four channels: Red, Green, Blue and Alpha.  
- **(R, G, B, A)_d** are the destination image channels.  
- **(R, G, B, A)_s** are the source image channels.  
- **(R, G, B, A)** are the resultant image channels.

For simplicity, we will assume that all channels lie in the range [0, 1].

## Interlude: The OpenGL blending API

If blending is not activated explicitly, OpenGL overwrites the destination with the source
image by default. In order to enable and control blending, there are three main function
calls:

1. `glEnable(GL_BLEND)` activates blending.

2. `glBlendEquation(mode)` sets the blend mode. The blend mode dictates what is done with
   the scaled source and destination values.  
   Example:  
   - The most common blend mode, `GL_FUNC_ADD`, evaluates channels by addition:
     ```
     R = R_s * k_s + R_d * k_d
     ```
   - `GL_FUNC_SUBTRACT` evaluates by subtraction:
     ```
     R = R_s * k_s - R_d * k_d
     ```
     *(Green, Blue, and Alpha channels are computed similarly.)*

3. `glBlendFunc(k_s, k_d)`: This function is used to set the values of the scaling factors **k_s** and **k_d** for source and destination respectively.

## The mathematics of alpha compositing

The following formula described in the Porter-Duff paper is used to get the final
composited pixel colors:

```
(R, G, B) = (R_s, G_s, B_s) * A_s + (R_d, G_d, B_d) * A_d * (1 - A_s)
A = A_s + A_d * (1 - A_s)
```

## Optimization: Opaque destination

Many times—for instance in a game frame—your background image is guaranteed to be
fully opaque, meaning **A_d = 1**. In this case, the formula becomes:

```
(R, G, B) = (R_s, G_s, B_s) * A_s + (R_d, G_d, B_d) * (1 - A_s)
```

In this case, you can set up OpenGL to do blending as follows:
```c
glBlendEquation(GL_FUNC_ADD);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

## Pragmatism interlude: What if my destination isn’t opaque

With **A_d ≠ 1**, the formula seemingly breaks the OpenGL API. It needs two multiplications
instead of one:  
- **A_d (1 - A_s)** and **A_s**, and one final division by **A**.

How do we achieve this?

**Pragmatic answer**: If you can get away with it performance-wise, write a shader that
samples two textures and renders to a third target texture with the composited result.  
The shader itself would look something like this:

```glsl
uniform sampler2D tex_dst;
uniform sampler2D tex_src;
varying vec2 frag_uv;

void main() {
    vec4 dst = texture2D(tex_dst, frag_uv);
    vec4 src = texture2D(tex_src, frag_uv);
    float final_alpha = src.a + dst.a * (1.0 - src.a);

    gl_FragColor = vec4(
        (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / final_alpha,
        final_alpha
    );
}
```

## Premultiplied alpha

To use hardware blending, even for non-opaque destinations, consider this formula:
```
(R, G, B, A) * A = (R_s, G_s, B_s) * A_s + (R_d, G_d, B_d) * A_d * (1 - A_s)
```

When loading the source image, multiply its color channels by its alpha. Do the same when
loading the destination image. Use the following settings:
```c
glBlendEquation(GL_FUNC_ADD);
glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
```

Finally, reverse the premultiplication process when rendering the composited result onto
another texture by dividing by the alpha:
```
R = R / A, G = G / A, B = B / A
```

---

For more details, check references:
- [Porter-Duff Paper](https://keithp.com/~keithp/porterduff/p253-porter.pdf)
- [Life of a Triangle](https://developer.nvidia.com/content/life-triangle-nvidias-logical-pipeline)
- [Alpha Blending Guide](https://developer.nvidia.com/content/alpha-blending-pre-or-not-pre)
```

Let me know if you'd like further formatting or updates.