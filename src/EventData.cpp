#include "EventData.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>

EventData::EventData(std::string trackFileName) {
	this->file = trackFileName;
};

void EventData::ReadFile() {
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
		
		std::string x;
		std::string y;
		std::string z;
		std::string colHitPeakTime;
		std::string colHitWidth;
		std::string colHitADC;
		std::string indHitPeakTime;
		std::string indHitWidth;
		std::string indHitADC;
		
		x = s1;
		getline(linestream, y, ' ');
		getline(linestream, z, ' ');
		getline(linestream, colHitPeakTime, ' ');
		getline(linestream, colHitWidth, ' ');
		getline(linestream, colHitADC, ' ');
		getline(linestream, indHitPeakTime, ' ');
		getline(linestream, indHitWidth, ' ');
		getline(linestream, indHitADC);
		
		Hit rawHit;
		rawHit.x = atof(x.c_str());
		rawHit.y = atof(y.c_str());
		rawHit.z = atof(z.c_str());
		rawHit.colHitPeakTime = atof(colHitPeakTime.c_str());
		rawHit.colHitWidth = atof(colHitWidth.c_str());
		rawHit.colHitADC = atof(colHitADC.c_str());
		rawHit.indHitPeakTime = atof(indHitPeakTime.c_str());
		rawHit.indHitWidth = atof(indHitWidth.c_str());
		rawHit.indHitADC =atof(indHitADC.c_str());
		
		events.at(eventNumber - 1).rawHits.push_back(rawHit);
	}
}

std::vector<Event> EventData::getEvents() const {
	return events;
}
