// Copyright (c) 2022 Niklas Bettgen

#include <AUnitVerbose.h>
#include "./debouncer.h"
// #include "../../lib/timer/timer.h"


template<class T>
class TestableDebouncer : public Debouncer<T> {
 public:
    using Debouncer<T>::Debouncer;
    // Run debounce() function for specific amount of time to simulate hardware states easily
    void runFor(const T value, const Timer::ms time) {
        Timer t;
        t.start(time);
        while (!t.check())
            Debouncer<T>::debounce(value);
    }
};

class DebouncerFixture : public aunit::TestOnce {
 public:
    DebouncerFixture() :
    m_bool_deb(),
    m_int_deb(42),
    m_ulong_deb(69),
    m_byte_deb(99),
    m_enum_deb(100),
    m_str_deb(123) {}

 protected:
    void setup() override {
        TestOnce::setup();
    }
    void teardown() override {
        TestOnce::teardown();
    }

    enum dummy_enum : int16_t { FOO, BAR, FOOBAR, SIXTYNINE };
    TestableDebouncer<bool>             m_bool_deb;
    TestableDebouncer<int16_t>          m_int_deb;
    TestableDebouncer<uint32_t>         m_ulong_deb;
    TestableDebouncer<uint8_t>          m_byte_deb;
    TestableDebouncer<dummy_enum>       m_enum_deb;
    TestableDebouncer<String>           m_str_deb;
};


testF(DebouncerFixture, BasicTimingAndState) {
    assertEqual(m_bool_deb.getThreshold(), 0u);

    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.debounce(true);
    assertTrue(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), true);
    assertEqual(m_bool_deb.getRaw(), true);

    m_bool_deb.runFor(false, 50);
    assertTrue(m_bool_deb.isReady());
    assertTrue(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.reset();
    m_bool_deb.setThreshold(69);
    assertEqual(m_bool_deb.getThreshold(), 69u);

    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.debounce(true);
    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), true);

    m_bool_deb.runFor(false, 42);
    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.runFor(true, 42);
    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), true);

    m_bool_deb.runFor(false, 123);
    assertTrue(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.runFor(true, 123);
    assertTrue(m_bool_deb.isReady());
    assertTrue(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), true);
    assertEqual(m_bool_deb.getRaw(), true);

    m_bool_deb.runFor(false, 42);
    assertTrue(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), true);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.runFor(false, 42);
    assertTrue(m_bool_deb.isReady());
    assertTrue(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);

    m_bool_deb.reset();
    assertEqual(m_bool_deb.getThreshold(), 69u);
    assertFalse(m_bool_deb.isReady());
    assertFalse(m_bool_deb.hasChanged());
    assertEqual(m_bool_deb.getDebounced(), false);
    assertEqual(m_bool_deb.getRaw(), false);
}

