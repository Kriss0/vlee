/* Copyright (C) 2007-2008 Erik Faye-Lund and Egbert Teeselink
 * For conditions of distribution and use, see copyright notice in LICENSE.TXT
 */

#include "stdafx.h"
#include "data.h"

using namespace sync;

Data::~Data()
{
	for (size_t i = 0; i < actualTracks.size(); ++i)
		delete actualTracks[i];
}

size_t Data::getTrackIndex(const std::basic_string<TCHAR> &name)
{
	TrackContainer::iterator iter = tracks.find(name);
	if (iter != tracks.end())
	{
		return iter->second;
	}
	
	size_t index = actualTracks.size();
	tracks[name] = index;
	actualTracks.push_back(new sync::Track);
	return index;
}

Track &Data::getTrack(const std::basic_string<TCHAR> &name)
{
	size_t index = getTrackIndex(name);
	assert(index < int(actualTracks.size()));
	assert(NULL != actualTracks[index]);
	return *actualTracks[index];
}

size_t Data::getTrackCount() const
{
	return tracks.size();
}
