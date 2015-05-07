import java.util.Objects;

/**
 * Created by andrew_liu on 15/5/6.
 * 单词查找树的符号表
 *
 * 应用:
 * 拼写检查
 */
public class TrieST<Value> {
    private static int R = 256; //基数
    private Node root;  //单词查找树的根节点
    private static class Node {
        private Objects val;
        private Node[] next = new Node[R]; //保存指向其他Node的引用
    }
    public Value get(String key) {
        Node x = get(root, key, 0);
        if (x == null)
            return null;
        return (Value) x.val;
    }
    private Node get(Node x, String key, int d) {
        //返回x作为根节点的子单词查找树中与key相关的值
        if (x == null)
            return null;
        if (d == key.length())
            return x;
        char c = key.charAt(d);  //查找第d个字符对应的单词查找树
        return get(x.next[c], key, d + 1);
    }
    public void put(String key, Value val) {
        root = put(root, key, val, 0);
    }
    private Node put(Node x, String key, Value val, int d) {
        //如果key存在与以x为根节点的子单词查找树中则更新与它相关值
        if (x == null)
            x = new Node();
        if (d == key.length()) {
            x.val = (Objects) val;
            return x;
        }
        char c = key.charAt(d); //找到第d个字符对应的单词查找树
        x.next[c] = put(x.next[c], key, val, d + 1);
        return x;
    }
}
