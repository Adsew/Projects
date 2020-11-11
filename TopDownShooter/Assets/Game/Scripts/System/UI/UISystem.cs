using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UISystem : Singleton<UISystem> {

    private List<Menu> menus = new List<Menu>();
    private List<Menu> activeMenus = new List<Menu>();

    private Menu loadingScreen = null;

    [SerializeField]
    private string loadingScreenID;

    /// <summary>
    /// Registers a new menu to the system.
    /// </summary>
    /// <param name="m">Menu to be added.</param>
    /// <param name="makeActive">Make the menu active when it is added.</param>
    public void AddMenu(Menu m, bool makeActive = false)
    {
        if (m.menuID == loadingScreenID)
        {
            loadingScreen = m;
        }
        else
        {
            for (int i = 0; i < menus.Count; i++)
            {
                if (menus[i].menuID == m.menuID)
                {
                    Debug.LogWarning("UISystem: Did not add " + m.menuID + ". Menu is already registered.");

                    return;
                }
            }

            menus.Add(m);
        }

        m.Disable();

        if (makeActive)
        {
            activeMenus.Add(m);
            m.Enable();
        }
    }

    /// <summary>
    /// Remove a menu from the system registry. Menu will no longer be togglable through the system.
    /// </summary>
    /// <param name="m">Menu to remove.</param>
    public void RemoveMenu(Menu m)
    {
        if (menus != null && activeMenus != null)
        {
            menus.Remove(m);
            activeMenus.Remove(m);
        }
    }

    /// <summary>
    /// Activate the given menu. Optionally disable all other menus.
    /// </summary>
    /// <param name="menuID">Menu to enable.</param>
    /// <param name="disableActiveMenus">Disable all other menus if true.</param>
    public void EnableMenu(string menuID, bool disableActiveMenus = false)
    {
        if (disableActiveMenus)
        {
            for (int j = 0; j < activeMenus.Count; j++)
            {
                activeMenus[j].Disable();
            }

            activeMenus.Clear();
        }

        if (!string.IsNullOrEmpty(menuID))
        {
            for (int i = 0; i < menus.Count; i++)
            {
                if (menus[i].menuID == menuID)
                {
                    menus[i].Enable();
                    activeMenus.Add(menus[i]);

                    return;
                }
            }

            Debug.LogWarning("UISystem: The given menu ID, " + menuID + ", to be activated is not registered.");
        }
        else
        {
            Debug.LogWarning("UISystem: Could not enable MenuID with no value.");
        }
    }

    /// <summary>
    /// Disable the specified menu ID.
    /// </summary>
    /// <param name="menuID">The menu to be disabled.</param>
    public void DisableMenu(string menuID)
    {
        for (int i = 0; i < activeMenus.Count; i++)
        {
            if (activeMenus[i].menuID == menuID)
            {
                activeMenus[i].Disable();
                activeMenus.RemoveAt(i);

                return;
            }
        }
    }

    /// <summary>
    /// Shows the loading screen over any current UI menus. This does not effect other menus.
    /// </summary>
    public void ShowLoadingScreen()
    {
        loadingScreen.Enable();
    }

    /// <summary>
    /// Hides the loading screen. This does not effect other menus.
    /// </summary>
    public void HideLoadingScreen()
    {
        loadingScreen.Disable();
    }
}
