/**
 * Created by andrew_liu on 15/5/4.
 */
public class FlowEdge {
    private final int v, w;  //边的起点和终点
    private final double capacity;  //流量
    private double flow;   //流
    public FlowEdge(int v, int w, double capacity) {
        this.v = v;
        this.w = w;
        this.capacity = capacity;
    }
    public int from() {
        return v;
    }
    public int to() {
        return w;
    }
    public double capacity() {
        return capacity;
    }
    public double flow() {
        return flow;
    }
    public int other(int vertex) {
        if (vertex == v) {
            return w;
        } else if (vertex == w) {
            return v;
        } else {
            throw new RuntimeException("Inconsistent edge");
        }
    }
    public double residualCapacityTo(int vertex) {
        if (vertex == v) {  //反向边
            return flow;
        } else if (vertex == w) {  //正向边
            return capacity - flow;
        } else {
            throw new IllegalArgumentException();
        }
    }
    public void addResidualFlowTo(int vertex, double delta) {
        if (vertex == v) {
            flow -= delta;
        } else if (vertex == w) {
            flow += delta;
        } else {
            throw new IllegalArgumentException();
        }
    }
}
