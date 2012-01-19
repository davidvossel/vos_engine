CC=g++
CFLAGS=-c -Wall -g -static
LDFLAGS=-lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_mixer
SOURCES=vos_main.cpp vos_screen.cpp vos_image.cpp vos_media_engine.cpp vos_collision_engine.cpp vos_text.cpp vos_font.cpp vos_sound.cpp vos_map.cpp vos_map_common.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf $(EXECUTABLE)
