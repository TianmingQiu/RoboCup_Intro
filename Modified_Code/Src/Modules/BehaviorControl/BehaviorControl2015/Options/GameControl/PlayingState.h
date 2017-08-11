option(PlayingState) //this option chooses which role option to execute. For now, we use static role assignment
{
  initial_state(role_selection) //TODO: replace by common transition for Dynamic Role Selection
  {	
    //(see 8.1.5) of BHuman code release 2013 for explanation how teams are formed in simulation scene
    //first team is the blue team (numbers 1-5), red team is numbers 7-11, but are mapped to numbers 1-5 below here (so red robot 7 has the same role as blue robot 1)
    // no idea why number 6 is skipped, it's very weird
    
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
      Annotation("Choosing roles...");
    }
  }
  
  state(keeper){
	  action{
		  Keeper();
	  }
  }
  
  state(defender){
	  action{
		  Defender();
	  }
  }
  
  state(supporter){ //TODO implement this role
	  action{
		  StandRobot();  
	  }
  }
  
  state(striker){
	  action{
		  Striker();
	  }
  }
  
  
// test states
  state(findBall){
	  action{
		  FindBall();
	  }
  }
  state(standRobot){
        action{
            StandRobot();
        }
    }
}
