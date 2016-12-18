#pragma once

#include "Drawable.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

namespace Drawlist {

	struct DrawElement {
		bool isUsed = false;
		Drawable* item = nullptr;

		DrawElement(bool isUsed_, Drawable* item_) : isUsed(isUsed_), item(item_) {}
	};

	std::vector<DrawElement> mList;

	int add(Drawable* d) {
		int index = -1;

		for (int i = 0; i < mList.size(); i++) {
			if (!mList[i].isUsed) {
				mList[i].isUsed = true;
				mList[i].item = d;
				index = i;
				break;
			}
		}

		if (index = -1) {
			DrawElement element(true, d);
			mList.push_back(element);
			index = mList.size() - 1;
		}

		return index;
	}

	void remove(int index) {
		if (index != -1 && index < (mList.size())) {
			mList.at(index).isUsed = false;
			mList.at(index).item = nullptr;
		}
	}

	void draw(GLuint program) {
		for (DrawElement& e : mList) {
			if (e.isUsed) {
				e.item->Draw(program);
			}
		}
	}
}