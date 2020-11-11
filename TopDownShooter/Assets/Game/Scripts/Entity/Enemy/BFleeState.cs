using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BFleeState : StateMachineBehaviour
{
    private EnemyController eControl = null;

    private float fleeTimer = 0.0f;
    private float timeToFlee = 0.0f;

    public string fleeExitTrigger = "Flee";

    [Tooltip("Flee a random time between x and y.")]
    public Vector2 fleeTime = new Vector2(1.0f, 5.0f);

    [Tooltip("Closer to target, more direct fleeing. After flee distance, wanders around.")]
    [Range(1.0f, 20.0f)]
    public float fleeDistance = 5.0f;

    [Tooltip("Effects how randomly flee will occur beyond flee distance.")]
    [Range(0.0f, 100.0f)]
    public float wanderRadius = 1.0f;

    public Transform fleeFrom = null;

    [Tooltip("Overrides target and use player's transform.")]
    public bool fleePlayer = false;


    // OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl == null)
        {
            eControl = animator.GetComponent<EnemyController>();
        }
        
        fleeTimer = 0.0f;

        timeToFlee = Random.Range(fleeTime.x, fleeTime.y);

        if (eControl != null)
        {
            // Safely initialize agent destination to current position
            eControl.movement.Move(eControl.trans.position, Vector3.zero);
        }
    }

    // OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (eControl != null)
        {
            if (fleePlayer && Player.Exists)
            {
                fleeFrom = Player.Instance.trans;
            }

            if (fleeFrom != null)
            {
                Vector3 dirFromTarget = eControl.trans.position - fleeFrom.position;
                dirFromTarget.y = 0.0f;

                // Will determine which position finding has more effect
                float percSafeDist = Mathf.Min(dirFromTarget.magnitude, fleeDistance) / fleeDistance;

                // Flee position
                dirFromTarget = dirFromTarget.normalized * fleeDistance * (1.0f - percSafeDist);

                // Wander position
                Vector3 dirWander = eControl.movement.agent.destination;
                Vector3 radialOffset = new Vector3();

                radialOffset.x = Random.Range(-1.0f, 1.0f);
                radialOffset.z = (1.0f - Mathf.Abs(radialOffset.x)) * ((Random.Range(0, 2) * 2) - 1);   // Remainder of unit vector * -1/1 for random signage
                radialOffset *= wanderRadius;

                dirWander += radialOffset;
                dirWander = dirWander - eControl.trans.position;    // Convert to direction from position
                dirWander *= percSafeDist;

                eControl.movement.Move(eControl.trans.position, dirFromTarget + dirWander);
            }

            fleeTimer += Time.deltaTime;

            if (fleeTimer >= timeToFlee)
            {
                animator.SetTrigger(fleeExitTrigger);
                eControl.movement.Stop();
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
