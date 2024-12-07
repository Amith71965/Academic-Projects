package emergency;

import java.io.*;
import java.util.*;

public class AdministrativeAssistant extends Resource {
    private final int checkoutTime;

    public AdministrativeAssistant(String id, int checkoutTime) {
        super(id);
        if (checkoutTime <= 0) {
            throw new IllegalArgumentException("Checkout time must be positive");
        }
        this.checkoutTime = checkoutTime;
    }

    public int getCheckoutTime() {
        return checkoutTime;
    }

    @Override
    public String toString() {
        return "AA" + getId() + ": " + (isAvailable() ? "Available" : "Busy");
    }
}