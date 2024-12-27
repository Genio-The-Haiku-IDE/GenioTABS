/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <ControlLook.h>
#include <SupportDefs.h>
#include <Button.h>
#include <Window.h>

float GetH();
class TabButton : public BButton {
public:
	TabButton(const char* label = nullptr, BMessage* message = nullptr)
		: BButton(label, message)
	{
		SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
		SetExplicitMaxSize(BSize(50, B_SIZE_UNSET));
		SetExplicitMinSize(BSize(50, B_SIZE_UNSET));
		SetExplicitPreferredSize(BSize(50, B_SIZE_UNSET));

		fTabHeight = GetH();
	}

	virtual BSize MinSize()
	{
		return BSize(20, fTabHeight);
	}

	virtual BSize MaxSize()
	{
		return MinSize();
	}

	virtual BSize PreferredSize()
	{
		return MinSize();
	}

	virtual void Draw(BRect updateRect)
	{

		BRect bounds(Bounds());
		rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
		uint32 flags = be_control_look->Flags(this);
		uint32 borders = BControlLook::B_TOP_BORDER
			| BControlLook::B_BOTTOM_BORDER;
		be_control_look->DrawInactiveTab(this, bounds, updateRect, base,
			0, borders);
		if (IsEnabled()) {
			rgb_color button = tint_color(base, 1.07);
			be_control_look->DrawButtonBackground(this, bounds, updateRect,
				button, flags, 0);
		}
		bounds.left = (bounds.left + bounds.right) / 2 - 6;
		bounds.top = (bounds.top + bounds.bottom) / 2 - 6;
		bounds.right = bounds.left + 12;
		bounds.bottom = bounds.top + 12;
		DrawSymbol(bounds, updateRect, base);
	}

	virtual void DrawSymbol(BRect frame, const BRect& updateRect,
		const rgb_color& base)
	{
	}

	float	fTabHeight;
};


class ScrollLeftTabButton : public TabButton {
public:
	ScrollLeftTabButton(BMessage* message)
		: TabButton(" ", message)
	{
	}

	virtual void DrawSymbol(BRect frame, const BRect& updateRect,
		const rgb_color& base)
	{
		float tint = IsEnabled() ? B_DARKEN_4_TINT : B_DARKEN_1_TINT;
		be_control_look->DrawArrowShape(this, frame, updateRect,
			base, BControlLook::B_LEFT_ARROW, 0, tint);
	}
};


class ScrollRightTabButton : public TabButton {
public:
	ScrollRightTabButton(BMessage* message)
		: TabButton(" ", message)
	{
	}

	virtual void DrawSymbol(BRect frame, const BRect& updateRect,
		const rgb_color& base)
	{
		frame.OffsetBy(1, 0);
		float tint = IsEnabled() ? B_DARKEN_4_TINT : B_DARKEN_1_TINT;
		be_control_look->DrawArrowShape(this, frame, updateRect,
			base, BControlLook::B_RIGHT_ARROW, 0, tint);
	}
};


class TabMenuTabButton : public TabButton {
public:
	TabMenuTabButton(BMessage* message)
		: TabButton(" ", message)
		, fCloseTime(0)
	{
	}
/*
	virtual BSize MinSize()
	{
		return BSize(18, 12);
	}*/

	virtual void DrawSymbol(BRect frame, const BRect& updateRect,
		const rgb_color& base)
	{
		be_control_look->DrawArrowShape(this, frame, updateRect,
			base, BControlLook::B_DOWN_ARROW, 0, B_DARKEN_4_TINT);
	}

	virtual void MouseDown(BPoint point)
	{
		// Don't reopen the menu if it's already open or freshly closed.
		bigtime_t clickSpeed = 2000000;
		get_click_speed(&clickSpeed);
		bigtime_t clickTime = Window()->CurrentMessage()->FindInt64("when");
		if (!IsEnabled() || (Value() == B_CONTROL_ON)
			|| clickTime < fCloseTime + clickSpeed) {
			return;
		}

		// Invoke must be called before setting B_CONTROL_ON
		// for the button to stay "down"
		Invoke();
		SetValue(B_CONTROL_ON);
	}

	virtual void MouseUp(BPoint point)
	{
		// Do nothing
	}

	void MenuClosed()
	{
		fCloseTime = system_time();
		SetValue(B_CONTROL_OFF);
	}

private:
	bigtime_t fCloseTime;
};


