using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FadeInElements : MonoBehaviour {

    public List<Image> elements = new List<Image>();

    public float fadeInTime = 2.0f;
    public float delay = 0.0f;

    public bool disableWhileDelayed = false;

    private void OnEnable()
    {
        StartCoroutine(FadeIn());
    }

    private IEnumerator FadeIn()
    {
        float timer = 0.0f;
        
        for (int i = 0; i < elements.Count; i++)
        {
            elements[i].color = new Color(elements[i].color.r, elements[i].color.g, elements[i].color.b, 0.0f);

            if (disableWhileDelayed)
            {
                elements[i].gameObject.SetActive(false);
            }
        }

        while (timer < delay)
        {
            timer += Time.deltaTime;

            yield return null;
        }

        timer = 0.0f;

        if (disableWhileDelayed)
        {
            for (int i = 0; i < elements.Count; i++)
            {
                elements[i].gameObject.SetActive(true);
            }
        }

        while (timer <= fadeInTime)
        {
            for (int i = 0; i < elements.Count; i++)
            {
                elements[i].color = new Color(elements[i].color.r, elements[i].color.g, elements[i].color.b, timer / fadeInTime);
            }

            timer += Time.deltaTime;

            yield return null;
        }

        for (int i = 0; i < elements.Count; i++)
        {
            elements[i].color = new Color(elements[i].color.r, elements[i].color.g, elements[i].color.b, 1.0f);
        }
    }
}
