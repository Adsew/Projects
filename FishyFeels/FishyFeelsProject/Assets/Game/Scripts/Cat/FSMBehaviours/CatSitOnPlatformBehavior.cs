using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatSitOnPlatformBehavior : StateMachineBehaviour {

    private GameObject platform;

    private float timer;

    public GameObject viewLine = null;
    
    public GameObject flower;

    [Range(0, 100)]
    public int walkChance = 30;

    public float actionWaitTime;
    public float jumpVelo;
    public float retreatingSpd;
    public bool retreating;

    // OnStateEnter is called when a transition starts and the state machine starts to evaluate this state
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        timer = 0;
        platform = CatController.Instance.standOn;
        retreating = false;
    }
    

    // OnStateUpdate is called on each Update frame between OnStateEnter and OnStateExit callbacks
    override public void OnStateUpdate(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
        timer += Time.deltaTime;

        if (animator.GetBool("haveFish"))
        {
            CatController.Instance.animMessanger.sendTriggerMessage("party");

            GameObject newFlower = Instantiate(flower, animator.gameObject.transform.position, Quaternion.identity) as GameObject;
            newFlower.transform.parent = animator.gameObject.transform;

            timer = 0.0f;
            actionWaitTime = 3.0f;
            animator.SetBool("haveFish", false);
        }
        else if (animator.GetBool("fishInSight") && animator.GetBool("grounded"))
        {
            animator.SetTrigger("jumping");
        }
        // Ground disapeared
        else if (platform == null)
        {
            animator.SetTrigger("falling");
            CatController.Instance.animMessanger.sendTriggerMessage("falling");
        }
        else
        {
            if (Mathf.Abs(animator.gameObject.transform.position.x - platform.transform.position.x) / (platform.GetComponent<SpriteRenderer>().size.x / 2f) > 0.8f)
            {
                //retreating = true;
            }

            if (retreating)
            {
                animator.gameObject.GetComponent<Rigidbody2D>().velocity = new Vector2(-Mathf.Sign(animator.gameObject.transform.position.x - platform.transform.position.x) * retreatingSpd, 0);
                if (Mathf.Abs(animator.gameObject.transform.position.x - platform.transform.position.x) / (platform.GetComponent<SpriteRenderer>().size.x / 2f) < 0.4f)
                {
                    animator.gameObject.GetComponent<Rigidbody2D>().velocity = Vector2.zero;
                    retreating = false;
                }
            }
            else if (timer > actionWaitTime)
            {
                actionWaitTime = Random.Range(1.0f, 3.0f);
                timer = 0;

                int chance = Random.Range(0, 100);

                // Always leave a chance to walk instead of taking action
                if (chance < walkChance)
                {
                    if (!retreating)
                    {
                        animator.SetTrigger("walk");
                    }
                }
                else
                {
                    // Stare longingly at fish.
                    // Set action time should be longer than beg animation
                    if (CatController.Instance.platforms.Count == 0)
                    {
                        actionWaitTime = 3.0f;

                        CatController.Instance.animMessanger.sendTriggerMessage("stare");

                        // Rotate towards fish
                        if (CatController.Instance.fish.transform.position.x - CatController.Instance.transform.position.x > 0.0f)
                        {
                            CatController.Instance.transform.right = Vector3.right;
                        }
                        else
                        {
                            CatController.Instance.transform.right = Vector3.left;
                        }

                        if (viewLine != null)
                        {
                            GameObject line = Instantiate(viewLine);

                            line.transform.position = CatController.Instance.transform.position + CatController.Instance.transform.right / 2.0f + CatController.Instance.transform.up / 2.0f;

                            line.transform.right = (CatController.Instance.transform.position - CatController.Instance.fish.transform.position).normalized;
                        }
                    }
                    if (CatController.Instance.platforms.Count > 1)
                    {
                        animator.SetTrigger("jumping");
                    }
                }
            }
        }
    }
}
