using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class GameStateSystem : Singleton<GameStateSystem> {

    private Transform startPosition;
    
    private float originalFixedTime;

    public GameObject player { get; private set; }

    public bool playing { get; private set; }

    public GameObject playerPrefab;
    
    public string pauseMenuName;

    public UnityEvent OnRespawn = new UnityEvent();
    public UnityEvent OnContinue = new UnityEvent();
    public UnityEvent OnEndGame = new UnityEvent();

    public bool paused { get; private set; }

    [Range(1.0f, 3600.0f)]
    public float maxLevelTime = 60.0f;
    public float levelTimer { get; private set; }
    public bool timerRunning = false;

    public int difficulty = 1;

    private void Start()
    {
        originalFixedTime = Time.fixedDeltaTime;
    }

    private void Update()
    {
        if (levelTimer > 0.0f && playing && timerRunning)
        {
            levelTimer -= Time.deltaTime;
        }
    }

    /// <summary>
    /// Calling this function spawns the players and sets any initial settings to begin a level.
    /// Only call this once the level is loaded.
    /// </summary>
    public void StartGame()
    {
        if (startPosition != null && playerPrefab != null)
        {
            player = Instantiate(playerPrefab, startPosition.position, startPosition.rotation);
            
            levelTimer = maxLevelTime;

            difficulty = 1;
        }
        else
        {
            if (startPosition == null)
            {
                Debug.LogError("GameStateSystem: No start position found on game start. Couldn't spawn player.");
            }
            if (playerPrefab == null)
            {
                Debug.LogError("GameStateSystem: No player prefab is set.");
            }
        }

        playing = true;
        timerRunning = true;
        
        Time.timeScale = 1.0f;

        OnRespawn.Invoke();
    }

    /// <summary>
    /// Toggles the game between pause mode and play mode.
    /// </summary>
    public void PauseGame()
    {
        if (playing)
        {
            if (!paused)
            {
                UISystem.Instance.EnableMenu(pauseMenuName, true);
                Time.timeScale = 0.0f;
                Time.fixedDeltaTime = 0.0f;

                paused = true;
            }
            else if (paused)
            {
                UISystem.Instance.DisableMenu(pauseMenuName);
                Time.timeScale = 1.0f;
                Time.fixedDeltaTime = originalFixedTime;

                paused = false;
            }
        }
    }

    /// <summary>
    /// Restarts the current level.
    /// </summary>
    public void RestartGame()
    {
        Destroy(player);
        player = Instantiate(playerPrefab, startPosition.position, startPosition.rotation);

        levelTimer = maxLevelTime;

        if (paused)
        {
            PauseGame();
        }

        Time.timeScale = 1.0f;

        timerRunning = true;
        
        OnRespawn.Invoke();
    }

    /// <summary>
    /// Continues game to the next level.
    /// </summary>
    public void ContinueGame()
    {
        player.transform.SetPositionAndRotation(startPosition.position, startPosition.rotation);

        levelTimer = maxLevelTime;

        difficulty++;

        if (paused)
        {
            PauseGame();
        }

        Time.timeScale = 1.0f;

        timerRunning = true;
        
        OnContinue.Invoke();
    }

    /// <summary>
    /// Ends the current game by removing the player and resetting any game state values.
    /// </summary>
    public void EndGame()
    {
        Destroy(player);
        player = null;

        difficulty = 1;

        Time.timeScale = 1.0f;
        
        if (paused)
        {
            PauseGame();
        }

        playing = false;
        timerRunning = false;

        OnEndGame.Invoke();
    }

    /// <summary>
    /// Start or stop the level timer while the game is playing.
    /// </summary>
    /// <param name="pause"></param>
    public void PauseLevelTimer(bool pause)
    {
        if (playing)
        {
            timerRunning = pause;
        }
    }

    

    /// <summary>
    /// Set the initial spawn position of the player in the current level.
    /// </summary>
    /// <param name="position">Position player will spawn.</param>
    public void SetStartPosition(Transform position)
    {
        startPosition = position;
    }
}
