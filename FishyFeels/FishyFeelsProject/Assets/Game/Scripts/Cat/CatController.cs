using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatController : Singleton<CatController> {

    public Rigidbody2D rb = null;

    public List<GameObject> platforms = new List<GameObject>();

    public GameObject standOn;
    public GameObject lastStandOn;
    public GameObject fish;

    public AnimMessager animMessanger = null;

    public float platformDisplacement;

    public bool jumpingDown = false;

    private void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update () {
        
        // Correct velocity to 0 to help with platform collisions
        if (rb != null && Mathf.Abs(rb.velocity.y) < 0.05f)
        {
            Vector2 v = rb.velocity;
            v.y = 0.0f;
            rb.velocity = v;
        }

        if (standOn)
        {
            platformDisplacement = (gameObject.transform.position.x - standOn.transform.position.x) / (standOn.GetComponent<SpriteRenderer>().size.x / 2f);
        }
        else
        {
            platformDisplacement = 0;
        }
        
        platforms.Remove(null); // Clean deleted platforms
	}
    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Fish")
        {
            gameObject.GetComponent<Animator>().SetBool("haveFish", true);
        }
        else if (((collision.gameObject != lastStandOn && jumpingDown) || !jumpingDown))
        {
            if (collision.gameObject.tag == ("Platform") || collision.gameObject.tag == ("Ground"))
            {
                gameObject.GetComponent<Animator>().SetBool("grounded", true);
                animMessanger.sendBoolMessage("grounded", true);

                standOn = collision.gameObject;

                jumpingDown = false;

                if (lastStandOn != null)
                {
                    PlatformEffector2D platEff = lastStandOn.GetComponent<PlatformEffector2D>();

                    if (platEff != null)
                    {
                        platEff.rotationalOffset = 0;
                    }
                }
            }
        }
    }
    private void OnCollisionExit2D(Collision2D collision)
    {
        if (collision.gameObject == fish)
        {
            gameObject.GetComponent<Animator>().SetBool("fishInSight", false);
        }
        else if (collision.gameObject.tag == ("Platform") || collision.gameObject.tag == ("Ground"))
        {
            gameObject.GetComponent<Animator>().SetBool("grounded", false);
            animMessanger.sendBoolMessage("grounded", false);

            if (standOn != null)
            {
                lastStandOn = standOn;
            }
            standOn = null;
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject == fish)
        {
            gameObject.GetComponent<Animator>().SetBool("fishInSight", true);
        }
        else if (collision.gameObject.tag == "Platform")
        {
            platforms.Add(collision.gameObject);
        }
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        if (platforms.Contains (collision.gameObject ))
        {
            platforms.Remove(collision.gameObject);
        }
    }
}
