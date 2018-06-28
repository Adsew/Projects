using UnityEngine;
using System.Collections;

public class FloorPadInputManager : MonoBehaviour
{
	void Start ()
	{
		FloorPadInput.Initialize ();
	}

	void Update ()
	{
		FloorPadInput.Update ();
	}

	void OnApplicationQuit ()
	{
		FloorPadInput.OnQuit ();
	}
}
