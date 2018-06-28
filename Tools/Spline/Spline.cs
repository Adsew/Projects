using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[ExecuteInEditMode]
[RequireComponent(typeof(LineRenderer))]
public class Spline : MonoBehaviour {
    
    private LineRenderer debugLine = null;

    private List<SplineObject> splineObjects = new List<SplineObject>();

    public bool debug = true;
    public bool loop = false;

    public enum GameModes {

        Invalid,
        None,
        Manual,
        Forward,
        Backward,
        PingPong,
        Pause
    };

    /*
     * Modes:
     * Invalid - default setting does nothing or usable as error case.
     * None - no movement set, does nothing.
     * Manual - objects dictate their own position on the spline.
     * Forward - objects are moved forward on the spline automatically.
     * Backward - objects move backward on the spline automatically.
     * Pingpong - objects that reach the end of the spline change direction.
     * Pause - objects stop where they are on the spline.
     */
    [Tooltip("Modes:\n"
     + "Invalid - default setting does nothing or usable as error case.\n"
     + "None - no movement set, does nothing.\n"
     + "Manual - objects dictate their own position on the spline.\n"
     + "Forward - objects are moved forward on the spline automatically.\n"
     + "Backward - objects move backward on the spline automatically.\n"
     + "Pingpong - objects that reach the end of the spline change direction.\n"
     + "Pause - objects stop where they are on the spline.")]
    public GameModes gameMode = GameModes.Forward;

    public enum PlayBackType {

        Constant_Time,
        Constant_Speed
    }
    public PlayBackType playType = PlayBackType.Constant_Time;

    [Tooltip("Constant Time - Time to travel between points.\n"
        + "Constant Speed - Speed the object moves.")]
    [Range(1.0f, 60.0f)]
    public float dt = 1.0f;                      // Time between steps (1 - 60 seconds) or speed

    public List<SplineObject> splineObjectsOnStart = new List<SplineObject>();

    public List<GameObject> controlPoints = new List<GameObject>();
    
    
    public void AddHead(SplineObject newHead) {

        splineObjects.Add(newHead);
        newHead.spline = this;
    }

    public void AddHeadAtPoint(SplineObject newHead, GameObject point) {

        if (controlPoints.Contains(point)) {
            
            splineObjects.Add(newHead);
            newHead.t = controlPoints.IndexOf(point) - 1;
            newHead.oldT = newHead.t;
            newHead.lastT = newHead.t;
            newHead.spline = this;
        }
    }

    public void AddHeadAtClosestLocation(SplineObject newHead) {

        splineObjects.Add(newHead);

        newHead.t = getClosestLocationT(newHead.transform.position);
        newHead.oldT = newHead.t;
        newHead.lastT = newHead.t;
    }

    public void AddHeadAtT(SplineObject newHead, float t) {

        if (t > controlPoints.Count) {

            t -= controlPoints.Count;
        }

        splineObjects.Add(newHead);
        newHead.t = t;
        newHead.oldT = newHead.t;
        newHead.lastT = newHead.t;
        newHead.spline = this;
    }

    public void RemoveHead(SplineObject oldHead) {

        int index = splineObjects.IndexOf(oldHead);
        
        if (index >= 0) {

            splineObjects.RemoveAt(index);
        }
    }

    public void RemoveAllHeads() {

        splineObjects.Clear();
    }

    public GameObject GetNextPointForHead(SplineObject h) {
        
        if (splineObjects.Contains(h)) {

            int nextPIndex = 0;

            if (gameMode == GameModes.Forward 
                || (gameMode == GameModes.PingPong && h.pingpongSign > 0.0f)) {

                nextPIndex = ((int)h.t + 2) % controlPoints.Count;
            }
            else {

                nextPIndex = ((int)h.t + 1) % controlPoints.Count;
            }

            return controlPoints[nextPIndex];
        }

        return null;
    }

    public GameObject GetPrevPointForHead(SplineObject h) {
        
        if (splineObjects.Contains(h)) {

            int nextPIndex = 0;

            if (gameMode == GameModes.Forward
                || (gameMode == GameModes.PingPong && h.pingpongSign > 0.0f)) {

                nextPIndex = ((int)h.t + 1) % controlPoints.Count;
            }
            else {

                nextPIndex = ((int)h.t + 2) % controlPoints.Count;
            }

            return controlPoints[nextPIndex];
        }

        return null;
    }

