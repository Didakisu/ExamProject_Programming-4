#include "HighscoreManager.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace dae
{
    HighscoreManager::HighscoreManager(const std::string& filename)
        : m_FileName(filename)
    {
    }

    void HighscoreManager::LoadHighscore()
    {
        std::ifstream file(m_FileName);
        if (!file.is_open())
        {
            std::cerr << "Failed to open highscore file: " << m_FileName << '\n';
            m_Initials = "___";
            m_Score = 0;
            return;
        }

        std::string initials;
        int score = 0;
        file >> initials >> score;

        if (file)
        {
            m_Initials = initials;
            m_Score = score;
        }
        else
        {
            std::cerr << "Failed to read highscore data from file: " << m_FileName << '\n';
            m_Initials = "___";
            m_Score = 0;
        }
    }

    void HighscoreManager::SaveHighscore(const std::string& initials, int score)
    {
        std::ofstream file(m_FileName, std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "Failed to open highscore file for writing: " << m_FileName << '\n';
            return;
        }

        file << initials << ' ' << std::setw(6) << std::setfill('0') << score;
    }

    int HighscoreManager::GetHighscore() const
    {
        return m_Score;
    }

    std::string HighscoreManager::GetInitials() const
    {
        return m_Initials;
    }
}