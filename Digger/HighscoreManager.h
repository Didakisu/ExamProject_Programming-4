#pragma once
#include <string>
#include <vector>

namespace dae
{
    struct HighscoreEntry
    {
        std::string initials;
        int score;
    };

    class HighscoreManager
    {
    public:
        explicit HighscoreManager(const std::string& filename);

        void LoadHighscore();
        void SaveHighscore(const std::string& initials, int score); 

        int GetHighscore() const;                    
        std::string GetBestInitials() const;        
        const std::vector<HighscoreEntry>& GetAllScores() const; 

    private:
        std::string m_FileName;
        std::vector<HighscoreEntry> m_Entries;
    };
}
