/** Sets all members of the MotionRequest representation for executing a TargetMode-WalkRequest
 *  (i.e. Walk to a \c target at a \c speed)
 *  @param speed Walking speeds, in percentage.
 *  @param target Walking target, in mm and radians, relative to the robot.
 */
option(WalkToTarget, (const Pose2f&) speed, (const Pose2f&) target, (int) (1) enableCollisionAvoidance, (float) (1000) collisionAvoidanceRange)
{
  /** Set the motion request. */
  initial_state(setRequest)
  {
    transition
    {
      if(theMotionInfo.motion == MotionRequest::walk)
        goto requestIsExecuted;
    }
    action
    {
      theMotionRequest.motion = MotionRequest::walk;
      theMotionRequest.walkRequest.mode = WalkRequest::targetMode;
      theMotionRequest.walkRequest.target = target;
      theMotionRequest.walkRequest.speed = speed;
      theMotionRequest.walkRequest.kickType = WalkRequest::none;
    }
  }

  /** The motion process has started executing the request. */
  target_state(requestIsExecuted)
  {
    transition
    {
      if(theMotionInfo.motion != MotionRequest::walk)
        goto setRequest;
    }
    action
    {
      theMotionRequest.motion = MotionRequest::walk;
      theMotionRequest.walkRequest.mode = WalkRequest::targetMode;
      
      if (enableCollisionAvoidance == 1){
          Vector2f targetPositionCorrected = libCodeRelease.AvoidObstacles(target.translation, collisionAvoidanceRange); //alter path so as to avoid obstacles
          Pose2f targetUpdated = Pose2f(target.rotation, targetPositionCorrected);
      theMotionRequest.walkRequest.target = targetUpdated;
      }
      else{
                theMotionRequest.walkRequest.target = target;
      }
      theMotionRequest.walkRequest.speed = speed;
      theMotionRequest.walkRequest.kickType = WalkRequest::none;
    }
  }
}
