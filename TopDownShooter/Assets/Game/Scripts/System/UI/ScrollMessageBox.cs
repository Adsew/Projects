using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScrollMessageBox : MessageBox {

    private List<Text> liveText = new List<Text>();
    
    [Range(0.0f, 1000.0f)]
    public float textDuration;
    public float scrollSpeed;

    public bool fadeTextOverDuration = false;
    
    public override void AppendMessage(string message)
    {
        GameObject newMessage = new GameObject();

        newMessage.transform.parent = transform;

        StartCoroutine(ScrollText(newMessage.AddComponent<Text>()));
    }

    public override void ClearMessages()
    {
        for (int i = 0; i < liveText.Count; i++)
        {
            Destroy(liveText[i]);
        }

        liveText.Clear();
    }

    private IEnumerator ScrollText(Text text)
    {
        Transform trans = text.transform;
        float timer = 0.0f;

        while (timer < textDuration)
        {
            yield return null;

            // Safety if game object is deleted and this runs another cycle
            if (text == null)
            {
                yield break;
            }

            trans.position = trans.position + trans.up * scrollSpeed;

            if (fadeTextOverDuration)
            {
                text.color = new Color(text.color.r, text.color.g, text.color.b, 1.0f - (timer / textDuration));
            }

            timer += Time.deltaTime;
        }

        Destroy(text.gameObject);
    }
}
