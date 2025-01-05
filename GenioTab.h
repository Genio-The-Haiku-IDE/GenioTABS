/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <SupportDefs.h>
#include <TabView.h>
#include <Box.h>

#include "GenioTabView.h"

class GTabContainer : public BBox {
public:
	GTabContainer(BView* view);;
	BView*	ContentView() { return fView;}
private:
	BView* fView;
};


#define kCloseButtonWidth	20

class GenioTab : public BTab {
protected:
friend GenioTabView;
		GenioTab(BView* view, tab_id id, bool withCloseButton);
		GenioTab(const GenioTab*);

		tab_id	Id() { return fTabId;}

	virtual	void				DrawLabel(BView* owner, BRect frame);
	virtual	void				DrawTab(BView* owner, BRect frame,
									tab_position position, bool full = true);


			BRect	RectCloseButton(const BRect& tabRect);
			void	SetOverCloseRect(bool over) { fOverCloseRect = over; }
			bool	OverCloseRect() { return fOverCloseRect; }
			void	SetClicked(bool clicked) { fClicked = clicked; }
			bool	Clicked() { return fClicked; }

private:

			void	_DrawCloseButton(BView* owner, BRect& tabRect);

		GTabContainer*	fGTabContainer;
		tab_id			fTabId;
		bool			fWithCloseButton;
		bool			fOverCloseRect = false;
		bool			fClicked = false;
};


