using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bird : MonoBehaviour {
    Vector2 fishTarget;
    public GameObject fish;

    public bool going;
    Vector3 initial;

    public float spd;
    bool givenFish;
	// Use this for initialization
	void Start () {
        initial = transform.position;
        givenFish = false;
	}
	
	// Update is called once per frame
	void Update () {
		if (going )
        {
            transform.position = Vector3.MoveTowards(transform.position, initial + new Vector3(20, 0, 0), spd*Time.deltaTime);
            if (transform.position .x > fishTarget.x && !givenFish )
            {
                Debug.Log("Spawn fish");
                GameObject newFish = Instantiate(fish, transform.position, Quaternion.identity) as GameObject;
                newFish.SendMessage("SetTarget", fishTarget);
                givenFish = true;
            }
            if (transform.position == initial + new Vector3(20, 0, 0))
            {
                going = false;
                givenFish = false;
                transform.position = initial;
                
            }
        }
	}
    public void GiveFish(Vector2 target)
    {
        if (!going)
        {
            going = true;
            fishTarget = target;
        }
       
    }
   
}
