# Pell方程
# T组数据，给出n，求x*x-n*y*y=1的最小正整数解集(x,y)
T=int(raw_input())
while T>0:
    n=int(raw_input())
    j=1
    while j*j<n:
        j+=1
    if j*j==n:
        print j,1
    if j*j>n:
        p=[0 for i in range(0,1001)]
        q=[0 for i in range(0,1001)]
        a=[0 for i in range(0,1001)]
        g=[0 for i in range(0,1001)]
        h=[0 for i in range(0,1001)]
        p[1]=q[0]=h[1]=1
        p[0]=q[1]=g[1]=0
        a[2]=j-1
        i=2
        while 1:
            g[i]=-g[i-1]+a[i]*h[i-1]
            h[i]=(n-g[i]*g[i])/h[i-1]
            a[i+1]=(g[i]+a[2])/h[i]
            p[i]=a[i]*p[i-1]+p[i-2]
            q[i]=a[i]*q[i-1]+q[i-2]
            if(p[i]*p[i]-n*q[i]*q[i]==1):
                print p[i],q[i]
                break
            i+=1
    T-=1
