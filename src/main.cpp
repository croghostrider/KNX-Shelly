#include <Arduino.h>
#include <WiFiManager.h>
#include <knx.h>
#include <shelly.h>

int32_t HeartbeatValue;
int32_t StartupDelayValue;

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

void ReactionTrue()
{
    bool value = true;
    if (!RelaisDisconnected)
    {
        digitalWrite(RELAYPIN, value);
        KoSwitchStatus.value(value);
        Serial.print("KoSwitchStatus: ");
        Serial.println(value);
    }
    KoBinaryStaus.value(value);
    Serial.print("KoBinaryStaus: ");
    Serial.println(value);
}

void ReactionFalse()
{
    bool value = false;
    if (!RelaisDisconnected)
    {
        digitalWrite(RELAYPIN, value);
        KoSwitchStatus.value(value);
        Serial.print("KoSwitchStatus: ");
        Serial.println(value);
    }
    KoBinaryStaus.value(value);
    Serial.print("KoBinaryStaus: ");
    Serial.println(value);
}

void ReactionToggel()
{
    bool value = !KoBinaryStaus.value();
    if (!RelaisDisconnected)
    {
        digitalWrite(RELAYPIN, value);
        KoSwitchStatus.value(value);
        Serial.print("KoSwitchStatus: ");
        Serial.println(value);
    }
    KoBinaryStaus.value(value);
    Serial.print("KoBinaryStaus: ");
    Serial.println(value);
}

bool startupDelay()
{
    return !delayCheck(gRuntimeData.startupDelay, StartupDelayValue * 1000);
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
    switch (HeartbeatSelection)
    {
        case 0:
            HeartbeatValue = 0;
            break;

        case 1:
            HeartbeatValue = 1;
            break;

        case 2:
            HeartbeatValue = 2;
            break;

        case 5:
            HeartbeatValue = 5;
            break;

        case 10:
            HeartbeatValue = 10;
            break;

        case 30:
            HeartbeatValue = 30;
            break;

        case 60:
            HeartbeatValue = 60;
            break;

        case 120:
            HeartbeatValue = 120;
            break;

        case 121:
            HeartbeatValue = 300;
            break;

        case 122:
            HeartbeatValue = 600;
            break;

        case 123:
            HeartbeatValue = 1800;
            break;

        case 200:
            HeartbeatValue = Heartbeat;
            break;

        default:
            HeartbeatValue = 0;
            break;
    }

    switch (StartupDelaySelection)
    {
        case 1:
            StartupDelayValue = 1;
            break;

        case 2:
            StartupDelayValue = 2;
            break;

        case 5:
            StartupDelayValue = 5;
            break;

        case 10:
            StartupDelayValue = 10;
            break;

        case 30:
            StartupDelayValue = 30;
            break;

        case 60:
            StartupDelayValue = 60;
            break;

        case 120:
            StartupDelayValue = 120;
            break;

        case 121:
            StartupDelayValue = 300;
            break;

        case 122:
            StartupDelayValue = 600;
            break;

        case 123:
            StartupDelayValue = 1800;
            break;

        case 200:
            StartupDelayValue = StartupDelay;
            break;

        default:
            StartupDelayValue = 0;
            break;
    }
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
                switch (OnReactionSelection)
                {

                    case 1:
                        ReactionTrue();
                        break;

                    case 2:
                        ReactionFalse();
                        break;

                    case 3:
                        ReactionToggel();
                        break;

                    default:
                        break;
                }
            }
            else
            {
                Serial.println("Button has been released"); // Here you put whatever code you want to take action when the button is released
                switch (OffReactionSelection)
                {
                    bool value;
                    case 1:
                        ReactionTrue();
                        break;

                    case 2:
                        ReactionFalse();
                        break;

                    case 3:
                        ReactionToggel();
                        break;

                    default:
                        break;
                }
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
                        bool value;
                        case 1:
                            value = true;
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryLongStaus.value(value);
                            Serial.print("KoBinaryLongStaus: ");
                            Serial.println(value);
                            break;

                        case 2:
                            value = false;
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryLongStaus.value(value);
                            Serial.print("KoBinaryLongStaus: ");
                            Serial.println(value);
                            break;

                        case 3:
                            value = !KoBinaryLongStaus.value();
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryLongStaus.value(value);
                            Serial.print("KoBinaryLongStaus: ");
                            Serial.println(value);
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
                        bool value;
                        case 1:
                            value = true;
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryStaus.value(value);
                            Serial.print("KoBinaryStaus: ");
                            Serial.println(value);
                            break;

                        case 2:
                            value = false;
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryStaus.value(value);
                            Serial.print("KoBinaryStaus: ");
                            Serial.println(value);
                            break;

                        case 3:
                            value = !KoBinaryStaus.value();
                            if (!RelaisDisconnected)
                            {
                                digitalWrite(RELAYPIN, value);
                                KoSwitchStatus.value(value);
                                Serial.print("KoSwitchStatus: ");
                                Serial.println(value);
                            }
                            KoBinaryStaus.value(value);
                            Serial.print("KoBinaryStaus: ");
                            Serial.println(value);
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