#include "MapToPixels.h"
#include "math.h"
#include <fstream>
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom3.h"
#include <TStyle.h>


MapToPixels::MapToPixels(const EventData &t_inputData, const PixelCoordinates &t_pixelCoordinates, TH1D *t_noiseHisto) : 
		inputData(t_inputData), pixelCoordinates(t_pixelCoordinates), noiseHisto(t_noiseHisto) {
		
};

void MapToPixels::map() {

	events.clear();
	events.resize(inputData.getEvents().size());

	auto event = events.begin();
	
	//Loop over events
	for(const auto &data : inputData.getEvents()){
		std::cout << "Processing Run " << data.runID << " Subrun " << data.subrunID << 
				" Event " << data.eventID << std::endl;
		std::cout << "Running 2D hit finder...\n";
		event->runID = data.runID;
		event->subrunID = data.subrunID;
		event->eventID = data.eventID;
		event->rawHits = data.rawHits;
		
		find2DHits(event->runID,
			   event->subrunID,
			   event->eventID,
			   event->rawHits,
			   event->pixelHits,
			   event->pixelHitOrder,
			   event->roiHits,
			   event->roiHitOrder,
			   event->dataHitOrder);
		
		event++;
		
	}
	
	std::cout << std::endl;
	
	///BUILDING PIXEL HISTOGRAMS
	TFile histogramFile("../data/Histograms.root", "RECREATE");
	///Loop through events
	for (const auto &event : events) {
		
		///Create 2D histogram for this event
		std::cout << "Creating pixel histogram for Run " << event.runID << " Subrun " << event.subrunID << 
                  " Event " << event.eventID << std::endl;
		const std::string pixelHistoName = "Run" + std::to_string(event.runID) + "_Subrun" + std::to_string(event.subrunID) + "_Event" + std::to_string(event.eventID) + "_Pixels";
		TH2D *pixelHisto = new TH2D(pixelHistoName.c_str(), pixelHistoName.c_str(), pixelCoordinates.getNPixels(), 0, pixelCoordinates.getNPixels(), pixelCoordinates.getNSamples() + 1000, 0, pixelCoordinates.getNSamples() + 1000); 
		pixelHisto->GetYaxis()->SetTitle("Time Samples");
		pixelHisto->GetXaxis()->SetTitle("Pixel Channel");
		
		///Loop through each channel, filling the 1D channel histos
		std::cout << "Filling channel histograms...\n";
		for (unsigned channel = 0; channel < pixelHisto->GetNbinsX(); channel++){
			
			///Build a hit histogram
			const std::string Name = "Event " + std::to_string(event.eventID) + " Pixel " + std::to_string(channel);
			TH1D *hitHisto = new TH1D(Name.c_str(), Name.c_str(), pixelHisto->GetNbinsY(), 0, pixelHisto->GetNbinsY());
	
			///Loop through the pixel hit order for this event
			for (auto it = event.pixelHitOrder.begin(); it != event.pixelHitOrder.end(); it++) {
				
				///Mean shifted by 250 to account for pre and post sample
				double mean = it->first + 250;
				double stdDev = it->second.colHitWidth/(2*sqrt(2*log(2)));
				
				///Find all IDs matching this channel
				if (it->second.channel == channel) {
					///Fill a hit histogram
					TRandom3 r;
					for (int i=0; i < 10000; i++) { 
						hitHisto->Fill(r.Gaus(mean, stdDev));
					}
					double scale = it->second.colHitADC/hitHisto->GetBinContent(hitHisto->GetMaximumBin());
					for (auto sample = 0; sample < hitHisto->GetNbinsX(); sample++) {
						///Scale to proper ADC value
						double content = hitHisto->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise
						rescale = rescale*50;
						if (mean <= 3000) hitHisto->SetBinContent((sample + 1), rescale);
					}
				}
			}
			
			///Set the content for all samples
			for (unsigned sample = 0; sample < pixelHisto->GetNbinsY(); sample++) {
				double noise = noiseHisto->GetBinContent((sample + 1));
				if(sample > 2000) noise = noiseHisto->GetBinContent((sample - 1000));
				double hit = hitHisto->GetBinContent(sample + 1);
				double noiseAndHit = noise + hit;
				hitHisto->SetBinContent(sample + 1, noiseAndHit);
				pixelHisto->SetBinContent((channel + 1), (sample + 1), noiseAndHit);
				if (pixelHisto->GetBinContent((channel + 1), (sample + 1)) != noiseAndHit) std::cout << "Pixel bin content error!!\n";
			}
			delete hitHisto;
		}
		pixelHisto->Write();
		delete pixelHisto;
	}
	std::cout << std::endl;
	
	///BUILDING ROI HISTOGRAMS
	///Loop through events);
	for (const auto &event : events) {
		
		///Create 2D histogram for this event
		std::cout << "Creating ROI histogram for Run " << event.runID << " Subrun " << event.subrunID << 
                  " Event " << event.eventID << std::endl;
		const std::string roiHistoName = "Run" + std::to_string(event.runID) + "_Subrun" + std::to_string(event.subrunID) + "_Event" + std::to_string(event.eventID) + "_ROI";
		TH2D *roiHisto = new TH2D(roiHistoName.c_str(), roiHistoName.c_str(), pixelCoordinates.getNROI(), 0, pixelCoordinates.getNROI(), pixelCoordinates.getNSamples() + 1000, 0, pixelCoordinates.getNSamples() + 1000); 
		roiHisto->GetYaxis()->SetTitle("Time Samples");
		roiHisto->GetXaxis()->SetTitle("ROI Channel");
		
		///Loop through each channel, filling the 1D channel histos
		std::cout << "Filling channel histograms...\n";
		for (unsigned channel = 0; channel < roiHisto->GetNbinsX(); channel++){
			
			///Build a hit histogram
			const std::string posName     = "Event " + std::to_string(event.eventID) + " Positive ROI " + std::to_string(channel);
			const std::string negName     = "Event " + std::to_string(event.eventID) + " Negative ROI " + std::to_string(channel);
			const std::string negTempName = "Event " + std::to_string(event.eventID) + " Negative ROI temp " + std::to_string(channel);
			const std::string hitName     = "Event " + std::to_string(event.eventID) + " ROI hit Channel " + std::to_string(channel);
			TH1D *positiveHitHisto  = new TH1D(posName.c_str(), posName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());
			TH1D *negativeHitTemp   = new TH1D(negTempName.c_str(), negTempName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());
			TH1D *negativeHitHisto  = new TH1D(negName.c_str(), negName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());
			TH1D *hitHisto          = new TH1D(hitName.c_str(), hitName.c_str(), roiHisto->GetNbinsY(), 0, roiHisto->GetNbinsY());
			
			///Loop through the ROI hit order for this event
			for (auto it = event.roiHitOrder.begin(); it != event.roiHitOrder.end(); it++) {
				
				///Mean shifted by 250 to account for pre and post sample andHitDisc to make ROI pulse before pixel
				double mean = it->first + 250 - pixelCoordinates.getHitDisc();
				double stdDev = 0.5*(it->second.indHitWidth/(2*sqrt(2*log(2))));
				
				///Find all IDs matching this channel
				if (it->second.channel == channel) {
					//std::cout << it->first << std::endl;
					///Fill a positive hit histogram
					TRandom3 r;
					for (int i=0; i < 10000; i++) { 
						positiveHitHisto->Fill(r.Gaus(mean, stdDev)); 
					}
					//positiveHitHisto->Write();
					double scale = it->second.indHitADC/positiveHitHisto->GetBinContent(positiveHitHisto->GetMaximumBin());
					///Scale to proper ADC value
					for (auto sample = 0; sample < positiveHitHisto->GetNbinsX(); sample++) {
						double content = positiveHitHisto->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise data
						rescale = rescale*50;
						if (mean <= 3000) positiveHitHisto->SetBinContent((sample + 1), rescale);
					}
					///Fill a negative hit histogram
					TRandom3 w;
					stdDev = 0.5*(it->second.indHitWidth/(2*sqrt(2*log(2))));
					mean = mean + 3*stdDev;
					for (int i=0; i < 10000; i++) { 
						negativeHitTemp->Fill(w.Gaus(mean, stdDev)); 
					}
					//negativeHitTemp->Write();
					scale = it->second.indHitADC/negativeHitTemp->GetBinContent(negativeHitTemp->GetMaximumBin());
					///Scale to proper ADC value
					for (auto sample = 0; sample < negativeHitTemp->GetNbinsX(); sample++) {
						double content = negativeHitTemp->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise data
						rescale = rescale*50;
						if (mean <= 3000) negativeHitTemp->SetBinContent((sample + 1), rescale);
					}
					//negativeHitTemp->Write();
					for (unsigned sample = 0; sample < negativeHitTemp->GetNbinsX(); sample++) {
						double content = -1*negativeHitTemp->GetBinContent((sample + 1));
						//if(content > 0) std::cout << "HEYYYYYY!!!!!! Channel " << channel << "      Content " << content << "\n";
						negativeHitHisto->SetBinContent((sample + 1), content);
					}
				}
			}
			
			///Set the content for all samples
			for (unsigned sample = 0; sample < roiHisto->GetNbinsY(); sample++) {
				double noise = noiseHisto->GetBinContent((sample + 1));
				double posHit = positiveHitHisto->GetBinContent(sample + 1);
				double negHit = negativeHitHisto->GetBinContent(sample + 1);
				if(sample > 2000) noise = noiseHisto->GetBinContent((sample - 1000));
				//if (posHit != 0) std::cout << posHit << std::endl;
				double noiseAndHit = noise + posHit + negHit;
				hitHisto->SetBinContent((sample + 1), (posHit + negHit + noise));
				roiHisto->SetBinContent((channel + 1), (sample + 1), noiseAndHit);
				if (roiHisto->GetBinContent((channel + 1), (sample + 1)) != noiseAndHit) std::cout << "ROI bin content error!!\n";
			}
			//positiveHitHisto->Write();
			delete positiveHitHisto;
			//negativeHitHisto->Write();
			delete negativeHitHisto;
			//hitHisto->Write();
			delete hitHisto;
			delete negativeHitTemp;
		}
		roiHisto->Write();
		delete roiHisto;
	}
	std::cout << std::endl;
	
	///BUILDING YZ PLOTS
	///Loop through events
	for (const auto &event : inputData.getEvents()) {
		
		///Create 2D histogram for this event
		std::cout << "Creating YZ histogram for Run " << event.runID << " Subrun " << event.subrunID << 
                  " Event " << event.eventID << std::endl;
		const std::string yzHistoName = "Run " + std::to_string(event.runID) + " Subrun " + std::to_string(event.subrunID) + " Event " + std::to_string(event.eventID);
		TH2D *yzHisto = new TH2D(yzHistoName.c_str(), yzHistoName.c_str(), 90, 0, 90, 40, -20, 20); 
		yzHisto->GetYaxis()->SetTitle("Y (cm)");
		yzHisto->GetXaxis()->SetTitle("Z (cm)");
		
		for (auto it = event.rawHits.begin(); it != event.rawHits.end(); it++) {
			//std::cout << it->second.z << "  " << it->second.y << std::endl;
			//auto noise = noiseHisto->GetBinContent((sample + 1));
			//auto noiseAndHit = noise + 100*it->second.colHitADC;
			for (int i = 0; i < 100; i++) {
			
				if (pcbCanSee(it->z, it->y, true)) {
					yzHisto->SetMarkerStyle(kBlack);
					yzHisto->Fill(it->z, it->y);
				}
				else {
					yzHisto->SetMarkerStyle(kRed);
					yzHisto->Fill(it->z,it->y);
					
				}
			}
			//sample++;
		}
		
		yzHisto->Write();
		delete yzHisto;
	}
	
	histogramFile.Close();
};

