#pragma once

#include <vector>

template <
	typename InputType
>
class Action {
public:
	virtual void doAction(InputType input) = 0;
};

template <
	typename InputType
>
class GenericActionList {

	struct Element {
		Action<InputType>* item = nullptr;
		bool isUsed = false;

		Element(bool isUsed_, Action<InputType>* item_) : item(item_), isUsed(isUsed_) {}
	};

	std::vector<Element> mList;

public:

	unsigned int add(Action<InputType>* e) {
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

	void doActions(InputType input) {
		for (Element& e : mList) {
			if (e.isUsed) {
				e.item->doAction(input);
			}
		}
	}
};