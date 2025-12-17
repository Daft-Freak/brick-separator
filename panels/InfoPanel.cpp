#include "InfoPanel.h"

// this may be a bit redundant...
static const char *bitmapTypeName(wxBitmapType type)
{
    switch(type)
    {
        case wxBITMAP_TYPE_BMP:
            return "BMP";
        case wxBITMAP_TYPE_ICO:
            return "ICO";
        case wxBITMAP_TYPE_CUR:
            return "CUR";
        case wxBITMAP_TYPE_XBM:
            return "XBM";
        case wxBITMAP_TYPE_XPM:
            return "XPM";
        case wxBITMAP_TYPE_TIFF:
            return "TIFF";
        case wxBITMAP_TYPE_GIF:
            return "GIF";
        case wxBITMAP_TYPE_PNG:
            return "PNG";
        case wxBITMAP_TYPE_JPEG:
            return "JPEG";
        case wxBITMAP_TYPE_PNM:
            return "PNM";
        case wxBITMAP_TYPE_PCX:
            return "PCX";
        case wxBITMAP_TYPE_PICT:
            return "PICT";
        case wxBITMAP_TYPE_ICON:
            return "ICON";
        case wxBITMAP_TYPE_ANI:
            return "ANI";
        case wxBITMAP_TYPE_IFF:
            return "IFF";
        case wxBITMAP_TYPE_TGA:
            return "TGA";

        default:
            return "image";
    }
}

void ImageInfoPanel::setImage(const wxImage &image)
{
    bitmap->SetBitmap(wxBitmapBundle::FromImage(image));

    auto size = image.GetSize();
    auto type = bitmapTypeName(image.GetType());

    imageInfoLabel->SetLabel(wxString::Format("%ix%i %s", size.GetWidth(), size.GetHeight(), type));

    scrollWindow->SetMaxSize(bitmap->GetSize());
}