/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TabView.h"
#include <ControlLook.h>
#include "TabsContainer.h"
#include <Bitmap.h>

#define ALPHA 			200

TabView::TabView(const char* label, TabsContainer* controller)
	: BView("_tabView_", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE),
	fTabsContainer(controller), fIsFront(false), fLabel(label), fTabDragging(false)
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
	float labelWidth = 150.0f;
	return BSize(labelWidth, TabViewTools::DefaultTabHeigh());
}


void
TabView::Draw(BRect updateRect)
{
	DrawTab(this, updateRect);
	if (fTabDragging) {
		rgb_color color = ui_color(B_CONTROL_HIGHLIGHT_COLOR);
		color.alpha = 170;
		SetHighColor(color);
		SetDrawingMode(B_OP_ALPHA);
		FillRect(Bounds());
	}
}

void
TabView::DrawTab(BView* owner, BRect updateRect)
{
	BRect frame(owner->Bounds());
    if (fIsFront) {
        frame.left--;
        frame.right++;
	}

	DrawBackground(owner, frame, updateRect, fIsFront);

	if (fIsFront) {
		frame.top += 0.0f;
	} else
		frame.top += 3.0f;

	float spacing = be_control_look->DefaultLabelSpacing();
	frame.InsetBy(spacing, spacing / 2);
	DrawContents(owner, frame, updateRect, fIsFront);
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

	OnMouseDown(where);
}


void
TabView::MouseUp(BPoint where)
{
	OnMouseUp(where);
	if (fTabDragging) {
		fTabDragging = false;
		Invalidate();
	}
}


void
TabView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
	if (dragMessage &&
	    dragMessage->what == TAB_DRAG &&
		_ValidDragAndDrop(dragMessage)) {
		switch (transit) {
			case B_ENTERED_VIEW:
			case B_INSIDE_VIEW:
			{
				TabView* fromTab = (TabView*)dragMessage->GetPointer("tab_view", this);
				fTabDragging = (fromTab != this);
				Invalidate();
				return;
			}
			break;
			default:
			if (fTabDragging) {
				fTabDragging = false;
				Invalidate();
			}
			break;
		};
	} else {

		OnMouseMoved(where);
		if (fTabDragging) {
			fTabDragging = false;
			Invalidate();
		}
	}
}


void
TabView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case TAB_DRAG:
			if(_ValidDragAndDrop(message))
				fTabsContainer->OnDropTab(this, message);
		break;
		default:
			BView::MessageReceived(message);
		break;
	};
}



