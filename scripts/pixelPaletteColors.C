void pixelPaletteColors() {
	TFile *f = new TFile("../data/Histograms.root", "r");
	TH2D *h = nullptr; 
	f->GetObject("Run9102_Subrun54_Event4986_Pixels", h);
	if(h == NULL) {
		std::cout << "Could not find histogram!\n";
		exit(1);
	}
	gStyle->SetOptStat(0);
	const Int_t NRGBs = 4;
	const Int_t NCont = 100;
	Double_t stops[NRGBs] = { 0.00, 0.02, 0.30, 1.00 };
	Double_t red[NRGBs]   = { 0.00, 0.90, 1.00, 1.00 };
	Double_t green[NRGBs] = { 0.00, 0.90, 1.00, 0.00 };
	Double_t blue[NRGBs]  = { 0.60, 1.00, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);

	h->Draw("colz");


}
