# Custom Shader Pack

A Windows previewer for UI shader looks. The rail on the left is the catalog. The stage on the right is the live fill. Click a row, or use the arrows.

![Preview](docs/preview.mp4)

<p align="center">
  <img src="docs/preview.png" alt="Custom Shader Pack">
</p>

## How it works

Each look is a short pixel-shader body in `include/pack/looks.hpp`. `include/pack/catalog.hpp` names them and points at those bodies. The preview splices the body into a Direct3D 11 fill and draws it twice: once on the stage, once on the thumbnail.

The pointer is in local space on the plate. Hover can part nearby particles, flakes, or rain. Left-click pulls a small disk under the cursor. A few looks use the click to look around. The whole image does not slide with the mouse.

| Key | Action |
| --- | --- |
| Esc | Quit |
| Arrows | Previous / next look |
| Home / End | First / last |

Wheel over the rail scrolls the list.

`preview.exe` is in [Releases](https://github.com/ff0l/Custom-Shader-Pack/releases).

## Looks

Thirty-nine fills. Field looks are warped bands and metal. Light looks are rays, burst, and flare. Volume looks are tunnels, fire, and fly-throughs. Particle looks are dots, snow, rain, and sparks.

Color Bends, Galaxy, Lightning, Light Rays, Line Waves, Liquid Chrome, Liquid Ether, Silk, Particles, Pixel Snow, Rain, Plasma, Prismatic Burst, Aurora, Caustics, Ember, Fog, Iridescence, Smoke, Marble, Ripple, Magnet, Tunnel, Warp, Flame, Kaleido, Cells, Water, Matrix, Lava, Horror Spiral, Creation, Star Nest, Julia, Electric, Hexagons, Grain, Scanlines, Flaring.

## Tree

```
include/pack/looks.hpp      shader bodies
include/pack/catalog.hpp    names and list
src/Main.cpp                preview window
docs/preview.png            screenshot
docs/preview.mp4            preview clip
```

## Build

Windows 10 SDK, MSVC, CMake 3.20, Ninja. Needs [custom-framework](https://github.com/ff0l/custom-framework) at `../etc/custom-framework`.

```
cmake --preset windows-release
cmake --build --preset windows-release
```

Run `build/windows-release/ShaderPack.exe`.
