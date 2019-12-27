#pragma once
#ifndef HASH_DOMINO_TILE_PTR_H
#define HASH_DOMINO_TILE_PTR_H

#include "DominoTile.h"
#include <functional>

class HashDominoTilePtr {
public:
	size_t operator()(const DominoTile* d) const {
		return (std::hash<short>()(d->first) ^ std::hash<short>()(d->second));
	}
};

#endif // HASH_DOMINO_TILE_PTR_H
