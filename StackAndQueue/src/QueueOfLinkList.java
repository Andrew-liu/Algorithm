
public class QueueOfLinkList {
	//对头出队, 队尾入队
	private Node first, last;  //始终指向对头
	static int size = 0;
	private class Node {
		String item;
		Node next;
	}
	public QueueOfLinkList() {
		// TODO Auto-generated constructor stub
	}
	void enQueue(String item) {
		Node oldLast = last;   //保存过去队尾指针的位置
		Node last = new Node();
		last.item = item;
		last.next = null;
		size++;
		if (isEmpty()) {
			first = last;  //头尾指针指向同一个位置	
		} else {
			oldLast.next = last;
		}
	}
	String deQueue() {
		String item = first.item;
		first = first.next;
		size--;
		if (isEmpty()) {  //最后一个元素的情况
			last = null;
		}
		return item;
	}
	boolean isEmpty() {
//		return size == 0 ? true : false;
		return first == null;
	}
	int size() {
		return size;
	}
}
