using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

[InitializeOnLoad]
public class SplineTool {
    
    private static List<GameObject> splines;   // All splines in the scenes

    private static GameObject workingSpline;


    // Called on unity launch
    // Done to get already made splines on opening a project
    static SplineTool() {

        GetAllSplinesInScene();
    }

    // Gets all splines in the scene for further use
    // Used only as a safety fallback after initial use on startup
    [MenuItem("Custom Tools/Spline/Refresh Active Splines")]
    private static void GetAllSplinesInScene() {

        Component[] inSceneSplines = GameObject.FindObjectsOfType<Spline>();

        splines = new List<GameObject>();   // Gaurentee no doubling splines in list by reseting it

        foreach (Component s in inSceneSplines) {

            splines.Add(s.gameObject);
        }

        // Change working object to something usable if any exist
        if (workingSpline == null && splines.Count > 0) {

            workingSpline = splines[0];
        }
    }

    // Creates a new ,empty spline
    [MenuItem("Custom Tools/Spline/New Spline")]
    public static void CreateNewSpline() {

        if (splines == null) {

            GetAllSplinesInScene();
        }

        GameObject emptySpline = new GameObject("New Spline " + splines.Count);

        if (emptySpline != null) {

            emptySpline.AddComponent<Spline>();

            splines.Add(emptySpline);

            workingSpline = emptySpline;
        }
    }

    // Add point to current working spline
    [MenuItem("Custom Tools/Spline/Add Point")]
    public static void AddSplinePoint() {

        if (workingSpline != null) {

            GameObject newPoint = GameObject.CreatePrimitive(PrimitiveType.Sphere);

            // Add the new point as child of spline and add to spline's list
            if (newPoint != null) {

                Component.DestroyImmediate(newPoint.GetComponent<SphereCollider>());

                Spline sp = workingSpline.GetComponent<Spline>();

                if (sp != null) {

                    newPoint.name = "Spline Point";
                    newPoint.transform.localScale = new Vector3(0.2f, 0.2f, 0.2f);
                    newPoint.transform.position = workingSpline.transform.position;
                    newPoint.transform.parent = workingSpline.transform;

                    if (sp.controlPoints == null) {

                        sp.controlPoints = new List<GameObject>();
                    }

                    newPoint.name = "Point " + sp.controlPoints.Count;
                    sp.controlPoints.Add(newPoint);
                }
                else {

                    Debug.Log("Can't add point to spline. No spline selected.");

                    GameObject.Destroy(newPoint);
                }
            }
        }
        else {

            Debug.Log("No spline selected. Select spline before adding a point.");
        }
    }

    // Changes the spline that functions will operate on
    [MenuItem("CONTEXT/Spline/Select As Current Spline")]
    public static void SelectCurrentSpline(MenuCommand mc) {

        Spline selected = mc.context as Spline;

        if (selected != null) {

            // A spline exists that is not managed, so add it
            if (!splines.Contains(selected.gameObject)) {

                splines.Add(selected.gameObject);
            }

            workingSpline = selected.gameObject;
        }
    }

    // Add point to the right-clicked spline
    [MenuItem("CONTEXT/Spline/Add Point")]
    public static void AddSplinePointContext(MenuCommand mc) {

        SelectCurrentSpline(mc);
        AddSplinePoint();
    }

    [MenuItem("CONTEXT/Spline/Save Spline")]
    public static void SaveSplineContext(MenuCommand mc) {

        SelectCurrentSpline(mc);
        SaveCurrentSpline();
    }

    // Save all splines to an xml file
    [MenuItem("Custom Tools/Spline/Save All Splines")]
    public static void SaveAllSplines() {

        SplineFileManager fileMgr = SplineFileManager.CreateInstance<SplineFileManager>();

        fileMgr.Save(splines);
    }

    // Save current spline to an xml file
    [MenuItem("Custom Tools/Spline/Save Current Spline")]
    public static void SaveCurrentSpline() {

        List<GameObject> singleSpline = new List<GameObject>();
        singleSpline.Add(workingSpline);

        SplineFileManager fileMgr = SplineFileManager.CreateInstance<SplineFileManager>();

        fileMgr.Save(singleSpline);
    }

    [MenuItem("Custom Tools/Spline/Load from File")]
    public static void LoadSplineFile() {

        SplineFileManager fileMgr = SplineFileManager.CreateInstance<SplineFileManager>();

        fileMgr.Load();
    }

    [MenuItem("Custom Tools/Spline/Help")]
    public static void HelpDisplay() {

        HelpPrompt help = HelpPrompt.CreateInstance<HelpPrompt>();

        help.Initialize();
    }


    /***** Nested Classes for Editor Save/Load and Serializing *****/


    // Single point in a spline
    [System.Serializable]
    public class SplinePointSerial {

        [XmlAttribute("Name")]
        public string name;

        [XmlAttribute("PosX")]
        public float x;

        [XmlAttribute("PosY")]
        public float y;

        [XmlAttribute("PosZ")]
        public float z;
    }

