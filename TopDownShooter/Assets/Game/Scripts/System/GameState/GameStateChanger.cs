using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameStateChanger : MonoBehaviour {

    public enum State
    {
        Start,
        Pause,
        Restart,
        Continue,
        End
    }
    public State game;

    public enum Mode
    {
        Start,
        FunctionCall
    }
    public Mode mode;
    
	// Use this for initialization
	void Start ()
    {
		if (mode == Mode.Start)
        {
            ChangeState();
        }
	}

    public void ChangeState()
    {
        if (game == State.Start)
        {
            GameStateSystem.Instance.StartGame();
        }
        else if (game == State.Pause)
        {
            GameStateSystem.Instance.PauseGame();
        }
        else if (game == State.Restart)
        {
            GameStateSystem.Instance.RestartGame();
        }
        else if (game == State.Continue)
        {
            GameStateSystem.Instance.ContinueGame();
        }
        else if (game == State.End)
        {
            GameStateSystem.Instance.EndGame();
        }
    }
}
