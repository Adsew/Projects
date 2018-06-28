using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatFallingBehavior : StateMachineBehaviour {
    
    //OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (animator.GetBool("grounded"))
        {
            animator.SetTrigger("land");
        }
    }

    // OnStateExit is called when a transition ends and the state machine finishes evaluating this state
    override public void OnStateExit(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        CatController.Instance.animMessanger.sendTriggerMessage("landed");
    }
}
