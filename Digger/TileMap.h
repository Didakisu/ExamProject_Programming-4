#pragma once
#include "TileType.h"
#include <vector>
#include <string>

class TileMap {
public:
    static constexpr int TILE_WIDTH_EMPTY = 32;
    static constexpr int TILE_HEIGHT_EMPTY = 32;

    static constexpr int TILE_WIDTH = 32;
    static constexpr int TILE_HEIGHT = 32;

    void LoadFromText(const std::vector<std::string>& lines);
    void Render() const;

    int GetWidth() const { return static_cast<int>(m_Tiles[0].size()); }
    int GetHeight() const { return static_cast<int>(m_Tiles.size()); }

    TileType GetTile(int x, int y) const;
    void SetTile(int x, int y, TileType type);
    std::vector<std::vector<TileType>> m_Tiles;
};