#include "Controller.hpp"

Controller::Controller(GLFWwindow *window, std::vector<Planet*> planets, std::vector<std::reference_wrapper<Button>> buttons, int windowWidth, int windowHeight, std::function<int(float, float)> tryClickPlanet, std::function<void(bool, std::string)> updateWatchingPlanetState) {
	this->window = window;
	this->tryClickPlanet = tryClickPlanet;
	this->updateWatchingPlanetState = updateWatchingPlanetState;
    this->planets = planets;
    this->buttons = buttons;
    setWindowSize(windowWidth, windowHeight);
    this->renderType = RENDER_NORMAL;
    this->currAction = NoAction;
    this->zoomedPlanet = 2; // intialize to random value
}

void Controller::setZoomedPlanetIndex(int index){

    this->zoomedPlanet = index;
	std::string name=getZoomedPlanet()->getName();
	name[0] = std::toupper(name[0]);
	updateWatchingPlanetState(currAction == ZoomPlanet, name);

}

void Controller::setZoomedPlanetIndexRelative(int offset){

	setZoomedPlanetIndex((this->zoomedPlanet + offset + planets.size()) % planets.size());

}

ControllerActions Controller::listenEvent() {

	double xpos, ypos;
	static int halfZoomedPlanet = -2; //-1 is valid, means clicked background
	glfwGetCursorPos(window, &xpos, &ypos);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (lastKey == -1) {
			lastKey = GLFW_MOUSE_BUTTON_LEFT;
			bool pressedAnyButton = processMousePressed(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
			if (!pressedAnyButton) {
				halfZoomedPlanet = tryClickPlanet(xpos/windowWidth*2-1,ypos/windowHeight*2-1);
			} else {
				//TODO disable panning
			}
		} else {
			processMouseHeld(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
		}
	} else {
		processMouseNotPressed(xpos / windowWidth * 2 - 1, ypos / windowHeight * 2 - 1);
		if (lastKey == GLFW_MOUSE_BUTTON_LEFT) {
			lastKey = -1;
			int found = tryClickPlanet(xpos/windowWidth*2-1,ypos/windowHeight*2-1);
			if (found == halfZoomedPlanet) {
				if (found != -1) {
					std::cout << "Clicked planet " << found << std::endl;
					currAction = ZoomPlanet;
					setZoomedPlanetIndex(found);
				}
			}
			halfZoomedPlanet = -2;
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (lastKey == -1) {
			lastKey = GLFW_KEY_SPACE;
			currAction = ControllerActions(!currAction);
			setZoomedPlanetIndex(zoomedPlanet);
		}
	} else {
		if (lastKey == GLFW_KEY_SPACE) {
			lastKey = -1;
		}
	}
	if (glfwGetKey(this->window, GLFW_KEY_L) == GLFW_PRESS) {
		if (lastKey == -1) {
			lastKey = GLFW_KEY_L;
			if (renderType == RENDER_LOD) {
				renderType = RENDER_NORMAL;
			} else {
				renderType = RENDER_LOD;
			}
		}
	} else {
		if (lastKey == GLFW_KEY_L) {
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

RenderType Controller::getRenderType() {
	return this->renderType;
}
