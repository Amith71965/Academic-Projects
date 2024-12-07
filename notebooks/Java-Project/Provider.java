package emergency;

import java.io.*;
import java.util.*;

public class Provider extends Resource {
    private final int serviceTime;

    public Provider(String id, int serviceTime) {
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
        return "D" + getId() + ": " + (isAvailable() ? "Available" : "Busy");
    }
}