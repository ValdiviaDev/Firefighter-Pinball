#include "UIElem.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"

UIElem::UIElem()
{}

UIElem:: ~UIElem() 
{}

void UIElem::Update() {
	if (this != nullptr) {
		Draw();
	}
}

void UIElem::Draw() {
}


UI_TYPE UIElem::GetType() {
	return type;
}