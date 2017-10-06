#ifndef HITFINDER_H
#define HITFINDER_H

#include "EventData.h"

class HitFinder {

	public:
		HitFinder(const EventData &inputData);
	private:
		const EventData& coordinateData;
};

#endif
