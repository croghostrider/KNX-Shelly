#include <Arduino.h>
#include <WiFiManager.h>
#include <knx.h>
#include <shelly.h>

struct sParameters
{
    int32_t HeartbeatValue;
    int32_t StartupDelayValue;
};

sParameters gParameters;

struct sRuntimeInfo
{
    unsigned long startupDelay;
    unsigned long heartbeatDelay;
};

sRuntimeInfo gRuntimeData;

bool delayCheck(uint32_t iOldTimer, uint32_t iDuration)
{
    return millis() - iOldTimer >= iDuration;
}

// callback from switch-GO
void switchCallback(GroupObject &go)
{
    if ((KoSwitchLock.value() && LockWithFalse == 0) || (!KoSwitchLock.value() && LockWithFalse == 1))
        return;

    bool value = KoSwitch.value();
    digitalWrite(RELAYPIN, value);
    KoSwitchStatus.value(value);
    Serial.print("KoSwitchStatus: ");
    Serial.println(value);
}

void ReactionFuntion(bool isToggel, bool valueFuntion, bool isLong)
{
    bool value;
    if (isToggel)
    {
        value = !KoBinaryStaus.value();
    }
    else
    {
        value = valueFuntion;
    }

    if (!RelaisDisconnected)
    {
        digitalWrite(RELAYPIN, value);
        KoSwitchStatus.value(value);
        Serial.print("KoSwitchStatus: ");
        Serial.println(value);
    }
    if (isLong)
    {
        KoBinaryLongStaus.value(value);
        Serial.print("KoBinaryLongStaus: ");
        Serial.println(value);
    }
    else
    {
        KoBinaryStaus.value(value);
        Serial.print("KoBinaryStaus: ");
        Serial.println(value);
    }
}

void runReaction(const int Reaction)
{
    switch (Reaction)
    {
        case 1:
            ReactionFuntion(false, true, false);
            break;

        case 2:
            ReactionFuntion(false, false, false);
            break;

        case 3:
            ReactionFuntion(true, false, false);
            break;

        default:
            break;
    }
}

uint32_t setSeletionValue(uint8_t SelectionParameter)
{
    switch (SelectionParameter)
    {
        case 0:
            return 0;
            break;

        case 1:
            return 1;
            break;

        case 2:
            return 2;
            break;

        case 5:
            return 5;
            break;

        case 10:
            return 10;
            break;

        case 30:
            return 30;
            break;

        case 60:
            return 60;
            break;

        case 120:
            return 120;
            break;

        case 121:
            return 300;
            break;

        case 122:
            return 600;
            break;

        case 123:
            return 1800;
            break;

        default:
            return 0;
            break;
    }
}

bool startupDelay()
{
    return !delayCheck(gRuntimeData.startupDelay, gParameters.StartupDelayValue * 1000);
}

void PrintParameters()
{
    Serial.print("HeartbeatSelection: ");
    Serial.println(HeartbeatSelection);
    Serial.print("Heartbeat: ");
    Serial.println(Heartbeat);
    Serial.print("StartupDelaySelection: ");
    Serial.println(StartupDelaySelection);
    Serial.print("StartupDelay: ");
    Serial.println(StartupDelay);
    Serial.print("LockWithFalse: ");
    Serial.println(LockWithFalse);
    Serial.print("RelaisDisconnected: ");
    Serial.println(RelaisDisconnected);

    Serial.print("LongPress: ");
    Serial.println(LongPress);
    Serial.print("InputDefault: ");
    Serial.println(InputDefault);
    Serial.print("ShortReaction: ");
    Serial.println(ShortReaction);
    Serial.print("LongReaction: ");
    Serial.println(LongReaction);
    Serial.print("DebounceDelaySelection: ");
    Serial.println(DebounceDelaySelection);
    Serial.print("LongPressDelay: ");
    Serial.println(LongPressDelay);
    Serial.print("OnReactionSelection: ");
    Serial.println(OnReactionSelection);
    Serial.print("OffReactionSelection: ");
    Serial.println(OffReactionSelection);
}

void SetParameters()
{
    gParameters.HeartbeatValue = setSeletionValue(HeartbeatSelection);
    gParameters.StartupDelayValue = setSeletionValue(StartupDelaySelection);
    if (HeartbeatSelection == 200)
        gParameters.HeartbeatValue = Heartbeat;

    if (StartupDelaySelection == 200)
        gParameters.StartupDelayValue = StartupDelay;
}

