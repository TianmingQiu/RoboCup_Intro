/**
* @file LibCodeRelease.cpp
*/

#include "../LibraryBase.h"
//#include "../Options/Output/Annotation.h"

namespace Behavior2015
{
  #include "LibCodeRelease.h"
  
  LibCodeRelease::LibCodeRelease():
    angleToGoal(0.f)
  {}
  
  void LibCodeRelease::preProcess()
  {
    angleToGoal = (theRobotPose.inverse() * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
    
    // own variables
    angleToOwnGoal = (theRobotPose.inverse() * Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f)).angle();

    centerOpponentGroundLine = Vector2f(theFieldDimensions.xPosOpponentGroundline, theFieldDimensions.yPosCenterGoal);
	centerOwnGoalGroundLine = Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f);
    centerPenaltyAreaFieldCoordinates = Vector2f((theFieldDimensions.xPosOwnGroundline + theFieldDimensions.xPosOwnPenaltyArea)/2, 0);
	centerField = Vector2f(theFieldDimensions.xPosHalfWayLine, theFieldDimensions.yPosCenterGoal);
    goalWidth = (int)std::abs(theFieldDimensions.yPosLeftGoal - theFieldDimensions.yPosRightGoal)*(float)0.9;
    fieldWidth = (int)std::abs(theFieldDimensions.yPosLeftSideline - theFieldDimensions.yPosRightSideline);
	defenseRadius = 1800.f;
	centerDefenseAreaFieldCoordinates = Vector2f(theFieldDimensions.xPosOwnGroundline + defenseRadius, 0.f); //own goal is always at negative coordinates, so plus
	lostDeviation = 45; 
    localizedDeviation = 35;
	minValidity = 0.75f;
    strikerStartPosition = Vector2f(theFieldDimensions.xPosHalfWayLine - theFieldDimensions.centerCircleRadius - 100, 100); 
    initialDelay = 1;//5000; (in milliseconds)
  }

  void LibCodeRelease::postProcess()
  {
  }
  
  int LibCodeRelease::timeSinceBallWasSeen()
  {
    return theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen);
  }
  
  bool LibCodeRelease::between(float value, float min, float max)
  {
    return value >= min && value <= max;
  }  
  
  // Own functions
  float LibCodeRelease::distanceBetween(Vector2f vector1, Vector2f vector2){
	Vector2f distanceVector = vector1 - vector2;
    
	return distanceVector.norm();
  }
  
  Vector2f LibCodeRelease::robotToField(Vector2f vector){
      return Transformation::robotToField(theRobotPose, vector);
  }
  
 
  
  Vector2f LibCodeRelease::fieldToRobot(Vector2f vector){
      return Transformation::fieldToRobot(theRobotPose, vector);
  }
  
   Vector2f LibCodeRelease::vectorAdd(Vector2f vector1,Vector2f vector2)
  {
      Vector2f res;
      res=vector1 + vector2;
      return res;
      
  }
  
  Vector2f LibCodeRelease::AvoidObstacles(Vector2f target, float range) //returns a target position with some offset that makes the robot avoid obstacles (potential field method)
  {
      float magnitude=2;
      Vector2f nextpos=Vector2f::Zero();
      Vector2f move;
      int nbObstacles=theObstacleModel.obstacles.size();
      
      for(int i=0;i<nbObstacles;i++)
      {
          if(theObstacleModel.obstacles[i].type==Obstacle::someRobot || theObstacleModel.obstacles[i].type==Obstacle::opponent  //obstacles are referenced relative to the robot
          || theObstacleModel.obstacles[i].type==Obstacle::teammate)
          {
              /*ANNOTATION("obstacleX",theObstacleModel.obstacles[i].center.x()); 
            ANNOTATION("obstacleY",theObstacleModel.obstacles[i].center.y());*/
              move=-theObstacleModel.obstacles[i].center; //get vector that points away from obstacle
              float distance=move.norm();         
              move=move/distance; //unit vector         
              if(distance<range) // only evade if object too close
              {
                  float scale=range-distance; //size of evasive manouvre depends on distance to object (closer = larger evade)
                  move=move*scale*magnitude;
                  nextpos+=move;
                  }
          }
        }
        return target + nextpos;
    }
}