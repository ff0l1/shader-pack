# Shader pack

A Windows previewer for UI shader fills. The left rail is the catalog. The right side is the live plate. Click a row, or use the arrows.

![Preview](docs/preview.mp4)

<p align="center">
  <img src="docs/preview.png" alt="Shader pack">
</p>

## How a look is drawn

Each look is a short pixel-shader body in `include/pack/looks.hxx`. `include/pack/catalog.hxx` names them. The preview splices the body into a Direct3D 11 fill and draws it twice: stage, then thumbnail.

The pointer is in local space on the plate. Hover can part particles, flakes, or rain. Left-click pulls a small disk under the cursor. A few looks use the click to look around. The whole image does not slide with the mouse.

| Key | |
| --- | --- |
| Esc | quit |
| Arrows | previous / next |
| Home / End | first / last |

The wheel over the rail scrolls the list.

There are thirty-nine fills: warped bands, metal, rays, tunnels, fire, snow, rain, sparks, and the rest listed in the catalog (Color Bends through Flaring).

## Build

Windows 10 SDK, MSVC, CMake 3.20, Ninja. Needs [ui-framework](https://github.com/ff0l1/ui-framework) checked out at `../custom-framework`.

```
cmake --preset windows-release
cmake --build --preset windows-release
```

Run `build/windows-release/ShaderPack.exe`.

## Files

```
include/pack/looks.hxx     shader bodies
include/pack/catalog.hxx   names
src/Main.cxx               window
docs/preview.png
docs/preview.mp4
```
