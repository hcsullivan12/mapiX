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
	
	if (argc < 3) {
		std::cerr << "Usage: " << argv[0] << " trackFileName " << " pixelCoordinatesFileName " << std::endl;
		exit(1);
	}

	const std::string trackFileName = std::string(argv[1]);
	const std::string pixelCoordinatesFileName = std::string(argv[2]);
	
	///Read in the spatial coordinates 
	EventData inputData(trackFileName);
	inputData.ReadFile();
	
	///Get the the pixel coordinates and parameters
	PixelCoordinates pixelCoordinates(pixelCoordinatesFileName);
	
	///Get noise data from sample noise histogram
    	TFile noiseDataRootFile("../NoiseData.root", "READ");
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
	
	///Map the coordinates to the pixels and ROI
	MapToPixels mapToPixels(inputData, pixelCoordinates, noiseHisto);
	mapToPixels.map();

	///Run python script to make xyz plots, because root is very annoying about this plot
	std::cout << "\nRunning python scripts for 3D event displays..." << std::endl;
	std::string command1 = "python ../scripts/xyzPlot.py " + std::to_string(pixelCoordinates.getTPCLength()) + " " + std::to_string(pixelCoordinates.getTPCHeight()) + " " + std::to_string(pixelCoordinates.getTPCWidth());
	system(command1.c_str());

	///Run GUI
	std::cout << "\nRunning Event Display... " << std::endl;
	std::string command2 = "root -l ../scripts/MapixEventDisplay.C";
	system(command2.c_str());
}

