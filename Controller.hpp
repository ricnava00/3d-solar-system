#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Planet.hpp"
#include "Button.hpp"
#include "ScrollButton.hpp"
#include "ScrollKnob.hpp"

enum ControllerActions {
    NoAction,
    ZoomPlanet
};

enum RenderType {
	RENDER_NORMAL,
	RENDER_LOD
};

class Controller{

    private:
		GLFWwindow *window;
		int windowWidth, windowHeight;
		std::function<int(float,float)> tryClickPlanet;
		std::function<void(bool, std::string)> updateWatchingPlanetState;
        std::vector<Planet *> planets;
        std::vector<std::reference_wrapper<Button>> buttons;
        int lastKey = 0;
        RenderType renderType;
        ControllerActions currAction;
        int zoomedPlanet; // identifier of the planet currently zoomed
        bool panningEnabled;

    public:

        Controller(GLFWwindow *window, std::vector<Planet*> planets, std::vector<std::reference_wrapper<Button>> buttons, int windowWidth, int windowHeight, std::function<int(float, float)> tryClickPlanet, std::function<void(bool, std::string)> updateWatchingPlanetState);

		void setWindowSize(int windowWidth, int windowHeight);
        ControllerActions listenEvent();
        Planet *getZoomedPlanet();
        void toggleAction();
        void setZoomedPlanetIndex(int index);
        void setZoomedPlanetIndexRelative(int offset);
        bool processMousePressed(float mouseX, float mouseY);
        void processMouseHeld(float mouseX, float mouseY);
        void processMouseNotPressed(float mouseX, float mouseY);
        RenderType getRenderType();
        bool isPanningEnabled();
};
