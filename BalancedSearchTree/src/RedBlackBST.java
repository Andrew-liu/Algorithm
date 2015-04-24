/**
 * Created by andrew_liu on 15/4/24.
 * 红黑树
 */
public class RedBlackBST<Key extends Comparable<Key>, Value> {
    private Node root;
    private static final boolean RED = true;
    private static final boolean BLACK = false;
    private class  Node{
        Key key;
        Value val;
        Node left, right;
        int N;  //子树节点的个数
        boolean color;  //链路颜色
        public Node(Key key, Value val, int n, boolean color) {
            this.key = key;
            this.val = val;
            this.N = n;
            this.color = color;
        }
    }

    private boolean isRea(Node x) {
        if (x == null) {
            return false;
        }
        return x.color = RED;
    }
    private Node rotateLeft(Node h) {
        Node x = h.right;
        h.right = x.left;
        x.left = h;
        x.color = h.color;
        h.color = RED;
        x.N = h.N;
        h.N = 1 + size(h.left) + size(h.right);
        return x;
    }
    private Node rotateRight(Node h) {
        Node x = h.left;
        x.left = x.right;
        x.right = h;
        x.color = h.color;
        h.color = RED;
        x.N = h.N;
        h.N = 1 + size(h.left) + size(h.right);
        return x;
    }
    private void flipColors(Node h) {
        h.color = RED;
        h.left.color = BLACK;
        h.right.color = BLACK;
    }
    public int size() {
        return size(root);
    }
    private int size(Node x) {
        if (x == null)
            return 0;
        else
            return x.N;
    }
    public void put(Key key, Value val) {
        root = put(root, key, val);
        root.color = RED;
    }
    private Node put(Node h, Key key, Value val) {
        if (h == null)
            return new Node(key, val, 1, RED);
        int cmp = key.compareTo(h.key);
        if (cmp < 0)
            h.left = put(h.left, key, val);
        else if (cmp > 0)
            h.right = put(h.right, key, val);
        else
            h.val = val;

        if (isRea(h.right) && !isRea(h.left))
            h = rotateLeft(h);
        if (isRea(h.left) && isRea(h.left.left))
            h = rotateRight(h);
        if (isRea(h.left) && isRea(h.right))
            flipColors(h);

        h.N = size(h.left) + size(h.right) + 1;
        return h;
    }
}
