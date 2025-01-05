/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <GroupView.h>
#include <Invoker.h>
#include <SupportDefs.h>
#include "Draggable.h"

class GTab;
class GTabView;

typedef uint32 tab_affinity;

class TabsContainer : public BGroupView, public BInvoker, public Draggable {
public:

			TabsContainer(GTabView* tabView, BMessage* message = nullptr);

	void	AddTab(GTab* tab, int32 index = -1, bool select = false);

	int32 	CountTabs();

	GTab* TabAt(int32 index); //used?

	GTab* RemoveTab(GTab* tab); //just remove, not delete.

	int32	IndexOfTab(GTab* tab);

	void	ShiftTabs(int32 delta); // 0 to refresh the current state

	void MouseDown(GTab* tab, BPoint where);

	void FrameResized(float w, float h) override;

	void OnDropTab(GTab* toTab, BMessage* message);

	GTab*	SelectedTab();

	void	SelectTab(GTab* tab, bool invoke = true);

	GTabView*	GetGTabView() { return fGTabView; }

	tab_affinity	GetAffinity() { return fAffinity; }
	void			SetAffinity(tab_affinity aff) { fAffinity = aff; }

private:
	void	_PrintToStream();
	void	_UpdateScrolls();

	GTab*	fSelectedTab;
	GTabView*	fGTabView;
	int32		fTabShift;
	tab_affinity	fAffinity;
};
