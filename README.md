# ShinkUnicode

Unicode compatibility layer for FVP (Favorite View Point) engine visual novels. Hooks Win32 API calls at runtime to fix locale issues, enabling these games to run correctly on non-Japanese systems without Locale Emulator.

## Features

- Forces ShiftJIS codepage for correct text encoding
- Bridges ANSI Win32 API calls to their Unicode equivalents
- Custom font loading from a `font/` directory
- Resizable game window with fullscreen toggle
- Wine compatibility (automatic video codec fallback to WebM)

## Supported games

- Wiz Anniversary and its fandisc
- Happy Margaret!
- Hoshizora no Memoria, Eternal Heart and HD remaster
- Irotoridori no Sekai trilogy and HD remaster
- AstralAir, its fandisc and HD remaster
- Sakura, Moyu.

The target game is selected at compile time via `FVP_GAME_ID` in `src/game.h`. Note that original version of Irotoridori no Sekai trilogy uses slightly different engine build than AstralAir and all subsequent games and HD remasters. Following distinct versions exist so far:
- `WIZANNIVERSARY`
- `HOSHINOMEMORIA`
- `IROSEKA_WORLD`
- `ASTRALAIR_WE`

Resizable game window is only supported for `HOSHINOMEMORIA` and newer games, older engine versions setup their game window in substantially different way (only one window class instead of two) and are unlikely to benefit from this feature anyway. Proper scaling on D3D reset is only supported since `IROSEKA_WORLD`.

## Building

```
git submodule update --init
MSBuild.exe ShinkUnicode.sln -p:Configuration=Release -p:Platform=Win32
```

Output: `bin/Release-x86/ShinkUnicode.dll`

## Usage

Use [SetDLL](https://github.com/microsoft/detours/wiki/SampleSetdll) from Microsoft Detours to inject the DLL into the game executable:

```
setdll /d:ShinkUnicode.dll GameExecutable.exe
```

Optionally, place `.ttf` or `.otf` font files in a `font/` directory next to the game executable to load custom fonts.

## Dependencies

- [Microsoft Detours](https://github.com/microsoft/Detours)
- [Boost.PFR](https://github.com/boostorg/pfr)
