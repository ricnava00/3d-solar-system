PROGRAM = SolarSystem3D
LIB = headers
LINK = Planet.cpp Controller.cpp

CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SolarSystem3D: $(PROGRAM).cpp
	g++ $(CFLAGS) -I$(LIB) -o $(PROGRAM) $(PROGRAM).cpp $(LINK) $(LDFLAGS)

.PHONY: run clean

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM)