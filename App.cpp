/*
 * Copyright 2024, My Name <my@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "App.h"
#include "TGenioWindow.h"

const char* kApplicationSignature = "application/x-vnd.MyName-MyApp";


App::App()
	:
	BApplication(kApplicationSignature)
{
	//(new MainWindow())->Show();
	(new TGenioWindow())->Show();
	//(new NeoWindow())->Show();
}


App::~App()
{
}


int
main()
{
	App* app = new App();
	app->Run();
	delete app;
	return 0;
}
