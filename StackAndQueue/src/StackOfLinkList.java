
public class StackOfLinkList {
	private Node first = null;  //栈顶指针
	private static int size = 0;
	private class Node {
		String item;
		Node next;
	}
	public boolean isEmpty() {
		return first == null;
	}
	public void push(String item) {
		Node oldNode = first;
		first = new Node();  //first始终作为栈顶指针
		first.item = item;
		first.next = oldNode; //链接到原来的stack中
	}
	public String pop() {
		if(first == null) {
			throw new RuntimeException("空栈异常");
		}
		String item = first.item;
		first = first.next;
		return item;
	}
	public static void main(String[] args) {
		StdOut.print("Please input some words:\n");
		StackOfLinkList stack = new StackOfLinkList();
		while(!StdIn.isEmpty()) {
            String s = StdIn.readString();
            if (s.equals("-")) {
                StdOut.print(stack.pop());
            }
            else {
                stack.push(s);
            }
        }
	}
}
