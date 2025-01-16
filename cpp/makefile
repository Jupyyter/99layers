# Compiler
CXX = g++ -g

# Project Name
TARGET = ikea

# Directories
INCLUDE_DIR = D:\ikeaBattle\SFML-2.6.1\include
LIB_DIR = D:\ikeaBattle\SFML-2.6.1\lib
FREETYPE_INCLUDE = D:\ikeaBattle\freeType\freetype-2.13.3\include
FREETYPE_LIB = D:\path\to\freetype\lib

# Source Files
SRC = Camera.cpp menu.cpp Object.cpp laserEnd.cpp Terrain.cpp GameMap.cpp InteractiveObject.cpp HappyEnd.cpp EditorMap.cpp Boss.cpp npc.cpp npcs.cpp mainMenu.cpp item.cpp inventory.cpp attack.cpp idk.cpp Animation.cpp GameOverScreen.cpp TextBox.cpp PacMan.cpp hedgehog.cpp CollisionDetector.cpp player.cpp staticObjects.cpp CutScene.cpp bloodParticle.cpp ObjectFactory.cpp penguin.cpp sprite.cpp ak47.cpp Background.cpp SpawnPoint.cpp bloodParticles.cpp main.cpp

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

# Default target - builds and runs
default: $(TARGET) copy_openal run

# Build the target
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copy OpenAL DLL
copy_openal:
	cp "$(LIB_DIR)/../bin/openal32.dll" ./

# Run the executable
run: $(TARGET)
	./$(TARGET).exe

# Clean rule to remove the executable, object files, and DLLs
clean:
	rm -f *.o $(TARGET).exe *.dll

.PHONY: default clean run copy_openal