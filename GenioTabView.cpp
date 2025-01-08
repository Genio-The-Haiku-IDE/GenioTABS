/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GenioTabView.h"

#include <TabViewPrivate.h>
#include <Layout.h>
#include <CardLayout.h>
#include <GroupView.h>
#include <cassert>

#include "GenioTab.h"



class IndexGTab : public GenioTab  {
public:
	IndexGTab(const GenioTab* fromTab, uint32 index):GenioTab(fromTab), fIndex(index){}
	uint32	fIndex;
};

BTab*
GenioTabView::_TabFromPoint(BPoint where, int32& index)
{
	index = _TabAt(where);
	if (index < 0)
		return nullptr;
	return TabAt(index);
}


int32
GenioTabView::_TabAt(BPoint where)
{
	for (int32 i = 0; i < CountTabs(); i++) {
		if (TabFrame(i).Contains(where))
			return i;
	}
	return -1;
}

#define TAB_DRAG 'TDRA'
#define ALPHA 230

#include <Bitmap.h>
#include <ControlLook.h>


bool
GenioTabView::InitiateDrag(BPoint where)
{
	uint32 index = _TabAt(where);
	if (index < 0)
		return false;

	BRect frame(TabFrame(index));
	BTab* tab = TabAt(index);
	if (tab != nullptr) {
		BMessage message(TAB_DRAG);

		message.AddPointer("genio_tab_view", this);
		message.AddUInt32("tab_drag_affinity", fTabAffinity);
		message.AddInt32("index", index);

		BRect updateRect = frame.OffsetToCopy(BPoint(0, 0));

		BBitmap* dragBitmap = new BBitmap(updateRect, B_RGB32, true);
		if (dragBitmap->IsValid()) {
			BView* view = new BView(dragBitmap->Bounds(), "helper", B_FOLLOW_NONE, B_WILL_DRAW);
			dragBitmap->AddChild(view);
			dragBitmap->Lock();
			tab->DrawTab(view, updateRect, B_TAB_FRONT, true);

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

#include <Button.h>

GenioTabView::GenioTabView(const char* name,
							tab_drag_affinity affinity,
							orientation orientation,
							bool withCloseButtons):
	BTabView(name),
	fTabAffinity(affinity),
	fOrientation(orientation),
	fWithCloseButtons(withCloseButtons),
	fScrollByY(0.0f)
{
//	AddChild(new BButton(BRect(10000,0,10,30), "x", "x", nullptr, B_FOLLOW_LEFT_TOP));
}


void
GenioTabView::MouseDown(BPoint where)
{
	if (fWithCloseButtons) {
		for (int32 i = 0; i < CountTabs(); i++) {
			GenioTab* tab = dynamic_cast<GenioTab*>(BTabView::TabAt(i));
			if (tab) {
				BRect fr = TabFrame(i);
				BRect cl = tab->RectCloseButton(fr);
				if (cl.Contains(where)) {
					tab->SetClicked(true);
					tab->DrawTab(this, fr, B_TAB_ANY);
					return; //avoid select if click on close
				} else {
					if (tab->Clicked() != false) {
						tab->SetClicked(false);
						tab->DrawTab(this, fr, B_TAB_ANY);
					}
				}
			}
		}
	}
	BTabView::MouseDown(where);
	OnMouseDown(where);
}


void
GenioTabView::MouseUp(BPoint where)
{
	if (fWithCloseButtons) {
		for (int32 i = 0; i < CountTabs(); i++) {
			GenioTab* tab = dynamic_cast<GenioTab*>(BTabView::TabAt(i));
			if (tab) {
				BRect fr = TabFrame(i);
				BRect cl = tab->RectCloseButton(fr);
				if (cl.Contains(where) && tab->Clicked()) {
					tab->SetClicked(false);
					tab->DrawTab(this, fr, B_TAB_ANY);

					RemoveTab(i);
					Invalidate();
					return; //avoid select if click on close
				} else {
					if (tab->Clicked() != false) {
						tab->SetClicked(false);
						tab->DrawTab(this, fr, B_TAB_ANY);
					}
				}
			}
		}
	}
	BTabView::MouseUp(where);
	OnMouseUp(where);
	if (fDropTargetHighlightFrame.IsValid()) {
		Invalidate(fDropTargetHighlightFrame);
		fDropTargetHighlightFrame = BRect();
	}
}


void
GenioTabView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
	bool sameTabView = true;
	if (dragMessage &&
	    dragMessage->what == TAB_DRAG &&
		_ValidDragAndDrop(dragMessage, &sameTabView)) {
		switch (transit) {
			case B_ENTERED_VIEW:
			case B_INSIDE_VIEW:
			{
				int32 fromIndex = dragMessage->GetInt32("index", -1);
				int32 toIndex = _TabAt(where);
				if (toIndex >= 0) {
					if (fromIndex != toIndex || sameTabView == false) {
						BRect highlightFrame = TabFrame(toIndex);

						if (fDropTargetHighlightFrame != highlightFrame) {
							Invalidate(fDropTargetHighlightFrame);
							fDropTargetHighlightFrame = highlightFrame;
							Invalidate(fDropTargetHighlightFrame);
						}
						return;
					}
				} else {
					BRect frame =TabFrame(CountTabs()-1);
					float right = 0;
					float bottom = Bounds().bottom;
					if (CountTabs() > 0) {
						right = frame.right;
						bottom = frame.bottom;
					}
					if (where.x < right)
						return;

					BRect highlightFrame = Bounds();
					highlightFrame.left = right;
					highlightFrame.bottom = bottom;

					if (fDropTargetHighlightFrame != highlightFrame) {
						Invalidate(fDropTargetHighlightFrame);
						fDropTargetHighlightFrame = highlightFrame;
						Invalidate(fDropTargetHighlightFrame);
					}
					return;
				}
			} break;
			default:
				if (fDropTargetHighlightFrame.IsValid()) {
					Invalidate(fDropTargetHighlightFrame);
					fDropTargetHighlightFrame = BRect();
				}
				break;
		};
	} else {
		// Other buttons are used to select a tab by clicking directly on it
		if (fWithCloseButtons) {
			for (int32 i = 0; i < CountTabs(); i++) {
				GenioTab* tab = dynamic_cast<GenioTab*>(BTabView::TabAt(i));
				if (tab) {
					BRect fr = TabFrame(i);
					BRect cl = tab->RectCloseButton(fr);
					if (cl.Contains(where)) {
						tab->SetOverCloseRect(true);
						tab->DrawTab(this, fr, B_TAB_ANY);
						return; //avoid select if click on close
					} else {
						if (tab->OverCloseRect() != false) {
							tab->SetOverCloseRect(false);
							Invalidate(fr);
						}
					}
				}
			}
		}
		BTabView::MouseMoved(where, transit, dragMessage);
		OnMouseMoved(where);
		if (fDropTargetHighlightFrame.IsValid()) {
			Invalidate(fDropTargetHighlightFrame);
			fDropTargetHighlightFrame = BRect();
		}
	}
}


void
GenioTabView::MoveTabs(uint32 from, uint32 to, GenioTabView* fromTabView)
{
	const GenioTab*	fromTab = dynamic_cast<GenioTab*>(fromTabView->TabAt(from));
	if (!fromTab)
		return;

	GTabContainer* fromContainer = dynamic_cast<GTabContainer*>(fromTabView->BTabView::ViewForTab(from));
	if (fromContainer == nullptr)
		return;

	BView*	fromView = fromContainer->ContentView();
	if (!fromView)
		return;

	fromView->RemoveSelf();

	BString label = fromTab->Label(); //TODO copy all the props
	fromTabView->RemoveTab(from);
	GenioTab* indexTab = new IndexGTab(fromTab, to);
	indexTab->SetLabel(label.String());
	_AddTab(indexTab);
}
/*
void
GenioTabView::AddTab(BView* target, BTab* tab)
{
	BTabView::AddTab(target, tab);
}
*/

void
BTabView::AddTab(BView* target, BTab* tab)
{
	if (tab == NULL)
		tab = new BTab(target);
	else
		tab->SetView(target);

	IndexGTab *gtab = dynamic_cast<IndexGTab *>(tab);
	int32 index = CountTabs();
	if (gtab != nullptr) {
		if (gtab->fIndex < (uint32)CountTabs())
			index = gtab->fIndex;
	}
//	printf("New Index to: %d\n", index);
	if (fContainerView->GetLayout())
			fContainerView->GetLayout()->AddView(index, target);
	fTabList->AddItem(tab, index);

	BTab::Private(tab).SetTabView(this);

	// When we haven't had a any tabs before, but are already attached to the
	// window, select this one.
	if (Window() != NULL) {
		if (CountTabs() == 1)
			Select(0);
		else {
			Select(index);

		// make the view visible through the layout if there is one
		BCardLayout* layout
			= dynamic_cast<BCardLayout*>(fContainerView->GetLayout());
		if (layout != NULL)
			layout->SetVisibleItem(index);
		}
	}
}

void
GenioTabView::AddTab(BView* target, tab_id id)
{
	assert(fTabIdMap.contains(id) == false);
	GenioTab* newTab = new GenioTab(target, id, fWithCloseButtons);
	_AddTab(newTab);
}


BTab*
GenioTabView::TabFromView(BView* view) const
{
	for (int32 i = 0; i < CountTabs(); i++) {
		GTabContainer* fromContainer = dynamic_cast<GTabContainer*>(BTabView::ViewForTab(i));
		if (fromContainer != nullptr && fromContainer->ContentView() == view) {
			return TabAt(i);
		}
	}
	return nullptr;
}


void
GenioTabView::SelectTab(tab_id id)
{
	if (fTabIdMap.contains(id)) {
		BTab* tab = fTabIdMap[id];
		BTabView::Select(BTabView::IndexOf(tab));
	}
}


bool
GenioTabView::HasTab(tab_id id)
{
	return fTabIdMap.contains(id);
}


BRect
GenioTabView::TabFrame(int32 index) const
{
	if (!fWithCloseButtons) {
		return BTabView::TabFrame(index);
	}
	else {
		BRect rect = BTabView::TabFrame(index);
		//printf("-? [%d][%f] ", index, rect.Width()); rect.PrintToStream();
		rect.left  += (index * kCloseButtonWidth) + fScrollByY;
		rect.right += ((index+1) * kCloseButtonWidth) + fScrollByY;
		return rect;
	}
}


void
GenioTabView::FrameResized(float newWidth, float newHeight)
{
/*	if (CountTabs() > 0 && newWidth <= TabFrame(CountTabs()-1).right)
		printf("NON ci sta -> %s \n", TabAt(CountTabs()-1)->Label());*/
	BTabView::FrameResized(newWidth, newHeight);
}


void
GenioTabView::DebugFrame()
{
	printf("-----------------------------------------\n");
	for (int32 i = 0; i < CountTabs(); i++) {
		BRect frame = TabFrame(i);
		printf("-> [%d][%f] ", i, frame.Width()); frame.PrintToStream();
	}
}

void
GenioTabView::ScrollBy(float y)
{
	fScrollByY -= y;
	Invalidate();
}

#include <GroupLayout.h>
void
GenioTabView::_ChangeGroupViewDirection(GenioTab* tab)
{
	GTabContainer* fromContainer = dynamic_cast<GTabContainer*>(tab->View());
	if (!fromContainer)
		return;

	BView* view = fromContainer->ContentView();
	if (!view)
		return;
	BLayout* layout = view->GetLayout();
	if (!layout)
		return;
	BGroupLayout* grpLayout = dynamic_cast<BGroupLayout*>(layout);
	if (!grpLayout) {
		return;
	}
	if (grpLayout->Orientation() != fOrientation)
	{
		grpLayout->SetOrientation(fOrientation);
	}

}
/*
BSize
GenioTabView::MinSize()
{
	if (CountTabs() == 0)
		return BSize(64, 64);
	return BTabView::MinSize();
}

BSize
GenioTabView::MaxSize()
{
	if (CountTabs() == 0)
		return BSize(64, 64);
	return BTabView::MaxSize();
}
BSize
GenioTabView::PreferredSize()
{
	if (CountTabs() == 0)
		return BSize(64, 64);
	return BTabView::PreferredSize();
}
*/
BTab*
GenioTabView::RemoveTab(int32 tabIndex)
{
	BTab* tab = BTabView::RemoveTab(tabIndex);
	for(const auto& [k, v] : fTabIdMap) {
		if (v == tab) {
			fTabIdMap.erase(k);
			break;
		}
	}

	FrameResized(Bounds().Width(), Bounds().Height());
	return tab;
}



void
GenioTabView::_AddTab(GenioTab* tab)
{
	//experimental: let's try to improve the GroupView.
	_ChangeGroupViewDirection(tab);
	fTabIdMap[tab->Id()] = tab;
	BTabView::AddTab(tab->View(), tab);
		FrameResized(Bounds().Width(), Bounds().Height());
	_PrintMap();
}


void
GenioTabView::_OnDrop(BMessage* msg)
{
	int32 dragIndex = msg->GetInt32("index", -1);
	if (dragIndex < 0)
		return;

	if (!_ValidDragAndDrop(msg))
			return;

	BPoint drop_point;
	if (msg->FindPoint("_drop_point_", &drop_point) != B_OK)
		return;

	int32 toIndex = _TabAt(ConvertFromScreen(drop_point));
	if (CountTabs() == 0)
		toIndex = 0;

	if (toIndex < 0) {
		if (drop_point.x < TabFrame(CountTabs()-1).right)
			return;
		toIndex = CountTabs();
	}

	GenioTabView* fromTabView = (GenioTabView*)msg->GetPointer("genio_tab_view", this);

	MoveTabs(dragIndex, toIndex, fromTabView);
	Invalidate();
	if (fromTabView != this)
		fromTabView->Invalidate();
}


void
GenioTabView::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case TAB_DRAG:
		{
			_OnDrop(msg);
		} break;
		default:
			BTabView::MessageReceived(msg);
			break;
	};
}
/*
void
GenioTabView::AddTab(BView* target, BTab* tab)
{
	BTabView::AddTab(target, tab);
}
*/
BRect
GenioTabView::DrawTabs()
{
	BRect rect = BTabView::DrawTabs();
	_DrawTabIndicator();
	return rect;
}


void
GenioTabView::_DrawTabIndicator()
{
	if (fDropTargetHighlightFrame.IsValid()) {
		rgb_color color = ui_color(B_CONTROL_HIGHLIGHT_COLOR);
		color.alpha = 170;
		SetHighColor(color);
		SetDrawingMode(B_OP_ALPHA);
		FillRect(fDropTargetHighlightFrame);
	}
}

bool
GenioTabView::_ValidDragAndDrop(const BMessage* msg, bool* sameTabView)
{
	if (sameTabView)
		*sameTabView = (msg->GetPointer("genio_tab_view", nullptr) == this);

	if (fTabAffinity == 0 && msg->GetPointer("genio_tab_view", nullptr) != this)
		return false;

	if (msg->GetUInt32("tab_drag_affinity", 0) != fTabAffinity)
		return false;

	return true;
}


void
GenioTabView::_PrintMap()
{
	for(const auto& [k, v] : fTabIdMap) {
		printf("TabList: '%.4s' -> %p\n", (char *)&k, v);
	}
}
