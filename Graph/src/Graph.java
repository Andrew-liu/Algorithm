/**
 * Created by andrew_liu on 15/4/27.
 * 邻接表实现图
 */
public class Graph {
    private final int V; //顶点
    private int E; //边
    private Bag<Integer>[] adj; //临街表
    //构造V个顶点无边矩阵
    public Graph(int V) {
        this.V = V;
        this.E = 0;
        adj = (Bag<Integer>[]) new Bag[V];
        for (int v = 0; v < V; v++) {
            adj[v] = new Bag<Integer>();
        }
    }
    //从输入流中读取图的顶点书, 边数, 顶点的关系对
    public Graph(In in) {
        this(in.readInt());
        int E = in.readInt();
        for (int i = 0; i < E; i++) {
            int v = in.readInt();
            int w = in.readInt();
            addEdge(v, w);
        }
    }
    //顶点的数目
    public int V() {
        return V;
    }
    //边的数目
    public int E() {
        return E;
    }
    //添加边
    public void addEdge(int v, int w) {
        adj[v].add(w);
        adj[w].add(w);
        E++;
    }
    //v的临接顶点
    public Iterable<Integer> adj(int v) {
        return adj[v];
    }
}
