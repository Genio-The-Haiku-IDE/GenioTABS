/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SpecialTabView.h"
#include "GTab.h"
#include <cstdio>

class STab : public GTab
{
	public:
		STab(const char* label):GTab(label){}

	BSize	MinSize()
	{
		return MaxSize();
	}

	BSize	MaxSize()
	{
		BSize min;
		float w = StringWidth(Label().String());
		float spacing = be_control_look->DefaultLabelSpacing();
		min.width = w + spacing * 4;
		min.height = TabViewTools::DefaultTabHeigh();
		return min;
	}

	void SetLabel(const char* label) override
	{
		GTab::SetLabel(label);
		InvalidateLayout();
		Invalidate();
	}
};

#include "TabsContainer.h"

void
SpecialTabView::SetLabelAt(int32 index, const char* label)
{
	printf("FATTO\n");
	Container()->TabAt(index)->SetLabel(label);
}

GTab*
SpecialTabView::CreateTabView(const char* label)
{
	return new STab(label);
}

GTab*
SpecialTabView::CreateTabView(GTab* clone)
{
	return nullptr;
}