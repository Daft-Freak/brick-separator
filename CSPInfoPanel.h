#pragma once

#include "BrickSeparatorBase.h"

#include "CSPFile.h"
#include "LLSFile.h"

class FileSystem;

class CSPInfoPanel final : public CSPInfoPanelBase
{
public:
    CSPInfoPanel(wxWindow *parent, FileSystem &fs);

    bool loadFile(const uint8_t *data, size_t size);

private:
    void onSpritePaint(wxPaintEvent &event) override;
    void onAnimationPlayToggle(wxCommandEvent &event) override;
    void onAnimationTimer(wxTimerEvent &event) override;

    void loadSprite(const std::string &name);

    FileSystem &fs;

    CSPFile compositeFile;

    std::vector<LLSFile> spriteFiles;
    std::vector<unsigned> spriteFrames;

    int minX = 0, minY = 0, maxX = 0, maxY = 0;
};
