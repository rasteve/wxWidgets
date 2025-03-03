/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcsvg.h
// Purpose:     wxSVGFileDC
// Author:      Chris Elliott
// Created:
// Copyright:   (c) Chris Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSVG_H_
#define _WX_DCSVG_H_

#include "wx/defs.h"

#if wxUSE_SVG

#include "wx/string.h"
#include "wx/filename.h"
#include "wx/dc.h"

#include <memory>

#define wxSVGVersion wxT("v0101")

enum wxSVGShapeRenderingMode
{
    wxSVG_SHAPE_RENDERING_AUTO = 0,
    wxSVG_SHAPE_RENDERING_OPTIMIZE_SPEED,
    wxSVG_SHAPE_RENDERING_CRISP_EDGES,
    wxSVG_SHAPE_RENDERING_GEOMETRIC_PRECISION,

    wxSVG_SHAPE_RENDERING_OPTIMISE_SPEED = wxSVG_SHAPE_RENDERING_OPTIMIZE_SPEED
};

class WXDLLIMPEXP_FWD_BASE wxFileOutputStream;

class WXDLLIMPEXP_FWD_CORE wxSVGFileDC;

// Base class for bitmap handlers used by wxSVGFileDC, used by the standard
// "embed" and "link" handlers below but can also be used to create a custom
// handler.
class WXDLLIMPEXP_CORE wxSVGBitmapHandler
{
public:
    // Write the representation of the given bitmap, appearing at the specified
    // position, to the provided stream.
    virtual bool ProcessBitmap(const wxBitmap& bitmap,
                               wxCoord x, wxCoord y,
                               wxOutputStream& stream) const = 0;

    virtual ~wxSVGBitmapHandler() = default;
};

// Predefined standard bitmap handler: creates a file, stores the bitmap in
// this file and uses the file name in the generated SVG.
class WXDLLIMPEXP_CORE wxSVGBitmapFileHandler : public wxSVGBitmapHandler
{
public:
    wxSVGBitmapFileHandler()
        : m_path()
    {
    }

    explicit wxSVGBitmapFileHandler(const wxFileName& path)
        : m_path(path)
    {
    }

    virtual bool ProcessBitmap(const wxBitmap& bitmap,
                               wxCoord x, wxCoord y,
                               wxOutputStream& stream) const override;

private:
    wxFileName m_path; // When set, name will be appended with _image#.png
};

// Predefined handler which embeds the bitmap (base64-encoding it) inside the
// generated SVG file.
class WXDLLIMPEXP_CORE wxSVGBitmapEmbedHandler : public wxSVGBitmapHandler
{
public:
    virtual bool ProcessBitmap(const wxBitmap& bitmap,
                               wxCoord x, wxCoord y,
                               wxOutputStream& stream) const override;
};

class WXDLLIMPEXP_CORE wxSVGFileDCImpl : public wxDCImpl
{
public:
    wxSVGFileDCImpl(wxSVGFileDC* owner, const wxString& filename,
                    int width = 320, int height = 240, double dpi = 72.0,
                    const wxString& title = wxString());

    virtual ~wxSVGFileDCImpl();

    bool IsOk() const override { return m_OK; }

    virtual bool CanDrawBitmap() const override { return true; }
    virtual bool CanGetTextExtent() const override { return true; }

    virtual int GetDepth() const override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::GetDepth Call not implemented"));
        return -1;
    }

    virtual void Clear() override;

    virtual void DestroyClippingRegion() override;

    virtual wxCoord GetCharHeight() const override;
    virtual wxCoord GetCharWidth() const override;

#if wxUSE_PALETTE
    virtual void SetPalette(const wxPalette& WXUNUSED(palette)) override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::SetPalette not implemented"));
    }
#endif

    virtual void SetLogicalFunction(wxRasterOperationMode WXUNUSED(function)) override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::SetLogicalFunction Call not implemented"));
    }

    virtual wxRasterOperationMode GetLogicalFunction() const override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::GetLogicalFunction() not implemented"));
        return wxCOPY;
    }

    virtual void ComputeScaleAndOrigin() override;

    virtual void SetBackground(const wxBrush& brush) override;
    virtual void SetBackgroundMode(int mode) override;
    virtual void SetBrush(const wxBrush& brush) override;
    virtual void SetFont(const wxFont& font) override;
    virtual void SetPen(const wxPen& pen) override;

    virtual void* GetHandle() const override { return nullptr; }

    void SetBitmapHandler(wxSVGBitmapHandler* handler);

    void SetShapeRenderingMode(wxSVGShapeRenderingMode renderingMode);

