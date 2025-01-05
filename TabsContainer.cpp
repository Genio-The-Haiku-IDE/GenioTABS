/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabsContainer.h"
#include "TabView.h"
#include "GTabView.h"
#include <cassert>

TabsContainer::TabsContainer(GTabView* tabView, BMessage* message):
	BGroupView(B_HORIZONTAL, 0.0f),
	BInvoker(message, nullptr, nullptr),
	fSelectedTab(nullptr),
	fGTabView(tabView),
	fTabShift(0),
	fAffinity(0)
{
	SetFlags(Flags()|B_FRAME_EVENTS);
	GroupLayout()->AddView(0, new Filler(this));
	SetExplicitMinSize(BSize(100, TabViewTools::DefaultTabHeigh()));
	SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
}


void
TabsContainer::AddTab(TabView* tab, int32 index, bool select)
{
	if (index == -1)
		index = CountTabs();

	BLayoutItem* item = GroupLayout()->AddView(index, tab);
	tab->SetLayoutItem (item);

	if (select || CountTabs() == 1) {
		SelectTab(tab);
	}

	ShiftTabs(0);

	_PrintToStream();
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
	if (fSelectedTab == nullptr)
		return -1;
	return GroupLayout()->IndexOfItem(tab->LayoutItem());
}


TabView*
TabsContainer::RemoveTab(TabView* tab)
{
	tab->LayoutItem()->RemoveSelf();
	tab->RemoveSelf();


	if (CountTabs() == 0) {
		SelectTab(nullptr);
	} else {
		SelectTab(TabAt(0));
	}

	delete tab->LayoutItem();
	tab->SetLayoutItem(nullptr);

	ShiftTabs(0);

	return tab;
}


TabView*
TabsContainer::SelectedTab()
{
	return fSelectedTab;
}


void
TabsContainer::SelectTab(TabView* tab, bool invoke)
{
	if (tab != fSelectedTab) {
		if (fSelectedTab)
			fSelectedTab->SetIsFront(false);

		fSelectedTab = tab;

		if (fSelectedTab)
			fSelectedTab->SetIsFront(true);

		if (invoke && Message() && Target()) {
			BMessage msg = *Message();
			msg.AddPointer("tab", fSelectedTab);
			msg.AddInt32("index", IndexOfTab(fSelectedTab));
			Invoke(&msg);
		}
	}
}



void
TabsContainer::ShiftTabs(int32 delta)
{
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
}



void
TabsContainer::MouseDown(TabView* tab, BPoint where)
{
	SelectTab(tab);
	OnMouseDown(where);
}



void
TabsContainer::FrameResized(float w, float h)
{
	//Auto-scroll:
	if (fTabShift > 0) {
		int32 tox = 0;
		TabView* last = TabAt(CountTabs()-1);
		float right =  last->Frame().right;
		for (int32 i=fTabShift - 1;i>=0;i--){
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
TabsContainer::OnDropTab(TabView* toTab, BMessage* message)
{
	TabView*		fromTab = (TabView*)message->GetPointer("tab_view", nullptr);
	TabsContainer*	fromContainer = (TabsContainer*)message->GetPointer("tab_container", nullptr);

	if (fromTab == nullptr || fromContainer == nullptr || toTab == fromTab)
		return;

	fGTabView->MoveTabs(fromTab, toTab, fromContainer);
}


void
TabsContainer::_PrintToStream()
{
	for (int32 i=0;i<GroupLayout()->CountItems();i++) {
		printf("%d) %s\n", i, GroupLayout()->ItemAt(i)->View()->Name());
	}
}



void
TabsContainer::_UpdateScrolls()
{
	if (CountTabs() > 0) {
		GroupLayout()->Relayout(true);
		TabView* last = TabAt(CountTabs() - 1);
		if (last == nullptr) {
			debugger("qui");
			printf("Count tabs: %d\n", CountTabs() - 1);
			last = TabAt(CountTabs() - 1);
			_PrintToStream();
		}

		if(fGTabView != nullptr && last != nullptr)
			fGTabView->UpdateScrollButtons(fTabShift != 0, last->Frame().right > Bounds().right);
	}
}

