/**
 * Created by andrew_liu on 15/5/4.
 */
public class FlowNetwork {
    private final int V;  //顶点个数
    private Bag<FlowEdge>[] adj;
    public FlowNetwork(int V) {
        this.V = V;
        adj = (Bag<FlowEdge>[]) new Bag[V];
        for (int v = 0; v < V; v++) {
            adj[v] = new Bag<>();
        }
    }
    //想流图中增加边
    public void addEdge(FlowEdge e) {
        int v = e.from();
        int w = e.to();
        adj[v].add(e);  //正向边
        adj[w].add(e);  //反向边
    }
    public int V() {
        return V;
    }
    public Iterable<FlowEdge> adj(int v) {  //返回邻接边
        return adj[v];
    }
}