private:
    virtual bool DoGetPixel(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                            wxColour* WXUNUSED(col)) const override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::DoGetPixel Call not implemented"));
        return true;
    }

    virtual bool DoBlit(wxCoord xdest, wxCoord ydest,
                        wxCoord width, wxCoord height,
                        wxDC* source,
                        wxCoord xsrc, wxCoord ysrc,
                        wxRasterOperationMode rop,
                        bool useMask = false,
                        wxCoord xsrcMask = wxDefaultCoord,
                        wxCoord ysrcMask = wxDefaultCoord) override;

    virtual void DoCrossHair(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y)) override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::CrossHair Call not implemented"));
    }

    virtual void DoDrawArc(wxCoord x1, wxCoord y1,
                           wxCoord x2, wxCoord y2,
                           wxCoord xc, wxCoord yc) override;

    virtual void DoDrawBitmap(const wxBitmap& bmp, wxCoord x, wxCoord y,
                              bool useMask = false) override;

    virtual void DoDrawEllipse(wxCoord x, wxCoord y,
                               wxCoord width, wxCoord height) override;

    virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                   double sa, double ea) override;

    virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y) override;

    virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2) override;

    virtual void DoDrawLines(int n, const wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset) override;
#if wxUSE_SPLINES
    void DoDrawSpline(const wxPointList* points) override;
#endif // wxUSE_SPLINES

    virtual void DoDrawPoint(wxCoord x, wxCoord y) override;

    virtual void DoDrawPolygon(int n, const wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               wxPolygonFillMode fillStyle = wxODDEVEN_RULE) override;

    virtual void DoDrawPolyPolygon(int n, const int count[], const wxPoint points[],
                                   wxCoord xoffset, wxCoord yoffset,
                                   wxPolygonFillMode fillStyle) override;

    virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height) override;

    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                   double angle) override;

    virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                        wxCoord width, wxCoord height,
                                        double radius) override;

    virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y) override;

    virtual bool DoFloodFill(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                             const wxColour& WXUNUSED(col),
                             wxFloodFillStyle WXUNUSED(style)) override
    {
        wxFAIL_MSG(wxT("wxSVGFILEDC::DoFloodFill Call not implemented"));
        return false;
    }

    virtual void DoGradientFillLinear(const wxRect& rect,
                                      const wxColour& initialColour,
                                      const wxColour& destColour,
                                      wxDirection nDirection) override;

    virtual void DoGradientFillConcentric(const wxRect& rect,
                                          const wxColour& initialColour,
                                          const wxColour& destColour,
                                          const wxPoint& circleCenter) override;

    virtual void DoGetSize(int* width, int* height) const override
    {
        if ( width )
            *width = m_width;
        if ( height )
            *height = m_height;
    }

    virtual void DoGetTextExtent(const wxString& string,
                                 wxCoord* x, wxCoord* y,
                                 wxCoord* descent = nullptr,
                                 wxCoord* externalLeading = nullptr,
                                 const wxFont* theFont = nullptr) const override;

    virtual void DoSetDeviceClippingRegion(const wxRegion& region) override;

    virtual void DoSetClippingRegion(wxCoord x, wxCoord y,
                                     wxCoord w, wxCoord h) override;

    virtual void DoGetSizeMM(int* width, int* height) const override;

    virtual wxSize GetPPI() const override;

    virtual wxSize FromDIP(const wxSize& sz) const override;

    virtual wxSize ToDIP(const wxSize& sz) const override;

    void Init(const wxString& filename, int width, int height,
              double dpi, const wxString& title);

    void write(const wxString& s);

private:
    // If m_graphics_changed is true, close the current <g> element and start a
    // new one for the last pen/brush change.
    void NewGraphicsIfNeeded();

    // Open a new graphics group setting up all the attributes according to
    // their current values in wxDC.
    void DoStartNewGraphics();

    wxString            m_filename;
    bool                m_OK;
    bool                m_graphics_changed;  // set by Set{Brush,Pen}()
    int                 m_width, m_height;
    double              m_dpi;
    std::unique_ptr<wxFileOutputStream> m_outfile;
    std::unique_ptr<wxSVGBitmapHandler> m_bmp_handler; // class to handle bitmaps
    wxSVGShapeRenderingMode m_renderingMode;

    // The clipping nesting level is incremented by every call to
    // SetClippingRegion() and reset when DestroyClippingRegion() is called.
    size_t m_clipNestingLevel;

    // Unique ID for every clipping graphics group: this is simply always
    // incremented in each SetClippingRegion() call.
    size_t m_clipUniqueId;

    // Unique ID for every gradient.
    size_t m_gradientUniqueId;

    wxDECLARE_ABSTRACT_CLASS(wxSVGFileDCImpl);
    wxDECLARE_NO_COPY_CLASS(wxSVGFileDCImpl);
};


class WXDLLIMPEXP_CORE wxSVGFileDC : public wxDC
{
public:
    wxSVGFileDC(const wxString& filename,
                int width = 320,
                int height = 240,
                double dpi = 72.0,
                const wxString& title = wxString())
        : wxDC(new wxSVGFileDCImpl(this, filename, width, height, dpi, title))
    {
    }

    // wxSVGFileDC-specific methods:

    // Use a custom bitmap handler: takes ownership of the handler.
    void SetBitmapHandler(wxSVGBitmapHandler* handler);

    void SetShapeRenderingMode(wxSVGShapeRenderingMode renderingMode);

private:
    wxDECLARE_ABSTRACT_CLASS(wxSVGFileDC);
};

#endif // wxUSE_SVG

#endif // _WX_DCSVG_H_
