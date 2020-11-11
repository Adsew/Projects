using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuSwitcher : MonoBehaviour {

    [Tooltip("Menu to switch to.")]
    public string menuName;
    [Tooltip("True if this menu is to appear, false if this menu is to disappear.")]
    public bool enableThisMenu = true;
    [Tooltip("Enable to disable all other menus on switch.")]
    public bool disableOtherMenus = false;

    public enum Mode
    {
        Start,
        FunctionCall
    }
    [Tooltip("Switch menus on Start or when the function is called.")]
    public Mode mode;

    // Use this for initialization
    void Start ()
    {
        if (mode == Mode.Start)
        {
            SwitchMenu();
        }
	}

    public void SwitchMenu()
    {
        if (UISystem.Exists)
        {
            if (enableThisMenu)
            {
                UISystem.Instance.EnableMenu(menuName, disableOtherMenus);
            }
            else
            {
                UISystem.Instance.DisableMenu(menuName);
            }
        }
    }
}
