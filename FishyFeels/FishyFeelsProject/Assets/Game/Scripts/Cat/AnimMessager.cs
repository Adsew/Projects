using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class AnimMessager : MonoBehaviour {

    private Animator anim = null;

	// Use this for initialization
	void Start () {

        anim = GetComponent<Animator>();

        CatController.Instance.animMessanger = this;
	}
	
    public void sendTriggerMessage(string varName) {

        if (anim != null) {

            anim.SetTrigger(varName);
        }
    }

    public void sendFloatMessage(string varName, float val) {

        if (anim != null) {

            anim.SetFloat(varName, val);
        }
    }

    public void sendIntMessage(string varName, int val) {

        if (anim != null) {

            anim.SetInteger(varName, val);
        }
    }

    public void sendBoolMessage(string varName, bool val) {

        if (anim != null) {

            anim.SetBool(varName, val);
        }
    }
}
