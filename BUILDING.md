# How 2 build - Windows

Install Visual Studio Community 2019 and make sure to install the "Desktop development with C++" workload.

Double click on the PhloxEngine.sln file to open the project.

Press F5 to build and run the engine. (Make sure you're on x86 Release!!)

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