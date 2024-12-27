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

class TabViewController;

class TabView : public BControl {
public:
								TabView(const char* label, TabViewController* controller);
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
			bool				fIsFirst;
			bool				fIsLast; //Not used?

			TabViewController*	fTabViewController;
			BLayoutItem*		fLayoutItem;
};

class Filler : public BView
{
	public:
		Filler():BView("filler", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
		{
			//SetExplicitMinSize(BSize(B_SIZE_UNSET, B_SIZE_UNSET));
		}

		void Draw(BRect rect)
		{
			rgb_color base = ui_color(B_PANEL_BACKGROUND_COLOR);
			uint32 borders = BControlLook::B_TOP_BORDER | BControlLook::B_BOTTOM_BORDER;
			be_control_look->DrawInactiveTab(this, rect, rect, base, 0, borders);
		}
};

class TabViewController : public BGroupView
{
public:
		public:



		BGroupLayout*	GroupLayout() { return dynamic_cast<BGroupLayout*>(GetLayout());}
		TabViewController():BGroupView(B_HORIZONTAL, 0.0f)/* ("name", B_WILL_DRAW)*/, fSelectedTab(nullptr)
		{
			GroupLayout()->AddView(0, new Filler());
			SetExplicitMinSize(BSize(100, GetH()));
			SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
			for(int32 i=0;i<2;i++) {
				BString label("Tab");
				label << i;
				AddTab(new TabView(label.String(), this));
			}
	}

	void	AddTab(TabView* tab)
	{
		BLayoutItem* item = GroupLayout()->AddView(CountTabs(), tab);
		tab->SetLayoutItem (item);

		if (CountTabs() == 1) {
			printf("Adding %s\n", tab->Label());
			tab->Update(true, false, true);
			fSelectedTab = tab;
		} else {
			TabView* prev = TabAt(CountTabs()-2);
			printf("Prev: %s\n", prev->Label());
			prev->Update(false /* ?? */, false, prev == fSelectedTab);
			tab->Update(false, false, false);
		}
	}

	int32	CountTabs()
	{
		return GroupLayout()->CountItems() - 1; //exclude the Filler.
	}

	TabView*	TabAt(int32 index)
	{
		if (index < 0 || index >= CountTabs())
			return nullptr;

		return dynamic_cast<TabView*>(GroupLayout()->ItemAt(index)->View());
	}


	void MouseDown(TabView* tab, BPoint where)
	{
		if (tab == fSelectedTab)
			return;

		if (fSelectedTab != nullptr)
			fSelectedTab->Update(false, tab->IsLast(), false);

		tab->Update(false, tab->IsLast(), true);

		fSelectedTab = tab;

		BGroupLayout* x = GroupLayout();
		printf("l %p vs %p\n", tab->Parent()->GetLayout(), x);

		BLayoutItem*	element =	x->ItemAt(0);

		printf("l %p %d\n", element, x->IndexOfItem(tab->LayoutItem()));

		//x->ItemAt(0, 0)->SetVisible(false);

		if (x) {

		}
	}

private:
		TabView*		fSelectedTab;

};
