option(LookLeftAndRight, (float) (1.56) turnAngle, (float) (0.38f) tilt, (float)(.2*pi) speed )
//ursprünglich .38f tilt
{
  /** Simply sets the necessary angles */
  initial_state(lookForward)
  {	
	transition
    {
      if(action_done)
        goto lookRight;
    }  
    action
    {
	  SetHeadPanTilt(0.f, tilt, speed);  
    }
  }
  
  state(lookRight)
  {
    transition
    {
      if(action_done)
        goto lookLeft;
    }
    action
    {
      SetHeadPanTilt(turnAngle, tilt, speed); //90 degrees, but in radians
    }
  }
  
  state(lookLeft)
  {
    transition
    {
      if(action_done)
        goto lookForward;
    }
    action
    {
      SetHeadPanTilt(-turnAngle, tilt, speed);
    }
  }
}

