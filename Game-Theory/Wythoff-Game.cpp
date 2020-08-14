/*
    威佐夫博弈
        有两堆各若干个物品，两个人轮流从某一堆或同时从两堆中取同样多的物品，
        规定每次至少取一个，多者不限，最后取光者得胜。
    策略：
        任给一个局势(a,b)，如下公式判断它是不是奇异局势(输)： 
        ak=[k(1+√5)/2]，bk=ak+k(k=0,1,2,…,n方括号表示取整函数）。
*/
bool Wythoff(int n,int m){
    if(n<m) swap(n,m);
    int k=n-m;
    n=(int)(k*(1+sqrt(5))/2.0);
    if(n==m)return 0;
    else return 1;
}
// JAVA
import java.math.BigDecimal;  
import java.util.Scanner;
public class Main{
    public static void main(String args[]){
    	Scanner sc=new Scanner(System.in);
    	BigDecimal two=new BigDecimal(2);
    	BigDecimal three= new BigDecimal(3);
    	BigDecimal five=new BigDecimal(5);
    	BigDecimal l=two,r=three;
    	for(int i=0;i<500;i++){
    		BigDecimal mid=l.add(r).divide(two);
    		if(mid.multiply(mid).compareTo(five)<0)
    			l=mid;
    		else
    			r=mid;	
    	}
    	BigDecimal gold=l.add(BigDecimal.ONE).divide(two);//黄金分割数
    	BigDecimal a,b;
    	while(sc.hasNext()){
    		a=sc.nextBigDecimal();
    		b=sc.nextBigDecimal();
    		if(a.compareTo(b)>0){
    			BigDecimal tmp=a;
    			a=b;b=tmp;
    		}
    		b=b.subtract(a).multiply(gold);
    		b=b.setScale(0,BigDecimal.ROUND_DOWN);
    		if(a.compareTo(b)==0)
    			System.out.println("0");
    		else
    			System.out.println("1");
    	}	
    }
}