    // Properties of a spline
    [System.Serializable]
    public class SplineSerial {

        [XmlArray("Points"), XmlArrayItem("Point")]
        public SplinePointSerial[] points;
        
        [XmlAttribute("Name")]
        public string name;

        [XmlAttribute("Debug")]
        public bool debug;

        [XmlAttribute("Loop")]
        public bool loop;

        [XmlAttribute("Mode")]
        public Spline.GameModes gameMode;

        [XmlAttribute("Type")]
        public Spline.PlayBackType playType;

        [XmlAttribute("Speed")]
        public float dt;
    }

    // Root object to allow saving list to XML file
    // Cant add list to other class due to "cant serialize transform" error
    [XmlRoot("SplineSave")]
    public class SplineXmlObject {

        [XmlArray("Splines"), XmlArrayItem("Spline")]
        public SplineSerial[] splines;
    }

    // Class that can get a line of text from the user using a popup window
    // For saving and loading splines

    public class SplineFileManager : EditorWindow {

        public string fileName = "File.xml";
        public string errorMsg = "";

        public bool[] selected = null;
        public List<GameObject> splineList = null;

        public SplineSerial[] splinesToSave;

        private bool saveStarted = false;
        private bool loadStarted = false;

        public void Save(List<GameObject> sl) {

            int i = 0;

            saveStarted = true;
            splineList = sl;
            selected = new bool[splineList.Count];

            // Default true
            for (i = 0; i < selected.Length; i++) {

                selected[i] = true;
            }

            SplineFileManager window = (SplineFileManager)EditorWindow.GetWindow(typeof(SplineFileManager), true, "Save Splines to File");
            window.Show();
        }

        public void Load() {

            loadStarted = true;
            splineList = null;
            selected = null;

            SplineFileManager window = (SplineFileManager)EditorWindow.GetWindow(typeof(SplineFileManager), true, "Load Splines from File");
            window.Show();
        }

        private void OnGUI() {

            int i = 0, j = 0, k = 0;

            GUILayout.Label(errorMsg);
            GUILayout.Label("File Name (include .xml):");
            fileName = EditorGUILayout.TextField(fileName);

            // For saving option
            if (saveStarted) {

                bool somethingSelected = false;

                GUILayout.Label("Select Splines to Save:");

                // Get each checked spline to save
                for (i = 0; i < selected.Length; i++) {

                    if (splineList[i] != null) {

                        selected[i] = EditorGUILayout.Toggle(splineList[i].name, selected[i]);
                    }
                    else {

                        selected[i] = false;
                    }
                }

                // Only show save button if something selected
                for (i = 0; i < selected.Length; i++) {

                    if (selected[i] == true) {

                        somethingSelected = true;
                        break;
                    }
                }

                if (somethingSelected) {

                    if (GUILayout.Button("Save")) {

                        int numToSave = 0;

                        for (i = 0; i < selected.Length; i++) {

                            if (selected[i] == true) {

                                numToSave++;
                            }
                        }

                        splinesToSave = new SplineSerial[numToSave];

                        j = 0;  // Increment for each completed spline to save

                        // Construct spline data
                        for (i = 0; i < selected.Length; i++) {

                            if (selected[i] == true) {

                                Spline curSpline = splineList[i].GetComponent<Spline>();

                                // Save spline
                                if (curSpline != null) {

                                    splinesToSave[j] = new SplineSerial();
                                    splinesToSave[j].points = new SplinePointSerial[curSpline.controlPoints.Count];
                                    
                                    splinesToSave[j].name = splineList[i].name;
                                    splinesToSave[j].debug = curSpline.debug;
                                    splinesToSave[j].loop = curSpline.loop;
                                    splinesToSave[j].gameMode = curSpline.gameMode;
                                    splinesToSave[j].playType = curSpline.playType;
                                    splinesToSave[j].dt = curSpline.dt;

                                    k = 0;  // Increment for each point to save

                                    // Save points
                                    foreach (GameObject p in curSpline.controlPoints) {

                                        splinesToSave[j].points[k] = new SplinePointSerial();

                                        splinesToSave[j].points[k].name = p.name;
                                        splinesToSave[j].points[k].x = p.transform.position.x;
                                        splinesToSave[j].points[k].y = p.transform.position.y;
                                        splinesToSave[j].points[k].z = p.transform.position.z;

                                        k++;
                                    }

                                    j++;
                                }
                                else {

                                    Debug.Log("Error saving spline " + splineList[i].name + " to " + fileName + ".");

                                    splinesToSave[j].name = splineList[i].name;
                                    j++;
                                }
                            }
                        }

                        // Push to file
                        SplineXmlObject splineXML = new SplineXmlObject();
                        splineXML.splines = splinesToSave;

                        var serializer = new XmlSerializer(typeof(SplineXmlObject));

                        using (var stream = new FileStream(fileName, FileMode.Create)) {

                            serializer.Serialize(stream, splineXML);
                        }

                        saveStarted = false;

                        this.Close();
                    }
                }
            }


            // For loading option
            if (loadStarted) {

                if (GUILayout.Button("Load")) {

                    SplineXmlObject loadedSplineObj = null;
                    var serializer = new XmlSerializer(typeof(SplineXmlObject));

                    try {
                        using (var stream = new FileStream(fileName, FileMode.Open)) {

                            loadedSplineObj = serializer.Deserialize(stream) as SplineXmlObject;
                        }
                    }
                    catch (System.Exception e) {

                        errorMsg = "File does not exist or is not spline xml.";
                        Debug.Log(e);
                    }

                    if (loadedSplineObj != null) {

                        SplineSerial[] loadedSplines = loadedSplineObj.splines;

                        // Create each spline saved i nthe file
                        for (i = 0; i < loadedSplines.Length; i++) {

                            GameObject newSpline = new GameObject();
                            Spline newSplineScript = newSpline.AddComponent<Spline>();

                            // Create spline
                            newSpline.name = loadedSplines[i].name;
                            newSplineScript.debug = loadedSplines[i].debug;
                            newSplineScript.loop = loadedSplines[i].loop;
                            newSplineScript.gameMode = loadedSplines[i].gameMode;
                            newSplineScript.playType = loadedSplines[i].playType;
                            newSplineScript.dt = loadedSplines[i].dt;
                            newSplineScript.controlPoints = new List<GameObject>();

                            // Create spline's points
                            for (j = 0; j < loadedSplines[i].points.Length; j++) {

                                SplinePointSerial loadedPoint = loadedSplines[i].points[j];

                                GameObject newPoint = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                                newPoint.name = loadedPoint.name;
                                newPoint.transform.position = new Vector3(loadedPoint.x, loadedPoint.y, loadedPoint.z);
                                newPoint.transform.localScale = new Vector3(0.2f, 0.2f, 0.2f);
                                newPoint.transform.parent = newSpline.transform;

                                newSplineScript.controlPoints.Add(newPoint);
                            }
                        }

                        loadStarted = false;

                        this.Close();
                    }
                }
            }
        }
    }

