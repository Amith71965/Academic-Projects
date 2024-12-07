package emergency;

import java.io.*;
import java.util.*;

public class PatientGenerator {
    private final double[] arrivalProbabilities;
    private int nextPatientId;
    private final Random random;

    public PatientGenerator(double[] arrivalProbabilities) {
        if (!validateProbabilities(arrivalProbabilities)) {
            throw new IllegalArgumentException("Invalid probability distribution");
        }
        this.arrivalProbabilities = arrivalProbabilities.clone();
        this.nextPatientId = 1;
        this.random = new Random();
    }

    private boolean validateProbabilities(double[] probabilities) {
        if (probabilities == null || probabilities.length < 3) {
            return false;
        }

        double sum = 0;
        for (double prob : probabilities) {
            if (prob < 0 || prob > 1) {
                return false;
            }
            sum += prob;
        }

        // Allow for small floating-point differences
        return Math.abs(sum - 1.0) < 0.0001;
    }

    public Patient generatePatient() {
        double rand = random.nextDouble();
        double cumulative = 0;

        for (int priority = 0; priority < arrivalProbabilities.length; priority++) {
            cumulative += arrivalProbabilities[priority];
            if (rand <= cumulative) {
                return new Patient(nextPatientId++, priority);
            }
        }

        // Fallback to lowest priority if we somehow get here
        return new Patient(nextPatientId++, arrivalProbabilities.length - 1);
    }

    public boolean shouldGeneratePatient() {
        return random.nextDouble() < 0.5; // 50% chance of generating a patient each interval
    }
}