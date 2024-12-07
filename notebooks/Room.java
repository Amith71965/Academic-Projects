package emergency;

import java.io.*;
import java.util.*;

public class Room {
    private final String id;
    private Patient currentPatient;
    private Resource assignedResource; // Can be either nurse or provider
    private long totalTimeUsed;
    private long currentStartTime;

    public Room(String id) {
        this.id = id;
        this.currentPatient = null;
        this.assignedResource = null;
        this.totalTimeUsed = 0;
        this.currentStartTime = 0;
    }

    public String getId() {
        return id;
    }

    public boolean isAvailable() {
        return currentPatient == null;
    }

    public Patient getCurrentPatient() {
        return currentPatient;
    }

    public Resource getAssignedResource() {
        return assignedResource;
    }

    public void assignPatient(Patient patient, Resource resource) {
        if (!isAvailable()) {
            throw new IllegalStateException("Room is already occupied");
        }
        this.currentPatient = patient;
        this.assignedResource = resource;
        this.currentStartTime = System.currentTimeMillis();
    }

    public void releasePatient() {
        if (isAvailable()) {
            throw new IllegalStateException("Room is already empty");
        }
        totalTimeUsed += System.currentTimeMillis() - currentStartTime;
        this.currentPatient = null;
        this.assignedResource = null;
    }

    public long getTotalTimeUsed() {
        return totalTimeUsed / 1000; // Convert to seconds
    }

    @Override
    public String toString() {
        if (isAvailable()) {
            return "Room" + id + ": Empty";
        }
        String resourceId = assignedResource != null ? " " + assignedResource.getId() : "";
        return "Room" + id + ": P" + currentPatient.getId() + resourceId;
    }
}