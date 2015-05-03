import java.util.Iterator;

/**
 * Created by andrew_liu on 15/5/3.
 * 每次取权重最小的边, 判断是否成环, 如果没有加入mst, 否则删除
 */
public class kruskalMST {
    private Queue<Edge> mst = new Queue<Edge>();
    public kruskalMST(EdgeWeightedGraph G) {
        MinPQ<Edge> pq = new MinPQ<Edge>(); //优先队列
        for (Edge e : G.edges())
            pq.insert(e);
        UF uf = new UF(G.V());
        while (!pq.isEmpty()  && mst.size() < G.V() - 1) {
            Edge e = pq.delMin();
            int v = e.either(), w = e.other(v);
            if (!uf.connected(v, w)) {
                uf.union(v, w);
                mst.enqueue(e);
            }
        }
    }
    public Iterable<Edge> edges() {
        return mst;
    }
}