void MapToPixels::find2DHits(const unsigned &t_runID,
 				 const unsigned &t_subrunID,
 			     const unsigned &t_eventID, 
 			     const std::vector<Hit> &t_rawHits,
			     std::vector<Hit2d> &t_pixelHits,
		         std::multimap<double, Hit2d> &t_pixelHitOrder,
	             std::vector<Hit2d> &t_roiHits,
                 std::multimap<double, Hit2d> &t_roiHitOrder,
			     std::multimap<double, Hit> &t_dataHitOrder) {
					 
	///Take the given input event data hits and order according to x component 
	///These will now be ordered in time
	///Loop through the coordinates of this event and store results in this event's dataHitOrder multimap
	for (auto it : t_rawHits){
		///Convert x to time samples
		///Digitization speed is in ns, drift speed is in us
		double x_compInSamples = (1000*it.x)/(pixelCoordinates.getDigitizationSpeed()*pixelCoordinates.getDriftSpeed()); 
		t_dataHitOrder.insert( std::pair<double, Hit>(x_compInSamples, it) );
		//indWidth = indWidth + it.indHitWidth;
		//colWidth = colWidth + it.colHitWidth;
		//n++;
		
	}
	//std::cout << "AVERAGE INDUCTION PULSE WIDTH: " << indWidth/n << std::endl;
	//std::cout << "AVERAGE COLLECTION PULSE WIDTH: " << colWidth/n << std::endl;
	/*for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
			std::cout << it->first << "  " << it->second.x << "  " << it->second.z << "  " << it->second.y << "  "  << it->second.colHitPeakTime << "  " << it->second.colHitWidth << "  " << it->second.colHitADC << "  " 
					<< it->second.indHitPeakTime << "  "  << it->second.indHitWidth << "  " << it->second.indHitADC << std::endl;
	}
	std::cout << std::endl;*/
	
	//std::cout << "PCB Origin: " << "Z ---> " << pixelCoordinates.getPCBOrigin().at(0) << "  Y ----> " << pixelCoordinates.getPCBOrigin().at(1) << std::endl;
	
	///Convert coordinates relative to PCB origin (y, z)
	for(auto iter = t_dataHitOrder.begin(); iter != t_dataHitOrder.end(); iter++){
		convertToPCBCoordinates(iter->second.z, iter->second.y);
	}
	/*for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
		if(it->first > 2800 || it->first < 0 && t_eventID == 4987) { std::cout << it->first << "  " << it->second.x << "  " << it->second.y << "  " << it->second.z << "  "  << it->second.colHitPeakTime << "  " << it->second.colHitWidth << "  " << it->second.colHitADC << "  " 
			<< it->second.indHitPeakTime << "  "  << it->second.indHitWidth << "  " << it->second.indHitADC << std::endl;}
	}
	std::cout << std::endl;	*/
	
	///Convert YZ coordinates to units of pixel pitch
	///std::cout << "In units of pixel pitch\n";
	for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++) {
		convertYZToPixelUnits(it->second.z, it->second.y);
	}
	/*for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
		if (it->first < 0 && t_eventID == 4987) {std::cout << it->first << "  " << it->second.x << "  " << it->second.y << "  " << it->second.z << "  "  << it->second.colHitPeakTime << "  " << it->second.colHitWidth << "  " << it->second.colHitADC << "  " 
				<< it->second.indHitPeakTime << "  "  << it->second.indHitWidth << "  " << it->second.indHitADC << std::endl;}
	}
	std::cout << std::endl;*/	
	
	///Convert YZ coordinates to ROI IDs
	convertYZToROIandPixelIDs(t_dataHitOrder, t_roiHits, t_roiHitOrder, t_pixelHits, t_pixelHitOrder);

	/*for(auto iter = t_dataHitOrder.begin(); iter != t_dataHitOrder.end(); iter++){
		std::cout << iter->first << "  " <<iter->second.first << "  " << iter->second.second << "       " << abs(fmod(iter->second.first, 8)) << "  " << abs(fmod(iter->second.second, 15)) << std::endl;
	}
	std::cout << std::endl;*/

};

