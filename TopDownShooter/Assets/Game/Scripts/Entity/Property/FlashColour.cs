using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlashColour : MonoBehaviour {

    private Renderer myMesh;

    private List<Color> originalColour = new List<Color>();

    public Color flashColour;

    [Range(0.1f, 5.0f)]
    public float duration = 1.0f;

    private void Start()
    {
        myMesh = GetComponent<Renderer>();
        
        if (myMesh != null)
        {
            for (int i = 0; i < myMesh.materials.Length; i++)
            {
                originalColour.Add(myMesh.materials[i].color);
            }
        }
    }

    public void BeginFlash()
    {
        StartCoroutine(Flash());
    }

    private IEnumerator Flash()
    {
        if (myMesh != null)
        {
            float timer = 0.0f;

            while (timer < duration)
            {
                timer += Time.deltaTime;

                if (timer < (duration / 2.0f))
                {
                    float lerp = timer / (duration / 2.0f);

                    for (int i = 0; i < myMesh.materials.Length; i++)
                    {
                        myMesh.materials[i].color = Color.Lerp(originalColour[i], flashColour, lerp);
                    }
                }
                else
                {
                    float lerp = timer / (duration / 2.0f) - 1.0f;

                    for (int i = 0; i < myMesh.materials.Length; i++)
                    {
                        myMesh.materials[i].color = Color.Lerp(flashColour, originalColour[i], lerp);
                    }
                }
                yield return null;
            }

            for (int i = 0; i < myMesh.materials.Length; i++)
            {
                myMesh.materials[i].color = originalColour[i];
            }
        }
    }
}
