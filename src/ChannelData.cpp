#include "ChannelData.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>

ChannelData::ChannelData(std::string trackFileName) {
	this->file = trackFileName;
};

void ChannelData::ReadFile() {
	std::ifstream inputFile(file.c_str());
	if (!inputFile) {
	        std::cout << "Cannot open " << file << std::endl;
                exit(1);
	}
	std::string line;
	//int trackID;
	int eventNumber = 0;
	while(getline(inputFile, line)) {
		std::stringstream linestream(line);
		std::string s1;
		getline(linestream, s1, ' ');
		std::string runID;
		std::string subrunID;
		std::string eventID;
		
		if (s1 == "Run") {
			///In Event block
			///Get Run, Subrun, and Event IDs
			getline(linestream, runID);
			
			bool foundEvent = false;
			std::string s2;
			while (!foundEvent) {
				getline(inputFile, line);
				std::stringstream linestream(line);
				getline(linestream, s2, ' ');
				getline(linestream, s1);
				if(s2 == "Subrun") {
					
					subrunID = s1;
				}
				if(s2 == "Event") {
					eventID = s1;
					foundEvent = true;
					Event event;
					event.runID = atoi(runID.c_str());
					event.subrunID = atoi(subrunID.c_str());
					event.eventID = atoi(eventID.c_str());
					events.push_back(event);
					eventNumber++;
				}
			}
			continue;
		}
		
		std::string hitChannelNumber;
		std::string hitPeakTime;
		std::string hitADC;
		std::string hitWidth;
		
		hitChannelNumber = s1;
		getline(linestream, hitPeakTime, ' ');
		getline(linestream, hitADC, ' ');
		getline(linestream, hitWidth);
		
		Hit rawHit;
		rawHit.hitChannelNumber = atoi(hitChannelNumber.c_str());
		rawHit.hitPeakTime = atof(hitPeakTime.c_str());
		rawHit.hitADC = atof(hitADC.c_str());
		rawHit.hitWidth = atof(hitWidth.c_str());
		
		events.at(eventNumber - 1).rawHits.push_back(rawHit);
	}
}

std::vector<Event> ChannelData::getEvents() const {
	return events;
}
