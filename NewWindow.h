/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <Window.h>
class GTabView;

class NewWindow : public BWindow
{
public:
							NewWindow();
	virtual					~NewWindow();

			void			MessageReceived(BMessage* msg);
			bool			QuitRequested();
private:

			GTabView*		fTabView1;
			GTabView*		fTabView2;
			GTabView*		fTabView3;
};

