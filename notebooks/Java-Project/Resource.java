package emergency;

import java.io.*;
import java.util.*;

public abstract class Resource {
    private final String id;
    private boolean available;
    private long totalTimeUsed;
    private long currentStartTime;

    public Resource(String id) {
        this.id = id;
        this.available = true;
        this.totalTimeUsed = 0;
        this.currentStartTime = 0;
    }

    public String getId() {
        return id;
    }

    public boolean isAvailable() {
        return available;
    }

    public void startService() {
        if (!available) {
            throw new IllegalStateException("Resource is already in use");
        }
        available = false;
        currentStartTime = System.currentTimeMillis();
    }

    public void endService() {
        if (available) {
            throw new IllegalStateException("Resource is not in use");
        }
        totalTimeUsed += System.currentTimeMillis() - currentStartTime;
        available = true;
    }

    public long getTotalTimeUsed() {
        return totalTimeUsed / 1000; // Convert to seconds
    }

    @Override
    public String toString() {
        return id + ": " + (available ? "Available" : "Busy");
    }
}
