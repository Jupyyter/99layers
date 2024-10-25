# Compiler
CXX = g++ -g

# Project Name
TARGET = cpp/levelEditor

# Directories
INCLUDE_DIR = D:\ikeaBattle\SFML-2.6.1\include
LIB_DIR = D:\ikeaBattle\SFML-2.6.1\lib
FREETYPE_INCLUDE = D:\ikeaBattle\freeType\freetype-2.13.3\include
FREETYPE_LIB = D:\path\to\freetype\lib

# Source Files without the "cpp/" prefix
SRC_FILES =Camera.cpp Object.cpp laserEnd.cpp Terrain.cpp GameMap.cpp HappyEnd.cpp EditorMap.cpp leveleditor.cpp  Boss.cpp npc.cpp Menu.cpp item.cpp inventory.cpp attack.cpp idk.cpp Animation.cpp GameOverScreen.cpp TextBox.cpp PacMan.cpp CollisionDetector.cpp player.cpp CutScene.cpp ObjectFactory.cpp penguin.cpp sprite.cpp ak47.cpp Background.cpp Capybara.cpp SpawnPoint.cpp bloodParticles.cpp

# Add the "cpp/" prefix to each source file
SRC = $(addprefix cpp/,$(SRC_FILES))

# Object Files
OBJ = $(SRC:.cpp=.o)

# Compiler Flags
CXXFLAGS = -I$(INCLUDE_DIR) -I$(FREETYPE_INCLUDE) -DSFML_STATIC

# Linker Flags
LDFLAGS = -L$(LIB_DIR) -L$(FREETYPE_LIB) \
    -lsfml-audio-s \
    -lsfml-graphics-s \
    -lsfml-window-s \
    -lsfml-system-s \
    -lsfml-network-s \
    -lopengl32 \
    -lfreetype \
    -lwinmm \
    -lgdi32 \
    -lopenal32 \
    -lFLAC \
    -lvorbisenc \
    -lvorbisfile \
    -lvorbis \
    -logg \
    -static

# Default rule
all: $(TARGET) run

# Build the target
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable after building
run:
	./$(TARGET).exe

# Clean rule to remove the executable and object files
clean:
	rm *.o prog3

.PHONY: all clean run
