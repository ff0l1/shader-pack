#pragma once

namespace pack {
namespace looks {

inline constexpr const char* ColorBends = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.32;
Float2 Q = Uv;
Q /= 0.50 + 0.20 * dot( Q, Q );
Q += Float2( 0.20 * cos( T ) - 7.56, 0.10 * sin( T * 0.65 ) );
float Prox = exp( -dot( Uv - Aim, Uv - Aim ) * 14.0 );
Q -= ( ( Uv - Aim ) / max( length( Uv - Aim ), 0.001 ) ) * Prox * Held * 0.16;
for ( int J = 0; J < 5; J++ )
{
    Float2 R = sin( 1.5 * ( Q.yx * 1.85 ) + 2.0 * cos( Q * 1.85 ) );
    Q += ( R - Q ) * 0.15;
}
Float3 Sum = Float3( 0.0, 0.0, 0.0 );
Float3 Band[ 4 ];
Band[ 0 ] = Float3( 0.66, 0.22, 0.40 );
Band[ 1 ] = Float3( 0.16, 0.34, 0.72 );
Band[ 2 ] = Float3( 0.86, 0.58, 0.24 );
Band[ 3 ] = Float3( 0.20, 0.48, 0.44 );
for ( int I = 0; I < 4; I++ )
{
    Q -= Float2( 0.010, 0.007 );
    Float2 R = sin( 1.5 * ( Q.yx * 1.85 ) + 2.0 * cos( Q * 1.85 ) );
    Float2 Disp = ( R - Q ) * 0.55;
    Float2 Warped = Q + Disp;
    float M0 = length( R + sin( 5.0 * R.y * 1.85 - 3.0 * T + float( I ) ) * 0.25 );
    float M1 = length( Warped + sin( 5.0 * Warped.y * 1.85 - 3.0 * T + float( I ) ) * 0.25 );
    float M = Lerp( M0, M1, 0.72 );
    float W = 1.0 - exp( -1.55 / exp( 1.55 * M ) );
    Sum += Band[ I ] * W;
}
Final.rgb = Saturate( Sum );
)";

inline constexpr const char* Galaxy = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.07;
float Cs = cos( T );
float Sn = sin( T );
Uv = Float2( Uv.x * Cs - Uv.y * Sn, Uv.x * Sn + Uv.y * Cs );
float Wash = 0.5 + 0.5 * sin( Uv.x * 1.3 + T * 0.6 ) * sin( Uv.y * 1.05 - T * 0.45 );
Float3 Col = Float3( 0.010, 0.012, 0.028 ) + Float3( 0.045, 0.018, 0.070 ) * Wash * 0.55;
for ( int Layer = 0; Layer < 5; Layer++ )
{
    float Depth = Fract( float( Layer ) * 0.2 + T * 0.28 );
    float Scale = Lerp( 18.0, 3.4, Depth );
    float Fade = Depth * smoothstep( 1.0, 0.82, Depth );
    Float2 Grid = Uv * Scale + Float2( float( Layer ) * 37.1, float( Layer ) * 19.7 );
    Float2 Cell = floor( Grid );
    Float2 Fr = Fract( Grid ) - 0.5;
    for ( int Oy = -1; Oy <= 1; Oy++ )
    {
        for ( int Ox = -1; Ox <= 1; Ox++ )
        {
            Float2 Off = Float2( float( Ox ), float( Oy ) );
            Float2 Id = Cell + Off;
            float Seed = Fract( sin( dot( Id, Float2( 123.34, 456.21 ) ) ) * 43758.5453 );
            float Size = Fract( Seed * 345.32 );
            Float2 Jitter = Float2( Fract( Seed * 13.1 ), Fract( Seed * 27.7 ) ) - 0.5;
            Float2 Q = Fr - Off - Jitter * 0.62;
            float Dist = length( Q );
            float Core = ( 0.010 + 0.016 * Size ) / max( Dist, 0.00035 );
            float Spike = smoothstep( 0.0, 1.0, 1.0 - abs( Q.x * Q.y * 1400.0 ) );
            Float2 Rq = Float2( Q.x * 0.707 - Q.y * 0.707, Q.x * 0.707 + Q.y * 0.707 );
            Spike += 0.32 * smoothstep( 0.0, 1.0, 1.0 - abs( Rq.x * Rq.y * 1400.0 ) );
            float Flare = Spike * smoothstep( 0.72, 1.0, Size ) * 0.22;
            float Star = ( Core + Flare ) * smoothstep( 0.72, 0.10, Dist );
            float Twinkle = 0.72 + 0.28 * sin( Moment * ( 1.6 + Size * 2.2 ) + Seed * 6.283 );
            Float3 Tint = Float3( 0.70 + Fract( Seed * 3.1 ) * 0.30, 0.80, 1.0 - Fract( Seed * 5.7 ) * 0.22 );
            Col += Tint * Star * Twinkle * Fade * ( 0.35 + 0.75 * Size );
        }
    }
}
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Lightning = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
Uv.x *= 1.25;
float N = 0.0;
float Amp = 0.5;
Float2 P = Uv * 2.5 + Float2( Moment * 0.18, 0.0 );
for ( int I = 0; I < 8; I++ )
{
    Float2 Ip = floor( P );
    Float2 Fp = Fract( P );
    float Ha = Fract( sin( dot( Ip, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Hb = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Hc = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Hd = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    Float2 S = Fp * Fp * ( 3.0 - 2.0 * Fp );
    N += Amp * Lerp( Lerp( Ha, Hb, S.x ), Lerp( Hc, Hd, S.x ), S.y );
    float Rot = 0.45;
    P = Float2( P.x * cos( Rot ) - P.y * sin( Rot ), P.x * sin( Rot ) + P.y * cos( Rot ) ) * 2.0;
    Amp *= 0.5;
}
Float3 Col = Float3( 0.018, 0.022, 0.040 );
for ( int Bolt = 0; Bolt < 4; Bolt++ )
{
    float Bi = float( Bolt );
    float Seed = Fract( sin( Bi * 19.17 + 2.3 ) * 43758.5453 );
    float Speed = 0.28 + Seed * 0.22;
    float Tick = Moment * Speed + Seed * 4.1;
    float Phase = Fract( Tick );
    float Roll = Fract( sin( floor( Tick ) * 51.13 + Bi * 9.7 ) * 43758.5453 );
    float Live = step( Lerp( 0.12, 0.58, Saturate( Bi * 0.34 ) ), Roll );
    float Front = Lerp( -1.30, 1.40, Saturate( Phase / 0.17 ) );
    float Reveal = smoothstep( Front + 0.12, Front - 0.03, Uv.y );
    float Hold = 1.0 - smoothstep( 0.16, 0.40, Phase );
    float Flick = 0.50 + 0.50 * Fract( sin( Moment * 21.0 + Seed * 8.0 ) * 43758.5453 );
    float Shift = ( Seed - 0.5 ) * 1.35;
    float Twist = ( 2.0 * N - 1.0 ) * ( 0.20 + 0.12 * Seed ) + 0.07 * sin( Uv.y * 3.4 + Bi * 1.7 );
    float Dist = abs( Uv.x - Shift + Twist );
    float Core = pow( 0.016 / max( Dist, 0.0011 ), 1.28 );
    float Halo = pow( 0.050 / max( Dist, 0.0022 ), 0.82 );
    float Gain = Reveal * Hold * Flick * Live;
    Col += Float3( 0.78, 0.88, 1.0 ) * Saturate( Core * 1.15 * Gain );
    Col += Float3( 0.24, 0.38, 0.82 ) * Saturate( Halo * 0.28 * Gain );
}
Final.rgb = Saturate( Col );
)";

inline constexpr const char* LightRays = R"(
Float2 Uv = Local / max( Extent.y, 1.0 ) + Float2( 0.0, 0.35 );
Float2 Origin = Float2( 0.0, 1.15 );
Float2 Dir = normalize( Float2( 0.08, -1.0 ) );
Float2 To = Uv - Origin;
float Dist = length( To );
float CosA = dot( normalize( To ), Dir );
float Spread = pow( max( CosA + 0.08 * sin( Moment * 1.6 + Dist * 2.4 ), 0.0 ), 4.2 );
float Fall = Saturate( 1.0 - Dist * 0.42 );
float Pulse = 0.82 + 0.18 * sin( Moment * 2.1 );
float A = 0.45 + 0.15 * sin( CosA * 36.2 + Moment * 1.4 );
float B = 0.30 + 0.20 * cos( -CosA * 21.1 + Moment * 1.1 );
float Ray = ( A * 0.55 + B * 0.40 ) * Spread * Fall * Pulse;
float Lift = Saturate( 0.25 + ( 0.5 - Uv.y ) * 0.55 );
Float3 Tint = Float3( 0.22 + Lift * 0.55, 0.38 + Lift * 0.40, 0.62 + Lift * 0.28 );
Final.rgb = Saturate( Tint * Ray + Float3( 0.03, 0.04, 0.07 ) );
)";

inline constexpr const char* LineWaves = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 C = Local / max( Extent.y, 1.0 );
float T = Moment * 0.42;
float HT = T * 0.5;
float Pull = Held * 0.85 * exp( -dot( C - Aim, C - Aim ) * 12.0 );
float DAy = 0.20 * sin( C.y * 2.123 ) + 0.10 * sin( C.y * 3.234 + HT * 4.345 ) + 0.50 * sin( C.y * 0.589 + HT * 0.934 );
float DBy = 0.30 * sin( C.y * 1.345 ) + 0.20 * sin( C.y * 2.734 + HT * 3.345 ) + 0.30 * sin( C.y * 0.189 + HT * 0.934 );
float DAx = 0.20 * sin( C.x * cos( T ) * 1.235 * 2.123 ) + 0.10 * sin( C.x * cos( T ) * 1.235 * 3.234 + HT * 4.345 ) + 0.50 * sin( C.x * cos( T ) * 1.235 * 0.589 + HT * 0.934 );
float DBx = 0.30 * sin( C.x * sin( T ) * 1.235 * 1.345 ) + 0.20 * sin( C.x * sin( T ) * 1.235 * 2.734 + HT * 3.345 ) + 0.30 * sin( C.x * sin( T ) * 1.235 * 0.189 + HT * 0.934 );
Float2 Fa = Float2( C.x + DAy * 0.55 + Pull, C.y - DAx * 0.55 );
Float2 Fb = Float2( C.x + DBy * 0.55 + Pull, C.y - DBx * 0.55 );
float MixX = Saturate( Lerp( Fa.x, Fb.x, 0.5 ) * 0.5 + 0.5 );
float MixY = Saturate( Lerp( Fa.y, Fb.y, 0.5 ) * 0.5 + 0.5 );
Float2 Bl = Float2( Lerp( Fa.x, Fb.x, MixX ), Lerp( Fa.y, Fb.y, MixY ) );
float VMask = 1.0 - smoothstep( 0.62, 1.08, abs( Bl.y ) );
float Tile = Bl.y * Lerp( 6.5, 15.5, VMask );
float FrY = abs( Fract( Tile ) - 0.5 ) * 2.0;
float Lines = pow( 1.0 - FrY, 3.6 ) + 0.45 * pow( 1.0 - FrY, 8.0 );
float Ridge = pow( Saturate( cos( ( Tile + Bl.x * 0.35 ) * 3.141593 ) ), 6.0 );
float Pattern = VMask * ( Lines * 0.72 + Ridge * 0.38 );
float Cycle = T * 0.28;
Float3 ToneA = Float3( 0.20, 0.46, 0.78 );
Float3 ToneB = Float3( 0.78, 0.30, 0.44 );
Float3 ToneC = Float3( 0.86, 0.74, 0.30 );
Float3 Col = Float3( 0.04, 0.045, 0.06 );
Col += ToneA * Pattern * ( 0.55 + 0.35 * cos( Bl.y + Cycle ) );
Col += ToneB * Pattern * ( 0.32 + 0.28 * sin( Bl.x + Cycle * 1.7 ) );
Col += ToneC * Pattern * 0.16;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* LiquidChrome = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( min( Extent.x, Extent.y ), 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( min( Extent.x, Extent.y ), 1.0 );
float T = Moment * 0.55;
Float2 Delta = Uv - Aim;
float Prox = exp( -dot( Delta, Delta ) * 14.0 );
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * Prox * Held * 0.14;
for ( int I = 1; I < 8; I++ )
{
    float F = float( I );
    Uv.x += 0.55 / F * cos( F * 1.15 * Uv.y + T );
    Uv.y += 0.55 / F * cos( F * 1.35 * Uv.x + T );
}
float Shine = abs( sin( T - Uv.y - Uv.x ) );
Float3 Metal = Float3( 0.62, 0.68, 0.74 ) / max( Shine, 0.08 );
Final.rgb = Saturate( Metal * 0.42 );
)";

inline constexpr const char* LiquidEther = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 1.35;
Float2 Away = Uv - Aim * 1.35;
float Prox = exp( -dot( Away, Away ) * 8.0 );
Uv -= Away * Prox * Held * 0.16;
float Clock = Moment * 0.11;
Float2 Qx = Uv + Float2( Clock, 0.0 );
Float2 Qy = Uv + Float2( 5.2, 1.3 + Clock * 0.7 );
float Q1 = 0.0;
float Q2 = 0.0;
float Gain = 0.5;
Float2 Ix = Float2( 0.0, 0.0 );
Float2 Fx = Float2( 0.0, 0.0 );
Float2 Ux = Float2( 0.0, 0.0 );
Float2 Iy = Float2( 0.0, 0.0 );
Float2 Fy = Float2( 0.0, 0.0 );
Float2 Uy = Float2( 0.0, 0.0 );
float A00 = 0.0;
float A10 = 0.0;
float A01 = 0.0;
float A11 = 0.0;
float B00 = 0.0;
float B10 = 0.0;
float B01 = 0.0;
float B11 = 0.0;
for ( int Oa = 0; Oa < 4; Oa++ )
{
    Ix = floor( Qx );
    Fx = Fract( Qx );
    Ux = Fx * Fx * ( 3.0 - 2.0 * Fx );
    A00 = Fract( sin( dot( Ix, Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A10 = Fract( sin( dot( Ix + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A01 = Fract( sin( dot( Ix + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A11 = Fract( sin( dot( Ix + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    Q1 += Gain * Lerp( Lerp( A00, A10, Ux.x ), Lerp( A01, A11, Ux.x ), Ux.y );
    Iy = floor( Qy );
    Fy = Fract( Qy );
    Uy = Fy * Fy * ( 3.0 - 2.0 * Fy );
    B00 = Fract( sin( dot( Iy, Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B10 = Fract( sin( dot( Iy + Float2( 1.0, 0.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B01 = Fract( sin( dot( Iy + Float2( 0.0, 1.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B11 = Fract( sin( dot( Iy + Float2( 1.0, 1.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    Q2 += Gain * Lerp( Lerp( B00, B10, Uy.x ), Lerp( B01, B11, Uy.x ), Uy.y );
    Qx *= 2.02;
    Qy *= 2.02;
    Gain *= 0.5;
}
Float2 Warp = Float2( Q1, Q2 );
Float2 Rx = Uv + 4.0 * Warp + Float2( 1.7 + Clock * 0.4, 9.2 );
Float2 Ry = Uv + 4.0 * Warp + Float2( 8.3, 2.8 - Clock * 0.35 );
float R1 = 0.0;
float R2 = 0.0;
Gain = 0.5;
for ( int Ob = 0; Ob < 4; Ob++ )
{
    Ix = floor( Rx );
    Fx = Fract( Rx );
    Ux = Fx * Fx * ( 3.0 - 2.0 * Fx );
    A00 = Fract( sin( dot( Ix, Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A10 = Fract( sin( dot( Ix + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A01 = Fract( sin( dot( Ix + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    A11 = Fract( sin( dot( Ix + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    R1 += Gain * Lerp( Lerp( A00, A10, Ux.x ), Lerp( A01, A11, Ux.x ), Ux.y );
    Iy = floor( Ry );
    Fy = Fract( Ry );
    Uy = Fy * Fy * ( 3.0 - 2.0 * Fy );
    B00 = Fract( sin( dot( Iy, Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B10 = Fract( sin( dot( Iy + Float2( 1.0, 0.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B01 = Fract( sin( dot( Iy + Float2( 0.0, 1.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    B11 = Fract( sin( dot( Iy + Float2( 1.0, 1.0 ), Float2( 269.5, 183.3 ) ) ) * 43758.5453 );
    R2 += Gain * Lerp( Lerp( B00, B10, Uy.x ), Lerp( B01, B11, Uy.x ), Uy.y );
    Rx *= 2.02;
    Ry *= 2.02;
    Gain *= 0.5;
}
Float2 DensP = Uv + 4.0 * Float2( R1, R2 );
float Dens = 0.0;
Gain = 0.5;
for ( int Oc = 0; Oc < 4; Oc++ )
{
    Float2 Ip = floor( DensP );
    Float2 Fp = Fract( DensP );
    Float2 U = Fp * Fp * ( 3.0 - 2.0 * Fp );
    float N00 = Fract( sin( dot( Ip, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float N10 = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float N01 = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float N11 = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    Dens += Gain * Lerp( Lerp( N00, N10, U.x ), Lerp( N01, N11, U.x ), U.y );
    DensP *= 2.02;
    Gain *= 0.5;
}
float Filament = pow( Saturate( Dens ), 2.4 );
float Void = pow( Saturate( 1.0 - Dens * 1.15 ), 1.6 );
Float3 Deep = Float3( 0.012, 0.022, 0.038 );
Float3 Vein = Float3( 0.07, 0.38, 0.46 );
Float3 Flash = Float3( 0.62, 0.90, 0.84 );
Float3 Ink = Float3( 0.28, 0.10, 0.26 );
Float3 Col = Lerp( Deep, Vein, smoothstep( 0.28, 0.62, Dens ) );
Col = Lerp( Col, Ink, Saturate( length( Warp ) * 0.55 ) * Void );
Col = Lerp( Col, Flash, Filament * Saturate( R2 ) );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Silk = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.16;
float Wave = sin( Uv.x * 1.65 + Uv.y * 0.32 + T );
Wave += 0.55 * sin( Uv.x * 2.75 - Uv.y * 0.92 + T * 1.22 + 2.094 );
Wave += 0.30 * sin( Uv.y * 3.35 + Uv.x * 0.38 + T * 0.58 + 4.189 );
float Ramp = Saturate( Wave * 0.28 + 0.50 );
Float3 Stop0 = Float3( 0.07, 0.06, 0.08 );
Float3 Stop1 = Float3( 0.36, 0.20, 0.26 );
Float3 Stop2 = Float3( 0.68, 0.50, 0.40 );
Float3 Stop3 = Float3( 0.80, 0.76, 0.70 );
Float3 Stop4 = Float3( 0.13, 0.12, 0.15 );
float Slot = Ramp * 4.0;
Float3 Col = Lerp( Stop0, Stop1, Saturate( Slot ) );
Col = Lerp( Col, Stop2, Saturate( Slot - 1.0 ) );
Col = Lerp( Col, Stop3, Saturate( Slot - 2.0 ) );
Col = Lerp( Col, Stop4, Saturate( Slot - 3.0 ) );
float Sheen = pow( Saturate( 0.5 + 0.5 * sin( Uv.x * 5.4 + Wave * 2.6 + T * 0.8 ) ), 10.0 );
Col += Float3( 0.14, 0.12, 0.10 ) * Sheen;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Rain = R"(
Float2 AimPx = Mouse - ( Screen - Local );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 AimN = AimPx / max( Extent * 2.0, Float2( 1.0, 1.0 ) ) + 0.5;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
Float3 Col = Float3( 0.035, 0.042, 0.055 );
float Wind = 0.24;
for ( int Layer = 0; Layer < 4; Layer++ )
{
    float Scale = 12.0 + float( Layer ) * 9.0;
    float Speed = 0.50 + float( Layer ) * 0.38;
    Float2 Q;
    Q.x = Uv.x * Scale + Uv.y * Wind * Scale;
    Q.y = Uv.y * Scale * 0.32 + Moment * Speed;
    Float2 Cell = floor( Q );
    Float2 Fr = Fract( Q );
    float Seed = Fract( sin( dot( Cell, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Keep = step( 0.76 + float( Layer ) * 0.035, Seed );
    float CX = 0.22 + 0.56 * Fract( Seed * 13.0 );
    Float2 AimQ = Float2( AimN.x * Scale + AimN.y * Wind * Scale, AimN.y * Scale * 0.32 + Moment * Speed );
    Float2 Push = Float2( Cell.x + CX, Cell.y + Fr.y ) - AimQ;
    float Prox = exp( -dot( Push, Push ) * 0.85 );
    float Force = Hot * 0.16 - Held * 0.55;
    CX += ( Push.x / max( abs( Push.x ) + abs( Push.y ), 0.001 ) ) * Prox * Force;
    float Streak = smoothstep( 0.050, 0.0, abs( Fr.x - CX ) );
    float Len = 0.28 + 0.58 * Fract( Seed * 7.0 );
    float Head = smoothstep( 0.0, 0.07, Fr.y ) * ( 1.0 - smoothstep( Len, Len + 0.14, Fr.y ) );
    Col += Float3( 0.58, 0.66, 0.76 ) * Streak * Head * Keep * ( 0.22 + 0.18 * float( Layer ) );
}
Col += Float3( 0.07, 0.09, 0.12 ) * pow( Uv.y, 2.4 ) * 0.45;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Particles = R"(
Float2 AimPx = Mouse - ( Screen - Local );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 AimN = AimPx / max( Extent * 2.0, Float2( 1.0, 1.0 ) ) + 0.5;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
Float3 Col = Float3( 0.04, 0.045, 0.06 );
for ( int Layer = 0; Layer < 4; Layer++ )
{
    float Depth = 6.0 + float( Layer ) * 5.5;
    float DriftT = Moment * ( 0.12 + float( Layer ) * 0.05 );
    Float2 Drift = Float2( sin( Moment * 0.28 + float( Layer ) ) * 0.40, sin( DriftT + float( Layer ) * 1.7 ) * 0.35 );
    Float2 Q = Uv * Depth + Drift;
    Float2 Cell = floor( Q );
    Float2 Fr = Fract( Q );
    float Seed = Fract( sin( dot( Cell, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Keep = step( 0.72 + float( Layer ) * 0.05, Seed );
    Float2 Center = Float2( 0.30 + 0.40 * Fract( Seed * 13.0 ), 0.30 + 0.40 * Fract( Seed * 7.0 ) );
    Float2 World = ( Cell + Center - Drift ) / Depth;
    Float2 Push = World - AimN;
    float Prox = exp( -dot( Push, Push ) * 32.0 );
    float Force = Hot * 0.16 - Held * 0.52;
    Center += ( Push / max( length( Push ), 0.001 ) ) * Prox * Force;
    float Dist = length( Fr - Center );
    float Soft = Saturate( 1.0 - Dist * ( 16.0 - float( Layer ) * 2.0 ) );
    float Dot = pow( Soft, 1.8 ) * Keep;
    Float3 Tint = Float3( 0.55, 0.72, 1.0 ) + 0.25 * sin( Float3( Seed, Seed * 1.7, Seed * 2.3 ) * 6.28 + Moment );
    Col += Tint * Dot * ( 0.40 + 0.22 * float( Layer ) );
}
Final.rgb = Saturate( Col );
)";

inline constexpr const char* PixelSnow = R"(
Float2 AimPx = Mouse - ( Screen - Local );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 AimN = AimPx / max( Extent * 2.0, Float2( 1.0, 1.0 ) ) + 0.5;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
Float3 Col = Float3( 0.035, 0.040, 0.055 );
for ( int Layer = 0; Layer < 3; Layer++ )
{
    float Depth = 8.0 + float( Layer ) * 9.0;
    float Fall = Moment * ( 0.09 + float( Layer ) * 0.05 );
    Float2 Drift = Float2( sin( Moment * 0.30 + float( Layer ) ) * 0.35, -Fall );
    Float2 Q = Uv * Depth + Drift;
    Float2 Cell = floor( Q );
    Float2 Fr = Fract( Q );
    float Seed = Fract( sin( dot( Cell, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Keep = step( 0.82 + float( Layer ) * 0.04, Seed );
    Float2 Center = Float2( 0.35 + 0.30 * Fract( Seed * 13.0 ), 0.35 + 0.30 * Fract( Seed * 7.0 ) );
    Float2 World = ( Cell + Center - Drift ) / Depth;
    Float2 Push = World - AimN;
    float Prox = exp( -dot( Push, Push ) * 30.0 );
    float Force = Hot * 0.16 - Held * 0.52;
    Center += ( Push / max( length( Push ), 0.001 ) ) * Prox * Force;
    float Dist = length( Fr - Center );
    float Soft = Saturate( 1.0 - Dist * ( 14.0 - float( Layer ) * 2.0 ) );
    float Flake = pow( Soft, 1.6 ) * Keep;
    Col += Float3( 0.86, 0.90, 1.0 ) * Flake * ( 0.35 + 0.25 * float( Layer ) );
}
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Plasma = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.45;
float A = sin( Uv.x * 3.2 + T );
float B = sin( Uv.y * 2.7 - T * 0.85 );
float C = sin( ( Uv.x + Uv.y ) * 2.1 + T * 0.6 );
float D = sin( length( Uv ) * 4.4 - T );
float Mix = A + B + C * 0.65 + D * 0.45;
Float3 Col = Float3(
    0.5 + 0.5 * sin( Mix + 0.0 ),
    0.5 + 0.5 * sin( Mix + 2.1 ),
    0.5 + 0.5 * sin( Mix + 4.2 )
);
Final.rgb = Saturate( Col * 0.72 + Float3( 0.04, 0.03, 0.06 ) );
)";

inline constexpr const char* PrismaticBurst = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 );
float Rad = length( Uv );
float Ang = atan2( Uv.y, Uv.x );
float T = Moment * 0.38;
Float3 Col = Float3( 0.016, 0.018, 0.030 );
for ( int K = 0; K < 16; K++ )
{
    float Seed = Fract( sin( float( K ) * 17.13 + 3.7 ) * 43758.5453 );
    float Sign = step( 0.5, Fract( Seed * 9.0 ) ) * 2.0 - 1.0;
    float Spin = T * ( 0.20 + Seed * 0.62 ) * Sign;
    float Drift = 0.42 * sin( T * ( 0.65 + Seed * 1.25 ) + Seed * 6.28 );
    float Center = Seed * 6.283185 + Spin + Drift;
    float Delta = Ang - Center;
    Delta = Delta - 6.283185 * floor( ( Delta + 3.141593 ) / 6.283185 );
    float Wobble = 0.18 * sin( Rad * ( 3.8 + Seed * 10.0 ) + T * ( 2.4 + Seed * 3.1 ) + Seed * 8.0 );
    Wobble += 0.07 * sin( Rad * 16.0 - T * 3.6 + float( K ) );
    float Breath = 0.58 + 0.42 * sin( T * ( 1.9 + Seed * 2.6 ) + Seed * 4.2 );
    float Tight = Lerp( 10.0, 24.0, Seed ) * Breath;
    float Shaft = exp( -abs( Delta + Wobble ) * Tight );
    float Fork = exp( -abs( Delta + Wobble * 1.35 + 0.038 * Sign ) * ( Tight * 1.55 ) );
    float Pulse = 0.50 + 0.50 * sin( T * ( 1.4 + Seed * 2.0 ) + Rad * 3.4 );
    float Travel = Fract( T * ( 0.18 + Seed * 0.16 ) + Seed );
    float Along = smoothstep( 0.015, 0.11, Rad ) * ( 1.0 - smoothstep( 0.70 + Travel * 0.40, 1.38, Rad ) );
    float Live = 0.40 + 0.60 * step( 0.16, Fract( sin( floor( T * 1.35 + Seed * 8.0 ) * 19.1 ) * 43758.5453 ) );
    Float3 Tint = Lerp( Float3( 0.62, 0.74, 0.92 ), Float3( 0.78, 0.42, 0.38 ), Fract( Seed * 3.1 ) );
    Tint = Lerp( Tint, Float3( 0.90, 0.82, 0.55 ), Fract( Seed * 7.7 ) * 0.35 );
    Col += Tint * ( Shaft * 0.82 + Fork * 0.38 ) * Along * Pulse * Live * ( 0.20 + 0.30 * Seed );
}
float Pinch = exp( -dot( Uv - Aim, Uv - Aim ) * 10.0 ) * Held;
Col += Float3( 0.72, 0.78, 0.92 ) * Pinch * 0.22;
Col += Float3( 0.34, 0.42, 0.58 ) * exp( -Rad * 7.5 ) * ( 0.10 + 0.07 * sin( T * 2.2 ) );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Aurora = R"(
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
float T = Moment * 0.12;
float Curtain = 0.0;
Curtain += 0.55 * sin( Uv.x * 3.4 + T );
Curtain += 0.30 * sin( Uv.x * 6.1 - T * 0.7 );
Curtain += 0.18 * sin( Uv.x * 9.7 + Uv.y * 1.2 + T * 1.3 );
float Height = 0.22 + 0.55 * ( 0.5 + 0.5 * Curtain );
float Sheet = Saturate( 1.0 - abs( Uv.y - ( 0.38 + Curtain * 0.08 ) ) / Height );
Sheet = pow( Sheet, 1.8 );
float Band = pow( Saturate( 1.0 - abs( Uv.y - 0.42 + Curtain * 0.06 ) * 2.4 ), 2.2 );
Float3 Deep = Float3( 0.02, 0.03, 0.06 );
Float3 Teal = Float3( 0.12, 0.55, 0.48 );
Float3 Green = Float3( 0.28, 0.72, 0.38 );
Float3 Violet = Float3( 0.42, 0.22, 0.72 );
Float3 Col = Deep;
Col = Lerp( Col, Teal, Sheet * 0.65 );
Col = Lerp( Col, Green, Band * Saturate( 0.4 + Curtain * 0.4 ) );
Col = Lerp( Col, Violet, pow( Saturate( Uv.y * 0.7 + Curtain * 0.15 ), 2.4 ) * Sheet * 0.45 );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Caustics = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.22;
Float2 P = Uv * 2.4;
Float2 Delta = Uv - Aim;
P -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 12.0 ) * Held * 0.35;
float A = 0.0;
float B = 0.0;
for ( int I = 0; I < 5; I++ )
{
    float F = float( I + 1 );
    A += sin( P.x * ( 1.6 + F * 0.35 ) + P.y * 0.4 + T * ( 0.7 + F * 0.15 ) ) / F;
    B += cos( P.y * ( 1.5 + F * 0.32 ) - P.x * 0.35 - T * ( 0.55 + F * 0.12 ) ) / F;
    P = Float2( P.x * 1.15 - P.y * 0.22, P.x * 0.22 + P.y * 1.15 );
}
float Lattice = pow( Saturate( 1.0 - abs( A * B ) * 1.8 ), 3.2 );
float Soft = pow( Saturate( 0.55 + 0.45 * A ), 2.4 );
Float3 Deep = Float3( 0.02, 0.04, 0.07 );
Float3 Light = Float3( 0.55, 0.82, 0.88 );
float Pool = exp( -dot( Uv - Aim, Uv - Aim ) * 10.0 ) * Held;
Final.rgb = Saturate( Deep + Light * ( Lattice * 0.85 + Soft * 0.18 + Pool * 0.18 ) );
)";

inline constexpr const char* Ember = R"(
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
Float3 Col = Float3( 0.04, 0.018, 0.012 );
for ( int Layer = 0; Layer < 4; Layer++ )
{
    float Depth = 7.0 + float( Layer ) * 6.0;
    float Drift = Moment * ( 0.14 + float( Layer ) * 0.06 );
    Float2 Q = Uv * Depth;
    Q.x += sin( Moment * 0.35 + float( Layer ) ) * 0.30;
    Q.y += Drift;
    Float2 Cell = floor( Q );
    Float2 Fr = Fract( Q );
    float Seed = Fract( sin( dot( Cell, Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
    float Keep = step( 0.74 + float( Layer ) * 0.04, Seed );
    Float2 Center = Float2( 0.30 + 0.40 * Fract( Seed * 13.0 ), 0.25 + 0.45 * Fract( Seed * 7.0 ) );
    float Dist = length( Fr - Center );
    float Soft = Saturate( 1.0 - Dist * ( 18.0 - float( Layer ) * 2.4 ) );
    float Spark = pow( Soft, 2.1 ) * Keep;
    float Fade = 1.0 - pow( Uv.y, 1.6 );
    Float3 Tint = Lerp( Float3( 0.85, 0.22, 0.06 ), Float3( 1.0, 0.72, 0.22 ), Fract( Seed * 5.0 ) );
    Col += Tint * Spark * Fade * ( 0.40 + 0.22 * float( Layer ) );
}
Col += Float3( 0.18, 0.04, 0.01 ) * pow( 1.0 - Uv.y, 2.8 ) * 0.35;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Fog = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.06;
float N = 0.0;
float Amp = 0.52;
Float2 P = Uv * 1.6 + Float2( T * 0.35, T * 0.12 );
for ( int I = 0; I < 5; I++ )
{
    Float2 Ip = floor( P );
    Float2 Fp = Fract( P );
    Float2 U = Fp * Fp * ( 3.0 - 2.0 * Fp );
    float A = Fract( sin( dot( Ip, Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float B = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float C = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float D = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    N += Amp * Lerp( Lerp( A, B, U.x ), Lerp( C, D, U.x ), U.y );
    P = P * 2.07 + Float2( 8.0, 3.0 );
    Amp *= 0.5;
}
float Soft = pow( Saturate( N * 1.15 ), 1.45 );
Float3 Deep = Float3( 0.06, 0.08, 0.11 );
Float3 Mist = Float3( 0.42, 0.48, 0.54 );
Final.rgb = Saturate( Lerp( Deep, Mist, Soft ) );
)";

inline constexpr const char* Iridescence = R"(
Float2 Uv = Local / max( min( Extent.x, Extent.y ), 1.0 );
float T = Moment * 0.20;
float Film = sin( Uv.x * 4.2 + Uv.y * 2.6 + T );
Film += 0.45 * sin( length( Uv ) * 7.5 - T * 1.4 );
float Hue = Fract( Film * 0.18 + T * 0.04 );
Float3 Spec = 0.5 + 0.5 * cos( 6.283185 * Hue + Float3( 0.0, 2.094, 4.189 ) );
float Gloss = pow( Saturate( 0.55 + 0.45 * Film ), 3.4 );
Float3 Base = Float3( 0.08, 0.07, 0.10 );
Final.rgb = Saturate( Base + Spec * Gloss * 0.72 );
)";

inline constexpr const char* Smoke = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.08;
Float2 P = Uv * 2.6 + Float2( sin( T * 0.35 ) * 0.30, -T * 0.32 );
float N = 0.0;
float Amp = 0.55;
for ( int I = 0; I < 5; I++ )
{
    Float2 Ip = floor( P );
    Float2 Fp = Fract( P );
    Float2 U = Fp * Fp * ( 3.0 - 2.0 * Fp );
    float A = Fract( sin( dot( Ip, Float2( 41.2, 93.7 ) ) ) * 43758.5453 );
    float B = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 41.2, 93.7 ) ) ) * 43758.5453 );
    float C = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 41.2, 93.7 ) ) ) * 43758.5453 );
    float D = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 41.2, 93.7 ) ) ) * 43758.5453 );
    N += Amp * Lerp( Lerp( A, B, U.x ), Lerp( C, D, U.x ), U.y );
    P = Float2( P.x * 1.65 - P.y * 0.28, P.x * 0.28 + P.y * 1.65 ) + Float2( 3.1, 1.7 );
    Amp *= 0.5;
}
float Plume = pow( Saturate( N * 1.15 ), 1.55 );
float Lift = Saturate( 0.88 - Uv.y * 0.14 );
Float3 Deep = Float3( 0.04, 0.04, 0.05 );
Float3 Ash = Float3( 0.40, 0.40, 0.42 );
Final.rgb = Saturate( Lerp( Deep, Ash, Plume * Lift ) );
)";

inline constexpr const char* Marble = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.09;
Float2 P = Uv * 2.05;
P += 0.42 * Float2( sin( P.y * 1.7 + T ), cos( P.x * 1.45 - T * 0.85 ) );
P += 0.22 * Float2( sin( P.y * 3.4 - T * 0.55 ), cos( P.x * 2.9 + T * 0.40 ) );
P += 0.08 * Float2( sin( P.y * 7.0 + T * 0.9 ), cos( P.x * 6.2 - T * 0.7 ) );
float Vein = abs( sin( P.x * 2.35 + sin( P.y * 3.05 ) * 1.35 ) );
Vein *= abs( sin( P.y * 1.55 + cos( P.x * 2.15 ) * 0.85 ) );
Vein = pow( 1.0 - Saturate( Vein ), 9.5 );
float Fine = pow( 1.0 - Saturate( abs( sin( P.x * 7.2 + P.y * 2.8 + T * 0.6 ) ) ), 18.0 );
float Drift = 0.5 + 0.5 * sin( P.x * 1.1 - T * 0.7 + P.y * 0.35 );
float Spec = pow( Saturate( Drift ), 14.0 );
float Grain = Fract( sin( dot( Uv * 42.0 + T * 0.2, Float2( 12.9, 78.2 ) ) ) * 43758.5453 );
Float3 Stone = Float3( 0.76, 0.72, 0.67 );
Float3 Ink = Float3( 0.14, 0.12, 0.13 );
Float3 Umber = Float3( 0.42, 0.32, 0.24 );
Float3 Col = Lerp( Stone, Ink, Vein * 0.88 );
Col = Lerp( Col, Umber, Fine * 0.35 );
Col += Float3( 0.90, 0.88, 0.82 ) * Spec * 0.16;
Col -= Grain * 0.035;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Ripple = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 );
float Dist = length( Uv - Aim );
float Wave = sin( Dist * 26.0 - Moment * 7.2 );
float Ring = exp( -abs( Wave ) * 3.6 ) * exp( -Dist * 1.35 );
float Soft = exp( -Dist * Dist * 8.0 );
float Quiet = 0.5 + 0.5 * sin( length( Uv ) * 10.0 - Moment * 1.4 );
Float3 Deep = Float3( 0.04, 0.055, 0.07 );
Float3 Crest = Float3( 0.62, 0.78, 0.86 );
Float3 Col = Deep + Crest * Quiet * 0.05;
Col += Crest * ( Ring * 0.70 + Soft * 0.28 ) * Held;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Magnet = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 );
Float2 Delta = Uv - Aim;
float Dist = length( Delta );
Float2 Dir = Delta / max( Dist, 0.001 );
float Pull = Held * exp( -Dist * Dist * 10.0 );
Uv -= Dir * Pull * 0.18;
float T = Moment * 0.28;
float Band = 0.0;
for ( int M = 0; M < 5; M++ )
{
    float F = float( M + 1 );
    Band += sin( Uv.x * ( 3.2 + F * 0.7 ) + Uv.y * ( 2.1 - F * 0.25 ) + T * ( 0.6 + F * 0.12 ) ) / F;
}
float Filament = pow( Saturate( 0.55 + 0.45 * Band ), 3.2 );
float Core = exp( -Dist * Dist * 6.5 ) * Hot;
Float3 Deep = Float3( 0.05, 0.04, 0.07 );
Float3 Vein = Float3( 0.62, 0.42, 0.58 );
Float3 HotTint = Float3( 0.85, 0.72, 0.48 );
Float3 Col = Deep + Vein * Filament * 0.55;
Col += HotTint * Core * 0.45;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Tunnel = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 );
Float2 Delta = Uv - Aim;
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 12.0 ) * Held * 0.14;
float T = Moment * 0.20;
float Ang = atan2( Uv.y, Uv.x );
float Rad = length( Uv );
float Z = 0.22 / max( Rad, 0.035 ) + T;
float Rings = abs( sin( Z * 7.0 ) );
float Rib = abs( sin( Ang * 8.0 + Z * 1.4 ) );
float Shade = pow( Saturate( Rings * 0.65 + Rib * 0.35 ), 2.2 );
float Fog = Saturate( Rad * 1.15 );
Float3 Deep = Float3( 0.02, 0.025, 0.04 );
Float3 Wall = Float3( 0.42, 0.48, 0.58 );
Float3 Core = Float3( 0.78, 0.82, 0.90 );
Float3 Col = Lerp( Deep, Wall, Shade * Fog );
Col = Lerp( Col, Core, pow( Saturate( 1.0 - Rad * 2.2 ), 3.4 ) * 0.35 );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Warp = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
Float2 Delta = Uv - Aim;
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 12.0 ) * Held * 0.14;
float T = Moment * 0.32;
float Ang = atan2( Uv.y, Uv.x );
float Rad = length( Uv );
float Travel = Fract( 0.18 / max( Rad, 0.02 ) - T );
float Spoke = pow( Fract( sin( Ang * 36.0 ) * 43758.5453 ), 14.0 );
float Streak = Spoke * smoothstep( 0.04, 0.18, Rad ) * ( 1.0 - smoothstep( 0.95, 1.45, Rad ) );
Streak *= 0.35 + 0.65 * Travel;
Float3 Deep = Float3( 0.010, 0.012, 0.022 );
Float3 Star = Float3( 0.78, 0.84, 0.96 );
Final.rgb = Saturate( Deep + Star * Streak * 0.95 );
)";

inline constexpr const char* Flame = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
float Alt = Press.y;
Float2 Uv = Local / max( Extent.y, 1.0 );
Float2 Delta = Uv - Aim;
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 12.0 ) * Held * 0.12;
float T = Moment * 0.22;
float N = 0.0;
float Amp = 0.55;
Float2 P = Uv * 2.4 + Float2( 0.0, -T * 0.85 );
for ( int Fi = 0; Fi < 5; Fi++ )
{
    Float2 Ip = floor( P );
    Float2 Fp = Fract( P );
    Float2 U = Fp * Fp * ( 3.0 - 2.0 * Fp );
    float A = Fract( sin( dot( Ip, Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float B = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float C = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    float D = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 27.1, 61.7 ) ) ) * 43758.5453 );
    N += Amp * Lerp( Lerp( A, B, U.x ), Lerp( C, D, U.x ), U.y );
    P = P * 2.05 + Float2( 3.1, 1.4 );
    Amp *= 0.5;
}
float Shape = Saturate( 0.55 - Uv.y * 0.42 - abs( Uv.x ) * 0.55 + N * 0.55 );
Shape = pow( Shape, 1.45 );
Shape += Held * exp( -dot( Uv - Aim, Uv - Aim ) * 10.0 ) * 0.18;
Float3 Deep = Float3( 0.04, 0.018, 0.012 );
Float3 Red = Float3( 0.72, 0.16, 0.04 );
Float3 Gold = Float3( 0.95, 0.72, 0.22 );
Float3 Col = Deep;
Col = Lerp( Col, Red, smoothstep( 0.12, 0.42, Shape ) );
Col = Lerp( Col, Gold, pow( Saturate( Shape ), 3.2 ) );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Kaleido = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.14;
float Ang = atan2( Uv.y, Uv.x ) - T * 0.35;
float Rad = length( Uv );
float Fold = 6.283185 / 6.0;
Ang = abs( Fract( Ang / Fold + 0.5 ) - 0.5 ) * Fold;
Float2 P = Float2( cos( Ang ), sin( Ang ) ) * Rad;
P += 0.18 * Float2( sin( P.y * 3.2 + T ), cos( P.x * 2.8 - T ) );
float Band = 0.5 + 0.5 * sin( P.x * 6.0 + P.y * 4.0 + T * 1.4 );
float Vein = pow( abs( sin( P.x * 8.0 - T ) ), 4.0 );
Float3 Deep = Float3( 0.05, 0.04, 0.07 );
Float3 A = Float3( 0.52, 0.28, 0.40 );
Float3 B = Float3( 0.22, 0.42, 0.58 );
Float3 Col = Lerp( Deep, A, Band * 0.65 );
Col = Lerp( Col, B, Vein * 0.40 );
Col += Float3( 0.80, 0.78, 0.70 ) * pow( Saturate( 0.55 + 0.45 * Band ), 10.0 ) * 0.12;
Col += Float3( 0.80, 0.78, 0.70 ) * exp( -dot( Uv - Aim, Uv - Aim ) * 12.0 ) * Held * 0.18;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Cells = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 4.2;
Float2 AimC = Aim * 4.2;
Float2 Delta = Uv - AimC;
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 3.5 ) * Held * 0.45;
float T = Moment * 0.12;
Float2 Cell = floor( Uv );
Float2 Fr = Fract( Uv );
float Best = 8.0;
float Second = 8.0;
for ( int Oy = -1; Oy <= 1; Oy++ )
{
    for ( int Ox = -1; Ox <= 1; Ox++ )
    {
        Float2 Off = Float2( float( Ox ), float( Oy ) );
        Float2 Id = Cell + Off;
        float Seed = Fract( sin( dot( Id, Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
        Float2 Point = Off + Float2( Fract( Seed * 13.1 + T ), Fract( Seed * 27.7 - T ) ) - Fr;
        float Dist = length( Point );
        if ( Dist < Best )
        {
            Second = Best;
            Best = Dist;
        }
        else if ( Dist < Second )
        {
            Second = Dist;
        }
    }
}
float Edge = Saturate( ( Second - Best ) * 3.4 );
Float3 Deep = Float3( 0.06, 0.07, 0.09 );
Float3 Fill = Float3( 0.22, 0.28, 0.34 );
Float3 Line = Float3( 0.70, 0.76, 0.82 );
Float3 Col = Lerp( Deep, Fill, Saturate( Best ) );
Col = Lerp( Col, Line, pow( 1.0 - Edge, 6.0 ) * 0.55 );
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Water = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment * 0.28;
float Wave = 0.0;
Wave += 0.45 * sin( Uv.x * 3.4 + Uv.y * 1.2 + T );
Wave += 0.28 * sin( Uv.x * 5.8 - Uv.y * 2.1 - T * 0.8 );
Wave += 0.16 * sin( Uv.y * 7.0 + T * 1.3 );
float Dist = length( Uv - Aim );
float Splash = sin( Dist * 22.0 - T * 8.0 ) * exp( -Dist * Dist * 8.0 ) * Held * 0.70;
float Height = Wave + Splash;
float Spec = pow( Saturate( 0.55 + 0.45 * Height ), 8.0 );
Float3 Deep = Float3( 0.04, 0.08, 0.12 );
Float3 Foam = Float3( 0.55, 0.72, 0.78 );
Final.rgb = Saturate( Deep + Foam * ( 0.22 + Height * 0.18 + Spec * 0.28 ) );
)";

inline constexpr const char* Matrix = R"(
Float2 Uv = Local / max( Extent.y, 1.0 ) * 0.5 + 0.5;
float T = Moment * 0.55;
float Cols = 28.0;
float ColX = floor( Uv.x * Cols );
float Seed = Fract( sin( ColX * 19.13 ) * 43758.5453 );
float Speed = 0.35 + Seed * 0.85;
float Head = Fract( Uv.y + T * Speed + Seed );
float Glyph = pow( Fract( sin( ( ColX + floor( ( Uv.y + T * Speed ) * 18.0 ) ) * 12.9898 ) * 43758.5453 ), 3.4 );
float Trail = pow( 1.0 - Head, 2.4 );
float Line = smoothstep( 0.018, 0.0, abs( Fract( Uv.x * Cols ) - 0.5 ) );
Float3 Deep = Float3( 0.02, 0.03, 0.025 );
Float3 Green = Float3( 0.22, 0.72, 0.38 );
Float3 Bright = Float3( 0.70, 0.95, 0.72 );
Float3 Col = Deep;
Col += Lerp( Green, Bright, pow( Trail, 4.0 ) ) * Glyph * Trail * Line * 0.85;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* Lava = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Hot = step( abs( AimPx.x ), Extent.x + 2.0 ) * step( abs( AimPx.y ), Extent.y + 2.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
Float2 Delta = Uv - Aim;
Uv -= ( Delta / max( length( Delta ), 0.001 ) ) * exp( -dot( Delta, Delta ) * 12.0 ) * Held * 0.12;
float T = Moment * 0.10;
Float2 P = Uv * 2.0;
P += 0.35 * Float2( sin( P.y * 1.8 + T ), cos( P.x * 1.6 - T ) );
P += 0.18 * Float2( sin( P.y * 3.6 - T * 0.7 ), cos( P.x * 3.1 + T * 0.5 ) );
float Crust = abs( sin( P.x * 2.6 + sin( P.y * 2.2 ) ) );
Crust = pow( 1.0 - Saturate( Crust ), 4.5 );
float Glow = pow( Saturate( 0.55 + 0.45 * sin( P.x * 3.0 - T + P.y ) ), 2.4 );
Float3 Rock = Float3( 0.08, 0.05, 0.04 );
Float3 Magma = Float3( 0.82, 0.22, 0.05 );
Float3 Gold = Float3( 0.95, 0.70, 0.18 );
Float3 Col = Lerp( Rock, Magma, Glow * 0.75 );
Col = Lerp( Col, Gold, Crust * 0.55 );
Col += Gold * exp( -dot( Uv - Aim, Uv - Aim ) * 12.0 ) * Held * 0.20;
Final.rgb = Saturate( Col );
)";

inline constexpr const char* HorrorSpiral = R"(
Float2 AimPx = Mouse - ( Screen - Local );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y * 2.0, 1.0 );
float T = Moment;
Float3 Ro = Float3( 0.10 * cos( T ), 0.0, -T );
Ro.x += Held * ( -AimPx.x * 0.01 );
Ro.y += Held * ( -AimPx.y * 0.01 );
Float3 Rd = Float3( Uv.x, Uv.y, -1.0 );
Rd = Rd / max( length( Rd ), 0.001 );
Float3 Pos = Ro;
float Hit = 0.0;
for ( int Hs = 0; Hs < 100; Hs++ )
{
    Hit = float( Hs ) * 0.01;
    Float3 Q = Pos;
    Q.y += cos( T * 2.0 ) * 0.20;
    float Tw = T + Q.z;
    float Tc = cos( Tw );
    float Ts = sin( Tw );
    Q = Float3( Q.x * Tc - Q.y * Ts, Q.x * Ts + Q.y * Tc, Q.z );
    float Dist = 0.0;
    float Grow = 1.0;
    for ( int Hm = 0; Hm < 3; Hm++ )
    {
        Float3 W = Q * Grow + 1.0;
        Float3 Tile = abs( W - 2.0 * floor( W * 0.5 ) - 1.0 );
        Float3 Cross = max( Tile, Tile.yzx );
        Dist = max( Dist, ( 0.90 - min( Cross.x, min( Cross.y, Cross.z ) ) ) / Grow );
        Grow *= 3.0;
    }
    if ( Dist < 0.0001 )
        break;
    Pos += Rd * Dist * 0.40;
}
Hit /= 0.40 * sqrt( max( abs( tan( T ) + Pos.x * Pos.x + Pos.y * Pos.y ), 0.00001 ) );
Float3 Tint = Lerp( Float3( 0.10, 0.10, 0.30 ), Float3( 0.70, 0.10, 0.30 ), Hit * sin( Pos.z ) );
Final.rgb = Saturate( Tint );
)";

inline constexpr const char* Creation = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv01 = Local / max( Extent * 2.0, Float2( 1.0, 1.0 ) ) + 0.5;
Float2 Base = Uv01 - 0.5;
Base.x *= Extent.x / max( Extent.y, 1.0 );
float Len = max( length( Base ), 0.001 );
Float2 Pull = ( ( Base - Aim ) / max( length( Base - Aim ), 0.001 ) ) * exp( -dot( Base - Aim, Base - Aim ) * 14.0 ) * Held * 0.10;
float Z = Moment;
Float3 Acc = Float3( 0.0, 0.0, 0.0 );
Z += 0.07;
Float2 P0 = Base - Pull;
float L0 = max( length( P0 ), 0.001 );
Float2 U0 = Uv01 + P0 / L0 * ( sin( Z ) + 1.0 ) * abs( sin( L0 * 9.0 - Z - Z ) );
Acc.x = 0.01 / max( length( Fract( U0 ) - 0.5 ), 0.0008 );
Z += 0.07;
Float2 P1 = Base - Pull;
float L1 = max( length( P1 ), 0.001 );
Float2 U1 = Uv01 + P1 / L1 * ( sin( Z ) + 1.0 ) * abs( sin( L1 * 9.0 - Z - Z ) );
Acc.y = 0.01 / max( length( Fract( U1 ) - 0.5 ), 0.0008 );
Z += 0.07;
Float2 P2 = Base - Pull;
float L2 = max( length( P2 ), 0.001 );
Float2 U2 = Uv01 + P2 / L2 * ( sin( Z ) + 1.0 ) * abs( sin( L2 * 9.0 - Z - Z ) );
Acc.z = 0.01 / max( length( Fract( U2 ) - 0.5 ), 0.0008 );
Final.rgb = Saturate( Acc / Len );
)";

inline constexpr const char* StarNest = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.x * 2.0, 1.0 );
float Time = Moment * 0.010 + 0.25;
float A1 = 0.50 + Time * 0.35 + Held * Aim.x * 1.20;
float A2 = 0.80 + Time * 0.22 + Held * Aim.y * 1.20;
float C1 = cos( A1 );
float S1 = sin( A1 );
float C2 = cos( A2 );
float S2 = sin( A2 );
Float3 Dir = Float3( Uv.x * 0.80, Uv.y * 0.80, 1.0 );
Dir = Float3( Dir.x * C1 - Dir.z * S1, Dir.y, Dir.x * S1 + Dir.z * C1 );
Dir = Float3( Dir.x * C2 - Dir.y * S2, Dir.x * S2 + Dir.y * C2, Dir.z );
Float3 Eye = Float3( 1.0 + Time * 2.0, 0.5 + Time, -1.5 );
Eye = Float3( Eye.x * C1 - Eye.z * S1, Eye.y, Eye.x * S1 + Eye.z * C1 );
Eye = Float3( Eye.x * C2 - Eye.y * S2, Eye.x * S2 + Eye.y * C2, Eye.z );
float March = 0.10;
float Fade = 1.0;
Float3 Vol = Float3( 0.0, 0.0, 0.0 );
for ( int Vr = 0; Vr < 16; Vr++ )
{
    Float3 P = Eye + March * Dir * 0.50;
    Float3 Span = Float3( 1.70, 1.70, 1.70 );
    P = abs( Float3( 0.85, 0.85, 0.85 ) - ( P - Span * floor( P / Span ) ) );
    float Prev = 0.0;
    float Acc = 0.0;
    for ( int Vi = 0; Vi < 14; Vi++ )
    {
        float Den = max( dot( P, P ), 0.000001 );
        P = abs( P ) / Den - 0.53;
        float Ln = length( P );
        Acc += abs( Ln - Prev );
        Prev = Ln;
    }
    float Dark = max( 0.0, 0.30 - Acc * Acc * 0.001 );
    Acc *= Acc * Acc;
    if ( Vr > 6 )
        Fade *= 1.0 - Dark;
    Vol += Fade;
    Vol += Float3( March, March * March, March * March * March * March ) * Acc * 0.0015 * Fade;
    Fade *= 0.730;
    March += 0.10;
}
float Grey = length( Vol );
Vol = Lerp( Float3( Grey, Grey, Grey ), Vol, 0.850 );
Final.rgb = Saturate( Vol * 0.01 );
)";

inline constexpr const char* Julia = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float Prox = exp( -dot( Uv - Aim, Uv - Aim ) * 14.0 );
Uv -= ( ( Uv - Aim ) / max( length( Uv - Aim ), 0.001 ) ) * Prox * Held * 0.12;
float Ang = 0.62 + 0.38 * sin( Moment * 0.22 );
Float2 Seed = Float2( 0.7885 * cos( Ang ), 0.7885 * sin( Ang ) );
Float2 Z = Uv * 1.25;
float Esc = 0.0;
for ( int Ji = 0; Ji < 64; Ji++ )
{
    Esc = float( Ji );
    if ( dot( Z, Z ) > 16.0 )
        break;
    Z = Float2( Z.x * Z.x - Z.y * Z.y, 2.0 * Z.x * Z.y ) + Seed;
}
float Ln = log( max( length( Z ), 1.0001 ) );
float Smooth = Esc - log2( max( Ln, 0.0001 ) );
float Tone = Saturate( Smooth / 64.0 );
Float3 Deep = Float3( 0.02, 0.03, 0.07 );
Float3 Mid = Float3( 0.12, 0.42, 0.58 );
Float3 Hot = Float3( 0.92, 0.72, 0.38 );
Float3 Tint = Lerp( Deep, Mid, Saturate( Tone * 2.2 ) );
Tint = Lerp( Tint, Hot, pow( Tone, 3.4 ) );
Final.rgb = Saturate( Tint );
)";

inline constexpr const char* Electric = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 ) * 4.0;
float Prox = exp( -dot( Uv * 0.25 - Aim, Uv * 0.25 - Aim ) * 10.0 );
Uv -= ( ( Uv * 0.25 - Aim ) / max( length( Uv * 0.25 - Aim ), 0.001 ) ) * Prox * Held * 0.45;
float T = Moment;
Float2 N0 = Uv * 0.70;
float Rz = 0.0;
float Gain = 2.0;
Float2 Np = N0;
for ( int E0 = 0; E0 < 5; E0++ )
{
    Float2 I = floor( Np );
    Float2 F = Fract( Np );
    Float2 U = F * F * ( 3.0 - 2.0 * F );
    float H00 = Fract( sin( dot( I + Float2( 0.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H10 = Fract( sin( dot( I + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H01 = Fract( sin( dot( I + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H11 = Fract( sin( dot( I + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float N = Lerp( Lerp( H00, H10, U.x ), Lerp( H01, H11, U.x ), U.y );
    Rz += abs( N - 0.5 ) * 2.0 / Gain;
    Gain *= 2.0;
    Np *= 2.0;
}
Float2 N1 = Uv * 0.70 + Float2( T * 1.60, -T * 1.70 );
float Rz2 = 0.0;
Gain = 2.0;
Np = N1;
for ( int E1 = 0; E1 < 5; E1++ )
{
    Float2 I = floor( Np );
    Float2 F = Fract( Np );
    Float2 U = F * F * ( 3.0 - 2.0 * F );
    float H00 = Fract( sin( dot( I + Float2( 0.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H10 = Fract( sin( dot( I + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H01 = Fract( sin( dot( I + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H11 = Fract( sin( dot( I + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float N = Lerp( Lerp( H00, H10, U.x ), Lerp( H01, H11, U.x ), U.y );
    Rz2 += abs( N - 0.5 ) * 2.0 / Gain;
    Gain *= 2.0;
    Np *= 2.0;
}
Float2 Warp = Float2( Rz, Rz2 ) - 0.5;
Warp *= 0.20;
Uv += Warp;
float Vein = 0.0;
Gain = 2.0;
Np = Uv;
for ( int E2 = 0; E2 < 5; E2++ )
{
    Float2 I = floor( Np );
    Float2 F = Fract( Np );
    Float2 U = F * F * ( 3.0 - 2.0 * F );
    float H00 = Fract( sin( dot( I + Float2( 0.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H10 = Fract( sin( dot( I + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H01 = Fract( sin( dot( I + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float H11 = Fract( sin( dot( I + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float N = Lerp( Lerp( H00, H10, U.x ), Lerp( H01, H11, U.x ), U.y );
    Vein += abs( N - 0.5 ) * 2.0 / Gain;
    Gain *= 2.0;
    Np *= 2.0;
}
float Pulse = 0.35 + 0.65 * abs( sin( T * 1.4 ) );
Float3 Tint = Float3( 0.22, 0.10, 0.48 ) / max( Vein * 0.85, 0.08 );
Tint += Float3( 0.55, 0.35, 0.95 ) * exp( -Vein * 3.2 ) * Pulse;
Final.rgb = Saturate( pow( abs( Tint ), 0.92 ) );
)";

inline constexpr const char* Hexagons = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float Prox = exp( -dot( Uv - Aim, Uv - Aim ) * 14.0 );
Uv -= ( ( Uv - Aim ) / max( length( Uv - Aim ), 0.001 ) ) * Prox * Held * 0.14;
float T = Moment * 0.18;
float Zoom = 4.8 + 0.35 * sin( T * 0.7 );
Float2 P = Uv * Zoom;
P += Float2( sin( T * 0.6 ), cos( T * 0.45 ) ) * 0.35;
Float2 S = Float2( 1.0, 1.7320508 );
Float2 A = P - S * floor( P / S ) - S * 0.5;
Float2 B = ( P - S * 0.5 ) - S * floor( ( P - S * 0.5 ) / S ) - S * 0.5;
float Da = dot( A, A );
float Db = dot( B, B );
Float2 H = Da < Db ? A : B;
float Cell = min( Da, Db );
float Edge = abs( length( H ) - 0.42 );
float Ring = exp( -Edge * Edge * 38.0 );
float Fill = smoothstep( 0.22, 0.04, Cell );
Float3 Deep = Float3( 0.04, 0.05, 0.07 );
Float3 Line = Float3( 0.28, 0.72, 0.78 );
Float3 Core = Float3( 0.92, 0.78, 0.32 );
Float3 Tint = Lerp( Deep, Line, Ring * 0.85 + Fill * 0.18 );
Tint += Core * exp( -Cell * 14.0 ) * ( 0.25 + 0.20 * sin( T * 3.0 + Cell * 20.0 ) );
Final.rgb = Saturate( Tint );
)";

inline constexpr const char* Grain = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment;
float N0 = Fract( sin( dot( Local + Float2( T * 37.1, T * 91.7 ), Float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
float N1 = Fract( sin( dot( Local + Float2( T * 19.3, -T * 53.2 ), Float2( 39.346, 11.135 ) ) ) * 23421.631 );
float Speck = ( N0 - 0.5 ) * 0.22 + ( N1 - 0.5 ) * 0.08;
Float3 Base = Float3( 0.10, 0.10, 0.105 );
Final.rgb = Saturate( Base + Speck );
)";

inline constexpr const char* Scanlines = R"(
Float2 Uv = Local / max( Extent.y, 1.0 );
float T = Moment;
float Line = 0.55 + 0.45 * sin( Local.y * 3.14159 * 0.85 + T * 8.0 );
float Mask = 0.82 + 0.18 * sin( Local.x * 2.094 );
float Roll = 0.04 * exp( -pow( Fract( Uv.y * 0.35 + T * 0.08 ) - 0.5, 2.0 ) * 80.0 );
Float3 Phosphor = Float3( 0.16, 0.72, 0.38 );
Float3 Tint = Phosphor * Line * Mask * 0.55 + Roll;
Final.rgb = Saturate( Tint );
)";

inline constexpr const char* Flaring = R"(
Float2 AimPx = Mouse - ( Screen - Local );
Float2 Aim = AimPx / max( Extent.y, 1.0 );
float Held = Press.x;
Float2 Uv = Local / max( Extent.y, 1.0 );
float Prox = exp( -dot( Uv - Aim, Uv - Aim ) * 12.0 );
Uv -= ( ( Uv - Aim ) / max( length( Uv - Aim ), 0.001 ) ) * Prox * Held * 0.14;
float T = Moment * 0.22;
float Rad = length( Uv );
float Ang = atan2( Uv.y, Uv.x );
float Disk = exp( -Rad * Rad * 14.0 );
float Halo = exp( -Rad * Rad * 3.2 );
float N = 0.0;
float Amp = 0.55;
Float2 Pn = Float2( Ang * 1.15, Rad * 3.4 - T );
for ( int Fl = 0; Fl < 5; Fl++ )
{
    Float2 Ip = floor( Pn );
    Float2 Fp = Fract( Pn );
    Float2 U = Fp * Fp * ( 3.0 - 2.0 * Fp );
    float A = Fract( sin( dot( Ip, Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float B = Fract( sin( dot( Ip + Float2( 1.0, 0.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float C = Fract( sin( dot( Ip + Float2( 0.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    float D = Fract( sin( dot( Ip + Float2( 1.0, 1.0 ), Float2( 127.1, 311.7 ) ) ) * 43758.5453 );
    N += Amp * Lerp( Lerp( A, B, U.x ), Lerp( C, D, U.x ), U.y );
    Pn = Pn * 2.15 + Float2( 1.7, 3.1 );
    Amp *= 0.5;
}
float Spike = pow( abs( sin( Ang * 8.0 + T * 1.6 + N * 4.0 ) ), 6.5 );
Spike += 0.45 * pow( abs( sin( Ang * 14.0 - T * 0.9 ) ), 10.0 );
float Corona = Spike * exp( -Rad * 1.85 ) * ( 0.35 + N * 0.85 );
float Prom = pow( Saturate( N * 1.2 ), 2.4 ) * exp( -abs( Rad - 0.42 ) * 6.0 );
Float3 Night = Float3( 0.02, 0.018, 0.028 );
Float3 Ember = Float3( 0.95, 0.42, 0.08 );
Float3 Gold = Float3( 1.0, 0.82, 0.38 );
Float3 Tint = Night + Ember * ( Disk * 1.15 + Halo * 0.35 );
Tint += Gold * Disk * 0.85;
Tint += Ember * Corona * 0.90;
Tint += Gold * Prom * 0.45;
Final.rgb = Saturate( Tint );
)";

}
}
