/* -*- mode: c++; c-basic-offset: 2; tab-width: 2; -*- */
#include <avr/pgmspace.h>
#include <Arduino.h>

#include "console.h"
#include "mc6850.h"
#include "pins.h"
#include "pins_map.h"

class Pins Pins;
class Mc6850 Mc6850(Pins::ioBaseAddress());

static uint8_t data;

void ioRequest() {
  const uint16_t ioAddr = Pins.ioRequestAddress();
  const bool ioWrite = Pins.ioRequestWrite();
  if (Mc6850.isSelected(ioAddr)) {
    if (ioWrite) Mc6850.write(Pins.ioGetData(), ioAddr);
    else Pins.ioSetData(Mc6850.read(ioAddr));
  } else {
    if (ioWrite) data = Pins.ioGetData();
    else Pins.ioSetData(data);
  }

  Pins.acknowledgeIoRequest();
  Pins.leaveIoRequest();
}

void Pins::loop() {
  Mc6850.loop();
}

#define __concat2__(a,b) a##b
#define BP(name)  name##_PIN
#define BM(name)   _BV(BP(name))
#define BUS(name)  (name ## _BUS)
#define __PORT__(name) name##_PORT
#define __DDR__(port) __concat2__(DDR,port)
#define __POUT__(port) __concat2__(PORT,port)
#define __PIN__(port) __concat2__(PIN,port)
#define DDR(name) __DDR__(__PORT__(name))
#define PORT(name) __POUT__(__PORT__(name))
#define PIN(name) __PIN__(__PORT__(name))
#define pinMode(name, mode) do {                      \
    if (mode == INPUT) DDR(name) &= ~BM(name);        \
    if (mode == INPUT_PULLUP) DDR(name) &= ~BM(name); \
    if (mode == INPUT_PULLUP) PORT(name) |= BM(name); \
    if (mode == OUTPUT) DDR(name) |= BM(name);        \
  } while (0)
#define digitalRead(name) (PIN(name) & BM(name))
#define digitalWrite(name, val) do {            \
    if (val == LOW) PORT(name) &= ~BM(name);    \
    if (val == HIGH) PORT(name) |= BM(name);    \
  } while (0)

uint8_t Pins::Dbus::getDbus() {
  return PIN(DB);
}

void Pins::Dbus::begin() {
  setDbus(INPUT, 0);
}

void Pins::Dbus::setDbus(uint8_t dir, uint8_t data) {
  if (dir == OUTPUT && digitalRead(RD_WR) == LOW) {
    Console.println(F("!! R/W is LOW"));
    return;
  }
  if (dir == OUTPUT || _capture) {
    digitalWrite(RAM_E, HIGH); // disable RAM
  } else {
    digitalWrite(RAM_E, LOW);  // enable RAM
  }
  _dir = dir;
  if (dir == INPUT) {
    DDR(DB) = 0x00;
  } else {
    DDR(DB) = 0xFF;
    PORT(DB) = data;
  }
}

void Pins::Dbus::input() {
  setDbus(INPUT, 0);
}

void Pins::Dbus::output() {
  if (_valid) {
    _valid = false;
    setDbus(OUTPUT, _data);
  } else {
    input();
  }
}

void Pins::Dbus::set(uint8_t data) {
  _data = data;
  _valid = true;
}

void Pins::Dbus::capture(bool enabled) {
  _capture = enabled;
}

void Pins::Status::get() {
  uint8_t p = 0;
  if (digitalRead(BA))    p |= Status::ba;
  if (digitalRead(BS))    p |= Status::bs;
  if (digitalRead(RESET)) p |= Status::reset;
  if (digitalRead(HALT))  p |= Status::halt;
  if (digitalRead(LIC))   p |= Status::lic;
  if (digitalRead(AVMA))  p |= Status::avma;
  if (digitalRead(RD_WR)) p |= Status::rw;
  pins = p;
  dbus  = Dbus::getDbus();
}

bool Pins::unchanged() const {
  return _signals.pins == _previous.pins
    && _signals.dbus == _previous.dbus;
}

void Pins::print() const {
  Console.print(_signals.pins & Status::lic   ? 'L' : ' ');
  Console.print(_signals.pins & Status::avma  ? 'A' : ' ');
  Console.print(_signals.pins & Status::rw    ? 'R' : 'W');
  Console.print(_signals.pins & Status::halt  ? 'H' : ' ');
  Console.print(_signals.pins & Status::babs, DEC);
  Console.print(_signals.pins & Status::reset ? ' ' : 'R');

  Console.print(F(" DB=0x"));
  printHex8(_signals.dbus);

  Console.print(' ');
  if (vectorFetch()) {
    Console.print('V');
  } else if (running()) {
    if (writeCycle()) {
      Console.print('W');
    } else if (readCycle()) {
      Console.print('R');
    } else {
      Console.print('-');
    }
  } else {
    Console.print('H');
  }
  Console.println();
}

bool Pins::inHalt() const {
  return (_signals.pins & Status::babs) == Status::babs;
}

bool Pins::vectorFetch() const {
  return (_signals.pins & Status::babs) == Status::bs;
}

bool Pins::running() const {
  return (_signals.pins & Status::babs) == 0;
}

bool Pins::lastInstCycle() const {
  return _signals.pins & Status::lic;
}

bool Pins::writeCycle() const {
  return (_previous.pins & Status::avma) && (_signals.pins & Status::rw) == 0;
}

