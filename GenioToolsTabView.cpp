/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "GenioToolsTabView.h"
#include <cassert>

GenioToolsTabView::GenioToolsTabView(const char* name, orientation orientation) :
						GTabView(name, 'GTOL',orientation, true, false)
{

}


void
GenioToolsTabView::AddTab(BView* target, tab_id id)
{
	assert(fTabIdMap.contains(id) == false);
	GTab* tab = GTabView::AddTab(target->Name(), target);
	fTabIdMap[id] = { tab , target };
}