bool
TabView::InitiateDrag(BPoint where)
{
	TabView* tab = this;
	if (tab != nullptr) {
		BMessage message(TAB_DRAG);

		message.AddPointer("tab_view", this);
		message.AddPointer("tab_container", fTabsContainer);

		const BRect& updateRect = tab->Bounds();

		BBitmap* dragBitmap = new BBitmap(updateRect, B_RGB32, true);
		if (dragBitmap->IsValid()) {
			BView* view = new BView(dragBitmap->Bounds(), "helper", B_FOLLOW_NONE, B_WILL_DRAW);
			dragBitmap->AddChild(view);
			dragBitmap->Lock();
			tab->DrawTab(view, updateRect);
			view->Sync();
			uint8* bits = (uint8*)dragBitmap->Bits();
			int32 height = (int32)dragBitmap->Bounds().Height() + 1;
			int32 width = (int32)dragBitmap->Bounds().Width() + 1;
			int32 bpr = dragBitmap->BytesPerRow();
			for (int32 y = 0; y < height; y++, bits += bpr) {
				uint8* line = bits + 3;
				for (uint8* end = line + 4 * width; line < end; line += 4)
					*line = ALPHA;
			}
			dragBitmap->Unlock();
		} else {
			delete dragBitmap;
			dragBitmap = NULL;
		}
		const BRect& frame = updateRect;
		if (dragBitmap != NULL) {
			DragMessage(&message, dragBitmap, B_OP_ALPHA,
				BPoint(where.x - frame.left, where.y - frame.top));
		} else {
			DragMessage(&message, frame, this);
		}

		return true;
	}
	return false;
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


bool
TabView::_ValidDragAndDrop(const BMessage* message)
{
	TabView*		fromTab = (TabView*)message->GetPointer("tab_view", nullptr);
	TabsContainer*	fromContainer = (TabsContainer*)message->GetPointer("tab_container", nullptr);

	if (fromTab == nullptr || fromContainer == nullptr)
		return false;

	if (fTabsContainer->GetAffinity() == 0 || fromContainer->GetAffinity() == 0)
		return false;


	return fTabsContainer->GetAffinity() == fromContainer->GetAffinity();
}


///////////////////////////////////////////////////////////////////////////////

//TODO MOve in TabUtils.
#define kCloseButtonWidth 19.0f
const int32 kBrightnessBreakValue = 126;

static void inline
IncreaseContrastBy(float& tint, const float& value, const int& brightness)
{
	tint *= 1 + ((brightness >= kBrightnessBreakValue) ? +1 : -1) * value;
}


TabViewCloseButton::TabViewCloseButton(const char* label,
										TabsContainer* controller,
										const BHandler* handler):
										TabView(label, controller),
										fOverCloseRect(false),
										fClicked(false),
										fHandler(handler)
{

}

//FIX: we should better understand how to extend the default sizes.
BSize
TabViewCloseButton::MinSize()
{
	return TabView::MinSize();
}

BSize
TabViewCloseButton::MaxSize()
{
	BSize s(TabView::MaxSize());
	//s.width += kCloseButtonWidth;
	return s;
}


void
TabViewCloseButton::DrawContents(BView* owner, BRect frame,
										const BRect& updateRect, bool isFront)
{
	BRect labelFrame = frame;
	labelFrame.right -= kCloseButtonWidth;
	TabView::DrawContents(owner, labelFrame, updateRect, isFront);
	frame.left = labelFrame.right;
	DrawCloseButton(owner, frame, updateRect, isFront);
	return;
}


void
TabViewCloseButton::MouseDown(BPoint where)
{
	BRect closeRect = RectCloseButton();
	bool inside = closeRect.Contains(where);
	if (inside != fClicked) {
		fClicked = inside;
		Invalidate(closeRect);
		return;
	}
	TabView::MouseDown(where);
}

void
TabViewCloseButton::MouseUp(BPoint where)
{
	if (fClicked) {
		fClicked = false;
		Invalidate();

		BRect closeRect = RectCloseButton();
		bool inside = closeRect.Contains(where);
		if (inside && fTabsContainer) {
			CloseButtonClicked();
		}
	}
	TabView::MouseUp(where);
}



void
TabViewCloseButton::MouseMoved(BPoint where, uint32 transit,
										const BMessage* dragMessage)
{
	TabView::MouseMoved(where, transit, dragMessage);
	if (dragMessage == nullptr) {
		BRect closeRect = RectCloseButton();
		bool inside = closeRect.Contains(where);
		if (inside != fOverCloseRect) {
			fOverCloseRect = inside;
			if (inside == false)
				fClicked = false;
			Invalidate(closeRect);
		}
	}
}


BRect
TabViewCloseButton::RectCloseButton()
{
	BRect frame  = Bounds();
	frame.right -= be_control_look->DefaultLabelSpacing();
	frame.left = frame.right - kCloseButtonWidth + 3;
	frame.bottom -= be_control_look->DefaultLabelSpacing()/2 + TabViewTools::DefaultFontDescent() - 2;
	frame.top = frame.bottom - frame.Width();
	return frame;
}


void
TabViewCloseButton::CloseButtonClicked()
{
	BMessage msg(kTVCloseButton);
	msg.AddPointer("tab", this);
	BMessenger(fHandler).SendMessage(&msg);
}



void
TabViewCloseButton::DrawCloseButton(BView* owner, BRect buttonRect, const BRect& updateRect,
							bool isFront)
{
	BRect closeRect = RectCloseButton();
	rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
	float tint = B_LIGHTEN_1_TINT;
	if (base.Brightness() >= kBrightnessBreakValue) {
		tint = B_DARKEN_1_TINT *1.2;
	}

	if (fOverCloseRect) {
		// Draw the button frame
		be_control_look->DrawButtonFrame(owner, closeRect, updateRect,
			base, base,
			BControlLook::B_ACTIVATED | BControlLook::B_BLEND_FRAME);

		rgb_color background = ui_color(B_PANEL_BACKGROUND_COLOR);
		be_control_look->DrawButtonBackground(owner, closeRect, updateRect,
			background, BControlLook::B_ACTIVATED);
	} else {
		closeRect.top += 4;
		closeRect.left += 4;
		closeRect.right -= 2;
		closeRect.bottom -= 2;
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

///////////////////////////////////////////////////////////////////////////////

Filler::Filler(TabsContainer* tabsContainer)
		: BView("_filler_", B_WILL_DRAW),
			  fTabsContainer(tabsContainer)
{

}

void
Filler::Draw(BRect rect)
{
	BRect bounds(Bounds());
	TabViewTools::DrawTabBackground(this, bounds, rect);
	if (fTabDragging) {
		rgb_color color = ui_color(B_CONTROL_HIGHLIGHT_COLOR);
		color.alpha = 170;
		SetHighColor(color);
		SetDrawingMode(B_OP_ALPHA);
		FillRect(Bounds());
	}
}


void
Filler::MouseUp(BPoint where)
{
	if (fTabDragging) {
		fTabDragging = false;
		Invalidate();
	}
}


void
Filler::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case TAB_DRAG:
			if(_ValidDragAndDrop(message))
				fTabsContainer->OnDropTab(nullptr, message);
		break;
		default:
			BView::MessageReceived(message);
		break;
	};
}

bool
Filler::_ValidDragAndDrop(const BMessage* message)
{
	TabView*		fromTab = (TabView*)message->GetPointer("tab_view", nullptr);
	TabsContainer*	fromContainer = (TabsContainer*)message->GetPointer("tab_container", nullptr);

	if (fromTab == nullptr || fromContainer == nullptr)
		return false;

	if (fTabsContainer->GetAffinity() == 0 || fromContainer->GetAffinity() == 0)
		return false;


	return fTabsContainer->GetAffinity() == fromContainer->GetAffinity();
}


void
Filler::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
	if (dragMessage &&
		dragMessage->what == TAB_DRAG &&
		_ValidDragAndDrop(dragMessage)) {
		switch (transit) {
			case B_ENTERED_VIEW:
			case B_INSIDE_VIEW:
			{
				fTabDragging = true;
				Invalidate();
				return;
			}
			break;
			default:
			if (fTabDragging) {
				fTabDragging = false;
				Invalidate();
			}
			break;
		};
	} else {
		if (fTabDragging) {
			fTabDragging = false;
			Invalidate();
		}
	}
}
