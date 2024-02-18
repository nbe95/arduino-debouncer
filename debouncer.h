// Copyright (c) 2022 Niklas Bettgen

#include "Arduino.h"

#ifndef NBE95_ARDUINO_DEBOUNCER_H_
#define NBE95_ARDUINO_DEBOUNCER_H_

#include "./lib/timer/timer.h"


// Debouncer class for any kind of data type
template<class T>
class Debouncer {
 public:
    explicit Debouncer(const Timer::ms threshold = 0) :
    m_value(), m_debounced(), m_timer(threshold) {}

    // Getters for current state
    bool isReady() const            { return m_ready; }         // Check if a valid signal has been recorded yet
    const T& getDebounced() const   { return m_debounced; }     // Fetch debounced value
    const T& getRaw() const         { return m_value; }         // Fetch last recorded raw value
    bool hasChanged();                                          // Check if the signal has changed since the last call

    // Get/set the debouncing threshold
    void setThreshold(const Timer::ms threshold)    { m_timer.setDuration(threshold); }
    Timer::ms getThreshold() const                  { return m_timer.getDuration(); }

    // Setters, action events
    void debounce(const T value);   // Update the internal value and performs the debouncing
    void reset();                   // Reset all internal states

    // Conversion operator
    explicit operator bool() const { return isReady(); }

 protected:
    T       m_value;                // Last recorded raw-value
    T       m_debounced;            // Processed debounced value
    bool    m_changed = false;      // State changed flag (persistent until queried)
    bool    m_ready = false;        // Flag indicating that a valid value has been processed
    Timer   m_timer;                // Internal timer
};


template<class T> void Debouncer<T>::debounce(const T value) {
    // Reset timer upon value change
    m_timer.start();
    if (value != m_value) {
        m_timer.restart();
        m_value = value;
    }

    // Fetch value changes
    if (!m_timer || m_timer.check()) {
        // Set ready flag once we have a stable value
        if (m_debounced != m_value) {
            m_debounced = m_value;
            m_changed |= m_ready;
        }
        m_ready = true;
    }
}

template<class T> void Debouncer<T>::reset() {
    m_ready = false;
    m_changed = false;
    m_timer.reset();
}

template<class T> bool Debouncer<T>::hasChanged() {
    // Note: This method is 'self-resetting'!
    // Call only once and store the return value as necessary
    bool changed = m_changed;
    m_changed = false;
    return changed;
}


// Class for buttons, switches and any type of raw binary sensors
class DebouncedSwitch : public Debouncer<bool> {
 public:
    explicit DebouncedSwitch(const uint8_t pin, const Timer::ms threshold = 0, const bool pullup = true) :
    Debouncer(threshold), m_pin(pin), m_pullup(pullup) {}

    // Switch status for easy handling
    virtual void    debounce()              { Debouncer::debounce(digitalRead(m_pin) == m_pullup ? LOW : HIGH); }
    virtual bool    isOpen() const          { return !getDebounced(); }
    virtual bool    isClosed() const        { return getDebounced(); }

 protected:
    const uint8_t   m_pin;
    const bool      m_pullup;
};

#endif  // NBE95_ARDUINO_DEBOUNCER_H_
