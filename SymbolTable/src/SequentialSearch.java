/**
 * Created by andrew_liu on 15/4/21.
 * 无序链表实现顺序查找
 */
public class SequentialSearch<Key, Value> {
    private Node first;
    private class Node {
        Key key;
        Value value;
        Node next;
        public Node(Key key, Value value, Node next) {
            this.key = key;
            this.value = value;
            this.next = next;
        }
    }
    //通过key查找元素
    public Value get(Key key) {
        Node x = first;
        for (; x != null; x = x.next) {
            if (key.equals(x.key))
                return x.value;
        }
        return null;
    }
    public void put(Key key, Value value) {
        for (Node x = first; x != null; x = x.next) {
            if (key.equals(x.key)) {
                x.value = value;  //查找成功覆盖原来元素
                return;
            }
        }
        first = new Node(key, value, first); //插入新元素
    }

    public static void main(String[] args) {
        SequentialSearch<String, Integer> st = new SequentialSearch<String, Integer>();
        for (int i = 0; !StdIn.isEmpty(); i++) {
            String key = StdIn.readString();
            st.put(key, i);
        }
//        for (String s : st.keys()) {
//            StdOut.println(s + " " + st.get(s));
//        }
    }
}
