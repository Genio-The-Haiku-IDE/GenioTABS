/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GTabView.h"
#include "TabButtons.h"
#include "GTab.h"
#include "TabsContainer.h"

enum {

	kLeftTabButton	= 'GTlb',
	kRightTabButton = 'GTrb',
	kSelectedTabButton = 'GTse'

};

GTabView::GTabView(const char* name,
				   tab_affinity affinity,
				   orientation content_orientation,
				   bool closeButton,
				   bool menuButton) :

		BGroupView(name, B_VERTICAL, 0.0f),
		fScrollLeftTabButton(nullptr),
		fTabsContainer(nullptr),
		fScrollRightTabButton(nullptr),
		fTabMenuTabButton(nullptr),
		fCardView(nullptr),
		fCloseButton(closeButton),
		fContentOrientation(content_orientation),
		fMenuButton(menuButton)
{
	_Init(affinity);
}

void
GTabView::AddTab(const char* label, BView* view)
{
	fTabsContainer->AddTab(CreateTabView(label));
	fCardView->AddChild(view);
	_FixContentOrientation(view);
}


void
GTabView::DestroyTabAndView(GTab* tab)
{
	//Remove the View from CardView
	int32 fromIndex = fTabsContainer->IndexOfTab(tab);
	BLayoutItem* fromLayout = fCardView->CardLayout()->ItemAt(fromIndex);
	BView*	fromView = fromLayout->View();
	if (!fromView)
		return;

	fromView->RemoveSelf();

	fCardView->CardLayout()->RemoveItem(fromLayout);

	GTab* rtab = fTabsContainer->RemoveTab(tab);
	if (rtab)
		delete rtab;

	delete fromView;
}


void
GTabView::UpdateScrollButtons(bool left, bool right)
{
	fScrollLeftTabButton->SetEnabled(left);
	fScrollRightTabButton->SetEnabled(right);
	fTabMenuTabButton->SetEnabled(fTabsContainer->CountTabs() > 0);
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
		case TabsContainer::kTVCloseTab:
		{
			GTab* tab = (GTab*)message->GetPointer("tab", nullptr);
			if (tab != nullptr) {
				DestroyTabAndView(tab);
			}
		}
		break;
		default:
			BGroupView::MessageReceived(message);
	};
}

void
GTabView::_Init(tab_affinity affinity)
{
	fScrollLeftTabButton  = new ScrollLeftTabButton(new BMessage(kLeftTabButton));
	fScrollRightTabButton = new ScrollRightTabButton(new BMessage(kRightTabButton));

	fTabsContainer = new TabsContainer(this, affinity, new BMessage(kSelectedTabButton));
	fTabMenuTabButton = new TabMenuTabButton(nullptr);

	fCardView = new BCardView("_cardview_");

	auto builder = BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f);

	builder.AddGroup(B_HORIZONTAL, 0.0f)
			.SetExplicitAlignment(BAlignment(B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET))
			.Add(fScrollLeftTabButton)
			.Add(fTabsContainer)
				.AddGroup(B_HORIZONTAL, 0.0f)
					.SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT, B_ALIGN_VERTICAL_CENTER))
					.Add(fScrollRightTabButton);

	if (fMenuButton == true)
		builder.Add(fTabMenuTabButton).End();

	builder.End();	// last AddGroup
	builder.End();	// first AddGroup

	builder.Add(fCardView)
		   .AddGlue(0);

	UpdateScrollButtons(false, false);
}


void
GTabView::_FixContentOrientation(BView* view)
{
	BLayout* layout = view->GetLayout();
	if (!layout)
		return;
	BGroupLayout* grpLayout = dynamic_cast<BGroupLayout*>(layout);
	if (!grpLayout) {
		return;
	}
	if (grpLayout->Orientation() != fContentOrientation)
	{
		grpLayout->SetOrientation(fContentOrientation);
	}
}


void
GTabView::MoveTabs(GTab* fromTab, GTab* toTab, TabsContainer* fromContainer)
{
	//Remove the View from CardView
	int32 fromIndex = fromContainer->IndexOfTab(fromTab);
	int32 toIndex = -1 ;

	if (toTab == nullptr) {
		toIndex =  fTabsContainer->CountTabs(); // last position
		if (fTabsContainer == fromContainer )
			toIndex--; //exclude filler?
	} else {
		toIndex = fTabsContainer->IndexOfTab(toTab);
	}

	BLayoutItem* fromLayout = fromContainer->GetGTabView()->CardView()->CardLayout()->ItemAt(fromIndex);
	BView*	fromView = fromLayout->View();
	if (!fromView)
		return;

	fromView->RemoveSelf();

	fromLayout->RemoveSelf();

	BString label = fromTab->Label(); //TODO copy all the props
	GTab* removedTab = fromContainer->RemoveTab(fromTab);
	delete removedTab;

	GTab* newTab = CreateTabView(label.String());
	fTabsContainer->AddTab(newTab, toIndex);
	fCardView->CardLayout()->AddItem(toIndex, fromLayout);
	fTabsContainer->SelectTab(newTab);
	_FixContentOrientation(fromView);
}


GTab*
GTabView::CreateTabView(const char* label)
{
	return fCloseButton ? new GTabCloseButton(label, fTabsContainer, this)
						: new GTab(label, fTabsContainer);
}








