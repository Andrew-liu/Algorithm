/**
 * Created by andrew_liu on 15/5/3.
 * Prim算法计算最小生成树
 */
public class LazyPrimMST {
    private boolean[] marked;  //最小生成树的顶点
    private Queue<Edge> mst; //最小生成树的边
    private MinPQ<Edge> pq;

    public LazyPrimMST(EdgeWeightedGraph G) {
        pq = new MinPQ<>();
        marked = new boolean[G.V()];
        mst = new Queue<>();
        visit(G, 0);
        while (!pq.isEmpty()) {
            Edge e = pq.delMin(); //权重最小的边
            int v = e.either(), w = e.other(v);
            if (marked[v] && marked[w])  //会形成环的情况
                continue;
            mst.enqueue(e);
            if (!marked[v])
                visit(G, v);
            if (!marked[w])
                visit(G, w);
        }
    }

    private void visit(EdgeWeightedGraph G, int v) {
        marked[v] = true;  //标记为已访问
        for (Edge e : G.adj(v))
            if (!marked[e.other(v)]) //如果v的邻边的点没有被访问, 则这条边入优先入列, 否则会出现成环或者已经入mst的边
                pq.insert(e);
    }
}
