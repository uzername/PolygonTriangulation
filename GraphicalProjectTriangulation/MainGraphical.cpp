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
    max_width = aFinalTriangulation.initialvertlist[0][0][0]; max_height = aFinalTriangulation.initialvertlist[0][0][1];
    min_width = aFinalTriangulation.initialvertlist[0][0][0]; min_height = aFinalTriangulation.initialvertlist[0][0][1];
    for (int i = 2; i < aFinalTriangulation.initialvertlist[0].size(); i += 2) {
        if (max_width < aFinalTriangulation.initialvertlist[0][i][0]) {
            max_width = aFinalTriangulation.initialvertlist[0][i][0];
        }
        if (max_height < aFinalTriangulation.initialvertlist[0][i][1]) {
            max_height = aFinalTriangulation.initialvertlist[0][i][1];
        }
        if (min_width > aFinalTriangulation.initialvertlist[0][i][0]) {
            min_width = aFinalTriangulation.initialvertlist[0][i][0];
        }
        if (min_height > aFinalTriangulation.initialvertlist[0][i][1]) {
            min_height = aFinalTriangulation.initialvertlist[0][i][1];
        }
    }


    return true;
}

MyApp::~MyApp()
{
    
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
    //draw the outer contour
    for (int i = 0; i < aFinalTriangulation.initialvertlist[0].size(); i += 1) {
    int begincoordinate_h = offset + int((dc_width)/scalingFactor * aFinalTriangulation.initialvertlist[0][i][0] );
    int begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[0][i][1]);
    int endcoordinate_h; int endcoordinate_v;
    if (  i + 1 >= aFinalTriangulation.initialvertlist[0].size()  ) {
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[0][0][0]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[0][0][1]);
    } else {
        endcoordinate_h = offset + int((dc_width ) / scalingFactor * aFinalTriangulation.initialvertlist[0][i+1][0]);
        endcoordinate_v = offset + int((dc_height ) / scalingFactor * aFinalTriangulation.initialvertlist[0][i+1][1]);
    }
    dc.DrawLine(wxPoint{begincoordinate_h, begincoordinate_v}, wxPoint{endcoordinate_h, endcoordinate_v});
    }
    for (int i = 1; i < aFinalTriangulation.initialvertlist.size(); i += 1) {
        for (int j = 0; j < aFinalTriangulation.initialvertlist[i].size(); j += 1) {
            int begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[i][j][0]);
            int begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[i][j][1]);
            int endcoordinate_h; int endcoordinate_v;
            if (j + 1 >= aFinalTriangulation.initialvertlist[i].size()) {
                endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[i][0][0]);
                endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[i][0][1]);
            }
            else {
                endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[i][j + 1][0]);
                endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[i][j + 1][1]);
            }
            dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        }
    }
    
    dc.SetPen(wxPen(wxColor(0, 0, 255), 1)); // 1-pixels-thick blue outline
    int i = 0;
    while ( i < aFinalTriangulation.finalvertlist.size()-2 ) {
        int begincoordinate_h; int begincoordinate_v; int endcoordinate_h; int endcoordinate_v;
        std::tuple<eNochka, eNochka> theTuple = getCoordinateByIndice(aFinalTriangulation.finalvertlist[i]); eNochka contour = std::get<0>(theTuple); eNochka contourItem = std::get<1>(theTuple);
        std::tuple<eNochka, eNochka> theTuple1 = getCoordinateByIndice(aFinalTriangulation.finalvertlist[i+1]); eNochka contour1 = std::get<0>(theTuple1); eNochka contourItem1 = std::get<1>(theTuple1);
        std::tuple<eNochka, eNochka> theTuple2 = getCoordinateByIndice(aFinalTriangulation.finalvertlist[i+2]); eNochka contour2 = std::get<0>(theTuple2); eNochka contourItem2 = std::get<1>(theTuple2);

        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour ][contourItem][0]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour][contourItem][1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour1][contourItem1][0]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour1][contourItem1][1]);
        dc.DrawText(wxT("+"), wxPoint{ begincoordinate_h, begincoordinate_v }); dc.DrawText(wxT("+"), wxPoint{ endcoordinate_h, endcoordinate_v });
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour1][contourItem1][0]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour1][contourItem1][1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour2][contourItem2][0]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour2][contourItem2][1]);
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        begincoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour2][contourItem2][0]);
        begincoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour2][contourItem2][1]);
        endcoordinate_h = offset + int((dc_width) / scalingFactor * aFinalTriangulation.initialvertlist[contour][contourItem][0]);
        endcoordinate_v = offset + int((dc_height) / scalingFactor * aFinalTriangulation.initialvertlist[contour][contourItem][1]);
        dc.DrawText(wxT("+"), wxPoint{ endcoordinate_h, endcoordinate_v });
        dc.DrawLine(wxPoint{ begincoordinate_h, begincoordinate_v }, wxPoint{ endcoordinate_h, endcoordinate_v });
        i+=3;
    }
    
#endif

}