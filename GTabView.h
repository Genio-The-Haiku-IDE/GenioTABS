/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <GroupView.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include <SupportDefs.h>

class ScrollLeftTabButton;
class TabViewController;
class ScrollRightTabButton;
class TabMenuTabButton;

class GTabView : public BGroupView
{
	public:
		GTabView();

		void	AddTab(const char* label);

	private:

		void	_Init();

		ScrollLeftTabButton*	fScrollLeftTabButton;
		TabViewController*		fTabViewController;
		ScrollRightTabButton*	fScrollRightTabButton;
		TabMenuTabButton*		fTabMenuTabButton;
};

