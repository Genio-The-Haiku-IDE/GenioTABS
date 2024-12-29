/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabView.h"
#include <ControlLook.h>
#include "TabsContainer.h"

TabView::TabView(const char* label, TabsContainer* controller)
	: BView("_tabView_", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	fTabsContainer(controller), fIsFront(false), fLabel(label)
{
}


TabView::~TabView()
{
}


BSize
TabView::MinSize()
{
	BSize size(MaxSize());
	size.width = 100.0f;
	return size;
}

BSize
TabView::MaxSize()
{
	float extra = be_control_look->DefaultLabelSpacing();
	float labelWidth = 150.0f;
	return BSize(labelWidth, TabViewTools::DefaultTabHeigh());
}


void
TabView::Draw(BRect updateRect)
{
	BRect frame(Bounds());
    if (fIsFront) {
        frame.left--;
        frame.right++;
	}

	DrawBackground(this, frame, updateRect, fIsFront);

	if (fIsFront) {
		frame.top += 0.0f;
	} else
		frame.top += 3.0f;

	float spacing = be_control_look->DefaultLabelSpacing();
	frame.InsetBy(spacing, spacing / 2);
	DrawContents(this, frame, updateRect, fIsFront);
}


void
TabView::DrawBackground(BView* owner, BRect frame, const BRect& updateRect, bool isFront)
{
	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	uint32 borders = BControlLook::B_TOP_BORDER | BControlLook::B_BOTTOM_BORDER;

	if (isFront) {
		be_control_look->DrawActiveTab(owner, frame, updateRect, base,
			0, borders);
	} else {
		be_control_look->DrawInactiveTab(owner, frame, updateRect, base,
			0, borders);
	}
}


void
TabView::DrawContents(BView* owner, BRect frame, const BRect& updateRect, bool isFront)
{
	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	be_control_look->DrawLabel(owner, fLabel.String(), frame, updateRect,
		base, 0, BAlignment(B_ALIGN_LEFT, B_ALIGN_MIDDLE));
}


void
TabView::MouseDown(BPoint where)
{
 	if (fTabsContainer)
		fTabsContainer->MouseDown(this, where);
}


void
TabView::MouseUp(BPoint where)
{
}


void
TabView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
}

void
TabView::SetIsFront(bool isFront)
{
	if (fIsFront == isFront)
		return;

	fIsFront = isFront;
	Invalidate();
}

bool
TabView::IsFront() const
{
	return fIsFront;
}

