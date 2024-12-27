/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GTabView.h"
#include "TabButtons.h"
#include "TabView.h"
#include "TabViewController.h"

GTabView::GTabView() :
		BGroupView(B_VERTICAL, 0.0f),
		fScrollLeftTabButton(nullptr),
		fTabViewController(nullptr),
		fScrollRightTabButton(nullptr),
		fTabMenuTabButton(nullptr)
{
	_Init();
}

void
GTabView::AddTab(const char* label)
{
	fTabViewController->AddTab(new TabView(label, fTabViewController));
}


void
GTabView::_Init()
{
	fScrollLeftTabButton = new ScrollLeftTabButton(nullptr);
	fTabViewController = new TabViewController();
	fScrollRightTabButton = new ScrollRightTabButton(nullptr);
	fTabMenuTabButton = new TabMenuTabButton(nullptr);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.AddGroup(B_HORIZONTAL, 0.0f)
			.Add(fScrollLeftTabButton)
			.Add(fTabViewController)
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
}