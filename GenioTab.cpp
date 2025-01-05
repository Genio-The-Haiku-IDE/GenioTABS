/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GenioTab.h"

#include <GroupLayout.h>



GTabContainer::GTabContainer(BView* view):BBox(view->Name()),fView(view)
{
	SetBorder(B_NO_BORDER);
	SetLayout(new BGroupLayout(B_VERTICAL));
	if (fView)
		AddChild(fView);
}



GenioTab::GenioTab(BView* view, tab_id id, bool withCloseButton):BTab(),
		fGTabContainer(nullptr),
		fTabId(id),
		fWithCloseButton(withCloseButton)
{
	fGTabContainer = new GTabContainer(view);
	SetView(fGTabContainer);
}

GenioTab::GenioTab(const GenioTab* fromTab):BTab()
{
	fGTabContainer = new GTabContainer(fromTab->fGTabContainer->ContentView());
	fTabId = fromTab->fTabId;
	fWithCloseButton = fromTab->fWithCloseButton;
	SetView(fGTabContainer);
}
/*
BSize
GenioTab::MaxSize()
{
	return BTab::MaxSize();
}

BSize
GenioTab::PreferredSize()
{
	return BTab::PreferredSize();
}
*/

#include <ControlLook.h>

void
GenioTab::DrawLabel(BView* owner, BRect frame)
{
	if (fWithCloseButton)
		frame.right -= kCloseButtonWidth;
	BTab::DrawLabel(owner, frame);
}

void
GenioTab::DrawTab(BView* owner, BRect frame, tab_position position, bool full)
{
	BTab::DrawTab(owner, frame, position, full);
	if (fWithCloseButton)
		_DrawCloseButton(owner, frame);
}

const int32 kBrightnessBreakValue = 126;

static void inline
IncreaseContrastBy(float& tint, const float& value, const int& brightness)
{
	tint *= 1 + ((brightness >= kBrightnessBreakValue) ? +1 : -1) * value;
}

BRect
GenioTab::RectCloseButton(const BRect& tabRect)
{
	BRect frame  = tabRect;
	frame.right -= 2;
	frame.left   = frame.right - kCloseButtonWidth ;
	frame.right  = frame.left + 15;
	frame.top   += (frame.Height() / 2.0f) - 8;
	frame.bottom = frame.top + 15;
	return frame;
}

void GenioTab::_DrawCloseButton(BView* owner, BRect& tabRect)
{
	BRect closeRect = RectCloseButton(tabRect);

	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	float tint = B_LIGHTEN_1_TINT;
	if (base.Brightness() >= kBrightnessBreakValue) {
		tint = B_DARKEN_1_TINT *1.2;
	}

	if (fOverCloseRect) {
		// Draw the button frame
		be_control_look->DrawButtonFrame(owner, closeRect, closeRect,
			base, base,
			BControlLook::B_ACTIVATED | BControlLook::B_BLEND_FRAME);
		rgb_color background = ui_color(B_PANEL_BACKGROUND_COLOR);
		be_control_look->DrawButtonBackground(owner, closeRect, closeRect,
			background, BControlLook::B_ACTIVATED);
	}

	// Draw the ×
	if (fClicked)
		IncreaseContrastBy(tint, .2, base.Brightness());

	base = tint_color(base, tint);
	owner->SetHighColor(base);
	owner->SetPenSize(2);

	owner->StrokeLine(closeRect.LeftTop(), closeRect.RightBottom());
	owner->StrokeLine(closeRect.LeftBottom(), closeRect.RightTop());
	owner->SetPenSize(1);
}


