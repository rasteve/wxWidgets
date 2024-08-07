/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/spinbutt.h
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_SPINBUTT_H_
#define _WX_QT_SPINBUTT_H_

#include "wx/spinbutt.h"
class QSpinBox;

class WXDLLIMPEXP_CORE wxSpinButton : public wxSpinButtonBase
{
public:
    wxSpinButton() = default;

    wxSpinButton(wxWindow *parent,
                 wxWindowID id = -1,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxSP_VERTICAL,
                 const wxString& name = wxSPIN_BUTTON_NAME);

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_VERTICAL,
                const wxString& name = wxSPIN_BUTTON_NAME);

    virtual int GetValue() const override;
    virtual void SetValue(int val) override;
    virtual void SetRange(int min, int max) override;

    QSpinBox* GetQSpinBox() const;

    wxDECLARE_DYNAMIC_CLASS(wxSpinButton);
};

#endif // _WX_QT_SPINBUTT_H_
