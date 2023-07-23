#pragma once

#include "Button.hpp"

class ScrollButton: public Button {
public:
	ScrollButton(std::string text, glm::vec2 offset, Alignment alignment = ALIGN_LEFT, glm::vec2 minSize = glm::vec2(0), Button *adjacentButton = nullptr);
	bool processMousePressed(float mouseX, float mouseY) override;
	void processMouseNotPressed(float mouseX, float mouseY) override;
};
