#ifndef FLUKE_XCB_ERRORS_HPP
#define FLUKE_XCB_ERRORS_HPP

#pragma once


namespace fluke {
	// http://www.rahul.net/kenton/xproto/xrequests.html
	constexpr const char* request_str[] = {
		"Unknown",
		"CreateWindow",
		"ChangeWindowAttributes",
		"GetWindowAttributes",
		"DestroyWindow",
		"DestroySubwindows",
		"ChangeSaveSet",
		"ReparentWindow",
		"MapWindow",
		"MapSubwindows",
		"UnmapWindow",
		"UnmapSubwindows",
		"ConfigureWindow",
		"CirculateWindow",
		"GetGeometry",
		"QueryTree",
		"InternAtom",
		"GetAtomName",
		"ChangeProperty",
		"DeleteProperty",
		"GetProperty",
		"ListProperties",
		"SetSelectionOwner",
		"GetSelectionOwner",
		"ConvertSelection",
		"SendEvent",
		"GrabPointer",
		"UngrabPointer",
		"GrabButton",
		"UngrabButton",
		"ChangeActivePointerGrab",
		"GrabKeyboard",
		"UngrabKeyboard",
		"GrabKey",
		"UngrabKey",
		"AllowEvents",
		"GrabServer",
		"UngrabServer",
		"QueryPointer",
		"GetMotionEvents",
		"TranslateCoords",
		"WarpPointer",
		"SetInputFocus",
		"GetInputFocus",
		"QueryKeymap",
		"OpenFont",
		"CloseFont",
		"QueryFont",
		"QueryTextExtents",
		"ListFonts",
		"ListFontsWithInfo",
		"SetFontPath",
		"GetFontPath",
		"CreatePixmap",
		"FreePixmap",
		"CreateGC",
		"ChangeGC",
		"CopyGC",
		"SetDashes",
		"SetClipRectangles",
		"FreeGC",
		"ClearArea",
		"CopyArea",
		"CopyPlane",
		"PolyPoint",
		"PolyLine",
		"PolySegment",
		"PolyRectangle",
		"PolyArc",
		"FillPoly",
		"PolyFillRectangle",
		"PolyFillArc",
		"PutImage",
		"GetImage",
		"PolyText8",
		"PolyText16",
		"ImageText8",
		"ImageText16",
		"CreateColormap",
		"FreeColormap",
		"CopyColormapAndFree",
		"InstallColormap",
		"UninstallColormap",
		"ListInstalledColormaps",
		"AllocColor",
		"AllocNamedColor",
		"AllocColorCells",
		"AllocColorPlanes",
		"FreeColors",
		"StoreColors",
		"StoreNamedColor",
		"QueryColors",
		"LookupColor",
		"CreateCursor",
		"CreateGlyphCursor",
		"FreeCursor",
		"RecolorCursor",
		"QueryBestSize",
		"QueryExtension",
		"ListExtensions",
		"ChangeKeyboardMapping",
		"GetKeyboardMapping",
		"ChangeKeyboardControl",
		"GetKeyboardControl",
		"Bell",
		"ChangePointerControl",
		"GetPointerControl",
		"SetScreenSaver",
		"GetScreenSaver",
		"ChangeHosts",
		"ListHosts",
		"SetAccessControl",
		"SetCloseDownMode",
		"KillClient",
		"RotateProperties",
		"ForceScreenSaver",
		"SetPointerMapping",
		"GetPointerMapping",
		"SetModifierMapping",
		"GetModifierMapping",
		"NoOperation",
	};


	// http://www.rahul.net/kenton/xproto/xevents_errors.html
	constexpr const char* error_str[] = {
		"Success",
		"BadRequest",
		"BadValue",
		"BadWindow",
		"BadPixmap",
		"BadAtom",
		"BadCursor",
		"BadFont",
		"BadMatch",
		"BadDrawable",
		"BadAccess",
		"BadAlloc",
		"BadColor",
		"BadGC",
		"BadIDChoice",
		"BadName",
		"BadLength",
		"BadImplementation",
	};


