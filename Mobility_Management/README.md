# Power and Sense Management

This section documents the power system and sensing capabilities of our robot.

Sensors used:

- 3x Ultrasonic sensors: obstacle detection and distance measurement.
- HuskyLens AI Vision Sensor: real-time object and color detection.
- TCS34725 Color Sensor: precise color sensing.
- HMC5883L Magnetometer: absolute directional control.

Power system:

- Powered by battery pack (specify type if needed).
- Efficient power distribution ensuring reliable sensor and motor performance.

Relevant diagrams:
- See `schemes/` folder (ElectricDiagram.png).

Software integration:
- I2C-based communication with HuskyLens and compass.
- Optimized Arduino code using Wire.h and Adafruit libraries.
