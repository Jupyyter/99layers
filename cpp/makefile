# Compiler with maximum optimization
CXX = g++
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

# Maximum Optimization Flags
CXXFLAGS = -I$(INCLUDE_DIR) -I$(FREETYPE_INCLUDE) -DSFML_STATIC \
    -Ofast \
    -march=native \
    -mtune=native \
    -fno-signed-zeros \
    -fno-trapping-math \
    -ffast-math \
    -funroll-all-loops \
    -faggressive-loop-optimizations \
    -fomit-frame-pointer \
    -ftree-vectorize \
    -fopt-info-vec-optimized \
    -fprefetch-loop-arrays \
    -finline-functions \
    -finline-limit=1000 \
    -fmodulo-sched \
    -fmodulo-sched-allow-regmoves \
    -fgcse-after-reload \
    -fsched-pressure \
    -fsched-spec-load \
    -fipa-pta \
    -fdevirtualize-at-ltrans \
    -fdevirtualize-speculatively \
    -ftree-loop-distribution \
    -ftree-loop-ivcanon \
    -ftree-loop-im \
    -fivopts \
    -fvariable-expansion-in-unroller \
    -freorder-blocks-algorithm=stc \
    -freorder-blocks-and-partition \
    -fuse-linker-plugin \
    -fno-stack-protector \
    -fno-math-errno \
    -pipe \
    -DNDEBUG \
    -DGL_SILENCE_DEPRECATION \
    -static

# Enhanced Linker Flags
LDFLAGS = -L$(LIB_DIR) -L$(FREETYPE_LIB) \
    -s \
    -Wl,--as-needed \
    -Wl,--gc-sections \
    -Wl,-O3 \
    -Wl,--strip-all \
    -Wl,--relax \
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

# Default target - builds and runs with maximum optimization
default: clean $(TARGET) optimize copy_openal run

# Build the target
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Post-build optimization
optimize: $(TARGET)
	strip --strip-all $(TARGET).exe
	# Optional: Use UPX for additional compression if available
	# -if command -v upx >/dev/null 2>&1; then upx --best --lzma $(TARGET).exe; fi

# Compile rule for .cpp files with maximum optimization
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Copy OpenAL DLL
copy_openal:
	cp "$(LIB_DIR)/../bin/openal32.dll" ./

# Run the executable
run: $(TARGET)
	./$(TARGET).exe

# Enhanced clean rule
clean:
	rm -f *.o $(TARGET).exe *.dll *.gch *.pch

.PHONY: default clean run copy_openal optimize