
#include <Arduino.h>
#include <WiFiManager.h>
#include <knx.h>
#include <shelly.h>

// callback from switch-GO
void switchCallback(GroupObject &go)
{
    if ((KoSwitchLock.value() && Lock == 0) || (!KoSwitchLock.value() && Lock == 1))
        return;

    bool value = KoSwitch.value();
    digitalWrite(RELAYPIN, value);
    KoSwitchStatus.value(value);
    Serial.print("KoSwitchStatus: ");
    Serial.println(value);
}

void ButtonToggle()
{
    uint32_t currentMillis = millis();                 // Millis times uses to debounce the button
    static uint32_t lastMillis;                        // Start of the debounce timeout
    bool currentButtonState = digitalRead(SWINPUTPIN); // Reads the current state of the button and saves the result in a bool
    static bool lastButtonState = HIGH;                // Holds the previous debounced state of the button, initialised as HIGH because the input is high when the button is not pressed

    if (lastButtonState != currentButtonState)
    { // Checks to see if the button has been pressed or released, at this point the button has not been debounced
        if (currentMillis - lastMillis >= debounceDelay)
        {                                         // Checks to see if the state of the button has been stable for at least bounceTimeout duration
            lastButtonState = currentButtonState; // At this point the button has been debounced, so save the last state
            if (currentButtonState == LOW)
            {                                              // The button might have been pressed or released, this make sure only presses are acted on, not releases
                Serial.println("Button has been pressed"); // Here you put whatever code you want to take action when the button is pressed
                switch (OnReaction)
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
            else
            {
                Serial.println("Button has been released"); // Here you put whatever code you want to take action when the button is released
                switch (OffReaction)
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
            if (currentMillis - lastMillis >= debounceDelay)
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

void setup()
{
    Serial.begin(115200);
    ArduinoPlatform::SerialDebug = &Serial;
    pinMode(SWINPUTPIN, INPUT_PULLUP);
    pinMode(RELAYPIN, OUTPUT);
    WiFiManager wifiManager;
    wifiManager.autoConnect("knx-demo");
    knx.readMemory();
    if (knx.configured())
    {
        Serial.print("StartupDelayAlive: ");
        Serial.println(StartupDelayAlive);
        Serial.print("StartupDelaySelection: ");
        Serial.println(StartupDelaySelection);
        Serial.print("Heartbeat: ");
        Serial.println(Heartbeat);
        Serial.print("Lock: ");
        Serial.println(Lock);
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
        Serial.print("debounceDelay: ");
        Serial.println(debounceDelay);
        Serial.print("StartupDelayRestart: ");
        Serial.println(StartupDelayRestart);
        // register callback for reset GO
        KoSwitch.callback(switchCallback);
        KoSwitch.dataPointType(Dpt(1, 1));
        KoSwitchLock.dataPointType(Dpt(1, 2));
        KoSwitchStatus.dataPointType(Dpt(1, 1));
        KoHeartbeat.dataPointType(Dpt(1, 2));
        KoBinaryStaus.dataPointType(Dpt(1, 1));
        KoBinaryLongStaus.dataPointType(Dpt(1, 1));

        if (Lock == 1)
            KoSwitchLock.value(true);
    }
    knx.start();
}

void loop()
{
    knx.loop();
    if (!knx.configured())
        return;

    Button();
}