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
class TabView;

class GTabView : public BGroupView
{
	public:
		GTabView(bool closeButton);

		void	AddTab(const char* label, BView* view);

		void	UpdateScrollButtons(bool left, bool right);

		void	AttachedToWindow();

		void	MessageReceived(BMessage* message);

		void	MoveTabs(TabView* fromTab, TabView* toTab, TabsContainer* fromContainer);

	private:

		virtual TabView*	CreateTabView(const char* label);
		BCardView*			CardView() { return fCardView;}
		void				DestroyTabAndView(TabView* tab); //Remove and delete a tab and the view.

	private:

		void	_Init();
		void	_SetButtonVisibility(TabButton*, bool newState);


		ScrollLeftTabButton*	fScrollLeftTabButton;
		TabsContainer*			fTabsContainer;
		ScrollRightTabButton*	fScrollRightTabButton;
		TabMenuTabButton*		fTabMenuTabButton;
		BCardView*				fCardView;
		bool					fCloseButton;
};

