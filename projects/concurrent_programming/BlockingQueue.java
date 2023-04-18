package przyklady06;

import java.util.LinkedList;

public class BlockingQueue<T> {
    private int capacity;
    private int size;

    public BlockingQueue(int capacity) {
        this.capacity = capacity;
        this.size = 0;
    }

    private LinkedList<T> queue = new LinkedList<T>();

    public synchronized T take() throws InterruptedException {
        while(size == 0) {
            wait();
        }
        T el = queue.getFirst();
        queue.removeFirst();
        size--;
        notify();
        return el;
    }

    public synchronized void put(T item) throws InterruptedException {
        while(size == capacity) {
            wait();
        }
        queue.addLast(item);
        size++;
        notify();
    }

    public synchronized int getSize() {
        return size;
    }

    public int getCapacity() {
        return capacity;
    }
}
