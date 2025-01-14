# How 2 build - Windows (Visual Studio)

Install Visual Studio Community 2019 and make sure to install the "Desktop development with C++" workload.

Double click on the PhloxEngine.sln file to open the project.

Press F5 to build and run the engine. (Make sure you're on x86 Release!!)

# How 2 build - Windows (MSYS2) - No idea if this actually works.

Install msys2.

Run these commands:

```bash
pacman -Syu
pacman -S make mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut mingw-w64-x86_64-freetype mingw-w64-x86_64-mesa mingw-w64-x86_64-openal mingw-w64-x86_64-libvorbis
```

# How 2 build - Arch Linux

First run:

```bash
sudo pacman -S freetype2 freeglut
```

Then run:

```bash
make
```

# How to build - Ubuntu

First run:

```bash
sudo apt-get install libfreetype-dev libfreetype6 libfreetype6-dev glut
```

Then run:

```bash
make
```