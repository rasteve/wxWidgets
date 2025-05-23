///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/ole/dropsrc.h
// Purpose:     declaration of the wxDropSource class
// Author:      Vadim Zeitlin
// Created:     06.03.98
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef   _WX_OLEDROPSRC_H
#define   _WX_OLEDROPSRC_H

#if wxUSE_DRAG_AND_DROP

// ----------------------------------------------------------------------------
// forward declarations
// ----------------------------------------------------------------------------

class wxIDropSource;
class WXDLLIMPEXP_FWD_CORE wxDataObject;
class WXDLLIMPEXP_FWD_CORE wxWindow;

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

// this macro may be used instead for wxDropSource ctor arguments: it will use
// the cursor 'name' from the resources under MSW, but will expand to
// something else under GTK. If you don't use it, you will have to use #ifdef
// in the application code.
#define wxDROP_ICON(name)   wxCursor(wxT(#name))

// ----------------------------------------------------------------------------
// wxDropSource is used to start the drag-&-drop operation on associated
// wxDataObject object. It's responsible for giving UI feedback while dragging.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxDropSource : public wxDropSourceBase
{
public:
    // ctors: if you use default ctor you must call SetData() later!
    //
    // NB: the "wxWindow *win" parameter is unused and is here only for wxGTK
    //     compatibility, as well as both icon parameters
    wxDropSource(wxWindow *win = nullptr,
                 const wxCursorBundle& cursorCopy = {},
                 const wxCursorBundle& cursorMove = {},
                 const wxCursorBundle& cursorStop = {});
    wxDropSource(wxDataObject& data,
                 wxWindow *win = nullptr,
                 const wxCursorBundle& cursorCopy = {},
                 const wxCursorBundle& cursorMove = {},
                 const wxCursorBundle& cursorStop = {});

    virtual ~wxDropSource();

    // do it (call this in response to a mouse button press, for example)
    // params: if bAllowMove is false, data can be only copied
    virtual wxDragResult DoDragDrop(int flags = wxDrag_CopyOnly) override;

    // overridable: you may give some custom UI feedback during d&d operation
    // in this function (it's called on each mouse move, so it shouldn't be
    // too slow). Just return false if you want default feedback.
    virtual bool GiveFeedback(wxDragResult effect) override;

protected:
    void Init();

private:
    // The window passed to the ctor.
    wxWindow* const m_win;

    wxIDropSource *m_pIDropSource;  // the pointer to COM interface

    // Last cursor used in GiveFeedback(): we need to keep it alive to ensure
    // its HCURSOR remains valid.
    wxCursor m_feedbackCursor;

    wxDECLARE_NO_COPY_CLASS(wxDropSource);
};

#endif  //wxUSE_DRAG_AND_DROP

#endif  //_WX_OLEDROPSRC_H
