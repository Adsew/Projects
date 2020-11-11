using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StaticMessageBox : MessageBox {

    [SerializeField]
    private Text textBox = null;

    private void Awake()
    {
        if (textBox == null)
        {
            textBox = GetComponent<Text>();
        }
    }

    public override void AppendMessage(string message)
    {
        if (textBox != null)
        {
            if (style == AppendStyle.Top)
            {
                textBox.text = message + "\n" + textBox.text;
            }
            else
            {
                textBox.text = textBox.text + "\n" + message;
            }
        }
    }

    public override void ClearMessages()
    {
        if (textBox != null)
        {
            textBox.text = "";
        }
    }
}
