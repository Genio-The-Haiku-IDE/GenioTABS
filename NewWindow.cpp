/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "NewWindow.h"

#include <Application.h>
#include <TabView.h>
#include <StringView.h>
#include <GroupLayout.h>
#include <Button.h>
#include "GTabView.h"
#include <LayoutBuilder.h>
#include <cstdio>

NewWindow::NewWindow()
	:
	BWindow(BRect(100, 100, 900, 900), "GTabView", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	fTabView1 = new GTabView("tab1", 'GTAB', B_VERTICAL,   true);
	fTabView2 = new GTabView("tab2", 'GTA2', B_VERTICAL,   true);
	fTabView3 = new GTabView("tab3", 'GTAB', B_HORIZONTAL, true);

	for (int32 i=0;i<3;i++) {
		BString label("Tab ");
		label << i;
		BString text("Panel ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView1->AddTab(label.String(), stringView);
	}

	for (int32 i=0;i<3;i++) {
		BString label("Lab ");
		label << i;
		BString text("Label ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView2->AddTab(label.String(), stringView);
	}

	for (int32 i=0;i<3;i++) {
		BString label("Hor ");
		label << i;
		BString text("Hor ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView3->AddTab(label.String(), stringView);
	}


	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.AddGroup(B_HORIZONTAL, 0.0f)
			.Add(fTabView1)
			.AddGroup(B_VERTICAL, 0.0f)
				.Add(new BButton("Debug","Debug", new BMessage('ack!')))
				.Add(new BButton("__","Scroll ->", new BMessage('scro')))
				.Add(new BButton("__","Scroll <-", new BMessage('scr2')))
			.End()
			.Add(fTabView2)
		.End()
		.Add(fTabView3)
	;

	fTabView1->SetExplicitMaxSize(BSize(64,64));
	fTabView2->SetExplicitMaxSize(BSize(64,64));
	fTabView3->SetExplicitMaxSize(BSize(64,64));
}


NewWindow::~NewWindow()
{
}

void
NewWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'ack!':
		{

			break;
		}
		case 'scro':
		{
//			fTabView1->ScrollBy(50.0f);
			break;
		}
		case 'scr2':
		{
//			fTabView1->ScrollBy(-50.0f);
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
NewWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
