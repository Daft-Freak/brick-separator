#pragma once
#include <memory>

#include "BrickSeparatorBase.h"

#include "formats/Model3DFile.h"

class FileSystem;

class Model3DInfoPanel final : public Model3DInfoPanelBase
{
public:
    Model3DInfoPanel(wxWindow *parent, FileSystem &fs);

    bool loadFile(const uint8_t *data, size_t size, std::string_view filename);

private:
    void onGLPaint(wxPaintEvent &event) override;
    void onAnimationPlayToggle(wxCommandEvent &event) override;
    void onAnimationSliderScroll(wxScrollEvent &event) override;
    void onAnimationTimer(wxTimerEvent &event) override;

    unsigned loadTexture(std::string_view path, int index);

    FileSystem &fs;

    std::unique_ptr<wxGLContext> context;
    unsigned placeholderTexture;

    Model3DFile modelFile;
    unsigned frame = 0;

    std::vector<unsigned> textures;
};
