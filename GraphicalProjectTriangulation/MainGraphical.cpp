#include <wx/wx.h>
#include <wx/sizer.h>

#include "TriangulationFile.h"

double max_width = 0; double min_width = 0;
double max_height = 0; double min_height = 0;

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    // some useful events
    /*
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    */
    
    DECLARE_EVENT_TABLE()
};


class MyApp : public wxApp
{
    bool OnInit();
    ~MyApp();
    wxFrame *frame;
    BasicDrawPane * drawPane;
public:

};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1, wxT("Hello wxDC"), wxPoint(50, 50), wxSize(800, 600));

    drawPane = new BasicDrawPane((wxFrame*)frame);
    sizer->Add(drawPane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Show();

    aFinalTriangulation = performtesttriangulation();
    max_width = aFinalTriangulation.initialvertlist[0]; max_height = aFinalTriangulation.initialvertlist[1];
    min_width = aFinalTriangulation.initialvertlist[0]; min_height = aFinalTriangulation.initialvertlist[1];
    for (int i = 2; i < aFinalTriangulation.vertcount * 2; i += 2) {
        if (max_width < aFinalTriangulation.initialvertlist[i]) {
            max_width = aFinalTriangulation.initialvertlist[i];
        }
        if (max_height < aFinalTriangulation.initialvertlist[i + 1]) {
            max_height = aFinalTriangulation.initialvertlist[i + 1];
        }
        if (min_width > aFinalTriangulation.initialvertlist[i]) {
            min_width = aFinalTriangulation.initialvertlist[i];
        }
        if (min_height > aFinalTriangulation.initialvertlist[i + 1]) {
            min_height = aFinalTriangulation.initialvertlist[i + 1];
        }
    }


    return true;
}

MyApp::~MyApp()
{
    if (aFinalTriangulation.free_required) {
    free(aFinalTriangulation.initialvertlist);
    free(aFinalTriangulation.triangleindiceslist);
    free(aFinalTriangulation.finalvertlist);
    }
}

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
EVT_MOTION(BasicDrawPane::mouseMoved)
EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
EVT_LEFT_UP(BasicDrawPane::mouseReleased)
EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
EVT_KEY_DOWN(BasicDrawPane::keyPressed)
EVT_KEY_UP(BasicDrawPane::keyReleased)
EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
*/

// catch paint events
EVT_PAINT(BasicDrawPane::paintEvent)

END_EVENT_TABLE()


// some useful events
/*
void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
void BasicDrawPane::mouseDown(wxMouseEvent& event) {}
void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
void BasicDrawPane::rightClick(wxMouseEvent& event) {}
void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
*/

BasicDrawPane::BasicDrawPane(wxFrame* parent) :
    wxPanel(parent)
{
}

/*
* Called by the system of by wxWidgets when the panel needs
* to be redrawn. You can also trigger this call by
* calling Refresh()/Update().
*/

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

/*
* Alternatively, you can use a clientDC to paint on the panel
* at any time. Using this generally does not free you from
* catching paint events, since it is possible that e.g. the window
* manager throws away your drawing when the window comes to the
* background, and expects you will redraw it when the window comes
* back (by sending a paint event).
*
* In most cases, this will not be needed at all; simply handling
* paint events and calling Refresh() when a refresh is needed
* will do the job.
*/
void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

/*
* Here we do the actual rendering. I put it in a separate
* method so that it can work no matter what type of DC
* (e.g. wxPaintDC or wxClientDC) is used.
*/
void BasicDrawPane::render(wxDC&  dc)
{
#ifdef PERFORMTESTING
    // draw some text
    dc.DrawText(wxT("Testing"), 40, 60);
    // draw a circle
    dc.SetBrush(*wxGREEN_BRUSH); // green filling
    dc.SetPen(wxPen(wxColor(255, 0, 0), 5)); // 5-pixels-thick red outline
    dc.DrawCircle(wxPoint(200, 100), 25 /* radius */);
    // draw a rectangle
    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline
    dc.DrawRectangle(300, 100, 400, 200);
    // draw a line
    dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
    dc.DrawLine(300, 100, 700, 300); // draw line across the rectangle
                                     // Look at the wxDC docs to learn how to draw other stuff
#else
    int dc_width = 0; int dc_height = 0;
    int offset = 0; double scalingFactor = ((max_width - min_width)> (max_height - min_height)) ? (max_width - min_width) : (max_height - min_height);
    dc.GetSize(&dc_width, &dc_height);
    dc.SetPen(wxPen(wxColor(255, 0, 0), 3)); // 3-pixels-thick red outline
    if ((max_height == min_height) || (max_width == min_width)) {
    return;
    }
    for (int i = 0; i < aFinalTriangulation.vertcount * 2; i += 2) {
    int begincoordinate_h = offset + int((dc_width)/scalingFactor * aFinalTriangulation.initialvertlist[i] );
    int begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[i+1]);
    int endcoordinate_h; int endcoordinate_v;
    if ((i + 2 >= aFinalTriangulation.vertcount*2)||(i + 3 >= aFinalTriangulation.vertcount*2)) {
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[0]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[1]);
    } else {
        endcoordinate_h = offset + int((dc_width ) / scalingFactor * aFinalTriangulation.initialvertlist[i + 2]);
        endcoordinate_v = offset + int((dc_height ) / scalingFactor * aFinalTriangulation.initialvertlist[i+3]);
    }
    dc.DrawLine(wxPoint{begincoordinate_h, begincoordinate_v}, wxPoint{endcoordinate_h, endcoordinate_v});
    }
    dc.SetPen(wxPen(wxColor(0, 0, 255), 1)); // 1-pixels-thick blue outline
    for (int i = 0; i < aFinalTriangulation.triangleindiceslistcount * 3; i+=3) {
        int begincoordinate_h; int begincoordinate_v; int endcoordinate_h; int endcoordinate_v;
        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i]*2 ]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i]*2+1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i+1]*2]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i+1]*2 + 1]);
        dc.DrawText(wxT("+"), wxPoint{ begincoordinate_h, begincoordinate_v }); dc.DrawText(wxT("+"), wxPoint{ endcoordinate_h, endcoordinate_v });
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i+1]*2]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i+1]*2 + 1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i+2]*2]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i + 2]*2 + 1]);
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i]*2]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i]*2 + 1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i +2]*2]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.finalvertlist[aFinalTriangulation.triangleindiceslist[i +2]*2 + 1]);
        dc.DrawText(wxT("+"), wxPoint{ endcoordinate_h, endcoordinate_v });
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        
    }
#endif

}