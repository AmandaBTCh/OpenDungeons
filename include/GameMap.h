#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "Tile.h"
#include "Creature.h"
#include "Player.h"
#include "Room.h"
#include "Seat.h"

typedef map< pair<int,int>, Tile*> TileMap_t;

/*! \brief The class which stores the entire game state on the server and a subset of this on each client.
 *
 * This class is one of the key classes in the OpenDungeons game.  The map
 * itself, consisting of tiles and rooms, as well as any creatures and items in
 * the game are managed by an instance of the game map.  The game map can also
 * be queried by other subroutines to answer basic questions like "what is the
 * sortest path between two tiles" or "what creatures are in some particular
 * tile".
 */
class GameMap
{
	public:
		void createNewMap(int xSize, int ySize);
		void createAllEntities();
		void clearAll();

		// Game state methods
		void clearTiles();
		void addTile(Tile *t);
		Tile* getTile(int x, int y);
		TileMap_t::iterator firstTile();
		TileMap_t::iterator lastTile();
		unsigned int numTiles();

		void clearCreatures();
		void addCreature(Creature *c);
		void removeCreature(Creature *c);
		void queueCreatureForDeletion(Creature *c);
		Creature* getCreature(int index);
		Creature* getCreature(string cName);
		unsigned int numCreatures();

		void clearClasses();
		void addClassDescription(Creature c);
		void addClassDescription(Creature *c);
		Creature* getClassDescription(int index);
		Creature* getClassDescription(string query);
		unsigned int numClassDescriptions();

		void clearPlayers();
		bool addPlayer(Player *p);
		Player* getPlayer(int index);
		Player* getPlayer(string cName);
		Player* getPlayerByColour(int colour);
		unsigned int numPlayers();

		void clearRooms();
		void addRoom(Room *r);
		Room* getRoom(int index);
		unsigned int numRooms();

		void clearSeats();
		void addSeat(Seat *s);
		Seat* getSeat(int index);
		Seat* popSeat();
		unsigned int numSeats();

		// AI Methods
		void doTurn();

		list<Tile*> path(int x1, int y1, int x2, int y2, Tile::TileClearType passability);
		vector<Tile*> neighborTiles(int x, int y);
		list<Tile*> lineOfSight(int x1, int y1, int x2, int y2);
		bool pathIsClear(list<Tile*> path, Tile::TileClearType passability);
		void cutCorners(list<Tile*> &path, Tile::TileClearType passability);

		Player *me;

	private:
		map< pair<int,int>, Tile*> tiles;
		vector<Creature*> classDescriptions;
		vector<Creature*> creatures;
		vector<Creature*> creaturesToDelete;
		vector<Player*> players;
		vector<Room*> rooms;
		vector<Seat*> seats;
};

/*! \brief A helper class for the A* search in the GameMap::path function.
 *
 * This class stores the requesite information about a tile which is placed in
 * the search queue for the A-star, or A*, algorithm which is used to
 * calculate paths in the path function.
 */
class astarEntry
{
	public:
		Tile *tile;
		astarEntry *parent;
		double g, h;
		double fCost()	{return g+h;}
		void setHeuristic(int x1, int y1, int x2, int y2)
		{
			h = fabs((double)(x2-x1)) + fabs((double)(y2-y1));
		}
};

#endif

