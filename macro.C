void macro() {
	TFile *f = new TFile("/home/hunter/projects/mapiX/data/Histograms.root", "READ");
	TH2S *h = nullptr;
	f->GetObject("Event 7465 ROI", h);
	
	TStyle* mcStyle = new TStyle("mcStyle","Manuel's Root Styles"); 
	const Int_t NRGBs = 4;
 	const Int_t NCont = 20;
 	Double_t stops[NRGBs] = { 0.00, 0.30, 0.61, 0.84 };
 	Double_t red[NRGBs] = { 0.00, 0.00, 0.57, 0.90 };
 	Double_t green[NRGBs] = { 0.00, 0.65, 0.95, 0.20 };
 	Double_t blue[NRGBs] = { 0.00, 0.15, 0.51, 0.55 };
 	TColor::CreateGradientColorTable(NRGBs, stops, red, blue, green, NCont);
	gStyle->SetNumberContours(NCont);

	h->Draw("colz");


}