    public GameObject GetClosestPoint(Vector3 point) {

        if (controlPoints.Count > 0) {

            GameObject closestPoint = controlPoints[0];
            float closestDist = Vector3.Magnitude(closestPoint.transform.position - point);

            for (int i = 1; i < controlPoints.Count; i++) {

                float tempDist = Vector3.Magnitude(controlPoints[i].transform.position - point);

                if (tempDist < closestDist) {

                    closestDist = tempDist;
                    closestPoint = controlPoints[i];
                }
            }

            return closestPoint;
        }

        return null;
    }

    public float getClosestLocationT(Vector3 location) {

        if (controlPoints.Count > 0) {

            // Find closest initial point
            int closestPoint = 2;
            float closestDist = Vector3.Magnitude(controlPoints[2].transform.position - location);

            for (int i = 3; i < controlPoints.Count - 3; i++) {

                float tempDist = Vector3.Magnitude(controlPoints[i].transform.position - location);

                if (tempDist < closestDist) {

                    closestDist = tempDist;
                    closestPoint = i;
                }
            }

            // Check which surrounding point is closer, then find a t value between
            int secondClosestPoint = closestPoint - 1;

            float prevPointDist = Vector3.Magnitude(controlPoints[closestPoint - 1].transform.position - location);
            float nextPointDist = Vector3.Magnitude(controlPoints[closestPoint + 1].transform.position - location);

            if (nextPointDist < prevPointDist) {

                secondClosestPoint = closestPoint + 1;
            }

            // Now with closest and second closest, find where between the head is
            // and approximate t value between
            float distToClosest = Vector3.Magnitude(controlPoints[closestPoint].transform.position - location);
            float distToSecondClosest = Vector3.Magnitude(controlPoints[secondClosestPoint].transform.position - location);

            float tPercentFromClosest = distToClosest / (distToClosest + distToSecondClosest);

            if (closestPoint > secondClosestPoint) {

                return closestPoint - tPercentFromClosest - 1.0f;
            }

            return closestPoint + tPercentFromClosest - 1.0f;
        }

        return 0.0f;
    }

    public Vector3 CatmullRomSpline(float tVal) {
        
        // Ensure enough points for spline given position t
        if (controlPoints.Count >= 4) {

            // Calculate which points to be used (needed for loop mode)
            int i1 = (int)tVal;
            int i2 = i1 + 1;
            int i3 = i1 + 2;
            int i4 = i1 + 3;

            // Wrap points around
            if (i1 > (controlPoints.Count - 1)) {

                i1 = i1 % controlPoints.Count;
            }
            if (i2 > (controlPoints.Count - 1)) {

                i2 = i2 % controlPoints.Count;
            }
            if (i3 > (controlPoints.Count - 1)) {

                i3 = i3 % controlPoints.Count;
            }
            if (i4 > (controlPoints.Count - 1)) {

                i4 = i4 % controlPoints.Count;
            }
            
            if (controlPoints[i1] != null
                && controlPoints[i2] != null
                && controlPoints[i3] != null
                && controlPoints[i4] != null
                ) {

                Vector3 newPosition;
                
                Vector3 p0 = controlPoints[i1].transform.position;
                Vector3 p1 = controlPoints[i2].transform.position;
                Vector3 p2 = controlPoints[i3].transform.position;
                Vector3 p3 = controlPoints[i4].transform.position;
                
                float catmulT = tVal - (int)tVal;    // Keep number between 0 and 1

                //Catmull rom equation converted
                newPosition = 0.5f * (
                    (p1 * 2.0f)
                    + (p2 - p0) * catmulT
                    + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * catmulT * catmulT
                    + (3.0f * p1 - p0 - 3.0f * p2 + p3) * catmulT * catmulT * catmulT
                    );
                
                return newPosition;
            }
        }

        return new Vector3(0.0f, 0.0f, 0.0f);
    }

