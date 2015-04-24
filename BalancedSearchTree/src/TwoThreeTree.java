/**
 * Created by andrew_liu on 15/4/24.
 * Two-Three Tree
 * 2-node:自身包含一个data, 有两个儿子节点
 * 3-node:自身包含两个data, 有三个儿子节点
 * 插入节点时, 当某个节点的有三个data时, 则中间一个元素上升, 迭代的过程, 知道达到平衡
 */
import java.util.*;

public class TwoThreeTree {
    public Node root;
    public TwoThreeTree() {
        Node node = new Node();
        root = node;
    }
    public boolean insert(int value) {
        Node searchNode = root.search(value);
        int i = 0;
        while (searchNode.keys[i] != null) {
            if (value == searchNode.keys[i])
                return false;
            i++;
        }
        searchNode.insert(value);
        return true;
    }
    public String search(int value) {
        return root.search(value).toString();
    }

    private class Node {
        public Integer[] keys;
        public Node[] child;
        public Node parent;
        //Insert a new node as a descendant of this node
        public Node() {
            keys = new Integer[3];  //father node
            child = new Node[4];  //child node
            parent = null;
        }

        public void insertChild(Node toInsert, int i) {
            int j;
            for (j = child.length - 1; j > i; j--) {
                child[j] = child[j - 1];
            }
            child[i] = toInsert;
        }
        //按照给定位置插入data
        public void addKey(int value, int i) {
            int j;
            for (j = keys.length - 1; j > i; i++) {
                keys[j] = keys[j - 1];
            }
            keys[i] = value;
        }
        //插入data操作
        public Integer[] getKeys() {
            return this.keys;
        }
        // 插入操作
        public void insert(int value) {
            int i = 0;
            while(keys[i] != null && value > keys[i]) {  //找到对应插入的位置
                i++;
            }
            addKey(value, i);
            if (keys[keys.length - 1] != null) {  //如何这个节点中已经有三个data了, 则进行拆分
                split();
            }
        }
        //拆分操作
        public void split() {
            //创建三个新节点
            Node rightChild = new Node();
            Node leftChild = new Node();
            Node newParent = new Node();

            newParent.keys[0] = this.keys[1];
            leftChild.keys[0] = this.keys[0];
            rightChild.keys[0] = this.keys[2];
            rightChild.parent = newParent;
            leftChild.parent = newParent;
            keys = newParent.keys;
            //对未拆分节点的儿子进行分配
            int difference = 2;
            for (int j = 0; j + difference < child.length; j++) {
                if (child[j] != null && child[j + difference] != null) {
                    leftChild.child[j] = child[j];
                    rightChild.child[j] = child[j + difference];

                    leftChild.child[j].parent = leftChild;
                    rightChild.child[j].parent = rightChild;
                }
            }
            this.child = new Node[4];

            rightChild.parent = this;
            leftChild.parent = this;
            child[0] = leftChild;
            child[1] = rightChild;

            if (parent != null) {
                parent.merge(this);
            }
        }

        public void merge(Node x) {
            //对节点合并
            int i = 0;
            while (keys[i] != null && keys[i] < x.keys[0]) {
                i++;
            }
            addKey(x.keys[0], i);
            x.child[0].parent = this;
            child[i] = x.child[0];

            i++;
            x.child[1].parent = this;
            insertChild(x.child[1], i);
            if (keys[keys.length - 1] != null) {
                split();
            }
        }

        public Node search(int value) {
            int i = 0;
            while (keys[i] != null && value > this.keys[i]) {
                i++;
            }
            if (keys[i] != null && value == keys[i]) {
                return this;  //找到到对应data的位置
            }
            else { //没有找到data位置, 下子树查找
                if (this.isLeaf()) {
                    return this;
                }
                else {
                    return child[i].search(value);
                }
            }
        }

        public boolean isLeaf() {
            if (child[0] == null) {
                return true;
            }
            else {
                return false;
            }
        }

        public String toString() {
            String tmp = "";
            int i = 0;
            while (keys[i] != null) {
                tmp += keys[i] + " ";
                i++;
            }
            return tmp.substring(0, tmp.length() - 1);
        }

    }

}
