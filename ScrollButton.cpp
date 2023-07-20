#include "ScrollButton.hpp"

ScrollButton::ScrollButton(std::string text, glm::vec2 offset, float alignRight, glm::vec2 minSize, Button *adjacentButton) :
		Button([](Button&) {}, text, offset, alignRight, minSize, adjacentButton) {
	type = BUTTON_SCROLL;
	info = buttonInfo.at(type);
}

bool ScrollButton::processMousePressed(float mouseX, float mouseY) {
	return (mouseIsOver(mouseX, mouseY));
}

void ScrollButton::processMouseNotPressed(float mouseX, float mouseY) {
}
