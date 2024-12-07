package emergency;

import java.io.*;
import java.util.*;

public class SimulationController {
    private final int numPriorities;
    private final PriorityQueue<Patient> patientQueue;
    private final Queue<Patient> checkoutQueue;
    private final Stack<Provider> providers;
    private final Stack<Nurse> nurses;
    private final Stack<AdministrativeAssistant> adminAssistants;
    private final Room[] rooms;
    private final PatientGenerator patientGenerator;
    private final FileWriter outputWriter;

    private int totalPatientsServed;
    private long totalWaitTime;

    public SimulationController(
            int numPriorities,
            int numProviders,
            int providerTime,
            int numNurses,
            int nurseTime,
            int numRooms,
            int numAdmins,
            int adminTime,
            double[] arrivalProbabilities) throws IOException {

        // Initialize queues and resources
        this.numPriorities = numPriorities;
        this.patientQueue = new PriorityQueue<>(numPriorities);
        this.checkoutQueue = new Queue<>();

        // Initialize stacks
        this.providers = new Stack<>();
        this.nurses = new Stack<>();
        this.adminAssistants = new Stack<>();

        // Initialize providers
        for (int i = numProviders; i > 0; i--) {
            providers.push(new Provider(String.valueOf(i), providerTime));
        }

        // Initialize nurses
        for (int i = numNurses; i > 0; i--) {
            nurses.push(new Nurse(String.valueOf(i), nurseTime));
        }

        // Initialize admin assistants
        for (int i = numAdmins; i > 0; i--) {
            adminAssistants.push(new AdministrativeAssistant(String.valueOf(i), adminTime));
        }

        // Initialize rooms
        this.rooms = new Room[numRooms];
        for (int i = 0; i < numRooms; i++) {
            rooms[i] = new Room(String.valueOf(i + 1));
        }

        this.patientGenerator = new PatientGenerator(arrivalProbabilities);
        this.outputWriter = new FileWriter("simulationOut.txt");
        this.totalPatientsServed = 0;
        this.totalWaitTime = 0;
    }

    public void runSimulationStep() throws IOException {
        // Generate new patients
        if (patientGenerator.shouldGeneratePatient()) {
            Patient newPatient = patientGenerator.generatePatient();
            patientQueue.enqueue(newPatient, newPatient.getPriority());
        }

        // Process nurse assignments
        processNurseAssignments();

        // Process provider visits
        processProviderVisits();

        // Process checkouts
        processCheckouts();

        // Update and write current status
        writeStatus();
    }

    private void processNurseAssignments() {
        while (!nurses.isEmpty() && !patientQueue.isEmpty()) {
            // Find available room
            Room availableRoom = findAvailableRoom();
            if (availableRoom == null) break;

            Nurse nurse = nurses.pop();
            Patient patient = patientQueue.dequeue();

            patient.startNurseAssessment();
            availableRoom.assignPatient(patient, nurse);
            nurse.startService();

            // Schedule nurse completion
            scheduleNurseCompletion(nurse, patient, availableRoom);
        }
    }

    private void processProviderVisits() {
        for (Room room : rooms) {
            if (!room.isAvailable() &&
                    room.getCurrentPatient().getStatus() == PatientStatus.WAITING_IN_ROOM &&
                    !providers.isEmpty()) {

                Provider provider = providers.pop();
                Patient patient = room.getCurrentPatient();

                patient.startProviderAssessment();
                room.assignPatient(patient, provider);
                provider.startService();

                // Schedule provider completion
                scheduleProviderCompletion(provider, patient, room);
            }
        }
    }

    private void processCheckouts() {
        while (!adminAssistants.isEmpty() && !checkoutQueue.isEmpty()) {
            AdministrativeAssistant admin = adminAssistants.pop();
            Patient patient = checkoutQueue.dequeue();

            patient.startCheckout();
            admin.startService();

            // Schedule checkout completion
            scheduleCheckoutCompletion(admin, patient);
        }
    }

