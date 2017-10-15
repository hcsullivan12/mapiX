void pixelPaletteColors() {
	TFile *f = new TFile("../data/Histograms.root", "r");
	
	TH2D *h1 = nullptr; 
	f->GetObject("Run9102_Subrun54_Event4986_Pixels", h1);
	if(h1 == NULL) {
		std::cout << "Could not find histogram!\n";
		exit(1);
	}

        TH2D *h2 = nullptr; 
        f->GetObject("Run9102_Subrun54_Event4987_Pixels", h2);
        if(h2 == NULL) {
                std::cout << "Could not find histogram!\n";
                exit(1);
        }

        TH2D *h3 = nullptr; 
        f->GetObject("Run9102_Subrun54_Event4998_Pixels", h3);
        if(h3 == NULL) {
                std::cout << "Could not find histogram!\n";
                exit(1);
        }

        TH2D *h4 = nullptr; 
        f->GetObject("Run9102_Subrun54_Event5003_Pixels", h4);
        if(h4 == NULL) {
                std::cout << "Could not find histogram!\n";
                exit(1);
        }

        TH2D *h5 = nullptr; 
        f->GetObject("Run9102_Subrun303_Event25294_Pixels", h5);
        if(h5 == NULL) {
                std::cout << "Could not find histogram!\n";
                exit(1);
        }

	gStyle->SetOptStat(0);
	const Int_t NRGBs = 4;
	const Int_t NCont = 100;
	Double_t stops[NRGBs] = { 0.00, 0.30, 0.50, 1.00 };
	Double_t red[NRGBs]   = { 0.00, 0.90, 1.00, 1.00 };
	Double_t green[NRGBs] = { 0.00, 0.90, 1.00, 0.00 };
	Double_t blue[NRGBs]  = { 0.60, 1.00, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);

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
