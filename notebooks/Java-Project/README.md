Emergency Department Simulation System

A Java-based simulation of a hospital emergency department that models real-time patient flow and resource management.

## Project Overview
This system simulates an emergency department's operations, focusing on patient prioritization and resource allocation. It helps healthcare management teams evaluate and optimize resource distribution for effective patient care.

## Key Features
- Custom implementation of priority queues for patient management
- Real-time resource allocation system for medical staff and facilities
- Multi-threaded event handling for concurrent patient processing
- Dynamic statistics generation for performance metrics
- Terminal-based real-time visualization of department status

## Technical Implementation
- Custom data structures (no Java collections):
  - Priority Queues for patient management
  - Stacks for resource management (providers, nurses, admin staff)
  - Arrays for room management
- Real-time event scheduling system
- File-based status monitoring and reporting
- Shell script integration for live updates

## Resources Managed
- Healthcare Providers
- Nurses
- Treatment Rooms
- Administrative Staff
- Patient Queues (Multiple Priority Levels)

## System Requirements
- Java JDK 11 or higher
- Unix-like environment (MacOS/Linux) for shell script execution
- Terminal with bash support

## Installation and Usage
1. Clone the repository
2. Compile the Java files: `javac src/emergency/*.java`
3. Run the simulation: `java -cp src emergency.EmergencyDepartmentSimulation`
4. In a separate terminal: `./simulation.sh`

## Simulation Parameters
- Number of providers, nurses, and admin staff
- Service time for each resource type
- Number of treatment rooms
- Priority queue configurations
- Patient arrival probabilities
- Simulation duration and update intervals

## Output
- Real-time department status
- Resource utilization statistics
- Patient wait times
- Queue lengths and processing times
- Detailed performance metrics

## Academic Project Context
Developed as part of CS342 coursework, demonstrating practical application of:
- Data Structures
- Object-Oriented Programming
- Concurrent Programming
- Resource Management Algorithms
- Real-time System Monitoring
