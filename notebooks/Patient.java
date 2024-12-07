package emergency;

import java.io.*;
import java.util.*;

public class Patient {
    private final int id;
    private final int priority;
    private PatientStatus status;
    private final long arrivalTime;
    private long nurseStartTime;
    private long providerStartTime;
    private long checkoutStartTime;
    private long completionTime;
    private long totalWaitTime;

    public Patient(int id, int priority) {
        if (priority < 0) {
            throw new IllegalArgumentException("Priority must be non-negative");
        }
        this.id = id;
        this.priority = priority;
        this.status = PatientStatus.WAITING_FOR_NURSE;
        this.arrivalTime = System.currentTimeMillis();
        this.totalWaitTime = 0;
    }

    // Getters
    public int getId() {
        return id;
    }

    public int getPriority() {
        return priority;
    }

    public PatientStatus getStatus() {
        return status;
    }

    public long getWaitTime() {
        return totalWaitTime / 1000; // Convert to seconds
    }

    // Status transition methods with wait time tracking
    public void startNurseAssessment() {
        if (status != PatientStatus.WAITING_FOR_NURSE) {
            throw new IllegalStateException("Patient must be waiting for nurse");
        }
        totalWaitTime += System.currentTimeMillis() - arrivalTime;
        nurseStartTime = System.currentTimeMillis();
        status = PatientStatus.WITH_NURSE;
    }

    public void finishNurseAssessment() {
        if (status != PatientStatus.WITH_NURSE) {
            throw new IllegalStateException("Patient must be with nurse");
        }
        status = PatientStatus.WAITING_IN_ROOM;
    }

    public void startProviderAssessment() {
        if (status != PatientStatus.WAITING_IN_ROOM) {
            throw new IllegalStateException("Patient must be waiting in room");
        }
        totalWaitTime += System.currentTimeMillis() - nurseStartTime;
        providerStartTime = System.currentTimeMillis();
        status = PatientStatus.WITH_PROVIDER;
    }

    public void finishProviderAssessment() {
        if (status != PatientStatus.WITH_PROVIDER) {
            throw new IllegalStateException("Patient must be with provider");
        }
        status = PatientStatus.WAITING_FOR_CHECKOUT;
    }

    public void startCheckout() {
        if (status != PatientStatus.WAITING_FOR_CHECKOUT) {
            throw new IllegalStateException("Patient must be waiting for checkout");
        }
        totalWaitTime += System.currentTimeMillis() - providerStartTime;
        checkoutStartTime = System.currentTimeMillis();
        status = PatientStatus.WITH_ADMIN;
    }

    public void completeCheckout() {
        if (status != PatientStatus.WITH_ADMIN) {
            throw new IllegalStateException("Patient must be with admin");
        }
        completionTime = System.currentTimeMillis();
        status = PatientStatus.COMPLETED;
    }

    public long getTotalTime() {
        if (status != PatientStatus.COMPLETED) {
            throw new IllegalStateException("Patient has not completed treatment");
        }
        return (completionTime - arrivalTime) / 1000; // Convert to seconds
    }

    @Override
    public String toString() {
        return "P" + id;
    }
}