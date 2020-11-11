using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Scorable : MonoBehaviour {

    [Tooltip("Score this unity is worth.")]
    public int score = 1;

    /// <summary>
    /// Tells game state system to earn the score of this unit.
    /// Score does not perminently apply until next round.
    /// </summary>
    public void AwardScore()
    {
        if (ScoreKeeper.Exists)
        {
            ScoreKeeper.Instance.AddScore(score);
        }
    }
}
