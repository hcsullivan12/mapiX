#ifndef CHANNELDATA_H
#define CHANNELDATA_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Event.h"

class ChannelData {

	public:
		ChannelData(std::string trackFileName);
		void ReadFile();
		std::vector<Event> getEvents() const ;

	private:
		std::string file;
        std::vector<Event> events;
};
#endif
