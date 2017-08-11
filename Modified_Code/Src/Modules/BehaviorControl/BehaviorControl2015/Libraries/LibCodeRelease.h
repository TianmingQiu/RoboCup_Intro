/**
* @file LibCodeRelease.h
*/
class LibCodeRelease : public LibraryBase
{
public:
    /** Constructor for initializing all members*/

	
	LibCodeRelease();
    void preProcess() override;

    void postProcess() override;

    bool between(float value, float min, float max);

    int timeSinceBallWasSeen();



    // Own functions and variables
    float angleToGoal; //angle to enemy goal
    float angleToOwnGoal;
    // often used functions, to keep the roles clean
    float distanceBetween(Vector2f vector1, Vector2f vector2);

    Vector2f robotToField(Vector2f vector); //converts coordinates from relative to the robot to absolute field coordinates
    
    Vector2f fieldToRobot(Vector2f vector);
    //function to sum vectors
    Vector2f vectorAdd(Vector2f vector1,Vector2f vector2);
    //function to avoid obstacles
    Vector2f AvoidObstacles(Vector2f target, float range=1000);
    
    // often used variables, to keep the code clean
    Vector2f centerOpponentGroundLine;    //the center of the opponents ground line
	Vector2f centerOwnGoalGroundLine; //the center of your own goal ground line
    Vector2f centerPenaltyAreaFieldCoordinates;
    Vector2f centerField ;
    Vector2f centerDefenseAreaFieldCoordinates;     //coordinates where the defender starts
    Vector2f strikerStartPosition;                  //just outside the center circle

    float goalWidth;
    float fieldWidth;
    float defenseRadius; //distance the defender keeps from the goal, it moves on a circle with this radius
    float initialDelay;  //number of seconds to wait and look around when going into Playing state
    
    int lostDeviation; // higher than this means you're lost
    int localizedDeviation; //lower than this means you've localized yourself
	float minValidity; // lower than this means you're lost
};