bool Pins::readCycle() const {
  return (_previous.pins & Status::avma) && (_signals.pins & Status::rw);
}

void Pins::reset(bool show) {
  digitalWrite(RESET, LOW);
  digitalWrite(HALT, HIGH);
  delayMicroseconds(10);
  digitalWrite(STEP, LOW);
  digitalWrite(RESET, HIGH);
  do {
    cycle();
    digitalWrite(HALT, LOW);
    if (show) print();
  } while (!inHalt());
  digitalWrite(STEP, HIGH);
}

void Pins::cycle() {
  _previous = _signals;
  _signals.get();
  if (running()) {
    if (writeCycle()) {
      _dbus.input();
    } else if (readCycle()) {
      _dbus.output();
    }
  } else {
    _dbus.input();
  }
  _signals.get();
  digitalWrite(ACK, LOW);
  delayMicroseconds(4);
  _dbus.input();
  digitalWrite(ACK, HIGH);
  delayMicroseconds(4);
}

void Pins::run() {
  digitalWrite(HALT, HIGH);
  digitalWrite(STEP, HIGH);
  digitalWrite(USR_LED, LOW);
  _freeRunning = true;
  attachInterrupt(INT_INTERRUPT, ioRequest, FALLING);
}

void Pins::halt(bool show) {
  detachInterrupt(INT_INTERRUPT);
  digitalWrite(STEP, LOW);
  delayMicroseconds(10);
  digitalWrite(HALT, LOW);
  do {
    cycle();
    if (show) print();
  } while (running());
  digitalWrite(STEP, HIGH);
  digitalWrite(USR_LED, HIGH);
  _freeRunning = false;
}

void Pins::setData(uint8_t data) {
  _dbus.set(data);
}

void Pins::unhalt() {
  digitalWrite(STEP, LOW);
  delayMicroseconds(10);
  digitalWrite(HALT, HIGH);
  do {
    cycle();
    digitalWrite(HALT, LOW);
  } while (!running() || !lastInstCycle());
}

void Pins::execInst(const uint8_t *inst, uint8_t len, bool show) {
  unhalt();
  for (uint8_t i = 0; i < len; i++) {
    setData(inst[i]);
    cycle();
    if (show) print();
  }
  while (!lastInstCycle()) {
    cycle();
    if (show) print();
  }
  digitalWrite(STEP, HIGH);
}

void Pins::captureWrites(const uint8_t *inst, uint8_t len, uint8_t *buf, uint8_t max) {
  unhalt();
  for (uint8_t i = 0; i < len; i++) {
    setData(inst[i]);
    cycle();
  }
  _dbus.capture(true);
  while (!lastInstCycle()) {
    cycle();
    if (writeCycle() && max > 0) {
      *buf++ = _signals.dbus;
      max--;
    }
  }
  _dbus.capture(false);
  digitalWrite(STEP, HIGH);
}

void Pins::step(bool show) {
  unhalt();
  do {
    cycle();
    if (show) print();
  } while (!lastInstCycle());
  digitalWrite(STEP, HIGH);
}

void Pins::begin() {
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  pinMode(HALT,  OUTPUT);
  digitalWrite(HALT, LOW);
  pinMode(IRQ, OUTPUT);
  digitalWrite(IRQ, HIGH);

  pinMode(STEP, OUTPUT);
  pinMode(ACK, OUTPUT);
  pinMode(INT, INPUT_PULLUP);
  digitalWrite(STEP, LOW);
  digitalWrite(ACK, HIGH);

  pinMode(BS,    INPUT);
  pinMode(BA,    INPUT);
  pinMode(LIC,   INPUT);
  pinMode(AVMA,  INPUT);
  pinMode(RD_WR, INPUT_PULLUP);
  pinMode(RAM_E, OUTPUT);
  digitalWrite(RAM_E, HIGH);

  pinMode(ADR0, INPUT_PULLUP);
  pinMode(ADR1, INPUT_PULLUP);

  pinMode(USR_SW, INPUT_PULLUP);
  pinMode(USR_LED, OUTPUT);
  digitalWrite(USR_LED, HIGH);

  _dbus.begin();

  _previous.get();

  reset();
}

void Pins::assertIrq(uint8_t mask) {
  _irq |= mask;
  if (_irq)
    digitalWrite(IRQ, LOW);
}

void Pins::negateIrq(uint8_t mask) {
  _irq &= ~mask;
  if (_irq == 0)
    digitalWrite(IRQ, HIGH);
}

uint16_t Pins::ioRequestAddress() const {
  uint16_t addr = ioBaseAddress();
  if (digitalRead(ADR0)) addr |= 0x01;
  if (digitalRead(ADR1)) addr |= 0x02;
  return addr;
}

bool Pins::ioRequestWrite() const {
  return digitalRead(RD_WR) == LOW;
}

uint8_t Pins::ioGetData() {
  _dbus.input();
  return Dbus::getDbus();
}

void Pins::ioSetData(uint8_t data) {
  _dbus.set(data);
  _dbus.output();
}

void Pins::acknowledgeIoRequest() {
  digitalWrite(ACK, LOW);
}

void Pins::leaveIoRequest() {
  _dbus.input();
  digitalWrite(ACK, HIGH);
}

void Pins::attachUserSwitch(void (*isr)()) const {
  attachInterrupt(USR_SW_INTERRUPT, isr, FALLING);
}
