#include "Button.hpp"

Button::Button(std::function<void(Button&)> callback, std::string text, glm::vec2 offset, float alignRight, glm::vec2 minSize, Button *adjacentButton) {
	this->callback = callback;
	this->text = text;
	this->offset = offset;
	this->alignedOffset = offset;
	this->alignRight = alignRight;
	this->adjacentButton = adjacentButton;
	info = buttonInfo.at(type);
	this->minSize = minSize - info.marginHorizontal * 2;
}

void Button::align(bool reset) {
	if (reset || !aligned) {
		alignedOffset = offset;
		if (alignRight) {
			alignedOffset.x -= getFullWidth();
		}
		if (adjacentButton != nullptr) {
			adjacentButton->align(); //If target button has yet to be aligned, do it first
			alignedOffset += adjacentButton->alignedOffset;
			if (!alignRight) {
				alignedOffset.x += adjacentButton->getFullWidth();
			}
		}
		aligned = true;
	}
}

void Button::updateBoundingBox() {
	Geometry geometry=getSideGeometry(0);
	boundingBox = { geometry.offset.x, geometry.offset.y, geometry.offset.x + getFullWidth(), geometry.offset.y + info.height };
}

bool Button::processMousePressed(float mouseX, float mouseY) {
	if (mouseIsOver(mouseX, mouseY)) {
		halfPressed = true;
		return true;
	}
	return false;
}

void Button::processMouseNotPressed(float mouseX, float mouseY) {
	setHover(mouseIsOver(mouseX, mouseY));
	if (hover && halfPressed) {
		callback(*this);
		halfPressed = false;
	}
}

Geometry Button::getTextGeometry() {
	glm::vec2 textOffset;
	textOffset.x = (getFullWidth() - textSize.x / Ar) / 2;
	textOffset.y = (info.height - textSize.y) / 2 + info.offsetVertical;
	return {alignedOffset + textOffset, glm::vec2(1.0)};
}

Geometry Button::getSideGeometry(int side) {
	float scaleCenter;
	glm::vec2 offset = alignedOffset;
	if (side >= 1) {
		scaleCenter = getScaleCenter();
		offset += glm::vec2(info.widthLeft / Ar, 0);
	}
	if (side == 2) {
		offset += glm::vec2(info.widthCenter / Ar * scaleCenter, 0);
	}
	return {offset, glm::vec2(side == 1 ? scaleCenter : 1.0, 1.0)};
}
