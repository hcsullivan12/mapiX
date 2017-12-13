#include <iostream>
#include <string> 
#include <fstream>
#include "EventData.h"
#include "HitFinder.h"
#include "PixelCoordinates.h"
#include "MapCoordinatesToPixels.h"
#include "MapChannelsToPixels.h"
#include "TFile.h"
#include "TH1.h"
#include "Event.h"
#include "ChannelData.h"
#include <sstream>

void ReadChannelMapping(std::multimap< unsigned, std::pair<std::string, unsigned> > &lariatChannelToPCB);

int main(int argc, char** argv) {
	
	if (argc < 5) {
		std::cerr << "Usage: " << argv[0] << " mappingType(pointsToPixels or channelsToPixels)" << " inputFilename(points or channels) " << " pixelCoordinatesFilename " << " rootOutputFilename " << std::endl;
		exit(1);
	}
	
	const std::string mappingType = std::string(argv[1]);
	const std::string inputFileName = std::string(argv[2]);
	const std::string pixelCoordinatesFileName = std::string(argv[3]);
	const std::string outputFile = std::string(argv[4]);
	
	///Get the the pixel coordinates and parameters
	PixelCoordinates pixelCoordinates(pixelCoordinatesFileName);
	
	///Get noise data from pixy histograms
	TFile noiseDataRootFile("/home/hunter/projects/pixy/data/FilteredHistograms.root", "READ");
	if (!noiseDataRootFile.IsOpen()) {
		std::cerr << "ERROR: Failed to open noise data file!" << std::endl;
		exit(1);
	}
	TH1D* noiseHisto = nullptr;
	noiseDataRootFile.GetObject("Event218_PixelChannel0", noiseHisto);
	if (!noiseHisto) {
		std::cerr << "ERROR: Failed to find histo!" << std::endl;
		exit(1);
	}
	TFile noiseData("/home/hunter/projects/mapiX/data/NoiseData.root", "RECREATE");
	noiseHisto->Write();
	noiseData.Close();
	
	if (mappingType == "pointsToPixels") {
		///Read in the spatial coordinates 
		EventData inputData(inputFileName);
		inputData.ReadFile();
		
		auto events = inputData.getEvents();
		std::cout << "Received data from ...\n\n";
		for (auto it = events.begin(); it != events.end(); it++) {
			std::cout << "Run: " << it->runID << std::endl;
			std::cout << "Subrun: " << it->subrunID << std::endl;
			std::cout << "Event: " << it->eventID << std::endl;
			std::cout << std::endl;
		}
		
		///Map the coordinates to the pixels and ROI
		MapCoordinatesToPixels mapToPixels(inputData, pixelCoordinates, noiseHisto, outputFile);
		mapToPixels.map();
			
	} else {
		if (mappingType == "channelsToPixels") {
			///Read in the channel hit information 
			ChannelData inputData(inputFileName);
			inputData.ReadFile();
			
			///Read in channel mapping
			///Will help matching raw data to pixels and ROI
			std::multimap< unsigned, std::pair<std::string, unsigned> > lariatChannelToPCB; // ( lariatChannel, (type, ID) )  type: pixel or ROI
			ReadChannelMapping(lariatChannelToPCB);
			
			/*for (auto it = lariatChannelToPCB.begin(); it != lariatChannelToPCB.end(); it++) {
				std::cout << "Channel " << it->first << "   " << it->second.first << it->second.second << std::endl;
			}*/
			
			auto events = inputData.getEvents();
			std::cout << "\nReceived data from ...\n\n";
			for (auto it = events.begin(); it != events.end(); it++) {
				std::cout << "Run: " << it->runID << std::endl;
				std::cout << "Subrun: " << it->subrunID << std::endl;
				std::cout << "Event: " << it->eventID << std::endl;
				std::cout << std::endl;
			}
			
			///Map the channels to the pixels and ROI
			MapChannelsToPixels mapToPixels(inputData, pixelCoordinates, noiseHisto, lariatChannelToPCB, outputFile);
			mapToPixels.map();
		
		} else {
			std::cout << "\nERROR! Please enter an appropriate mapping type: pointsToPixels or channelsToPixels\n\n";
			exit(1);
		}
	}
	
	
	std::cout << std::endl;
	std::cout << "PCB side width: " << pixelCoordinates.getPCBSideWidth() << std::endl;
	std::cout << "Pixel region width: " << pixelCoordinates.getPixelRegionWidth() << std::endl;
	std::cout << "Pixel region height: " << pixelCoordinates.getPixelRegionHeight() << std::endl; 
	std::cout << "PCB origin is: " << pixelCoordinates.getPCBOrigin().at(0) << "  " << pixelCoordinates.getPCBOrigin().at(1) << std::endl;
	std::cout << "Drift speed: " << pixelCoordinates.getDriftSpeed() << std::endl;
	std::cout << "Digitization speed: " << pixelCoordinates.getDigitizationSpeed() << std::endl;
	std::cout << "Number of samples: " << pixelCoordinates.getNSamples() << std::endl;
	std::cout << "Y limit: " << -1*(pixelCoordinates.getPixelRegionHeight() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch() << std::endl;
	std::cout << "Z limit: " << (pixelCoordinates.getPixelRegionWidth() - pixelCoordinates.getPixelPitch()/2)/pixelCoordinates.getPixelPitch() <<  std::endl;
	
}

void ReadChannelMapping(std::multimap< unsigned, std::pair<std::string, unsigned> > &lariatChannelToPCB) {
	
	///Read in channel mapping file
	
	std::string filename = "../config/PixLArChannelMapping.txt";
	std::ifstream mappingFile(filename.c_str());
	if (!mappingFile) {
		throw std::runtime_error("PixLArReco::reconfigure ERROR! Cannot open channel mapping file.");
	} else {
	
		std::string line;
		std::string s1;
		std::string s2;

		/*std::getline(linestream, s1, ' ');
		std::getline(linestream, s2);*/
		
		///Define side of pcb
		std::string pcb;
		
		while (std::getline(mappingFile, line)) {
			
			std::stringstream linestream(line);
			std::getline(linestream, s1, ' ');
			std::getline(linestream, s2);
			//std::cout << s1 << "  " << s2 << std::endl;
			
			if (s2 == "Channel") {
				
				// Mark which pcb we're mapping
				pcb = s1;;
				
				if (s1 == "SideB") {
					pcb = "upstream";
				} else {
					if (s1 == "SideA") {
						pcb = "downstream";
					} else { 
						throw std::runtime_error("PixLArReco::reconfigure ERROR! Cannot identify PCB.");
					}
				}
			} else {
				
				///Data for side "pcb"
				/*std::getline(mappingFile, line);
				std::stringstream linestream(line);
				std::getline(linestream, s1, ' ');
				std::getline(linestream, s2);
				//std::cout << s1 << "  " << s2 << std::endl;*/
					
				// Found lariat channel
				unsigned lariatChannel = std::atoi(s2.c_str());
				///Identify if this is a pixel or ROI
				if ( s1.at(0) == 'P') {
					// This is a pixel
					std::string type = "pixel";
					unsigned pixelID;
				
					// Delete the P-R identifier
					s1.erase(0, 1);
				
					if (pcb == "downstream") {
						// Origin of PCB coordinate is downstream, top left pixel
						pixelID = std::atoi(s1.c_str()) - 1; ///Convert to 0 - 239
					} else {
						// Have to add 120 to pixelID
						pixelID = 120 + std::atoi(s1.c_str()) - 1; ///Convert to 0 - 239
					}
				
					// Store info in ChannelToPCB Map             
					std::pair< std::string, unsigned > p;
					p = std::make_pair( type, pixelID );
					lariatChannelToPCB.insert( std::pair< unsigned, std::pair< std::string, unsigned> >( lariatChannel, p ) );
				} else {
					if ( s1.at(0) == 'R') {
						// This is a ROI
						std::string type = "ROI";
						
						// Delete the P-R identifier
						s1.erase(0, 1);
						
						// Store info in ChannelToPCB Map
						std::pair< std::string, unsigned > p;
						unsigned roiID = std::atoi(s1.c_str()) - 1; ///Convert to 0 - 239
						p = std::make_pair( type, roiID );
						lariatChannelToPCB.insert( std::pair< unsigned, std::pair< std::string, unsigned> >( lariatChannel, p ) );
					} else {
						std::cout << s1 << std::endl;
						throw std::runtime_error("PixLArReco::reconfigure ERROR! Cannot identify pixel or ROI.");
					}
				}
			}
		}
	}
}
