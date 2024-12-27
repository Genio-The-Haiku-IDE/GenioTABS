/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "MainWindow.h"

#include <Application.h>
#include <TabView.h>
#include <StringView.h>
#include <GroupLayout.h>
#include <Button.h>
#include "GenioTabView.h"
#include <LayoutBuilder.h>

MainWindow::MainWindow()
	:
	BWindow(BRect(100, 100, 900, 900), "My tabs", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	fTabView1 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL,   true);
	fTabView2 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL,   true);
	fTabView3 = new GenioTabView("genio_tab_view", 'GTAB', B_HORIZONTAL, true);

	for (int32 i=0;i<3;i++) {
		BString label("Tab ");
		label << i;
		BString text("Panel ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView1->AddTab(stringView, 1000+i);
	}

	for (int32 i=0;i<3;i++) {
		BString label("Lab ");
		label << i;
		BString text("Label ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView2->AddTab(stringView, 2000+i);
	}

	for (int32 i=0;i<3;i++) {
		BString label("Hor ");
		label << i;
		BString text("Hor ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView3->AddTab(stringView, 3000+i);
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


MainWindow::~MainWindow()
{
}

void
printSize(const BSize& s)
{
	printf("w: %f h: %f\n", s.Width(), s.Height());
}

void
MainWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'ack!':
		{
			fTabView1->DebugFrame();
			printf("-- TAB1 --\n");
			printf("Min: "); printSize(fTabView1->MinSize());
			printf("Max: "); printSize(fTabView1->MaxSize());
			if (fTabView1->GetLayout())
				printf("Lay: "); printSize(fTabView1->GetLayout()->MaxSize());
			printf("Pre: "); printSize(fTabView1->PreferredSize());
			break;
		}
		case 'scro':
		{
			fTabView1->ScrollBy(50.0f);
			break;
		}
		case 'scr2':
		{
			fTabView1->ScrollBy(-50.0f);
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
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