void MapToPixels::convertYZToPixelUnits(double &z, double &y) {
	
	///Divide by the pixel pitch (in cm)
	y = y/pixelCoordinates.getPixelPitch();
	z = z/pixelCoordinates.getPixelPitch();

	///Round to nearest pixel
	y = static_cast<int>(round(y));
	z = static_cast<int>(round(z));
};

void MapToPixels::convertYZToROIandPixelIDs(const std::multimap<double, Hit> &t_dataHitOrder, 
				    std::vector<Hit2d> &t_roiHits, std::multimap<double, Hit2d> &t_roiHitOrder,
				    std::vector<Hit2d> &t_pixelHits, std::multimap<double, Hit2d> &t_pixelHitOrder) {
	///Loop through all the hits
	///Check to make sure the pixels can see hit
	for (auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
		
		if(!pcbCanSee(it->second.z, it->second.y, false)) {
			//std::cout << "Skipping: " << it->second.first << "  " << it->second.second << std::endl;
			continue;
		}
		else {
			
			///Loop through the ROI Coordinates
			unsigned ID = 0;
			unsigned roiID = 0;
			bool foundID = false;
			
			auto roiCoor = pixelCoordinates.getROICoor();
			
			while(!foundID) {
				///Entry: First is Y, Second is Z        roiCoor: 0 is Y, 1 is Z
				///Y condition                                    Z condition
				//std::cout << "IN WHILE LOOP " << roiCoor.at(ID).at(0) << " >= " << it->second.first << " > " << roiCoor.at(ID).at(0) - 8 <<  "\n";
				if((roiCoor.at(ID).at(0) >= it->second.y && it->second.y > (roiCoor.at(ID).at(0) - 8)) && 
				   (roiCoor.at(ID).at(1) <= it->second.z && it->second.z < (roiCoor.at(ID).at(1) + 15))){
					//std::cout << "IN IF STATEMENT!\n";
					roiID = ID;
					foundID = true;
				}
				
				ID++;
			}
			
			///ID is found, build a hit 
			Hit2d roiHit;
			roiHit.channel = roiID;
			roiHit.indHitPeakTime = it->second.indHitPeakTime;
			roiHit.indHitWidth = it->second.indHitWidth;
			roiHit.indHitADC = it->second.indHitADC;
			t_roiHits.push_back(roiHit);
			t_roiHitOrder.insert( std::pair<double, Hit2d>(it->first, roiHit) );
			
			Hit2d pixelHit;
			pixelHit.channel = 15*abs(fmod(it->second.y, 8)) + abs(fmod(it->second.z, 15));
			if (it->second.z >= 120) pixelHit.channel = pixelHit.channel + 120; 
			pixelHit.colHitPeakTime = it->second.colHitPeakTime;
			pixelHit.colHitWidth = it->second.colHitWidth;
			pixelHit.colHitADC = it->second.colHitADC;
			t_pixelHits.push_back(pixelHit);
			t_pixelHitOrder.insert( std::pair<double, Hit2d>(it->first, pixelHit) );
			//std::cout << "Time " << it->first << "    Y: " << it->second.y << "     Z: " << it->second.z << std::endl;
			//std::cout << "Is matched to ROI #" << roiID  << "    Pixel #" << pixelHit.channel << std::endl;
			//std::cout << it->first << "  " << it->second.y << "  " << it->second.z << std::endl;//"       " << abs(fmod(it->second.y, 8)) << "  " << abs(fmod(it->second.z, 15)) << std::endl;
			//std::cout << "Pixel channel is: " << pixelHit.channel << " \n";
			//std::cout << "ROI channel is: " << roiHit.channel << "\n\n";
		}
	}
};

void MapToPixels::convertToPCBCoordinates(double &z, double &y) {
	///Z component
	z = z - pixelCoordinates.getPCBOrigin().at(0);
	///Y component
	y = y - pixelCoordinates.getPCBOrigin().at(1);
}

bool MapToPixels::pcbCanSee(const double &z, const double &y, bool tpcCoor) {
	
	double zPrime = z;
	double yPrime = y;
	
	if (tpcCoor) {
		convertToPCBCoordinates(zPrime, yPrime);
	}
	if (yPrime > 0 || zPrime < 0 || 
		yPrime < -1*(pixelCoordinates.getPixelRegionHeight() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch() || 
		zPrime > (pixelCoordinates.getPixelRegionWidth() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch()) {
	
		return false;
	}
	return true;
	
}
