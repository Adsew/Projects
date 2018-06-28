using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SingleFishMovement : MonoBehaviour {
    Vector2 target;
    public float spd;
    public void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.GetComponentInChildren<CatController>())
        {
            Destroy(gameObject);
        }
    }
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        transform.position = Vector2.MoveTowards(transform.position, target, spd*Time.deltaTime);
	}
    public void SetTarget(Vector2 t)
    {
        target = t;
    }
}
