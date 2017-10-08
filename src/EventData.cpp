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
		std::string s;
		getline(linestream, s, ' ');
		std::string runID;
		std::string subrunID;
		std::string eventID;
		
		if (s == "====================================") {
			//std::cout << s << std::endl;
			continue;
		}
		
		if (s == "Run") {
			///In Event block
			///Get Run, Subrun, and Event IDs
			getline(linestream, runID);
			
			bool foundEvent = false;
			std::string title;
			while (!foundEvent) {
				getline(inputFile, line);
				std::stringstream linestream(line);
				getline(linestream, title, ' ');
				getline(linestream, s);
				if(title == "Subrun") {
					subrunID = s;
				}
				if(title == "Event") {
					eventID = s;
					foundEvent = true;
					Event event;
					event.runID = atoi(runID.c_str());
					event.subrunID = atoi(subrunID.c_str());
					event.eventID = atoi(eventID.c_str());
					events.push_back(event);
					eventNumber++;
				}
			}
			//std::cout << "runID: " << runID << "  " << "SubrunID: " << subrunID << "  " << "Event: " << eventID << std::endl;
			
			//trackID = -1;
			continue;
		}

		if(s == "Track") {
			//trackID++;
			//Track track;
			//track.trackID = trackID;
			//events.at(eventNumber - 1).tracks.push_back(track);
			//std::cout << "Track: " << trackID << std::endl;
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
		
		x = s;
		getline(linestream, y, ' ');
		getline(linestream, z, ' ');
		getline(linestream, colHitPeakTime, ' ');
		getline(linestream, colHitWidth, ' ');
		getline(linestream, colHitADC, ' ');
		getline(linestream, indHitPeakTime, ' ');
		getline(linestream, indHitWidth, ' ');
		getline(linestream, indHitADC);
		
		
		if(atof(x.c_str()) == 0 && atof(y.c_str()) == 0 && atof(z.c_str()) == 0) {
			continue;
		}
		
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
		
		/*///Get Run, Subrun, and Event IDs
		getline(linestream, s, ',');
		getline(linestream, y, ',');
		getline(linestream, z);*/
		
/*
		if(x == "Run" && y == " SubRun" && z == " Event") {
                        getline(inputFile, line);
                        std::stringstream linestream(line);
                        std::string x;
                        std::string y;
                        std::string z;
                        getline(linestream, x, ',');
                        getline(linestream, y, ',');
                        getline(linestream, z);
                        event = atoi(z.c_str());
			events.push_back(event);
                        continue;
		}
		if(atof(x.c_str()) == 0 && atof(y.c_str()) == 0 && atof(z.c_str()) == 0) {
			continue;
		}

		std::vector<double> v = {atof(x.c_str()), atof(y.c_str()), atof(z.c_str())};
		eventMap.insert(std::make_pair(event, v));
	}

	//std::multimap<int, std::vector<double> >::const_iterator it;
	for (const auto &event : events){
		std::cout << "Event: " << event << std::endl;
		for (auto it = eventMap.lower_bound(event); it != eventMap.upper_bound(event); it++) {
			std::cout << it->second.at(0) << ", " << it->second.at(1) << ", " << it->second.at(2) << std::endl;
		}
	}*/
}

std::vector<Event> EventData::getEvents() const {
	return events;
}
