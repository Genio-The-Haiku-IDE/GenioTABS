[1mdiff --git a/GenioTabView.cpp b/GenioTabView.cpp[m
[1mindex cfe8578..3e59247 100644[m
[1m--- a/GenioTabView.cpp[m
[1m+++ b/GenioTabView.cpp[m
[36m@@ -416,8 +416,30 @@[m [mGenioTabView::_ChangeGroupViewDirection(GTab* tab)[m
 	}[m
 [m
 }[m
[32m+[m[32m/*[m
[32m+[m[32mBSize[m
[32m+[m[32mGenioTabView::MinSize()[m
[32m+[m[32m{[m
[32m+[m	[32mif (CountTabs() == 0)[m
[32m+[m		[32mreturn BSize(64, 64);[m
[32m+[m	[32mreturn BTabView::MinSize();[m
[32m+[m[32m}[m
 [m
[31m-[m
[32m+[m[32mBSize[m
[32m+[m[32mGenioTabView::MaxSize()[m
[32m+[m[32m{[m
[32m+[m	[32mif (CountTabs() == 0)[m
[32m+[m		[32mreturn BSize(64, 64);[m
[32m+[m	[32mreturn BTabView::MaxSize();[m
[32m+[m[32m}[m
[32m+[m[32mBSize[m
[32m+[m[32mGenioTabView::PreferredSize()[m
[32m+[m[32m{[m
[32m+[m	[32mif (CountTabs() == 0)[m
[32m+[m		[32mreturn BSize(64, 64);[m
[32m+[m	[32mreturn BTabView::PreferredSize();[m
[32m+[m[32m}[m
[32m+[m[32m*/[m
 BTab*[m
 GenioTabView::RemoveTab(int32 tabIndex)[m
 {[m
[1mdiff --git a/GenioTabView.h b/GenioTabView.h[m
[1mindex c2b6b87..0f39ae4 100644[m
[1m--- a/GenioTabView.h[m
[1m+++ b/GenioTabView.h[m
[36m@@ -31,6 +31,10 @@[m [mpublic:[m
 	virtual	void	MouseMoved(BPoint where, uint32 transit,[m
 									const BMessage* dragMessage);[m
 	virtual void	MessageReceived(BMessage* msg);[m
[32m+[m[32m//[m
[32m+[m	[32m// virtual BSize	MaxSize();[m
[32m+[m	[32m// virtual BSize	MinSize();[m
[32m+[m	[32m// virtual BSize	PreferredSize();[m
 [m
 			void	AddTab(BView* target, tab_id id);[m
 [m
[36m@@ -50,7 +54,7 @@[m [mprivate:[m
 			using BTabView::Select;[m
 [m
 			BTab*	RemoveTab(int32 tabIndex);[m
[31m-			BView*	ContainerView() const = delete;[m
[32m+[m[32m//			BView*	ContainerView() const = delete;[m
 			BView*	ViewForTab(int32 tabIndex) const = delete;[m
 [m
 				void	_AddTab(GTab* tab);[m
[1mdiff --git a/MainWindow.cpp b/MainWindow.cpp[m
[1mindex b778000..853e533 100644[m
[1m--- a/MainWindow.cpp[m
[1m+++ b/MainWindow.cpp[m
[36m@@ -19,8 +19,8 @@[m [mMainWindow::MainWindow()[m
 	BWindow(BRect(100, 100, 900, 900), "My tabs", B_TITLED_WINDOW,[m
 		B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)[m
 {[m
[31m-	fTabView1 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL, true);[m
[31m-	fTabView2 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL, false);[m
[32m+[m	[32mfTabView1 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL,   true);[m
[32m+[m	[32mfTabView2 = new GenioTabView("genio_tab_view", 'GTAB', B_VERTICAL,   true);[m
 	fTabView3 = new GenioTabView("genio_tab_view", 'GTAB', B_HORIZONTAL, true);[m
 [m
 	for (int32 i=0;i<3;i++) {[m
[36m@@ -47,7 +47,7 @@[m [mMainWindow::MainWindow()[m
 		BString text("Hor ");[m
 		text << i;[m
 		BStringView* stringView = new BStringView(label.String(), text.String());[m
[31m-		fTabView3->AddTab(stringView, 2000+i);[m
[32m+[m		[32mfTabView3->AddTab(stringView, 3000+i);[m
 	}[m
 [m
 [m
[36m@@ -59,6 +59,10 @@[m [mMainWindow::MainWindow()[m
 		.End()[m
 		.Add(fTabView3)[m
 	;[m
[32m+[m
[32m+[m	[32mfTabView1->SetExplicitMaxSize(BSize(64,64));[m
[32m+[m	[32mfTabView2->SetExplicitMaxSize(BSize(64,64));[m
[32m+[m	[32mfTabView3->SetExplicitMaxSize(BSize(64,64));[m
 }[m
 [m
 [m
[36m@@ -80,11 +84,11 @@[m [mMainWindow::MessageReceived(BMessage* message)[m
 		{[m
 			fTabView1->DebugFrame();[m
 			printf("-- TAB1 --\n");[m
[31m-			printf("Min: "); printSize(fTabView1->MinSize());[m
[31m-			printf("Max: "); printSize(fTabView1->MaxSize());[m
[32m+[m			[32mprintf("Min: "); printSize(fTabView1->ContainerView()->MinSize());[m
[32m+[m			[32mprintf("Max: "); printSize(fTabView1->ContainerView()->MaxSize());[m
 			if (fTabView1->GetLayout())[m
[31m-				printf("Lay: "); printSize(fTabView1->GetLayout()->MaxSize());[m
[31m-			printf("Pre: "); printSize(fTabView1->PreferredSize());[m
[32m+[m				[32mprintf("Lay: "); printSize(fTabView1->ContainerView()->GetLayout()->MaxSize());[m
[32m+[m			[32mprintf("Pre: "); printSize(fTabView1->ContainerView()->PreferredSize());[m
 			break;[m
 		}[m
 		default:[m
