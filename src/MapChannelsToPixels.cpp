#include "MapChannelsToPixels.h"
#include "math.h"
#include <fstream>
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom3.h"
#include <TStyle.h>


MapChannelsToPixels::MapChannelsToPixels(const ChannelData &t_inputData, const PixelCoordinates &t_pixelCoordinates, TH1D *t_noiseHisto, 
										const std::multimap< unsigned, std::pair<std::string, unsigned> > &t_lariatChannelToPCB, const std::string t_outputFile) : 
		inputData(t_inputData), pixelCoordinates(t_pixelCoordinates), noiseHisto(t_noiseHisto), lariatChannelToPCB(t_lariatChannelToPCB), outputFile(t_outputFile) {
		
};

void MapChannelsToPixels::map() {

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
	std::string outputFilePath = "../data/" + outputFile;
	TFile histogramFile(outputFilePath.c_str(), "RECREATE");
	
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
				double stdDev = it->second.hitWidth/(2*sqrt(2*log(2)));
				//std::cout << "Channel: " << it->second.hitChannelNumber << "    time: " << it->first << "   hitWidth: " << it->second.hitWidth << std::endl;
				
				///Find all IDs matching this channel
				if (it->second.hitChannelNumber == channel) {
					//std::cout << "Channel: " << it->second.hitChannelNumber << "    time: " << it->first << "   hitWidth: " << it->second.hitWidth;
					///Fill a hit histogram
					TRandom3 r;
					for (int i=0; i < 10000; i++) { 
						hitHisto->Fill(r.Gaus(mean, stdDev));
					}
					double scale = it->second.hitADC/hitHisto->GetBinContent(hitHisto->GetMaximumBin());
					for (auto sample = 0; sample < hitHisto->GetNbinsX(); sample++) {
						///Scale to proper ADC value
						double content = hitHisto->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise
						rescale = rescale*100;
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
			for (auto it = event.roiHits.begin(); it != event.roiHits.end(); it++) {
				
				///Mean shifted by 250 to account for pre and post sample andHitDisc to make ROI pulse before pixel
				double mean = it->hitPeakTime + 250 - pixelCoordinates.getHitDisc();
				double stdDev = 0.5*(it->hitADC/(2*sqrt(2*log(2))));
				
				///Find all IDs matching this channel
				if (it->hitChannelNumber == channel) {
					//std::cout << it->first << std::endl;
					///Fill a positive hit histogram
					TRandom3 r;
					for (int i=0; i < 10000; i++) { 
						positiveHitHisto->Fill(r.Gaus(mean, stdDev)); 
					}
					//positiveHitHisto->Write();
					double scale = it->hitADC/positiveHitHisto->GetBinContent(positiveHitHisto->GetMaximumBin());
					///Scale to proper ADC value
					for (auto sample = 0; sample < positiveHitHisto->GetNbinsX(); sample++) {
						double content = positiveHitHisto->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise data
						rescale = rescale*1000;
						if (mean <= 3000) positiveHitHisto->SetBinContent((sample + 1), rescale);
					}
					///Fill a negative hit histogram
					TRandom3 w;
					stdDev = 0.5*(it->hitWidth/(2*sqrt(2*log(2))));
					mean = mean + 3*stdDev;
					for (int i=0; i < 10000; i++) { 
						negativeHitTemp->Fill(w.Gaus(mean, stdDev)); 
					}
					//negativeHitTemp->Write();
					scale = it->hitWidth/negativeHitTemp->GetBinContent(negativeHitTemp->GetMaximumBin());
					///Scale to proper ADC value
					for (auto sample = 0; sample < negativeHitTemp->GetNbinsX(); sample++) {
						double content = negativeHitTemp->GetBinContent(sample + 1);
						double rescale = content*scale;
						///Scale to noise data
						rescale = rescale*1000;
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
	
	/*///BUILDING YZ PLOTS
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
	}*/
	
	//histogramFile.Close();
	
	//std::cout << std::endl;
	
	/*///WRITING CHANNEL HISTOGRAMS TO DATA FILE
	TFile *data = new TFile("../data/data.root", "RECREATE");
	for (const auto &event : events) {
		
		std::cout << "Writing channel histograms for Run " << event.runID << " Subrun " << event.subrunID << " Event " << event.eventID << std::endl;
		
		const std::string roiHistoName = "Run" + std::to_string(event.runID) + "_Subrun" + std::to_string(event.subrunID) + "_Event" + std::to_string(event.eventID) + "_Pixels";
		TH2D *pixelHisto = nullptr;
		
		histogramFile.GetObject(roiHistoName.c_str(), pixelHisto);
		
		if (pixelHisto == NULL) {
			std::cout << "Could not find histogram " << roiHistoName << std::endl;
		}
		
		for (unsigned channel = 0; channel < pixelHisto->GetNbinsX(); channel++) {
			
			TH1D *channelHisto = new TH1D("", "", pixelHisto->GetNbinsY(), 0, pixelHisto->GetNbinsY());
			
		}
		
	}*/
	
	
};

void MapChannelsToPixels::find2DHits(const unsigned &t_runID,
 				 const unsigned &t_subrunID,
 			     const unsigned &t_eventID, 
 			     const std::vector<Hit> &t_rawHits,
			     std::vector<Hit2d> &t_pixelHits,
		         std::multimap<double, Hit2d> &t_pixelHitOrder,
	             std::vector<Hit2d> &t_roiHits,
                 std::multimap<double, Hit2d> &t_roiHitOrder,
			     std::multimap<double, Hit> &t_dataHitOrder) {
					 
					 
	///Take channels from rawhits and convert to pixel hit or ROI hit with 
	///the help of the channel mapping
	for (auto it : t_rawHits) {
		///Get channel
		unsigned lariatChannel = it.hitChannelNumber;
		
		///Identify if this is a pixel or ROI from channel map
		std::string type = lariatChannelToPCB.find( lariatChannel )->second.first;
		unsigned ID =  lariatChannelToPCB.find( lariatChannel )->second.second;
		
		//std::cout << "Current channel: " << it.hitChannelNumber << "   " << type << ID << std::endl;
		
		if (type == "pixel") {
			//std::cout << "Found pixel\n";
			///Create pixel hit
			Hit2d pixelHit; 
			pixelHit.hitChannelNumber = ID;
			pixelHit.hitPeakTime = it.hitPeakTime;
			pixelHit.hitADC = it.hitADC;
			pixelHit.hitWidth = it.hitWidth;
			t_pixelHits.push_back(pixelHit);
			t_pixelHitOrder.insert( std::pair<double, Hit2d>(it.hitPeakTime, pixelHit) );
			//std::cout << "Pixel" << "      hitPeakTime: " << it.hitPeakTime << "   hitChannelNumber: " << it.hitChannelNumber << "    hitADC: " << it.hitADC << "   hitWidth: " << it.hitWidth << std::endl;  
		} else {
			if ("ROI") {
				//std::cout << "Found ROI\n";
				///Create ROI hit
				Hit2d roiHit;
				roiHit.hitChannelNumber = ID;
				roiHit.hitPeakTime = it.hitPeakTime;
				roiHit.hitADC = it.hitADC;
				roiHit.hitWidth = it.hitWidth;
				t_roiHits.push_back(roiHit);
				t_roiHitOrder.insert( std::pair<double, Hit2d>(it.hitPeakTime, roiHit) );
				//std::cout << "ROI " << ID << "      hitPeakTime: " << it.hitPeakTime << "    hitADC: " << it.hitADC << "   hitWidth: " << it.hitWidth << std::endl;  
				
			} else {
				std::cout << "ERROR! Could not identify pixel or ROI.\n\n";
				exit(1);
			}
		}
	}
	
	for (auto it = t_roiHitOrder.begin(); it != t_roiHitOrder.end(); it++) {
		std::cout << "ROI" << it->second.hitChannelNumber << "    PeakTime: " << it->first <<  std::endl;  
	}

};

