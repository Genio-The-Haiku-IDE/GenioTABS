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

typedef uint32 tab_affinity;

class TabsContainer : public BGroupView, public BInvoker, public Draggable {
public:

			TabsContainer(GTabView* tabView, BMessage* message = nullptr);

	void	AddTab(TabView* tab, int32 index = -1, bool select = false);

	int32 	CountTabs();

	TabView* TabAt(int32 index); //used?

	TabView* RemoveTab(TabView* tab); //just remove, not delete.

	int32	IndexOfTab(TabView* tab);

	void	ShiftTabs(int32 delta); // 0 to refresh the current state

	void MouseDown(TabView* tab, BPoint where);

	void FrameResized(float w, float h) override;

	void OnDropTab(TabView* toTab, BMessage* message);

	TabView*	SelectedTab();

	void	SelectTab(TabView* tab, bool invoke = true);

	GTabView*	GetGTabView() { return fGTabView; }

	tab_affinity	GetAffinity() { return fAffinity; }
	void			SetAffinity(tab_affinity aff) { fAffinity = aff; }

private:

	void	_UpdateScrolls();

	TabView*	fSelectedTab;
	GTabView*	fGTabView;
	int32		fTabShift;
	tab_affinity	fAffinity;
};
