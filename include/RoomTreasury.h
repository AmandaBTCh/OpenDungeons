#ifndef ROOMTREASURY_H
#define ROOMTREASURY_H

#include "Room.h"

class RoomTreasury : public Room
{
	public:
		RoomTreasury();

		// Functions overriding virtual functions in the Room base class.
		void doUpkeep();
		void addCoveredTile(Tile* t);
		void removeCoveredTile(Tile* t);
		void clearCoveredTiles();

		// Functions specific to this class.
		int getTotalGold();
		void depositGold(int gold, Tile *tile);

	private:
		map<Tile*,int> goldInTile;
};

#endif
