
public class QueueOfString {
	private String[] Queue;
	private static int head = 0;
	private static int tail = 0;
	private static int capac;
	public QueueOfString(int capacity) {
		// TODO Auto-generated constructor stub
		Queue = new String[capacity];
		capac = capacity;
	}
	void enQueue(String item) {
		Queue[tail++] = item;
		if (size() == capac) {
			resize(capac * 2); 
		}
	}
	String deQueue() {
		String item = Queue[head++];
		return item;
	}
	boolean isEmpty() {
		return tail == head;
	}
	int size() {
		return tail - head;
	}
	void resize(int capacity) {
		String[] q = new String[capacity];
		for (int i = 0; i < size(); i++) {
			q[i] = Queue[i];
		}
		Queue = q;
	}
}
