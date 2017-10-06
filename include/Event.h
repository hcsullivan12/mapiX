#ifndef EVENT_H
#define EVENT_H

#include <map>
#include <vector>

struct Hit2d {
        ///
        /// Pixel channel.
        ///
        unsigned channel;

        ///
        /// Index in the raw histogram of the first sample of the pulse.
        ///
        unsigned firstSample;

        ///
        /// Index in the raw histogram of the positive peak sample of the pulse.
        ///
        unsigned posPeakSample;

        ///
        /// Index in the raw histogram of the first negative sample of the pulse.
        ///
        unsigned zeroCrossSample;

        ///
        /// Index in the raw histogram of the negative peak sample of the pulse.
        ///
        unsigned negPeakSample;

        ///
        /// Index in the raw histogram of the last sample of the pulse.
        ///
        unsigned lastSample;

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

struct Event {
        ///
        /// Event ID.
        ///
        unsigned eventID;

	///
        /// Map from viper2dHit.firstSample to its index in pixelHits.
        ///
        std::multimap<double, std::pair<double, double>> dataHitOrder;

        ///
        /// Pixel hits stored at viper2dHit structs.
        ///
        std::vector<Hit2d> pixelHits;

        ///
        /// Map from viper2dHit.firstSample to its index in pixelHits.
        /// (sample, pixelID)
        std::multimap<double, unsigned> pixelHitOrder;

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
        std::multimap<double, unsigned> roiHitOrder;

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

        std::vector<int> pcaIds;

        PrincipalComponents principalComponents;
};

#endif
