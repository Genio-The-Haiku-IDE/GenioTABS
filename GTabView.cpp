/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GTabView.h"
#include "TabButtons.h"
#include "TabView.h"
#include "TabsContainer.h"

enum {

	kLeftTabButton	= 'GTlb',
	kRightTabButton = 'GTrb'

};

GTabView::GTabView() :
		BGroupView(B_VERTICAL, 0.0f),
		fScrollLeftTabButton(nullptr),
		fTabsContainer(nullptr),
		fScrollRightTabButton(nullptr),
		fTabMenuTabButton(nullptr)
{
	_Init();
}

void
GTabView::AddTab(const char* label)
{
	fTabsContainer->AddTab(new TabView(label, fTabsContainer));
}

void
GTabView::UpdateScrollButtons(bool left, bool right)
{
	_SetButtonVisibility(fScrollLeftTabButton,  left);
	_SetButtonVisibility(fScrollRightTabButton, right);
}


void
GTabView::AttachedToWindow()
{
	fScrollLeftTabButton->SetTarget(this);
	fScrollRightTabButton->SetTarget(this);
	//fTabsContainer->SetTarget(this);
	fTabMenuTabButton->SetTarget(this);
}


void
GTabView::MessageReceived(BMessage* message)
{
	switch(message->what) {
		case kLeftTabButton:
			fTabsContainer->ShiftTabs(-1);
		break;
		case kRightTabButton:
			fTabsContainer->ShiftTabs(+1);
		break;
		default:
			BGroupView::MessageReceived(message);
	};
}

void
GTabView::_Init()
{
	fScrollLeftTabButton  = new ScrollLeftTabButton(new BMessage(kLeftTabButton));
	fScrollRightTabButton = new ScrollRightTabButton(new BMessage(kRightTabButton));

	fTabsContainer = new TabsContainer(this);
	fTabMenuTabButton = new TabMenuTabButton(nullptr);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.AddGroup(B_HORIZONTAL, 0.0f)
			.Add(fScrollLeftTabButton)
			.Add(fTabsContainer)
			.AddGroup(B_HORIZONTAL, 0.0f)
				.Add(fScrollRightTabButton)
				.Add(fTabMenuTabButton)
				.SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT, B_ALIGN_VERTICAL_CENTER))
				.End()
			.SetExplicitAlignment(BAlignment(B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET))
			.End()
		.AddGlue(1)
		.Add(new BStringView("test", "text", B_WILL_DRAW))
		.AddGlue(1)
		;

	fScrollLeftTabButton->Hide();
	fScrollRightTabButton->Hide();
}

void
GTabView::_SetButtonVisibility(TabButton* button, bool newState)
{
	if (newState == false && !button->IsHidden()){
			button->Hide();
	} else if (newState == true  && button->IsHidden())
			button->Show();
}

