CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
INCLUDES = -I./include -I/usr/include/freetype2 -I./external
LIBS = -lopengl32 -lfreeglut -lglu32 -lfreetype -lopenal -lvorbisfile -lvorbis -logg

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
		  
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/engine/*.cpp) \
          $(wildcard $(SRC_DIR)/states/*.cpp) \
		  $(wildcard $(SRC_DIR)/gameName/*.cpp) \
		  $(wildcard $(SRC_DIR)/gameName/states/*.cpp) \
		  $(wildcard $(SRC_DIR)/gameName/substates/*.cpp) 

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

EXECUTABLE = $(BIN_DIR)/PhloxEngine

.PHONY: all clean copy_assets

all: $(EXECUTABLE) copy_assets

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

copy_assets:
	@mkdir -p $(BIN_DIR)/assets
	@cp -r assets/* $(BIN_DIR)/assets/

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

-include $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MT $(@:.d=.o) $< > $@