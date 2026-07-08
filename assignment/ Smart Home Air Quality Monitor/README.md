# Indoor Air Quality Monitoring System

An ESP32-based system that monitors CO/smoke levels and temperature/humidity in a home that uses wood burning for winter heating, and alerts residents locally when air quality drops below safe thresholds — **entirely without needing an internet connection or router**.

## Problem Statement

A family in Milan burns wood for heating during winter. Wood combustion can produce elevated CO and smoke indoors, especially with poor ventilation, a maintenance issue, or a chimney/flue problem. The family needs continuous monitoring with an alert that works reliably even if their home router or internet connection goes down — a moment when relying on a cloud-connected system would be a dangerous single point of failure.

## Solution Approach

The system is built in two tiers, but only Tier 1 is required for the core safety guarantee:

**Tier 1 — Local safety loop (implemented, no network dependency)**
An ESP32 continuously reads the MQ-2 gas sensor and DHT11 temperature/humidity sensor, evaluates the reading against a locally calibrated threshold, and drives an LED + buzzer alert directly. This loop never touches WiFi, MQTT, or any cloud service — it runs identically whether the router is online or completely down.

**Tier 2 — Connectivity layer (future extension, not covered in this repository)**
When WiFi is available, readings and alerts can additionally be published to an MQTT broker for remote visibility, with local buffering to store data during outages and forward it once connectivity returns. This is a nice-to-have layered on top — it never gates or delays the Tier 1 alert.

Key design decisions:
- **Relative thresholds, not absolute ppm.** MQ-2 does not output calibrated ppm without a full Rs/R0 reference curve from its datasheet. The system instead calibrates a clean-air baseline at boot and flags readings as Safe / Elevated / Danger based on multiples of that baseline — an honest approach given the sensor's actual capabilities.
- **Three-tier alerting**, not a single on/off trigger, since a wood stove naturally causes some smoke fluctuation. Green/Yellow/Red LEDs plus distinct buzzer tones for each tier.
- **Non-blocking alert logic.** The danger alert is driven by direct digital writes, never `delay()`-blocked, so the sensor loop keeps running at full responsiveness even while an alarm is active.
- **CSV-formatted Serial logging** so readings can be copied directly into a spreadsheet for analysis or reporting without extra tooling.

## Hardware Required

| Component | Purpose |
|---|---|
| ESP32 dev board | Main controller |
| MQ-2 gas sensor module | Detects CO/smoke/combustible gas (analog output) |
| DHT11 sensor | Temperature and humidity |
| 1x Green LED | Safe status |
| 1x Yellow LED | Elevated/moderate status |
| 1x Red LED | Danger status |
| 3x ~220Ω resistors | Current limiting for LEDs |
| Passive buzzer | Audible alert (3 distinct tones by severity) |
| Breadboard + jumper wires | Prototyping |

## Circuit Diagram

```
                    ┌───────────────────────┐
   MQ-2 sensor      │                       │      DHT11 sensor
   (AO → GPIO34) ──►│                       │◄──  (data → GPIO5)
                     │        ESP32          │
                     │   (main controller)   │
                     │                       │
                     └───────────────────────┘
                        │        │       │       │
                        ▼        ▼       ▼       ▼
                    Green LED  Yellow   Red    Buzzer
                    (GPIO25)   LED      LED   (GPIO14)
                    safe      (GPIO26) (GPIO27)
                              elevated  danger
```

The ESP32 sits at the center as the controller. MQ-2 and DHT11 are inputs feeding sensor data in; the three LEDs and buzzer are outputs the ESP32 drives directly based on the evaluated air quality state.

## Wiring

| Component | ESP32 Pin | Power |
|---|---|---|
| MQ-2 AO (analog out) | GPIO34 | VCC → 5V (VIN) |
| DHT11 data | GPIO5 | VCC → 3.3V |
| Green LED (anode, via ~220Ω resistor) | GPIO25 | GND on cathode side |
| Yellow LED (anode, via ~220Ω resistor) | GPIO26 | GND on cathode side |
| Red LED (anode, via ~220Ω resistor) | GPIO27 | GND on cathode side |
| Buzzer (passive) | GPIO14 | GND |

