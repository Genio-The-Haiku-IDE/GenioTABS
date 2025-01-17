/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <GroupView.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include <SupportDefs.h>
#include <CardView.h>

class GTabScrollLeftButton;
class TabsContainer;
class GTabScrollRightButton;
class GTabMenuTabButton;
class GTabButton;
class GTab;

typedef uint32 tab_affinity;

class GTabView : public BGroupView
{
	public:
				GTabView(const char* name,
						 tab_affinity affinity,
						 orientation orientation = B_HORIZONTAL,
						 bool closeButton = false,
						 bool menuButton = false);

		GTab*	AddTab(const char* label, BView* view, int32 index = -1);

		int32	CountTabs();

		void	UpdateScrollButtons(bool left, bool right);

		void	AttachedToWindow();

		void	MessageReceived(BMessage* message);

		void	MoveTabs(GTab* fromTab, GTab* toTab, TabsContainer* fromContainer);

		virtual void OnMenuTabButton();

	private:

		virtual GTab*		CreateTabView(const char* label);
				BCardView*	CardView() { return fCardView;}
				void		DestroyTabAndView(GTab* tab); //Remove and delete a tab and the view.

	private:

		void	_Init(tab_affinity affinity);
		void	_FixContentOrientation(BView* view);


		GTabScrollLeftButton*	fScrollLeftTabButton;
		TabsContainer*			fTabsContainer;
		GTabScrollRightButton*	fScrollRightTabButton;
		GTabMenuTabButton*		fTabMenuTabButton;
		BCardView*				fCardView;
		bool					fCloseButton;
		orientation				fContentOrientation;
		bool					fMenuButton;
};

