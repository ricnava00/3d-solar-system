#pragma once

#include "Button.hpp"

class ScrollKnob: public Button {
private:
	std::function<void(float)> updateCallback;
	int steps;
	int currentStep;
	float widthFactor;

	int getStepFromMouseX(float mouseX);

public:
	ScrollKnob(std::function<void(float)> updateCallback, int steps, int initialStep, float widthFactor, glm::vec2 offset, Alignment alignment = ALIGN_LEFT, glm::vec2 minSize = glm::vec2(0), Button *adjacentButton = nullptr);
	void setOffset(const glm::vec2 offset);
	void align(bool reset = false);
	float getScaleCenter() override;
	bool processMousePressed(float mouseX, float mouseY) override;
	void processMouseHeld(float mouseX, float mouseY) override;
	void processMouseNotPressed(float mouseX, float mouseY) override;
	Geometry getSideGeometry(int side) override;
};
