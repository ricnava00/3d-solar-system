#include "ScrollKnob.hpp"

ScrollKnob::ScrollKnob(std::function<void(float)> updateCallback, int steps, int initialStep, float widthFactor, glm::vec2 offset, Alignment alignment, glm::vec2 minSize, Button *adjacentButton) :
		Button([](Button&) {
		}, "", offset, alignment, minSize, adjacentButton) {
	type = SCROLL_KNOB;
	this->offset.x += minSize.x / Ar * alignment / 2;
	this->updateCallback = updateCallback;
	this->steps = steps;
	this->currentStep = initialStep;
	this->widthFactor = widthFactor;
	info = buttonInfo.at(type);
	halfPressed = true; //To run callback after initialization
}

void ScrollKnob::setOffset(const glm::vec2 offset) {
	this->offset = offset;
	this->offset.x += minSize.x / Ar * alignment / 2;
}

bool ScrollKnob::processMousePressed(float mouseX, float mouseY) {
	if (mouseIsOver(mouseX, mouseY)) {
		halfPressed = enabled;
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
	setHover(enabled && mouseIsOver(mouseX, mouseY));
	if (halfPressed) {
		updateCallback((float) currentStep / steps);
		needUpdate = true;
	}
	halfPressed = false;
}

float ScrollKnob::getScaleCenter() {
	return 1.f;
}

void ScrollKnob::align(bool reset) {
	if (reset || !aligned) {
		alignedOffset = offset;
		alignedOffset.x -= minSize.x / Ar * alignment / 2;
		aligned = true;
	}
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
