import java.util.Iterator;
// Don't use a library until you understand its API
public class Stack<Item> implements Iterable<Item>{
	/*
	 * Iterable 返回一个Iterator
	 * public interface Iterable<Item> {
	 *     Iterator<Item> iterator();
	 * }
	 * public interface Iterator<Item> {
	 *     boolean hasNext();
	 *     Item next();
	 * }
	 * 有方法hasNext(), next()
	 */
	public Iterator<Item> iterator() {
		return new ListIterator();  //创建一个class
	}
	public class ListIterator implements Iterator<Item> {
		private Node current = first;
		public boolean hasNext() {
			return current != null;
		}
		public void remove() { /* */ }
		public Item next() {
			Item item = current.item;
			current = current.next;
			return item;
		}
	}
	// 泛型
	private Node first = null;
	private class Node {
		Item item;
		Node next;
	}
	public boolean isEmpty() {
		return first == null;
	}
	public void push(Item item) {
		Node oldFirst = first;
		first = new Node();
		first.item = item;
		first.next = oldFirst;
	}
	public Item pop() {
		Item item = first.item;
		first = first.next;
		return item;
	}
}
/*
public class FixedCapacityStack<Item> {
	private Item[] s;
	private int N = 0;
	public Iterator<Item> iterator() {
		return new ArrayIterator();  //创建一个class
	}
	public class ArrayIterator implements Iterator<Item> {
		private int i = N;
		public boolean hasNext() {
			return i > 0;
		}
		public void remove() {  }
		public Item next() {
			return s[--i];
		}
	}
	public FixedCapacityStack(int capacity) {
		//s = new Item[capacity];  //不允许
		s = (Item[]) new Object[capacity];
	}
	public boolean isEmpty() {
		return N == 0;
	}
	public void push(Item item) {
		s[N++] = item;
	}
	public Item pop() {
		return s[--N];
	}
}
*/