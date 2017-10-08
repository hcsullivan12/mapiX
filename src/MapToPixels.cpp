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
		const std::string pixelHistoName = "Run " + std::to_string(event.runID) + " Subrun " + std::to_string(event.subrunID) + " Event " + std::to_string(event.eventID) + " Pixels";
		TH2S *pixelHisto = new TH2S(pixelHistoName.c_str(), pixelHistoName.c_str(), pixelCoordinates.getNPixels(), 0, pixelCoordinates.getNPixels(), pixelCoordinates.getNSamples(), 0, pixelCoordinates.getNSamples()); 
		pixelHisto->GetYaxis()->SetTitle("Time Samples");
		pixelHisto->GetXaxis()->SetTitle("Pixel Channel");
		
		/*///Loop through each channel, filling the 1D channel histos
		std::cout << "Filling channel histograms...\n";
		for (unsigned channel = 0; channel < pixelHisto->GetNbinsX(); channel++){
			
			///Build a hit histogram
			const std::string Name = "Event " + std::to_string(event.eventID) + " Pixel " + std::to_string(channel);
			TH1D *hitHisto = new TH1D(Name.c_str(), Name.c_str(), pixelHisto->GetNbinsY(), 0, pixelHisto->GetNbinsY());
			
			///Loop through the pixel hit order for this event
			for (auto it = event->pixelHitOrder.begin(); it != event->pixelHitOrder.end(); it++) {
				///Find all IDs matching this channel
				if (it->second == channel) {
					
					///Fill a hit histogram
					TRandom3 r;
					for (int i=0; i < 100000; i++) { 
						hitHisto->Fill(r.Gaus(it->first, pixelCoordinates.getWidthOfPixelPulse()));
					}
				}
			}
			//hitHisto->Write();
			///Set the content for all samples

			for (unsigned sample = 0; sample < pixelHisto->GetNbinsY(); sample++) {
				auto noise = noiseHisto->GetBinContent((sample + 1));
				auto noiseAndHit = noise + hitHisto->GetBinContent(sample + 1);
				//std::cout << noise << "   " << noiseAndHit << "\n";
				pixelHisto->SetBinContent((channel + 1), (sample + 1), noiseAndHit);
			}
			delete hitHisto;
		}*/
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
		const std::string roiHistoName = "Run " + std::to_string(event.runID) + " Subrun " + std::to_string(event.subrunID) + " Event " + std::to_string(event.eventID) + " ROI";
		TH2S *roiHisto = new TH2S(roiHistoName.c_str(), roiHistoName.c_str(), pixelCoordinates.getNROI(), 0, pixelCoordinates.getNROI(), pixelCoordinates.getNSamples(), 0, pixelCoordinates.getNSamples()); 
		roiHisto->GetYaxis()->SetTitle("Time Samples");
		roiHisto->GetXaxis()->SetTitle("ROI Channel");
		/*
		///Loop through each channel, filling the 1D channel histos
		std::cout << "Filling channel histograms...\n";
		for (unsigned channel = 0; channel < roiHisto->GetNbinsY(); channel++){
			
			///Build a hit histogram
			const std::string posName = "Event " + std::to_string(eventID) + " Positive ROI " + std::to_string(channel);
			const std::string negName = "Event " + std::to_string(eventID) + " Negative ROI " + std::to_string(channel);
			TH1D *positiveHitHisto = new TH1D(posName.c_str(), posName.c_str(), roiHisto->GetNbinsX(), 0, roiHisto->GetNbinsX());
			TH1D *negativeHitTemp = new TH1D("Temp", "Temp", roiHisto->GetNbinsX(), 0, roiHisto->GetNbinsX());
			TH1D *negativeHitHisto = new TH1D(negName.c_str(), negName.c_str(), roiHisto->GetNbinsX(), 0, roiHisto->GetNbinsX());
			
			///Loop through the ROI hit order for this event
			for (auto it = event->roiHitOrder.begin(); it != event->roiHitOrder.end(); it++) {
				///Find all IDs matching this channel
				if (it->second == channel) {
					
					///Fill a positive hit histogram
					TRandom3 r;
					for (int i=0; i < 100000; i++) { 
						positiveHitHisto->Fill(r.Gaus(it->first - pixelCoordinates.getHitDisc(), pixelCoordinates.getWidthOfROIPulse())); ////THISSSSS
					}
					
					//int maxPositiveSample = (positiveHitHisto->GetMaximumBin());
					//std::cout << it->first << "   " << maxPositiveSample << std::endl;
					TRandom3 w;
					for (int i=0; i < 100000; i++) { 
						negativeHitTemp->Fill(r.Gaus(it->first - pixelCoordinates.getHitDisc() + pixelCoordinates.getROIPeakDifference(), pixelCoordinates.getWidthOfROIPulse())); ////THISSSSS
					}
					for (unsigned sample = 0; sample < negativeHitTemp->GetNbinsX(); sample++) {
						auto content = -1*negativeHitTemp->GetBinContent((sample + 1));
						//if(content > 0) std::cout << "HEYYYYYY!!!!!! Channel " << channel << "      Content " << content << "\n";
						negativeHitHisto->SetBinContent((sample + 1), content);
					}
				}
			}
			//positiveHitHisto->Write();
			//negativeHitHisto->Write();
			///Set the content for all samples

			for (unsigned sample = 0; sample < roiHisto->GetNbinsX(); sample++) {
				auto noise = noiseHisto->GetBinContent((sample + 1));
				auto noiseAndHit = noise + positiveHitHisto->GetBinContent(sample + 1) + negativeHitHisto->GetBinContent(sample + 1);
				//std::cout << noise << "   " << noiseAndHit << "\n";
				roiHisto->SetBinContent((sample + 1), (channel + 1), noiseAndHit);
			}
			delete positiveHitHisto;
			delete negativeHitHisto;
			delete negativeHitTemp;
		}*/
		roiHisto->Write();
		delete roiHisto;
		//delete mcStyle;
	}
	
	histogramFile.Close();
};