**Note:** MQ-2 is wired to an ADC1 pin (GPIO32–39) deliberately. ADC2 pins conflict with WiFi on the ESP32 and give unreliable readings once WiFi is active — relevant if this project is extended with Tier 2 connectivity later.

## How It Works

1. **Warm-up (20s):** MQ-2 has a heating element that needs to stabilize before its readings are meaningful. The system waits before taking any readings.
2. **Calibration:** The system takes 50 samples in clean air at boot and averages them into a `baselineMQ2` value. This is the sensor's own personal "normal" reading — MQ-2 does not give a calibrated ppm value out of the box, so all thresholds are defined **relative to this baseline** rather than as absolute numbers.
3. **Threshold evaluation**, every 2 seconds:
   - **SAFE** — MQ-2 reading below 1.5× baseline
   - **ELEVATED** — 1.5×–2.5× baseline
   - **DANGER** — above 2.5× baseline
4. **Local alerts**, driven immediately and independent of any network state:
   - LED: green / yellow / red matching the current state
   - Buzzer: silent (SAFE), short beep at 1000 Hz (ELEVATED), continuous tone at 2500 Hz (DANGER)
5. **Logging:** Every reading is printed to Serial Monitor in CSV format (`timestamp_ms,mq2_raw,mq2_ratio,temperature_c,humidity_pct,state`), so output can be copied directly into a spreadsheet for analysis or reporting.

## Design Notes

- **Why relative thresholds, not absolute ppm:** MQ-2 requires a full Rs/R0 calibration curve (referencing its datasheet) to output trustworthy absolute ppm. Without that, presenting a specific ppm number would be misleading. Comparing against a clean-air baseline is an honest and still-useful approach for detecting a meaningful rise in smoke/gas.
- **Why the buzzer only re-triggers on state change:** Calling `tone()` every loop cycle while already in DANGER state would restart the tone repeatedly rather than sustaining it cleanly. The code tracks `previousState` and only acts when the state actually changes.
- **Why `NA` instead of `0` for failed DHT11 reads:** DHT11 intermittently fails to read even when correctly wired — a known sensor quirk. Logging `0°C` on a failed read would silently corrupt any later analysis; `NA` makes the gap explicit.
- **MQ-2 vs. true CO sensing:** MQ-2 responds broadly to smoke/combustible gases, not CO specifically. It is a reasonable low-cost proxy for this use case, but a dedicated CO sensor (e.g., MQ-7 or an electrochemical CO sensor) would give more targeted and accurate readings if this project is taken further.
- **No network dependency in this tier:** Sensing, threshold logic, and alerting are all self-contained in the local loop, so the system continues to function correctly during a router outage or internet failure — the scenario this project is specifically designed to remain safe through.

## Setup Instructions

1. Wire the components per the table above.
2. Flash the sketch to the ESP32.
3. Open Serial Monitor at **115200 baud**.
4. Let the sensor complete its 20-second warm-up and calibration in genuinely clean air (window open, no combustion nearby).
5. Once calibrated, the system runs continuously — LED and buzzer respond in real time, and Serial Monitor logs every reading as a CSV row.
6. To capture data for analysis: select and copy the Serial Monitor output, paste into a `.csv` file, and open in Excel/Google Sheets.

## Known Limitations

- Timestamps are `millis()`-based (time since boot), not real wall-clock time — acceptable for local testing, but would need an RTC module or NTP sync (only available when online) for absolute timestamps.
- Threshold multipliers (1.5× / 2.5×) are starting points, not empirically validated against real wood-smoke conditions — worth tuning with actual test exposure (e.g., controlled incense/smoke test) before relying on them in a real home.
- This tier has no remote visibility — alerts and data exist only locally at the device (LED/buzzer/Serial). A planned Tier 2 (WiFi/MQTT connectivity with offline buffering) would add remote monitoring while preserving this tier's standalone safety guarantee.
