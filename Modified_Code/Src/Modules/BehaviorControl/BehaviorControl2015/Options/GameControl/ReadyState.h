/** behavior for the ready state. In this state, all robots move to their designated starting positions (depends on their role) */
option(ReadyState)
{
  Annotation("robotPose deviation: ", theRobotPose.deviation);
  Annotation("robotPose validity: ", theRobotPose.validity);
  common_transition{
    if (state_time > 15000 && (theRobotPose.deviation > libCodeRelease.lostDeviation || theRobotPose.validity < libCodeRelease.minValidity))
        goto localizeSelf;
  }
    
  initial_state(moveForward)
  { 
    transition{
        if(state_time > 5000)
            goto chooseRole;
    }
    action
    {
      theHeadControlMode = HeadControl::lookLeftAndRight;
      headControlTurnAngle = (float)1.0;
      WalkAtSpeedPercentage(Pose2f(0.f, 1.f, 0.f));
      Annotation("moving onto the field...");
    }	
  }
  
  state(chooseRole){
    transition{
          switch (theRobotInfo.number){ 
			case 1: theBehaviorStatus.role = Role::keeper; goto keeper;break; 
            case 2: theBehaviorStatus.role = Role::defender;goto defender;break;
			case 3: theBehaviorStatus.role = Role::supporter;goto supporter;break;
			case 4: theBehaviorStatus.role = Role::striker;goto striker;break;
			default: goto standRobot;
		}
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      Stand();
      Annotation("Choosing starting position depending on role...");
    }
  }
  
  state(keeper){
       //Annotation("Keeper selected");
       transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.centerPenaltyAreaFieldCoordinates);
            if(distance < 200.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)1.0;
            Vector2f centerPenaltyAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.centerPenaltyAreaFieldCoordinates); // convert to relative coordinates
            Pose2f target = Pose2f(centerPenaltyAreaRobotCoordinates.angle(),
                                   centerPenaltyAreaRobotCoordinates); // turn to face in the right direction
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), target);
        }
  }
  
  state(defender){
      //Annotation("Defender selected");
      transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.centerDefenseAreaFieldCoordinates+ Vector2f(0.f,-100.f));
            if(distance < 200.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)1.0;
            Vector2f centerDefenseAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.centerDefenseAreaFieldCoordinates + Vector2f(0.f,-100.f)); // give offset so keeper can see; convert to relative coordinates
            Pose2f target = Pose2f(centerDefenseAreaRobotCoordinates.angle(),
                                   centerDefenseAreaRobotCoordinates); // turn to face in the right direction
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), target);
        }
  }
  
  state(supporter){
      //Annotation("Supporter selected");
      //TODO implement this properly, now it's the same as defender
      transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.centerDefenseAreaFieldCoordinates+ Vector2f(0.f,100.f));
            if(distance < 200.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)1.0;
            Vector2f centerDefenseAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.centerDefenseAreaFieldCoordinates + Vector2f(0.f,100.f)); // give offset so keeper can see; convert to relative coordinates
            Pose2f target = Pose2f(centerDefenseAreaRobotCoordinates.angle(),
                                   centerDefenseAreaRobotCoordinates); // turn to face in the right direction
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), target);
        }
  }
  
  state(striker){
      //Annotation("Striker selected");
	  transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.strikerStartPosition);
            if(distance < 200.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)1.0;
            Vector2f centerDefenseAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.strikerStartPosition); // convert to relative coordinates
            Pose2f target = Pose2f(centerDefenseAreaRobotCoordinates.angle(),
                                   centerDefenseAreaRobotCoordinates); // turn to face in the right direction
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), target);
        }
  }
  
  state(faceOpponentDirection)
    {
        action
        { // face toward the center of the field
            Annotation("Turning towards opponent...");
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)0.5; // in radians
            Vector2f relativeOpponentGroundLine =
                libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
        }
    }
    
    state(standRobot){
        action{
            StandRobot();
        }
    }
    
    state(localizeSelf)
    {
        transition
        {
            if(theRobotPose.deviation < libCodeRelease.localizedDeviation && theRobotPose.validity > libCodeRelease.minValidity)
                goto chooseRole;
        }
        action
        {
            //first, face towards where you think the enemy is
            Vector2f relativeOpponentGroundLine =
            libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
            if (state_time < 2000){
                theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(1.f);
            }
            else {
                WalkAtSpeedPercentage(Pose2f(0.3f, 0.05f, 0.05f));
                theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(1.f);
            }
        }
    }
}
