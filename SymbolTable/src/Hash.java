import java.util.Date;

/**
 * Created by andrew_liu on 15/4/25.
 * 哈希函数计算
 * 冲突解决策略
 * 实现用户定义的哈希编码 : 是要是使用31x + y原则
 * 1. 如果原始类型 和 引用类型, hashCode()
 * 2. 如果array, 应用Arrays.deepHashCode()
 * 3. 如果是null, 直接返回0
 */

public final class Hash { //implements Comparable<Hash>
    private final String who;
    private final Date when;
    private final double amount;
    public Hash(String who, Date when, double amount) {
        this.who = who;
        this.when = when;
        this.amount = amount;
    }
//    public boolean equals(Object y)
    public int hashCode() {
        int hash = 17;
        //调用三种类型内建的hashCode
        hash = 31 * hash + who.hashCode();
        hash = 31 * hash + when.hashCode();
        hash = 31 * hash + ((Double) amount).hashCode();
        return hash;
    }
}
