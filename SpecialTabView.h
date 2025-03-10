/*
 * Copyright 2025, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <SupportDefs.h>
#include "GTabView.h"


class SpecialTabView : public GTabView {
public:
		SpecialTabView():GTabView("special", 'AFTY', B_VERTICAL){}

		void SetLabelAt(int32 index, const char* label);

protected:

		GTab*	CreateTabView(const char* label) override;
		GTab*	CreateTabView(GTab* clone) override;

};


