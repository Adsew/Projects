using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BChaseState : StateMachineBehaviour
{
    private EnemyController eControl = null;

    public Transform target = null;

    [Tooltip("Overrides target and use player's transform.")]
    public bool targetPlayer = false;

    [Tooltip("When distance to target is less than this, enter attack.")]
    public float attackRange = 2.0f;

    public string attackTrigger = "Attack";

    // OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl == null)
        {
            eControl = animator.GetComponent<EnemyController>();
        }
    }

    // OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl != null)
        {
            if (targetPlayer && Player.Exists)
            {
                target = Player.Instance.trans;
            }

            if (target != null)
            {
                if (Vector3.Magnitude(target.position - eControl.trans.position) <= attackRange)
                {
                    eControl.movement.Stop();
                    animator.SetTrigger(attackTrigger);
                }
                else
                {
                    eControl.movement.Move(target.position, Vector3.zero);
                }
            }
        }
    }

    // OnStateExit is called when a transition ends and the state machine finishes evaluating this state
    //override public void OnStateExit(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    //{

    //}

    // OnStateMove is called right after Animator.OnAnimatorMove(). Code that processes and affects root motion should be implemented here
    //override public void OnStateMove(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
    //
    //}

    // OnStateIK is called right after Animator.OnAnimatorIK(). Code that sets up animation IK (inverse kinematics) should be implemented here.
    //override public void OnStateIK(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
    //
    //}
}
