/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "TGenioWindow.h"
#include <Application.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <ScrollView.h>

TGenioWindow::TGenioWindow():BWindow(BRect(100, 100, 600, 900), "GTabView", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	_Init();
}

bool
TGenioWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void
TGenioWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'ADD ':
		{
			static int32 c = 0;
			BString label;
			label << c;
			label << " Tab";

			fTabView1->AddTab(label.String(), _NewPanel(c++));
		}
		break;
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}
	}
}

BView*
TGenioWindow::_NewPanel(int32 id)
{
	BTextView*	textView = new BTextView("_textView_");
	textView->SetResizingMode(B_FOLLOW_ALL);
	BString text("Long text for id: ");
	text << id;
	textView->SetText(text.String());
	BScrollView* scrollView = new BScrollView("_scrollView_", textView, B_FOLLOW_ALL, 0, true, true);
	return scrollView;
}

void
TGenioWindow::_Init()
{
/*		// Layout
	fRootLayout = BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(fMenuBar)
		.Add(fToolBar)

		.AddSplit(B_VERTICAL, 0.0f) // output split
		.SetInsets(-2.0f, 0.0f, -2.0f, -2.0f)
			.AddSplit(B_HORIZONTAL, 0.0f) // sidebar split
				.Add(fPanelTabManager->GetPanelTabView(kTabViewLeft), kProjectsWeight)
				.Add(fEditorTabsGroup, kEditorWeight)  // Editor
				.Add(fPanelTabManager->GetPanelTabView(kTabViewRight), 1)
			.End() // sidebar split
			.Add(fPanelTabManager->GetPanelTabView(kTabViewBottom), kOutputWeight)
		.End() //  output split
		.Add(fStatusView = new GlobalStatusView())
	;*/
	BMenuBar* fMenuBar = new BMenuBar("menubar");
	BMenu* fileMenu = new BMenu("File");
	fileMenu->AddItem(new BMenuItem("Add", new BMessage('ADD '), 'a'));
	fMenuBar->AddItem(fileMenu);

	fMenuBar->SetTargetForItems(this);


	fTabView1 = new GTabView("tab1", 'GTAB', B_VERTICAL,   true, true);
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.Add(fMenuBar)
		.AddSplit(B_VERTICAL, 0.0f) // output split
		.Add(fTabView1, 0.5f)
		.Add(_NewPanel(-1), 0.5f)
		.End()
		.Add(new BStringView("_", "StatusBar"))
		;
}