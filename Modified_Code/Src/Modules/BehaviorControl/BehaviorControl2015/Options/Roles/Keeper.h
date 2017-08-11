/** The Keeper */
option(Keeper)
{
	// Debug
    Annotation("robotPose deviation: ", theRobotPose.deviation);
    Annotation("robotPose validity: ", theRobotPose.validity);
	
    common_transition{
        if (state_time > 5000 && (theRobotPose.deviation > libCodeRelease.lostDeviation || theRobotPose.validity < libCodeRelease.minValidity))
            goto localizeSelf;
    }
    
    initial_state(start)
    {
        transition
        {
            if(state_time > libCodeRelease.initialDelay) // TODO remove delay before starting
                goto walkToStartPosition;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
        }
    }
	
	 state(walkToStartPosition){
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

    state(faceOpponentDirection)
    {
        transition
        {
            // if (action_done)
            //	goto defendGoal;
            if(std::abs(libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine).angle()) < 5_deg) {
                goto defendGoal;
            }
        }
        action
        { // face toward the center of the field
            Annotation("Turning towards opponents...");
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)0.5; // in radians
            Vector2f relativeOpponentGroundLine =
                libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
        }
    }

    state(defendGoal)
    { // place yourself on a line between ball and goal center, stay on goal x position
        transition
        {
            // ball close to goalkeeper -> kick away
            if(libCodeRelease.timeSinceBallWasSeen() > 3000)
                goto searchForBall;
            else if(theBallModel.estimate.position.norm()< 800.f) {
                goto turnToBall;
            }
            // otherwise stay in this state, unless you lose track of the ball
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; // look left and right,but don't turn your head too much
            headControlTurnAngle = 0.5;                         // in radians
            float yPosScalar = libCodeRelease.goalWidth / libCodeRelease.fieldWidth * (float)0.9; //0.9 to prevent hitting the poles
            // position yourself between ball and goal, stay on goal-x position and limit y movement to goal width
            // instead of field width (=don't leave the goal)
            Vector2f defendGoalPosition = libCodeRelease.fieldToRobot(
                Vector2f(libCodeRelease.centerPenaltyAreaFieldCoordinates.x(),
                         libCodeRelease.robotToField(theBallModel.estimate.position).y() * yPosScalar));
            Pose2f target = Pose2f(theBallModel.estimate.position.angle(),
                                   defendGoalPosition); // target position in absolute coordinates
            WalkToTarget(Pose2f(20.f, 20.f, 20.f), target);
        }
    }
    
    state(walkBack)
    {
        transition
        { // go to searchForBall if you've reached the goal
            // currentPos - goalPos
            Vector2f ownPosFieldCoordinates = libCodeRelease.robotToField(Vector2f(0.f, 0.f));
            float distance = libCodeRelease.distanceBetween(ownPosFieldCoordinates,
                                                            libCodeRelease.centerPenaltyAreaFieldCoordinates);
            if(distance < 200.f) // after reaching the center of the penalty area, face toward the opponent side
                goto faceOpponentDirection;
			else if (( libCodeRelease.timeSinceBallWasSeen() < 300) && (theBallModel.estimate.position.norm()< 1000.f) )
                goto turnToBall;  
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)0.8;
            Vector2f centerPenaltyAreaRobotCoordinates = libCodeRelease.fieldToRobot(
                libCodeRelease.centerPenaltyAreaFieldCoordinates);          // convert to relative coordinates
            Pose2f target = Pose2f(0.0f, centerPenaltyAreaRobotCoordinates); 
            WalkToTarget(Pose2f(0.f, 100.f, 100.f), target); //disallow turning
        }
    }

