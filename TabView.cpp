/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabView.h"
#include <ControlLook.h>
#include "TabsContainer.h"

TabView::TabView(const char* label, TabsContainer* controller)
	: BControl("_tabView_", label, nullptr, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	fIsFirst(false),
	fIsLast(false),
	fTabsContainer(controller)
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
	BRect frame(Bounds()); //fLayoutItem->Frame());

	if (Value() == B_CONTROL_ON) {
		// Extend the front tab outward left/right in order to merge
		// the frames of adjacent tabs.
		if (!fIsFirst)
			frame.left--;
		if (!fIsLast)
			frame.right++;
	}
	//frame.bottom++;

	DrawBackground(this, frame, updateRect, fIsFirst, fIsLast,
		Value() == B_CONTROL_ON);

	if (Value() == B_CONTROL_ON) {
		frame.top += 0.0f;
		if (!fIsFirst)
			frame.left++;
		if (!fIsLast)
			frame.right--;
	} else
		frame.top += 3.0f;

	float spacing = be_control_look->DefaultLabelSpacing();
	frame.InsetBy(spacing, spacing / 2);
	DrawContents(this, frame, updateRect, fIsFirst, fIsLast,
		Value() == B_CONTROL_ON);
}


void
TabView::DrawBackground(BView* owner, BRect frame, const BRect& updateRect,
	bool isFirst, bool isLast, bool isFront)
{
	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	uint32 borders = BControlLook::B_TOP_BORDER
		| BControlLook::B_BOTTOM_BORDER;

	if (isFirst)
		borders |= BControlLook::B_LEFT_BORDER;
	if (isLast)
		borders |= BControlLook::B_RIGHT_BORDER;
	if (isFront) {
		be_control_look->DrawActiveTab(owner, frame, updateRect, base,
			0, borders);
	} else {
		be_control_look->DrawInactiveTab(owner, frame, updateRect, base,
			0, borders);
	}
}


void
TabView::DrawContents(BView* owner, BRect frame, const BRect& updateRect,
	bool isFirst, bool isLast, bool isFront)
{
	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	be_control_look->DrawLabel(owner, Label(), frame, updateRect,
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
	Update(fIsFirst, fIsLast, isFront);
}


bool
TabView::IsFront() const
{
	return Value() == B_CONTROL_ON;
}


void
TabView::SetIsLast(bool isLast)
{
	Update(fIsFirst, isLast, Value() == B_CONTROL_ON);
}


void
TabView::Update(bool isFirst, bool isLast, bool isFront)
{
	int32 control = isFront ?  B_CONTROL_ON : B_CONTROL_OFF;
	if (fIsFirst == isFirst && fIsLast == isLast && Value() == control)
		return;

	fIsFirst = isFirst;
	fIsLast = isLast;
	SetValue(control);
	Invalidate();
}

/*
void
TabView::SetContainerView(TabContainerView* containerView)
{
	fContainerView = containerView;
}


TabContainerView*
TabView::ContainerView() const
{
	return fContainerView;
}
*/
/*
BLayoutItem*
TabView::LayoutItem() const
{
	return nullptr;//TabButton::LayoutItem();
}
*/
/*
void
TabView::SetLabel(const char* label)
{
	BControl::SetLabel(label);
	Invalidate();
}
*/

