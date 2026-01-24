#include "dpl/CPlaylist.h"
#include <algorithm>
#include <random>
#include <cstdint>

void CPlaylist::Shuffle() {
	static std::mt19937 rng{ std::random_device{}() };
	std::shuffle(m_Tracks, m_Tracks + m_TrackCount, rng);
}