#ifndef EVENTDATA_H
#define EVENTDATA_H
#include <iostream>
#include <string>
#include <vector>
#include <map>

class EventData {

	public:
		EventData(std::string trackFileName);
		void ReadFile();
		std::multimap<int, std::vector<double> > getEventMap() const;
		std::vector<int> getEvents() const ;

	private:
		std::string file;
                std::multimap<int, std::vector<double> > eventMap;
                std::vector<int> events;
};
#endif
