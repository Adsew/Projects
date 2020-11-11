using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreKeeper : Singleton<ScoreKeeper> {

    public int score { get; private set; }
    public int roundScore { get; private set; }

    private void Start()
    {
        score = 0;
        roundScore = 0;

        GameStateSystem.Instance.OnContinue.AddListener(AddRoundScoreToScore);
        GameStateSystem.Instance.OnRespawn.AddListener(ResetRoundScore);
        GameStateSystem.Instance.OnEndGame.AddListener(ResetScore);
    }

    public void AddRoundScoreToScore()
    {
        score += roundScore;
        roundScore = 0;
    }

    public void ResetRoundScore()
    {
        roundScore = 0;
    }

    public void ResetScore()
    {
        score = 0;
        roundScore = 0;
    }

    /// <summary>
    /// Adds score to the current round's score.
    /// </summary>
    /// <param name="scoreToAdd">The amount to add.</param>
    public void AddScore(int scoreToAdd)
    {
        roundScore += scoreToAdd;
    }
}
