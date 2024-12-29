/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabsContainer.h"
#include "TabView.h"
#include "GTabView.h"
#include <cassert>

class Filler : public BView
{
	public:
		Filler():BView("filler", B_WILL_DRAW /*| B_FULL_UPDATE_ON_RESIZE*/)	{ }

		void Draw(BRect rect)
		{
			BRect bounds(Bounds());
			TabViewTools::DrawTabBackground(this, bounds, rect);
		}
};

TabsContainer::TabsContainer(GTabView* tabView, BMessage* message):
	BGroupView(B_HORIZONTAL, 0.0f),
	BInvoker(message, nullptr, nullptr),
	fSelectedTab(nullptr),
	fGTabView(tabView),
	fTabShift(0)
{
	SetFlags(Flags()|B_FRAME_EVENTS);
	GroupLayout()->AddView(0, new Filler());
	SetExplicitMinSize(BSize(100, TabViewTools::DefaultTabHeigh()));
	SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
}


void
TabsContainer::AddTab(TabView* tab)
{
	BLayoutItem* item = GroupLayout()->AddView(CountTabs(), tab);
	tab->SetLayoutItem (item);

	if (CountTabs() == 1) {
		//printf("Adding %s\n", tab->Label());
		tab->SetIsFront(true);
		fSelectedTab = tab;
	} else {
		TabView* prev = TabAt(CountTabs()-2);
		//printf("Prev: %s\n", prev->Label());
		prev->SetIsFront(prev == fSelectedTab);
		tab->SetIsFront(false);
	}
}

int32
TabsContainer::CountTabs()
{
	return GroupLayout()->CountItems() - 1; //exclude the Filler.
}

TabView*
TabsContainer::TabAt(int32 index)
{
	if (index < 0 || index >= CountTabs())
		return nullptr;

	return dynamic_cast<TabView*>(GroupLayout()->ItemAt(index)->View());
}


int32
TabsContainer::IndexOfTab(TabView* tab)
{
	assert(tab);
	return GroupLayout()->IndexOfItem(tab->LayoutItem());
}

void
TabsContainer::ShiftTabs(int32 delta)
{
	//printf("1) ShiftTabs %d\n", fTabShift);
	int32 newShift = fTabShift + delta;
	if (newShift < 0)
		newShift = 0;

	int32 max = std::max(newShift, fTabShift);

	for (int32 i=0;i<max;i++) {
		TabView* tab = TabAt(i);
		if (i < newShift) {
			if (tab->IsHidden() == false)
				tab->Hide();
		} else {
			if (tab->IsHidden() == true)
				tab->Show();
		}
	}

	fTabShift = newShift;

	_UpdateScrolls();
	//printf("2) ShiftTabs %d\n", fTabShift);
}



void
TabsContainer::MouseDown(TabView* tab, BPoint where)
{
	if (tab != fSelectedTab) {
		int32 index = IndexOfTab(tab);

		if (fSelectedTab != nullptr)
			fSelectedTab->SetIsFront(false);

		tab->SetIsFront(true);

		fSelectedTab = tab;


		if (Message() && Target()) {
			BMessage msg = *Message();
			msg.AddPointer("tab", tab);
			msg.AddInt32("index", index);
			Invoke(&msg);
		}
	}
	OnMouseDown(where);
}

void
TabsContainer::FrameResized(float w, float h)
{
	//Auto-scroll:
	if (fTabShift > 0) {
		//ts 1 -> 0
		//ts 2 -> 0-1
		int32 tox = 0;
		TabView* last = TabAt(CountTabs()-1);
		float right =  last->Frame().right;
		for (int32 i=fTabShift - 1;i>=0;i--){
			//int32 idx = fTabShift - i - 1;
//			printf("IDX %d\n", i);
			TabView* tab = TabAt(i);
			right =  right + tab->Frame().Width();
			if (right < w)
				tox--;
			else
				break;
		}
		if (tox != 0)
			ShiftTabs(tox);
	}
	//end
	_UpdateScrolls();
	BGroupView::FrameResized(w,h);
}

void
TabsContainer::_UpdateScrolls()
{
	if (CountTabs() > 0) {
		GroupLayout()->Relayout(true);
		TabView* last = TabAt(CountTabs()-1);

		if(fGTabView != nullptr)
			fGTabView->UpdateScrollButtons(fTabShift != 0, last->Frame().right > Bounds().right);
	}
}