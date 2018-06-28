using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatJumpingBehaviour : StateMachineBehaviour {

    public float velocityMultiplier = 8.0f;

    //OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        if (animator.GetBool("fishInSight"))
        {
            JumpTo(CatController.Instance.fish);
        }
        else
        {
            ChooseAPlatformToJump();
        }
    }

    ////OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    //override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    //{

    //}

    ////OnStateExit is called when a transition ends and the state machine finishes evaluating this state
    //override public void OnStateExit(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    //{

    //}

    // Determine which platform to jump to
    void ChooseAPlatformToJump()
    {
        Vector2 toFish = CatController.Instance.fish.transform.position - CatController.Instance.gameObject.transform.position;

        float smallestAngle = 359;
        GameObject bestP = null;

        foreach (GameObject p in CatController.Instance.platforms)
        {
            if (p != null && p != CatController.Instance.standOn && p.gameObject.tag != "Ground")
            {
                Vector2 toP = p.transform.position - CatController.Instance.gameObject.transform.position;
                float angle = Vector2.Angle(toFish.normalized, toP.normalized);

                if (angle < smallestAngle)
                {
                    smallestAngle = angle;
                    bestP = p;
                }
            }
        }

        JumpTo(bestP);
    }

    // Calculate tragectory path
    void JumpTo(GameObject target)
    {
        if (target != null)
        {
            float x = target.transform.position.x - CatController.Instance.gameObject.transform.position.x;
            float y = target.transform.position.y - CatController.Instance.gameObject.transform.position.y;

            float velocity = 0.0f;

            // Use lower velocity when going down
            if (y >= 0.0f)
            {
                velocity = Mathf.Clamp(Vector2.SqrMagnitude(new Vector2(x, y)) * 2.0f, 6.0f, 8.0f);
            }
            else
            {
                velocity = Mathf.Clamp(Vector2.SqrMagnitude(new Vector2(x, y)) * 2.0f, 3.0f, 5.0f);

                CatController.Instance.jumpingDown = true;
            }
            
            float gravity = -Physics2D.gravity.y;
            
            float rootedVal = Mathf.Sqrt(
                Mathf.Pow(velocity, 4.0f)
                - gravity * ( gravity * Mathf.Pow(x, 2.0f) + 2.0f * (y * Mathf.Pow(velocity, 2.0f)) )
            );
            
            float angle = Mathf.Rad2Deg * Mathf.Atan((Mathf.Pow(velocity, 2.0f) + rootedVal) / (gravity * x));
            
            Debug.Log(angle);

            // Rotate the cat to aim in the direction of the jump, then fire and re-adjust rotation to normal
            if ((angle >= 0.0f))
            {
                CatController.Instance.transform.right = Vector3.right;

                CatController.Instance.transform.Rotate(Vector3.forward, Mathf.Abs(angle));
                CatController.Instance.gameObject.GetComponent<Rigidbody2D>().velocity = CatController.Instance.transform.right * velocity;

                CatController.Instance.transform.right = Vector3.right;
            }
            else
            {
                CatController.Instance.transform.right = Vector3.left;

                CatController.Instance.transform.Rotate(Vector3.forward, Mathf.Abs(angle));
                CatController.Instance.gameObject.GetComponent<Rigidbody2D>().velocity = CatController.Instance.transform.right * velocity;

                CatController.Instance.transform.right = Vector3.left;
            }
            
            CatController.Instance.gameObject.GetComponent<Animator>().SetTrigger("falling");

            if (CatController.Instance.standOn != null)
            {
                PlatformEffector2D platEff = CatController.Instance.standOn.GetComponent<PlatformEffector2D>();

                if (platEff != null)
                {
                    platEff.rotationalOffset = 180;
                }
            }
            
            CatController.Instance.animMessanger.sendTriggerMessage("jumping");
        }
        else
        {
            CatController.Instance.gameObject.GetComponent<Animator>().SetTrigger("walk");
        }
    }
}
