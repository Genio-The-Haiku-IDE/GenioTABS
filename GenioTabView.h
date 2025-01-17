/*
 * Copyright 2024, Andrea Anzani <andrea.anzani@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once


#include <SupportDefs.h>
#include <TabView.h>
#include <cstdio>
#include <map>

#include "Draggable.h"

class GenioTab;

typedef uint32	tab_drag_affinity;
typedef uint32  tab_id;

typedef std::map<uint32, BTab*> IdMap;

class GenioTabView :  public BTabView, private Draggable {
public:
					GenioTabView(const char* name,
								tab_drag_affinity affinity,
								orientation orientation = B_HORIZONTAL,
								bool withCloseButtons = false);

	virtual	void	MouseDown(BPoint where);
	virtual	void	MouseUp(BPoint where);
	virtual	void	MouseMoved(BPoint where, uint32 transit,
									const BMessage* dragMessage);
	virtual void	MessageReceived(BMessage* msg);

	virtual	void	FrameResized(float newWidth, float newHeight);

//
	// virtual BSize	MaxSize();
	// virtual BSize	MinSize();
	// virtual BSize	PreferredSize();

			void	AddTab(BView* target, tab_id id);

			BTab*	TabFromView(BView* view) const;

			void	SelectTab(tab_id id);

			bool	HasTab(tab_id id);

	virtual		BRect	TabFrame(int32 index) const;

	void DebugFrame();

	void				ScrollBy(float y);

private:
			using BTabView::TabAt;
			using BTabView::Select;
			using BTabView::AddTab;

			BTab*	RemoveTab(int32 tabIndex);
			BView*	ContainerView() const = delete;
			BView*	ViewForTab(int32 tabIndex) const = delete;

				void	_AddTab(GenioTab* tab);

		//virtual	void	AddTab(BView* target, BTab* tab);
		virtual	BRect	DrawTabs();


		void	MoveTabs(uint32 from, uint32 to, GenioTabView* fromTabView);
		bool	InitiateDrag(BPoint where);
		void	_OnDrop(BMessage* msg);

		int32	_TabAt(BPoint where);
		BTab*	_TabFromPoint(BPoint where, int32& index);
		void	_DrawTabIndicator();
		bool	_ValidDragAndDrop(const BMessage* msg, bool* sameTabView = nullptr);

		void	_ChangeGroupViewDirection(GenioTab* tab);

		void	_PrintMap();

		BRect				fDropTargetHighlightFrame;
		tab_drag_affinity	fTabAffinity;
		orientation			fOrientation;
		IdMap				fTabIdMap;
		bool				fWithCloseButtons;
		float				fScrollByY;

};


