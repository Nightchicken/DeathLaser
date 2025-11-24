# Goals of the Project
Create a tracking heat tracking laser turret using the ESP32-H2

# Tasks

1. Camera reading
2. Servo movement and limits
3. Laser on/off
4. Optional web based controller




Design and Fabrication of a Dual-Axis Servo Turret Mounts
Designed & Created by: Joel Bowman

1.	Motivation

The design of a dual-axis turret serves as a practical demonstration of the ESP-32 microcontroller’s capabilities. Although the ESP-32 is based on a RISC architecture with a reduced instruction set, it remains highly capable of generating precise pulse-width modulation (PWM) signals required for servo control. By integrating two MG995 servos, the system achieves motion along both the X and Y axes, enabling accurate positioning and tracking. When equipped with a camera, the turret can collect visual data and execute real-time tracking operations, showcasing the ESP-32’s ability to coordinate hardware and software tasks. This project therefore highlights not only the mechanical design and fabrication of servo mounts, but also the broader technical achievement of combining embedded control, signal modulation, and data-driven actuation in a cohesive system.
2.	Objectives

1.	To design, fabricate, and test servo brackets for MG995 servos enabling dual-axis rotation.          
2.	Mounts must securely hold the MG995 servos. 
3.	High strength is required to hold the weight extra attachments (Not breaking under use). 

3. Design Methodology and Materials 
•	CAD Modeling:
o	Tools used: Tinker CAD.
o	Designed plan: Breaking down the 
 
•	3D Printer Filament Material Selection:
o	PETG is chosen for durability, flexibility, and heat resistance compared to PLA.
o	While PETG is not as durable as TPU it is more ridged and easier to manufacture making it ideal material for mounting hardware.
o	PETG brand used and Specs:
 

4. Fabrication Process
3D Printing Setup
•	Printer Model: Creality Ender V3 SE
 
•	Nozzle Diameter: 0.2 mm
•	Layer Height: 0.2 mm
•	Infill Density: 45% (grid pattern)
•	Print Speed: 45 mm/s (optimized for PETG)
•	Material: PETG filament, selected for its higher impact resistance and thermal stability compared to PLA.
Observed Challenges:
•	PETG stringing due to insufficient retraction settings.
•	Bed adhesion difficulties requiring surface treatment.
•	Warping at bracket edges under prolonged print cycles.
•	Poor interlayer bonding at higher print speeds.
Assembly
•	Servo Mounting: MG995 servos were secured into the printed brackets using mechanical fasteners.
•	Fasteners: 1.4 × 12 mm screws, selected to reduce stress concentration and allow thicker bracket walls compared to larger bolts.
•	Wiring: Standard jumper wires were used for prototyping, enabling rapid connection to the ESP-32 microcontroller.
5. System Integration
The turret employs a dual-axis configuration:
•	Bottom Servo (Yaw): Provides rotation along the X-axis.
•	Top Servo (Pitch): Mounted orthogonally to enable vertical rotation along the Y-axis.
Control Architecture:
•	The ESP-32 generates PWM signals to modulate servo position.
•	Signals are routed through the servo driver circuitry.
•	Mechanical motion is achieved via the coupled servo brackets.

6. Challenges Encountered
•	Bracket Dimensions and Fitment: Initial bracket iterations obstructed servo insertion due to interference with structural features. Other designs were too thin, resulting in cracking under load when using 4 × 12 mm bolts. Transitioning to 1.4 × 12 mm screws allowed thicker bracket walls and improved rigidity.
o	Solution: Increased infill to 100% and introduced pilot holes using a 1/64 in drill bit. Simplified bracket geometry reduced stress concentrations and improved both print quality and mechanical strength.
•	Layer Adhesion in PETG: PETG requires precise tuning to achieve strong interlayer bonding. At default slicer settings (70 mm/s), adhesion was poor, leading to fragile parts.
o	Solution: Research indicated optimal print speeds between 35–50 mm/s. Adjusting to 40 mm/s improved layer fusion and overall part durability without sacrificing print time efficiency.
Minor Printing Issues
•	Bed Adhesion: Improved using glue stick application and elevated bed temperatures.
•	Stringing: Mitigated through retraction tuning and nozzle temperature adjustments.
Mechanical Issues
•	Bracket Snapping: Occurred under torque loads with larger bolts. Switching to smaller screws reduced localized stress and enabled thicker bracket design.
•	Reinforcement Strategies: Implemented 8 perimeter wall loops and increased infill density to enhance load-bearing capacity.
7. Results & Testing
•	Turret Performance:
o	Range of motion achieved.
o	Stability underload.
o	No misalignments and major vibrations.
 
8. Conclusion
	Overall, the task was a success as the Ender V3 SE performed well. Showing that Knowledge about the material and tunning was the most important aspect. As most of the issues occurring were due to improper tunning or a design that was not optimal for the structural limitations of the PETG. 
9. References/Specs 
 
 
