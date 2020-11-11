using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BRangedState : StateMachineBehaviour
{
    private EnemyController eControl = null;

    public Transform target = null;

    [Tooltip("Overrides target and use player's transform.")]
    public bool targetPlayer = false;

    [Tooltip("Strafe to new position if closer than x, move closer if farther than y.")]
    public Vector2 attackRange = new Vector2(3.0f, 5.0f);

    public string chaseTrigger = "Chase";
    public string fleeTrigger = "Flee";

    // OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl == null)
        {
            eControl = animator.GetComponent<EnemyController>();
        }

        if (eControl != null)
        {
            eControl.movement.rb.velocity = Vector3.zero;
        }
    }

    // OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl != null)
        {
            eControl.movement.rb.velocity = Vector3.zero;

            if (targetPlayer && Player.Exists)
            {
                target = Player.Instance.trans;
            }

            if (target != null)
            {
                float distance = Vector3.Magnitude(target.position - eControl.trans.position);

                if (distance >= attackRange.y)
                {
                    animator.SetTrigger(chaseTrigger);
                }
                else if (distance <= attackRange.x)
                {
                    animator.SetTrigger(fleeTrigger);
                }

                eControl.trans.LookAt(target);
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
