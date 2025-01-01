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
	float extra = be_control_look->DefaultLabelSpacing();
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

Filler::Filler(TabsContainer* tabsContainer)
		: BView("filler", B_WILL_DRAW),
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
			/*if(_ValidDragAndDrop(message))*/
				fTabsContainer->OnDropTab(nullptr, message);
		break;
		default:
			BView::MessageReceived(message);
		break;
	};
}


void
Filler::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
	if (dragMessage &&
		dragMessage->what == TAB_DRAG /*&&
		_ValidDragAndDrop(dragMessage)*/) {
		switch (transit) {
			case B_ENTERED_VIEW:
			case B_INSIDE_VIEW:
			{
				//TabView* fromTab = (TabView*)dragMessage->GetPointer("tab_view", this);
				fTabDragging = true;//(fromTab != this);
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
