/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <GroupView.h>
#include <Invoker.h>
#include <SupportDefs.h>
#include "Draggable.h"

class TabView;
class GTabView;

class TabsContainer : public BGroupView, public BInvoker, public Draggable {
public:
	TabsContainer(GTabView* tabView, BMessage* message = nullptr);
	void AddTab(TabView* tab);
	int32 CountTabs();

	TabView* TabAt(int32 index);
	int32	IndexOfTab(TabView* tab);

	void	ShiftTabs(int32 delta);

	void MouseDown(TabView* tab, BPoint where);

	void FrameResized(float w, float h) override;

private:

	void	_UpdateScrolls();

	TabView*	fSelectedTab;
	GTabView*	fGTabView;
	int32		fTabShift;
};
