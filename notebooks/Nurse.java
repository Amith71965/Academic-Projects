package emergency;

import java.io.*;
import java.util.*;

public class Nurse extends Resource {
    private final int serviceTime;

    public Nurse(String id, int serviceTime) {
        super(id);
        if (serviceTime <= 0) {
            throw new IllegalArgumentException("Service time must be positive");
        }
        this.serviceTime = serviceTime;
    }

    public int getServiceTime() {
        return serviceTime;
    }

    @Override
    public String toString() {
        return "N" + getId() + ": " + (isAvailable() ? "Available" : "Busy");
    }
}