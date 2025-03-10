/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include "GTabView.h"
#include <SupportDefs.h>
#include <Window.h>
#include <MenuBar.h>

class TGenioWindow : public BWindow {
public:
			TGenioWindow();

			bool			QuitRequested();
			void			MessageReceived(BMessage* msg);


private:
		void	_Init();
		BView*	_NewPanel(int32 id);

		 GTabView* fTabView1 = nullptr;
		 BMenuBar* fMenuBar = nullptr;
};


