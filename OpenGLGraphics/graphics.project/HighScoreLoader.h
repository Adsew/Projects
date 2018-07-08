#pragma once

#include "Component.h"

class HighScoreLoader : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(HighScoreLoader, Component);

protected:
    std::string scores;

    HighScoreLoader();
    void initialize() override;
public:
    std::string getHighScores();
    void saveHighScore(std::string name, std::string score);
};

