#pragma once

#include "local_glm.h"

#include "../GenericActionList.h"
#include "GUI.h"

namespace GUI {

	class Object : public Action<GLuint> {
	private:

		glm::vec2 mClickPosition;
		glm::vec2 mClickSize;

		void(*mClickCallback)(bool& handled) = nullptr;

	protected:
		glm::vec2 mPosition;
		glm::vec2 mSize;

	public:
		Object(
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 size = glm::vec2(0.0f),
			glm::vec2 clickPosition = glm::vec2(0.0f),
			glm::vec2 clickSize = glm::vec2(0.0f)
		) :
			mPosition(position),
			mSize(size),
			mClickPosition(clickPosition),
			mClickSize(clickSize) {
		}

		bool isClickInside(glm::vec2 clickPosition) const {
			// A vector from bottom right to click position
			glm::vec2 a = clickPosition - mClickPosition;
			// A vector from click position to upper left
			glm::vec2 b = (mClickPosition + mClickSize) - clickPosition;

			// if the click position is inside of the angle betweeen a and b
			// will be 90d < theta < 180d 
			//       = pi/2 < theta < pi
			float theta = glm::angle(a, b);
			return (glm::pi<float>() / 2.0f) < theta &&
				theta < glm::pi<float>();
		}

		void handleClick(bool& handled) {
			if (mClickCallback != nullptr) {
				mClickCallback(handled);
			} else {
				handled = false;
			}
		}

		void registerClickCallback(void(*clickCallback)(bool& handled)) {
			mClickCallback = clickCallback;
		}
	};

}