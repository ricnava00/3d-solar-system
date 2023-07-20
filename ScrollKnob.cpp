#include "ScrollKnob.hpp"

ScrollKnob::ScrollKnob(std::function<void(float)> updateCallback, int steps, int initialStep, float widthFactor, glm::vec2 offset, float alignRight, glm::vec2 minSize, Button *adjacentButton) :
		Button([](Button&) {
		}, "", offset, alignRight, minSize, adjacentButton) {
	type = SCROLL_KNOB;
	this->updateCallback = updateCallback;
	this->steps = steps;
	this->currentStep = initialStep;
	this->widthFactor = widthFactor;
	info = buttonInfo.at(type);
	halfPressed = true; //To run callback after initialization
}

bool ScrollKnob::processMousePressed(float mouseX, float mouseY) {
	if (mouseIsOver(mouseX, mouseY)) {
		halfPressed = true;
		return true;
	}
	return false;
}

void ScrollKnob::processMouseHeld(float mouseX, float mouseY) {
	if (halfPressed) {
		int currentStep = getStepFromMouseX(mouseX);
		if (this->currentStep != currentStep) {
			this->currentStep = currentStep;
			updateCallback((float) currentStep / steps);
			needUpdate = true;
		}
	}
}

void ScrollKnob::processMouseNotPressed(float mouseX, float mouseY) {
	setHover(mouseIsOver(mouseX, mouseY));
	if (halfPressed) {
		updateCallback((float) currentStep / steps);
		needUpdate = true;
	}
	halfPressed = false;
}

float ScrollKnob::getScaleCenter() {
	return 1.f;
}

Geometry ScrollKnob::getSideGeometry(int side) {
	Geometry geometry = Button::getSideGeometry(side);
	geometry.offset.x -= getFullWidth() / 2;
	geometry.offset.x += ((float) currentStep / steps * widthFactor + (1 - widthFactor) / 2) * minSize.x / Ar;
	return geometry;
}

int ScrollKnob::getStepFromMouseX(float mouseX) {
	return std::clamp(static_cast<int>(std::round(((mouseX - Button::getSideGeometry(0).offset.x) / minSize.x * Ar - (1 - widthFactor) / 2) / widthFactor * steps)), 0, steps);
}
