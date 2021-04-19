/*
  Control the power to a system using the SparkFun Soft Power Switch
  By: Nathan Seidle
  SparkFun Electronics
  Date: February 2nd, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  This example assumes the Fast Off pin is connected.

  A simple tap of the power button will turn on the system. If the power button is not being pressed 
  (accidental tap) the system will turn off after ~20ms. If the system turns on and sees the power 
  button held for 0.5s, it will begin normal operation.

  During normal system operation, if user presses the power button for 2s, the system
  will shut down. This means the power button can also be used as a general 'select' button as
  long as user doesn't hold button for more than 2s.

  This example will output serial states. If you are powering your board over a USB connection then
  the power button can't do its job. We did our testing by connecting the TX and GND pins of a RedBoard to 
  an external USB to Serial adapter. That way the battery + Soft Power Switch could control power
  to the board.  
*/

//Hardware connections
//Nearly any GPIO will do but avoid GPIOs with aut-enabled pullups, ie ESP32 pins 0, 5, 15.
int POWER_BUTTON = 14;
int FAST_OFF = 32;

int STAT_LED = 13;

long powerPressedStartTime = 0;

int debounceDelay = 20;

//Uncomment the following line to turn on shutdown time measurements
//#define PRINT_TIMER_OUTPUT

void setup()
{
  Serial.begin(115200);
  Serial.println("Soft Power Switch example");

  pinMode(POWER_BUTTON, INPUT_PULLUP);

  //User has pressed the power button to turn on the system
  //Was it an accidental bump or do they really want to turn on?
  //Let's make sure they continue to press for two seconds
  Serial.print("Initial power on check.");
  powerPressedStartTime = millis();
  while (digitalRead(POWER_BUTTON) == LOW)
  {
    //Wait for user to stop pressing button.
    //What if user has left something heavy pressing the power button?
    //The soft power switch will automatically turn off the system! Handy.
    delay(100);

    if (millis() - powerPressedStartTime > 500)
      break;
    Serial.print(".");
  }
  Serial.println();

  if (millis() - powerPressedStartTime < 500)
  {
    Serial.println("Power button tap. Returning to off state. Powering down.");
    fastPowerDown();
  }

  Serial.println("User wants to turn system on!");
  powerPressedStartTime = 0; //Reset var to return to normal 'on' state

  //Here we display something to user indicating system is on and running
  //For example an external display or LED turns on
  pinMode(STAT_LED, OUTPUT);
  digitalWrite(STAT_LED, HIGH);

  Serial.println("Press 'r' to enter infinite loop to test power-down override");
  Serial.println("Press 'z' to do a slow powerdown");
  Serial.println("Press and hold power button for 2s to do a fast powerdown");
}

void loop()
{
  if (Serial.available())
  {
    byte incoming = Serial.read();

    if (incoming == 'z')
    {
      Serial.println("Slow power down");
      slowPowerDown();
    }
    else if (incoming == 'r')
    {
      Serial.println("System locked. Now hold power button to force power down without using software.");

      //Here we wait for user press button so we can time it
      while (digitalRead(POWER_BUTTON) == HIGH) delay(1);

      powerPressedStartTime = millis();
      Serial.print("Doing nothing, waiting for power override to kick in");
      while (1)
      {
#ifdef PRINT_TIMER_OUTPUT
        Serial.println(millis() - powerPressedStartTime);
#endif
        delay(1);

        if(digitalRead(POWER_BUTTON) == HIGH) break;
      }
      Serial.println("User released button before forced powered could complete. Try again, but hold power button for 15s.");
    }
  }

  if (digitalRead(POWER_BUTTON) == LOW && powerPressedStartTime == 0)
  {
    //Debounce check
    delay(debounceDelay);
    if (digitalRead(POWER_BUTTON) == LOW)
    {
      Serial.println("User is pressing power button. Start timer.");
      powerPressedStartTime = millis();
    }
  }
  else if (digitalRead(POWER_BUTTON) == LOW && powerPressedStartTime > 0)
  {
    //Debounce check
    delay(debounceDelay);
    if (digitalRead(POWER_BUTTON) == LOW)
    {
      if ((millis() - powerPressedStartTime) > 2000)
      {
        Serial.println("Time to power down!");
        fastPowerDown();
      }
    }
  }
  else if (digitalRead(POWER_BUTTON) == HIGH && powerPressedStartTime > 0)
  {
    //Debounce check
    delay(debounceDelay);
    if (digitalRead(POWER_BUTTON) == HIGH)
    {
      Serial.print("Power button released after ms: ");
      Serial.println(millis() - powerPressedStartTime);
    }
    powerPressedStartTime = 0; //Reset var to return to normal 'on' state
  }
}

//Immediately power down
void fastPowerDown()
{
  //Indicate to user we are shutting down
  digitalWrite(STAT_LED, LOW);

  pinMode(POWER_BUTTON, OUTPUT);
  digitalWrite(POWER_BUTTON, LOW);

  pinMode(FAST_OFF, OUTPUT);
  digitalWrite(FAST_OFF, LOW);

  powerPressedStartTime = millis();
  Serial.print("Pulling power line low");
  while (1)
  {
#ifdef PRINT_TIMER_OUTPUT
    Serial.println(millis() - powerPressedStartTime);
#endif
    delay(1);
  }
}

//This is the less common method for power down. It's basically
//the same as holding the power button but a little faster.
//This function is useful for seeing how long a system would take to power
//down if the fast power down method was not used.
void slowPowerDown()
{
  //Indicate to user we are shutting down
  digitalWrite(STAT_LED, LOW);

  pinMode(POWER_BUTTON, OUTPUT);
  digitalWrite(POWER_BUTTON, LOW);

  powerPressedStartTime = millis();
  Serial.print("Pulling power line low");
  while (1)
  {
#ifdef PRINT_TIMER_OUTPUT
    Serial.println(millis() - powerPressedStartTime);
#endif
    delay(1);
  }
}
