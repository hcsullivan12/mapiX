#ifndef MAPCHANNELSTOPIXELS_H
#define MAPCHANNELSTOPIXELS_H

#include "Event.h"
#include "ChannelData.h"
#include "PixelCoordinates.h"
#include "TH1.h"

class MapChannelsToPixels {

	public:
		MapChannelsToPixels(const ChannelData &t_inputData, const PixelCoordinates &t_pixelCoordinates, TH1D *t_noiseHisto, const std::multimap< unsigned, std::pair<std::string, unsigned> > &t_lariatChannelToPCB, const std::string t_outputFile);
		void map();
		

	private:
		std::vector<Event> events;
		const ChannelData &inputData;
		const PixelCoordinates &pixelCoordinates;
		const TH1D *noiseHisto;
		std::multimap< unsigned, std::pair<std::string, unsigned> > lariatChannelToPCB;
		const std::string outputFile;

		void find2DHits(const unsigned &t_runID,
				 const unsigned &t_subrunID,
 			     const unsigned &t_eventID,
 			     const std::vector<Hit> &t_rawHits,
			     std::vector<Hit2d> &t_pixelHits,
		         std::multimap<double, Hit2d> &t_pixelHitOrder,
	             std::vector<Hit2d> &t_roiHits,
                 std::multimap<double, Hit2d> &t_roiHitOrder,
			     std::multimap<double, Hit> &t_dataHitOrder);
};

#endif
