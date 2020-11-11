using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class Menu : MonoBehaviour {

    [Tooltip("Used to identify this menu by name.")]
    public string menuID;

    [Tooltip("When the scene first loads, this menu will activate.")]
    public bool activateOnStart = false;

    [Tooltip("First UI element selected when enabled.")]
    public Selectable initialFocus;

    void Start()
    {
        UISystem.Instance.AddMenu(this, activateOnStart);
    }

    public void Enable()
    {
        gameObject.SetActive(true);

        if (initialFocus != null)
        {
            initialFocus.Select();
            initialFocus.OnSelect(null);
        }
    }

    public void Disable()
    {
        gameObject.SetActive(false);
    }
}