void ButtonToggle()
{
    uint32_t currentMillis = millis();                 // Millis times uses to debounce the button
    static uint32_t lastMillis;                        // Start of the debounce timeout
    bool currentButtonState = digitalRead(SWINPUTPIN); // Reads the current state of the button and saves the result in a bool
    static bool lastButtonState = HIGH;                // Holds the previous debounced state of the button, initialised as HIGH because the input is high when the button is not pressed

    if (lastButtonState != currentButtonState)
    { // Checks to see if the button has been pressed or released, at this point the button has not been debounced
        if (currentMillis - lastMillis >= DebounceDelaySelection)
        {                                         // Checks to see if the state of the button has been stable for at least bounceTimeout duration
            lastButtonState = currentButtonState; // At this point the button has been debounced, so save the last state
            if (currentButtonState == LOW)
            {                                              // The button might have been pressed or released, this make sure only presses are acted on, not releases
                Serial.println("Button has been pressed"); // Here you put whatever code you want to take action when the button is pressed
                runReaction(OnReactionSelection);
            }
            else
            {
                Serial.println("Button has been released"); // Here you put whatever code you want to take action when the button is released
                runReaction(OffReactionSelection);
            }
        }
    }
    else
    {
        lastMillis = currentMillis; // Saves the current value of millis in last millis so the debounce timer starts from current millis
    }
}

void ButtonLongPress()
{
    uint32_t currentMillis = millis();                 // Millis times uses to debounce the button
    static uint32_t lastMillis;                        // Start of the timeouts
    bool currentButtonState = digitalRead(SWINPUTPIN); // Reads the current state of the button and saves the result in a bool
    static bool lastButtonState = HIGH;                // Holds the previous debounced state of the button

    if (lastButtonState != currentButtonState)
    {                                         // Checks to see if the button has been pressed or released, at this point the button has not been debounced
        lastButtonState = currentButtonState; // This is to ensure the following code only responds once to each press or release of the button
        if (currentButtonState == LOW)
        {
            lastMillis = currentMillis; // Save the value of millis to start the timer
        }
        else
        {
            if (currentMillis - lastMillis >= DebounceDelaySelection)
            { // Checks that at least the debounc time has elapsed since lastMillis was updated
                if (currentMillis - lastMillis >= LongPressDelay)
                {                                 // Checks to see if longPressTime has been exceeded
                    Serial.println("Long press"); // LongPressTime has been exceeded
                    switch (LongReaction)
                    {
                        case 1:
                            ReactionFuntion(false, true, true);
                            break;

                        case 2:
                            ReactionFuntion(false, false, true);
                            break;

                        case 3:
                            ReactionFuntion(true, false, true);
                            break;

                        default:
                            break;
                    }
                }
                else
                {
                    Serial.println("Short press"); // LongPressTime has not been exceeded
                    switch (ShortReaction)
                    {
                        case 1:
                            ReactionFuntion(false, true, false);
                            break;

                        case 2:
                            ReactionFuntion(false, false, false);
                            break;

                        case 3:
                            ReactionFuntion(true, false, false);
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }
}

void Button()
{
    if (LongPress)
    {
        ButtonLongPress();
    }
    else
    {
        ButtonToggle();
    }
}

void ProcessHeartbeat()
{
    // the first heartbeat is send directly after startup delay of the device
    if (gRuntimeData.heartbeatDelay == 0 || delayCheck(gRuntimeData.heartbeatDelay, Heartbeat * 1000))
    {
        // we waited enough, let's send a heartbeat signal
        KoHeartbeat.value(true);

        gRuntimeData.heartbeatDelay = millis();
        // The module prints its firmware version to the console
    }
}

void setup()
{
    Serial.begin(115200);
    ArduinoPlatform::SerialDebug = &Serial;
    pinMode(SWINPUTPIN, INPUT_PULLUP);
    pinMode(RELAYPIN, OUTPUT);
    WiFiManager wifiManager;
    wifiManager.autoConnect("KNX-Shelly");
    knx.readMemory();
    if (knx.configured())
    {
        PrintParameters();
        // register callback for reset GO
        KoSwitch.callback(switchCallback);
        KoSwitch.dataPointType(Dpt(1, 1));
        KoSwitchLock.dataPointType(Dpt(1, 2));
        KoSwitchStatus.dataPointType(Dpt(1, 1));
        KoHeartbeat.dataPointType(Dpt(1, 2));
        KoBinaryStaus.dataPointType(Dpt(1, 1));
        KoBinaryLongStaus.dataPointType(Dpt(1, 1));

        if (LockWithFalse == 1)
            KoSwitchLock.value(true);

        SetParameters();
    }
    knx.start();
}

void loop()
{
    knx.loop();
    if (!knx.configured())
        return;

    if (startupDelay())
        return;

    ProcessHeartbeat();

    Button();
}