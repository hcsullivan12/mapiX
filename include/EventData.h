#ifndef EVENTDATA_H
#define EVENTDATA_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Event.h"

class EventData {

	public:
		EventData(std::string trackFileName);
		void ReadFile();
		std::vector<Event> getEvents() const ;

	private:
		std::string file;
        std::vector<Event> events;
};
#endif
