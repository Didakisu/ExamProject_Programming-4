#pragma once
#include <string>

namespace dae
{
    class HighscoreManager
    {
    public:
        HighscoreManager(const std::string& filename);

        void LoadHighscore();
        void SaveHighscore(const std::string& initials, int score);

        int GetHighscore() const;
        std::string GetInitials() const;

    private:
        std::string m_FileName;
        std::string m_Initials;
        int m_Score;
    };
}