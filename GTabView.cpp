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
	kRightTabButton = 'GTrb',
	kSelectedTabButton = 'GTse'

};

GTabView::GTabView(bool closeButton) :
		BGroupView(B_VERTICAL, 0.0f),
		fScrollLeftTabButton(nullptr),
		fTabsContainer(nullptr),
		fScrollRightTabButton(nullptr),
		fTabMenuTabButton(nullptr),
		fCardView(nullptr),
		fCloseButton(closeButton)
{
	_Init();
}

void
GTabView::AddTab(const char* label, BView* view)
{
	fTabsContainer->AddTab(CreateTabView(label));
	fCardView->AddChild(view);
}


void
GTabView::CloseTab(TabView* tab)
{
	//Remove the View from CardView
	int32 fromIndex = fTabsContainer->IndexOfTab(tab);
	BLayoutItem* fromLayout = fCardView->CardLayout()->ItemAt(fromIndex);
	BView*	fromView = fromLayout->View();
	if (!fromView)
		return;

	fromView->RemoveSelf();

	fCardView->CardLayout()->RemoveItem(fromLayout);

	delete fTabsContainer->RemoveTab(tab);
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
	fTabsContainer->SetTarget(this);
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
		case kSelectedTabButton:
		{
			int32 index = message->GetInt32("index", 0);
			if (index > -1)
				fCardView->CardLayout()->SetVisibleItem(index);
		}
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

	fTabsContainer = new TabsContainer(this, new BMessage(kSelectedTabButton));
	fTabMenuTabButton = new TabMenuTabButton(nullptr);

	fCardView = new BCardView("_cardview_");

	fTabsContainer->SetAffinity('AFFY');

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
		.Add(fCardView)
		//.Add(new BStringView("test", "text", B_WILL_DRAW))
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


void
GTabView::MoveTabs(TabView* fromTab, TabView* toTab, TabsContainer* fromContainer)
{
	//Remove the View from CardView
	int32 fromIndex = fromContainer->IndexOfTab(fromTab);
	int32 toIndex = toTab == nullptr ? fTabsContainer->CountTabs() : fTabsContainer->IndexOfTab(toTab);

	BLayoutItem* fromLayout = fCardView->CardLayout()->ItemAt(fromIndex);
	BView*	fromView = fromLayout->View();
	if (!fromView)
		return;

	fromView->RemoveSelf();

	fCardView->CardLayout()->RemoveItem(fromLayout);

	BString label = fromTab->Label(); //TODO copy all the props
	TabView* removedTab = fromContainer->RemoveTab(fromTab);
	delete removedTab;

	TabView* newTab = CreateTabView(label.String());
	fTabsContainer->AddTab(newTab, toIndex);
	fCardView->CardLayout()->AddItem(toIndex, fromLayout);
	fTabsContainer->SelectTab(newTab);
}


TabView*
GTabView::CreateTabView(const char* label)
{
	return fCloseButton ? new TabViewCloseButton(label, fTabsContainer)
						: new TabView(label, fTabsContainer);
}








