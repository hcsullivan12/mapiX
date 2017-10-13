void paletteColors() {
	TFile *f = new TFile("../data/Histograms.root", "READ");
	
	TH2D *h1 = nullptr;
	f->GetObject("Run9102_Subrun54_Event4986_ROI", h1);
	if(h1 == NULL) {
		std::cout << "Couldn't find histogram.\n";
		exit(1);
	}
        
	TH2D *h2 = nullptr;
        f->GetObject("Run9102_Subrun54_Event4987_ROI", h2);
        if(h2 == NULL) {
                std::cout << "Couldn't find Event 4987.\n";
                exit(1);
        }

	TH2D *h3 = nullptr;
        f->GetObject("Run9102_Subrun54_Event4998_ROI", h3);
        if(h3 == NULL) {
                std::cout << "Couldn't find Event 4998.\n";
                exit(1);
        }

	TH2D *h4 = nullptr;
        f->GetObject("Run9102_Subrun54_Event5003_ROI", h4);
        if(h4 == NULL) {
                std::cout << "Couldn't find Event 5003.\n";
                exit(1);
        }

	TH2D *h5 = nullptr;
        f->GetObject("Run9102_Subrun303_Event25294_ROI", h5);
        if(h5 == NULL) {
                std::cout << "Couldn't find Event 25264.\n";
                exit(1);
        }


	gStyle->SetOptStat(0);
	const Int_t NRGBs = 4;
	const Int_t NCont = 100;
	Double_t stops[NRGBs] = { 0.00, 0.52, 0.60, 1.00 };
	Double_t red[NRGBs]   = { 1.00, 0.00, 0.95, 0.95 };
	Double_t green[NRGBs] = { 1.00, 0.00, 0.95, 0.95 };
	Double_t blue[NRGBs]  = { 0.00, 0.60, 1.00, 1.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);

	//gStyle->SetPalette(kRainBow);
	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	h1->Draw("colz");
	TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
	h2->Draw("colz");
	TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
	h3->Draw("colz");
	TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);
	h4->Draw("colz");
	TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
	h5->Draw("colz");



}
