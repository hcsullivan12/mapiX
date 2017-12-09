#include <iostream>
#include <string> 
#include <fstream>
#include "EventData.h"
#include "HitFinder.h"
#include "PixelCoordinates.h"
#include "MapToPixels.h"
#include "TFile.h"
#include "TH1.h"
#include "Event.h"

int main(int argc, char** argv) {
	
	if (argc < 4) {
		std::cerr << "Usage: " << argv[0] << " mappingType(pointsToPixels or channelsToPixels)" << " inputFileName(points or channels) " << " pixelCoordinatesFileName " << std::endl;
		exit(1);
	}
	
	const std::string mappingType = std::string(argv[1]);
	const std::string inputFileName = std::string(argv[2]);
	const std::string pixelCoordinatesFileName = std::string(argv[3]);
	
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
		//inputData.ReadFile();
		
		///Map the coordinates to the pixels and ROI
		MapToPixels mapToPixels(inputData, pixelCoordinates, noiseHisto);
		//mapToPixels.map();
			
	} else {
		if (mappingType == "channelsToPixels") {
			std::cout << "In channelsToPixels!\n";
		
		} else {
			std::cout << "\nERROR! Please enter an appropriate mapping type: pointsToPixels or channelsToPixels\n\n";
			exit(1);
		}
	}
	
	/*
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
	*/
}