    void UpdatePositionUsingGameMode() {

        for (int i = 0; i < splineObjects.Count; i++) {

            float timeStep = 0.1f;
            float oldT = splineObjects[i].t;

            splineObjects[i].lastT = splineObjects[i].oldT;

            if (playType == PlayBackType.Constant_Time) {

                timeStep = Time.deltaTime / (float)dt;  // dt used as seconds to complete interval
            }
            else {

                timeStep = Time.deltaTime;  // Still move in relation to time passed
            }

            // Position modification
            if (gameMode == GameModes.None || gameMode == GameModes.Invalid) {

                splineObjects[i].t = 0.0f;
            }

            else if (gameMode == GameModes.Manual) {
                
                // Treat whole spline as one length and convert t to a position along the whole spline
                if (playType == PlayBackType.Constant_Speed) {

                    float distanceMag = Vector3.Magnitude(CatmullRomSpline(splineObjects[i].oldT) - CatmullRomSpline(splineObjects[i].t));

                    if (distanceMag > 0 && splineObjects[i].t != 0) {

                        splineObjects[i].t = splineObjects[i].oldT +
                            (Mathf.Abs(splineObjects[i].t - splineObjects[i].oldT) / (distanceMag)) 
                            * (dt / 2.0f) * Mathf.Abs(splineObjects[i].t - splineObjects[i].oldT)                                       // Speed
                            * ((splineObjects[i].t - splineObjects[i].oldT) / Mathf.Abs(splineObjects[i].t - splineObjects[i].oldT));   // Sign direction
                    }
                }

                splineObjects[i].oldT = splineObjects[i].t;

                // Keep T value fixed within bounds
                if (loop) {
                    if (splineObjects[i].t > controlPoints.Count) {

                        splineObjects[i].t -= controlPoints.Count;
                        splineObjects[i].oldT = splineObjects[i].t;
                    }
                    else if (splineObjects[i].t < 0.0f) {

                        splineObjects[i].t += controlPoints.Count;
                        splineObjects[i].oldT = splineObjects[i].t;
                    }
                }
                else {
                    if (splineObjects[i].t > (controlPoints.Count - 3)) {

                        if (splineObjects[i].loopAtEnds) { splineObjects[i].t -= controlPoints.Count - 3; }
                        else { splineObjects[i].t = controlPoints.Count - 3; }

                        splineObjects[i].oldT = splineObjects[i].t;
                    }
                    else if (splineObjects[i].t < 0.0f) {

                        if (splineObjects[i].loopAtEnds) { splineObjects[i].t += controlPoints.Count - 3.0f; }
                        else { splineObjects[i].t = 0.0f; }

                        splineObjects[i].oldT = splineObjects[i].t;
                    }
                }
            }

            else if (gameMode == GameModes.Forward) {

                splineObjects[i].t = splineObjects[i].t + timeStep;

                if (playType == PlayBackType.Constant_Speed) {

                    float distanceMag = Vector3.Magnitude(CatmullRomSpline(oldT) - CatmullRomSpline(splineObjects[i].t));

                    splineObjects[i].t = oldT + (timeStep / (distanceMag * 10.0f)) * (dt / 2.0f);    // Redo new t calculation (dt used as speed)
                }

                if (loop && splineObjects[i].t > controlPoints.Count) {

                    splineObjects[i].t = 0.0f;
                }
                else if (splineObjects[i].t > (controlPoints.Count - 3)) {

                    if (splineObjects[i].loopAtEnds) { splineObjects[i].t -= controlPoints.Count - 3; }
                    else { splineObjects[i].t = controlPoints.Count - 3; }

                    splineObjects[i].oldT = splineObjects[i].t;
                }
            }

            else if (gameMode == GameModes.Backward) {

                timeStep = -1.0f * timeStep;

                splineObjects[i].t = splineObjects[i].t + timeStep;

                if (playType == PlayBackType.Constant_Speed) {

                    float distanceMag = Vector3.Magnitude(CatmullRomSpline(oldT) - CatmullRomSpline(splineObjects[i].t));

                    splineObjects[i].t = oldT + (timeStep / (distanceMag * 10.0f)) * (dt / 2.0f);    // Redo new t calculation (dt used as speed)
                }

                if (loop && splineObjects[i].t <= 0.0f) {

                    splineObjects[i].t = controlPoints.Count;
                }
                if (splineObjects[i].t <= 0.0f) {

                    if (splineObjects[i].loopAtEnds) { splineObjects[i].t = controlPoints.Count - 3.0f; }
                    else { splineObjects[i].t = 0.0f; }

                    splineObjects[i].oldT = splineObjects[i].t;
                }
            }

            else if (gameMode == GameModes.PingPong) {

                timeStep = splineObjects[i].pingpongSign * timeStep;

                splineObjects[i].t = splineObjects[i].t + timeStep;

                if (playType == PlayBackType.Constant_Speed) {

                    float distanceMag = Vector3.Magnitude(CatmullRomSpline(oldT) - CatmullRomSpline(splineObjects[i].t));

                    splineObjects[i].t = oldT + (timeStep / (distanceMag * 10.0f)) * (dt / 2.0f);    // Redo new t calculation (dt used as speed)
                }

                if (loop) {
                    // Go Backward
                    if (splineObjects[i].t > controlPoints.Count) {

                        splineObjects[i].t = controlPoints.Count;
                        splineObjects[i].pingpongSign = -1.0f;
                    }
                    // Go Forward
                    else if (splineObjects[i].t <= 0.0f) {

                        splineObjects[i].t = 0.0f;
                        splineObjects[i].pingpongSign = 1.0f;
                    }
                }
                else {
                    // Go Backward
                    if (splineObjects[i].t >= (controlPoints.Count - 3)) {

                        splineObjects[i].t = controlPoints.Count - 3.0f;
                        splineObjects[i].pingpongSign = -1.0f;
                    }
                    // Go Forward
                    else if (splineObjects[i].t <= 0.0f) {

                        splineObjects[i].t = 0.0f;
                        splineObjects[i].pingpongSign = 1.0f;
                    }
                }
            }

            Vector3 newPos = CatmullRomSpline(splineObjects[i].lastT);
            Vector3 calcPos = CatmullRomSpline(splineObjects[i].t);
            Vector3 newForward = calcPos - newPos;

            newPos = splineObjects[i].transform.position;

            if (splineObjects[i].x) {

                newPos.x = calcPos.x;
            }
            if (splineObjects[i].y) {

                newPos.y = calcPos.y;
            }
            if (splineObjects[i].z) {

                newPos.z = calcPos.z;
            }

            if (!splineObjects[i].x) {

                newForward.x = 0.0f;
            }
            if (!splineObjects[i].y) {

                newForward.y = 0.0f;
            }
            if (!splineObjects[i].z) {

                newForward.z = 0.0f;
            }

            if (newForward != Vector3.zero) {
                
                splineObjects[i].transform.forward = newForward.normalized;
            }

            if (splineObjects[i].useRigidbodyMovement) {

                Rigidbody rb = splineObjects[i].GetComponent<Rigidbody>();

                if (rb != null) {

                    rb.MovePosition(newPos);
                    rb.MoveRotation(splineObjects[i].transform.rotation);
                }
                else {

                    Debug.Log("Object " + splineObjects[i].gameObject.name + " does not have rigidbody. Using normal movement.");
                }
            }
            else {

                splineObjects[i].transform.SetPositionAndRotation(newPos, splineObjects[i].transform.rotation);
            }
        }
    }