testF(DebouncerFixture, TemplateTypes) {
    // int
    m_int_deb.runFor(-10, 10);
    assertFalse(m_int_deb.isReady());
    assertFalse(m_int_deb.hasChanged());
    assertEqual(m_int_deb.getDebounced(), 0);
    assertEqual(m_int_deb.getRaw(), -10);

    m_int_deb.runFor(-20, 50);
    assertTrue(m_int_deb.isReady());
    assertFalse(m_int_deb.hasChanged());
    assertEqual(m_int_deb.getDebounced(), -20);
    assertEqual(m_int_deb.getRaw(), -20);

    m_int_deb.runFor(-30, 60);
    assertTrue(m_int_deb.isReady());
    assertTrue(m_int_deb.hasChanged());
    assertEqual(m_int_deb.getDebounced(), -30);
    assertEqual(m_int_deb.getRaw(), -30);

    m_int_deb.runFor(-40, 20);
    assertTrue(m_int_deb.isReady());
    assertFalse(m_int_deb.hasChanged());
    assertEqual(m_int_deb.getDebounced(), -30);
    assertEqual(m_int_deb.getRaw(), -40);


    // unsigned long
    m_ulong_deb.runFor(123456, 30);
    assertFalse(m_ulong_deb.isReady());
    assertFalse(m_ulong_deb.hasChanged());
    assertEqual(m_ulong_deb.getDebounced(), 0u);
    assertEqual(m_ulong_deb.getRaw(), 123456u);

    m_ulong_deb.runFor(2000000, 80);
    assertTrue(m_ulong_deb.isReady());
    assertFalse(m_ulong_deb.hasChanged());
    assertEqual(m_ulong_deb.getDebounced(), 2000000u);
    assertEqual(m_ulong_deb.getRaw(), 2000000u);

    m_ulong_deb.runFor(3030303, 90);
    assertTrue(m_ulong_deb.isReady());
    assertTrue(m_ulong_deb.hasChanged());
    assertEqual(m_ulong_deb.getDebounced(), 3030303u);
    assertEqual(m_ulong_deb.getRaw(), 3030303u);

    m_ulong_deb.runFor(4444444, 40);
    assertTrue(m_ulong_deb.isReady());
    assertFalse(m_ulong_deb.hasChanged());
    assertEqual(m_ulong_deb.getDebounced(), 3030303u);
    assertEqual(m_ulong_deb.getRaw(), 4444444u);


    // byte
    m_byte_deb.runFor(0x01, 70);
    assertFalse(m_byte_deb.isReady());
    assertFalse(m_byte_deb.hasChanged());
    assertEqual(m_byte_deb.getDebounced(), (byte)0x00);
    assertEqual(m_byte_deb.getRaw(), (byte)0x01);

    m_byte_deb.runFor(0x42, 110);
    assertTrue(m_byte_deb.isReady());
    assertFalse(m_byte_deb.hasChanged());
    assertEqual(m_byte_deb.getDebounced(), (byte)0x42);
    assertEqual(m_byte_deb.getRaw(), (byte)0x42);

    m_byte_deb.runFor(0xFF, 150);
    assertTrue(m_byte_deb.isReady());
    assertTrue(m_byte_deb.hasChanged());
    assertEqual(m_byte_deb.getDebounced(), (byte)0xFF);
    assertEqual(m_byte_deb.getRaw(), (byte)0xFF);

    m_byte_deb.runFor(0x69, 75);
    assertTrue(m_byte_deb.isReady());
    assertFalse(m_byte_deb.hasChanged());
    assertEqual(m_byte_deb.getDebounced(), (byte)0xFF);
    assertEqual(m_byte_deb.getRaw(), (byte)0x69);


    // enum
    m_enum_deb.runFor(dummy_enum::FOO, 70);
    assertFalse(m_enum_deb.isReady());
    assertFalse(m_enum_deb.hasChanged());
    assertEqual(m_enum_deb.getDebounced(), dummy_enum::FOO);
    assertEqual(m_enum_deb.getRaw(), dummy_enum::FOO);

    m_enum_deb.runFor(dummy_enum::BAR, 130);
    assertTrue(m_enum_deb.isReady());
    assertFalse(m_enum_deb.hasChanged());
    assertEqual(m_enum_deb.getDebounced(), dummy_enum::BAR);
    assertEqual(m_enum_deb.getRaw(), dummy_enum::BAR);

    m_enum_deb.runFor(dummy_enum::FOOBAR, 150);
    assertTrue(m_enum_deb.isReady());
    assertTrue(m_enum_deb.hasChanged());
    assertEqual(m_enum_deb.getDebounced(), dummy_enum::FOOBAR);
    assertEqual(m_enum_deb.getRaw(), dummy_enum::FOOBAR);

    m_enum_deb.runFor(dummy_enum::SIXTYNINE, 81);
    assertTrue(m_enum_deb.isReady());
    assertFalse(m_enum_deb.hasChanged());
    assertEqual(m_enum_deb.getDebounced(), dummy_enum::FOOBAR);
    assertEqual(m_enum_deb.getRaw(), dummy_enum::SIXTYNINE);


    // String
    m_str_deb.runFor("foo", 80);
    assertFalse(m_str_deb.isReady());
    assertFalse(m_str_deb.hasChanged());
    assertEqual(m_str_deb.getDebounced(), "");
    assertEqual(m_str_deb.getRaw(), "foo");

    m_str_deb.runFor("bar", 140);
    assertTrue(m_str_deb.isReady());
    assertFalse(m_str_deb.hasChanged());
    assertEqual(m_str_deb.getDebounced(), "bar");
    assertEqual(m_str_deb.getRaw(), "bar");

    m_str_deb.runFor("foobar", 150);
    assertTrue(m_str_deb.isReady());
    assertTrue(m_str_deb.hasChanged());
    assertEqual(m_str_deb.getDebounced(), "foobar");
    assertEqual(m_str_deb.getRaw(), "foobar");

    m_str_deb.runFor("baz!", 99);
    assertTrue(m_str_deb.isReady());
    assertFalse(m_str_deb.hasChanged());
    assertEqual(m_str_deb.getDebounced(), "foobar");
    assertEqual(m_str_deb.getRaw(), "baz!");
}

void setup() {}
void loop() { aunit::TestRunner::run(); }