	// https://www.x.org/releases/X11R7.6/doc/xproto/x11protocol.html#errors
	constexpr const char* help_str[] = {
		"no error reported",
		"malformed request not handled",
		"a numeric parameter supplied to this request is out of range",
		"specified Window does not exist",
		"specified Pixmap does not exist",
		"specified Atom does not exist",
		"specified Cursor does not exist",
		"specified Font does not exist",
		"parameter has incorrect type or value range",
		"specified Drawable (Pixmap or Window) does not exist",
		"there might be a window manager already running",
		"there is not enough memory to allocate a resource",
		"specified Colormap does not exist",
		"specified GContext does not exist",
		"ID for resource identifier is not in range or already used",
		"specified font or colour name does not exist",
		"request length incorrect or too long for server to handle",
		"part/all of the request could not be handled due to lack of implementation",
	};


	constexpr const char* randr_event_str[] = {
		"XCB_RANDR_SCREEN_CHANGE_NOTIFY",
		"XCB_RANDR_NOTIFY"
	};


	constexpr const char* event_str[] = {
		"ERROR",
		"SUCCESS",
		"XCB_KEY_PRESS",
		"XCB_KEY_RELEASE",
		"XCB_BUTTON_PRESS",
		"XCB_BUTTON_RELEASE",
		"XCB_MOTION_NOTIFY",
		"XCB_ENTER_NOTIFY",
		"XCB_LEAVE_NOTIFY",
		"XCB_FOCUS_IN",
		"XCB_FOCUS_OUT",
		"XCB_KEYMAP_NOTIFY",
		"XCB_EXPOSE",
		"XCB_GRAPHICS_EXPOSURE",
		"XCB_NO_EXPOSURE",
		"XCB_VISIBILITY_NOTIFY",
		"XCB_CREATE_NOTIFY",
		"XCB_DESTROY_NOTIFY",
		"XCB_UNMAP_NOTIFY",
		"XCB_MAP_NOTIFY",
		"XCB_MAP_REQUEST",
		"XCB_REPARENT_NOTIFY",
		"XCB_CONFIGURE_NOTIFY",
		"XCB_CONFIGURE_REQUEST",
		"XCB_GRAVITY_NOTIFY",
		"XCB_RESIZE_REQUEST",
		"XCB_CIRCULATE_NOTIFY",
		"XCB_CIRCULATE_REQUEST",
		"XCB_PROPERTY_NOTIFY",
		"XCB_SELECTION_CLEAR",
		"XCB_SELECTION_REQUEST",
		"XCB_SELECTION_NOTIFY",
		"XCB_COLORMAP_NOTIFY",
		"XCB_CLIENT_MESSAGE",
		"XCB_MAPPING_NOTIFY",
		"XCB_GE_GENERIC",
		"XCB_REQUEST",
		"XCB_VALUE",
		"XCB_WINDOW",
		"XCB_PIXMAP",
		"XCB_ATOM",
		"XCB_CURSOR",
		"XCB_FONT",
		"XCB_MATCH",
		"XCB_DRAWABLE",
		"XCB_ACCESS",
		"XCB_ALLOC",
		"XCB_COLORMAP",
		"XCB_G_CONTEXT",
		"XCB_ID_CHOICE",
		"XCB_NAME",
		"XCB_LENGTH",
		"XCB_IMPLEMENTATION",
		"XCB_CREATE_WINDOW",
		"XCB_CHANGE_WINDOW_ATTRIBUTES",
		"XCB_GET_WINDOW_ATTRIBUTES",
		"XCB_DESTROY_WINDOW",
		"XCB_DESTROY_SUBWINDOWS",
		"XCB_CHANGE_SAVE_SET",
		"XCB_REPARENT_WINDOW",
		"XCB_MAP_WINDOW",
		"XCB_MAP_SUBWINDOWS",
		"XCB_UNMAP_WINDOW",
		"XCB_UNMAP_SUBWINDOWS",
		"XCB_CONFIGURE_WINDOW",
		"XCB_CIRCULATE_WINDOW",
		"XCB_GET_GEOMETRY",
		"XCB_QUERY_TREE",
		"XCB_INTERN_ATOM",
		"XCB_GET_ATOM_NAME",
		"XCB_CHANGE_PROPERTY",
		"XCB_DELETE_PROPERTY",
		"XCB_GET_PROPERTY",
		"XCB_LIST_PROPERTIES",
		"XCB_SET_SELECTION_OWNER",
		"XCB_GET_SELECTION_OWNER",
		"XCB_CONVERT_SELECTION",
		"XCB_SEND_EVENT",
		"XCB_GRAB_POINTER",
		"XCB_UNGRAB_POINTER",
		"XCB_GRAB_BUTTON",
		"XCB_UNGRAB_BUTTON",
		"XCB_CHANGE_ACTIVE_POINTER_GRAB",
		"XCB_GRAB_KEYBOARD",
		"XCB_UNGRAB_KEYBOARD",
		"XCB_GRAB_KEY",
		"XCB_UNGRAB_KEY",
		"XCB_ALLOW_EVENTS",
		"XCB_GRAB_SERVER",
		"XCB_UNGRAB_SERVER",
		"XCB_QUERY_POINTER",
		"XCB_GET_MOTION_EVENTS",
		"XCB_TRANSLATE_COORDINATES",
		"XCB_WARP_POINTER",
		"XCB_SET_INPUT_FOCUS",
		"XCB_GET_INPUT_FOCUS",
		"XCB_QUERY_KEYMAP",
		"XCB_OPEN_FONT",
		"XCB_CLOSE_FONT",
		"XCB_QUERY_FONT",
		"XCB_QUERY_TEXT_EXTENTS",
		"XCB_LIST_FONTS",
		"XCB_LIST_FONTS_WITH_INFO",
		"XCB_SET_FONT_PATH",
		"XCB_GET_FONT_PATH",
		"XCB_CREATE_PIXMAP",
		"XCB_FREE_PIXMAP",
		"XCB_CREATE_GC",
		"XCB_CHANGE_GC",
		"XCB_COPY_GC",
		"XCB_SET_DASHES",
		"XCB_SET_CLIP_RECTANGLES",
		"XCB_FREE_GC",
		"XCB_CLEAR_AREA",
		"XCB_COPY_AREA",
		"XCB_COPY_PLANE",
		"XCB_POLY_POINT",
		"XCB_POLY_LINE",
		"XCB_POLY_SEGMENT",
		"XCB_POLY_RECTANGLE",
		"XCB_POLY_ARC",
		"XCB_FILL_POLY",
		"XCB_POLY_FILL_RECTANGLE",
		"XCB_POLY_FILL_ARC",
		"XCB_PUT_IMAGE",
		"XCB_GET_IMAGE",
		"XCB_POLY_TEXT_8",
		"XCB_POLY_TEXT_16",
		"XCB_IMAGE_TEXT_8",
		"XCB_IMAGE_TEXT_16",
		"XCB_CREATE_COLORMAP",
		"XCB_FREE_COLORMAP",
		"XCB_COPY_COLORMAP_AND_FREE",
		"XCB_INSTALL_COLORMAP",
		"XCB_UNINSTALL_COLORMAP",
		"XCB_LIST_INSTALLED_COLORMAPS",
		"XCB_ALLOC_COLOR",
		"XCB_ALLOC_NAMED_COLOR",
		"XCB_ALLOC_COLOR_CELLS",
		"XCB_ALLOC_COLOR_PLANES",
		"XCB_FREE_COLORS",
		"XCB_STORE_COLORS",
		"XCB_STORE_NAMED_COLOR",
		"XCB_QUERY_COLORS",
		"XCB_LOOKUP_COLOR",
		"XCB_CREATE_CURSOR",
		"XCB_CREATE_GLYPH_CURSOR",
		"XCB_FREE_CURSOR",
		"XCB_RECOLOR_CURSOR",
		"XCB_QUERY_BEST_SIZE",
		"XCB_QUERY_EXTENSION",
		"XCB_LIST_EXTENSIONS",
		"XCB_CHANGE_KEYBOARD_MAPPING",
		"XCB_GET_KEYBOARD_MAPPING",
		"XCB_CHANGE_KEYBOARD_CONTROL",
		"XCB_GET_KEYBOARD_CONTROL",
		"XCB_BELL",
		"XCB_CHANGE_POINTER_CONTROL",
		"XCB_GET_POINTER_CONTROL",
		"XCB_SET_SCREEN_SAVER",
		"XCB_GET_SCREEN_SAVER",
		"XCB_CHANGE_HOSTS",
		"XCB_LIST_HOSTS",
		"XCB_SET_ACCESS_CONTROL",
		"XCB_SET_CLOSE_DOWN_MODE",
		"XCB_KILL_CLIENT",
		"XCB_ROTATE_PROPERTIES",
		"XCB_FORCE_SCREEN_SAVER",
		"XCB_SET_POINTER_MAPPING",
		"XCB_GET_POINTER_MAPPING",
		"XCB_SET_MODIFIER_MAPPING",
		"XCB_GET_MODIFIER_MAPPING",
		"XCB_NO_OPERATION",
	};

}

#endif