void MapToPixels::find2DHits(const unsigned &t_runID,
 				 const unsigned &t_subrunID,
 			     const unsigned &t_eventID, 
 			     const std::vector<Hit> &t_rawHits,
			     std::vector<Hit2d> &t_pixelHits,
		         std::multimap<double, unsigned> &t_pixelHitOrder,
	             std::vector<Hit2d> &t_roiHits,
                 std::multimap<double, unsigned> &t_roiHitOrder,
			     std::multimap<double, Hit> &t_dataHitOrder) {
	
	/*///Get event hits from the data
	const auto &t_eventHits = inputData.getEvents().;*/
	/*///Take the given input event data hits and order according to x component 
	///These will now be ordered in time*/
	///Loop through the coordinates of this event and store results in this event's dataHitOrder multimap
	for (auto it : t_rawHits){
		//std::cout << "Converting xyz: " << it.x << " " << it.y << " " << it.z << " to time sample units and storing" << std::endl;
		//std::pair<double, double> yzCoor (it.y, it.z);
		///Convert x to time samples
		//double x_compInSamples = it->x;
		///Digitization speed is in ns, drift speed is in us
		double x_compInSamples = (1000*it.x)/(pixelCoordinates.getDigitizationSpeed()*pixelCoordinates.getDriftSpeed()); 
		t_dataHitOrder.insert( std::pair<double, Hit>(x_compInSamples, it) );
		//std::cout << x_compInSamples << " " << yzCoor.first << " " << yzCoor.second << std::endl;
			
	}
	
	/*for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
			std::cout << it->first << "  " << it->second.x << "  " << it->second.y << "  " << it->second.z << "  "  << it->second.colHitPeakTime << "  " << it->second.colHitWidth << "  " << it->second.colHitADC << "  " 
					<< it->second.indHitPeakTime << "  "  << it->second.indHitWidth << "  " << it->second.indHitADC << std::endl;
	}
	std::cout << std::endl;	*/

	///Convert coordinates relative to PCB origin (y, z)
	//std::cout << "With respect to the PCB Origin\n";
	/*for(auto iter = t_dataHitOrder.begin(); iter != t_dataHitOrder.end(); iter++){
		///Y component
		iter->second.first = iter->second.first - pixelCoordinates.getPCBOrigin().at(0);
		///Z component, positive axis is flipped
		iter->second.second = -1*(iter->second.second - pixelCoordinates.getPCBOrigin().at(1));
		//std::cout << iter->first << "  " << iter->second.first << "  " << iter->second.second << std::endl;
	}
	//std::cout << std::endl;
	
	///Convert YZ coordinates to units of pixel pitch
	///std::cout << "In units of pixel pitch\n";
	for(auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++) {
		convertYZToPixelUnits(it->second);
		//std::cout << it->second.first << "  " << it->second.second << std::endl;
	}
	
	///Convert YZ coordinates to ROI IDs
	convertYZToROIandPixelIDs(t_dataHitOrder, t_roiHits, t_roiHitOrder, t_pixelHits, t_pixelHitOrder);

	/*for(auto iter = t_dataHitOrder.begin(); iter != t_dataHitOrder.end(); iter++){
		std::cout << iter->first << "  " <<iter->second.first << "  " << iter->second.second << "       " << abs(fmod(iter->second.first, 8)) << "  " << abs(fmod(iter->second.second, 15)) << std::endl;
	}*/
	//std::cout << std::endl;*/

};

