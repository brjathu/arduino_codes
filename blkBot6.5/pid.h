


#define triggerLFront  23
#define echoLFront     22
#define maxDistance 180
float distanceLeftFront = 0;
float distanceLeftFrontLast = 0;
NewPing sonarLeftFront(triggerLFront, echoLFront, maxDistance);
///////////////////////////////////////////////right front sonar
#define triggerRFront  25
#define echoRFront     24
float distanceRightFront = 0;
float distanceRightFrontLast = 0;
NewPing sonarRightFront(triggerRFront, echoRFront, maxDistance);

/////////////////////////////////////////left back sonar
#define triggerLBack  29
#define echoLBack     28
#define maxDistance 200
float distanceLeftBack = 0;
float distanceLeftBackLast = 0;
NewPing sonarLeftBack(triggerLBack, echoLBack, maxDistance);
///////////////////////////////////////////////right back sonar
#define triggerRBack  27
#define echoRBack     26
float distanceRightBack = 0;
float distanceRightBackLast = 0;
NewPing sonarRightBack(triggerRBack, echoRBack, maxDistance);

int pidMax;
float error1;
float error2;



void pid() {
  //  if (dis > 20) {    // right wall following
  //
  //    error = (7.5 - disRight);
  //
  //    speed = constrain((error * 6 + 50 * (error - last_errorR) ), -pidMax, pidMax);
  //
  //    last_errorR = error;
  //
  //  }
  //  else if (disRight > 20) {    // left wall following
  //
  //    error = (disLeft - 7.5);
  //    speed = constrain((error * 6 + 50 * (error - last_errorL) ), -pidMax, pidMax);
  //
  //    last_errorL = error;
  //  }
  //
  //
  //  else {
  //
//  sonarFront();
 // sonarBack();
 // sonarForward();
  pidMax = 120;
  if ((distanceLeftFront == 30) || (distanceLeftBack == 30)) { //right wall follow
    //  md.setBrakes(400,400);
    //  delay(5000);
    error = (7.5 - (0.5 * (distanceRightFront + distanceRightBack))) - (distanceRightFront - distanceRightBack);

    speed = constrain((error * 8 + 160 * (error - last_errorR) ), -pidMax, pidMax);

    last_errorR = error;
  }
  else if (((distanceRightFront) == 30) || (distanceRightBack == 30)) { // left wall follow
    //  md.setBrakes(400,400);
    //  delay(5000);
    error = ((0.5 * (distanceLeftBack + distanceLeftFront)) - 7.5) + (distanceLeftFront - distanceLeftBack);
    speed = constrain((error * 8 + 150 * (error - last_errorL) ), -pidMax, pidMax);

    last_errorL = error;
  }
  else if (((distanceRightFront == 30) && (distanceLeftFront == 30)) || ((distanceRightBack == 30) && (distanceLeftBack == 30))) {
//    errorEncoder = ((countL) - countR);
//    speedEncoder = constrain(0.2 * errorEncoder + 0.2  * (errorEncoder - last_errorEncoder) , -50, 50);
//    last_errorEncoder = errorEncoder;
//    speed = speedEncoder;
    //forwardEncoder();
    speed = 10;
  }
  else {
    pidMax = 120;
    error1 = 0.5 * ((distanceLeftFront - distanceRightFront) + (distanceLeftBack - distanceRightBack));
    error2 = 0.5 * ((distanceLeftFront - distanceLeftBack) - (distanceRightFront - distanceRightBack)); //note
    error = error1 + error2;
    pTerm = Kp * (error);
    integrated_error += error;
    iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
    dTerm = Kd * (error - last_error);
    last_error = error;
    speed = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
    //  pTerm = 6 * (error1);
    //  integrated_error += error1;
    //  iTerm = Ki * constrain(integrated_error, -GUARD_GAIN, GUARD_GAIN);
    //  dTerm = Kd * (error1 - last_error);
    //  last_error = error1;
    //  speed1 = constrain(K * (pTerm + dTerm + iTerm), -pidMax, pidMax);
    //  //----------------------------------------------------------------------------
    //  pTerm2 = 12 * (error2);
    //  integrated_error2 += error2;
    //  iTerm2 = Ki * constrain(integrated_error2, -GUARD_GAIN, GUARD_GAIN);
    //  dTerm2 = Kd * (error2 - last_error2);
    //  last_error2 = error2;
    //  speed2 = constrain(K * (pTerm2 + dTerm2 + iTerm2), -pidMax, pidMax);
  }
}
