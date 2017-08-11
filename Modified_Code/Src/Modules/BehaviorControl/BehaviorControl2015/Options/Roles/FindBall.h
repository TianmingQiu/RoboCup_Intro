/** A test striker option without common decision */
option(FindBall)
{
  initial_state(start)
  {
    transition
    {
        goto SearchForball;
    }
    action
    {        
      LookLeftAndRight();      
    }
  }

  state(SearchForball)
  {
    transition
    {
        if(libCodeRelease.timeSinceBallWasSeen() < 300)                    
	    goto turnToBall;
               
    }
    action
    {
        theHeadControlMode = HeadControl::lookLeftAndRight;
            headControlTurnAngle = (float)2.0;
        if(libCodeRelease.timeSinceBallWasSeen() > 5000 && state_time>10000)
        {
            Annotation("Rotate to find the ball");
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(3.f,0.f,0.f));        
        }
        
    }
  }
  
  state(turnToBall)
  {
    transition
    {
      if(libCodeRelease.timeSinceBallWasSeen() > 5000)
        goto SearchForball;
      
    }
    action
    {
        if(std::abs(theBallModel.estimate.position.angle()) > 5_deg)
        {
            Annotation("rotating to the ball");
          theHeadControlMode = HeadControl::lookForward;
          WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
        }
        else
        {
            Annotation("ball found");
        }
    }
    }
  }
  
  
  
   
