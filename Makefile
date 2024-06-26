MAKEFLAGS += --no-builtin-rules

PROGRAM = SolarSystem3D

SOURCES = $(PROGRAM).cpp Planet.cpp Controller.cpp Button.cpp ScrollButton.cpp ScrollKnob.cpp
CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
INC = -Iheaders

.PHONY: all shaders build run clean $(PROGRAM)

all: build shaders

$(PROGRAM): build

build: $(SOURCES)
	g++ $(CFLAGS) $(INC) -o $(PROGRAM) $? $(LDFLAGS)

shaders:
	$(MAKE) -C shaders all

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)
