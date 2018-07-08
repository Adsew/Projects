#include "Core.h"

#include "HighScoreLoader.h"

IMPLEMENT_DYNAMIC_CLASS(HighScoreLoader);

HighScoreLoader::HighScoreLoader()
{
}

void HighScoreLoader::initialize()
{
    tinyxml2::XMLDocument saveFile;
    saveFile.LoadFile("Assets/Saves/scores.xml");

    scores = "";

    if (!saveFile.Error())
    {
        XMLElement *node = saveFile.FirstChildElement("Player");;
        do
        {
            if (node != NULL)
            {
                if (auto str = node->Attribute("name"))
                {
                    scores += str;
                }
                scores += " - ";
                if (auto str = node->Attribute("score"))
                {
                    scores += str;
                }
                scores += "\n";
            }

            node = node->NextSiblingElement();

        } while (node != NULL);
    }
    else
    {
        LOG("Error opening Assets/Saves/scores.xml for loading.");
    }
}

std::string HighScoreLoader::getHighScores()
{
    return scores;
}

void HighScoreLoader::saveHighScore(std::string name, std::string score)
{
    tinyxml2::XMLDocument saveFile;
    saveFile.LoadFile("Assets/Saves/scores.xml");

    if (!saveFile.Error())
    {
        if (XMLElement *newPlayer = saveFile.NewElement("Player"))
        {
            newPlayer->SetAttribute("name", name.c_str());
            newPlayer->SetAttribute("score", score.c_str());

            if (XMLElement *scoreNode = saveFile.FirstChildElement())
            {
                bool inserted = false;

                if (auto scoreVal = scoreNode->FloatAttribute("score"))
                {
                    if (scoreVal < std::stof(score))
                    {
                        saveFile.InsertFirstChild(newPlayer);

                        inserted = true;
                    }
                }
                
                if (!inserted)
                {
                    XMLElement *prev = scoreNode;
                    scoreNode = scoreNode->NextSiblingElement();

                    while (scoreNode != NULL)
                    {
                        if (auto scoreVal = scoreNode->FloatAttribute("score"))
                        {
                            if (scoreVal < std::stof(score))
                            {
                                saveFile.InsertAfterChild(prev, newPlayer);
                                inserted = true;
                                break;
                            }
                        }
                        prev = scoreNode;
                        scoreNode = scoreNode->NextSiblingElement();
                    } 
                }
                if (!inserted)
                {
                    saveFile.InsertEndChild(newPlayer);
                }
            }
            // No scores
            else
            {
                saveFile.InsertEndChild(newPlayer);
            }
        }
    }
    else
    {
        LOG("Error opening Assets/Saves/scores.xml for saving.");
    }

    saveFile.SaveFile("Assets/Saves/scores.xml");
}