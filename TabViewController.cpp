/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabViewController.h"
#include "TabView.h"

class Filler : public BView
{
	public:
		Filler():BView("filler", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)	{ }

		void Draw(BRect rect)
		{
			rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
			uint32 borders = BControlLook::B_TOP_BORDER | BControlLook::B_BOTTOM_BORDER;
			be_control_look->DrawInactiveTab(this, rect, rect, base, 0, borders);
		}
};


TabViewController::TabViewController():BGroupView(B_HORIZONTAL, 0.0f), fSelectedTab(nullptr)
{
	GroupLayout()->AddView(0, new Filler());
	SetExplicitMinSize(BSize(100, GetH()));
	SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
}


void
TabViewController::AddTab(TabView* tab)
{
	BLayoutItem* item = GroupLayout()->AddView(CountTabs(), tab);
	tab->SetLayoutItem (item);

	if (CountTabs() == 1) {
		printf("Adding %s\n", tab->Label());
		tab->Update(false, false, true);
		fSelectedTab = tab;
	} else {
		TabView* prev = TabAt(CountTabs()-2);
		printf("Prev: %s\n", prev->Label());
		prev->Update(false /* ?? */, false, prev == fSelectedTab);
		tab->Update(false, false, false);
	}
}

int32
TabViewController::CountTabs()
{
	return GroupLayout()->CountItems() - 1; //exclude the Filler.
}

TabView*
TabViewController::TabAt(int32 index)
{
	if (index < 0 || index >= CountTabs())
		return nullptr;

	return dynamic_cast<TabView*>(GroupLayout()->ItemAt(index)->View());
}


void
TabViewController::MouseDown(TabView* tab, BPoint where)
{
	if (tab == fSelectedTab)
		return;

	int32 index = GroupLayout()->IndexOfItem(tab->LayoutItem());

	if (fSelectedTab != nullptr)
		fSelectedTab->Update(false, tab->IsLast(), false);

	tab->Update(false, tab->IsLast(), true);

	fSelectedTab = tab;

	printf("INDEX %d\n", index);

}