using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TriggerBox : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
    private void OnTriggerEnter2D(Collider2D collision)
    {
        transform.parent.SendMessage("OnTriggerEnter2D", collision);
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        transform.parent.SendMessage("OnTriggerExit2D", collision);
    }
}
