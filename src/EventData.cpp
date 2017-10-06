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
	int event;
	while(getline(inputFile, line)) {
		std::stringstream linestream(line);
		std::string x;
		std::string y;
		std::string z;
		getline(linestream, x, ',');
		getline(linestream, y, ',');
		getline(linestream, z);

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
	/*for (const auto &event : events){
		std::cout << "Event: " << event << std::endl;
		for (auto it = eventMap.lower_bound(event); it != eventMap.upper_bound(event); it++) {
			std::cout << it->second.at(0) << ", " << it->second.at(1) << ", " << it->second.at(2) << std::endl;
		}
	}*/
}

std::multimap<int, std::vector<double> > EventData::getEventMap() const {
	return eventMap;
}

std::vector<int> EventData::getEvents() const {
	return events;
}
