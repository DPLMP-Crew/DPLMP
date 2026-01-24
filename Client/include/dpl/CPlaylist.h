#pragma once
#define TRACK_MISSING 0xFD
#define TRACK_END 0xFE
#define TRACK_INVALID 0xFF

class CPlaylist {
public:
	char m_TrackCount;
	char m_CurrentTrack;
	char m_Tracks[50];
	void* m_TrackFilenames;
	void Shuffle();
};