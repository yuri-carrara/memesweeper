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
	private:
		State state = State::Hidden;
		bool hasMeme = false;
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
private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[width * height];
};