// all the following except searchForBall are from the Striker, to kick the ball properly
// only modification is direction: away from own goal instead of toward enemy goal
    state(turnToBall)
    {
        transition
        {
            if(std::abs(theBallModel.estimate.position.angle()) < 5_deg && theBallModel.estimate.position.norm()< 500.f)
                goto walkToBall;
            else if(theBallModel.estimate.position.norm() >= 750.f) //if someone else kicks the ball away, go back to defending
                goto defendGoal;
            else if(libCodeRelease.timeSinceBallWasSeen() > 4000)
                goto walkBack;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
    }

    state(walkToBall)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() >= 750.f) //if someone else kicks the ball away, go back to defending
                goto defendGoal;
			else if(theBallModel.estimate.position.norm() < 400.f)
				goto alignToGoal;
            else if(libCodeRelease.timeSinceBallWasSeen() > 4000)
                goto walkBack;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.7);
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(),theBallModel.estimate.position));
        }
    }

    state(alignToGoal)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() >= 650.f) //if someone else kicks the ball away, go back to defending
                goto defendGoal;
            else if(libCodeRelease.timeSinceBallWasSeen() > 3000)
                goto walkBack;
            else if(std::abs(libCodeRelease.angleToOwnGoal + 3.14f) < 8_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
                goto alignBehindBall;
            else if(theBallModel.estimate.position.norm() > 500.f)
                goto walkToBall;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.7);
            WalkToTarget(Pose2f(100.f, 100.f, 100.f),
                         Pose2f(3.14f + libCodeRelease.angleToOwnGoal, //opposite side of own goal
                                theBallModel.estimate.position.x() - 200.f,
                                theBallModel.estimate.position.y()), 1, 500);  //set collision avoidance to lower value
        }
    }

    state(alignBehindBall)
    {
        transition
        {
            if(theBallModel.estimate.position.norm() >= 650.f) //if someone else kicks the ball away, go back to defending
                goto defendGoal;
            else if(libCodeRelease.timeSinceBallWasSeen() > 2000)
                goto walkBack;
            else if(libCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f) &&
               libCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f) &&
               std::abs(libCodeRelease.angleToOwnGoal + 3.14f) < 2_deg)
                goto kick;
            else if(theBallModel.estimate.position.norm() > 400.f)
                goto alignToGoal;
        }
        action
        {
            theHeadControlMode = HeadControl::lookLeftAndRight; headControlTurnAngle = float(0.5);
            WalkToTarget(Pose2f(80.f, 80.f, 80.f),
                         Pose2f(3.14f + libCodeRelease.angleToOwnGoal,
                                theBallModel.estimate.position.x() - 150.f,
                                theBallModel.estimate.position.y() - 30.f), 1, 200);  //set collision avoidance to lower value
        }
    }

    state(kick)
    {
        transition
        {
            if(state_time > 3000 || (state_time > 10 && action_done))
                goto walkBack; //walk back, don't allow turning so we keep looking at the ball
        }
        action
        {
            Annotation("Alive and Kickin'");
            theHeadControlMode = HeadControl::lookForward;
            InWalkKick(WalkRequest::left,
                       Pose2f(3.14f + libCodeRelease.angleToOwnGoal,
                              theBallModel.estimate.position.x() - 160.f,
                              theBallModel.estimate.position.y() - 55.f));
        }
    }
    
    
// here again non- kick states
    

    state(searchForBall)
    {
        transition
        {
            if(libCodeRelease.timeSinceBallWasSeen() < 300)
                goto defendGoal;
			else if(state_time > 10000)
				goto walkBack; //go back to goal, then face opponent direction
        }
        action
        {//first, turn towards ball, then turn in circles to look for it.
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
            theHeadControlMode = HeadControl::lookLeftAndRight;headControlTurnAngle = float(0.7f);
        }
    }
    
    state(localizeSelf)
    {
        transition
        {
            if(theRobotPose.deviation < libCodeRelease.localizedDeviation && theRobotPose.validity > libCodeRelease.minValidity)
                goto searchForBall;
			else if (state_time > 10000) //try to get out of local minima
				goto defendGoal;
        }
        action
        {
            //first, face towards where you think the enemy is
			Annotation("state_time: ", state_time);
            Vector2f relativeOpponentGroundLine =
            libCodeRelease.fieldToRobot(libCodeRelease.centerOpponentGroundLine); // transform to robot coordinates
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(relativeOpponentGroundLine.angle(), 0.f, 0.f));
            if (state_time < 2000){
                theHeadControlMode = HeadControl::lookLeftAndRight;
            }
            else {
                float turnSpeed = 0.5f; //clockwise
				if (libCodeRelease.robotToField(Vector2f(0.f, 0.f)).y() > 0)
					turnSpeed = -0.5f; //counterclockwise, to turn towards center of the field
                WalkAtSpeedPercentage(Pose2f(turnSpeed, 0.05f, 0.05f)); // add some x and y to prevent getting stuck in local minima
                theHeadControlMode = HeadControl::lookLeftAndRight;
            }
        }
    }

    
}
