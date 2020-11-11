using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public abstract class MessageBox : MonoBehaviour {

    public enum AppendStyle
    {
        Top,
        Bottom,
        Left,
        Right
    }
    public AppendStyle style = AppendStyle.Top;

    public abstract void AppendMessage(string message);

    public abstract void ClearMessages();
}
