#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

#include "pack/catalog.hpp"
#include "ur/ur.hpp"

namespace {

struct Dress {
    CColor Void = CColor( 18, 20, 24 );
    CColor Shell = CColor( 26, 28, 33 );
    CColor Rail = CColor( 22, 24, 29 );
    CColor Lift = CColor( 34, 38, 46 );
    CColor Ink = CColor( 228, 230, 234 );
    CColor Mute = CColor( 138, 144, 154 );
    CColor Dim = CColor( 92, 98, 108 );
    CColor Line = CColor( 48, 52, 60 );
    CColor Accent = CColor( 148, 172, 196 );
    CColor Pick = CColor( 40, 48, 60 );
};

Dress Tone;
int Selected = 0;
float RailAim = 0.0f;
float RailAt = 0.0f;

void DrawFill( const CRectangle& Bounds, unsigned int Effect, float Round ) {
    unsigned int Former = Canvas->Effect( Effect );
    Canvas->Rectangle( Bounds, CColor( 255, 255, 255 ), Round );
    Canvas->Effect( Former );
}

void DrawLabel( const CRectangle& Area, const char* Text, CColor Tint, bool Center ) {
    CVector Size = Font->Measure( Text );
    float Left = Center ? Area.Left + ( Area.Width - Size.Horizontal ) * 0.5f : Area.Left;
    float Top = Area.Top + ( Area.Height - Font->LineSpan ) * 0.5f;
    if ( Left < Area.Left )
        Left = Area.Left;
    Canvas->Write( Font.get( ), CVector( Left, Top ), Tint, Text );
}

void DrawHeading( const CRectangle& Area, const char* Text, CColor Tint ) {
    CVector Size = Heading->Measure( Text );
    float Left = Area.Left;
    float Top = Area.Top + ( Area.Height - Heading->LineSpan ) * 0.5f;
    Canvas->Write( Heading.get( ), CVector( Left, Top ), Tint, Text );
}

bool HitRow( const CRectangle& Bounds, bool& Hovered ) {
    CVector Point = Input->MousePosition;
    Hovered = Bounds.Contains( Point );
    return Hovered && Input->MousePressed( 0 );
}

void StepSelect( int Delta ) {
    Selected += Delta;
    if ( Selected < 0 )
        Selected = pack::LiveCount - 1;
    if ( Selected >= pack::LiveCount )
        Selected = 0;
}

void TickKeys( ) {
    if ( ur::pressed( ur::Key::Escape ) )
        ur::app::quit( );
    if ( ur::pressed( ur::Key::Up ) || ur::pressed( ur::Key::Left ) )
        StepSelect( -1 );
    if ( ur::pressed( ur::Key::Down ) || ur::pressed( ur::Key::Right ) )
        StepSelect( 1 );
    if ( ur::pressed( ur::Key::Home ) )
        Selected = 0;
    if ( ur::pressed( ur::Key::End ) )
        Selected = pack::LiveCount - 1;
}

void Paint( ) {
    float Wide = ( float )ur::app::width( );
    float Tall = ( float )ur::app::height( );
    if ( Wide < 8.0f || Tall < 8.0f )
        return;

    float Pad = 16.0f;
    float Head = 56.0f;
    float RailWide = 252.0f;
    float Gap = 12.0f;
    float Round = 12.0f;
    float Row = 44.0f;
    float Thumb = 32.0f;

    CRectangle Client( 0.0f, 0.0f, Wide, Tall );
    Canvas->Rectangle( Client, Tone.Void, 0.0f );

    CRectangle Header( Pad, Pad, Wide - Pad * 2.0f, Head );
    CRectangle Body( Pad, Header.Bottom( ) + 8.0f, Wide - Pad * 2.0f, Tall - Header.Bottom( ) - Pad - 8.0f );
    CRectangle Rail( Body.Left, Body.Top, RailWide, Body.Height );
    CRectangle Stage( Rail.Right( ) + Gap, Body.Top, Body.Width - RailWide - Gap, Body.Height );

    Canvas->Rectangle( Header, Tone.Shell, Round );
    Canvas->Border( Header, Tone.Line, Round, 1.0f );
    DrawHeading( CRectangle( Header.Left + 16.0f, Header.Top, 220.0f, Header.Height ), "Shader Pack", Tone.Ink );
    static char Count[ 48 ] = { };
    if ( Count[ 0 ] == 0 )
        wsprintfA( Count, "%d looks", pack::LiveCount );
    DrawLabel( CRectangle( Header.Left + 236.0f, Header.Top + 4.0f, 280.0f, Header.Height - 8.0f ), Count, Tone.Mute, false );
    DrawLabel( CRectangle( Header.Right( ) - 168.0f, Header.Top, 152.0f, Header.Height ), "Esc  ·  arrows", Tone.Dim, false );

    Canvas->Rectangle( Rail, Tone.Rail, Round );
    Canvas->Border( Rail, Tone.Line, Round, 1.0f );
    Canvas->Rectangle( Stage, Tone.Shell, Round );
    Canvas->Border( Stage, Tone.Line, Round, 1.0f );

    CRectangle Preview = Stage.Pad( 14.0f, 14.0f );
    Preview.Height = Stage.Height - 78.0f;
    if ( Preview.Height < 80.0f )
        Preview.Height = 80.0f;
    DrawFill( Preview, pack::effect( Selected ), 10.0f );
    Canvas->Border( Preview, Tone.Line, 10.0f, 1.0f );

    const pack::Entry& Current = pack::Live[ Selected ];
    CRectangle Title( Preview.Left, Preview.Bottom( ) + 10.0f, Preview.Width, 22.0f );
    CRectangle Note( Preview.Left, Title.Bottom( ) + 2.0f, Preview.Width, 22.0f );
    DrawHeading( Title, Current.Name, Tone.Ink );
    DrawLabel( Note, Current.Note, Tone.Mute, false );

    float ListTop = Rail.Top + 10.0f;
    float ListBot = Rail.Bottom( ) - 10.0f;
    float ListH = ListBot - ListTop;
    float LiveBlock = 22.0f + ( float )pack::LiveCount * Row;
    float Reach = LiveBlock + 8.0f;

    if ( Rail.Contains( Input->MousePosition ) && Input->WheelDelta != 0.0f ) {
        RailAim -= Input->WheelDelta * 48.0f;
        Input->WheelDelta = 0.0f;
    }
    float Most = Reach > ListH ? Reach - ListH : 0.0f;
    if ( RailAim > Most )
        RailAim = Most;
    if ( RailAim < 0.0f )
        RailAim = 0.0f;
    RailAt += ( RailAim - RailAt ) * 0.22f;

    Canvas->PushClip( CRectangle( Rail.Left + 1.0f, ListTop, Rail.Width - 2.0f, ListH ) );

    float Cursor = ListTop - RailAt;
    DrawLabel( CRectangle( Rail.Left + 12.0f, Cursor, Rail.Width - 24.0f, 20.0f ), "Live", Tone.Dim, false );
    Cursor += 22.0f;

    for ( int Index = 0; Index < pack::LiveCount; Index++ ) {
        CRectangle Slot( Rail.Left + 8.0f, Cursor, Rail.Width - 16.0f, Row - 4.0f );
        bool Hovered = false;
        if ( HitRow( Slot, Hovered ) )
            Selected = Index;

        bool On = Index == Selected;
        if ( On || Hovered )
            Canvas->Rectangle( Slot, On ? Tone.Pick : Tone.Lift, 8.0f );

        CRectangle Swatch( Slot.Left + 6.0f, Slot.Top + ( Slot.Height - Thumb ) * 0.5f, Thumb + 6.0f, Thumb );
        DrawFill( Swatch, pack::effect( Index ), 6.0f );
        if ( On )
            Canvas->Border( Swatch, Tone.Accent, 6.0f, 1.0f );

        DrawLabel( CRectangle( Swatch.Right( ) + 10.0f, Slot.Top, Slot.Width - 56.0f, Slot.Height ), pack::Live[ Index ].Name, On ? Tone.Ink : Tone.Mute, false );
        Cursor += Row;
    }

    Canvas->PopClip( );
}

}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int ) {
    ur::app::Config Config;
    Config.title = "Shader Pack";
    Config.width = 1280;
    Config.height = 760;
    Config.backend = ur::Backend::DX11;
    Config.vsync = true;
    Config.docking = false;
    Config.persist = false;

    return ur::app::run( Config, [ ] {
        static bool Ready = false;
        if ( !Ready ) {
            ur::theme::apply( 4 );
            pack::warm( );
            Ready = true;
        }
        TickKeys( );
        Paint( );
    } );
}
