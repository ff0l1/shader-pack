#pragma once

#include "pack/looks.hxx"

#include "Ports/Shaders/Shaders.hxx"

namespace pack {

enum class Family {
    Field,
    Light,
    Volume,
    Particles
};

struct Entry {
    const char* Id;
    const char* Name;
    Family Kind;
    const char* Note;
    const char* Body;
};

inline const char* familyName( Family Kind ) {
    if ( Kind == Family::Light )
        return "Light";
    if ( Kind == Family::Volume )
        return "Volume";
    if ( Kind == Family::Particles )
        return "Particles";
    return "Field";
}

inline constexpr Entry Live[] = {
    { "bends", "Color Bends", Family::Field, "Tight warped bands. Left-click pulls nearby.", looks::ColorBends },
    { "galaxy", "Galaxy", Family::Particles, "Layered star field with a slow spin.", looks::Galaxy },
    { "lightning", "Lightning", Family::Light, "Top-down strikes. A few bolts can fire at once.", looks::Lightning },
    { "rays", "Light Rays", Family::Light, "God rays from above a panel or hero.", looks::LightRays },
    { "waves", "Line Waves", Family::Field, "Dual-field ridges. Left-click pulls nearby lines.", looks::LineWaves },
    { "chrome", "Liquid Chrome", Family::Field, "Iterative cosine metal. Left-click pulls nearby.", looks::LiquidChrome },
    { "ether", "Liquid Ether", Family::Volume, "IQ double-warp filaments. Left-click pulls nearby.", looks::LiquidEther },
    { "silk", "Silk", Family::Field, "Satin bands. The expensive UI fill from React Bits / shadcn silk.", looks::Silk },
    { "particles", "Particles", Family::Particles, "Dots part on hover. Left-click pulls them in.", looks::Particles },
    { "snow", "Pixel Snow", Family::Particles, "Flakes part on hover. Left-click pulls them in.", looks::PixelSnow },
    { "rain", "Rain", Family::Particles, "Streaks only move. Left-click pulls nearby drops.", looks::Rain },
    { "plasma", "Plasma", Family::Field, "Classic sine plasma. Loud; tone down for chrome.", looks::Plasma },
    { "burst", "Prismatic Burst", Family::Light, "Sixteen shafts. Left-click pinches nearby.", looks::PrismaticBurst },
    { "aurora", "Aurora", Family::Volume, "Vertical curtains. Teal, green, violet on a night wash.", looks::Aurora },
    { "caustics", "Caustics", Family::Light, "Warped water lattice. Left-click pulls nearby.", looks::Caustics },
    { "ember", "Ember", Family::Particles, "Rising sparks. Heat and danger.", looks::Ember },
    { "fog", "Fog", Family::Volume, "Slow depth haze behind panels.", looks::Fog },
    { "iridescence", "Iridescence", Family::Field, "Thin-film oil sheen. Related to Chrome.", looks::Iridescence },
    { "smoke", "Smoke", Family::Volume, "Rising ash that fills the plate.", looks::Smoke },
    { "marble", "Marble", Family::Field, "Carrara veins that keep drifting. A polish travels.", looks::Marble },
    { "ripple", "Ripple", Family::Field, "Left-click rings stay under the cursor.", looks::Ripple },
    { "magnet", "Magnet", Family::Field, "Left-click pulls filaments in a small disk.", looks::Magnet },
    { "tunnel", "Tunnel", Family::Volume, "Shadertoy zoom tunnel. Left-click pinches nearby.", looks::Tunnel },
    { "warp", "Warp", Family::Particles, "Hyperspace streaks. Left-click pinches nearby.", looks::Warp },
    { "flame", "Flame", Family::Volume, "Rising fire. Left-click pulls a local lick.", looks::Flame },
    { "kaleido", "Kaleido", Family::Field, "Six-fold fold. Left-click brightens a local pull.", looks::Kaleido },
    { "cells", "Cells", Family::Field, "Voronoi cells. Left-click pulls nearby sites.", looks::Cells },
    { "water", "Water", Family::Field, "Layered swell. Left-click a local splash.", looks::Water },
    { "matrix", "Matrix", Family::Field, "Falling code rain. Thin columns, not blocks.", looks::Matrix },
    { "lava", "Lava", Family::Volume, "Cracked magma. Left-click pulls nearby glow.", looks::Lava },
    { "horror", "Horror Spiral", Family::Volume, "lsdlive/nextrix twisted menger. Left-click looks around.", looks::HorrorSpiral },
    { "creation", "Creation", Family::Field, "Silexars 1k intro. Left-click pulls nearby rings.", looks::Creation },
    { "starnest", "Star Nest", Family::Volume, "Kali kaliset volume. Left-click steers the view.", looks::StarNest },
    { "julia", "Julia", Family::Field, "Animated Julia set. Left-click pulls nearby.", looks::Julia },
    { "electric", "Electric", Family::Field, "Nimitz-style veins. Left-click pulls nearby.", looks::Electric },
    { "hexagons", "Hexagons", Family::Field, "Honeycomb lattice. Left-click pulls nearby cells.", looks::Hexagons },
    { "grain", "Grain", Family::Field, "Fine film grain. Static only, no pointer pan.", looks::Grain },
    { "scanlines", "Scanlines", Family::Field, "CRT phosphor bars. Overlay-style fill.", looks::Scanlines },
    { "flaring", "Flaring", Family::Light, "Corona around a star. Left-click pulls nearby fire.", looks::Flaring },
};

inline constexpr Entry Planned[] = {
    { "bloom", "Bloom", Family::Light, "Glow pass. Keep as an additive layer.", nullptr },
};

inline constexpr int LiveCount = ( int )( sizeof( Live ) / sizeof( Live[ 0 ] ) );
inline constexpr int PlannedCount = ( int )( sizeof( Planned ) / sizeof( Planned[ 0 ] ) );

inline unsigned int effect( int Index ) {
    static unsigned int Handles[ LiveCount ] = { };
    if ( Index < 0 || Index >= LiveCount )
        Index = 0;
    if ( Handles[ Index ] == 0 )
        Handles[ Index ] = Shaders->Compose( Live[ Index ].Name, Live[ Index ].Body );
    return Handles[ Index ];
}

inline void warm( ) {
    for ( int Index = 0; Index < LiveCount; Index++ )
        effect( Index );
}

}
