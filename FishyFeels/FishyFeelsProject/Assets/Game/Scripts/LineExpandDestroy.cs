using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LineExpandDestroy : MonoBehaviour {

    private SpriteRenderer sprite = null;

    public float timeTilDeath = 2.0f;

    private float timer = 0.0f;

	// Use this for initialization
	void Start () {

        sprite = GetComponent<SpriteRenderer>();
	}
	
	// Update is called once per frame
	void Update () {
		
        if (sprite != null)
        {
            timer += Time.deltaTime;
            sprite.size = new Vector2(sprite.size.x + (Time.deltaTime * 4.0f), sprite.size.y);

            if (timer > timeTilDeath)
            {
                Destroy(gameObject);
            }
        }
	}
}
