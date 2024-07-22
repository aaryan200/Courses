public class HWQueue<T> {
    AtomicReference<T>[] items;
    AtomicInteger tail;
    static final int CAPACITY = Integer.MAX_VALUE;
    public HWQueue() {
        items =(AtomicReference<T>[])Array.newInstance(AtomicReference.class,
        CAPACITY);
        for (int i = 0; i < items.length; i++) {
            items[i] = new AtomicReference<T>(null);
        }
        tail = new AtomicInteger(0);
    }
    public void enq(T x) {
        int i = tail.getAndIncrement();
        items[i].set(x);
    }
    public T deq() {
        while (true) {
            int range = tail.get();
            for (int i = 0; i < range; i++) {
                T value = items[i].getAndSet(null);
                if (value != null) {
                    return value;
                }
            }
        }
    }
}

class LockBasedQueue<T> {
    int head, tail;
    T[] items;
    Lock lock;
    public LockBasedQueue(int capacity) {
        head = 0; tail = 0;
        lock = new ReentrantLock();
        items = (T[])new Object[capacity];
    }
    public void enq(T x) throws FullException {
        lock.lock();
        try {
            if (tail - head == items.length) throw new FullException();
            items[tail % items.length] = x;
            tail++;
        } finally {
            lock.unlock();
        }
    }
    public T deq() throws EmptyException {
        lock.lock();
        try {
            if (tail == head) throw new EmptyException();
            T x = items[head % items.length];
            head++;
            return x;
        } finally {
        lock.unlock();
        }
    }
}