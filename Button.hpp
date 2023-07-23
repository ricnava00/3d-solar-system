#pragma once

#include <functional>
#include <string>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

enum ButtonType {
	BUTTON,
	BUTTON_SCROLL,
	SCROLL_KNOB
};

enum Alignment {
	ALIGN_LEFT,
	ALIGN_CENTER,
	ALIGN_RIGHT
};

struct ButtonInfo {
	std::string textureName;
	float widthLeft;
	float widthCenter;
	float widthRight;
	float height;
	float marginHorizontal;
	float offsetVertical;
};

const std::map<ButtonType, ButtonInfo> buttonInfo={
		{BUTTON,{
			"Button",
			134.0f/800/2,
			27.0f/800/2,
			134.0f/800/2,
			321.0f/800/2,
			60.0f/800,
			0.0f,
		}},
		{BUTTON_SCROLL,{
			"Button_scroll",
			134.0f/800/2,
			27.0f/800/2,
			134.0f/800/2,
			477.0f/800/2,
			60.0f/800,
			-78.0f/800/2
		}},
		{SCROLL_KNOB,{
			"Scroll_knob",
			44.0f/800/1.5,
			1.0f/800/1.5,
			44.0f/800/1.5,
			93.0f/800/1.5,
			0.0f,
			0.0f
		}}
};

struct Geometry {
	glm::vec2 offset;
	glm::vec2 scale;
};

struct BoundingBox {
	float left;
	float top;
	float right;
	float bottom;
};

class Button {
protected:
	ButtonType type = BUTTON;
	std::function<void(Button&)> callback;
	std::string text;
	glm::vec2 offset;
	glm::vec2 alignedOffset;
	Button *adjacentButton = nullptr;
	Alignment alignment;
	glm::vec2 minSize = glm::vec2(0);
	glm::vec2 textSize;
	bool visible = true;
	bool hover = false;
	bool active = true;
	bool enabled = true;
	float UIScale = 1.f;
	BoundingBox boundingBox;
	float Ar = 1.f;
	float aligned = false;
	bool halfPressed = false;
	bool needUpdate = false;
	ButtonInfo info;

public:
	Button(std::function<void(Button&)> callback, std::string text, glm::vec2 offset, Alignment alignment = ALIGN_LEFT, glm::vec2 minSize = glm::vec2(0), Button *adjacentButton = nullptr);

	virtual float getScaleCenter() {
		return ceil(std::max(0.0F, minSize.x + 2 * info.marginHorizontal - info.widthLeft - info.widthRight) / info.widthCenter);
	}

	bool mouseIsOver(float mouseX, float mouseY) {
		return (visible && mouseX >= boundingBox.left && mouseX <= boundingBox.right && mouseY >= boundingBox.top && mouseY <= boundingBox.bottom);
	}

	float getFullWidth() {
		return (info.widthLeft + info.widthRight + info.widthCenter * getScaleCenter()) / Ar;
	}

	ButtonType getType() const {
		return type;
	}

	virtual void setOffset(const glm::vec2 offset) {
		this->offset = offset;
	}

	bool getAlignment() const {
		return alignment;
	}

	void setAlignment(Alignment alignment) {
		this->alignment = alignment;
	}

	Button* getAdjacentButton() const {
		return adjacentButton;
	}

	void setAdjacentButton(Button *adjacentButton) {
		this->adjacentButton = adjacentButton;
	}

	glm::vec2 getMinSize() const {
		return minSize;
	}

	void setMinSize(glm::vec2 minSize) {
		this->minSize = minSize;
	}

	const std::string getText() const {
		return text;
	}

	void setText(const std::string text) {
		this->text = text;
		needUpdate = true;
	}

	const glm::vec2 getTextSize() const {
		return textSize;
	}

	void setTextSize(const glm::vec2 textSize) {
		this->textSize = textSize;
	}

	const ButtonInfo& getInfo() const {
		return info;
	}

	bool isEnabled() const {
		return enabled;
	}

	void setEnabled(bool enabled) {
		halfPressed &= enabled;
		if (this->enabled != enabled) {
			this->enabled = enabled;
			needUpdate = true;
		}
	}

	bool isActive() const {
		return active;
	}

	void setActive(bool active) {
		if (this->active != active) {
			this->active = active;
			needUpdate = true;
		}
	}

	bool isVisible() const {
		return visible;
	}

	void setVisible(bool visible) {
		if (this->visible != visible) {
			this->visible = visible;
			needUpdate = true;
		}
	}

	bool isHover() const {
		return hover;
	}

	void setHover(bool hover) {
		if (this->hover != hover) {
			this->hover = hover;
			needUpdate = true;
		}
	}

	void setUIScale(float UIScale) {
		minSize *= UIScale / this->UIScale;
		info.widthLeft *= UIScale / this->UIScale;
		info.widthCenter *= UIScale / this->UIScale;
		info.widthRight *= UIScale / this->UIScale;
		info.height *= UIScale / this->UIScale;
		info.marginHorizontal *= UIScale / this->UIScale;
		info.offsetVertical *= UIScale / this->UIScale;
		this->UIScale = UIScale;
		needUpdate = true;
	}

	void setAr(float ar) {
		Ar = ar;
		needUpdate = true;
	}

	bool needsUpdate() {
		bool u = needUpdate;
		needUpdate = false;
		return u;
	}

	virtual void align(bool reset = false);
	void updateBoundingBox();
	virtual bool processMousePressed(float mouseX, float mouseY);
	virtual void processMouseHeld(float mouseX, float mouseY) {}
	virtual void processMouseNotPressed(float mouseX, float mouseY);
	virtual Geometry getTextGeometry();
	virtual Geometry getSideGeometry(int side);
};
