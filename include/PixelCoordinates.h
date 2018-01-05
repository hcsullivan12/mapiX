#ifndef PIXELCOORDINATES_H
#define PIXELCOORDINATES_H

#include <array>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"

class PixelCoordinates {

	public:
		PixelCoordinates(std::string pixelCoordinatesFileName);
		
		unsigned getNPixels() const {
            		return nPixels;
        	}

		unsigned getNPixelsPerPCB() const {
            		return nPixelsPerPCB;
        	}

		unsigned getNROI() const {
            		return nROIs;
        	}

		double getPixelPitch() const {
            		return pixelPitch;
        	}
	
		double getDriftSpeed() const {
            		return driftSpeed;
        	}

		double getDigitizationSpeed() const {
            		return digitizationSpeed;
        	}

		unsigned getNSamples() const {
			return nSamples;
		}

		double getWidthOfPixelPulse() const {
			return widthOfPixelPulse;
		}
		
		double getWidthOfROIPulse() const {
			return widthOfROIPulse;
		}

		double getHitDisc() const {
			return hitDisc;
		}
		
		double getROIPeakDifference() const {
			return roiPeakDifference;
		}

		double getTPCLength() const {
			return tpcLength;
		}

		double getTPCWidth() const {
                        return tpcWidth;
                }

		double getTPCHeight() const {
                        return tpcHeight;
                }

		std::vector<double> getTPCOrigin() const {
			return tpcOrigin;
		}

		std::vector<std::vector<int>> getPixelCoor() const {
			return pixelCoor;
		}

		std::vector<std::vector<int>> getROICoor() const {
			return roiCoor;
		}
	
		double getPCBLength() const {
			return pcbLength;
		}

		double getPCBSideWidth() const {
            		return pcbSideWidth;
        	}

		double getPixelRegionWidth() const {
            		return pixelRegionWidth;
        	}

		double getPixelRegionHeight() const {
            		return pixelRegionHeight;
        	}

		std::vector<double> getPCBOrigin() const {
			return pcbOrigin;
		}

	private:

        	const rapidjson::Value& getJsonMember(
              		  const std::string t_memberName,
              		  const rapidjson::Type t_memberType,
                          const unsigned t_arraySize = 0,
                	  const rapidjson::Type t_arrayType = rapidjson::kNullType);
	
	        const std::array<std::string, 7> m_jsonTypes = {{"Null", "False", "True", "Object", "Array", "String", "Number"}};

		rapidjson::Document jsonDoc;

		unsigned nPixels;
		
		unsigned nPixelsPerPCB;

		unsigned nROIs;

		double pcbLength;

		double pcbSideWidth;

		double pixelRegionWidth;

		double pixelRegionHeight;
	
		std::vector<double> pcbOrigin;

		double pixelPitch;
		
		double driftSpeed;
		
		double digitizationSpeed;

		unsigned nSamples;

		double widthOfPixelPulse;
		
		double widthOfROIPulse;
		
		double hitDisc;
		
		double roiPeakDifference;

		double tpcLength;

		double tpcWidth;

		double tpcHeight;

		std::vector<double> tpcOrigin;

		std::vector<std::vector<int>> pixelCoor;

		std::vector<std::vector<int>> roiCoor;

		


};
#endif
