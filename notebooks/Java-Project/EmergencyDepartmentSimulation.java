package emergency;

import java.io.*;
import java.util.*;

public class EmergencyDepartmentSimulation {
    public static void main(String[] args) {
        try {
            Scanner scanner = new Scanner(System.in);

            // Get simulation parameters
            System.out.println("Enter simulation parameters:");

            System.out.print("Number of providers: ");
            int numProviders = validatePositiveInt(scanner);

            System.out.print("Provider service time (seconds): ");
            int providerTime = validatePositiveInt(scanner);

            System.out.print("Number of nurses: ");
            int numNurses = validatePositiveInt(scanner);

            System.out.print("Nurse service time (seconds): ");
            int nurseTime = validatePositiveInt(scanner);

            System.out.print("Number of rooms: ");
            int numRooms = validatePositiveInt(scanner);

            System.out.print("Number of administrative assistants: ");
            int numAdmins = validatePositiveInt(scanner);

            System.out.print("Administrative checkout time (seconds): ");
            int adminTime = validatePositiveInt(scanner);

            System.out.print("Number of priority queues (minimum 3): ");
            int numQueues = validateMinimumInt(scanner, 3);

            // Get arrival probabilities
            double[] arrivalProbabilities = new double[numQueues];
            double totalProb = 0;

            System.out.println("\nEnter arrival probabilities for each queue (must sum to 1.0):");
            for (int i = 0; i < numQueues; i++) {
                System.out.printf("Priority %d probability: ", i + 1);
                double prob = validateProbability(scanner);
                totalProb += prob;
                arrivalProbabilities[i] = prob;

                if (i < numQueues - 1 && totalProb >= 1.0) {
                    throw new IllegalArgumentException("Probabilities sum exceeds 1.0");
                }
            }

            if (Math.abs(totalProb - 1.0) > 0.0001) {
                throw new IllegalArgumentException("Probabilities must sum to 1.0");
            }

            System.out.print("Time interval between events (seconds): ");
            int timeInterval = validatePositiveInt(scanner);

            System.out.print("Total simulation time (minutes): ");
            int totalTime = validatePositiveInt(scanner);

            // Create and run simulation
            SimulationController simulator = new SimulationController(
                    numQueues, numProviders, providerTime, numNurses, nurseTime,
                    numRooms, numAdmins, adminTime, arrivalProbabilities
            );

            // Create and start simulation shell script
            createSimulationScript();

            // Run simulation
            Timer timer = new Timer();
            timer.scheduleAtFixedRate(new TimerTask() {
                @Override
                public void run() {
                    try {
                        simulator.runSimulationStep();
                    } catch (IOException e) {
                        System.err.println("Error writing simulation output: " + e.getMessage());
                        timer.cancel();
                    }
                }
            }, 0, timeInterval * 1000L);

            // Schedule simulation end
            new Timer().schedule(new TimerTask() {
                @Override
                public void run() {
                    try {
                        timer.cancel();
                        simulator.writeFinaStatistics();
                        System.out.println("\nSimulation completed. Check simulationOut.txt for results.");
                        System.exit(0);
                    } catch (IOException e) {
                        System.err.println("Error writing final statistics: " + e.getMessage());
                    }
                }
            }, totalTime * 60 * 1000L);

        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            System.exit(1);
        }
    }

    private static int validatePositiveInt(Scanner scanner) {
        int value = scanner.nextInt();
        if (value <= 0) {
            throw new IllegalArgumentException("Value must be positive");
        }
        return value;
    }

    private static int validateMinimumInt(Scanner scanner, int minimum) {
        int value = scanner.nextInt();
        if (value < minimum) {
            throw new IllegalArgumentException("Value must be at least " + minimum);
        }
        return value;
    }

    private static double validateProbability(Scanner scanner) {
        double value = scanner.nextDouble();
        if (value < 0 || value > 1) {
            throw new IllegalArgumentException("Probability must be between 0 and 1");
        }
        return value;
    }

    private static void createSimulationScript() throws IOException {
        String scriptContent = """
            for (( ; ; ))
            do
                clear
                cat simulationOut.txt
                sleep 3
            done
            """;

        File scriptFile = new File("simulation.sh");
        try (FileWriter writer = new FileWriter(scriptFile)) {
            writer.write(scriptContent);
        }

        // Make script executable
        scriptFile.setExecutable(true);
    }
}