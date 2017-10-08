#ifndef MAPTOPIXELS_H
#define MAPTOPIXELS_H

#include "Event.h"
#include "EventData.h"
#include "PixelCoordinates.h"
#include "TH1.h"

class MapToPixels {

	public:
		MapToPixels(const EventData &t_inputData, const PixelCoordinates &t_pixelCoordinates, TH1D *t_noiseHisto);
		void map();
		

	private:
		std::vector<Event> events;
		const EventData &inputData;
		const PixelCoordinates &pixelCoordinates;
		const TH1D *noiseHisto;

		void find2DHits(const unsigned &t_runID,
				 const unsigned &t_subrunID,
 			     const unsigned &t_eventID,
 			     const std::vector<Hit> &t_rawHits,
			     std::vector<Hit2d> &t_pixelHits,
		         std::multimap<double, unsigned> &t_pixelHitOrder,
	             std::vector<Hit2d> &t_roiHits,
                 std::multimap<double, unsigned> &t_roiHitOrder,
			     std::multimap<double, Hit> &t_dataHitOrder);
		void convertYZToPixelUnits(std::pair<double, double> &t_YZCoor); 
		void convertYZToROIandPixelIDs(const std::multimap<double, std::pair<double, double>> &t_dataHitOrder, 
                 std::vector<Hit2d> &t_roiHits, std::multimap<double, unsigned> &t_roiHitOrder,
                 std::vector<Hit2d> &t_pixelHits, std::multimap<double, unsigned> &t_pixelHitOrder);
};







#endif
