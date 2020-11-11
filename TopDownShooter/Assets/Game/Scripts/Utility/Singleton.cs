using UnityEngine;

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour {

    private static T _instance;

    /// <summary>
    /// Return true if an instance of this singletone exists.
    /// </summary>
    public static bool Exists
    {
        get
        {
            if (_instance == null)
            {
                _instance = FindObjectOfType<T>();

                if (_instance == null)
                {
                    return false;
                }
            }

            return true;
        }
    }

    /// <summary>
    /// Retrieve the instance of the singleton.
    /// </summary>
    public static T Instance {

        get
        {
            if (_instance == null)
            {
                _instance = FindObjectOfType<T>();

                //if (_instance == null)
                //{
                    //Debug.LogWarning("Singleton with the name '" + typeof(T).ToString() + "' was not found is scene! It either doesn't exist or has not yet been loaded.");
                //}
            }

            return _instance;
        }
    }
}
