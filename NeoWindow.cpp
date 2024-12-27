/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "NeoWindow.h"

#include <Application.h>
#include <TabView.h>
#include <StringView.h>
#include <GroupLayout.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <cstdio>
#include <Alignment.h>
#include "TabButtons.h"
#include "TabView.h"


/*
class Outer : public BView
{
	public: Outer():BView("name", B_WILL_DRAW, nullptr)
	{
		BButton* left = new ScrollLeftTabButton(nullptr);

		int i=0;
		BLayoutBuilder::Grid<> (this, B_HORIZONTAL, 0.0f)
				.Add(left, i++, 0)
				.Add(new TabView("Label 2", &fController), i++, 0)
				.Add(new TabView("Label 2", &fController), i++, 0)
				.Add(new TabView("Label 3", &fController), i++, 0)
				.Add(new TabView("Label 4", &fController), i++, 0)
				.Add(new TabView("Label 5", &fController), i++, 0)
				.Add(new TabView("Label 1", &fController), i++, 0)
				.Add(new TabView("Label 2", &fController), i++, 0)
				.Add(new TabView("Label 3", &fController), i++, 0)
				.Add(new TabView("Label 4", &fController), i++, 0)
				.Add(new TabView("Label 5", &fController), i++, 0)
			;

		SetExplicitMinSize(BSize(20, B_SIZE_UNSET));
		//SetExplicitPreferredSize(BSize(20, B_SIZE_UNSET));
		SetExplicitAlignment(BAlignment(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER));
	}

	void AttachedToWindow(){
		SetViewColor(0,0,100);

	}

	TabViewController	fController;
};*/

class Right : public BView
{
	public: Right():BView("Right", B_WILL_DRAW, nullptr)
	{
		int i=0;
		BLayoutBuilder::Grid<> (this, B_HORIZONTAL, 0.0f)
				.Add(new ScrollRightTabButton(nullptr), 0,0)
				.Add(new TabMenuTabButton(nullptr), 1, 0)
				;
		SetExplicitAlignment(BAlignment(B_ALIGN_RIGHT, B_ALIGN_VERTICAL_CENTER));
	}
	void AttachedToWindow(){
		SetViewColor(255,0,0);

	}


};


class TabContainer : public BGroupView {
	public:
		TabContainer():BGroupView(B_HORIZONTAL, 0.0)
		{
			BLayoutBuilder::Group<>(this, B_HORIZONTAL, 0.0f)
									.Add(new ScrollLeftTabButton(nullptr))
									.Add(new TabViewController())
									.Add(new Right())
									;
			SetExplicitAlignment(BAlignment(B_ALIGN_USE_FULL_WIDTH, B_ALIGN_VERTICAL_UNSET));
		}
		void AttachedToWindow()
		{
			SetViewColor({0,100,0});
		}
};

NeoWindow::NeoWindow()
	:
	BWindow(BRect(100, 100, 600, 200), "My tabs", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(new TabContainer())
		.AddGlue(1)
		.Add(new BStringView("test", "text", B_WILL_DRAW))
		.AddGlue(1)
		;
}


NeoWindow::~NeoWindow()
{
}

void
NeoWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'ack!':
		{
			break;
		}
		case 'scro':
		{
			break;
		}
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}
	}
}


bool
NeoWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
