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

class GTab : public BView , public Draggable {
public:
								GTab(const char* label, TabsContainer* controller);
	virtual						~GTab();

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

protected:
			bool				_ValidDragAndDrop(const BMessage* msg);

			TabsContainer*		fTabsContainer;
			BLayoutItem*		fLayoutItem;
			bool				fIsFront;
			BString				fLabel;
			bool				fTabDragging;
};

class GTabCloseButton : public GTab {
public:

							GTabCloseButton(const char* label,
												TabsContainer* controller,
												const BHandler* handler);

		virtual	BSize		MinSize() override;
		virtual	BSize		MaxSize() override;
		virtual	void		DrawContents(BView* owner, BRect frame,
										const BRect& updateRect, bool isFront) override;
		virtual	void		MouseDown(BPoint where) override;
		virtual	void		MouseUp(BPoint where) override;
		virtual	void		MouseMoved(BPoint where, uint32 transit,
										const BMessage* dragMessage) override;
private:
				void		DrawCloseButton(BView* owner, BRect butFrame, const BRect& updateRect,
											bool isFront);

				BRect		RectCloseButton();

				void		CloseButtonClicked();
private:

				bool fOverCloseRect;
				bool fClicked;
				const BHandler* fHandler;
};

class Filler : public BView
{
	public:
		Filler(TabsContainer* tabsContainer);

		void 	Draw(BRect rect) override;

		void	MouseUp(BPoint where) override;


		void	MessageReceived(BMessage* message) override;


		void 	MouseMoved(BPoint where, uint32 transit,
									const BMessage* dragMessage) override;

	private:

		bool	_ValidDragAndDrop(const BMessage* message);

		bool				fTabDragging;
		TabsContainer*		fTabsContainer;
};
