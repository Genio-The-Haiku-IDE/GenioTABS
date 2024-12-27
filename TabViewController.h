/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <GroupView.h>
#include <SupportDefs.h>

class TabView;

class TabViewController : public BGroupView {
public:
	TabViewController();
	void AddTab(TabView* tab);
	int32 CountTabs();

	TabView* TabAt(int32 index);


	void MouseDown(TabView* tab, BPoint where);

private:
	TabView* fSelectedTab;
};
