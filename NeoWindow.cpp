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
#include "GTabView.h"

NeoWindow::NeoWindow()
	:
	BWindow(BRect(100, 100, 600, 200), "My tabs", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	GTabView* tab1 = new GTabView();
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(tab1);

	for(int32 i=0;i<10;i++) {
		BString label("Tab");
		label << i;
		tab1->AddTab(label.String());
	}
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