void MapToPixels::convertYZToPixelUnits(std::pair<double, double> &t_YZCoor) {
	
	///Divide by the pixel pitch (in cm)
	t_YZCoor.first = t_YZCoor.first/pixelCoordinates.getPixelPitch();
	t_YZCoor.second = t_YZCoor.second/pixelCoordinates.getPixelPitch();

	///Round to nearest pixel
	t_YZCoor.first = static_cast<int>(round(t_YZCoor.first));
	t_YZCoor.second = static_cast<int>(round(t_YZCoor.second));
	

};

void MapToPixels::convertYZToROIandPixelIDs(const std::multimap<double, std::pair<double, double>> &t_dataHitOrder, 
				    std::vector<Hit2d> &t_roiHits, std::multimap<double, unsigned> &t_roiHitOrder,
				    std::vector<Hit2d> &t_pixelHits, std::multimap<double, unsigned> &t_pixelHitOrder) {
	///Loop through all the hits
	///Check to make sure the pixels can see hit
	for (auto it = t_dataHitOrder.begin(); it != t_dataHitOrder.end(); it++){
		
		if(it->second.first > 0 || it->second.second < 0 || 
			it->second.first < -1*(pixelCoordinates.getPixelRegionHeight() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch() || 
			it->second.second > (pixelCoordinates.getPixelRegionWidth() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch()) {
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
				if((roiCoor.at(ID).at(0) >= it->second.first && it->second.first > (roiCoor.at(ID).at(0) - 8)) && 
				   (roiCoor.at(ID).at(1) <= it->second.second && it->second.second < (roiCoor.at(ID).at(1) + 15))){
					//std::cout << "IN IF STATEMENT!\n";
					roiID = ID;
					foundID = true;
				}
				
				ID++;
			}
			
			///ID is found, build a hit
			//std::cout << it->second.first << "  " << it->second.second << " is match to ROI #" << roiID << std::endl; 
			Hit2d roiHit;
			roiHit.channel = roiID;
			t_roiHits.push_back(roiHit);
			t_roiHitOrder.insert( std::pair<double, unsigned>(it->first, roiID) );
			
			Hit2d pixelHit;
			pixelHit.channel = 15*abs(fmod(it->second.first, 8)) + abs(fmod(it->second.second, 15));
			t_pixelHits.push_back(pixelHit);
			t_pixelHitOrder.insert( std::pair<double, unsigned>(it->first, pixelHit.channel) );
			//std::cout << it->first << "  " << it->second.first << "  " << it->second.second << "       " << abs(fmod(it->second.first, 8)) << "  " << abs(fmod(it->second.second, 15)) << std::endl;
			//std::cout << "Pixel channel is: " << pixelHit.channel << " \n";
			//std::cout << "ROI channel is: " << roiHit.channel << "\n\n";
		}
	}
};
