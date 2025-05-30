#include "TileMap.h"
#include <stdexcept>

void TileMap::LoadFromText(const std::vector<std::string>& lines) {
    m_Tiles.clear();
    for (const auto& line : lines) {
        std::vector<TileType> row;
        for (char c : line) {
            switch (c) {
            case 'D': row.push_back(TileType::Dirt); break;
            case 'E': row.push_back(TileType::Empty); break;
            case 'G': row.push_back(TileType::Gem); break;
            case 'B': row.push_back(TileType::GoldBag); break;
            case '.': row.push_back(TileType::Boundary); break;
            case 'H': row.push_back(TileType::Hole); break;
            case 'U': row.push_back(TileType::UI); break;
            default: throw std::runtime_error("Invalid tile in level file");
            }
        }
        m_Tiles.push_back(std::move(row));
    }
}

void TileMap::Render() const {

}

TileType TileMap::GetTile(int x, int y) const {
    if (y < 0 || y >= static_cast<int>(m_Tiles.size()) ||
        x < 0 || x >= static_cast<int>(m_Tiles[y].size())) return TileType::Dirt;
    return m_Tiles[y][x];
}

void TileMap::SetTile(int x, int y, TileType type) {
    if (y >= 0 && y < static_cast<int>(m_Tiles.size()) &&
        x >= 0 && x < static_cast<int>(m_Tiles[y].size())) {
        m_Tiles[y][x] = type;
    }
}