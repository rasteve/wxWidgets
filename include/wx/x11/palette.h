/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/palette.h
// Purpose:     wxPalette class
// Author:      Julian Smart
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_
#define _WX_PALETTE_H_

#include "wx/list.h"

class WXDLLIMPEXP_FWD_CORE wxPalette;

// Palette for one display
class wxXPalette : public wxObject
{
    wxDECLARE_DYNAMIC_CLASS(wxXPalette);

public:
    wxXPalette();

    WXDisplay*        m_display;
    int               m_pix_array_n;
    unsigned char*    m_red;
    unsigned char*    m_green;
    unsigned char*    m_blue;
    unsigned long*    m_pix_array;
    WXColormap        m_cmap;
    bool              m_destroyable;
};

class WXDLLIMPEXP_CORE wxPaletteRefData: public wxGDIRefData
{
    friend class WXDLLIMPEXP_FWD_CORE wxPalette;
public:
    wxPaletteRefData();
    virtual ~wxPaletteRefData();

protected:
    wxList  m_palettes;
};

#define M_PALETTEDATA ((wxPaletteRefData *)m_refData)

class WXDLLIMPEXP_CORE wxPalette : public wxPaletteBase
{
    wxDECLARE_DYNAMIC_CLASS(wxPalette);

public:
    wxPalette();

    wxPalette(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);
    bool Create(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);
    int GetPixel(unsigned char red, unsigned char green, unsigned char blue) const;
    bool GetRGB(int pixel, unsigned char *red, unsigned char *green, unsigned char *blue) const;

    // X-specific
    WXColormap GetXColormap(WXDisplay* display = nullptr) const;
    bool TransferBitmap(void *data, int depth, int size);
    bool TransferBitmap8(unsigned char *data, unsigned long size, void *dest, unsigned int bpp);
    unsigned long *GetXPixArray(WXDisplay* display, int *pix_array_n);
    void PutXColormap(WXDisplay* display, WXColormap cmap, bool destroyable);
    virtual int GetColoursCount() const override;

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    wxNODISCARD virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;
};

#endif // _WX_PALETTE_H_
