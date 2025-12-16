#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/mstream.h>

#include "Model3DInfoPanel.h"

#include "filesystem/FileSystem.h"

// little helper so I don't have to use GLU (or my entire 3D engine...)
static void setPerspective(double fovY, double aspect, double zNear, double zFar)
{
    double tanF = tan(fovY * 0.017453293/*pi * (1/180)*/ * 0.5);

    double nearHeight = zNear * tanF;
    double nearWidth = nearHeight * aspect;

    glFrustum(-nearWidth, nearWidth, -nearHeight, nearHeight, zNear, zFar);
}

Model3DInfoPanel::Model3DInfoPanel(wxWindow *parent, FileSystem &fs) : Model3DInfoPanelBase(parent), fs(fs)
{
    context = std::make_unique<wxGLContext>(glCanvas);

    glCanvas->SetCurrent(*context);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // generate placeholder texture
    GLuint tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // draw into a bitmap
    const int size = 256;
    const int gridSize = 16;
    wxBitmap bitmap(size, size);
    wxMemoryDC dc;

    dc.SelectObject(bitmap);
    dc.SetBackground(*wxLIGHT_GREY_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxMEDIUM_GREY_BRUSH);

    for(int y = 0; y < size; y += gridSize)
    {
        int xOff = ((y / gridSize) & 1) * gridSize;
        for(int x = xOff; x < size; x += gridSize * 2)
            dc.DrawRectangle(x, y, gridSize, gridSize);
    }

    dc.SelectObject(wxNullBitmap);

    // convert to image and upload to texture
    auto image = bitmap.ConvertToImage();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
    placeholderTexture = tex;
}

bool Model3DInfoPanel::loadFile(const uint8_t *data, size_t size, std::string_view filename)
{
    if(!data || !modelFile.load(data, size))
    {
        infoLabel->SetLabel("Failed to load");
        return false;
    }

    infoLabel->SetLabel(wxString::Format("%u frames, %u triangles", modelFile.getNumFrames(), modelFile.getNumTriangles()));

    frameNumLabel->SetLabel(wxString::Format("0/%u", modelFile.getNumFrames()));

    if(modelFile.getNumFrames() > 1)
    {
        // animated
        animationSlider->SetMax(modelFile.getNumFrames() - 1);
    }
    else
    {
        // not animated
        animationToggle->Disable();
        animationTimer.Stop();
    }

    Layout();

    // the textures are located at Graphics1.res/models/[dir]/[.loc/.obj/.txt name][tex index padded to 4 digits].bmp
    // we only know the dir from the .3d file, fortunately there aren't many textures
    auto dirName = std::filesystem::path(filename).parent_path().stem().string();
    if(dirName == "geoff")
    {
        textures.push_back(loadTexture("geoff/Geoff", 0));
        textures.push_back(loadTexture("geoff/Geoff", 1));
    }
    else if(dirName == "Tracy")
    {
        textures.push_back(loadTexture("Tracy/Tracy", 0));
        textures.push_back(loadTexture("Tracy/Tracy", 1));
    }
    else if(dirName == "visitor")
    {
        textures.push_back(loadTexture("visitor/NewProject", 0));
        textures.push_back(loadTexture("visitor/NewProject", 1));
        textures.push_back(loadTexture("visitor/NewProject", 2));
        textures.push_back(loadTexture("visitor/NewProject", 3));
    }

    return true;
}

void Model3DInfoPanel::onGLPaint(wxPaintEvent &event)
{
    wxPaintDC dc(glCanvas);

    // not going to pull in a big pile of rendering code just for these simple models
    // so the old immediate mode will do

    // some more init...
    glCanvas->SetCurrent(*context);
    auto size = glCanvas->GetSize() * glCanvas->GetContentScaleFactor();
    glViewport(0, 0, size.GetWidth(), size.GetHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    setPerspective(45.0, double(size.GetWidth()) / size.GetHeight(), 1.0f, 150.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -100.0f);
    glRotatef(135.0f, 0.0f, 1.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw model
    int lastTexture = -2;

    glBegin(GL_TRIANGLES);
    for(unsigned i = 0; i < modelFile.getNumTriangles() * 3; i++)
    {
        auto vertex = modelFile.getVertex(frame, i);

        if(vertex.texIndex != lastTexture)
        {
            glEnd();

            // update texture state
            if(vertex.texIndex == -1)
                glDisable(GL_TEXTURE_2D);
            else
            {
                auto tex = vertex.texIndex >= textures.size() ? placeholderTexture : textures[vertex.texIndex];
                glBindTexture(GL_TEXTURE_2D, tex);
                glEnable(GL_TEXTURE_2D);
            }

            lastTexture = vertex.texIndex;

            // resume
            glBegin(GL_TRIANGLES);
        }

        glNormal3f(vertex.nor.x, -vertex.nor.y, vertex.nor.z);
        glColor3f(vertex.col.x, vertex.col.y, vertex.col.z);
        glTexCoord2f(vertex.tex.x, vertex.tex.y);
        glVertex3f(vertex.pos.x, -vertex.pos.y, vertex.pos.z);
    }
    glEnd();

    glCanvas->SwapBuffers();
}

void Model3DInfoPanel::onAnimationPlayToggle(wxCommandEvent &event)
{
    if(animationToggle->GetValue())
    {
        // start timer and disable slider
        animationToggle->SetLabel("Pause");
        animationTimer.Start();
        animationSlider->Disable();
    }
    else
    {
        // stop timer and enable slider
        animationToggle->SetLabel("Play");
        animationTimer.Stop();
        animationSlider->Enable();
    }
}

void Model3DInfoPanel::onAnimationSliderScroll(wxScrollEvent &event)
{
    frame = animationSlider->GetValue();
    frameNumLabel->SetLabel(wxString::Format("%u/%u", frame, modelFile.getNumFrames()));
    // redraw
    glCanvas->Refresh();
}

void Model3DInfoPanel::onAnimationTimer(wxTimerEvent &event)
{
    // advance animation and wrap
    frame++;

    if(frame >= modelFile.getNumFrames())
        frame = 0;

    // update status and redraw
    frameNumLabel->SetLabel(wxString::Format("%u/%u", frame, modelFile.getNumFrames()));
    animationSlider->SetValue(frame);

    glCanvas->Refresh();
}

unsigned Model3DInfoPanel::loadTexture(std::string_view path, int index)
{
    auto fullPath = wxString::Format("Volumes/Graphics1.res/Graphics/models/%s%04i.bmp", path, index);

    // get file data
    auto contents = fs.getFileContents(fullPath.ToStdString());
    wxImage image;

    if(!contents)
    {
        wxLogMessage("Failed to find %s", fullPath);
        return placeholderTexture;
    }

    // load image
    wxMemoryInputStream stream(contents->data(), contents->size());
    if(!image.LoadFile(stream))
    {
        wxLogMessage("Failed to read %s", fullPath);
        return placeholderTexture;
    }

    // create texture
    GLuint tex;
    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    auto size = image.GetSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.GetWidth(), size.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());

    return tex;
}
