package emergency;

import java.io.*;
import java.util.*;

public class PriorityQueue<T> {
    private Node<T>[] queues;
    private final int numPriorities;
    private int size;

    @SuppressWarnings("unchecked")
    public PriorityQueue(int numPriorities) {
        if (numPriorities < 1) {
            throw new IllegalArgumentException("Number of priorities must be positive");
        }
        this.numPriorities = numPriorities;
        this.queues = new Node[numPriorities];
        this.size = 0;
    }

    public void enqueue(T data, int priority) {
        if (priority < 0 || priority >= numPriorities) {
            throw new IllegalArgumentException("Invalid priority level");
        }

        Node<T> newNode = new Node<>(data);
        if (queues[priority] == null) {
            queues[priority] = newNode;
        } else {
            Node<T> current = queues[priority];
            while (current.getNext() != null) {
                current = current.getNext();
            }
            current.setNext(newNode);
        }
        size++;
    }

    public T dequeue() {
        if (isEmpty()) {
            throw new IllegalStateException("Priority queue is empty");
        }

        // Start from highest priority (0) to lowest
        for (int i = 0; i < numPriorities; i++) {
            if (queues[i] != null) {
                T data = queues[i].getData();
                queues[i] = queues[i].getNext();
                size--;
                return data;
            }
        }

        return null; // Should never reach here if size is maintained correctly
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return size;
    }

    public boolean isEmpty(int priority) {
        if (priority < 0 || priority >= numPriorities) {
            throw new IllegalArgumentException("Invalid priority level");
        }
        return queues[priority] == null;
    }

    public void clear() {
        for (int i = 0; i < numPriorities; i++) {
            queues[i] = null;
        }
        size = 0;
    }

    public int getNumPriorities() {
        return numPriorities;
    }
}