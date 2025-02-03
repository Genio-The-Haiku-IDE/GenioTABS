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

class StringsGroup : public BGroupView
{
	public:
		StringsGroup(const char* name):BGroupView(name, B_HORIZONTAL, 0.0f)
		{
			for(int32 i=0;i<3;i++) {
				BString label;
				label << i << "  " << name;
				AddChild(new BStringView("test", label.String(), B_WILL_DRAW));
			}
		}
};

GTab* third = nullptr;
NewWindow::NewWindow()
	:
	BWindow(BRect(100, 100, 600, 900), "GTabView", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	fTabView1 = new GTabView("tab1", 'GTAB', B_VERTICAL,   true, true);
	fTabView2 = new GTabView("tab2", 'GTA2', B_VERTICAL,   false, false);
	fTabView3 = new GTabView("tab3", 'GTAB', B_HORIZONTAL, true, true);

	for (int32 i=0;i<3;i++) {
		BString label;
		label << i;
		label << " Very Log Tab ";
		BString text("Panel ");
		text << i;
		third = fTabView1->AddTab(label.String(), new StringsGroup("test"));
	}
/*
	for (int32 i=0;i<3;i++) {
		BString label("Lab ");
		label << i;
		BString text("Label ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView2->AddTab(label.String(), stringView);
	}

	for (int32 i=0;i<0;i++) {
		BString label("New TAB 123");
		label << i;
		BString text("Hor ");
		text << i;
		BStringView* stringView = new BStringView(label.String(), text.String());
		fTabView3->AddTab(label.String(), stringView);
	}
*/

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0.0f)
		.AddGroup(B_HORIZONTAL, 0.0f)
			.Add(fTabView1)
			.AddGroup(B_VERTICAL, 0.0f)
				.Add(new BButton("Debug","1", new BMessage('ack!')))
				.Add(new BButton("__","2", new BMessage('scro')))
				.Add(new BButton("__","3", new BMessage('scr2')))
			.End()
			//.Add(fTabView2)
		.End()
		//.Add(fTabView3)
	;

	fTabView1->SetExplicitMaxSize(BSize(64,64));
	//fTabView2->SetExplicitMaxSize(BSize(64,64));
	//fTabView3->SetExplicitMaxSize(BSize(64,64));

	if (third)
		fTabView1->SelectTab(third);

	//printf("IS HIDDEN %d\n", ((BView*)third)->IsHidden());

}


NewWindow::~NewWindow()
{
}
#include "GTab.h"
void
NewWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case 'quit':
			debugger("op");
		break;
		case 'ack!':
		{
			if (third)
				fTabView1->SelectTab(third);
			break;
		}
		case 'scro':
		{
//			fTabView1->ScrollBy(50.0f);
			break;
		}
		case 'scr2':
		{
			BStringView* stringView = new BStringView("New TAB 123", "New TAB 123");
			GTab* x = fTabView3->AddTab("New TAB 123", stringView);
			printf("W %f\n", x->Frame().Width());
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