    void UpdateDebugLine() {

        // Ensure we have required line component
        if (debugLine == null) {

            LineRenderer myLine = this.gameObject.GetComponent<LineRenderer>();

            if (myLine == null) {

                debugLine = this.gameObject.AddComponent<LineRenderer>();
            }
            else {

                debugLine = myLine;
            }

            debugLine.startWidth = 0.2f;
            debugLine.endWidth = 0.2f;
        }

        if (debug == true) {

            foreach (GameObject point in controlPoints) {

                if (point != null) {

                    point.SetActive(true);
                }
            }

            if (controlPoints.Count >= 4) { 

                int pointsToDraw;

                if (loop == true) {
                    
                    pointsToDraw = ((controlPoints.Count) * 4) + 1;  // 4 points per segment + 1 for final point
                }
                else {

                    pointsToDraw = ((controlPoints.Count - 3) * 4) + 1;  // 4 points per segment + 1 for final point
                }

                debugLine.positionCount = pointsToDraw;
                
                for (int i = 0; i < pointsToDraw - 1; i++) {

                    debugLine.SetPosition(i, CatmullRomSpline(i / 4.0f)); 
                }

                // Special case to draw perfectly to end point
                if (loop == true) {

                    debugLine.SetPosition(pointsToDraw - 1, controlPoints[1].transform.position);
                }
                else {

                    debugLine.SetPosition(pointsToDraw - 1, controlPoints[controlPoints.Count-2].transform.position);
                }
            }
            // Not enough points to draw spline
            else {

                debugLine.positionCount = 0;
            }
        }
        else {

            debugLine.positionCount = 0;

            foreach (GameObject point in controlPoints) {

                if (point != null) {

                    point.SetActive(false);
                }
            }
        }
    }

    // Ensure points list stays up to date to avoid errors
    private void RefreshPoints() {

        int i = 0;

        if (controlPoints != null) {

            for (i = 0; i < controlPoints.Count; i++) {

                if (controlPoints[i] == null) {

                    controlPoints.RemoveAt(i);
                    i--;
                }
            }
        }
    }

    void Start() {
        
        foreach (SplineObject obj in splineObjectsOnStart) {

            obj.SetSpline(this);
        }
    }

    // Update is called once per frame or when something moves in editor
    void Update () {

        RefreshPoints();
        UpdateDebugLine();

        if (Application.isPlaying) {

            UpdatePositionUsingGameMode();
        }
    }

    public void SetDebug(bool b) {

        debug = b;
    }

    public void SetMode(GameModes gm) {

        gameMode = gm;
    }
}