    private Room findAvailableRoom() {
        for (Room room : rooms) {
            if (room.isAvailable()) {
                return room;
            }
        }
        return null;
    }

    private void scheduleNurseCompletion(Nurse nurse, Patient patient, Room room) {
        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                patient.finishNurseAssessment();
                nurse.endService();
                nurses.push(nurse);
            }
        }, nurse.getServiceTime() * 1000L);
    }

    private void scheduleProviderCompletion(Provider provider, Patient patient, Room room) {
        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                patient.finishProviderAssessment();
                provider.endService();
                providers.push(provider);
                room.releasePatient();
                checkoutQueue.enqueue(patient);
            }
        }, provider.getServiceTime() * 1000L);
    }

    private void scheduleCheckoutCompletion(AdministrativeAssistant admin, Patient patient) {
        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                patient.completeCheckout();
                admin.endService();
                adminAssistants.push(admin);
                totalPatientsServed++;
                totalWaitTime += patient.getWaitTime();
            }
        }, admin.getCheckoutTime() * 1000L);
    }

    private void writeStatus() throws IOException {
        StringBuilder status = new StringBuilder();

        // Admin Assistants Status
        for (int i = 1; i <= adminAssistants.size(); i++) {
            status.append("AA").append(i).append(": ")
                    .append(adminAssistants.peek().isAvailable() ? "Available" : "Busy")
                    .append("\n");
        }

        // Checkout Queue
        status.append("\nCheck-out Queue:");
        // Add checkout queue patients here
        status.append("\n\n");

        // Rooms Status
        for (Room room : rooms) {
            status.append(room.toString()).append("\n");
        }
        status.append("\n");

        // Providers Status
        for (int i = 1; i <= providers.size(); i++) {
            status.append("D").append(i).append(": ")
                    .append(providers.peek().isAvailable() ? "Available" : "Busy")
                    .append("\n");
        }
        status.append("\n");

        // Nurses Status
        for (int i = 1; i <= nurses.size(); i++) {
            status.append("N").append(i).append(": ")
                    .append(nurses.peek().isAvailable() ? "Available" : "Busy")
                    .append("\n");
        }

        // Priority Queues Status
        for (int i = 0; i < numPriorities; i++) {
            status.append("\nQ").append(i + 1).append(": ");
            // Add queue patients here
        }

        outputWriter.write(status.toString());
        outputWriter.flush();
    }

    public void writeFinaStatistics() throws IOException {
        StringBuilder stats = new StringBuilder("\n\nFinal Statistics:\n");
        stats.append("Total Patients Served: ").append(totalPatientsServed).append("\n");
        stats.append("Average Wait Time: ").append(totalPatientsServed > 0 ?
                totalWaitTime / totalPatientsServed : 0).append(" seconds\n\n");

        // Resource utilization statistics
        stats.append("Total Time Used:\n\n");

        // Admin Assistants
        for (int i = 1; i <= adminAssistants.size(); i++) {
            stats.append("AA").append(i).append(": ")
                    .append(adminAssistants.peek().getTotalTimeUsed())
                    .append(" seconds\n");
        }
        stats.append("\n");

        // Rooms
        for (Room room : rooms) {
            stats.append("Room").append(room.getId()).append(": ")
                    .append(room.getTotalTimeUsed())
                    .append(" seconds\n");
        }
        stats.append("\n");

        // Providers
        for (int i = 1; i <= providers.size(); i++) {
            stats.append("D").append(i).append(": ")
                    .append(providers.peek().getTotalTimeUsed())
                    .append(" seconds\n");
        }
        stats.append("\n");

        // Nurses
        for (int i = 1; i <= nurses.size(); i++) {
            stats.append("N").append(i).append(": ")
                    .append(nurses.peek().getTotalTimeUsed())
                    .append(" seconds\n");
        }

        outputWriter.append(stats.toString());
        outputWriter.flush();
        outputWriter.close();
    }
}