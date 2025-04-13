# How 2 build - Windows (Visual Studio)

Install Visual Studio Community 2019 (or 2022) and make sure to install the "Desktop development with C++" workload.

Double click on the PhloxEngine.sln file to open the project.

Press F5 to build and run the engine. (Make sure you're on x86 Release!!)

## How 2 build - Windows (MSYS2) - No idea if this actually works.

Install msys2.

Run these commands:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_ttf
```

## How 2 build - Arch Linux

First run:

```bash
sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

Then run:

```bash
make
```

## How to build - Ubuntu

First run:

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

Then run:

```bash
make
```

## Nintendo Switch (DevKitPro)

First run:

```bash
pacman -S switch-sdl2 switch-sdl2_image switch-sdl2_mixer switch-sdl2_ttf
```

then run:

```bash
nacptool --create "Hamburger Engine" "maybekoi" "1.0.0" hamburger-engine.nacp # reminder to change the title, author, and version to your game's info!
make -f Makefile.nx
```
