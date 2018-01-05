#include "PixelCoordinates.h"

PixelCoordinates::PixelCoordinates(const std::string pixelCoordinatesFileName) {
	
	FILE *pixelCoordinatesFile = fopen(pixelCoordinatesFileName.c_str(), "r");
        
        ///Use to rapidjson to read Parameter file
        char readBuffer[65536];
        rapidjson::FileReadStream jsonStream(pixelCoordinatesFile, readBuffer, sizeof(readBuffer));
        jsonDoc.ParseStream(jsonStream);
        fclose(pixelCoordinatesFile);
	if (!jsonDoc.IsObject()) {
            std::cerr << "ERROR: (!IsObject) Failed to parse pixel coordinate file " << pixelCoordinatesFileName << '!' << std::endl;
            exit(1);
        }
        if (jsonDoc.HasParseError()) {
            std::cerr << "ERROR: (HasParseError) Failed to parse pixel coordinate file " << pixelCoordinatesFileName << '!' << std::endl;
            exit(1);
        }

	nPixels              = getJsonMember("nPixels", rapidjson::kNumberType).GetUint();
	nPixelsPerPCB        = getJsonMember("nPixelsPerPCB", rapidjson::kNumberType).GetUint();
	nROIs                = getJsonMember("nROIs", rapidjson::kNumberType).GetUint();
	
        auto jsonArrayItr = getJsonMember("tpcOrigin", rapidjson::kArrayType, 3, rapidjson::kNumberType).Begin();
        for (int component = 0; component < 3; component++) {
            tpcOrigin.push_back(jsonArrayItr->GetDouble());
            ++jsonArrayItr;
        }
	
	pcbSideWidth         = getJsonMember("pcbSideWidth", rapidjson::kNumberType).GetDouble(); //cm
	pixelRegionWidth     = getJsonMember("pixelRegionWidth", rapidjson::kNumberType).GetDouble(); //cm
	pixelRegionHeight    = getJsonMember("pixelRegionHeight", rapidjson::kNumberType).GetDouble();	//cm
	
	pixelPitch           = getJsonMember("pixelPitch", rapidjson::kNumberType).GetDouble(); //cm
	driftSpeed           = getJsonMember("driftSpeed", rapidjson::kNumberType).GetDouble(); //cm/us
	digitizationSpeed    = getJsonMember("digitizationSpeed", rapidjson::kNumberType).GetDouble(); //ns/sample
	nSamples             = getJsonMember("nSamples", rapidjson::kNumberType).GetUint();
	widthOfPixelPulse    = getJsonMember("widthOfPixelPulse", rapidjson::kNumberType).GetDouble(); 
	widthOfROIPulse      = getJsonMember("widthOfROIPulse", rapidjson::kNumberType).GetDouble(); 
	hitDisc              = getJsonMember("hitDisc", rapidjson::kNumberType).GetDouble(); 
	roiPeakDifference    = getJsonMember("roiPeakDifference", rapidjson::kNumberType).GetDouble();
	
	tpcLength    = getJsonMember("tpcLength", rapidjson::kNumberType).GetDouble();
	tpcWidth    = getJsonMember("tpcWidth", rapidjson::kNumberType).GetDouble();
	tpcHeight    = getJsonMember("tpcHeight", rapidjson::kNumberType).GetDouble();
	
	pcbOrigin.push_back(pcbSideWidth + pixelPitch/2);            //z
	pcbOrigin.push_back(pixelRegionHeight/2 - pixelPitch/2);     //y
	

	///Pixel Coordinates (Y, Z)
        pixelCoor = std::vector<std::vector<int>>(nPixels, std::vector<int>(2));
        jsonArrayItr = getJsonMember("pixelCoorZ", rapidjson::kArrayType, nPixels, rapidjson::kNumberType).Begin();
        for (auto &&channel : pixelCoor) {
            channel.at(1) = jsonArrayItr->GetInt();
            ++jsonArrayItr;
        }
        jsonArrayItr = getJsonMember("pixelCoorY", rapidjson::kArrayType, nPixels, rapidjson::kNumberType).Begin();
        for (auto &&channel : pixelCoor) {
            channel.at(0) = jsonArrayItr->GetInt();
            ++jsonArrayItr;
        }
        
        ///ROI Coordinates (Y, Z)
        roiCoor = std::vector<std::vector<int>>(nROIs, std::vector<int>(2));
        jsonArrayItr = getJsonMember("roiCoorZ", rapidjson::kArrayType, nROIs, rapidjson::kNumberType).Begin();
        for (auto &&channel : roiCoor) {
            channel.at(1) = jsonArrayItr->GetInt();
            ++jsonArrayItr;
        }
        jsonArrayItr = getJsonMember("roiCoorY", rapidjson::kArrayType, nROIs, rapidjson::kNumberType).Begin();
        for (auto &&channel : roiCoor) {
            channel.at(0) = jsonArrayItr->GetInt();
            ++jsonArrayItr;
	}
}

const rapidjson::Value & PixelCoordinates::getJsonMember(
            const std::string t_memberName,
            const rapidjson::Type t_memberType,
            const unsigned t_arraySize,
            const rapidjson::Type t_arrayType) {

        ///Check to see if the document has memberName
        if (!jsonDoc.HasMember(t_memberName.c_str())) {
            std::cerr << "ERROR: Entry \"" << t_memberName << "\" in run parameter file not found!" << std::endl;
            exit(1);
        }
        
        ///Get the value specified for memberName
        rapidjson::Value &member = jsonDoc[t_memberName.c_str()];
        
        if (((t_memberType == rapidjson::kTrueType) || (t_memberType == rapidjson::kFalseType)) &&
                !((member.GetType() == rapidjson::kTrueType) || (member.GetType() == rapidjson::kFalseType))) {
            std::cerr << "ERROR: Entry \"" << t_memberName << "\" in run parameter file has wrong type!"
                      << std::endl;
            std::cerr << "Expected " << m_jsonTypes.at(rapidjson::kTrueType)
                      << " or " << m_jsonTypes.at(rapidjson::kFalseType)
                      << ", got " << m_jsonTypes.at(member.GetType()) << '.' << std::endl;
            exit(1);
        }
        else if (member.GetType() != t_memberType) {
            std::cerr << "ERROR: Entry \"" << t_memberName << "\" in run parameter file has wrong type!"
                      << std::endl;
            std::cerr << "Expected " << m_jsonTypes.at(t_memberType) << ", got " << m_jsonTypes.at(member.GetType())
                      << '.' << std::endl;
            exit(1);
        }
        if (member.GetType() == rapidjson::kArrayType) {
            if (member.Size() != t_arraySize) {
                std::cerr << "ERROR: Size mismatch for array \"" << t_memberName << "\" in run parameter file!"
                          << std::endl;
                std::cerr << "Expected " << t_arraySize << ", got " << member.Size() << '.' << std::endl;
                exit(1);
            }
            for (const auto &value : member.GetArray()) {
                if (value.GetType() != t_arrayType) {
                    std::cerr << "ERROR: Type mismatch in array \"" << t_memberName << "\" in run parameter file!"
                              << std::endl;
                    std::cerr << "Expected " << m_jsonTypes.at(t_arrayType) << ", got "
                              << m_jsonTypes.at(value.GetType()) << '.' << std::endl;
                    exit(1);
                }
            }
        }
        return member;
}


