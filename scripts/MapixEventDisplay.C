#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>

class MyMainFrame {
   RQ_OBJECT("MyMainFrame")
private:
   TGMainFrame         *fMain;
   TRootEmbeddedCanvas *fEcanvas;
public:
   MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
   virtual ~MyMainFrame();
   void DoDraw();
};
MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   fMain = new TGMainFrame(p,w,h);

/*  // Create title frame
   ULong_t blackcolor;
   gClient->GetColorByName("blue", blackcolor);
   TGHorizontalFrame *titleframe = new TGHorizontalFrame(fMain,1500,40);
   TGLabel *title = new TGLabel(titleframe, "mapiX Event Display");
   titleframe->AddFrame(title, new TGLayoutHints(kLHintsLeft,5,5,2,2) );   
   fMain->AddFrame(titleframe, new TGLayoutHints(kLHintsLeft,5,5,20,5));   
   title->SetTextColor(blackcolor);
*/
   // Create a horizontal frame
   TGHorizontalFrame *hframe1 = new TGHorizontalFrame(fMain,1500,1000);

   // Create vertical frames
   TGVerticalFrame *vframe1 = new TGVerticalFrame(hframe1,400,1000);
   TGVerticalFrame *vframe2 = new TGVerticalFrame(hframe1,1000,1000);

   // 1st vertical frame
   TGHorizontalFrame *vhframe1 = new TGHorizontalFrame(vframe1,400,40);
   
   TGLabel *goTo = new TGLabel(vhframe1, "Go to: ");
   vhframe1->AddFrame(goTo, new TGLayoutHints(kLHintsLeft,0,5,2,2) );

   TGListBox *listbox = new TGListBox(vhframe1, 100, 100);
   char tmp[10];
   for (int i = 0; i < 10; ++i) {
      sprintf(tmp, "Event %i", i+1);
      listbox->AddEntry(tmp, i+1);
   }
   listbox->Resize(100,150);
   vhframe1->AddFrame(listbox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5) );

   vframe1->AddFrame(vhframe1, new TGLayoutHints(kLHintsLeft,5,5,2,2) );

   TGLabel *event = new TGLabel(vframe1, "Event");
   vframe1->AddFrame(event, new TGLayoutHints(kLHintsLeft,5,5,2,2) );
 
   TGLabel *run = new TGLabel(vframe1, "Run");
   vframe1->AddFrame(run, new TGLayoutHints(kLHintsLeft,5,5,2,2) );

   TGLabel *spill = new TGLabel(vframe1, "Spill");
   vframe1->AddFrame(spill, new TGLayoutHints(kLHintsLeft,5,5,2,2) );

   TGTextButton *next = new TGTextButton(vframe1,"&Next");
   next->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   next->Resize(100,40);
   vframe1->AddFrame(next, new TGLayoutHints(kLHintsExpandX,5,5,50,5) );

   TGTextButton *previous = new TGTextButton(vframe1,"&Previous");
   previous->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   vframe1->AddFrame(previous, new TGLayoutHints(kLHintsExpandX,5,5,5,5) );
   
   TGTextButton *quit = new TGTextButton(vframe1,"&Quit", "gApplication->Terminate(0)");
   vframe1->AddFrame(quit, new TGLayoutHints(kLHintsExpandX | kLHintsBottom,5,5,5,5) );
   
   hframe1->AddFrame(vframe1, new TGLayoutHints(kLHintsExpandY,5,5,30,100));

   // 2nd vertical frame
   TRootEmbeddedCanvas *canvas1 = new TRootEmbeddedCanvas("Canvas1",vframe2,200,200);   
   vframe2->AddFrame(canvas1, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));   

   TRootEmbeddedCanvas *canvas2 = new TRootEmbeddedCanvas("Canvas2",vframe2,200,200);
   vframe2->AddFrame(canvas2, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));

   TRootEmbeddedCanvas *canvas3 = new TRootEmbeddedCanvas("Canvas3",vframe2,200,200);
   vframe2->AddFrame(canvas3, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY,5,5,5,5));

   hframe1->AddFrame(vframe2, new TGLayoutHints(kLHintsCenterX | kLHintsExpandX | kLHintsCenterY | kLHintsExpandY, 5,5,20,20) );


   // 3rd

   TGTextButton *colors = new TGTextButton(hframe1,"&Colors","gApplication->Terminate(0)");
   hframe1->AddFrame(colors, new TGLayoutHints(kLHintsCenterY | kLHintsExpandY,100,100,5,5));
 
   // Add horizontal to main frame
 
   fMain->AddFrame(hframe1, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));

   // Set a name to the main frame
   fMain->SetWindowName("Event Display");

   // Map all subwindows of main frame
   fMain->MapSubwindows();

   // Initialize the layout algorithm
   fMain->Resize(fMain->GetDefaultSize());

   // Map main frame
   fMain->MapWindow();

   fMain->Resize(1500, 1000);
}
void MyMainFrame::DoDraw() {
   // Draws function graphics in randomly chosen interval
   TF1 *f1 = new TF1("f1","sin(x)/x",0,gRandom->Rndm()*10);
   f1->SetLineWidth(3);
   f1->Draw();
   TCanvas *fCanvas = fEcanvas->GetCanvas();
   fCanvas->cd();
   fCanvas->Update();
}
MyMainFrame::~MyMainFrame() {
   // Clean up used widgets: frames, buttons, layout hints
   fMain->Cleanup();
   delete fMain;
}
void MapixEventDisplay() {
   // Popup the GUI...
   new MyMainFrame(gClient->GetRoot(),1500,1000);
}
