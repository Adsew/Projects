using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IsPlatform : MonoBehaviour {

    private SpriteRenderer platRenderer = null; // Used to scale with tiling
    private PlatformEffector2D platEffect = null;

    private float fixTimer = 0.0f;
    private float platFixTime = 3.0f;

    private bool fixTimerStarted = false;

    private int collidingPlatforms = 0; // Ignores growth if a platform is in the way

    public GameObject spawnAnimationObject = null;

    public Vector2Int tilePosition = new Vector2Int();

    public float growSpeed = 0.01f;

    public float minCutoff = 0.99f;
    public float maxCutoff = 5.0f;

    // Timing delay variables
    private bool growDelayComplete = false;
    private float growDelayTimer = 0.0f;

    public float growDelay = 1.0f;  // Time til can start growing
    bool gaveFish;
    void Start() {
        gaveFish = false;
        platRenderer = GetComponent<SpriteRenderer>();
        platEffect = GetComponent<PlatformEffector2D>();
    }

    private void Update() {
        
        // Fix platform effector after time
        if (platEffect != null && platEffect.rotationalOffset > 0.0f)
        {
            if (fixTimerStarted)
            {
                fixTimer += Time.deltaTime;

                if (fixTimer >= platFixTime)
                {
                    platEffect.rotationalOffset = 0.0f;

                    fixTimerStarted = false;
                }
            }
            else
            {
                fixTimerStarted = true;
                fixTimer = 0.0f;
            }
        }
        else
        {
            fixTimerStarted = false;
            fixTimer = 0.0f;
        }

        if (!growDelayComplete) {

            growDelayTimer += Time.deltaTime;

            if (growDelayTimer >= growDelay) {

                growDelayComplete = true;
            }
        }
    }

    private void OnCollisionEnter2D(Collision2D collision) {
        
        if (collision.gameObject.GetComponent<IsPlatform>()) {

            collidingPlatforms++;
        }
    }

    private void OnCollisionExit2D(Collision2D collision) {

        if (collision.gameObject.GetComponent<IsPlatform>()) {

            collidingPlatforms--;
        }
    }

    // Grow x size of platform
    public void grow() {

        if (growDelayComplete && platRenderer != null && collidingPlatforms <= 0) {

            Vector2 scale = platRenderer.size;

            if (scale.x <= maxCutoff) {

                scale.x += growSpeed;

                platRenderer.size = scale;
            }
            else
            {
                if (!gaveFish)
                {
                    PlatformMaker.Instance.GiveFish(gameObject);
                    gaveFish = true;
                }
            }
        }
    }

    // Shrink x size of platform, return false when too small to shrink further
    public bool shrink() {

        if (platRenderer != null) {

            Vector3 scale = platRenderer.size;

            if (scale.x > minCutoff) {

                scale.x -= growSpeed;

                platRenderer.size = scale;

                return true;
            }
            
        }

        return false;
    }
}
