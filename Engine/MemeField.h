#pragma once
#include "Graphics.h"
#include "Vei2.h"

class MemeField 
{
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};
	public:
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		void SpawnMeme();
		bool HasMeme() const;
		void Reveal();
		bool IsRevealed();
		void ToggleFlag();
		bool IsFlagged();
		void setNeighborMemeCount(int memeCount);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int neighborMemes = -1;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void RevealOnClick(const Vei2& screenPos);
	void FlagOnClick(const Vei2& screenPos);
private:
	Tile& TileAt(const Vei2& gridpos);
	const Tile& TileAt(const Vei2& gridpos) const;
	Vei2 ScreenToTile(const Vei2& screenPos);
	int countNeighborMemes(const Vei2& gridPos);
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	bool isFucked = false;
	Tile field[width * height];
};