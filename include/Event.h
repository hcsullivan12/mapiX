#ifndef EVENT_H
#define EVENT_H

#include <map>
#include <vector>

struct Hit2d {
	
	unsigned channel;
	
	double colHitPeakTime;
	
	double colHitWidth;
	
	double colHitADC;
	
	double indHitPeakTime;
	
	double indHitWidth;
	
	double indHitADC;
		
	///Variables used for ChannelsToPixels
	unsigned hitChannelNumber;
	
	double hitPeakTime;
	
	double hitADC;
	
	double hitWidth;

};


///
/// 3D hit struct.
/// Used to store 3D hit (candidates).
///
struct Hit3d {
        ///
        /// X coordinate in mm.
        ///
        float x;

        ///
        /// Y coordinate in mm.
        ///
        float y;

        ///
        /// Z coordinate in mm.
        ///
        float z;

        ///
        /// Charge in C.
        ///
        float charge;

        unsigned pixelHitId;

        unsigned roiHitId;
};


struct PrincipalComponents {
        unsigned numHitsUsed;
        std::vector<double> eigenValues;
        std::vector<std::vector<double>> eigenVectors;
        std::vector<double> avePosition;
        double aveHitDoca;
};

struct Hit {
	
	///Variables used for CoordinatesToPixels
	unsigned channel;
	
	double x;
	
	double y;
	
	double z;
	
	double colHitPeakTime;
	
	double colHitWidth;
	
	double colHitADC;
	
	double indHitPeakTime;
	
	double indHitWidth;
	
	double indHitADC;
	
	///Variables used for ChannelsToPixels
	unsigned hitChannelNumber;
	
	double hitPeakTime;
	
	double hitADC;
	
	double hitWidth;
	
};

struct Event {
        
    ///
    /// Event ID.
    ///
    unsigned eventID;
        
    ///
    /// Run ID.
    ///
    unsigned runID;
        
    ///
    /// Subrun ID.
    ///
    unsigned subrunID;
        
    ///
    /// Raw Hits
    ///
    std::vector<Hit> rawHits;
        
    ///
    /// Map from viper2dHit.firstSample to its index in pixelHits.
    ///
    std::multimap<double, Hit> dataHitOrder;

    ///
    /// Pixel hits stored at viper2dHit structs.
    ///
    std::vector<Hit2d> pixelHits;

    ///
    /// Map from viper2dHit.firstSample to its index in pixelHits.
    /// (sample, pixelID)
    std::multimap<double, Hit2d> pixelHitOrder;

    ///
    /// Map from viper2dHit.lastSample to its index in pixelHits.
    ///
    std::multimap<unsigned, unsigned> pixelHitOrderTrail;

    ///
    /// ROI hits stored at viper2dHit structs.
    ///
    std::vector<Hit2d> roiHits;

    ///
    /// Map from viper2dHit.firstSample to its index in roiHits.
    /// (sample, roiID)
    std::multimap<double, Hit2d> roiHitOrder;

    ///
    /// Map from viper2dHit.lastSample to its index in roiHits.
    ///
    std::multimap<unsigned, unsigned> roiHitOrderTrail;

    ///
    /// Vector of indices of all matched roiHits for each pixelHit entry.
    ///
    std::vector<std::vector<unsigned>> pixel2roi;

    ///
    /// Vector of indices of all matched pixelHits for each roiHit entry.
    ///
    std::vector<std::vector<unsigned>> roi2pixel;

    ///
    /// viper3dHit candidates generated from pixel2roi and pixelHits.
    /// Has the same dimensions as pixel2roi.
    ///
    std::vector<std::vector<Hit3d>> hitCandidates;
};

#endif
