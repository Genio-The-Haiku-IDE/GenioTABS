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

class ScrollLeftTabButton;
class TabsContainer;
class ScrollRightTabButton;
class TabMenuTabButton;
class TabButton;
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

		void	AddTab(const char* label, BView* view);

		void	UpdateScrollButtons(bool left, bool right);

		void	AttachedToWindow();

		void	MessageReceived(BMessage* message);

		void	MoveTabs(GTab* fromTab, GTab* toTab, TabsContainer* fromContainer);

	private:

		virtual GTab*		CreateTabView(const char* label);
				BCardView*	CardView() { return fCardView;}
				void		DestroyTabAndView(GTab* tab); //Remove and delete a tab and the view.

	private:

		void	_Init(tab_affinity affinity);
		void	_FixContentOrientation(BView* view);


		ScrollLeftTabButton*	fScrollLeftTabButton;
		TabsContainer*			fTabsContainer;
		ScrollRightTabButton*	fScrollRightTabButton;
		TabMenuTabButton*		fTabMenuTabButton;
		BCardView*				fCardView;
		bool					fCloseButton;
		orientation				fContentOrientation;
		bool					fMenuButton;
};

