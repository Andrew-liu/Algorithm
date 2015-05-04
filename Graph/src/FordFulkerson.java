import com.sun.tools.javac.comp.Flow;

/**
 * Created by andrew_liu on 15/5/4.
 */
public class FordFulkerson {
    private boolean[] marked;  //如果残留网络中有s->v路径, 则为true
    private FlowEdge[] edgeTo;  //s->v路径的最后的边
    private double value; //流

    public FordFulkerson(FlowNetwork G, int s, int t) {
        value = 0.0;
        //当找不到增广路径时终止
        while (hasAugmentingPaht(G, s, t)) {  //判断是否还有增广路径
            double bottle = Double.POSITIVE_INFINITY;
            for (int v = t; v != s; v = edgeTo[v].other(v)) {  //计算最大流量
                bottle = Math.min(bottle, edgeTo[v].residualCapacityTo(v));
            }
            for (int v = t; v != s; v = edgeTo[v].other(v)) {
                edgeTo[v].addResidualFlowTo(v, bottle);
            }
            value += bottle;
        }
    }
    private boolean hasAugmentingPaht(FlowNetwork G, int s, int t) {
        edgeTo = new FlowEdge[G.V()];
        marked = new boolean[G.V()];

        Queue<Integer> q = new Queue<>();
        q.enqueue(s);
        marked[s] = true;
        while (!q.isEmpty()) {
            int v = q.dequeue();
            for (FlowEdge e : G.adj(v)) {
                int w = e.other(v);
                if (e.residualCapacityTo(w) > 0 && !marked[w]) {
                    edgeTo[w] = e;
                    marked[w] = true;
                    q.enqueue(w);
                }
            }
        }
        return marked[t];
    }
    public double value() {
        return value;
    }
    public boolean intCut(int v) { //在残留网络中v->s是否可达
        return marked[v];
    }
}
