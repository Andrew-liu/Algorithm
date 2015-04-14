import java.awt.event.ItemEvent;

public class StackOfString {
    private String[] stack;
    private static int size = 0;
    private int capacity;
    StackOfString(int len) {
        stack = new String[len];
        capacity = len;
    }
    void push(String item) {
    	if (size == capacity){  //扩大数组容量
    		resize(2 * capacity);
    	}
        stack[size++] = item;
    }
    String pop() {
        if (this.isEmpty()) {
            throw new RuntimeException("空栈异常");
        }
        String item = stack[--size];
        if (size > 0 && size == stack.length / 4) {
			resize(stack.length / 2);  // 收缩栈大小
		}
        return item;
    }
    boolean isEmpty() {
        return size == 0 ? true : false;
    }
    int size() {
        return size;
    }
    void resize(int capacity) {  //将所有的元素复制到新的数组
		String[] copy = new String[capacity];
		StdOut.print(stack.length);
		for (int i = 0; i < stack.length; i++) {
			copy[i] = stack[i];
		}
		stack = copy;
	}
    public static void main(String[] args) {
    	int len = 10;
        StackOfString stack = new StackOfString(len);
        StdOut.print("Please input some words!!!\n");
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