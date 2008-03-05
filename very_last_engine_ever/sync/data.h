/* Copyright (C) 2007-2008 Erik Faye-Lund and Egbert Teeselink
 * For conditions of distribution and use, see copyright notice in LICENSE.TXT
 */

#pragma once

#include <string>
#include <map>
#include <vector>

#include <exception>
#include <cmath>
#include <cassert>

#ifdef WIN32
#include <tchar.h>
#else
#define TCHAR char
#endif

#include "track.h"

namespace sync
{
	class Data
	{
	public:
		~Data();
		size_t getTrackIndex(const std::basic_string<TCHAR> &name);
		Track &getTrack(const std::basic_string<TCHAR> &name);
		
		Track &getTrack(size_t track)
		{
			assert(track < actualTracks.size());
			assert(NULL != actualTracks[track]);
			return *actualTracks[track];
		}
		
		size_t getTrackCount() const;
		
		typedef std::map<const std::basic_string<TCHAR>, size_t> TrackContainer;
		TrackContainer tracks;
	protected:
		std::vector<Track*> actualTracks;
	};
}
