#include "Controller.hpp"

Controller::Controller(GLFWwindow *window, std::vector<Planet *> planets){

    this->window = window;
    this->planets = planets;
    this->KEYPRESS = false;
    this->currAction = NoAction;

    this->zoomedPlanet = 0; // intialize to random value

}

void Controller::detectZoomedPlanet(){

    this->zoomedPlanet = 2;

}

ControllerActions Controller::listenEvent(){

    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS){
    	if(!this->KEYPRESS){
			this->KEYPRESS = true;
			if (this->currAction == NoAction){
				this->currAction = ZoomPlanet;
				this->detectZoomedPlanet();
			}
			else if (this->currAction == ZoomPlanet){
				this->currAction = NoAction;
			}
    	}
	}
	else{
        this->KEYPRESS = false;
	}
    return this->currAction;
}

Planet *Controller::getZoomedPlanet(){

    return this->planets[this->zoomedPlanet];

}