    // Display a help prompt with all the information for using spline tool
    public class HelpPrompt : EditorWindow {

        Vector2 scrollPos;

        public void Initialize() {
            
            HelpPrompt window = (HelpPrompt)EditorWindow.GetWindow(typeof(HelpPrompt), true, "Help");

            window.minSize = new Vector2(500, 525);
            window.maxSize = new Vector2(500, 525);

            window.Show();
        }

        private void OnGUI() {

            GUI.skin.label.wordWrap = true;
            
            EditorGUILayout.BeginVertical();
            scrollPos = EditorGUILayout.BeginScrollView(scrollPos, GUILayout.Width(500), GUILayout.Height(500));

            GUILayout.Label("How to use");
            GUILayout.Label("-------------------------------");
            GUILayout.Label("Spline Tool allows the creation of splines and points for use in any scene. " +
                "All available options are in the JB Tools/Spline menu and the menu when right clicking a " +
                "spline script component on a game object spline in the inspector.");
            GUILayout.Label("");
            GUILayout.Label("Refresh Active Splines: refreshes internal list of splines in the scene. Only " +
                "needs to be called if splines were made manually since the time project was opened.");
            GUILayout.Label("");
            GUILayout.Label("New Spline: Create a new spline in the world space. The spline is set as the " +
                "active spline.");
            GUILayout.Label("");
            GUILayout.Label("Add Point: Adds a single point to the active spline.");
            GUILayout.Label("");
            GUILayout.Label("Save All Splines: Open a prompt to save all or only selected splines to a file.");
            GUILayout.Label("");
            GUILayout.Label("Save Current Spline: Open prompt to save the active spline.");
            GUILayout.Label("");
            GUILayout.Label("Load from File: Opens prompt which loads spline(s) from a file.");
            GUILayout.Label("");
            GUILayout.Label("In the right click menu:");
            GUILayout.Label("Select as Current Spline: Sets the chosen spline as the active spline.");
            GUILayout.Label("");
            GUILayout.Label("Add Point: Adds point to the selected spline and sets it as the active spline.");
            GUILayout.Label("");
            GUILayout.Label("Save Spline: Opens prompt to save the selected spline to a file.");
            GUILayout.Label("");
            GUILayout.Label("Required Files in Unity");
            GUILayout.Label("-------------------------------");
            GUILayout.Label("To use the spline tool, you require Spline.cs and SplineTool.cs. " +
                "Spline.cs will be a component on each individual spline and manage the data " +
                "associated with that spline. SplineTool.cs will be responsibile for the creation " +
                "of splines and points in the splines as well as granting the ability to save and " +
                "load splines to transfer them between programs. Spline.cs can be used and applied " +
                "independantly from SplineTool.cs, though manual creation will be required.");

            EditorGUILayout.EndScrollView();
            EditorGUILayout.EndVertical();

            if (GUILayout.Button("OK!")) {

                this.Close();
            }
        }
    }
}