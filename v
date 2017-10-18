[1mdiff --git a/data/Histograms.root b/data/Histograms.root[m
[1mindex 9b7cc6d..10b4e9d 100644[m
Binary files a/data/Histograms.root and b/data/Histograms.root differ
[1mdiff --git a/data/NoiseData.root b/data/NoiseData.root[m
[1mindex d3bcfa3..8feee87 100644[m
Binary files a/data/NoiseData.root and b/data/NoiseData.root differ
[1mdiff --git a/scripts/roiPaletteColors.C b/scripts/roiPaletteColors.C[m
[1mindex 04ddc2a..c16e55f 100644[m
[1m--- a/scripts/roiPaletteColors.C[m
[1m+++ b/scripts/roiPaletteColors.C[m
[36m@@ -40,7 +40,7 @@[m [mvoid roiPaletteColors() {[m
 	gStyle->SetOptStat(0);[m
 	const Int_t NRGBs = 4;[m
 	const Int_t NCont = 100;[m
[31m-	Double_t stops[NRGBs] = { 0.00, 0.46, 0.60, 1.00 };[m
[32m+[m	[32mDouble_t stops[NRGBs] = { 0.00, 0.47, 0.60, 1.00 };[m
 	Double_t red[NRGBs]   = { 1.00, 0.00, 0.95, 1.00 };[m
 	Double_t green[NRGBs] = { 1.00, 0.00, 0.95, 0.00 };[m
 	Double_t blue[NRGBs]  = { 0.00, 0.60, 1.00, 0.00 };[m
[1mdiff --git a/src/MapToPixels.cpp b/src/MapToPixels.cpp[m
[1mindex aade7f6..3563bc0 100644[m
[1m--- a/src/MapToPixels.cpp[m
[1m+++ b/src/MapToPixels.cpp[m
[36m@@ -137,6 +137,7 @@[m [mvoid MapToPixels::map() {[m
 			TH1D *negativeHitHisto  = new TH1D(negName.c_str(), negName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());[m
 			TH1D *hitHisto          = new TH1D(hitName.c_str(), hitName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());[m
 			[m
[32m+[m[41m			[m
 			///Loop through the ROI hit order for this event[m
 			for (auto it = event.roiHitOrder.begin(); it != event.roiHitOrder.end(); it++) {[m
 				[m
