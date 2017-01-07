#pragma once

#include <vector>

template <
	typename T
>
class GenericDB {

	struct Element {
		T item;
		bool isUsed = false;

		Element(bool isUsed_, T item_) : isUsed(isUsed_), item(item_) {}
	};

	std::vector<Element> mList;

public:

	unsigned int add(T e) {
		unsigned int index = 0;
		bool found = false;

		// Check if there's an open slot we can use
		for (unsigned int i = 0; i < mList.size(); ++i) {
			if (!mList[i].isUsed) {
				mList[i].isUsed = true;
				mList[i].item = e;
				index = i;
				found = true;
				break;
			}
		}

		// Otherwise push the element to the end
		if (!found) {
			Element element(true, e);
			mList.push_back(element);
			index = mList.size() - 1;
		}

		return index;
	}

	void remove(unsigned int index) {
		if (index != -1 && index < (mList.size())) {
			mList.at(index).isUsed = false;
			mList.at(index).item = nullptr;
		}
	}

	const T& at(unsigned int index) {
		return mList.at(index).item;
	}
};