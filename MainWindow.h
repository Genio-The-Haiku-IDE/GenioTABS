/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <Window.h>
class GenioTabView;

class MainWindow : public BWindow
{
public:
							MainWindow();
	virtual					~MainWindow();

			void			MessageReceived(BMessage* msg);
			bool			QuitRequested();
private:

			GenioTabView*		fTabView1;
			GenioTabView*		fTabView2;
			GenioTabView*		fTabView3;
};

#endif // MAINWINDOW_H
