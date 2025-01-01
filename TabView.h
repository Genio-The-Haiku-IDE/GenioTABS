/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <Button.h>
#include <GridLayout.h>
#include <LayoutBuilder.h>
#include <Rect.h>
#include <Size.h>
#include <SupportDefs.h>
#include <View.h>
#include <cstdio>
#include <typeinfo>
#include "TabButtons.h"
#include "Draggable.h"

class TabsContainer;

#define TAB_DRAG 		'DRAG'

//TODO: this class could derive from a BView as well.

class TabView : public BView , public Draggable {
public:
								TabView(const char* label, TabsContainer* controller);
	virtual						~TabView();

	virtual	BSize				MinSize() override;
	virtual	BSize				MaxSize() override;

			void 				Draw(BRect updateRect) override;

	virtual void				DrawTab(BView* owener, BRect updateRect);
	virtual	void				DrawBackground(BView* owner, BRect frame,
									const BRect& updateRect, bool isFront);
	virtual	void				DrawContents(BView* owner, BRect frame,
									const BRect& updateRect, bool isFront);

	virtual	void				MouseDown(BPoint where) override;
	virtual	void				MouseUp(BPoint where) override;
	virtual	void				MouseMoved(BPoint where, uint32 transit,
									const BMessage* dragMessage) override;

	virtual	void				MessageReceived(BMessage* message) override;

			bool 				InitiateDrag(BPoint where) override;

			void				SetIsFront(bool isFront);
			bool				IsFront() const;

			BLayoutItem*		LayoutItem() const { return fLayoutItem; }
			void				SetLayoutItem(BLayoutItem* layItem) { fLayoutItem = layItem; }

			BString				Label() { return fLabel; };

private:
			bool				_ValidDragAndDrop(const BMessage* msg);

			TabsContainer*		fTabsContainer;
			BLayoutItem*		fLayoutItem;
			bool				fIsFront;
			BString				fLabel;
			bool				fTabDragging;
};
