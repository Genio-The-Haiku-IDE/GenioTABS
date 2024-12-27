/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <Window.h>


class NeoWindow : public BWindow
{
public:
							NeoWindow();
	virtual					~NeoWindow();

			void			MessageReceived(BMessage* msg);
			bool			QuitRequested();

};

