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

class Controller{

    private:
		GLFWwindow *window;
        std::vector<Planet *> planets;
        std::vector<std::reference_wrapper<Button>> buttons;
        int lastKey = 0;

        ControllerActions currAction;
        int zoomedPlanet; // identifier of the planet currently zoomed

        void detectZoomedPlanet();

    public:

        Controller(GLFWwindow *window, std::vector<Planet *> planets, std::vector<std::reference_wrapper<Button>> buttons);
        
        ControllerActions listenEvent();
        Planet *getZoomedPlanet();
        bool processMousePressed(float mouseX, float mouseY);
        void processMouseHeld(float mouseX, float mouseY);
        void processMouseNotPressed(float mouseX, float mouseY);
};
