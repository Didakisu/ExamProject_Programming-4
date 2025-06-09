#include "HighscoreManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>

namespace dae
{
    HighscoreManager::HighscoreManager(const std::string& filename)
        : m_FileName(filename)
    {

    }

    void HighscoreManager::LoadHighscore()
    {
        std::ifstream file(m_FileName);
        m_Entries.clear();

        if (!file.is_open())
        {
            return;
        }

        std::string initials;
        int score = 0;
        while (file >> initials >> score)
        {
            m_Entries.push_back({ initials, score });
        }

        if (m_Entries.empty())
        {
            m_Entries.push_back({ "___", 0 });
        }
    }

    void HighscoreManager::SaveHighscore(const std::string& initials, int score)
    {
        std::filesystem::path absPath = std::filesystem::absolute(m_FileName);
        std::filesystem::path dir = absPath.parent_path();

        if (!std::filesystem::exists(dir))
        {
            std::error_code ec;
            if (!std::filesystem::create_directories(dir, ec))
            {
                return;
            }
        }

        std::ofstream file(m_FileName, std::ios::app);
        if (!file.is_open())
        {
            return;
        }

        file << initials << ' ' << std::setw(6) << std::setfill('0') << score << '\n';

        if (!file.good())
        {
            std::cerr << "Failed to write highscore to file." << std::endl;
        }
        else
        {
            std::cout << "Saved score: " << score << " with initials: " << initials << std::endl;
        }
    }

    int HighscoreManager::GetHighscore() const
    {
        int best = 0;
        for (const auto& entry : m_Entries)
        {
            if (entry.score > best)
                best = entry.score;
        }
        return best;
    }

    std::string HighscoreManager::GetBestInitials() const
    {
        int best = 0;
        std::string bestInitials = "___";
        for (const auto& entry : m_Entries)
        {
            if (entry.score > best)
            {
                best = entry.score;
                bestInitials = entry.initials;
            }
        }
        return bestInitials;
    }

    const std::vector<HighscoreEntry>& HighscoreManager::GetAllScores() const
    {
        return m_Entries;
    }
}
