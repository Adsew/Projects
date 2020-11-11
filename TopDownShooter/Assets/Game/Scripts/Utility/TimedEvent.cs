using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class TimedEvent : MonoBehaviour {

    private float timer = 0.0f;
    public float time = 1.0f;

    public bool started = false;

    public UnityEvent OnEvent = new UnityEvent();
    
	private void Update () {
		
        if (started)
        {
            timer += Time.deltaTime;

            if (timer >= time)
            {
                started = false;

                timer = 0.0f;

                OnEvent.Invoke();
            }
        }
	}

    /// <summary>
    /// If not already, begins the timer to activate the timed event.
    /// </summary>
    public void StartTimer()
    {
        started = true;
    }

    /// <summary>
    /// Stop timer from counting down. Down not reset timer.
    /// </summary>
    public void StopTimer()
    {
        started = false;
    }

    /// <summary>
    /// Reset time on the timer to 0. Does not stop timer.
    /// </summary>
    public void ResetTimer()
    {
        timer = 0.0f;
    }
}
