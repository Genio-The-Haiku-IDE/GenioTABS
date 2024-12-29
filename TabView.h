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

class TabsContainer;

//TODO: this class could derive from a BView as well.

class TabView : public BControl {
public:
								TabView(const char* label, TabsContainer* controller);
	virtual						~TabView();

	virtual	BSize				MinSize() override;
	virtual	BSize				MaxSize() override;

			void 				Draw(BRect updateRect) override;
	virtual	void				DrawBackground(BView* owner, BRect frame,
									const BRect& updateRect, bool isFirst,
									bool isLast, bool isFront);
	virtual	void				DrawContents(BView* owner, BRect frame,
									const BRect& updateRect, bool isFirst,
									bool isLast, bool isFront);

	virtual	void				MouseDown(BPoint where) override;
	virtual	void				MouseUp(BPoint where) override;
	virtual	void				MouseMoved(BPoint where, uint32 transit,
									const BMessage* dragMessage) override;

			void				SetIsFront(bool isFront);
			bool				IsFront() const;

			void				SetIsLast(bool isLast);	//not used..
			bool				IsLast() const { return fIsLast; } //not used..

	virtual	void				Update(bool isFirst, bool isLast,
									bool isFront);

			BLayoutItem*		LayoutItem() const { return fLayoutItem; }
			void				SetLayoutItem(BLayoutItem* layItem) { fLayoutItem = layItem; }

private:
			bool				fIsFirst; //Not used??
			bool				fIsLast; //Not used??

			TabsContainer*		fTabsContainer;
			BLayoutItem*		fLayoutItem;
};
