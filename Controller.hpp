#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Planet.hpp"

enum ControllerActions {
    NoAction,
    ZoomPlanet
};

class Controller{

    private:
        GLFWwindow *window;
        std::vector<Planet *> planets;
        bool KEYPRESS;

        ControllerActions currAction;
        int zoomedPlanet; // identifier of the planet currently zoomed

        void detectZoomedPlanet();

    public:

        Controller(GLFWwindow *window, std::vector<Planet *> planets);
        
        ControllerActions listenEvent();
        Planet *getZoomedPlanet();
};