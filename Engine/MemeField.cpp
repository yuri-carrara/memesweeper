#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "Graphics.h"
#include "SpriteCodex.h"
#include <iostream>

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch ( state )
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (HasMeme())
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTileNumberCount(screenPos, neighborMemes, gfx);
		}
	}
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme); //avoid to put a mine on a tile already taken
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed()
{
	return state == State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool MemeField::Tile::IsFlagged()
{
	return state == State::Flagged;
}

void MemeField::Tile::setNeighborMemeCount(int memeCount)
{
	assert(neighborMemes == -1);
	neighborMemes = memeCount;
}

MemeField::MemeField(int nMemes)
{
	assert(nMemes > 0 && nMemes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int nSpawn = 0; nSpawn < nMemes; ++nSpawn)
	{
		Vei2 spawnPos;
		do // looking in while loop until a tile that doesn't have a mine 
		{
			spawnPos = { xDist(rng), yDist(rng) };
			
		}
		while (TileAt(spawnPos).HasMeme());
		TileAt(spawnPos).SpawnMeme(); // then we spawn (we set meme to true)
	}
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; ++gridPos.y)
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
			TileAt(gridPos).setNeighborMemeCount(countNeighborMemes(gridPos));

}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; ++gridPos.y)
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
}

RectI MemeField::GetRect() const
{
	return RectI(0, width*SpriteCodex::tileSize,0,height*SpriteCodex::tileSize);
}

void MemeField::RevealOnClick(const Vei2& screenPos)
{
	Vei2 tilePos = ScreenToTile(screenPos);
	assert(tilePos.x >= 0 && tilePos.x <= width && tilePos.y >= 0 && tilePos.y <= height);
	if (!TileAt(tilePos).IsRevealed() && !TileAt(tilePos).IsFlagged())
	{
		TileAt(tilePos).Reveal();
	}
}

void MemeField::FlagOnClick(const Vei2& screenPos)
{
	Vei2 tilePos = ScreenToTile(screenPos);
	assert(tilePos.x >= 0 && tilePos.x <= width && tilePos.y >= 0 && tilePos.y <= height);
	Tile& tile = TileAt(tilePos);
	if (!tile.IsRevealed())
	{
		tile.ToggleFlag();
	}

}

MemeField::Tile& MemeField::TileAt(const Vei2& gridpos)
{
	return field[gridpos.y * width + gridpos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridpos) const
{
	return field[gridpos.y * width + gridpos.x];
}

Vei2 MemeField::ScreenToTile(const Vei2& screenPos)
{
	return Vei2(screenPos) / SpriteCodex::tileSize;
}

int MemeField::countNeighborMemes(const Vei2& gridPos)
{
	int startY = std::max(0, gridPos.y - 1);
	int startX = std::max(0, gridPos.x - 1);
	int endY = std::min(gridPos.y + 1, height - 1);
	int endX = std::min(gridPos.x + 1, width - 1);

	int count = 0;


	for (Vei2 gridPos{ startX,startY }; gridPos.y < endY; ++gridPos.y)
		for (gridPos.x = startX; gridPos.x < endX; ++gridPos.x)
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}

	return count;
}
