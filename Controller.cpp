#include "Controller.hpp"

Controller::Controller(GLFWwindow *window, std::vector<Planet*> planets, std::vector<std::reference_wrapper<Button>> buttons, int windowWidth, int windowHeight) {
	this->window = window;
    this->planets = planets;
    this->buttons = buttons;
    setWindowSize(windowWidth, windowHeight);
    this->currAction = NoAction;
    this->zoomedPlanet = 0; // intialize to random value
}

void Controller::detectZoomedPlanet(){

    //this->zoomedPlanet = 2;

}

void Controller::setZoomedPlanetIndex(int index){

    this->zoomedPlanet = index;

}

ControllerActions Controller::listenEvent() {

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (lastKey == -1) {
			lastKey = GLFW_MOUSE_BUTTON_LEFT;
			bool pressedAnyButton = processMousePressed(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
			if (!pressedAnyButton) {
				//tryClickPlanet(xpos/windowWidth*2-1,ypos/windowHeight*2-1);
				/* TODO
				 * If someone clicks a planet while panning, the action will execute.
				 * This can still happen if the clicked planet is checked both at click start and end since it could follow the mouse.
				 * Maybe implement a timer or measure the panned distance
				 */
			}
		} else {
			processMouseHeld(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
		}
	} else {
		processMouseNotPressed(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
		if (lastKey == GLFW_MOUSE_BUTTON_LEFT) {
			lastKey = -1;
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (lastKey == -1) {
			lastKey = GLFW_KEY_SPACE;
			if (this->currAction == NoAction) {
				this->currAction = ZoomPlanet;
				this->detectZoomedPlanet();
			} else if (this->currAction == ZoomPlanet) {
				this->currAction = NoAction;
			}
		}
	} else {
		if (lastKey == GLFW_KEY_SPACE) {
			lastKey = -1;
		}
	}
	return this->currAction;
}

bool Controller::processMousePressed(float mouseX, float mouseY) {
	bool clickedAny = false;
	for (Button &b : buttons) {
		clickedAny |= b.processMousePressed(mouseX, mouseY);
	}
	return clickedAny;
}

void Controller::processMouseHeld(float mouseX, float mouseY) {
	for (Button &b : buttons) {
		b.processMouseHeld(mouseX, mouseY);
	}
}

void Controller::processMouseNotPressed(float mouseX, float mouseY) {
	for (Button &b : buttons) {
		b.processMouseNotPressed(mouseX, mouseY);
	}
}

Planet *Controller::getZoomedPlanet(){

	return this->planets[this->zoomedPlanet];

}
