CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -g

SRCDIR = ../../src
BUILDDIR = ../../build
BINDIR = ../../bin
EXTERNALDIR = ../../external

SOURCES = $(shell find $(SRCDIR) -name "*.cpp")
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)

INCLUDES = -I$(SRCDIR) \
           -I$(SRCDIR)/engine \
		   -I$(SRCDIR)/gameName \
		   -I$(SRCDIR)/thirdparty \
           -I$(EXTERNALDIR)/SDL2 \
           -I$(EXTERNALDIR)/discord-rpc/include \
           -I$(EXTERNALDIR)/al/include \
           -I$(EXTERNALDIR)/ogg/include \
           -I$(EXTERNALDIR)/vorbis/include \
           -I$(EXTERNALDIR)/vlc/include

LIBS = -L$(EXTERNALDIR)/SDL2/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx \
       -L$(EXTERNALDIR)/al/lib -lopenal \
       -L$(EXTERNALDIR)/ogg/lib -logg \
       -L$(EXTERNALDIR)/vorbis/lib -lvorbis -lvorbisfile \
       -lpthread -ldl -lm \
       -L$(EXTERNALDIR)/lib/Linux64 -ldiscord-rpc \
       -L$(EXTERNALDIR)/vlc/lib -lvlc

TARGET = $(BINDIR)/hamburgerEngine

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(BUILDDIR)/engine/audio
	@mkdir -p $(BUILDDIR)/engine/core
	@mkdir -p $(BUILDDIR)/engine/debug
	@mkdir -p $(BUILDDIR)/engine/graphics
	@mkdir -p $(BUILDDIR)/engine/input
	@mkdir -p $(BUILDDIR)/engine/utils
	@mkdir -p $(BUILDDIR)/gameName/states
	@mkdir -p $(BUILDDIR)/gameName/substates

$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@$(CXX) $(OBJECTS) $(LIBS) -o $@
	@echo "Build complete!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR)/external/sdl2.vis/embedded_visualizer.o: $(EXTERNALDIR)/sdl2.vis/embedded_visualizer.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILDDIR)
	@rm -rf $(BINDIR)
	@echo "Clean complete!"

.PHONY: all clean directories
