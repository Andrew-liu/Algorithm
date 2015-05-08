import java.math.BigInteger;
import java.util.Random;

/**
 * Created by andrew_liu on 15/5/8.
 */
public class RabinKarp {
    private String pat;
    private long patHash;
    private int M;
    private long Q;
    private int R = 256;
    private long RM;

    public RabinKarp(String pat) {
        this.pat = pat;
        this.M = pat.length();
        Q = longRandomPrime(); //一个大的prime, 避免溢出
        RM = 1;
        for (int i = 1; i <= M - 1; i++)
            RM = (R * RM) % Q; //用于减掉第一个数字的时候计算
        patHash = hash(pat, M);
    }
    private static long longRandomPrime() {  //找一个尽可能大的数作为除留余数法的除数
        BigInteger prime = BigInteger.probablePrime(31, new Random());
        return prime.longValue();
    }
    public boolean check(int i) {
        return true;
    }
    private long hash(String key, int M) { //计算hash值
        long h = 0;
        for (int j = 0; j < M; j++) {
            h = (R * h + key.charAt(j)) % Q;
        }
        return h;
    }
    private int search(String txt) {
        int N = txt.length();
        long txtHash = hash(txt, M);
        if (patHash == txtHash && check(0))
            return 0; //初始就匹配成功
        for (int i = M; i < N; i++) {  //去掉第一个字符, 增加第M个字符(字符串匹配窗口向后移一位)
            txtHash = (txtHash + Q - RM * txt.charAt(i - M) % Q) % Q;
            txtHash = (txtHash * R + txt.charAt(i)) % Q;
            if (patHash == txtHash)
                if (check(i - M + 1))
                    return i - M + 1; //找到匹配
        }
        return N;
   }
}
