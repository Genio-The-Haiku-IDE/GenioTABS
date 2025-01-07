/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <SupportDefs.h>
#include <map>
#include "GTabView.h"

typedef uint32  tab_id;
struct obj {
	GTab* tab;
	BView* view;
};
typedef std::map<uint32, obj> IdMap;

class GenioToolsTabView : public GTabView {
public:
					GenioToolsTabView(const char* name, orientation orientation);

			void	AddTab(BView* target, tab_id id);

			GTab*	TabFromView(BView* view) const;

			void	SelectTab(tab_id id);

			bool	HasTab(tab_id id);

private:
			IdMap				fTabIdMap;
};


