# Building

## Windows (Visual Studio)

Install Visual Studio Community 2019 (or 2022) and make sure to install the "Desktop development with C++" workload.

Double click on the hamburger-engine.sln file to open the project in the platforms/VisualC folder.

Press F5 to build and run the engine.

## Windows (MSYS2) - No idea if this actually works.

Install msys2.

Run these commands:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_ttf
```

## Arch Linux

First run:

```bash
sudo pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

Then run:

```bash
cd platforms && cd Makefiles && make
```

## Ubuntu

First run:

```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

Then run:

```bash
cd platforms && cd Makefiles && make
```

## Nintendo Switch (DevKitPro)

First run:

```bash
pacman -S switch-sdl2 switch-sdl2_image switch-sdl2_mixer switch-sdl2_ttf switch-ffmpeg switch-zlib switch-dav1d
```

then run:

```bash
cd platforms && cd Makefiles && make -f Makefile.nx
```
